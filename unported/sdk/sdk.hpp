
/*
 * sdk.h
 *
 *  Created on: Dec 5, 2016
 *      Author: nullifiedcat
 */

/*
 *
 *	This source sdk was modified to suit the needs of cathook.
 *	Please respect cathook and valves licences.
 *
 */

// TODO: EMIMINATE THE USE OF THE SSDK AND SWITCH TO OUR OWN BUILD ONE
// NOTE: BE SURE TO EMIMINATE HUNGARIAN NOTATION AS WELL TO FIT THE CODE STYLE
// OF NEKOHOOK NOTE: Replace any same types between nekohook and the sdk with
// nekohooks, ie: Vector -> CatVector

#pragma once

// Source engine is mean with this so we just disable outright
#if defined(NDEBUG)
#define SDK_NDEBUG 1
#undef NDEBUG
#endif

// Multiplatform fixes

// linux
#if defined(__linux__)
#ifdef _GLIBCXX_USE_CXX11_ABI
#undef _GLIBCXX_USE_CXX11_ABI
#endif
#define _GLIBCXX_USE_CXX11_ABI 0
#define _POSIX 1
#define RAD_TELEMETRY_DISABLED 1
#define LINUX 1
#define USE_SDL 1
#define _LINUX 1
#define POSIX 1
#define GNUC 1
#define NO_MALLOC_OVERRIDE 1
// Windows
#elif defined(_WIN32)
#ifdef __MINGW32__
#define F1_GCC 1
#define F1_WINDOWS 1
#endif

#define WIN32 1
#define _WIN32 1
#define NDEBUG 1
#define _WINDOWS 1
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#define _ALLOW_RUNTIME_LIBRARY_MISMATCH 1
#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH 1
#define _ALLOW_MSC_VER_MISMATCH 1
#define COMPILER_MSVC32 1
#define COMPILER_MSVC 1
#define _DLL_EXT .dll
#endif

// To fix cathook from messing with our sdk. This is cathook specific
#undef min
#undef max

// Data structures
#include "cathook_sdk/client_class.hpp"
#include "cathook_sdk/in_buttons.h"
#include "cathook_sdk/usercmd.hpp"
#include "public/Color.h"
#include "public/mathlib/vector.h"
#include "public/mathlib/vmatrix.h"

// More class than data
#include "cathook_sdk/convar.hpp"
#include "public/globalvars_base.h"
#include "public/icliententity.h"

//#include "game/shared/usercmd.h"

// Interfaces
#include "game/client/iclientmode.h"  // IClientMode
#include "public/cdll_int.h"          // IVEngineClient013
#include "public/edict.h"             // IGlobalVars
#include "public/engine/IEngineTrace.h"
#include "public/engine/ivmodelinfo.h"
#include "public/iclient.h"
#include "public/icliententitylist.h"
#include "public/icvar.h"
#include "public/inputsystem/iinputsystem.h"
#include "public/toolframework/ienginetool.h"  // IEngineTool
#include "public/vgui/IPanel.h"
#include "public/vgui/ISurface.h"

inline const char* CUtlString::Get() const { return ""; }
using namespace vgui;

// To fix the sdk messing with our stuff. This is cathook specific
#undef min
#undef max

#if defined(SDK_NDEBUG)
#undef SDK_NDEBUG
#define NDEBUG 1
#endif
