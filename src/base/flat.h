/*
   $Id: flat.h,v 1.30 2009/04/25 13:17:49 ksterker Exp $

   Copyright (C) 2004/2006/2007 Kai Sterker <kaisterker@linuxgames.com>
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

#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "base/types.h"
#include "python/callback_support.h"

using std::string;

namespace base
{
    /**
     * This class can store basic data types in a way that helps to easily make
     * that data persistent or transfer it over a network for later retrieval.
     * As it can only cope with basic data types, objects must take care of saving
     * and restoring the data correctly.
     *
     * Internally, all data is stored in the byte order native to the CPU it runs on.
     * If external data with different byte order is encountered, it will be converted
     * to native byte order as neccessary.
     *
     * The data format is the following. First byte is the byte order, where 'L' 
     * represents "Little Endian" and 'B' represents "Big Endian". Then a list of 
     * blocks with the following structure follows:
     *
     * <pre>
     *
     * 0    n    n+1      n+2        n+6     n+size+6    total block length
     * +----+-----+--------+----------+----------+  
     * | id | \0  |  type  |   size   |   data   |
     * +----+-----+--------+----------+----------+
     *    n    1       1         4        size           field size
     *
     * </pre>
     *
     */
    class flat
    {
        public:
            /**
             * Data types that can be flattened.
             */
            typedef enum {
                T_UNKNOWN = -1, 
                T_BOOL = 0, 
                T_CHAR = 1, 
                T_UINT8 = 2, 
                T_SINT8 = 3, 
                T_UINT16 = 4,
                T_SINT16 = 5, 
                T_UINT32 = 6, 
                T_SINT32 = 7, 
                T_STRING = 8,
                T_FLOAT = 9, 
                T_DOUBLE = 10, 
                T_BLOB = 11, 
                T_FLAT = 12, 
                NBR_TYPES = 13
            } data_type;

#ifndef SWIG
		    /**
		     * Internal structure to store unflattened data
		     */
		    class data {
		        public:
		            char* Name;
		            data_type Type;
		            u_int32 Size;
		            char* Content;
                    
		            data *Next;
		                
		            ~data () { delete Next; }
		    };
#endif // SWIG
        
            /**
             * Create a new flattener.
             * @param size initial size of the internal buffer
             */
            flat (const u_int16 & size = 32);

#ifndef SWIG
            /**
             * Create a new flattener from contents of another one.
             * @param buffer buffer to copy into this object.
             * @param size length of the buffer.
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
                copy (f);
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
             * Reset flattener. This does not delete the current buffer, only the
             * decoded data.
             */
            void clear () {
                Size = 1;
                delete Data;
                Data = NULL;
                Ptr = Buffer + 1;
                Success = true;
            }
            
            /**
             * @name Member Access
             */
            //@{            
            /**
             * Return size of data stored in this object.
             * @return length of flattened data in bytes.
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
            //@}
            
            /**
             * Calculate a checksum for the internal data.
             * @return Adler32 checksum for this object.
             */
            u_int32 checksum () const;
            
            /**
             * @name Methods to flatten data
             */
            //@{
            /**
             * Store a boolean value.
             * @param name id used to retrieve the value later on.
             * @param b value to store.
             */
            void put_bool (const string & name, const bool & b) {
                u_int8 sb = (u_int8) b;
                put (name, T_BOOL, 1, (void *) &sb);  
            }
            
            /**
             * Store a character value.
             * @param name id used to retrieve the value later on.
             * @param c value to store.
             */
            void put_char (const string & name, const char & c) {
                put (name, T_CHAR, 1, (void *) &c);
            }
            
            /**
             * Store 8 bit unsigned integer value.
             * @param name id used to retrieve the value later on.
             * @param i value to store.
             */
            void put_uint8 (const string & name, const u_int8 & i) {
                put (name, T_UINT8, 1, (void*) &i);
            }

            /**
             * Store 8 bit signed integer value.
             * @param name id used to retrieve the value later on.
             * @param i value to store.
             */
            void put_sint8 (const string & name, const s_int8 & i) {
                put (name, T_SINT8, 1, (void*) &i);
            }

            /**
             * Store 16 bit unsigned integer value.
             * @param name id used to retrieve the value later on.
             * @param i value to store.
             */
            void put_uint16 (const string & name, const u_int16 & i) {
                put (name, T_UINT16, 2, (void*) &i);
            }

            /**
             * Store 16 bit signed integer value.
             * @param name id used to retrieve the value later on.
             * @param i value to store.
             */
            void put_sint16 (const string & name, const s_int16 & i) {
                put (name, T_SINT16, 2, (void*) &i);
            }

            /**
             * Store 32 bit unsigned integer value.
             * @param name id used to retrieve the value later on.
             * @param i value to store.
             */
            void put_uint32 (const string & name, const u_int32 & i) {
                put (name, T_UINT32, 4, (void*) &i);
            }

            /**
             * Store 32 bit signed integer value.
             * @param name id used to retrieve the value later on.
             * @param i value to store.
             */
            void put_sint32 (const string & name, const s_int32 & i) {
                put (name, T_SINT32, 4, (void*) &i);
            }

            /**
             * Store character string.
             * @param name id used to retrieve the value later on.
             * @param s string to store.
             */
            void put_string (const string & name, const string & s) {
                put (name, T_STRING, s.length () + 1, s.c_str ());
            }
            
            /**
             * Store floating point value.
             * @param name id used to retrieve the value later on.
             * @param f value to store.
             */
            void put_float (const string & name, const float & f) {
                // store floats in a format that is compatible across platforms
                char buffer[16];
                snprintf (buffer, 15, "%.12f", f);
                put (name, T_FLOAT, strlen (buffer) + 1, buffer);
            }
            
            /**
             * Store double value.
             * @param name id used to retrieve the value later on.
             * @param d value to store.
             */
            void put_double (const string & name, const double & d) {
                // store doubles in a format that is compatible across platforms
                char buffer[32];
                snprintf (buffer, 31, "%.24g", d);
                put (name, T_DOUBLE, strlen (buffer) + 1, buffer);
            }

            /**
             * Store binary data of arbitrary length.
             * @param name id used to retrieve the value later on.
             * @param b binary data to store.
             * @param size length of data stored.
             */
            void put_block (const string & name, void *b, const u_int32 & size) {
                put (name, T_BLOB, size, b);
            }
            
            /**
             * Store another flat. Used to create nested structures for easier
             * and safer value retrieval.
             * @param name id used to retrieve the value later on.
             * @param out flat to store.
             */
            void put_flat (const string & name, const flat & out) {
                put (name, T_FLAT, out.size (), out.getBuffer ());
            }
            //@}
            
            /**
             * @name Methods to retrieve flattened data
             */
            //@{
            /**
             * Retrieve a boolean value previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b false on error.
             */
            bool get_bool (const string & name, bool optional = false) {
                data *d = get (name, T_BOOL, optional);
                if (d) return (bool) *((u_int8*) d->Content);
                else return false;
            }
            
            /**
             * Retrieve a character value previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b '\\0' on error.
             */
            char get_char (const string & name, bool optional = false) {
                data *d = get (name, T_CHAR, optional);
                if (d) return (char) *d->Content;
                else return '\0';
            }
            
            /**
             * Retrieve 8 bit unsigned integer previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b 0 on error.
             */
            u_int8 get_uint8 (const string & name, bool optional = false) {
                data *d = get (name, T_UINT8, optional);
                if (d) return *((u_int8*) d->Content);
                else return 0;
            }

            /**
             * Retrieve 8 bit signed integer previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b -1 on error.
             */
            s_int8 get_sint8 (const string & name, bool optional = false) {
                data *d = get (name, T_SINT8, optional);
                if (d) return *((s_int8*) d->Content);
                else return -1;
            }

            /**
             * Retrieve 16 bit unsigned integer previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b 0 on error.
             */
            u_int16 get_uint16 (const string & name, bool optional = false) {
                data *d = get (name, T_UINT16, optional);
                if (d) return *((u_int16*) d->Content);
                else return 0;
            }

            /**
             * Retrieve 16 bit signed integer previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b -1 on error.
             */
            s_int16 get_sint16 (const string & name, bool optional = false) {
                data *d = get (name, T_SINT16, optional);
                if (d) return *((s_int16*) d->Content);
                else return -1;
            }

            /**
             * Retrieve 32 bit unsigned integer previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b 0 on error.
             */
            u_int32 get_uint32 (const string & name, bool optional = false) {
                data *d = get (name, T_UINT32, optional);
                if (d) return *((u_int32*) d->Content);
                else return 0;
                
            }

            /**
             * Retrieve 32 bit signed integer previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b -1 on error.
             */
            s_int32 get_sint32 (const string & name, bool optional = false) {
                data *d = get (name, T_SINT32, optional);
                if (d) return *((s_int32*) d->Content);
                else return -1;
            }

            /**
             * Retrieve string previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b "" (empty string) on error.
             */
            string get_string (const string & name, bool optional = false) {
                data *d = get (name, T_STRING, optional);
                if (d) return string (d->Content);
                else return string ("");
            }
            
            /**
             * Retrieve floating point number previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b 0.0 on error.
             */
            float get_float (const string & name, bool optional = false) {
                data *d = get (name, T_FLOAT, optional);
                if (d) return (float) strtod (d->Content, NULL);
                else return 0.0;
            }

            /**
             * Retrieve double value previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b 0.0 on error.
             */
            double get_double (const string & name, bool optional = false) {
                data *d = get (name, T_DOUBLE, optional);
                if (d) return strtod (d->Content, NULL);
                else return 0.0;
            }

            /**
             * Retrieve binary data previously stored with given id. Call 
             * success() to check whether value retrieval was successful.
             * Don't forget to delete the returned value once you're done 
             * with it.
             * @param name id used to retrieve the value later on.
             * @param size will contain number of bytes returned.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b NULL on error.
             */
            void* get_block (const string & name, int *size = NULL, bool optional = false) {
                data *d = get (name, T_BLOB, optional);
                if (d) {
                    if (size != NULL) *size = d->Size;
                    u_int8 *ret = new u_int8[d->Size];
                    return memcpy (ret, d->Content, d->Size);
                }
                
                if (size != NULL) *size = 0;
                return NULL;
            }
            
            /**
             * Retrieve a nested flat with given id. Call 
             * success() to check whether value retrieval was successful.
             * @param name id used to retrieve the value later on.
             * @param optional whether to gracefully ignore missing data.
             * @return value stored or \b empty flat on error.
             */
            flat get_flat (const string & name, bool optional = false) {
                data *d = get (name, T_FLAT, optional);
                if (d) return flat (d->Content, d->Size);
                else return flat ();
            }
            
            /**
             * @name Iteration
             */
            /**
             * Reset iterator to start of record
             */
            void first ()
            {
                Decoded = Data;
            }
            
            /**
             * Retrieve the next value in stream.
             * @param value will contain a pointer to the value
             * @param size will contain the size of value
             * @param name will contain the name of the field
             * @return type of value or T_UNKNOWN on end of stream.
             */
            data_type next (void **value, u_int32 *size = NULL, char **name = NULL);
            //@}

	        /**
	         * @name Type/Name - mapping
	         */
	        //@{
	        /**
	         * Get the name of a type when giving the type code.
	         * @return name of a given type.
	         */
	        static const char* name_for_type (data_type t)
	        {
	        	if (t == T_UNKNOWN) return "eof"; 
	            return TypeName[t];
	        }
	        
	        /**
	         * Get the data type code when giving a certain type name.
	         * @return key code or T_STRING if no match found.
	         */
	        static const data_type type_for_name (const std::string & name)
	        {
	            for (int i = 0; i < NBR_TYPES; i++)
	                if (TypeName[i] == name)
	                    return (data_type) i;
	            
	            fprintf (stderr, "*** flat::type_for_name: unknown type '%s' encountered!\n", name.c_str());
	            return T_UNKNOWN;
	        }        
	        //@}

            /**
             * Convert contents of this object into a string. For debugging purposes.
             * @return string of buffer contents.
             */
            std::string to_string () const
            {
                u_int32 j = 0;
                static const char *Bin2Hex = "0123456789ABCDEF";
                char *hex = new char[(Size * 2) + 1];
                
                hex[Size * 2] = 0;
                
                for (u_int32 i = 0; i < Size; i++) 
                {
                    hex[j++] = Bin2Hex[(Buffer[i] >> 4) & 0x0f];
                    hex[j++] = Bin2Hex[Buffer[i] & 0x0f];
                }
                
                return std::string (hex);
            }

            /**
             * @name Endianness
             */
            //@{
            /**
             * Return endianness of data in the buffer.
             * @return 'L' for little endian data, 'B' for big endian data.
             */
            u_int8 byte_order () const
            {
                return Buffer[0];
            }
            //@}
            
#ifndef SWIG
            GET_TYPE_NAME_VIRTUAL(base::flat)
            /**
             * @name Copying Flats
             */
            //@{            
            /**
             * Return internal buffer of this flattener.
             * @return byte array containing the flattened data.
             */
            const char *getBuffer () const { return Buffer; }
            
            /**
             * Assign a new buffer with given size to this flattener.
             * @param buffer byte array containing flattened data.
             * @param size length of the byte array.
             */
            void setBuffer (char* buffer, const u_int32 & size) {
                delete[] Buffer;
                delete Data;

                Buffer = buffer;
                Capacity = size;
                Success = true;
                Ptr = Buffer + 1;
                Size = size;
                Data = NULL;
            }
#endif // SWIG
            
            /**
             * Copy the contents of given flat into this flat.
             * @param source flat whose contents to copy.
             */
            void copy (const flat & source);
	        //@}
            
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
            void put (const string & name, const data_type & type, const u_int32 & size, const void *data);
            
            /**
             * Reads data from the buffer. If data is not found in the buffer, an error message
             * is printed and the success flag will be set to false, unless the data is specially
             * marked as optional by passing true in the last method argument.
             *
             * @param name Identifier of data to retrieve
             * @param type Type of data to retrieve
             * @param optional Whether the data is optional.
             *
             * @return data structure filled with desired data, or NULL if data does not exist.
             */
            data* get (const string & name, const data_type & type, const bool & optional = false);
            
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
            
            /// names for datatypes
            static const char* TypeName[NBR_TYPES];
    };
}
#endif // BASE_FLAT
