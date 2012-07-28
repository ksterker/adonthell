/*
 Copyright (C) Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   world/vector3.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the 3D vector template.
 * 
 */

#ifndef WORLD_VECTOR_3_H
#define WORLD_VECTOR_3_H

#include <cmath>
#include <sstream>
#include <adonthell/base/flat.h>

namespace world
{
    
/**
 * Implements a point in 3D space.
 */
template<class T> class vector3 
{
public:
    /**
     * Create a 3D vector located at the origin
     */
    vector3 () : X (0), Y (0), Z (0)
    {
    }

	/**
	 * Create a new 3D vector.
	 * @param x x coordinate
	 * @param y y coordinate
	 * @param z z coordinate
	 */
	vector3 (const T & x, const T & y, const T & z) : X (x), Y (y), Z (z)
	{
	}

    /**
     * Create a new 3D vector as copy of existing vector
     * @param v the vector to copy
     */
    vector3 (const vector3 & v) : X (v.X), Y (v.Y), Z (v.Z)
    {
    }
    
    /**
     * Create a new 3D vector of floats from an existing vector of ints.
     * @param v the vector to copy
     */
    template<class P>
    vector3 (const vector3<P> & v) : X (v.x()), Y (v.y()), Z (v.z())
    {
    }
    
    /**
     * @name Member Access
     */
    //@{
    /**
     * Set the location of a vector.
     * @param x new x location
     * @param y new y location
     * @param z new z location
     */
    void set (const T & x, const T & y, const T & z)
    {
    	X = x;
    	Y = y;
    	Z = z;
    }

    /**
     * Set the value of the vector from a string in the format
     * "[x, y, z]".
     * @param vec string representing this vector.
     * @return true on success, false otherwise.
     */
    bool set_str (const std::string & vec)
    {
        char delim;
        
        std::stringstream in (vec);        
        in >> std::skipws >> delim >> X >> delim >> Y >> delim >> Z >> delim; 
        
        if (in.fail ())    
        {
            fprintf (stderr, "*** vector3::set: error parsing vector '%s'", vec.c_str());
            return false;
        }
        
        return true;        
    }
    
    /*
     * Set the x coordinate of a vector.
     * @param x new x location
     */
    void set_x (const T & x) { X = x; } 
 
    /*
     * Set the y coordinate of a vector.
     * @param y new y location
     */
    void set_y (const T & y) { Y = y; } 
    
    /*
     * Set the z coordinate of a vector.
     * @param z new z location
     */
    void set_z (const T & z) { Z = z; } 

    /**
     * Update length of the vector.
     * @param length the vectors new length.
     */
    void set_length (const T & length)
    {
        float l = length / this->length ();
        set (static_cast<T>(X * l), static_cast<T>(Y * l), static_cast<T>(Z * l));
    }
    
    /**
     * Return x coordinate
     * @return x coordinate
     */
    const T & x () const { return X; }
    
    /**
     * Return y coordinate
     * @return y coordinate
     */
    const T & y () const { return Y; }
    
    /**
     * Return z coordinate
     * @return z coordinate
     */
    const T & z () const { return Z; }
    //@}
    
    /**
     * @name Vector Comparison
     */
    //@{
	/**
	 * Compare two sets of vectors for equality.
	 * @param v vector to compare with this.
	 * @return true if all members are equal, false otherwise.
	 */
	bool operator == (const vector3 & v) const
	{
		return (X == v.X && Y == v.Y && Z == v.Z);
	}

	/**
	 * Compare two sets of vectors for difference.
	 * @param v vector to compare with this.
	 * @return true if at least one members differs, false otherwise.
	 */
	bool operator != (const vector3 & v) const
	{
		return (X != v.X || Y != v.Y || Z != v.Z);
	}
	//@}

    /**
     * @name Vector Arithmetic
     */
    //@{
	/**
	 * Return a new vector that is the difference of two given vectors.
	 * @return new vector
	 */
    template<class PT>
	vector3 operator - (const vector3<PT> & v) const
	{
		return vector3 (X - v.x(), Y - v.y(), Z - v.z());
	}

	/**
	 * Return a vector that is the sum of the two given vectors.
	 * @return new vector.
	 */
	vector3 operator + (const vector3 & v) const
	{
		return vector3 (X + v.X, Y + v.Y, Z + v.Z);
	}
	
	/**
	 * Calculate the cross product of two given vectors.
	 * @return vector representing the cross product of two vectors.
	 */
	vector3 operator * (const vector3 & v) const
	{
		return vector3 (Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X);
	}
	
    /**
     * Multiply vector with scalar.
     * @param scalar value to multiply vector with
     * @return new vector
     */
    template<class PT>
    vector3<PT> operator * (const PT & scalar) const
    {
        return vector3 (X * scalar, Y * scalar, Z * scalar);
    }
    
    /**
     * Calculate the dot product of two given vectors.
     * @return the dot product of the vectors.
     */
    template<class PT>
    T dot (const vector3<PT> & v) const
    {
        return (X * v.x() + Y * v.y() + Z * v.z());
    }
    
	/**
	 * Return a normalized copy of this vector.
	 * @return a vector3<float> as a normalized version of this vector.
	 */
	vector3<float> normalize () const
	{
        float scale = 1.0f / length ();
        return vector3<float> (X * scale, Y * scale, Z * scale);
    }
    
    /**
     * Return the squared length of the vector. It's computed by
     * taking the dot product with itself.
     * @return squared length of this vector.
     */
    T squared_length () const
    {
        return dot (*this);
    }
    
    /**
     * Return the length of this vector.
     * @return the length of this vector.
     */
    float length () const
    {
        return sqrt (squared_length ());
    }
	//@}
	
    /**
     * @name Loading / Saving
     */
    //@{
    /**
     * Save the vector to a stream. Implemented for vector3<s_int16>.
     * @param file stream to save vector to.
     * @param name name to save vector under.
     * @return \b true if saving successful, \b false otherwise.
     */
    bool put_state (base::flat & file, const std::string & name = "p3d") const
    {
        std::stringstream out (std::ios::out);
        out << "[" << X << ", " << Y << ", " << Z << "]";
        
        file.put_string (name, out.str());
        
        return true;
    }

    /**
     * Load vector from stream. Implemented for vector3<s_int16>.
     * @param file stream to load vecotr from.
     * @return \b true if loading successful, \b false otherwise.
     */
    bool get_state (base::flat & file)
    {
        std::string vec = file.get_string ("p3d");
        return set_str (vec);
    } 
    //@}

protected:
	/// X coordinate
	T X; 
	/// Y coordinate
	T Y;
	/// Z coordinate
	T Z;
};

template <class PT>
std::ostream& operator<< (std::ostream& out, const vector3<PT> & v )
{
    out << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]";
    return out;
}

}

#endif /* WORLD_VECTOR_3_H */
