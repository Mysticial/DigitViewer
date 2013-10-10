/* DigitViewer_globals.h - Digit I/O Globals
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/31/2013
 * Last Modified    : 08/03/2013
 * 
 */

#pragma once
#ifndef _yc_DigitViewer_globals_H
#define _yc_DigitViewer_globals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef DIGITVIEWER_STANDALONE
//  Use this to compile the Digit Viewer as a stand-alone program.

#include "../SelfContained.h"
#include "../Exception.h"
#include "../ConsoleIO/ymo_APIs_cio_headers.h"

#if WIN32
#include "../FileIO/ymf_APIs_file_WinAPI_headers.h"
#elif __linux
#include "../FileIO/ymf_APIs_file_Default_headers.h"
#else
#include "../FileIO/ymf_APIs_file_Default_headers.h"
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif defined USE_Y_CRUNCHER_REPOSITORY
//  Use this to compile the Digit Viewer separately, but referencing its source
//  code directly from y-cruncher.

#include "../../../Digit Viewer (VSS)/source/y-cruncher/SelfContained.h"
#include "../../Modules_v1.0/Exception.h"
#include "../../Modules_v1.0/APIs/ConsoleIO/ymo_APIs_cio_headers.h"

#if WIN32
#include "../../Modules_v1.0/APIs/FileIO/ymf_APIs_file_WinAPI_headers.h"
#elif __linux
#include "../../Modules_v1.0/APIs/FileIO/ymf_APIs_file_Default_headers.h"
#else
#include "../../Modules_v1.0/APIs/FileIO/ymf_APIs_file_Default_headers.h"
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#else
//  y-cruncher needs the Digit Viewer to output the digits.
//  When compiling as a part of y-cruncher, use these.

#include "../../Modules_v1.0/Exception.h"
#include "../../Modules_v1.0/APIs/ymi_APIs_headers.h"

#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  File Tokens
#define YCF_CDF_FileVersion             "1.1.0"
#define YCF_CDF_TOKEN_FileVersion       "FileVersion:"
#define YCF_CDF_TOKEN_Base              "Base:"
#define YCF_CDF_TOKEN_FirstDigits       "FirstDigits:"
#define YCF_CDF_TOKEN_TotalDigits       "TotalDigits:"
#define YCF_CDF_TOKEN_BlockSize         "Blocksize:"
#define YCF_CDF_TOKEN_TotalBlocks       "TotalBlocks:"
#define YCF_CDF_TOKEN_BlockID           "BlockID:"
#define YCF_CDF_TOKEN_EndHeader         "EndHeader"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Errors
#define YCR_DIO_ERROR_INVALID_PARAMETERS        (int)0x80000001

#define YCR_DIO_ERROR_INVALID_FILE              (int)0x80001000
#define YCR_DIO_ERROR_INVALID_NAME              (int)0x80001001
#define YCR_DIO_ERROR_INVALID_EXTENSION         (int)0x80001002
#define YCR_DIO_ERROR_INVALID_BASE              (int)0x80001003
#define YCR_DIO_ERROR_UNEXPECTED_END_OF_FILE    (int)0x80001004
#define YCR_DIO_ERROR_NO_DECIMAL_PLACE          (int)0x80001005
#define YCR_DIO_ERROR_INVALID_DIGIT             (int)0x80001006
#define YCR_DIO_ERROR_MAXIMUM_PATHS_EXCEEDED    (int)0x80001007

#define YCR_DIO_ERROR_INCONSISTENT              (int)0x80002000
#define YCR_DIO_ERROR_INCONSISTENT_ID           (int)0x80002001
#define YCR_DIO_ERROR_INCONSISTENT_BASE         (int)0x80002002
#define YCR_DIO_ERROR_INCONSISTENT_DIGITS       (int)0x80002003

#define YCR_DIO_ERROR_INTERNAL                  (int)0x80004000
#define YCR_DIO_ERROR_INVALID_FILE_STATE        (int)0x80004001
#define YCR_DIO_ERROR_FILE_NOT_FOUND            (int)0x80004002
#define YCR_DIO_ERROR_CREATE_FAIL               (int)0x80004003
#define YCR_DIO_ERROR_OPEN_FAIL                 (int)0x80004004
#define YCR_DIO_ERROR_READ_FAIL                 (int)0x80004005
#define YCR_DIO_ERROR_WRITE_FAIL                (int)0x80004006
#define YCR_DIO_ERROR_MISSING_FILES             (int)0x80004007
#define YCR_DIO_ERROR_OUT_OF_RANGE              (int)0x80004008

#define YCR_DIO_ERROR_NO_DIGITS_LEFT            (int)0x80008001
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
