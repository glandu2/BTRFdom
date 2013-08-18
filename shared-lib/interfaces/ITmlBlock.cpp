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

#include "ITmlBlock.h"
#include "TmlBlock.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:createTmlBlock=_createTmlBlock@0")
#endif
DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV createTmlBlock() {
	return new TmlBlock;
}

#ifdef __cplusplus
}
#endif
