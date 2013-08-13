#include "ITmlBlock.h"
#include "TmlBlock.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV createTmlBlock() {
	return new TmlBlock;
}

#ifdef __cplusplus
}
#endif
