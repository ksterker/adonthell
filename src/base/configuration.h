/*
   $Id: configuration.h,v 1.4 2005/03/08 09:41:47 ksterker Exp $

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
 * @file 	base/configuration.h 
 * @author 	Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief 	Provides access to %configuration file.
 */

#ifndef BASE_CONFIGURATION_H
#define BASE_CONFIGURATION_H

#include <vector>
#include <string>
#include <map>

#include "base/types.h"

using std::string;
#ifndef SWIG
using std::vector;
using std::map;
#endif

namespace base
{
    /**
     * A single %configuration option, storing a value of arbitrary type. Base class
     * for a number of %configuration options that impose some constraints on the
     * value stored.
     */
    class cfg_option
    {
        public:
            /**
             * Types of %configuration options.
             */
            enum
            {
                UNDEF, BOOL, FREE, RANGE, CHOICE
            };            

            /**
             * Create a new option.
             * @param value value of option.
             * @param type type of %configuration option.
             */
            cfg_option (const string & value, const u_int8 & type = cfg_option::UNDEF);
            
            /**
             * Destructor.
             */
            virtual ~cfg_option () { }
            
            /**
             * Get value of this %configuration option.
             * @return value as a string.
             */
            string value ();
            
            /**
             * Change value of this option.
             * @param value new value.
             */
            virtual void set_value (const string & value);
        
            /**
             * Return the comment attached to this option.
             * @return description of %configuration option.
             */
            string comment () const { return Comment; }
            
            /**
             * Attach a comment attached to this option.
             * @param comment description of %configuration option.
             */
            void set_comment (const string & comment) { Comment = comment; }
            
            /**
             * Get type of %configuration option. This can be used by the %configuration
             * GUI to generate fitting widgets for this %configuration option.
             * @return type of %configuration option (configuration::UNDEF if unknown).
             */
            u_int8 type () const { return Type; }

        protected:
            /// Actual value of the %configuration option
            string Value;

        private:
            /// Description of this option
            string Comment;
            
            /// Type of the %configuration option (FREE, BOOL, RANGE, CHOICE ...)
            u_int8 Type;
    };

    /**
     * Restricts an option to a certain range. Only available for integer options.
     */
    class cfg_range : public cfg_option
    {
        public:
            /**
             * Create a new range with given value as default setting.
             * @param value initial setting
             */
            cfg_range (const string & value);
            
            /**
             * Specify boundries of range.
             * @param min minimum of range.
             * @param max maximum of range.
             */
            void set_range (const u_int16 & min, const u_int16 & max);
            
            /**
             * Change value of this option, within the allowed range. 
             * @param value new value.
             */
            void set_value (const string & value);

            /**
             * Return left border of range.
             * @return minimum of range.
             */
            u_int16 min () const { return Min; }
            
            /**
             * Return right border of range.
             * @return maximum of range.
             */
            u_int16 max () const { return Max; }
             
        private:
            /**
             * Make sure the options value is within the options range.
             * Changes the value, if neccessary.
             */
            void validate ();
            
            /// minimum value of range
            u_int16 Min;
            /// maximum value of range
            u_int16 Max;
    };
    
    /**
     * Restricts an option to a given number of choices. That is, valid settings can be
     * picked from a list of predefined options.
     */
    class cfg_choice : public cfg_option
    {
        public:
            /**
             * Create a new choice.
             * @param value value of option.
             */
            cfg_choice (const string & value);
        
            /**
             * Add another valid option to chose from.
             * @param label name of the option as displayed by the GUI.
             * @param value actual value of the option.
             */
            void add (const string & label, const string & value);
            
            /**
             * Methods for %configuration GUI
             */
            //@{
            /**
             * Set the current selection. Updates the current value.
             * @param label name of the option as displayed by the GUI.
             */
            void select (const string & label);
            
            /**
             * Get name of currently selected value.
             * @return label for the selected value to display by GUI.
             */
            string get_selection () const { return Selection; }
            
            /**
             * Get list of all available labels.
             * @return names of all settings known to this choice.
             */
            vector<const char*> get_choices () const;
            //@}
            
        private:
            /// label of the currently selected value
            string Selection;
            /// valid values, each with a unique label
            map<string, string> Choices;
    };

    /**
     * This class provides access to the %configuration file.
     * 
     * It also provides meta information to be used by a graphical user interface.
     * This meta information needs to be set by each module that stores settings
     * in the %configuration file. Each module also needs to check the values returned
     * by this class for sanity.
     *
     * To allow for a fully dynamic %configuration GUI, the %configuration file is
     * divided into different sections. The GUI should create a different page for
     * each section. Further, each individual option has to be given meta information,
     * telling the GUI about constraints and style to use for that option. Currently,
     * the following types of options are available:
     *
     * - BOOL: this option allows exactly two settings. On/Off, True/False, etc.
     * - FREE: this option has no constraints; it allows the user to enter arbitrary values.
     * - RANGE: this options accepts values out of given (integer) range.
     * - CHOICE: this option accepts values out of a predefined set of settings.
     *
     * High level methods allow retrieval of %configuration settings, cast to their 
     * expected type. Further methods allow to query for the names of available sections
     * and the options within a certain section. Finally, low level methods allow
     * access to the meta data kept by the %configuration class.
     */
    class configuration
    {
        /**
         * Class representing one section of the %configuration file.
         */
        class cfg_section
        {
            public:
                /**
                 * Destructor.
                 */
                ~cfg_section ();
                
                /**
                 * Add option with given label to this section.
                 * @param label name of the option to add.
                 * @param option actual value with meta information.
                 */
                void add (const string & label, cfg_option *option);

                /**
                 * Remove option with given label from this section.
                 * @param label name of the option to remove.
                 * @return number of options left in the section.
                 */
                int remove (const string & label);
                 
                /**
                 * Return option with the given label.
                 * @param label name of the option to retrieve.
                 * @return pointer to option structure.
                 */
                cfg_option *option (const string & label);
                
                /**
                 * Return names of options available in that section.
                 * @param get_undef unless \c true, only list options with type != UNDEF 
                 * @return list of labels for options in that section.
                 */
                vector<const char*> get_options (const bool & undef = false) const;
                
            private:
                /// Options available in that section.
                map<string, cfg_option*> Options;
        };
        
        public:
            /**
             * Destructor.
             */
            ~configuration ();

            /**
             * @name Value retrieval
             */
            //@{
            /**
             * Return the value of given option from given section as an integer. If the option
             * does not exist yet, it will be created and assigned the given default value.
             * @param section name of section.
             * @param option name of %configuration option.
             * @param value default value, in case the option does not exist yet.
             * @return %configuration setting.
             */
            s_int32 get_int (const string & section, const string & option, const s_int32 & value);
            
            /**
             * Return the value of given option from given section as a double. If the option
             * does not exist yet, it will be created and assigned the given default value.
             * @param section name of section.
             * @param option name of %configuration option.
             * @param value default value, in case the option does not exist yet.
             * @return %configuration setting.
             */
            double get_double (const string & section, const string & option, const double & value);
            
            /**
             * Return the value of given option from given section as string. If the option
             * does not exist yet, it will be created and assigned the given default value.
             * @param section name of section.
             * @param option name of %configuration option.
             * @param value default value, in case the option does not exist yet.
             * @return %configuration setting.
             */
            string get_string (const string & section, const string & option, const string & value);
            //@}
            
            /**
             * @name loading / saving
             */
            //@{
            /**
             * Read %configuration from file. The exact location of the %configuration
             * file is determined by the configuration::create_filename method.
             * @param name usually name of the game, whose %configuration to load.
             */
            bool read (const string & name);
            
            /**
             * Save %configuration to file. The exact location of the %configuration
             * file is determined by the configuration::create_filename method.
             * @param name usually name of the game, whose %configuration to save.
             */
            void write (const string & name) const;
            //@}
            
            /**
             * @name Content retrieval
             */ 
            //@{
            /**
             * Return names of available sections.
             * @param get_undef unless \c true, only list sections with 'defined' options 
             * @return list of section names.
             */
            vector<const char*> get_sections (const bool & get_undef = false) const;

            /**
             * Return names of options available in given section.
             * @param section name of section, whose entries to retrieve.
             * @param get_undef unless \c true, only list options with type != UNDEF 
             * @return list of labels for options in given section.
             */
            vector<const char*> get_options (const string & section, const bool & get_undef = false) const;
            //@}
            
            /**
             * @name Low level access
             */
            //@{
            /**
             * Return setting with given name from given section, cast to given option type.
             * @param section name of section.
             * @param option name of %configuration option.
             * @param type type of %configuration option.
             * @return value and meta-information of requested option, or NULL on error.
             */
            cfg_option *option (const string & section, const string & option, const u_int8 & type) const;

            /**
             * Add or replace an option in the given section with the given value. If the 
             * value is \c NULL, remove the option instead. 
             * @param section name of section.
             * @param option name of %configuration option.
             * @param value actual value and meta information of given option.
             */
            void add_option (const string & section, const string & option, cfg_option *value);

            /**
             * Remove an option in the given section. Removes the section too, if it is empty
             * afterwards.
             * @param section name of section.
             * @param option name of %configuration option.
             */
            void remove_option (const string & section, const string & option);
            //@}
            
        private:
            /**
             * Calculate name of %configuration file from given string. On UNIX-like
             * systems, the %configuration file will be $HOME/.adonthell/<name>.xml .
             * On Windows it will just be <name>.xml .
             * @param name filename for config file, usually the name of the game.
             */
            string create_filename (const string & name) const;
                    
            /// sections available in %configuration file
            map<string, cfg_section*> Sections;
    };
}

#endif // BASE_CONFIGURATION_H
