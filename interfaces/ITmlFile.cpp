#include "ITmlFile.h"
#include "TmlFile.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF ITmlFile * DLLCALLCONV createTmlFile() {
	return new TmlFile;
}

#ifdef __cplusplus
}
#endif
