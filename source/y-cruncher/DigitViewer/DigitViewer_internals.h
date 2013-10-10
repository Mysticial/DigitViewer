/* DigitViewer_internals.h - Digit I/O Globals
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/31/2013
 * Last Modified    : 07/31/2013
 * 
 */

#pragma once
#ifndef _yc_DigitViewer_internals_H
#define _yc_DigitViewer_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef DIGITVIEWER_STANDALONE
//  Use this to compile the Digit Viewer as a stand-alone program.

#include "ConsoleIO/ymo_APIs_cio_Shared.h"
#include "Strings/ymb_APIs_StringTools_internals.h"
#include "Strings/ymb_APIs_StringBuilder_internals.h"
#include "CVN/ymb_CVN_u64_forward.h"

#if WIN32
#include <windows.h>
#include "ConsoleIO/ymo_APIs_cio_Windows.h"
#include "FileIO/ymf_APIs_file_WinAPI_internals.h"
#elif defined __linux
#include "ConsoleIO/ymo_APIs_cio_ColorCodes.h"
#include "FileIO/ymf_APIs_file_Default_internals.h"
#else
#include "ConsoleIO/ymo_APIs_cio_Default.h"
#include "FileIO/ymf_APIs_file_Default_internals.h"
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif defined USE_Y_CRUNCHER_REPOSITORY
//  Use this to compile the Digit Viewer separately, but referencing its source
//  code directly from y-cruncher.

#include "../../Modules_v1.0/APIs/ConsoleIO/ymo_APIs_cio_Shared.h"
#include "../../Modules_v1.0/APIs/Strings/ymb_APIs_StringTools_internals.h"
#include "../../Modules_v1.0/APIs/Strings/ymb_APIs_StringBuilder_internals.h"
#include "../../Modules_v1.0/CVN/ymb_CVN_u64_forward.h"

#if WIN32
#include <windows.h>
#include "../../../Source Files/Modules_v1.0/APIs/ConsoleIO/ymo_APIs_cio_Windows.h"
#include "../../../Source Files/Modules_v1.0/APIs/FileIO/ymf_APIs_file_WinAPI_internals.h"
#elif defined __linux
#include "../../../Source Files/Modules_v1.0/APIs/ConsoleIO/ymo_APIs_cio_ColorCodes.h"
#include "../../../Source Files/Modules_v1.0/APIs/FileIO/ymf_APIs_file_Default_internals.h"
#else
#include "../../../Source Files/Modules_v1.0/APIs/ConsoleIO/ymo_APIs_cio_Default.h"
#include "../../../Source Files/Modules_v1.0/APIs/FileIO/ymf_APIs_file_Default_internals.h"
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#else
//  y-cruncher needs the Digit Viewer to output the digits.
//  When compiling as a part of y-cruncher, use these.

#include "../../Modules_v1.0/CVN/ymb_CVN_headers.h"

#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "DigitViewer_headers.h"
#include "DigitReaders/yc_DigitReader_internals.h"
#include "DigitWriters/yc_DigitWriter_internals.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
