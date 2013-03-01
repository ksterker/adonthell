/*
   $Id: quest.cc,v 1.14 2009/04/08 19:36:02 ksterker Exp $
   
   Copyright (C) 2004/2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   rpg/quest.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Keep track of progress in the game.
 */
 
#include <adonthell/python/python.h>
#include "quest.h"
#include <adonthell/event/manager.h>
#include "quest_event.h"
#include <adonthell/base/base.h>
#include <adonthell/base/diskio.h>

using rpg::quest;
using rpg::quest_part;
using rpg::quest_event;

// ctor
quest_part::quest_part (const std::string & id, quest_part *parent)
{
    Id = id;
    Code = "";
    Parent = parent;
    Started = false;
    Completed = false;
    EntryOnStart = NULL;
	EntryOnCompl = NULL;
	
	if (Parent != NULL)
		Parent->add_child (this);
}

// dtor
quest_part::~quest_part ()
{
    std::map<std::string, quest_part*>::iterator q;
    for (q = Children.begin (); q != Children.end (); q++)
        delete (*q).second;
		
	delete EntryOnStart;
	delete EntryOnCompl;
}

// set a step to completed
bool quest_part::set_completed ()
{
    // only steps can be completed that way
    if (!Children.empty ()) return false;
    
	// set state to started and completed
    update ();
    
    return true;
}

// return child quest part
const quest_part *quest_part::child (const std::string &id) const
{
    std::map<std::string, quest_part*>::const_iterator q;
    
    if ((q = Children.find (id)) != Children.end ())
        return (*q).second;
    else
        fprintf (stderr, "*** quest_part::child: '%s' is not part of quest '%s'!\n", id.c_str (), Id.c_str ());

    return NULL;
}

// add a quest to list of quests
void quest_part::add_child (quest_part *part)
{
    std::map<std::string, quest_part*>::iterator q;
    if ((q = Children.find (part->id ())) != Children.end ())
    {
        delete (*q).second;
        (*q).second = part;
    }
    else Children[part->id ()] = part;
}


// update state of a quest part from state of its children
void quest_part::update ()
{
    bool changed = false;
    
    // if update is called, it means that a child quest part has been completed
    // if the quest has been started before, we do not bother
    if (Started == false)
    {
        Started = true;
        changed = true;
    }
    
    // if the quest part is already completed, we also do not bother
    if (Completed == false)
    {
        Completed = true;
        
        if (Code == "")
        {
            // default completion rule --> check that all children are completed
            std::map<std::string, quest_part*>::const_iterator i;
            for (i = Children.begin (); i != Children.end (); i++)
            {
                Completed = Completed & (*i).second->is_completed ();
                if (Completed == false) break;
            }
        }
        else
        {
            Completed = evaluate ();
        }
            
        // has completion state changed?
        if (Completed == true) changed = true;
    }
    
	if (changed)
	{
		// fire quest event
		quest_event evt (full_name(), this);
		events::manager::raise_event (&evt);
		
		// if state changed, update parent too
		if (Parent != NULL) Parent->update ();
	}
}

// calculate the state of completion with given code
bool quest_part::evaluate ()
{
    std::string code = "", token = "";
    std::map<std::string, quest_part*>::iterator child;

    // prepare code for evaluation, replacing all references to quest parts with
    // their state of completion
    for (const char *cptr = Code.c_str(); *cptr != '\0'; cptr++)
    {
        if (!isalnum (*cptr) && *cptr != '_')
        {
            // check if we have just read a new token
            if (token != "")
            {
                // check if the token we read is a quest_part
                if ((child = Children.find (token)) != Children.end ())
                    // if so, add its state of completion to the code
                    code += (*child).second->is_completed () ? '1' : '0';
                else
                    // otherwise assume the token is part of the code
                    code += token;
                
                // reset token
                token = "";
            }
            code += *cptr;
        }
        else token += *cptr;
    }
    
    // now try to evaluate our code snippet
    PyObject *result = python::run_string (code);
    if (result == NULL || !PyInt_Check (result))
    {
        fprintf (stderr, "*** quest_part::evaluate: Error in quest completion calculation rule:\n'%s'\n", 
            code.c_str ());
        Py_XDECREF (result);
        return false;
    }
    
    // retrieve the result
    bool completed = PyInt_AS_LONG (result);
    Py_DECREF (result);
    return completed; 
}


// get full name of this quest part
std::string quest_part::full_name () const
{
	if (Parent != NULL) return Parent->full_name() + std::string(".") + Id;
	return Id;
}

// save quest_part
void quest_part::put_state (base::flat & file) const
{
    base::flat record;
    
	// id needs be saved here only for quest root
	if (Parent == NULL) record.put_string ("qid", Id);

    // save attributes
    record.put_bool ("qst", Started);
    record.put_bool ("qcp", Completed);
    record.put_string ("qcd", Code);
    
    // save associated log entries
    record.put_bool ("qse", EntryOnStart != NULL);
    if (EntryOnStart) EntryOnStart->put_state (record);
    record.put_bool ("qce", EntryOnCompl != NULL);
    if (EntryOnCompl) EntryOnCompl->put_state (record);

    // save child quest parts
    std::map<std::string, quest_part*>::const_iterator i;
    record.put_uint16 ("qnc", Children.size ());
    for (i = Children.begin (); i != Children.end (); i++)
	{
	    record.put_string ("qid", (*i).second->id ());
        (*i).second->put_state (record);
	}
	
    // save this quest part to stream
    file.put_flat ("q", record);
}

// load quest part
bool quest_part::get_state (base::flat & file)
{
    base::flat record = file.get_flat ("q");
    if (!file.success ()) return false;

	// id needs to be loaded only for quest root 
	if (Parent == NULL) Id = record.get_string ("qid");

    // get attributes
    Started = record.get_bool ("qst");
    Completed = record.get_bool ("qcp");
    Code = record.get_string ("qcd");

    // get log entry for quest start, if any
    if (record.get_bool ("qse") == true)
    {
        EntryOnStart = new rpg::log_entry ("", "", "");
        EntryOnStart->get_state (record);
    }

    // get log entries for quest completion, if any
    if (record.get_bool ("qce") == true)
    {
        EntryOnCompl = new rpg::log_entry ("", "", "");
        EntryOnCompl->get_state (record);
    }

    // get children, if any
    quest_part *part;
    for (u_int16 i = record.get_uint16 ("qnc"); i > 0; i--)
    {
		string id = record.get_string ("qid");

        part = new quest_part (id, this);
        part->get_state (record);
    }
    
    return record.success ();
}

// storage for the different quests
std::map<std::string, quest_part*> quest::Quests;

// delete all quests
void quest::cleanup ()
{
    std::map<std::string, quest_part*>::iterator q;
    
    // delete quest parts
    for (q = Quests.begin (); q != Quests.end (); q++)
        delete (*q).second;
    
    // empty quest array
    Quests.clear();
}

// check whether a given quest (step) has been started
bool quest::is_started (const std::string & id)
{
    const quest_part *part = get_part (id);
    if (part != NULL) return part->is_started ();
    return false;
}

// check whether a given quest (step) has been completed
bool quest::is_completed (const std::string & id)
{
    const quest_part *part = get_part (id);
    if (part != NULL) return part->is_completed ();
    return false;
}

// check whether a given quest (step) is in progress
bool quest::in_progress (const std::string & id)
{
    const quest_part *part = get_part (id);
    if (part != NULL) return part->in_progress ();
    return false;
}

// set a quest step completed
void quest::set_completed (const std::string & id)
{
    quest_part *part = (quest_part*) get_part (id);
    if (part != NULL) part->set_completed ();
}

// save quests to file
bool quest::save (const std::string & path)
{
    // save quests
    base::diskio record;
    put_state (record);
    
    // save to file
    return record.put_record (path + "/" + QUEST_DATA);
}

// save quests to record
void quest::put_state (base::flat & file)
{
    std::map<std::string, quest_part*>::const_iterator i;
    
    file.put_uint16 ("qsz", Quests.size ());
    for (i = Quests.begin (); i != Quests.end (); i++)
        (*i).second->put_state (file);
}

// load quests from file
bool quest::load ()
{
    // load quests
    base::diskio record;
    
    if (record.get_record (QUEST_DATA))
    	return get_state (record);
        
    return false;
}

// load quests from record
bool quest::get_state (base::flat & file)
{
    quest_part *part;
    u_int16 size = file.get_uint16 ("qsz");
    
    for (int i = 0; i < size; i++)
    {
        part = new quest_part ("", NULL);
        if (part->get_state (file)) add (part);
    }
    
    return file.success ();
}

// add a quest to list of quests
void quest::add (quest_part *part)
{
    std::map<std::string, quest_part*>::iterator q;
    if ((q = Quests.find (part->id ())) != Quests.end ())
    {
        delete (*q).second;
        (*q).second = part;
    }
    else Quests[part->id ()] = part;
}

// return a specific quest part
const quest_part* quest::get_part (const std::string & path)
{
    const std::vector<std::string> & result = split (path);
    std::vector<std::string>::const_iterator i = result.begin ();
    std::map<std::string, quest_part*>::const_iterator q;
    const quest_part *part = NULL;
    
    if ((q = Quests.find (*i)) != Quests.end ())
        part = (*q).second;
    else
        fprintf (stderr, "*** quest::get_part: quest '%s' not found!\n", (*i).c_str ());
    
    for (i++; i != result.end () && part != NULL; i++)
        part = part->child (*i);

    return part;
}

// split given path
std::vector<std::string> quest::split (const std::string & path)
{
    unsigned long idx, pos = 0;
    std::vector<std::string> result;
    
    // split
    while ((idx = path.find (".", pos)) != path.npos)
    {
        result.push_back (path.substr (pos, idx - pos));
        pos = idx + 1;
    }
    
    // add last part
    result.push_back (path.substr (pos));
    
    return result;
}
