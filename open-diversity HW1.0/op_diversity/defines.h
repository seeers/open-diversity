/* 
Open Diversity - Video based diversity software
Idea by Rangarid
Hardware design by Daniel
Implementation and refinement by Nils, Nachbrenner, Rangarid

Copyright 2011-2012 by Nils, Nachbrenner, Rangarid

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

#define LED1 12
#define LED2 13

#define BUZZER 8

#define SUP_VOLT A0
#define SENS_INPUT A4

#define RSSI1 A3 
#define RSSI2 A2 // A0 ist the Pin that is next to yellow led

// Framerate
#define SWITCH_EVERY SWITCH_RATE / FR_UPDATE_RATE
