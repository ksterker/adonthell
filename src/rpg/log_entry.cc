/*
   $Id: log_entry.cc,v 1.1 2004/07/03 05:55:38 ksterker Exp $
   
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
 * @file   log_entry.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Entry .
 */

#include "rpg/log_entry.h"
#include "event/date.h"

using rpg::log_entry;
using std::string;

// create new log entry
log_entry::log_entry (const string & topic, const string & text, const string & uid)
{
    Timestamp = event::date::time ();
    Topic = topic;
    Text = text;
    Uid = uid;
}
