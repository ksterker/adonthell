/*
 $Id: nls.cc,v 1.2 2006/09/12 06:49:27 ksterker Exp $
 
 Copyright (C) 2002/2003/2006 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file base/nls.cc
 *
 * @author Kai Sterker
 * @brief National Language Support
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <locale.h>
#include "base/gettext.h"
#include "base/nls.h"

using base::nls;

// Initialize NLS
void nls::init (base::configuration & config)
{
#if ENABLE_NLS
    // if no language specified in the config file, determine
    // the locale from the environment variables
    std::string locale = config.get_string ("Language", "Locale", "");
    if (locale == "")
        setlocale (LC_MESSAGES, "");
    // otherwise overwrite any environment variables
    else 
        set_language (locale);

    // open the message catalogue
    std::string location = base::Paths.game_data_dir () + "/locale";
    const char *domain = base::Paths.game ().c_str ();

    bindtextdomain (domain, location.c_str ());
    textdomain (domain);
    bind_textdomain_codeset (domain, "UTF-8");
#endif
}

// Set the language to use
void nls::set_language (const std::string &language)
{    
#if ENABLE_NLS
#ifndef __BEOS__
    std::string lang = "LANGUAGE=" + language;
    putenv ((char *) lang.c_str ());
#else
	// FIXME: no putenv on BEOS, but there should be setenv	
#endif
    {
        // tell gettext that the language has changed
        extern int _nl_msg_cat_cntr;
        ++_nl_msg_cat_cntr;
    }
    
    setlocale (LC_MESSAGES, language.c_str ());	
#endif
}

// Translate some text
const char* nls::translate (const std::string &text)
{
#if ENABLE_NLS
    return gettext (text.c_str ());
#else
    return text.c_str ();
#endif
}
