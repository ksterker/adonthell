/*
 $Id: dialog_line.cc,v 1.1 2006/01/22 21:32:40 ksterker Exp $
 
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
 * @file   rpg/dialog_line.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Class representig a line of a dialog to be displayed by the UI.
 */

#include "dialog_line.h"

using rpg::dialog_line;

// ctor
dialog_line::dialog_line (const std::string & speaker, const std::string & text, dialog_line *prev)
{
    Speaker = speaker;
    Text = text;
    SelectedAnswer = -1;
    Prev = prev;
    Next = NULL;
    
    if (Prev != NULL)
    {
        Prev->Next = this;
    }
}

// dtor
dialog_line::~dialog_line()
{
    if (Next != NULL)
    {
        delete Next;
    }
}
