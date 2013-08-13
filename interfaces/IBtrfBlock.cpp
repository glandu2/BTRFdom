#include "IBtrfBlock.h"
#include "BtrfBlock.h"
#include "TmlBlock.h"
#include "BtrfRootBlock.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV createBtrfBlock(ITmlBlock *fieldInfo, IBtrfRootBlock *rootBlock) {
	BtrfBlock *block = new BtrfBlock;
	block->construct(static_cast<TmlBlock*>(fieldInfo), static_cast<BtrfRootBlock*>(rootBlock));

	return block;
}

#ifdef __cplusplus
}
#endif
