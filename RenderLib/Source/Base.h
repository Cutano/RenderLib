#pragma once

#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <cassert>

#include "Utility/Log.h"
#include "Utility/Event/EventBus.h"

#define BIT(x) (1u << (x))
#define RL_ASSERT(exp, msg) if (!(exp)) {assert(false); Log::Logger()->critical(msg);}
