/*
   $Id: quest.cc,v 1.1 2004/08/02 07:37:29 ksterker Exp $
   
   Copyright (C) 2004 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

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
 
#include "rpg/quest.h"
#include "base/diskio.h"
#include "python/python.h"

using rpg::quest;
using rpg::quest_part;

// ctor
quest_part::quest_part (const std::string & id)
{
    Id = id;
    Code = "";
    Entry = NULL;
    Parent = NULL;
    Started = false;
    Completed = false;
}

// dtor
quest_part::~quest_part ()
{
    std::map<std::string, quest_part*>::iterator q;
    for (q = Children.begin (); q != Children.end (); q++)
        delete (*q).second;
}

// set a step to completed
bool quest_part::set_completed ()
{
    // only steps can be completed that way
    if (!Children.empty ()) return false;
    
    Completed = true;
    Started = true;

    // update parent, if it has one 
    if (Parent != NULL) Parent->update ();
    
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
    
    // if state changed, update parent too
    if (Parent != NULL && changed) Parent->update ();
}

// calculate the state of completion with given code
bool quest_part::evaluate ()
{
    std::string code = "", token = 0;
    std::map<std::string, quest_part*>::iterator child;

    // prepare code for evaluation, replacing all references to quest parts with
    // their state of completion
    for (std::string::const_iterator i = Code.begin (); i != Code.end (); i++)
    {
        if (!isalnum (*i)) 
        {
            // check if we have just read a new token
            if (token != "")
            {
                // check if the token we read is a quest_part
                if ((child = Children.find (token)) != Children.end ())
                    // if so, add it's state of completion to the code
                    code += (*child).second->is_completed () ? '1' : '0';
                else
                    // otherwise assume the token is part of the code
                    code += token;
                
                // reset token
                token = "";
            }
            code += *i;            
        }
        else token += *i;
    }
    
    // now try to evaluate our code snippet
    PyObject *result = python::run_string (code);
    if (result == NULL || !PyInt_Check (result))
    {
        fprintf (stderr, "*** quest_part::evaluate: Error in quest completion calculation rule:\n'%s'\n", 
            code.c_str ());
        return false;
    }
    
    // retrieve the result
    bool completed = PyInt_AS_LONG (result);
    Py_DECREF (result);
    return completed; 
}

// save quest_part
void quest_part::put_state (base::flat & out) const
{
    base::flat record;
    
    // save attributes
    record.put_string ("qid", Id);
    record.put_bool ("qst", Started);
    record.put_bool ("qcp", Completed);
    record.put_string ("qcd", Code);
    
    // save associated log entry
    record.put_bool ("qhe", Entry != NULL);
    if (Entry) Entry->put_state (record);

    // save child quest parts
    std::map<std::string, quest_part*>::const_iterator i;
    record.put_uint16 ("qnc", Children.size ());
    for (i = Children.begin (); i != Children.end (); i++)
        (*i).second->put_state (record);
        
    // save this quest part to stream
    out.put_flat ("q", record);
}

// load quest part
bool quest_part::get_state (base::flat & in)
{
    base::flat record = in.get_flat ("q");
    if (!in.success ()) return false;

    // get attributes
    Id = record.get_string ("qid");
    Started = record.get_bool ("qst");
    Completed = record.get_bool ("qcp");
    Code = record.get_string ("qcd");

    // get log entry, if any
    if (record.get_bool ("qhe") == true)
    {
        Entry = new rpg::log_entry ("", "", "");
        Entry->get_state (record);
    }

    // get children, if any
    quest_part *part;
    for (u_int16 i = record.get_uint16 ("qhc"); i > 0; i--)
    {
        part = new quest_part ("");
        part->get_state (record);
        part->Parent = this;
        Children[part->Id] = part;
    }
    
    return record.success ();
}

// storage for the different quests
std::map<std::string, quest_part*> quest::Quests;

// delete all quests
void quest::cleanup ()
{
    std::map<std::string, quest_part*>::iterator q;
    for (q = Quests.begin (); q != Quests.end (); q++)
        delete (*q).second;
}

// check wether a given quest (step) is in progress
bool quest::is_started (const std::string & id)
{
    const quest_part *part = get_part (id);
    if (part != NULL) return part->is_started ();
    return false;
}

// check wether a given quest (step) is in progress
bool quest::is_completed (const std::string & id)
{
    const quest_part *part = get_part (id);
    if (part != NULL) return part->is_completed ();
    return false;
}

// check wether a given quest (step) is in progress
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
void quest::put_state ()
{
    // open file
    base::ogzstream out (QUEST_DATA);
    if (!out.is_open ())
    {
        fprintf (stderr, "*** quest::put_state: cannot open '" QUEST_DATA "' for writing!\n");
        return; 
    }
    
    // save quests
    base::diskio record;
    put_state (record);
    
    // save to file
    record.put_record (out);
}

// save quests to record
void quest::put_state (base::flat & out)
{
    std::map<std::string, quest_part*>::const_iterator i;
    
    out.put_uint16 ("qsz", Quests.size ());
    for (i = Quests.begin (); i != Quests.end (); i++)
        (*i).second->put_state (out);
}

// load quests from file
bool quest::get_state ()
{
    // open file
    base::igzstream in (QUEST_DATA);
    if (!in.is_open ())
    {
        fprintf (stderr, "*** quest::put_state: cannot open '" QUEST_DATA "' for reading!\n");
        return false; 
    }
    
    // load quests
    base::diskio record;
    if (record.get_record (in))
    	return get_state (record);
        
    return false;
}

// load quests from record
bool quest::get_state (base::flat & in)
{
    quest_part *part;
    u_int16 size = in.get_uint16 ("qsz");
    
    for (int i = 0; i < size; i++)
    {
        part = new quest_part ("");
        if (part->get_state (in)) add (part);
    }
    
    return in.success ();
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
        fprintf (stderr, "*** quest::set_completed: quest '%s' no found!\n", (*i).c_str ());
    
    for (i = i++; i != result.end () && part != NULL; i++)
        part = part->child (*i);

    return part;
}

// split given path
std::vector<std::string> quest::split (const std::string & path)
{
    u_int32 idx, pos = 0;
    std::vector<std::string> result;
    
    // split
    while ((idx = path.find (".", pos)) != path.npos)
    {
        result.push_back (path.substr (pos, idx - pos));
        pos = idx;
    }
    
    // add last part
    result.push_back (path.substr (pos));
    
    return result;
}
