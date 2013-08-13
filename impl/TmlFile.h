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
		virtual TmlBlock* DLLCALLCONV getTemplate(const TemplateGuid& guid);
		virtual TmlBlock* DLLCALLCONV getTemplate(const char* name);

		virtual void DLLCALLCONV addTemplate(ITmlBlock *iBlock);

	private:
		std::unordered_map<TemplateGuid, TmlBlock*> templatesByGuid;
		std::unordered_map<std::string, TmlBlock*> templatesByName;
};

#endif // TML_TMLFILE_H
