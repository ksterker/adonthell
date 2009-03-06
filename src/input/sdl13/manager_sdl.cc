/*
   $Id: manager_sdl.cc,v 1.2 2009/03/06 22:53:30 ksterker Exp $

   Copyright (C) 2002/2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   manager_sdl.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  SDL version of the input manager.
 * 
 * 
 */

#include "manager_sdl.h"

#ifdef USE_LIBTOOL
/* exported names for libltdl */
#define input_manager_update _sdl_LTX_input_manager_update
#endif

extern "C"
{
    void input_manager_update();
}

namespace input
{
    /**
     * Translation table for SDL key scancodes. See SDL_scancode.h.
     */
    keyboard_event::key_type sdl_key_trans[SDL_NUM_SCANCODES] = 
    {
        /*   0 */ keyboard_event::UNKNOWN_KEY,
        /*   1 */ keyboard_event::UNKNOWN_KEY,
        /*   2 */ keyboard_event::UNKNOWN_KEY,
        /*   3 */ keyboard_event::UNKNOWN_KEY,
        
        /*   4 */ keyboard_event::A_KEY,
        /*   5 */ keyboard_event::B_KEY,
        /*   6 */ keyboard_event::C_KEY,
        /*   7 */ keyboard_event::D_KEY,
        /*   8 */ keyboard_event::E_KEY,
        /*   9 */ keyboard_event::F_KEY,
        /*  10 */ keyboard_event::G_KEY,
        /*  11 */ keyboard_event::H_KEY,
        /*  12 */ keyboard_event::I_KEY,
        /*  13 */ keyboard_event::J_KEY,
        /*  14 */ keyboard_event::K_KEY,
        /*  15 */ keyboard_event::L_KEY,
        /*  16 */ keyboard_event::M_KEY,
        /*  17 */ keyboard_event::N_KEY,
        /*  18 */ keyboard_event::O_KEY,
        /*  19 */ keyboard_event::P_KEY,
        /*  20 */ keyboard_event::Q_KEY,
        /*  21 */ keyboard_event::R_KEY,
        /*  22 */ keyboard_event::S_KEY,
        /*  23 */ keyboard_event::T_KEY,
        /*  24 */ keyboard_event::U_KEY,
        /*  25 */ keyboard_event::V_KEY,
        /*  26 */ keyboard_event::W_KEY,
        /*  27 */ keyboard_event::X_KEY,
        /*  28 */ keyboard_event::Y_KEY,
        /*  29 */ keyboard_event::Z_KEY,
        
        /*  30 */ keyboard_event::N1_KEY,
        /*  31 */ keyboard_event::N2_KEY,
        /*  32 */ keyboard_event::N3_KEY,
        /*  33 */ keyboard_event::N4_KEY,
        /*  34 */ keyboard_event::N5_KEY,
        /*  35 */ keyboard_event::N6_KEY,
        /*  36 */ keyboard_event::N7_KEY,
        /*  37 */ keyboard_event::N8_KEY,
        /*  38 */ keyboard_event::N9_KEY,
        /*  39 */ keyboard_event::N0_KEY,
        
        /*  40 */ keyboard_event::RETURN_KEY,
        /*  41 */ keyboard_event::ESCAPE_KEY,
        /*  42 */ keyboard_event::BACKSPACE_KEY,
        /*  43 */ keyboard_event::TAB_KEY,
        /*  44 */ keyboard_event::SPACE_KEY,

        /*  45 */ keyboard_event::MINUS_KEY,
        /*  46 */ keyboard_event::EQUALS_KEY,
        /*  47 */ keyboard_event::LEFTBRACKET_KEY,
        /*  48 */ keyboard_event::RIGHTBRACKET_KEY,
        /*  49 */ keyboard_event::BACKSLASH_KEY,
        /*  50 */ keyboard_event::BACKSLASH_KEY,
        /*  51 */ keyboard_event::SEMICOLON_KEY,
        /*  52 */ keyboard_event::BACKQUOTE_KEY, /* Apostrophe */
        /*  53 */ keyboard_event::CARET_KEY, /* Grave*/
        /*  54 */ keyboard_event::COMA_KEY,
        /*  55 */ keyboard_event::PERIOD_KEY,
        /*  56 */ keyboard_event::SLASH_KEY,
        
        /*  57 */ keyboard_event::CAPSLOCK_KEY,
        
        /*  58 */ keyboard_event::F1_KEY,
        /*  59 */ keyboard_event::F2_KEY,
        /*  60 */ keyboard_event::F3_KEY,
        /*  61 */ keyboard_event::F4_KEY,
        /*  62 */ keyboard_event::F5_KEY,
        /*  63 */ keyboard_event::F6_KEY,
        /*  64 */ keyboard_event::F7_KEY,
        /*  65 */ keyboard_event::F8_KEY,
        /*  66 */ keyboard_event::F9_KEY,
        /*  67 */ keyboard_event::F10_KEY,
        /*  68 */ keyboard_event::F11_KEY,
        /*  69 */ keyboard_event::F12_KEY,
        
        /*  70 */ keyboard_event::SCROLLLOCK_KEY,
        /*  71 */ keyboard_event::PRINT_KEY,
        /*  72 */ keyboard_event::PAUSE_KEY,
        /*  73 */ keyboard_event::INSERT_KEY,
        /*  74 */ keyboard_event::HOME_KEY,
        /*  75 */ keyboard_event::PAGEUP_KEY,
        /*  76 */ keyboard_event::DELETE_KEY,
        /*  77 */ keyboard_event::END_KEY,
        /*  78 */ keyboard_event::PAGEDOWN_KEY,
        /*  79 */ keyboard_event::RIGHT_KEY,
        /*  80 */ keyboard_event::LEFT_KEY,
        /*  81 */ keyboard_event::DOWN_KEY,
        /*  82 */ keyboard_event::UP_KEY,
        
        /*  83 */ keyboard_event::NUMLOCK_KEY,
        /*  84 */ keyboard_event::KP_DIVIDE_KEY,
        /*  85 */ keyboard_event::KP_MULTIPLY_KEY,
        /*  86 */ keyboard_event::KP_MINUS_KEY,
        /*  87 */ keyboard_event::KP_PLUS_KEY,            
        /*  88 */ keyboard_event::KP_ENTER_KEY,
        /*  89 */ keyboard_event::KP1_KEY,
        /*  90 */ keyboard_event::KP2_KEY,
        /*  91 */ keyboard_event::KP3_KEY,
        /*  92 */ keyboard_event::KP4_KEY,
        /*  93 */ keyboard_event::KP5_KEY,
        /*  94 */ keyboard_event::KP6_KEY,
        /*  95 */ keyboard_event::KP7_KEY,
        /*  96 */ keyboard_event::KP8_KEY,
        /*  97 */ keyboard_event::KP9_KEY,
        /*  98 */ keyboard_event::KP0_KEY,
        /*  99 */ keyboard_event::KP_PERIOD_KEY,
        
        /* 100 */ keyboard_event::BACKSLASH_KEY, /* non-us backslash */
        /* 101 */ keyboard_event::COMPOSE_KEY,
        /* 102 */ keyboard_event::POWER_KEY,
        /* 103 */ keyboard_event::KP_EQUALS_KEY,
        /* 104 */ keyboard_event::F13_KEY,
        /* 105 */ keyboard_event::F14_KEY,
        /* 106 */ keyboard_event::F15_KEY,
        /* 107 */ keyboard_event::UNKNOWN_KEY, /* F16 */
        /* 108 */ keyboard_event::UNKNOWN_KEY, /* F17 */
        /* 109 */ keyboard_event::UNKNOWN_KEY, /* F18 */
        /* 110 */ keyboard_event::UNKNOWN_KEY, /* F19 */
        /* 111 */ keyboard_event::UNKNOWN_KEY, /* F20 */
        /* 112 */ keyboard_event::UNKNOWN_KEY, /* F21 */
        /* 113 */ keyboard_event::UNKNOWN_KEY, /* F22 */
        /* 114 */ keyboard_event::UNKNOWN_KEY, /* F23 */
        /* 115 */ keyboard_event::UNKNOWN_KEY, /* F24 */
        /* 116 */ keyboard_event::UNKNOWN_KEY, /* Execute */
        /* 117 */ keyboard_event::HELP_KEY,
        /* 118 */ keyboard_event::MENU_KEY,
        /* 119 */ keyboard_event::UNKNOWN_KEY, /* Select */
        /* 120 */ keyboard_event::BREAK_KEY,   /* Stop */
        /* 121 */ keyboard_event::UNKNOWN_KEY, /* Again */
        /* 122 */ keyboard_event::UNKNOWN_KEY, /* Undo */
        /* 123 */ keyboard_event::UNKNOWN_KEY, /* Cut */
        /* 124 */ keyboard_event::UNKNOWN_KEY, /* Copy */
        /* 125 */ keyboard_event::UNKNOWN_KEY, /* Paste */
        /* 126 */ keyboard_event::UNKNOWN_KEY, /* Find */
        /* 127 */ keyboard_event::UNKNOWN_KEY, /* Mute */
        /* 128 */ keyboard_event::UNKNOWN_KEY, /* Volume up */
        /* 129 */ keyboard_event::UNKNOWN_KEY, /* Volume down */
        /* 130 */ keyboard_event::UNKNOWN_KEY, 
        /* 131 */ keyboard_event::UNKNOWN_KEY, 
        /* 132 */ keyboard_event::UNKNOWN_KEY,
        /* 133 */ keyboard_event::UNKNOWN_KEY, /* KP Comma */
        /* 134 */ keyboard_event::UNKNOWN_KEY, /* KP Equals */
        
        /* 135 */ keyboard_event::UNKNOWN_KEY,
        /* 136 */ keyboard_event::UNKNOWN_KEY,
        /* 137 */ keyboard_event::UNKNOWN_KEY,
        /* 138 */ keyboard_event::UNKNOWN_KEY,
        /* 139 */ keyboard_event::UNKNOWN_KEY,
        /* 140 */ keyboard_event::UNKNOWN_KEY,
        /* 141 */ keyboard_event::UNKNOWN_KEY,
        /* 142 */ keyboard_event::UNKNOWN_KEY,
        /* 143 */ keyboard_event::UNKNOWN_KEY,
        /* 144 */ keyboard_event::UNKNOWN_KEY,
        /* 145 */ keyboard_event::UNKNOWN_KEY,
        /* 146 */ keyboard_event::UNKNOWN_KEY,
        /* 147 */ keyboard_event::UNKNOWN_KEY,
        /* 148 */ keyboard_event::UNKNOWN_KEY,
        /* 149 */ keyboard_event::UNKNOWN_KEY,
        /* 150 */ keyboard_event::UNKNOWN_KEY,
        /* 151 */ keyboard_event::UNKNOWN_KEY,
        /* 152 */ keyboard_event::UNKNOWN_KEY,

        /* 153 */ keyboard_event::UNKNOWN_KEY, /* Alt Erase */
        /* 154 */ keyboard_event::SYSREQ_KEY,
        /* 155 */ keyboard_event::UNKNOWN_KEY, /* Cancel */
        /* 156 */ keyboard_event::CLEAR_KEY,

        /* 157 */ keyboard_event::UNKNOWN_KEY,
        /* 158 */ keyboard_event::UNKNOWN_KEY,
        /* 159 */ keyboard_event::UNKNOWN_KEY,
        /* 160 */ keyboard_event::UNKNOWN_KEY,
        /* 161 */ keyboard_event::UNKNOWN_KEY,
        /* 162 */ keyboard_event::UNKNOWN_KEY,
        /* 163 */ keyboard_event::UNKNOWN_KEY,
        /* 164 */ keyboard_event::UNKNOWN_KEY,
        /* 165 */ keyboard_event::UNKNOWN_KEY,
        /* 166 */ keyboard_event::UNKNOWN_KEY,
        /* 167 */ keyboard_event::UNKNOWN_KEY,
        /* 168 */ keyboard_event::UNKNOWN_KEY,
        /* 169 */ keyboard_event::UNKNOWN_KEY,
        /* 170 */ keyboard_event::UNKNOWN_KEY,
        /* 171 */ keyboard_event::UNKNOWN_KEY,
        /* 172 */ keyboard_event::UNKNOWN_KEY,
        /* 173 */ keyboard_event::UNKNOWN_KEY,
        /* 174 */ keyboard_event::UNKNOWN_KEY,
        /* 175 */ keyboard_event::UNKNOWN_KEY,
        /* 176 */ keyboard_event::UNKNOWN_KEY,
        /* 177 */ keyboard_event::UNKNOWN_KEY,
        /* 178 */ keyboard_event::UNKNOWN_KEY,
        /* 179 */ keyboard_event::UNKNOWN_KEY,
        /* 180 */ keyboard_event::UNKNOWN_KEY,
        /* 181 */ keyboard_event::UNKNOWN_KEY,
        
        /* 182 */ keyboard_event::LEFTPAREN_KEY,
        /* 183 */ keyboard_event::RIGHTPAREN_KEY,
        
        /* 184 */ keyboard_event::UNKNOWN_KEY,
        /* 185 */ keyboard_event::UNKNOWN_KEY,
        /* 186 */ keyboard_event::UNKNOWN_KEY,
        /* 187 */ keyboard_event::UNKNOWN_KEY,
        /* 188 */ keyboard_event::UNKNOWN_KEY,
        /* 189 */ keyboard_event::UNKNOWN_KEY,
        /* 190 */ keyboard_event::UNKNOWN_KEY,
        /* 191 */ keyboard_event::UNKNOWN_KEY,
        /* 192 */ keyboard_event::UNKNOWN_KEY,
        /* 193 */ keyboard_event::UNKNOWN_KEY,
        /* 194 */ keyboard_event::UNKNOWN_KEY,
        /* 195 */ keyboard_event::UNKNOWN_KEY,
        /* 196 */ keyboard_event::UNKNOWN_KEY,
        
        /* 197 */ keyboard_event::LESS_KEY,
        /* 198 */ keyboard_event::GREATER_KEY,
        /* 199 */ keyboard_event::AMPERSAND_KEY,
        /* 200 */ keyboard_event::UNKNOWN_KEY,
        /* 201 */ keyboard_event::CARET_KEY,
        /* 202 */ keyboard_event::UNKNOWN_KEY,
        /* 203 */ keyboard_event::COLON_KEY,
        /* 204 */ keyboard_event::HASH_KEY,
        /* 205 */ keyboard_event::UNKNOWN_KEY,
        /* 206 */ keyboard_event::AT_KEY,
        /* 207 */ keyboard_event::EXCLAIM_KEY,
        
        /* 208 */ keyboard_event::UNKNOWN_KEY,
        /* 209 */ keyboard_event::UNKNOWN_KEY,
        /* 210 */ keyboard_event::UNKNOWN_KEY,
        /* 211 */ keyboard_event::UNKNOWN_KEY,
        /* 212 */ keyboard_event::UNKNOWN_KEY,
        /* 213 */ keyboard_event::UNKNOWN_KEY,
        /* 214 */ keyboard_event::UNKNOWN_KEY,
        /* 215 */ keyboard_event::UNKNOWN_KEY,
        /* 216 */ keyboard_event::UNKNOWN_KEY,
        /* 217 */ keyboard_event::UNKNOWN_KEY,
        /* 218 */ keyboard_event::UNKNOWN_KEY,
        /* 219 */ keyboard_event::UNKNOWN_KEY,
        /* 220 */ keyboard_event::UNKNOWN_KEY,
        /* 221 */ keyboard_event::UNKNOWN_KEY,
        /* 222 */ keyboard_event::UNKNOWN_KEY,
        /* 223 */ keyboard_event::UNKNOWN_KEY,
        
        /* 224 */ keyboard_event::LCTRL_KEY,
        /* 225 */ keyboard_event::LSHIFT_KEY,
        /* 226 */ keyboard_event::LALT_KEY,
        /* 227 */ keyboard_event::LMETA_KEY,
        /* 228 */ keyboard_event::RCTRL_KEY,
        /* 229 */ keyboard_event::RSHIFT_KEY,
        /* 230 */ keyboard_event::RALT_KEY,
        /* 231 */ keyboard_event::RMETA_KEY,
        
        /* 232 */ keyboard_event::UNKNOWN_KEY,
        /* 233 */ keyboard_event::UNKNOWN_KEY,
        /* 234 */ keyboard_event::UNKNOWN_KEY,
        /* 235 */ keyboard_event::UNKNOWN_KEY,
        /* 236 */ keyboard_event::UNKNOWN_KEY,
        /* 237 */ keyboard_event::UNKNOWN_KEY,
        /* 238 */ keyboard_event::UNKNOWN_KEY,
        /* 239 */ keyboard_event::UNKNOWN_KEY,
        /* 240 */ keyboard_event::UNKNOWN_KEY,
        /* 241 */ keyboard_event::UNKNOWN_KEY,
        /* 242 */ keyboard_event::UNKNOWN_KEY,
        /* 243 */ keyboard_event::UNKNOWN_KEY,
        /* 244 */ keyboard_event::UNKNOWN_KEY,
        /* 245 */ keyboard_event::UNKNOWN_KEY,
        /* 246 */ keyboard_event::UNKNOWN_KEY,
        /* 247 */ keyboard_event::UNKNOWN_KEY,
        /* 248 */ keyboard_event::UNKNOWN_KEY,
        /* 249 */ keyboard_event::UNKNOWN_KEY,
        /* 250 */ keyboard_event::UNKNOWN_KEY,
        /* 251 */ keyboard_event::UNKNOWN_KEY,
        /* 252 */ keyboard_event::UNKNOWN_KEY,
        /* 253 */ keyboard_event::UNKNOWN_KEY,
        /* 254 */ keyboard_event::UNKNOWN_KEY,
        /* 255 */ keyboard_event::UNKNOWN_KEY,
        /* 256 */ keyboard_event::UNKNOWN_KEY,
        
        /* 257 */ keyboard_event::MODE_KEY
    };

    /**
     * Translation table for SDL buttons symbols.
     * 
     */
    mouse_event::button_type sdl_button_trans[SDL_BUTTON_RIGHT + 3] = 
    {
        mouse_event::NO_BUTTON,
        mouse_event::LEFT_BUTTON,
        mouse_event::MIDDLE_BUTTON,
        mouse_event::RIGHT_BUTTON,
        mouse_event::WHEEL_UP,
        mouse_event::WHEEL_DOWN
    };

    /**
     * Translation table for SDL joystick axis symbols.
     * 
     */
    joystick_event::button_type sdl_axis_trans[joystick_event::NBR_BUTTONS - 
                                               joystick_event::AXIS0_FORE] =
    {
        joystick_event::AXIS0_FORE, 
        joystick_event::AXIS0_BACK,
        joystick_event::AXIS1_FORE,
        joystick_event::AXIS1_BACK,
        joystick_event::AXIS2_FORE,
        joystick_event::AXIS2_BACK,
        joystick_event::AXIS3_FORE,
        joystick_event::AXIS3_BACK,
        joystick_event::AXIS4_FORE,
        joystick_event::AXIS4_BACK,
        joystick_event::AXIS5_FORE, 
        joystick_event::AXIS5_BACK,
        joystick_event::AXIS6_FORE,
        joystick_event::AXIS6_BACK,
        joystick_event::AXIS7_FORE,
        joystick_event::AXIS7_BACK,
        joystick_event::AXIS8_FORE,
        joystick_event::AXIS8_BACK,
        joystick_event::AXIS9_FORE,
        joystick_event::AXIS9_BACK
    };

    /**
     * Translation table for SDL joystick buttons symbols.
     * 
     */
    joystick_event::button_type sdl_joy_button_trans[joystick_event::AXIS0_FORE] =
    {
        joystick_event::BUTTON_0,
        joystick_event::BUTTON_1,
        joystick_event::BUTTON_2,
        joystick_event::BUTTON_3,
        joystick_event::BUTTON_4,
        joystick_event::BUTTON_5,
        joystick_event::BUTTON_6,
        joystick_event::BUTTON_7,
        joystick_event::BUTTON_8,
        joystick_event::BUTTON_9
    };

    /**
     * Logical state of joysticks.
     * 
     */
    bool joystick_state_table[joystick_event::NBR_BUTTONS * 10] = { false };

    /**
     * List of used SDL joysticks.
     * 
     */
    std::vector<SDL_Joystick *> joysticks;
}

void input_manager_update()
{
    static SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
            {
                // SDL 1.2 did not send keydown event when turning off caps lock
                if (event.key.keysym.scancode == SDL_SCANCODE_CAPSLOCK && event.key.keysym.mod != 0)
                {
                    return;
                }
                    
                // FIXME event.key.keysym.unicode is obsolete
                input::keyboard_event ke (input::keyboard_event::KEY_PUSHED, input::sdl_key_trans[event.key.keysym.scancode], 
                                          event.key.keysym.unicode);
                input::manager::raise_event (ke);
                break;
            }
            case SDL_KEYUP:
            {
                
                // SDL 1.2 did not send keyup event when turning on caps lock
                if (event.key.keysym.scancode == SDL_SCANCODE_CAPSLOCK && event.key.keysym.mod != 0)
                {
                    return;
                }
                
                // FIXME event.key.keysym.unicode is obsolete
                input::keyboard_event ke (input::keyboard_event::KEY_RELEASED, input::sdl_key_trans[event.key.keysym.scancode], 
                                          event.key.keysym.unicode);
                input::manager::raise_event (ke);
                break;
            }
            case SDL_TEXTINPUT:
            {
                // event.text.text
            }
            case SDL_MOUSEMOTION:
            {
                input::mouse_event me (input::mouse_event::MOUSE_MOTION, input::mouse_event::NO_BUTTON, 
                                       event.motion.x, event.motion.y);
                input::manager::raise_event(me);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                input::mouse_event me(input::mouse_event::BUTTON_PUSHED, input::sdl_button_trans[event.button.button],
                                      event.button.x, event.button.y);
                input::manager::raise_event(me);
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                input::mouse_event me(input::mouse_event::BUTTON_RELEASED, input::sdl_button_trans[event.button.button],
                                      event.button.x, event.button.y);
                input::manager::raise_event(me);
                break;
            }
            case SDL_JOYAXISMOTION:
            {
                int value = event.jaxis.value > 0 ? event.jaxis.value : -event.jaxis.value;                
                int pushed = (value > 10000) ? 1 : 0;
                int joynbr = event.jaxis.which;
                int axisnbr = (event.jaxis.axis << 1) + (event.jaxis.value > 0);
                
                if (event.jaxis.axis >= input::joystick_event::NBR_BUTTONS - input::joystick_event::AXIS0_FORE) break;
                
                if (!pushed && !input::joystick_state_table[joynbr * input::joystick_event::NBR_BUTTONS
                                                            + axisnbr]) break;
                
                if (pushed && input::joystick_state_table[joynbr * input::joystick_event::NBR_BUTTONS
                                                          + axisnbr]) break;
                
                input::joystick_event je(event.jaxis.which, pushed ?
                                         input::joystick_event::BUTTON_PUSHED : 
                                         input::joystick_event::BUTTON_RELEASED, 
                                         input::sdl_axis_trans[axisnbr]);

                input::joystick_state_table[joynbr * input::joystick_event::NBR_BUTTONS + axisnbr] = pushed;

                input::manager::raise_event(je);
                break;
            }
            case SDL_JOYBUTTONDOWN:
            {
                if (event.jbutton.button >= input::joystick_event::AXIS0_FORE) break;

                input::joystick_event je(event.jbutton.which, input::joystick_event::BUTTON_PUSHED,
                                         input::sdl_joy_button_trans[event.jbutton.button]);
                input::manager::raise_event(je);                
                break;
            }
            case SDL_JOYBUTTONUP:
            {
                if (event.jbutton.button >= input::joystick_event::AXIS0_FORE) break;

                input::joystick_event je(event.jbutton.which, input::joystick_event::BUTTON_RELEASED,
                                         input::sdl_joy_button_trans[event.jbutton.button]);
                input::manager::raise_event(je);
                break;
            }
        }
    }    
}

