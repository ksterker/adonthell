/*
   $Id: character.cc,v 1.1 2004/05/31 11:44:50 ksterker Exp $
   
   Copyright (C) 2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   rpg/character.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Base class for creatures, NPCs and Player characters.
 */

#include "rpg/character.h"

using rpg::character;

//
void character::put_state (base::flat & file) const
{
}

// 
bool character::get_state (base::flat & file)
{
    return file.success ();
}
