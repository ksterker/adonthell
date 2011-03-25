/*
  Copyright (C) 2009   Frederico Cerveira
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
  along with Adonthell; if not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02120-1301, USA
*/

/**
 * @file   world/pathfinding_manager.cc
 * @author Frederico Cerveira <frederico.cerveira@gmail.com>
 *
 * @brief  Implements the pathfinding_manager class
 */

#include "base/diskio.h"
#include "world/pathfinding_manager.h"
#include "world/area_manager.h"
#include "world/character.h"
#include "world/zone.h"

using world::pathfinding_manager;
using world::character;

// Max number of tasks, it's a fixed value so if every slot is occupied
// there's going to be a problem
static const s_int16 MAX_TASKS = 55;

// The various phases a task can have
static const u_int8 PHASE_PATHFINDING = 1;
static const u_int8 PHASE_MOVING = 2;
static const u_int8 PHASE_FINISHED = 4;
static const u_int8 PHASE_PAUSED = 128;

// ctor
pathfinding_manager::pathfinding_manager()
{
    m_task.reserve(MAX_TASKS);
    m_locked.reserve(MAX_TASKS);

    for (s_int16 i = 0; i < MAX_TASKS; i++)
    {
        std::vector<world::coordinates> * coor = new std::vector<world::coordinates>;
        m_task[i].callback = NULL;
        m_task[i].path = coor;
    }

    m_taskHighest = 0;
    m_locked.assign(MAX_TASKS, false);
}

// dtor
pathfinding_manager::~pathfinding_manager()
{
    clear ();
}

// reset to inital state
void pathfinding_manager::clear ()
{
    /*** FIXME 2011/03/22 Josh Glover <jmglov@gmail.com>
     *** Use an iterator to prevent double-freeing! ***/
    for (s_int16 i = 0; i < (s_int16)m_task.size() && i < MAX_TASKS; i++)
    {
        delete m_task[i].callback;
        m_task[i].callback = NULL;

        delete m_task[i].path;
        m_task[i].path = NULL;
    }
    /*** /FIXME 2011/03/22 Josh Glover <jmglov@gmail.com> ***/

    m_taskHighest = 0;
    m_chars.clear();
    m_locked.assign(MAX_TASKS, false);
}

s_int16 pathfinding_manager::add_task_sec(const character *chr)
{
    s_int16 i = 0;
    for (;i < MAX_TASKS; i++)
    {
        if (m_locked[i] == false)
            return i;
    }

    return -1;
}

bool pathfinding_manager::add_task_ll(const s_int16 id, character * chr,
                                      const world::vector3<s_int32> & target,
                                      const world::vector3<s_int32> & target2, const u_int8 phase,
                                      const u_int8 actualNode, const u_int8 actualDir)
{
    // delete any previously set callback
    delete m_task[id].callback;

    m_task[id].chr = chr;
    m_task[id].target = target;
    m_task[id].target2 = target2;
    m_task[id].callback = NULL;
    m_task[id].phase = phase;
    m_task[id].actualNode = actualNode;
    m_task[id].actualDir = static_cast<character::direction>(actualDir);
    m_task[id].path->clear();
    m_task[id].lastPos.set_x(m_task[id].chr->x());
    m_task[id].lastPos.set_y(m_task[id].chr->y());

    // Verify if we can indeed add this task, or if the character is already performing another task
    // This should be in add_task_sec, however due to a bug it has to stay here
    slist<character *>::iterator ichr = find(m_chars.begin(), m_chars.end(), chr);
    if (ichr == m_chars.end())
    {
        m_locked[id] = true;
        m_chars.push_front(chr);

        m_taskHighest = std::max(id, m_taskHighest); // Keep track of highest slot in use
        return true;
    } else {
        fprintf(stderr, "*** character already exists in list\n");
        return false;
    }
}

s_int16 pathfinding_manager::add_task(character * chr, const vector3<s_int32> & target, const character::direction finalDir)
{
    const s_int16 id = add_task_sec(chr);
    if ((id == -1) || (add_task_ll(id, chr, target, target, PHASE_PATHFINDING, 0, character::NONE) == -1))
        return -1;

    set_final_direction(id, finalDir);

    return id;
}

s_int16 pathfinding_manager::add_task(character * chr, const vector3<s_int32> & target1,
                                      const world::vector3<s_int32> & target2,
                                      const character::direction finalDir)
{
    const s_int16 id = add_task_sec(chr);
    if ((id == -1) || (add_task_ll(id, chr, target1, target2, PHASE_PATHFINDING, 0, character::NONE) == false))
        return -1;

    set_final_direction(id, finalDir);

    return id;
}

s_int16 pathfinding_manager::add_task(character * chr, character * target, const character::direction finalDir)
{
    world::vector3<s_int32> tempTarget1(((target->x()/20)-1)*20, ((target->y()/20)-1)*20, 0);
    world::vector3<s_int32> tempTarget2(((target->x()/20)+1)*20, ((target->y()/20)+1)*20, 0);

    const s_int16 id = add_task_sec(chr);
    if ((id == -1) || (add_task_ll(id, chr, tempTarget1, tempTarget2, PHASE_PATHFINDING, 0, character::NONE) == false))
        return -1;

    set_final_direction(id, finalDir);

    return id;
}

s_int16 pathfinding_manager::add_task(character * chr, std::string & name, const character::direction finalDir)
{
    zone * tempZone = chr->map().get_zone(name);

    if (tempZone == NULL)
        return -1;

    const s_int16 id = add_task_sec(chr);
    if ((id == -1) || (add_task_ll(id, chr, tempZone->min(), tempZone->max(), PHASE_PATHFINDING, 0, character::NONE) == false))
        return -1;

    set_final_direction(id, finalDir);

    return id;
}

void pathfinding_manager::set_callback (const s_int16 id, base::functor_1<const s_int32> * callback)
{
    // get rid of previously set callback, if any
    delete m_task[id].callback;
    m_task[id].callback = callback;
}

void pathfinding_manager::set_final_direction(const s_int16 id, const character::direction finalDir)
{
    m_task[id].finalDir = static_cast<u_int8>(finalDir);
}

void pathfinding_manager::pause_task(const s_int16 id)
{
    m_task[id].phase |= PHASE_PAUSED;
}

void pathfinding_manager::resume_task(const s_int16 id)
{
    m_task[id].phase &= ~PHASE_PAUSED;
}

bool pathfinding_manager::delete_task(const s_int16 id)
{
    // Deletion consists of pausing the task, unlocking it and popping out the character from the list
    if ((id >= MAX_TASKS) || (m_locked[id] == false))
        return false;

    pause_task(id);
    m_locked[id] = false;

    slist<character *>::iterator ichr = find(m_chars.begin(), m_chars.end(), m_task[id].chr);

    if (ichr == m_chars.end())
    {
        fprintf(stderr, "*** Can't find chr in m_chars when deleting task!!\n");
        return false;
    }

    m_chars.erase(ichr);

    // Update highest slot in use
    if (id == m_taskHighest)
    {
        // Verify if there are more tasks other than the one we just deleted
        if (!m_task.empty())
        {
            s_int8 i;
            for (i = m_taskHighest-1; i >= 0; i--)
            {
                if (m_locked[i] == true)
                {
                    // This task is the next highest slot in use. Let's update m_taskHighest
                    m_taskHighest = i;
                    break;
                }
            }
        } else m_taskHighest = 0;
    }

    return true;
}

pathfinding_manager::state pathfinding_manager::return_state(const s_int16 id)
{
    // This might not work always
    if (m_locked[id] == true)
    {
        return ACTIVE;
    } else {
        if (((m_task[id].target.x() / 20) == (m_task[id].chr->x() / 20)) &&
            ((m_task[id].target.y() / 20) == (m_task[id].chr->y() / 20)))
        {
            return SUCCESS;
        } else return FAILURE;
    }
}

void pathfinding_manager::update()
{
    for (s_int16 id = 0; id <= m_taskHighest; id++)
    {
        if (m_locked[id] == true)
        {
            switch(m_task[id].phase)
            {
                case PHASE_PATHFINDING:
                {
                    // Calc the path
                    if (m_pathfinding.find_path(m_task[id].chr, m_task[id].target, m_task[id].target2,
                        m_task[id].path) == false)
                    {
                        // Failed to find the path
                        m_task[id].phase = PHASE_FINISHED;
                        break;
                    }

                    m_task[id].phase = PHASE_MOVING;
                    break;
                }
                case PHASE_MOVING:
                {
                    if (move_chr(id) == true)
                        m_task[id].phase = PHASE_FINISHED;
                    break;
                }
                case PHASE_FINISHED:
                {
                    m_task[id].actualDir = m_task[id].finalDir;
                    m_task[id].chr->set_direction(m_task[id].finalDir);
                    m_task[id].chr->stop();
                    m_task[id].chr->update_state();
                    delete_task(id);

                    // needs to happen after task is unlocked, otherwise
                    // return_state() will always report ACTIVE.
                    if (m_task[id].callback != NULL)
                    {
                        (*m_task[id].callback)((s_int32) return_state(id));
                    }

                    break;
                }
            }
        }
    }
}

u_int8 pathfinding_manager::calc_distance(const world::coordinates & node, const world::character * chr)
{
    u_int32 x_diff = abs(chr->x() - node.x());
    u_int32 y_diff = abs(chr->y() - node.y());

    x_diff *= x_diff;
    y_diff *= y_diff;

    return sqrt((x_diff + y_diff));
}

bool pathfinding_manager::move_chr(const s_int16 id)
{
    s_int32 grid_x = m_task[id].chr->x() / 20;
    s_int32 grid_y = m_task[id].chr->y() / 20;
    s_int32 target_grid_x = m_task[id].path->at(m_task[id].actualNode).x();
    s_int32 target_grid_y = m_task[id].path->at(m_task[id].actualNode).y();
    
    // Check if the direction has already been computed
    if (m_task[id].actualDir != character::NONE)
    {

        // Verify if we're on the intended grid
        if ((grid_x == target_grid_x) && (grid_y == target_grid_y))
        {
            
            // Check if we've finished the path
            if (m_task[id].path->size() - 1 == m_task[id].actualNode)
            {
                return true;
            } else {
                ++m_task[id].actualNode;
                m_task[id].actualDir = character::NONE;
            }
        } else {
            
            // Verify if we are stuck
            if ((m_task[id].lastPos.x() == m_task[id].chr->x()) && (m_task[id].lastPos.y() == m_task[id].chr->y()))
            {
                // We are stuck, let's find another path
                
                // Stop the character
                m_task[id].chr->stop();
                m_task[id].chr->update_state();
                
                // Don't forget to clear the path
                m_task[id].path->clear();
                
                if (m_pathfinding.find_path(m_task[id].chr, m_task[id].target, m_task[id].target2, m_task[id].path) == false)
                    m_task[id].phase = PHASE_FINISHED;
                
                if ((m_task[id].path->empty()) || (m_task[id].path->size() == 1))
                    m_task[id].phase = PHASE_FINISHED;
                
                m_task[id].actualNode = 0;
                m_task[id].actualDir = character::NONE;
            }
            
            // Update lastPos with our actual position
            m_task[id].lastPos.set_x(m_task[id].chr->x());
            m_task[id].lastPos.set_y(m_task[id].chr->y());
            
            
         }
    } else {

        // Calculate the next node
     
        if (grid_y > target_grid_y)
        {
            // We have to move up
            m_task[id].actualDir |= character::NORTH;
            m_task[id].chr->set_direction(m_task[id].actualDir);

        } else if (grid_y < target_grid_y) {
            // We have to move down
            m_task[id].actualDir |= character::SOUTH;
            m_task[id].chr->set_direction(m_task[id].actualDir);
        }
	
        if (grid_x > target_grid_x)
        {
            // We have to move left
            m_task[id].actualDir |= character::WEST;
            m_task[id].chr->set_direction(m_task[id].actualDir);

        } else if (grid_x < target_grid_x) {
            // We have to move right
            m_task[id].actualDir |= character::EAST;
            m_task[id].chr->set_direction(m_task[id].actualDir);
        }

    }
    
    return false;
}

void pathfinding_manager::put_state(base::flat & file)
{
    base::flat record;
    base::flat taskBlock;

    for (s_int16 i = 0; i <= m_taskHighest; i++)
    {
        if (m_locked[i] == true)
        {
            std::string t = m_task[i].chr->uid();

            if (t.empty() == false)
            {
                taskBlock.put_string("chrName", t);
                taskBlock.put_sint32("target_x", m_task[i].target.x());
                taskBlock.put_sint32("target_y", m_task[i].target.y());
                taskBlock.put_sint32("target2_x", m_task[i].target2.x());
                taskBlock.put_sint32("target2_y", m_task[i].target2.y());
                taskBlock.put_uint8("phase", m_task[i].phase);
                taskBlock.put_uint8("aNode", m_task[i].actualNode);
                taskBlock.put_uint8("aDir", m_task[i].actualDir);
                taskBlock.put_uint8("finalDir", m_task[i].finalDir);

                std::stringstream a;

                // Saves the path
                for (u_int16 c = 0; c < m_task[i].path->size(); c++)
                {

                    a.clear();a.str("");
                    a << "node" << c << "x";
                    taskBlock.put_sint32(a.str(), m_task[i].path->at(c).x());

                    a.clear();a.str("");
                    a << "node" << c << "y";
                    taskBlock.put_sint32(a.str(), m_task[i].path->at(c).y());
                }

                a.clear();a.str("");
                a << i;

                record.put_flat(a.str(), taskBlock);
                taskBlock.clear();
            } else {
                fprintf(stderr, "Can't find it!!\n");
            }
        }
    }
    file.put_flat ("paths", record);
}

void pathfinding_manager::get_state(base::flat & file)
{
    base::flat record = file.get_flat ("paths");
    base::flat taskBlock;

    for (s_int16 i = 0; i < MAX_TASKS; i++)
    {
        std::stringstream a;

        a << i;

        taskBlock = record.get_flat(a.str(), true);

        if (taskBlock.size() == 1)
            continue; // There isn't any data for this position

        // Translates the character name into a pointer of a character existent
        // in the map given
        std::string chrName = taskBlock.get_string("chrName");
        placeable * tPlac = world::area_manager::get_map()->get_entity(chrName);

        if (tPlac == NULL)
            continue;

        character * tChr = dynamic_cast<character *>(tPlac);

        s_int32 tX = taskBlock.get_sint32("target_x");
        s_int32 tY = taskBlock.get_sint32("target_y");
        s_int32 tX2 = taskBlock.get_sint32("target2_x");
        s_int32 tY2 = taskBlock.get_sint32("target2_y");
        u_int8 phase = taskBlock.get_uint8("phase");
        u_int8 aNode = taskBlock.get_uint8("aNode");
        u_int8 aDir = taskBlock.get_uint8("aDir");
        u_int8 finalDir = taskBlock.get_uint8("finalDir");

        world::vector3<s_int32> tempTarget(tX, tY, 0);
        world::vector3<s_int32> tempTarget2(tX2, tY2, 0);

        // Creates a new task with all the info
        add_task_ll(i, tChr, tempTarget, tempTarget2, phase, aNode, aDir);
        set_final_direction(i, static_cast<character::direction>(finalDir));

        // Now let's load the path
        for (u_int16 c = 0; c < 65535; c++)
        {
            // Get the X axis value
            a.clear();a.str("");
            a << "node" << c << "x";
            s_int32 nodeX = taskBlock.get_sint32(a.str());

            if (taskBlock.success() == false)
                break; // This node doesn't exists so we've already seen every node

            // Get the Y axis value
            a.clear();a.str("");
            a << "node" << c << "y";
            s_int32 nodeY = taskBlock.get_sint32(a.str());

            // Add the node to the path
            coordinates tempNode(nodeX, nodeY, 0);
            m_task[i].path->push_back(tempNode);
        }

        // Set the character in the correct direction
        tChr->set_direction((character::direction)aDir);
        m_taskHighest = i;
    }
}

bool pathfinding_manager::save(std::string & fname)
{
    base::diskio record(base::diskio::XML_FILE);
    put_state(record);

    // write data to disk
    return record.put_record(fname);
}

bool pathfinding_manager::load(std::string & fname)
{
    base::diskio record (base::diskio::XML_FILE);

    if (record.get_record(fname))
    {
        get_state(record);
        return true;
    } else return false;
}
