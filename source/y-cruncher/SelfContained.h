/* SelfContainedHeader.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 03/24/2012
 * Last Modified    : 03/24/2012
 * 
 * 
 * 
 *  This file contains the all the datatype dependencies from y-cruncher.
 *  This file is not used in y-cruncher, but is needed for the sub-components to
 *  compile separately.
 */

#pragma once
#ifndef _ym_SelfContainedHeader_H
#define _ym_SelfContainedHeader_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  y-cruncher Datatypes

//  These are leftover from the pre-<stdint> days.
typedef     unsigned short      ym_u16;
typedef     unsigned int        ym_u32;
typedef     signed int          ym_s32;
typedef     unsigned long long  ym_u64;
typedef     signed long long    ym_s64;

//  Size of a pointer.
typedef     size_t              ym_pL;

typedef     double              ym_sF;

//  Size of types that are logarithmically bound.
typedef     unsigned int        ym_kL;

//  "Signed/Unsigned Length" - largest integer type.
typedef     ym_u64              ym_uL;
typedef     ym_s64              ym_sL;

//  File Length - Integer type for file sizes and offsets.
typedef     ym_u64              ym_fL;

////////////////////////////////////////////////////////////////////////////////
#define     YM_IND_BITS         64
#define     YMS_MAX_THREAD_K    8
////////////////////////////////////////////////////////////////////////////////
#define     ymo_print_uL_commas ymo_print_u64_commas
////////////////////////////////////////////////////////////////////////////////
#if defined WIN32
#define YM_NOINLINE __declspec(noinline)
#elif defined __GNUC__
#define YM_NOINLINE __attribute__ ((noinline))
#else
#define YM_NOINLINE
#endif
////////////////////////////////////////////////////////////////////////////////
ym_uL ymo_prompt_uL(const wchar_t *msg,ym_uL low_limit,ym_uL high_limit);
static void ym_pause(){
#ifdef WIN32
    system("pause");
#else
    ymo_prompt_uL(L"Press ENTER to continue . . .",0,(ym_uL)-1);
    getchar();
#endif
}
static void ym_quit(int val){
    ym_pause();
    exit(val);
}
extern  void    ymo_error               (const char *msg);
////////////////////////////////////////////////////////////////////////////////
//  This is the memory allocator used in y-cruncher. It is not needed for the
//  Digit Viewer. So mimic the interfaces and point all of them to "malloc()"
//  and "free()".
typedef struct ym_heap_{
} ym_heap;
static void ym_allocfail(){
    ymo_error("Memory Allocation Failure.");
}
static inline void* ym_ah_malloc(ym_heap *ah,ym_pL bytes,ym_pL align){
    if (align < sizeof(ym_pL))
        align = sizeof(ym_pL);

    void *ptr = malloc((size_t)(bytes + align + sizeof(ym_pL)));
    if (ptr == NULL)
        ym_allocfail();

    ym_pL *ret = (ym_pL*)((((ym_pL)ptr + sizeof(ym_pL)) & ~(ym_pL)(align - 1)) + align);

    ret[-1] = (ym_pL)ptr;

    return ret;
}
static inline void ym_ah_free(void *ptr){
    if (ptr == NULL)
        return;

    ptr = (void*)(((ym_pL*)ptr)[-1]);

    free(ptr);
}
static inline char* ym_ah_malloc_ch(ym_heap *ah,ym_pL cnt){
    return (char*)ym_ah_malloc(ah,cnt * sizeof(char),sizeof(char));
}
static inline wchar_t* ym_ah_malloc_wch(ym_heap *ah,ym_pL cnt){
    return (wchar_t*)ym_ah_malloc(ah,cnt * sizeof(wchar_t),sizeof(wchar_t));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
#endif
