/* y-cruncher.cpp - y-cruncher Internal Dependencies
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 03/24/2012
 * Last Modified    : 08/03/2013
 * 
 */

#include "y-cruncher.h"

#ifdef USE_Y_CRUNCHER_REPOSITORY
//  Use this to reference the dependencies directly from y-cruncher.

#include "../../../Source Files/y-cruncher/DigitViewer/DigitViewer_internals.h"
#include "../../../Source Files/y-cruncher/ProcessorCapability/cpu_x86_internals.h"

#else
//  Use this to compile the Digit Viewer as a stand-alone program.

//  APIs - General
#include "ConsoleIO/ymo_APIs_cio_Shared.h"
#include "Strings/ymb_APIs_StringTools_internals.h"
#include "Strings/ymb_APIs_StringBuilder_internals.h"

//  APIs - Non-portable
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

//  Conversions
#include "CVN/ymb_CVN_u64_forward.h"

//  Digit Readers
#include "DigitViewer/DigitReaders/yc_DigitReader_internals.h"

//  Digit Writers
#include "DigitViewer/DigitWriters/yc_DigitWriter_internals.h"

//  Processor Capability
#include "ProcessorCapability/cpu_x86_internals.h"

#endif
