/*
 $Id: nls.h,v 1.1 2006/01/22 21:32:39 ksterker Exp $
 
 Copyright (C) 2002/2006 Kai Sterker <kaisterker@linuxgames.com>
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
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*/

/** 
 * @file base/nls.h
 *
 * @author Kai Sterker
 * @brief National Language Support
 */

#ifndef BASE_NLS_H
#define BASE_NLS_H

#include "base/configuration.h"

namespace base
{
    /** 
     * Provides various services for i18n and l10n.
     */
    class nls
    {
    public:
        /**
         * Initialize national language support.
         * @param config The Engine's configuration 
         */
        static void init (configuration & config);
        
        /**
         * Set or change the language to use. Setting the language will
         * never fail, but only if there is a binary message catalogue
         * in <gamedir>/po/<country code>LC_MESSAGES/ named <gamename>.mo
         * the desired language will be actually used.
         * @param language The country code of the desired language.
         */
        static void set_language (const std::string &language);
        
        /**
         * Translate the given string if it's found in the message
         * catalogue. This is just a wrapper around the GNU gettext
         * function for older versions of Python.
         * @param text The text to translate
         * @return either the translated text, or the given text if
         *      no translation was found.
         */
        static const char* translate (const std::string &text);
    };
}

#endif // BASE_NLS_H
