/* ymf_APIs_file_Default_headers.h - File IO - Default
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/05/2010
 * Last Modified    : 07/26/2011
 * 
 */

#pragma once
#ifndef _ymf_APIs_file_Default_headers_H
#define _ymf_APIs_file_Default_headers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <stdio.h>
#include <errno.h>
#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
#ifndef _ym_SelfContainedHeader_H
#include "../../Globals.h"
#endif
#include "../ConsoleIO/ymo_APIs_cio_headers.h"
typedef struct ym_heap_ ym_heap;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Headers
extern  void    yma_APIs_file               ();
extern  void    ym_file_makedir             (const wchar_t *path);
extern  void    ym_file_rename_pp           (const wchar_t *oldname,const wchar_t *newname);
extern  ym_fL   ym_file_getsize             (const wchar_t *path);
extern  int     ym_file_exist               (const wchar_t *path);
extern  void    ym_file_delete_p            (const wchar_t *path);
extern  int     ym_file_check_directory     (ym_heap *ah,const wchar_t *directory);
////////////////////////////////////////////////////////////////////////////////
extern  ym_pL ym_file_BLOCK_SIZE;
typedef struct{
    wchar_t *path;          //  Path of the file
    FILE *filehandle;       //  File handle
} ym_file;
////////////////////////////////////////////////////////////////////////////////
extern  int     ym_file_open_r              (ym_heap *ah,ym_file *file,const wchar_t *path);
extern  void    ym_file_open                (ym_heap *ah,ym_file *file,const wchar_t *path);
extern  void    ym_file_close               (ym_file *file);
extern  int     ym_file_create_r            (ym_heap *ah,ym_file *file,ym_fL bytes,const wchar_t *path);
extern  void    ym_file_create              (ym_heap *ah,ym_file *file,ym_fL bytes,const wchar_t *path);
extern  void    ym_file_delete              (ym_file *file);
extern  void    ym_file_force_delete        (ym_file *file);
extern  void    ym_file_rename              (ym_heap *ah,ym_file *file,const wchar_t *path);
////////////////////////////////////////////////////////////////////////////////
extern  int     ym_file_setptr_r            (ym_file *file,ym_fL offset);
extern  void    ym_file_setptr              (ym_file *file,ym_fL offset);
extern  ym_fL   ym_file_getptr              (ym_file *file);
extern  ym_pL   ym_file_wr                  (ym_file *file,const void *T,ym_pL bytes);
extern  ym_pL   ym_file_rd                  (ym_file *file,void *T,ym_pL bytes);
extern  void    ym_file_flush               (ym_file *file);
extern  ym_pL   ym_file_wr_u16              (ym_heap *ah,ym_file *file,const wchar_t *str);
extern  ym_pL   ym_file_rd_u16              (ym_heap *ah,ym_file *file,wchar_t *str,ym_pL L);
////////////////////////////////////////////////////////////////////////////////
extern  ym_pL   ym_file_set_file_length_fails;
extern  void    ym_file_print_error();
extern  int     ym_file_get_error_code();
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
#endif
