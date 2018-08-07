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

#define EYEGAZECALIBRATION_DLL_EXPORTS

#if _WINDOWS
#ifdef EYEGAZECALIBRATION_DLL_EXPORTS
#define EYEGAZECALIBRATION_API __declspec(dllexport)   
#else  
#define EYEGAZECALIBRATION_API __declspec(dllimport)   
#endif
#else
#define EYEGAZECALIBRATION_API 
#endif

// TODO: reference additional headers your program requires here
