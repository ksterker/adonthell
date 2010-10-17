/*
   Copyright (C) 2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file 	base/configuration.cc
 * @author 	Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief 	Provides access to configuration file.
 */

#include <cstdio>
#include <stdlib.h>
#include <sys/stat.h>
#include "base/configuration.h"
#include "base/configio.h"
#include "base/base.h"

using base::configuration;
using base::cfg_choice;
using base::cfg_option;
using base::cfg_range;

// ctor
cfg_option::cfg_option (const string & value, const u_int8 & type)
{
    Value = value;
    Type = type;
}

// get value of configuration option
string cfg_option::value ()
{
    return Value;
}

// set value of configuration option
void cfg_option::set_value (const string & value)
{
    Value = value;
}


// ctor
cfg_range::cfg_range (const string & value)
    : cfg_option (value, cfg_option::RANGE)
{
    Min = 0;
    Max = 0;
}

// set valid range
void cfg_range::set_range (const u_int16 & min, const u_int16 & max)
{
    Min = min < max ? min : max;
    Max = max > min ? max : min;
    
    validate ();
}

// set value of configuration option
void cfg_range::set_value (const string & value)
{
    Value = value;
    validate ();
}

// make sure that the options value is within options range
void cfg_range::validate ()
{
    char *end = NULL;
    s_int32 newval;
    s_int32 intval = (s_int32) strtol (Value.c_str (), &end, 10);
    
    if (*end == '\0') 
    {
        if (intval >= Min)
        {
            // everything okay, so return
            if (intval <= Max) return;
            else newval = Max;
        }
        else newval = Min; 
    }
    else 
    {
        // value is not an integer
        fprintf (stderr, "*** cfg_range::validate: Can't convert '%s' to int!\n", Value.c_str ());
        newval = Min;
    }

    // set a valid value
    char value[8];
    snprintf (value, 7, "%i", newval);
    Value = value;
}


// ctor
cfg_choice::cfg_choice (const string & value)
    : cfg_option (value, cfg_option::CHOICE)
{
    Selection = "";
}

// add valid option with a label 
void cfg_choice::add (const string & label, const string & value)
{
    Choices[label] = value;
    // update current selection
    if (value == Value) Selection = label;
}

// change selection
void cfg_choice::select (const string & label)
{
    Value = Choices[label];
    Selection = label;
}

// return list of available choices
vector<const char*> cfg_choice::get_choices () const
{
    vector<const char*> list;
    map<string, string>::const_iterator i;

    for (i = Choices.begin (); i != Choices.end (); i++)
        list.push_back ((*i).first.c_str ());

    return list;
}


// dtor
configuration::cfg_section::~cfg_section ()
{
    map<string, cfg_option*>::iterator o;
    for (o = Options.begin (); o != Options.end (); o++)
        delete (*o).second;
}

// add option to a section
void configuration::cfg_section::add (const string & label, cfg_option * option)
{
    map<string, cfg_option*>::iterator o = Options.find (label.c_str ());
    if (o != Options.end ())
    {
        delete (*o).second;
        (*o).second = option;
    }
    else Options[label] = option;
}

// remove option from section
int configuration::cfg_section::remove (const string & label)
{
    map<string, cfg_option*>::iterator o = Options.find (label.c_str ());
    if (o != Options.end ())
    {
        delete (*o).second;
        Options.erase (o);
    }
    return Options.size ();
}

// return option with given name
cfg_option *configuration::cfg_section::option (const string & label)
{
    map<string, cfg_option*>::const_iterator o = Options.find (label.c_str ());
    if (o != Options.end ()) return (*o).second;
    
    return NULL;
}

// return names of all available options
vector<const char*> configuration::cfg_section::get_options (const bool & get_undef) const
{
    vector<const char*> list;
    map<string, cfg_option*>::const_iterator i;

    for (i = Options.begin (); i != Options.end (); i++)
        if (get_undef || (*i).second->type () != cfg_option::UNDEF)
            list.push_back ((*i).first.c_str ());

    return list;
}


// dtor
configuration::~configuration ()
{
    map<string, cfg_section*>::iterator s;
    for (s = Sections.begin (); s != Sections.end (); s++)
        delete (*s).second;
}

// get given option from given section as integer value.
s_int32 configuration::get_int (const string & section, const string & option, const s_int32 & value)
{
    cfg_option *o = this->option (section, option, cfg_option::UNDEF);
    if (o != NULL)
    {
        char *end = NULL;
        s_int32 retval = (s_int32) strtol (o->value ().c_str (), &end, 10);
        if (*end == '\0') return retval;
        else fprintf (stderr, "*** configuration::get_int: Can't convert '%s' to int!\n", o->value ().c_str ());
    }
    
    char val[16];
    snprintf (val, 15, "%i", value);
    add_option (section, option, new cfg_option (val));
    
    return value;
}

// get given option from given section as double value.
double configuration::get_double (const string & section, const string & option, const double & value)
{
    cfg_option *o = this->option (section, option, cfg_option::UNDEF);
    if (o != NULL)
    {
        char *end = NULL;
        double retval = (double) strtod (o->value ().c_str (), &end);
        if (*end == '\0') return retval;
        else fprintf (stderr, "*** configuration::get_double: Can't convert '%s' to double!\n", 
            o->value ().c_str ());
    }
    
    char val[16];
    snprintf (val, 15, "%f", value);
    add_option (section, option, new cfg_option (val));
    
    return value;
}

// get given option from given section as string value.
string configuration::get_string (const string & section, const string & option, const string & value)
{
    cfg_option *o = this->option (section, option, cfg_option::UNDEF);
    if (o != NULL) return o->value ();
    
    // add option with default value
    add_option (section, option, new cfg_option (value));
    return value;
}

// read configuration from file
bool configuration::read (const string & name)
{
    // try to read file
    return base::config_io::read (create_filename (name), this);
}

// write configuration to file
void configuration::write (const string & name) const
{
	// do not override configuration if startup fails before
	// the existing configuration was read
	if (Sections.empty()) return;

    // try to write file
    if (!base::config_io::write (create_filename (name), this))
    {
        fprintf (stderr, "*** configuration::write: failed to save configuration!\n");
    }
}

// get names of available sections
vector<const char*> configuration::get_sections (const bool & get_undef) const
{
    vector<const char*> list;
    map<string, cfg_section*>::const_iterator i;

    for (i = Sections.begin (); i != Sections.end (); i++)
        if (get_undef || (*i).second->get_options (get_undef).size () > 0)
            list.push_back ((*i).first.c_str ());

    return list;
}

// get names of available options in given section
vector<const char*> configuration::get_options (const string & section, const bool & get_undef) const
{
    map<string, cfg_section*>::const_iterator s = Sections.find (section.c_str ());

    if (s != Sections.end ()) return (*s).second->get_options (get_undef);
    else return vector<const char*>();
}

// get given option from given section.
cfg_option *configuration::option (const string & section, const string & option, const u_int8 & type) const
{
    map<string, cfg_section*>::const_iterator s = Sections.find (section.c_str ());
    if (s != Sections.end ())
    {
        cfg_option *opt = (*s).second->option (option);
        if (opt != NULL) 
        {
            // if types match, return
            if (type == cfg_option::UNDEF || opt->type () == type) return opt;
            
            // otherwise, cast to requested type
            cfg_option *new_option;
            
            switch (type)
            {
                case cfg_option::FREE:
                case cfg_option::BOOL:
                {
                    new_option = new cfg_option (opt->value (), type);
                    break;
                }
                case cfg_option::RANGE:
                {
                    new_option = new cfg_range (opt->value ());
                    break;
                }
                case cfg_option::CHOICE:
                {
                    new_option = new cfg_choice (opt->value ());
                    break;
                }
                default:
                {
                    fprintf (stderr, "*** configuration::option: Unknown config option type %i", type);
                    return NULL;
                }
            }
            
            // set the new option (automatically frees the previous option)
            (*s).second->add (option, new_option);

            return new_option;
        }
        else fprintf (stderr, "*** configuration::option: No option '%s' in section '%s'!\n", 
            option.c_str(), section.c_str()); 
    }
    else fprintf (stderr, "*** configuration::option: No section '%s' in configuration file!\n", 
        section.c_str());
    
    return NULL;
}

// add or replace a configuration option
void configuration::add_option (const string & section, const string & option, cfg_option *value)
{
    if (value == NULL)
    {
        remove_option (section, option);
        return;
    }

    map<string, cfg_section*>::iterator s = Sections.find (section.c_str ());
    
    // if section does not exist yet, create it
    if (s == Sections.end ())
    {
        cfg_section *new_section = new cfg_section ();
        new_section->add (option, value);
        Sections[section] = new_section; 
    }
    else
    {
        (*s).second->add (option, value);
    }
}

// remove a configuration option
void configuration::remove_option (const string & section, const string & option)
{
    map<string, cfg_section*>::iterator s = Sections.find (section.c_str ());
    
    if (s != Sections.end ())
    {
        // if section is empty after removal, remove it as well
        if ((*s).second->remove (option) == 0)
        {
            delete (*s).second;
            Sections.erase (s);
        }
    }
}


// calculate file name
string configuration::create_filename (const string & name) const
{
    // add file name to configuration data directory
    return base::Paths.cfg_data_dir () + name + ".xml";
}    

vector<string> configuration::split_value (const string & values, const char & separator)
{
    string::size_type i, idx, pos = 0;
    vector<string> result;

    // split pipe separated list of controls
    do
    {
    	idx = values.find (separator, pos);
    	string value = values.substr (pos, idx - pos);

    	// trim whitespace
    	if ((i = value.find_last_not_of(' ')) != string::npos)
    	{
    		// strip back
    	    value.erase (i + 1);
    	    if ((i = value.find_first_not_of(' ')) != string::npos)
    	    {
    	    	// strip front
    	    	value.erase (0, i);
    	    }

    	    // found a possibly valid control
    	    result.push_back (value);
    	}
    	// else: only whitespace, so we skip it

    	pos = idx + 1;
    }
    while (idx != string::npos);

    return result;
}
