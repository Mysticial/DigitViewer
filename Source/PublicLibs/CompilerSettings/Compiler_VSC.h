/* Compiler_VSC.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 12/30/2009
 * Last Modified    : 08/09/2015
 * 
 *      This includes the Intel Compiler on Windows since it supports all of
 *  Visual Studio's features.
 * 
 */

#pragma once
#ifndef _ymp_Compiler_VSC_H
#define _ymp_Compiler_VSC_H
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YM_NO_INLINE        __declspec(noinline)
#define YM_FORCE_INLINE     inline __forceinline
#define YM_ALIGN(x)         __declspec(align(x))
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Restrict
template <typename type> using r_ptr = type *__restrict;
template <typename type> using c_ptr = type const*__restrict;
template <typename type> using r_ref = type &__restrict;
template <typename type> using c_ref = type const&__restrict;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dynamic Linking
#ifdef YMP_STANDALONE
#define YMP_EXPORT
#else
#ifdef _WINDLL
#define YMP_EXPORT __declspec(dllexport)
#else
#define YMP_EXPORT __declspec(dllimport)
#define YMP_IMPORT
#endif
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Warnings
#pragma warning(disable:4100)   //  Unreferenced Formal Parameter
#pragma warning(disable:4127)   //  Constant Expression
#pragma warning(disable:4324)   //  Structure Padding
#pragma warning(disable:4456)   //  Variable Shadowing
//#pragma warning(disable:4457)   //  Variable Hides Parameter
#pragma warning(disable:4458)   //  Member Hiding
//#pragma warning(disable:4459)   //  Parameter Hides Global
#pragma warning(disable:4505)   //  Unused Static Function
#pragma warning(disable:4661)   //  No Definition for Template Instantiation

//  Analysis Warnings
#pragma warning(disable:6011)   //  Dereference NULL pointer
#pragma warning(disable:6246)   //  Variable Shadowing
#pragma warning(disable:6326)   //  Constant Comparison

//  Disable specific ICC warnings
#if defined __INTEL_COMPILER
#pragma warning disable 186     //  Unsigned comparison with zero
#pragma warning disable 532     //  No Definition for Template Instantiation
#pragma warning disable 1125    //  Virtual function hiding
#pragma warning disable 3280    //  Member hiding
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
