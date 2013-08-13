#include "IBtrfParser.h"
#include "BtrfParser.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF IBtrfParser * DLLCALLCONV createBtrfParser(ITmlFile *tmlFile) {
	return new BtrfParser(static_cast<TmlFile*>(tmlFile));
}

#ifdef __cplusplus
}
#endif
