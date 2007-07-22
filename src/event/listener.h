/*
   $Id: listener.h,v 1.11 2007/07/22 21:50:37 ksterker Exp $

   Copyright (C) 2004/2005 Kai Sterker <kaisterker@linuxgames.com>
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
 * @brief  Declares the %listener base class.
 * 
 */

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <Python.h>
#include "event/event.h"
#include "base/callback.h"

namespace events
{
    /** Types of listeners */
    enum
    {
        LISTENER_CXX    = 0,
        LISTENER_PYTHON = 1
    };

    class factory;

    /**
     * %Listeners are usually created by a certain %event %factory and will
     * be destroyed if the %factory is destroyed. That way, it's not neccessary
     * to keep track of every single %listener. Instead, a few factories can
     * be used to create groups of listeners that can be disposed together.
     */
    class listener
    {
    public:
        /**
         * Create a new listener, which listens to the given event.
         * @param f the factory that created the listener or NULL
         * @param e the event that will trigger the attached callback
         */
        listener (factory *f, event *e);
        
        /**
         * Destroy the %listener. Automatically removes it from 
         * the %event %manager and %factory if required.
         */
        virtual ~listener ();
        
        /**
         * @name Member access
         */
        //@{
        /**
         * Get type of associated %event.
         * @return type of associated %event
         */
        u_int8 type () const
        { 
            return Event->type ();
        }
        
        /**
         * Get the %event this 5listener waits for.
         * @return %event attached to the %listener.
         */
        const event * get_event () const
        {
            return Event;
        }
        
        /**
         * Get the listener's id.
         * @return id of the %listener.
         */
        const string & id () const
        {
            return Id;
        }
        
        /**
         * Assign an id to the %listener, so it may be retrieved from
         * a %factory later on, without having a pointer to it.
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
         * @name Event handling
         */
        //@{  
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
        virtual bool connect_callback (const string & file, const string & classname, 
            const string & callback, PyObject *args = NULL) = 0;

#ifndef SWIG
        /**
         * Sets a C++ method to be executed whenever an
         * %event occurs that this listener listens to.
         *
         * @param callback The method to call.
         */
        virtual void connect_callback (base::functor_0 * callback) = 0;
#endif // SWIG

        /**
         * Execute the associated python script or callback.
         * 
         * @param evnt The %event that triggered the execution.
         * @return The number of times the %event needs to be repeated.
         */ 
        virtual s_int32 raise_event (const event* evnt) = 0;

        /** 
         * Check whether the given %event matches the %event attached to
         * the %listener.
         * @param e %event to compare with the attached %event.
         * @return \b true if events compare equal, \b false otherwise.
         */
        bool equals (const event *e) const
        {
            return Event->equals (e);
        }
        //@}
        
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
        virtual void put_state (base::flat& out) const;
        
        /** 
         * Loads the %event %listener from given stream.
         *
         * @param in stream to load the %event %listener from.
         */
        virtual bool get_state (base::flat& in);
        //@}

#ifndef SWIG
        /**
         * Allow %listener to be passed as python argument
         */
        GET_TYPE_NAME_VIRTUAL (events::listener)
#endif // SWIG

    protected:
        /**
         * The %event this %listener will react to.
         */
        event *Event;
        
        /**
         * (Optional) Id of the event
         */
        string Id;
        
    private:
        /**
         * Whether the %listener is registered with the %event %manager
         */
        bool Registered;
        
        /**
         * Whether the %listener is temporarily disabled or not
         */
        u_int16 Paused;
        
        /**
         * The %event %factory that created this listener.
         */
        factory *Factory;
    };
}

#endif // EVENT_LISTENER_H
