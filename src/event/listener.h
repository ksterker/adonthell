/*
   $Id: listener.h,v 1.2 2004/04/29 08:07:49 ksterker Exp $

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
 * @file   event/listener.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the %listener class.
 * 
 */

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "python/method.h"
#include "event/event.h"

#ifndef SWIG
/**
 * Pointer to a function returning a newly allocated %event
 */
typedef event::event* (*new_event)();
#endif // SWIG

namespace event
{
    class factory;

    /**
     *
     */
    class listener
    {
    public:
        /**
         * Create a new listener, which listens to the given event.
         */
        listener (factory *f, event *e); 
        
        /**
         * Destroy the %listener. Automatically removes it from 
         * the %event %manager and factory if required.
         */
        ~listener ();
        
        /**
         * @name Member access
         */
        //@{
        /**
         * Get type of associated %event.
         *
         * @return type of associated %event
         */
        u_int8 type () const
        { 
            return Event->type ();
        }
        
        const event * get_event () const
        {
            return Event;
        }
        
        /**
         * Get the event's id.
         *
         * @return id of the %event.
         */
        const string & id () const
        {
            return Id;
        }
        
        /**
         * Assign an id to the %event, so it may be retrieved from an
         * event_list later on, without having a pointer to it.
         *
         * @param id a string to identify the %event.
         */
        void set_id (const string & id)
        {
            Id = id;
        }

        /**
         * Deleting a %listener from Python side can cause problems.
         * Instead, it should be marked as deletable with this method.
         * It will then be deleted on C++ side as soon as possible.
         */
        void destroy ()
        {
            Event->set_repeat (0);
        }
        
        /**
         * Check whether the %listener is marked for deletion.
         */
        bool is_destroyed () const 
        {
            return Event->repeat () == 0;
        }
        
// these are internal methods that shouldn't be exported by SWIG!
#ifndef SWIG
        /**
         * Return whether the %listener is registered with the %event %manager.
         * @return \c true if this is the case, \c false otherwise.
         */
        bool is_registered () const
        {
            return Registered;
        }
        
        /**
         * This method is called by the %event %manager when the %listerner
         * is added to or removed from it. If the %listener is still added
         * when it is being destroyed, it will be automatically removed from
         * the %manager.
         *
         * @param state pass \c true if %listener is being connected, \c false otherwise 
         */
        void set_registered (bool state)
        {
            Registered = state;
        }
#endif // SWIG
        //@}
        
        /**
         * Sets a python method to be executed whenever an
         * %event occurs that this listener listens to.
         *
         * @param file Name of the script to load.
         * @param classname Name of the class containing the callback.
         * @param callback Name of the method to call.
         * @param args Additional arguments to pass to the callback.
         * @return \b false if connecting the callback failed, \b true otherwise.
         */
        bool connect_callback (const string & file, const string & classname, 
            const string & callback, PyObject *args = NULL);
    
        /**
         * Execute the associated python script or callback.
         * 
         * @param evnt The %event that triggered the execution.
         * @return The number of times the %event needs to be repeated.
         */ 
        s_int32 raise_event (const event* evnt);

        /** 
         *
         *
         */
        bool equals (const event *e) const
        {
            return Event->equals (e);
        }
        
        /**
         * @name Pausing / Resuming execution
         */
        //@{        
        /**
         * Disable the %event temporarily. As long as it in this state, the
         * event will neither be executed, nor will its repeat-count change.
         * As long as the %event is paused, it will be removed from its
         * %event handler.
         */
        void pause (const u_int16 & level = 1);
        
        /**
         * Re-enable an %event that has been paused. Re-registers it with
         * its %event handler.
         */
        void resume ();
    
        /**
         * Check whether the %event is temporarily disabled or not.
         * @return \b true if it is paused, \b false otherwise.                     
         */
        bool is_paused () const
        {
            return (Paused > 0);
        }
        //@}

        /**
         * @name Loading / Saving
         */
        //@{
        /** 
         * Flattens the %event %listener to the given stream.
         * 
         * @param out stream where to add the %event %listener.
         */ 
        void put_state (base::flat& out) const;
        
        /** 
         * Loads the %event %listener from given stream.
         *
         * @param in stream to load the %event %listener from.
         */
        bool get_state (base::flat& in);
        //@}

#ifndef SWIG
        /**
         * Register an %event for loading. Before the %event %factory can load
         * an %event from file, it needs a callback function that returns
         * a new instance of the %event of the given type.
         *
         * @param type the type of the %event to register
         * @param e a callback returning a new instance of an %event of the 
         *      given type.
         *
         * @sa get_state ()
         */
        static void register_event (u_int8 type, new_event e);
        
        /**
         * Allow %listener to be passed as python argument
         */
        GET_TYPE_NAME(event::listener)
#endif // SWIG

    private:
        /**
         * (Optional) Id of the event
         */
        string Id;
        
        /**
         * Whether the %listener is registered with the %event %manager
         */
        bool Registered;
        
        /**
         * Whether the %listener is temporarily disabled or not
         */
        u_int16 Paused;
        
        /**
         * The callback connected to this %listener
         */
        python::method *Method;
        
        /**
         * Arguments to pass to the method
         */
        PyObject *Args;
        
        /**
         * The %event this %listener will react to.
         */
        event *Event;
        
        /**
         * The %event %factory that created this listener.
         */
        factory *Factory;
        
        /**
         * Array with callbacks that return a newly allocated instance of an %event.
         * The event's type is the postion of the according callback in the array.
         */
        static new_event instanciate_event[MAX_EVENTS];
    };
}

#ifndef SWIG
/**
 * Registers an %event with the %event %factory, allowing it to load this %event
 * without knowing about it at compile time.
 */
#define REGISTER_EVENT(type,evt)\
    listener::register_event (type, (new_event) &new_ ## evt);
    
/**
 * A function that returns a new instance of an %event.
 */
#define NEW_EVENT(evt)\
    event::event* new_ ## evt () { return (event::event*) new event::evt; }

#endif // SWIG
#endif // EVENT_LISTENER_H
