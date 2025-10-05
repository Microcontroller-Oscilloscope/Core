/*
	compile_warnings.h - checks no issues with compilation
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

#ifndef COMPILE_WARNINGS_H
#define COMPILE_WARNINGS_H

#include "compile_flags.h"
#include "../nvm/nvm.h"

#ifndef __IGNORE_WARNINGS__

	#ifdef W_CUSTOM_NVM
		#warning Using custom NVM method
	#endif

	#ifdef __ENABLED_DEV_RELEASE__
		#warning Compiling in Dev mode for release
	#endif

	#ifdef __ENABLED_DEV_TEST__
		#warning Compiling in Dev mode for Unity testing
	#endif

	#if defined(NO_FLOAT_PRINT_SUPPORT)
		#warning Float/Double printing functions wont display anything
	#endif

#endif

#endif