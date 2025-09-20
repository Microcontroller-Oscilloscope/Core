/*
	nvm.h - includes relevant NVM implementation
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

#include "../compile_flags/compile_flags.h"

#if defined(NVM_EEPROM) && DEFAULT_NVM
	#include "core_eeprom.h"
#elif defined(NVM_PREF) && DEFAULT_NVM
	#include "core_pref.h"
#else
	#if !DEFAULT_NVM
		#include <custom_nvm.h>
		#define W_CUSTOM_NVM
	#else
		#error No NVM Enabled
	#endif
#endif