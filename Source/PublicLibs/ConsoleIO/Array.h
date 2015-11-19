/* Array.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 09/11/2014
 * Last Modified    : 09/11/2014
 * 
 */

#pragma once
#ifndef _ymp_Console_Array_H
#define _ymp_Console_Array_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "BasicIO.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename wtype> YM_NO_INLINE void print_af     (const wtype* A, upL_t L, char color = ' ');
template <typename wtype> YM_NO_INLINE void println_af   (const wtype* A, upL_t L, char color = ' ');
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
