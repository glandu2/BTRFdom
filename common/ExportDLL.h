#ifndef EXPORTDLL_H_INCLUDED
#define EXPORTDLL_H_INCLUDED

#ifdef __unix__
#	if defined(BUILDING_BTRF)
#		define DLLEXPORT_BTRF __attribute__((visibility("default")))
#	else
#		define DLLEXPORT_BTRF
#	endif
#else
#	if defined(BUILDING_BTRF)
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
