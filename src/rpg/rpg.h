/*
 Copyright (C) 2013 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file rpg/rpg.h
 *
 * @author Kai Sterker
 * @brief RPG module initialization
 */

#ifndef RPG_H_
#define RPG_H_

#include <adonthell/base/configuration.h>

namespace rpg
{
    /**
     * @name Initialization and cleanup.
     */
    //@{
    /**
     * Initialize the RPG module.
     * @param cfg engine configuration.
     */
    void init(base::configuration & cfg);

    /**
     * Shutdown the RPG module.
     */
    void cleanup();
    //@}
}

#endif /* RPG_H_ */
