/* 
Open Diversity - Video based diversity software
Idea by Rangarid
Hardware design by Daniel
Implementation by Rangarid

Copyright 2011-2012 by Rangarid

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

//LM1881
#define VSYNC0 2
#define VSYNC1 3
//#define CSYNC0
//#define CSYNC1

//ADG794
#define IN 10
#define EN 11

//LEDs
#define LED0 12
#define LED1 13

//Buzzer
#define BUZZER 8

//RSSI
#define RSSI0 A2
#define RSSI1 A3

//Battery
#define VBAT A0

volatile int syncs[2] = {0,0};
volatile int rssi[2] = {0,0};

enum sensitivitylevel {SLOW, MEDIUM, FAST};

