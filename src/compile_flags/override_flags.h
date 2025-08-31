/*
	override_flags.h - constant variables per board override
	Copyright (C) 2025 Camren Chraplak

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef OVERRIDE_FLAGS_H
#define OVERRIDE_FLAGS_H

#ifndef BAUD_RATE
	#define BAUD_RATE 9600 // baud rate to print to console
	#define W_OVERRIDEN
#endif

#ifndef NVM_SIZE
	#define NVM_SIZE 0 // size in bytes of NVM
	#define W_OVERRIDEN
#endif

#ifndef WAIT_RUN
	#define WAIT_RUN 0 // how long to wait for program to run in ms
	#define W_OVERRIDEN
#endif

#ifndef DEFAULT_NVM
	#define DEFAULT_NVM true // whether to use the default nvm methods over custom nvm methods
	#define W_OVERRIDEN
#endif

#endif