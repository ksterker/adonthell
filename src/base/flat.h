/*
   $Id: flat.h,v 1.7 2004/06/27 11:20:57 ksterker Exp $

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
 * @file base/flat.h 
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief An internal class used to flatten various data.
 */

#ifndef BASE_FLAT
#define BASE_FLAT

#include <stdio.h>
#include "base/types.h"
#include <string>
#include "base/endian.h"
#include "python/callback_support.h"

using std::string;

namespace base
{
    /**
     *
     */
    class flat
    {
        /**
         * Internal structure to store unflattened data
         */
        class data {
            public:
                char* Name;
                u_int8 Type;
                u_int32 Size;
                char* Content;
                    
                data *Next;
                    
                ~data () { delete Next; }
        };

        public:
        /**
         * Data types that can be flattened.
         */
            enum {
                T_BOOL, T_CHAR, T_UINT8, T_SINT8, T_UINT16,
                T_SINT16, T_UINT32, T_SINT32, T_STRING,
                T_FLOAT, T_DOUBLE, T_BLOB, T_FLAT
            };
        
            /**
             * Create a new flattener.
             * @param size initial size of the internal buffer
             */
            flat (const u_int16 & size = 32);

#ifndef SWIG
            /**
             * Create a new flattener. A copy of the given buffer
             * will be made.
             * @param buffer a buffer with flattened data.
             * @param size size of buffer in bytes
             */
            flat (const char *buffer, const u_int32 & size);

            /**
             * Copy constructor.
             * @param f another %flat to copy into this object.
             */
            flat (const flat & f);
            
            /**
             * assignment operator
             * @param f another &flat to be assigned to this object.
             */
            void operator= (const flat & f)
            {
                char *tmp = new char[f.size ()];
                memcpy (tmp, f.getBuffer (), f.size ());
                setBuffer (tmp, f.size ());
            }
#endif // SWIG            

            /**
             * Destructor
             */
            virtual ~flat () {
                delete[] Buffer;
                delete Data;
            }

            /**
             * Reset flattener.
             */
            void clear () {
                Size = 0;
                delete Data;
                Data = NULL;
                Ptr = Buffer;
                Success = true;
            }
            
            /**
             * Return size of data stored in this object.
             * @param length of flattened data in bytes.
             */
            u_int32 size () const {
                return Size;
            }
            
            /**
             * Check whether the last operation was successful.
             * @return \b false if an error occured, \b true otherwise.
             */
            bool success () const {
                return Success;
            }
            
            /**
             * Calculate a checksum for the internal data.
             * @return Adler32 checksum for this object.
             */
            u_int32 checksum () const;
            
            /**
             * @name Methods to flatten data
             */
            //@{
            void put_bool (const string & name, const bool & b) {
                char sb = (u_int8) b;
                put (name, T_BOOL, 1, (void *) &sb);  
            }
            
            void put_char (const string & name, const char & c) {
                put (name, T_CHAR, 1, (void *) &c);
            }
            
            void put_uint8 (const string & name, const u_int8 & i) {
                put (name, T_UINT8, 1, (void*) &i);
            }

            void put_sint8 (const string & name, const s_int8 & i) {
                put (name, T_SINT8, 1, (void*) &i);
            }

            void put_uint16 (const string & name, const u_int16 & i) {
	        SwapLE16 (i);
                put (name, T_UINT16, 2, (void*) &i);
            }

            void put_sint16 (const string & name, const s_int16 & i) {
	        SwapLE16 (i);
                put (name, T_SINT16, 2, (void*) &i);
            }

            void put_uint32 (const string & name, const u_int32 & i) {
	        SwapLE32 (i);
                put (name, T_UINT32, 4, (void*) &i);
            }

            void put_sint32 (const string & name, const s_int32 & i) {
	        SwapLE32 (i);
                put (name, T_SINT32, 4, (void*) &i);
            }

            void put_string (const string & name, const string & s) {
                put (name, T_STRING, s.length () + 1, s.c_str ());
            }
            
            void put_float (const string & name, const float & f) {
                // store floats in a format that is compatible across platforms
                char buffer[32];
                snprintf (buffer, 31, "%.24g", f);
                put (name, T_FLOAT, strlen (buffer) + 1, buffer);
            }
            
            void put_double (const string & name, const double & d) {
                // store doubles in a format that is compatible across platforms
                char buffer[64];
                snprintf (buffer, 63, "%.48g", d);
                put (name, T_DOUBLE, strlen (buffer) + 1, buffer);
            }

            void put_block (const string & name, void *b, const u_int32 & size) {
                put (name, T_BLOB, size, b);
            }
            
            void put_flat (const string & name, const flat & out) {
                put (name, T_FLAT, out.size (), out.getBuffer ());
            }
            //@}
            
            /**
             * @name Methods to retrieve flattened data
             */
            //@{
            bool get_bool (const string & name) {
                data *d = get (name, T_BOOL);
                if (d) return (bool) *((u_int8*) d->Content);
                else return false;
            }
            
            char get_char (const string & name) {
                data *d = get (name, T_CHAR);
                if (d) return (char) *d->Content;
                else return '\0';
            }
            
            u_int8 get_uint8 (const string & name) {
                data *d = get (name, T_UINT8);
                if (d) return *((u_int8*) d->Content);
                else return 0;
            }

            s_int8 get_sint8 (const string & name) {
                data *d = get (name, T_SINT8);
                if (d) return *((s_int8*) d->Content);
                else return -1;
            }

            u_int16 get_uint16 (const string & name) {
                data *d = get (name, T_UINT16);
                if (d) return SwapLE16 (*((u_int16*) d->Content));
                else return 0;
            }

            s_int16 get_sint16 (const string & name) {
                data *d = get (name, T_SINT16);
                if (d) return SwapLE16 (*((s_int16*) d->Content));
                else return -1;
            }

            u_int32 get_uint32 (const string & name) {
                data *d = get (name, T_UINT32);
                if (d) return SwapLE32 (*((u_int32*) d->Content));
                else return 0;
                
            }

            s_int32 get_sint32 (const string & name) {
                data *d = get (name, T_SINT32);
                if (d) return SwapLE32 (*((s_int32*) d->Content));
                else return -1;
            }

            string get_string (const string & name) {
                data *d = get (name, T_STRING);
                if (d) return string (d->Content);
                else return string ("");
            }
            
            float get_float (const string & name) {
                data *d = get (name, T_FLOAT);
                if (d) return (float) strtod (d->Content, NULL);
                else return 0.0;
            }

            double get_double (const string & name) {
                data *d = get (name, T_DOUBLE);
                if (d) return strtod (d->Content, NULL);
                else return 0.0;
            }

            void* get_block (const string & name, int *size = NULL) {
                data *d = get (name, T_BLOB);
                if (d) {
                    if (size != NULL) *size = d->Size;
                    char *ret = new char[d->Size];
                    return memcpy (ret, d->Content, d->Size);
                }
                
                if (size != NULL) *size = 0;
                return NULL;
            }
            
            flat get_flat (const string & name) {
                data *d = get (name, T_FLAT);
                if (d) return flat (d->Content, d->Size);
                else return flat ();
            }
            
            /**
             * Retrieve the next value in stream.
             * @param value will contain a pointer to the value
             * @param size will contain the size of value
             * @param name will contain the name of the field
             * @return type of value fetched.
             */
            int next (void **value, int *size = NULL, char **name = NULL);
            //@}

#ifndef SWIG
            GET_TYPE_NAME_VIRTUAL(base::flat)
#endif // SWIG
        protected:
            const char *getBuffer () const { return Buffer; }
            
            void setBuffer (char* buffer, const u_int32 & size) {
                delete[] Buffer;
                delete Data;

                Buffer = buffer;
                Capacity = size;
                Success = true;
                Ptr = Buffer;
                Size = size;
                Data = NULL;
            }
            
        private:
            /// Pointer to unflattened data. Valid after first call to parse().
            data *Data;
            
            /// Points to record last fetched with get_*() or next()
            data *Decoded;
            
            /**
             * Writes data into the buffer, growing it if neccessary.
             * @param name Identifier for the data
             * @param type Type of the data
             * @param size Size of the data
             * @param data the data to add to the buffer.
             */
            void put (const string & name, const u_int8 & type, const u_int32 & size, const void *data);
            
            /**
             * Reads data from the buffer.
             * @param name Identifier of data to retrieve
             * @param type Type of data to retrieve
             * @return data structure filled with desired data, or NULL if data does not exist.
             */
            data* get (const string & name, const u_int8 & type);
            
            /**
             * Unflatten the internal buffer for easier data retrieval. 
             */
            void parse ();
            
            /**
             * Grow the internal buffer. This will double its current capacity.
             */
            void grow ();
            
            /// Buffer storing the flattened objects
            char *Buffer;
            
            /// Pointer to the current position in the Buffer
            char *Ptr;
            
            /// Bufferspace currently used
            u_int32 Size;
            
            /// Maximum capacity of the Buffer
            u_int32 Capacity;
            
            /// Indicates an error during get
            bool Success;
    };
}
#endif // BASE_FLAT
