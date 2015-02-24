/*
 * BTRFdom - Rappelz BTRF Document Object Model
 * By Glandu2
 * Copyright 2013 Glandu2
 *
 * This file is part of BTRFdom.
 * BTRFdom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BTRFdom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with BTRFdom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXPORTDLL_H_INCLUDED
#define EXPORTDLL_H_INCLUDED

#ifdef __unix__
#	if defined(BUILDING_BTRF)
#		define DLLEXPORT_BTRF __attribute__((visibility("default")))
#	else
#		define DLLEXPORT_BTRF
#	endif
#else
#	if defined(BUILDING_BTRF_STATIC)
#		define DLLEXPORT_BTRF
#	elif defined(BUILDING_BTRF)
#		define DLLEXPORT_BTRF __declspec(dllexport)
#	else
#		define DLLEXPORT_BTRF __declspec(dllimport)
#	endif
#endif

#ifdef __unix__
#	define DLLCALLCONV
#else
#	define DLLCALLCONV __stdcall
#endif


#endif // EXPORTDLL_H_INCLUDED
