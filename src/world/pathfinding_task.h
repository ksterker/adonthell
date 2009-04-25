#ifndef PATHFINDING_TASK_H
#define PATHFINDING_TASK_H

#include "base/callback.h"
#include "world/character.h"
#include "world/coordinates.h"

namespace world
{
    class pathfinding_task
    {
    public:
        /// The character being moved
        world::character * chr;
        /// Callback to execute when task completes/fails
        base::functor_1<s_int32> * callback;
        /// The target position (in pixels)
        world::vector3<s_int32> target;

        /// The path to the target, as a group of nodes
        std::vector<coordinates> * path;
        /// Used to calc the distance walked
        world::coordinates startPos;
        /// The phase where this task is
        u_int8 phase;
        /// The actual node being moved to
        u_int8 actualNode;
        /// Actual Direction
        u_int8 actualDir;
        /// Number of pixels moved beetween two nodes
        u_int8 pixelsMoved;
        /// Number of pixels moved in the last iteration. Used for dynamic collision detection
        u_int8 pixelsMovedLst;
        /// Number of pixels that have to be moved in order to reach the next node
        u_int8 pixelsToMove;
        /// Number of frames stuck
        u_int8 framesStuck;
        /// Number of consecutive times we've been stuck
        u_int8 timesStuck;
        /// Final Direction to where a character points after finishing moving
        u_int8 finalDir;
    };
}

#endif // PATHFINDING_TASK_H
