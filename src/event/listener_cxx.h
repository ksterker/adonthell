/*
 $Id: listener_cxx.h,v 1.1 2006/06/18 19:25:53 ksterker Exp $
 
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
 * @file   event/listener_cxx.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares a %listener with python callback attached.
 * 
 */

#ifndef EVENT_LISTENER_CXX_H
#define EVENT_LISTENER_CXX_H

#include "event/listener.h"

namespace events
{
    /**
     * An %event %listener that contains a callback to a %python %method it
     * will execute when the event occurs that the %listener is waiting
     * for. A %listener is usually created by a certain %event %factory and will
     * be destroyed if the %factory is destroyed. That way, it's not neccessary
     * to keep track of every single %listener. Instead, a few factories can
     * be used to create groups of listeners that can be disposed together.
     */    
    class listener_cxx : public listener
    {
    public:
        /**
         * Create a new listener, which listens to the given event.
         * @param f the factory that created the listener or NULL
         * @param e the event that will trigger the attached callback
         */
        listener_cxx (factory *f, event *e);
        
        /**
         * Destroy the %listener. Automatically removes it from 
         * the %event %manager and %factory if required.
         */
        virtual ~listener_cxx ();

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
        bool connect_callback (const string & file, const string & classname, 
                               const string & callback, PyObject *args = NULL);
        
#ifndef SWIG
        /**
         * Sets a C++ method to be executed whenever an
         * %event occurs that this listener listens to.
         *
         * @param callback The method to call.
         */
        void connect_callback (base::functor_0 * callback);
#endif // SWIG
        
        /**
         * Execute the associated python script or callback.
         * 
         * @param evnt The %event that triggered the execution.
         * @return The number of times the %event needs to be repeated.
         */ 
        s_int32 raise_event (const event* evnt);
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
         * Allow %python %listener to be passed as python argument
         */
        GET_TYPE_NAME (events::listener_cxx);
#endif // SWIG
            
    private:
        /**
         * C++ callback that may be executed when the %event gets triggered.
         */
        base::functor_0 * Callback;
    };
}

#endif // EVENT_LISTENER_CXX_H
