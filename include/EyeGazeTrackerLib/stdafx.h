// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#ifdef _WINDOWS

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#endif

#define EYEGAZETRACKER_DLL_EXPORTS

#if _WINDOWS
    #ifdef EYEGAZETRACKER_DLL_EXPORTS
        #define EYEGAZETRACKER_API __declspec(dllexport)   
    #else  
        #define EYEGAZETRACKER_API __declspec(dllimport)   
    #endif
    #define EYEGAZETRACKER_API_LOCAL
#else
    #if __GNUC__ >= 4
        #define EYEGAZETRACKER_API __attribute__ ((visibility ("default")))
        #define EYEGAZETRACKER_API_LOCAL __attribute__ ((visibility ("hidden")))
    #else
        #define EYEGAZETRACKER_API
        #define EYEGAZETRACKER_API_LOCAL
    #endif 
#endif



// TODO: reference additional headers your program requires here
