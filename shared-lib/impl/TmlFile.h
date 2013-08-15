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

#ifndef TML_TMLFILE_H
#define TML_TMLFILE_H

#include <unordered_map>
#include <string>
#include "ITmlFile.h"
#include "TmlBlock.h"

class TmlFile : public ITmlFile
{
	public:
		TmlFile();
		~TmlFile();

		COM_BEGIN_DECLARE_IFACE
		COM_DECLARE_IFACE(ITmlFile)
		COM_END_DECLARE_IFACE

		virtual bool DLLCALLCONV parseFile(const char *file);
		virtual TmlBlock* DLLCALLCONV getTemplateByGuid(const TemplateGuid& guid);
		virtual TmlBlock* DLLCALLCONV getTemplateByName(const char* name);

		virtual void DLLCALLCONV addTemplate(ITmlBlock *iBlock);

	private:
		std::unordered_map<TemplateGuid, TmlBlock*> templatesByGuid;
		std::unordered_map<std::string, TmlBlock*> templatesByName;
};

#endif // TML_TMLFILE_H
