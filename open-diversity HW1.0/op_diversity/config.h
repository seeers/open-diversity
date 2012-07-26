/* 
Open Diversity - Video based diversity software
Idea by Rangarid
Hardware design by Daniel
Implementation and refinement by Nils, Nachbrenner

Copyright 2011-2012 by Nils, Nachbrenner

This file is part of Open Diversity

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */
// PINS
#define VSWITCH_DISABLE 11
#define VSWITCH_SELECT 10

#define LED_1 12
#define LED_2 13
#define BUZZER 8

#define SUP_VOLT A0
#define SENS_INPUT A4
#define LOWVOLTAG_WARN
#define ALARM_VOLT 500 // ~9.9V , 1023 ^= 20.15V

#define HISTORY_SIZE 10 // How many old framerates to keep to calculate avg
#define FR_UPDATE_INT 100 // update framerate records every XX milliseconds
#define SWITCH_INT 200 // look if I need to switch video input every XXX milliseconds. Needs to be > FR_UPDATE_RATE
#define BEEP_INT 400


// === Optional RSSI

//#define RSSI_ON // comment out to disable RSSI output
//#define SERIAL_OUTPUT_ON
#define BUZZER_CHANNEL_IND // acoustic channel chance indicator
#define RSSI1 A3 
#define RSSI2 A2 // A0 ist the Pin that is next to yellow led

// ===================

#define SWITCH_EVERY SWITCH_INT / FR_UPDATE_INT
#define BEEP_EVERY BEEP_INT / FR_UPDATE_INT


