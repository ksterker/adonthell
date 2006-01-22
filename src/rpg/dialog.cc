/*
 $Id: dialog.cc,v 1.1 2006/01/22 21:32:39 ksterker Exp $
 
 Copyright (C) 2006 Kai Sterker <kaisterker@linuxgames.com>
 Part of the Adonthell Project http://adonthell.linuxgames.com
 
 Adonthell is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 Adonthell is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Adonthell; if not, write to the Free Software 
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file   rpg/dialog.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Core of the dialogue system.
 */

#include "base/nls.h"
#include "rpg/dialog.h"

using rpg::dialog_line;
using rpg::dialog;
using base::nls;

// ctor
dialog::dialog (rpg::character & npc) : python::script ()
{
    Current = NULL;
    
    // get dialogue to load
    std::string dlg = npc.dialogue ();
    
    // create arguments for Python dialogue class ctor
    Args = PyTuple_New (2);
    PyTuple_SET_ITEM (Args, 0, python::pass_instance (rpg::character::get_player()));
    PyTuple_SET_ITEM (Args, 1, python::pass_instance (&npc));
    
    // instanciate Python dialogue class
    if (!create_instance (DIALOGUE_PACKAGE + dlg, dlg, Args))
    {
        fprintf (stderr, "*** dialog: loading of dialogue '%s' failed!\n", dlg.c_str());
        return;
    }
    
    // read data from Python dialogue class
    if (!init ())
    {
        fprintf (stderr, "*** dialog: initialization of dialogue '%s' failed!\n", dlg.c_str());
        return;
    }
}

// dtor
dialog::~dialog ()
{
    Py_DECREF (Args);
    cleanup ();
}

// tidy up
void dialog::cleanup ()
{
    delete history ();
    delete[] Lines;
    
    Loop.clear ();
    Successors.clear ();
    Current = NULL;
}

// reload Python dialogue script after it changed on disk
bool dialog::reload ()
{
    // tidy up
    cleanup ();
    
    // load script again
    if (!reload_instance (file_name(), class_name(), Args))
    {
        fprintf (stderr, "*** dialog::reload: instanciation of dialogue '%s' failed!\n", class_name().c_str());
        return false;
    }    
    
    // read data from Python dialogue class
    if (!init ())
    {
        fprintf (stderr, "*** dialog::reload: initialization of dialogue '%s' failed!\n", class_name().c_str());
        return false;
    }
    
    return true;
}

// misc. initialisation
bool dialog::init ()
{
    PyObject *list, *s;
    u_int32 i, size;
    
    // get lines of text allowed to loop
    list = get_attribute ("loop");
    if (list && PyList_Check (list))
    {
        size = PyList_Size (list);
        
        for (i = 0; i < size; i++)
        {
            s = PyList_GetItem (list, i);
            if (s && PyInt_Check (s)) Loop.push_back (PyInt_AsLong (s));
        }
        
        Py_DECREF (list);
    }
    
    // extract the dialogue's strings
    list = get_attribute ("text");
    if (list && PyList_Check (list))
    {
        size = PyList_Size (list);
        Lines = new char*[size];
        
        for (i = 1; i < size; i++)
        {
            s = PyList_GetItem (list, i);
            if (s && PyString_Check (s)) Lines[i] = PyString_AsString (s);
            else
            {
                fprintf (stderr, "*** dialog::setup: error loading string at index %i!\n", i);
                Lines[i] = "*** Error ***";
            }
        }
        
        Py_DECREF (list);
    }
    else
    {
        fprintf (stderr, "*** dialog::setup: dialogue contains no text!\n");
        return false;
    }
    
    // init the first answer
    Successors.push_back (0);
    
    return true;
}

// run one iteration of the dialogue
const dialog_line *dialog::run (const s_int32 & answer)
{
    // make sure the dialogue is properly initialized
    if (Instance != NULL)
    {
        PyObject *speakers, *speech, *arg, *result;
        bool stop = false;
        dialog_line *next;
        u_int32 size;
        
        // sanity check
        if (answer < -1 || answer + 1 >= (int) Successors.size())
        {
            fprintf (stderr, "dialog::run: answer %i out of bounds (-1, %i)\n", answer, (int)Successors.size()-1);
            return NULL;
        }
        
        // get successor of the previous line of dialogue
        s_int32 successor = Successors[answer + 1];
        
        // end dialogue if successor == -1
        if (successor == -1) return NULL;
        
        // these will be computed freshly for this iteration
        Successors.clear ();
        
        // if player selected answer, remember which one it was for history
        if (Current != NULL && answer >= 0)
        {
            Current->SelectedAnswer = answer;
        }
        
        // mark player text as used unless loops allowed
        if (find (Loop.begin (), Loop.end (), successor) == Loop.end ())
            Used.push_back (successor);
        
        do
        {
            // execute the next part of the dialogue
            arg = Py_BuildValue ("(i)", successor);
            call_method ("run", arg);
            Py_XDECREF (arg);
            
            // get list of speakers and what each of them said
            speakers = get_attribute ("speaker");
            speech = get_attribute ("speech");
            
            // filter lines of speech already used
            for (s_int32 i = 0; i < PyList_Size (speech); i++)
            {
                s_int32 s = PyInt_AsLong (PyList_GetItem (speech, i));
                
                // remove text that was already used and isn't allowed to loop
                if (find (Used.begin (), Used.end (), s) != Used.end ())
                {
                    PySequence_DelItem (speakers, i);
                    PySequence_DelItem (speech, i--);
                }
            }
            
            // check if some text is left at all
            size = PyList_Size (speech);
            if (size == 0) return NULL;
            
            // check type of speaker
            if (PyList_GetItem (speakers, 0) != Py_None)
            {
                // npc or narrator text found, so randomly chose a line
                s_int32 rnd = rand() % size;
                
                // get text and speaker
                successor = PyInt_AsLong (PyList_GetItem (speech, rnd));
                std::string text = scan_string (nls::translate (Lines[successor]));
                std::string speaker = PyString_AsString (PyList_GetItem (speakers, rnd));

                // create next line of dialog
                next = new dialog_line (speaker, text, Current);
                
                // check whether any player text follows
                arg = Py_BuildValue ("(i)", successor);
                result = call_method_ret ("stop", arg);
                stop = PyInt_AsLong (result) != 0;
                Py_XDECREF (result);
                Py_XDECREF (arg);
                
                // mark the NPC text as used unless loops allowed
                if (find (Loop.begin (), Loop.end (), successor) == Loop.end ())
                    Used.push_back (successor);
                
                // remember successor to this line of text
                Successors.push_back (successor);
            }
            else
            {
                // got player text, so let the player decide
                for (u_int32 i = 0; i < size; i++)
                {
                    // simply add all text to let the player select an answer
                    successor = PyInt_AsLong (PyList_GetItem (speech, i));
                    next->Answers.push_back (scan_string (nls::translate (Lines[successor])));
                    Successors.push_back (successor);
                }
                
                // let the player make his decision
                stop = true;
            }
            
            // cleanup
            Py_XDECREF (speakers);
            Py_XDECREF (speech);
        }
        while (!stop);
        
        // update current line of dialogue
        Current = next;
    }
    else
    {
        fprintf (stderr, "*** dialog::run: dialogue not properly initialized!\n");
    }
    
    return Current;
}

// execute embedded functions and replace shortcuts
// yeah, the c string library hurts, but at least it's fast ;)
std::string dialog::scan_string (const char *s)
{
    rpg::character *the_player = rpg::character::get_player();
    char *tmp, *start, *mid, *str = NULL;
    u_int32 begin, end, len;
    std::string newstr (s); 
    PyObject *result;
    
    // replace $... shortcuts
    while (1)
    {
        // check wether the string contains shortcut code at all
        start = strchr (newstr.c_str (), '$');
        if (start == NULL) break;
        
        // replace "$name"
        if (strncmp (start, "$name", 5) == 0)
        {
            begin = newstr.length () - strlen (start);
            string t (newstr, 0, begin);
            t += the_player->name ();
            t += (start+5);
            
            newstr = t;
            continue;
        }
        
        // replace "$fm"
        if (strncmp (start, "$fm", 3) == 0)
        {
            // extract the "$fm{.../...} part
            end = strcspn (start, "}");
            str = new char[end];
            str[end-1] = 0;        
            strncpy (str, start+3, end);
            
            if (the_player->get_attribute_int ("Gender") == FEMALE)
                mid = get_substr (str, "{", "/");
            else
                mid = get_substr (str, "/", "}");
            
            begin = newstr.length () - strlen(start);
            tmp = new char[newstr.length () - end + strlen (mid)];
            strncpy (tmp, newstr.c_str (), begin);
            tmp[begin] = 0;
            strcat (tmp, mid);
            strcat (tmp, start+end+1);
            
            delete[] str;
            delete[] mid;
            newstr = tmp;
            
            continue;
        }
        
        // Error!
        fprintf (stderr, "*** dialog::scan_string: unknown macro '%s' found!\n",  start);
        start[0] = ' ';
    }
    
    // execute python functions
    while (1)
    {
        // check wether the string contains python code at all
        start = strchr (newstr.c_str (), '{');
        if (start == NULL) break;
        
        end = strcspn (start, "}");
        mid = NULL;
        
        str = new char[end];
        str[end-1] = 0;        
        
        // extract the code without the brackets
        strncpy (str, start+1, end-1);
        
        // run the string
        result = PyObject_CallMethod (get_instance (false), str, NULL);
        
        if (result)
            if (PyString_Check (result))
                mid = (char*) nls::translate (PyString_AS_STRING (result));    
        
        // Replace existing with new, changed string
        // 1. Calculate string's length
        len = newstr.length ();
        begin = len - strlen (start);
        tmp = new char[(mid ? strlen(mid) : 0) + len - strlen(str)];
        
        // 2. Merge prefix, resulting string and postfix into new string
        strncpy (tmp, newstr.c_str (), begin);
        tmp[begin] = 0;
        if (mid) strcat (tmp, mid);
        strcat (tmp, start+end+1);
        
        // 3. Exchange strings
        newstr = tmp;
        
        // Cleanup
        Py_XDECREF (result);
        delete[] str;
        delete[] tmp;
    }
    
    return newstr;
}

char *dialog::get_substr (const char* string, char* begin, char* end)
{
    u_int32 b, e;
    b = strcspn (string, begin) + 1;
    e = strcspn (string, end) - b;
    
    char *result = new char[e+1];
    strncpy (result, string+b, e);
    result[e] = 0;
    
    return result;
}
