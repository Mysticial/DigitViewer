/* PrintHelpers.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/15/2015
 * Last Modified    : 01/15/2015
 * 
 */

#pragma once
#ifndef _ycr_DigitViewer_PrintHelpers_H
#define _ycr_DigitViewer_PrintHelpers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Types.h"
namespace DigitViewer{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ClearLine();
void DisplayChar(char digit, upL_t *b, upL_t *r, uiL_t *c);
void DisplayFancy(uiL_t pos, const char *str, upL_t digits);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif