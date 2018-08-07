/*
stdafx.h

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2017-2018 University of British Columbia

--- begin license - do not edit ---
    This file is a part of CGaze UI.
    
    CGaze UI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cgaze UI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cgaze UI.  If not, see <https://www.gnu.org/licenses/>.
--- end license ---
*/


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

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#define EYEGAZECAMERA_DLL_EXPORTS

#if _WINDOWS
    #ifdef EYEGAZECAMERA_DLL_EXPORTS
        #define EYEGAZECAMERA_API __declspec(dllexport)   
    #else  
        #define EYEGAZECAMERA_API __declspec(dllimport)   
    #endif
    #define EYEGAZECAMERA_API_LOCAL
#else
    #if __GNUC__ >= 4
        #define EYEGAZECAMERA_API __attribute__ ((visibility ("default")))
        #define EYEGAZECAMERA_API_LOCAL __attribute__ ((visibility ("hidden")))
    #else
        #define EYEGAZECAMERA_API
        #define EYEGAZECAMERA_API_LOCAL
    #endif 
#endif




// TODO: reference additional headers your program requires here
