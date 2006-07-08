/*
   $Id: keyboard_event.h,v 1.6 2006/07/08 17:04:03 ksterker Exp $

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   input/keyboard_event.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the keyboard_event class.
 * 
 * 
 */


#ifndef KEYBOARD_EVENT_H
#define KEYBOARD_EVENT_H

#include "input/event.h"

namespace input
{

    /**
     * Represents a keyboard event, that is a key identifier
     * and a status (KEY_PUSHED or KEY_RELEASED).
     * 
     */
    class keyboard_event : public event
    {
    public:

        /**
         * Types of keyboard events.
         * 
         */
        typedef enum
            {
                KEY_PUSHED, KEY_RELEASED
            } event_type; 

        /**
         * Keyboard keys enumeration.
         * 
         */
        typedef enum
            {
                UNKNOWN_KEY = 0,
                BACKSPACE_KEY,
                TAB_KEY,
                CLEAR_KEY,
                RETURN_KEY,
                PAUSE_KEY,
                ESCAPE_KEY,
                SPACE_KEY,
                EXCLAIM_KEY,
                DBLQUOTE_KEY,
                HASH_KEY,
                DOLLAR_KEY,
                AMPERSAND_KEY,
                QUOTE_KEY,
                LEFTPAREN_KEY,
                RIGHTPAREN_KEY,
                ASTERISK_KEY,
                PLUS_KEY,
                COMA_KEY,
                MINUS_KEY,
                PERIOD_KEY,
                SLASH_KEY,
                N0_KEY,
                N1_KEY,
                N2_KEY,
                N3_KEY,
                N4_KEY,
                N5_KEY,
                N6_KEY,
                N7_KEY,
                N8_KEY,
                N9_KEY,
                COLON_KEY,
                SEMICOLON_KEY,
                LESS_KEY,
                EQUALS_KEY,
                GREATER_KEY,
                QUESTION_KEY,
                AT_KEY,
                LEFTBRACKET_KEY,
                BACKSLASH_KEY,
                RIGHTBRACKET_KEY,
                CARET_KEY,
                UNDERSCORE_KEY,
                BACKQUOTE_KEY,
                A_KEY,
                B_KEY,
                C_KEY,
                D_KEY,
                E_KEY,
                F_KEY,
                G_KEY,
                H_KEY,
                I_KEY,
                J_KEY,
                K_KEY,
                L_KEY,
                M_KEY,
                N_KEY,
                O_KEY,
                P_KEY,
                Q_KEY,
                R_KEY,
                S_KEY,
                T_KEY,
                U_KEY,
                V_KEY,
                W_KEY,
                X_KEY,
                Y_KEY,
                Z_KEY,
                DELETE_KEY,
                WORLD_0_KEY,
                WORLD_1_KEY,
                WORLD_2_KEY,
                WORLD_3_KEY,
                WORLD_4_KEY,
                WORLD_5_KEY,
                WORLD_6_KEY,
                WORLD_7_KEY,
                WORLD_8_KEY,
                WORLD_9_KEY,
                WORLD_10_KEY,
                WORLD_11_KEY,
                WORLD_12_KEY,
                WORLD_13_KEY,
                WORLD_14_KEY,
                WORLD_15_KEY,
                WORLD_16_KEY,
                WORLD_17_KEY,
                WORLD_18_KEY,
                WORLD_19_KEY,
                WORLD_20_KEY,
                WORLD_21_KEY,
                WORLD_22_KEY,
                WORLD_23_KEY,
                WORLD_24_KEY,
                WORLD_25_KEY,
                WORLD_26_KEY,
                WORLD_27_KEY,
                WORLD_28_KEY,
                WORLD_29_KEY,
                WORLD_30_KEY,
                WORLD_31_KEY,
                WORLD_32_KEY,
                WORLD_33_KEY,
                WORLD_34_KEY,
                WORLD_35_KEY,
                WORLD_36_KEY,
                WORLD_37_KEY,
                WORLD_38_KEY,
                WORLD_39_KEY,
                WORLD_40_KEY,
                WORLD_41_KEY,
                WORLD_42_KEY,
                WORLD_43_KEY,
                WORLD_44_KEY,
                WORLD_45_KEY,
                WORLD_46_KEY,
                WORLD_47_KEY,
                WORLD_48_KEY,
                WORLD_49_KEY,
                WORLD_50_KEY,
                WORLD_51_KEY,
                WORLD_52_KEY,
                WORLD_53_KEY,
                WORLD_54_KEY,
                WORLD_55_KEY,
                WORLD_56_KEY,
                WORLD_57_KEY,
                WORLD_58_KEY,
                WORLD_59_KEY,
                WORLD_60_KEY,
                WORLD_61_KEY,
                WORLD_62_KEY,
                WORLD_63_KEY,
                WORLD_64_KEY,
                WORLD_65_KEY,
                WORLD_66_KEY,
                WORLD_67_KEY,
                WORLD_68_KEY,
                WORLD_69_KEY,
                WORLD_70_KEY,
                WORLD_71_KEY,
                WORLD_72_KEY,
                WORLD_73_KEY,
                WORLD_74_KEY,
                WORLD_75_KEY,
                WORLD_76_KEY,
                WORLD_77_KEY,
                WORLD_78_KEY,
                WORLD_79_KEY,
                WORLD_80_KEY,
                WORLD_81_KEY,
                WORLD_82_KEY,
                WORLD_83_KEY,
                WORLD_84_KEY,
                WORLD_85_KEY,
                WORLD_86_KEY,
                WORLD_87_KEY,
                WORLD_88_KEY,
                WORLD_89_KEY,
                WORLD_90_KEY,
                WORLD_91_KEY,
                WORLD_92_KEY,
                WORLD_93_KEY,
                WORLD_94_KEY,
                WORLD_95_KEY,    
                KP0_KEY,
                KP1_KEY,
                KP2_KEY,
                KP3_KEY,
                KP4_KEY,
                KP5_KEY,
                KP6_KEY,
                KP7_KEY,
                KP8_KEY,
                KP9_KEY,
                KP_PERIOD_KEY,
                KP_DIVIDE_KEY,
                KP_MULTIPLY_KEY,
                KP_MINUS_KEY,
                KP_PLUS_KEY,
                KP_ENTER_KEY,
                KP_EQUALS_KEY,
                UP_KEY,
                DOWN_KEY,
                RIGHT_KEY,
                LEFT_KEY,
                INSERT_KEY,
                HOME_KEY,
                END_KEY,
                PAGEUP_KEY,
                PAGEDOWN_KEY,
                F1_KEY,
                F2_KEY,
                F3_KEY,
                F4_KEY,
                F5_KEY,
                F6_KEY,
                F7_KEY,
                F8_KEY,
                F9_KEY,
                F10_KEY,
                F11_KEY,
                F12_KEY,
                F13_KEY,
                F14_KEY,
                F15_KEY,
                NUMLOCK_KEY,
                CAPSLOCK_KEY,
                SCROLLLOCK_KEY,
                RSHIFT_KEY,
                LSHIFT_KEY,
                RCTRL_KEY,
                LCTRL_KEY,
                RALT_KEY,
                LALT_KEY,
                RMETA_KEY,
                LMETA_KEY,
                LSUPER_KEY,
                RSUPER_KEY,
                MODE_KEY,
                COMPOSE_KEY,
                HELP_KEY,
                PRINT_KEY,
                SYSREQ_KEY,
                BREAK_KEY,
                MENU_KEY,
                POWER_KEY,
                EURO_KEY,        
                NBR_KEYS
            } key_type; 
    
        /** 
         * Constructor.
         * 
         * @param t kind of keyboard event (KEY_PUSHED or KEY_RELEASED)
         * @param k key concerned by this event.
         * @param u unicode character this event produced.
         *
         */
        keyboard_event (event_type t, key_type k, u_int16 u);

        /**
         * @name Member access
         */
        //@{
        /** 
         * Returns the type of this event.
         * 
         * 
         * @return type of this keyboard_event.
         */    
        event_type type () const
        {
            return Type; 
        }

        /** 
         * Returns the key concerned by this event.
         * 
         * 
         * @return key concerned by this event.
         */
        key_type key () const
        {
            return Key; 
        }

        /** 
         * Returns a reference to a string containing the symbol
         * of the key concerned by this event.
         * 
         * 
         * @return symbol of the key concerned by this event.
         */
        const std::string & key_symbol () const;

        /**
         * Return the unicode value of the key concerned by this event.
         * @return unicode key code.
         */
        u_int16 unikey () const
        {
            return Unikey;
        }
        //@}
    
        /**
         * @name Key/Name - mapping
         */
        //@{
        /**
         * Get the name of a key when giving the ASCII code.
         * @return name of a given key.
         */
        static const std::string & name_for_key (key_type key)
        {
            return Key_symbol[key];
        }
        
        /**
         * Get the ASCII code when giving a certain key name.
         * @return key code or UNKNOWN_KEY if no match found.
         */
        static const key_type key_for_name (const std::string & name)
        {
            for (int i = 0; i < NBR_KEYS; i++)
                if (Key_symbol[i] == name)
                    return (key_type) i;
            
            return UNKNOWN_KEY;
        }        
        //@}

#ifndef SWIG
        GET_TYPE_NAME(input::keyboard_event)
#endif // SWIG

    private:
        /// mapping of key types to strings
        static std::string Key_symbol[NBR_KEYS];
        /// type of event
        const event_type Type;
        /// key code (ASCII)
        const key_type Key; 
        /// key code (Unicode)
        const u_int16 Unikey;
    }; 
}

#endif
