#include "IBtrfRootBlock.h"
#include "BtrfRootBlock.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF IBtrfRootBlock * DLLCALLCONV createBtrfRootBlock(ITmlFile *tmlFile) {
	return new BtrfRootBlock(static_cast<TmlFile*>(tmlFile));
}

#ifdef __cplusplus
}
#endif
