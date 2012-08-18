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

#include <adonthell/base/diskio.h>
#include "pathfinding_manager.h"
#include "area_manager.h"
#include "character.h"
#include "zone.h"

using world::pathfinding_manager;
using world::character;

// Max number of tasks, it's a fixed value so if every slot is occupied
// there's going to be a problem
static const s_int16 MAX_TASKS = 55;

// The various phases a task can have
static const u_int8 PHASE_PATHFINDING = 1;
static const u_int8 PHASE_MOVING = 2;
static const u_int8 PHASE_FINISHED = 4;
static const u_int8 PHASE_FAILED = 8;
static const u_int8 PHASE_PAUSED = 128;

// ctor
pathfinding_manager::pathfinding_manager()
{
    m_task.reserve(MAX_TASKS);
    m_locked.reserve(MAX_TASKS);

    for (u_int16 i = 0; i < MAX_TASKS; i++)
    {
        m_task.push_back(new world::pathfinding_task);
        m_locked.push_back(false);
    }

    m_taskHighest = 0;
}

// dtor
pathfinding_manager::~pathfinding_manager()
{
    clear ();

    m_task.clear();
    m_locked.clear();
}

// reset to initial state
void pathfinding_manager::clear ()
{
    for (u_int16 i = 0; i < (u_int16)m_task.size() && i < MAX_TASKS; i++)
    {
        delete m_task[i]->callback;
        m_task[i]->callback = NULL;
    }

    m_taskHighest = 0;
    m_chars.clear();

    m_locked.assign(MAX_TASKS, false);
}

s_int16 pathfinding_manager::find_free_task()
{
    for (u_int16 i = 0; i < MAX_TASKS; i++)
    {
        if (m_locked[i] == false)
            return i;
    }

    return -1;
}

bool pathfinding_manager::add_task_ll(const s_int16 id, character * chr,
                                      const world::vector3<s_int32> & target,
                                      const world::vector3<s_int32> & target2,
                                      const character::direction & finalDir)
{
    // Verify if we can indeed add this task, or if the character is already performing another task
    slist<character *>::iterator ichr = find(m_chars.begin(), m_chars.end(), chr);
    if (ichr == m_chars.end())
    {
        // delete any previously set callback
        delete m_task[id]->callback;

        m_task[id]->chr = chr;
        m_task[id]->target = target;
        m_task[id]->target2 = target2;
        m_task[id]->callback = NULL;
        m_task[id]->phase = PHASE_PATHFINDING;
        m_task[id]->actualNode = 0;
        m_task[id]->actualDir = character::NONE;
        m_task[id]->finalDir = finalDir;
        m_task[id]->path.clear();
        m_task[id]->lastPos.set_x(m_task[id]->chr->x());
        m_task[id]->lastPos.set_y(m_task[id]->chr->y());
        m_task[id]->lastPos.set_z(m_task[id]->chr->z());
        m_task[id]->iterations = m_task[id]->m_pathfinding.init (chr, target, target2);

        m_locked[id] = true;
        m_chars.push_front(chr);

        m_taskHighest = std::max(id, m_taskHighest); // Keep track of highest slot in use
        return m_task[id]->iterations > 0;
    }
    else
    {
        LOG(WARNING) << "Character " << chr->uid() << " already has an active pathfinding task.";
        return false;
    }
}

s_int16 pathfinding_manager::add_task(character * chr, const vector3<s_int32> & target, const character::direction & finalDir)
{
    const s_int16 id = find_free_task();
    if ((id == -1) || (add_task_ll(id, chr, target, target, finalDir) == -1))
        return -1;

    return id;
}

s_int16 pathfinding_manager::add_task(character * chr, const vector3<s_int32> & target1,
                                      const world::vector3<s_int32> & target2,
                                      const character::direction & finalDir)
{
    const s_int16 id = find_free_task();
    if ((id == -1) || (add_task_ll(id, chr, target1, target2, finalDir) == false))
        return -1;

    return id;
}

s_int16 pathfinding_manager::add_task(character * chr, character * target, const character::direction & finalDir)
{
    world::vector3<s_int32> tempTarget1(((target->x()/20)-1)*20, ((target->y()/20)-1)*20, target->z());
    world::vector3<s_int32> tempTarget2(((target->x()/20)+1)*20, ((target->y()/20)+1)*20, target->z());

    const s_int16 id = find_free_task();
    if ((id == -1) || (add_task_ll(id, chr, tempTarget1, tempTarget2, finalDir) == false))
        return -1;

    return id;
}

s_int16 pathfinding_manager::add_task(character * chr, const std::string & name, const character::direction & finalDir)
{
    zone * tempZone = chr->map().get_zone(name);

    if (tempZone == NULL)
        return -1;

    const s_int16 id = find_free_task();
    if ((id == -1) || (add_task_ll(id, chr, tempZone->min(), tempZone->max(), finalDir) == false))
        return -1;

    return id;
}

void pathfinding_manager::set_callback (const s_int16 & id, base::functor_1<const s_int32> * callback)
{
    // get rid of previously set callback, if any
    delete m_task[id]->callback;
    m_task[id]->callback = callback;
}

void pathfinding_manager::set_final_direction(const s_int16 & id, const character::direction finalDir)
{
    m_task[id]->finalDir = static_cast<u_int8>(finalDir);
}

void pathfinding_manager::pause_task(const s_int16 & id)
{
    m_task[id]->phase |= PHASE_PAUSED;
}

void pathfinding_manager::resume_task(const s_int16 & id)
{
    m_task[id]->phase &= ~PHASE_PAUSED;
}

bool pathfinding_manager::delete_task(const s_int16 & id)
{
    // Deletion consists of pausing the task, unlocking it and popping out the character from the list
    if ((id >= MAX_TASKS) || (m_locked[id] == false))
        return false;

    pause_task(id);
    m_locked[id] = false;

    slist<character *>::iterator ichr = find(m_chars.begin(), m_chars.end(), m_task[id]->chr);

    if (ichr == m_chars.end())
    {
        LOG(WARNING) << "Can't find " << m_task[id]->chr->uid() << " when deleting task!";
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

// get pathfinding status of given task
pathfinding_manager::state pathfinding_manager::return_state(const s_int16 & id)
{
    switch(m_task[id]->phase)
    {
        case PHASE_PATHFINDING:
        case PHASE_MOVING:
            return ACTIVE;
        case PHASE_FINISHED:
            return SUCCESS;
        default:
            return FAILURE;
    }
}

const world::pathfinding_task* pathfinding_manager::get_task(const s_int16 & id)
{
    if (id >= 0 && (u_int16)id < m_task.size())
    {
        return m_task[id];
    }
    return NULL;
}

void pathfinding_manager::update()
{
    for (s_int16 id = 0; id <= m_taskHighest; id++)
    {
        if (m_locked[id] == true)
        {
            switch(m_task[id]->phase)
            {
                case PHASE_PATHFINDING:
                {
                    // calculate the path
                    bool pathFound = m_task[id]->m_pathfinding.find_path(m_task[id]->chr, m_task[id]->target, m_task[id]->target2, &m_task[id]->path);
                    if (pathFound)
                    {
                        // used to check if we're stuck
                        m_task[id]->iterations = 5;
                        // Done calculating the path
                        m_task[id]->phase = PHASE_MOVING;
                    }
                    else if (m_task[id]->iterations <= 1)
                    {
                        // Resets the node cache, the open list and the node bank
                        m_task[id]->m_pathfinding.reset();
                        // Failed to find the path
                        m_task[id]->phase = PHASE_FAILED;
                    }

                    --m_task[id]->iterations;
                    break;
                }
                case PHASE_MOVING:
                {
                    if (move_chr(id) == true)
                    {
                        // Reached the goal
                        m_task[id]->phase = PHASE_FINISHED;
                    }
                    break;
                }
                case PHASE_FAILED: /* fall through */
                case PHASE_FINISHED:
                {
                    m_task[id]->actualDir = m_task[id]->finalDir;
                    m_task[id]->chr->set_direction(m_task[id]->finalDir);
                    m_task[id]->chr->stop();
                    m_task[id]->chr->update_state();

                    if (m_task[id]->callback != NULL)
                    {
                        (*m_task[id]->callback)((s_int32) return_state(id));
                    }

                    delete_task(id);
                    break;
                }
            }
        }
    }
}

bool pathfinding_manager::move_chr(const s_int16 id)
{
    // Check if we've finished the path
    if (m_task[id]->path.size() -1 <= m_task[id]->actualNode)
    {
        return true;
    }

    s_int32 center_x = m_task[id]->chr->x() + m_task[id]->chr->placeable::length() / 2;
    s_int32 center_y = m_task[id]->chr->y() + m_task[id]->chr->placeable::width() / 2;

    s_int32 grid_x = center_x / 20;
    s_int32 grid_y = center_y / 20;

    s_int32 target_grid_x = m_task[id]->path.at(m_task[id]->actualNode).x();
    s_int32 target_grid_y = m_task[id]->path.at(m_task[id]->actualNode).y();
    
    s_int32 target_x = target_grid_x * 20 + 10;
    s_int32 target_y = target_grid_y * 20 + 10;

    if (m_task[id]->actualDir == character::NONE)
    {
        // Calculate the next node
        if (center_y > target_y + 2)
        {
            // We have to move up
            m_task[id]->actualDir |= character::NORTH;
        }
        else if (center_y < target_y - 2)
        {
            // We have to move down
            m_task[id]->actualDir |= character::SOUTH;
        }

        if (center_x > target_x + 2)
        {
            // We have to move left
            m_task[id]->actualDir |= character::WEST;
        }
        else if (center_x < target_x - 2)
        {
            // We have to move right
            m_task[id]->actualDir |= character::EAST;
        }

        if (m_task[id]->actualDir != m_task[id]->chr->current_dir())
        {
            m_task[id]->chr->set_direction(m_task[id]->actualDir);
        }
    }
    else
    {
        // stop once we reached the center of the target node
        if ((m_task[id]->actualDir & character::SOUTH) == character::SOUTH)
        {
            if (center_y >= target_y)
                m_task[id]->actualDir ^= character::SOUTH;
        }
        else if ((m_task[id]->actualDir & character::NORTH) == character::NORTH)
        {
            if (center_y <= target_y)
                m_task[id]->actualDir ^= character::NORTH;
        }

        if ((m_task[id]->actualDir & character::EAST) == character::EAST)
        {
            if (center_x >= target_x)
                m_task[id]->actualDir ^= character::EAST;
        }
        else if ((m_task[id]->actualDir & character::WEST) == character::WEST)
        {
            if (center_x <= target_x)
                m_task[id]->actualDir ^= character::WEST;
        }

        if (m_task[id]->actualDir != m_task[id]->chr->current_dir())
        {
            if (m_task[id]->actualDir == character::NONE)
            {
                ++m_task[id]->actualNode;
            }
            else
            {
                m_task[id]->chr->set_direction(m_task[id]->actualDir);
            }
        }

        // Verify if we are stuck
        if (m_task[id]->lastPos == *m_task[id]->chr)
        {
            // need to be stuck for multiple frames before we take action.
            // otherwise slow characters moving less than one pixel per frame
            // might get in trouble
            if (--m_task[id]->iterations > 0) return false;

            // We are stuck, let's find another path
            // Stop the character
            m_task[id]->chr->set_direction(character::NONE);

            // find first element of the path that is not blocked
            world::coordinates pos = m_task[id]->path[m_task[id]->actualNode];
            while(!is_blocked(pos, m_task[id]->chr) && m_task[id]->actualNode < m_task[id]->path.size() - 1)
            {
                pos = m_task[id]->path[++m_task[id]->actualNode];
            }

            // remove path up to and including first unblocked node
            m_task[id]->path.erase(m_task[id]->path.begin(), m_task[id]->path.begin()+m_task[id]->actualNode + 1);
            if (m_task[id]->path.empty())
            {
                // blocked all the way to the goal
                m_task[id]->phase = PHASE_FAILED;
            }
            else
            {
                world::coordinates target(pos.x() * 20 + 10, pos.y() * 20 + 10, pos.z());

                // add new path search with first unblocked node as the goal
                m_task[id]->iterations = m_task[id]->m_pathfinding.init(m_task[id]->chr, target, target);
                m_task[id]->phase = PHASE_PATHFINDING;
                m_task[id]->target = target;
                m_task[id]->target2 = target;
                m_task[id]->actualNode = 0;
                m_task[id]->actualDir = character::NONE;
            }
        }
        else if (m_task[id]->path.at(m_task[id]->actualNode).z() - m_task[id]->chr->z() > 40)
        {
            // search completely new path towards the goal
            m_task[id]->path.clear();
            m_task[id]->iterations = m_task[id]->m_pathfinding.init(m_task[id]->chr, m_task[id]->target, m_task[id]->target2);

            m_task[id]->phase = PHASE_PATHFINDING;
            m_task[id]->actualNode = 0;
            m_task[id]->actualDir = character::NONE;
        }
        else
        {
            // Update lastPos with our actual position
            m_task[id]->iterations = 5;
            m_task[id]->lastPos = *m_task[id]->chr;
        }
    }
    
    return false;
}

bool pathfinding_manager::is_blocked(const world::coordinates & pos, world::character * chr) const
{
    // make sure character does not appear as obstacle during pathfinding
    bool is_solid = chr->is_solid();
    chr->set_solid (false);

    // Check if there is an obstacle in this node
    vector3<s_int32> min(pos.x() * 20 + 11 - chr->placeable::length()/2, pos.y() * 20 + 11 - chr->placeable::width()/2, pos.z() + 1);
    vector3<s_int32> max(pos.x() * 20 + 9 + chr->placeable::length()/2, pos.y() * 20 + 9 + chr->placeable::width()/2, pos.z() + chr->height() - 1);

    std::list<chunk_info *> collisions = chr->map().objects_in_bbox(min, max, world::OBJECT | world::CHARACTER);

    chr->set_solid(is_solid);
    return collisions.empty();
}

void pathfinding_manager::put_state(base::flat & file)
{
    base::flat record;
    base::flat taskBlock;

    for (s_int16 i = 0; i <= m_taskHighest; i++)
    {
        if (m_locked[i] == true)
        {
            std::string t = m_task[i]->chr->uid();

            if (t.empty() == false)
            {
                taskBlock.put_string("chrName", t);
                taskBlock.put_sint32("target_x", m_task[i]->target.x());
                taskBlock.put_sint32("target_y", m_task[i]->target.y());
                taskBlock.put_sint32("target2_x", m_task[i]->target2.x());
                taskBlock.put_sint32("target2_y", m_task[i]->target2.y());
                taskBlock.put_uint8("phase", m_task[i]->phase);
                taskBlock.put_uint8("aNode", m_task[i]->actualNode);
                taskBlock.put_uint8("aDir", m_task[i]->actualDir);
                taskBlock.put_uint8("finalDir", m_task[i]->finalDir);

                std::stringstream a;

                // Saves the path
                for (u_int16 c = 0; c < m_task[i]->path.size(); c++)
                {

                    a.clear();a.str("");
                    a << "node" << c << "x";
                    taskBlock.put_sint32(a.str(), m_task[i]->path.at(c).x());

                    a.clear();a.str("");
                    a << "node" << c << "y";
                    taskBlock.put_sint32(a.str(), m_task[i]->path.at(c).y());
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
        add_task_ll(i, tChr, tempTarget, tempTarget2, static_cast<character::direction>(finalDir));
        m_task[i]->phase = phase;
        m_task[i]->actualNode = aNode;
        m_task[i]->actualDir = aDir;

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
            m_task[i]->path.push_back(tempNode);
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
