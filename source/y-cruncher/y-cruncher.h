/* y-cruncher.h - y-cruncher Internal Dependencies
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 03/24/2012
 * Last Modified    : 08/03/2013
 * 
 */

#define PROGRAM_NAME "y-cruncher Digit Viewer v2.2.302"

//  Prepare Dispatch Macros
#ifdef X86_SSE41
#define ymc_NPO_SSE41
#elif defined X86_SSE2
#define ymc_NPO_SSE2
#include <emmintrin.h>
#include <stdint.h>
#if !((defined _M_X64) || (defined __x86_64__))
//  This macro doesn't exist in 32-bit mode. Emulate it here.
inline static int64_t _mm_cvtsi128_si64(__m128i x){
    return (uint32_t)_mm_cvtsi128_si32(x) | (int64_t)_mm_cvtsi128_si32(_mm_shuffle_epi32(x,177)) << 32;
}
#endif
#endif

#if _WIN32
#pragma warning(disable:4100)   //  Unreferenced Formal Parameter
#pragma warning(disable:4127)   //  Constant Expression
//#pragma warning(disable:4324)   //  Structure Padding
#pragma warning(disable:4505)   //  Unused Static Function
#endif

#pragma once
#ifndef _y_cruncher_H
#define _y_cruncher_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "SelfContained.h"
#ifdef USE_Y_CRUNCHER_REPOSITORY
//  Use this reference the dependencies directly from y-cruncher.

#include "../../../Source Files/Modules_v1.0/CVN/ymb_CVN_headers.h"
#include "../../../Source Files/y-cruncher/DigitViewer/DigitViewer_headers.h"

#else
//  Use this to compile the Digit Viewer as a stand-alone program.

#include "Exception.h"
#include "ConsoleIO/ymo_APIs_cio_headers.h"

#if WIN32
#include "FileIO/ymf_APIs_file_WinAPI_headers.h"
#elif __linux
#include "FileIO/ymf_APIs_file_Default_headers.h"
#else
#include "FileIO/ymf_APIs_file_Default_headers.h"
#endif

#include "CVN/ymb_CVN_headers.h"
#include "DigitViewer/DigitViewer_headers.h"

#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
