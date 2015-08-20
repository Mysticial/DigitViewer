/* FileIO_Default.h - File IO - Default
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/05/2010
 * Last Modified    : 07/26/2011
 * 
 */

#pragma once
#ifndef _ymp_FileIO_Default_H
#define _ymp_FileIO_Default_H
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
#include <string>
#include "../Types.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicFile{
    std::wstring path;      //  Path of the file

public:
    FILE *filehandle;       //  File handle

public:
    BasicFile(const BasicFile&) = delete;
    void operator=(const BasicFile&) = delete;
    BasicFile(BasicFile&& x);
    void operator=(BasicFile&& x);

public:
    BasicFile(){};
    BasicFile(const wchar_t* path, bool retry = false);                 //  Open for read only.
    BasicFile(ufL_t bytes, const wchar_t* path, bool retry = false);    //  Create for read + write.
    ~BasicFile(){ Close(); }

    void Close();
    void Delete();
    void ForceDelete();

    const wchar_t* GetPath() const{ return path.c_str(); }
    bool IsOpen() const{ return !path.empty(); }
    void Rename(const wchar_t* name);

    int set_ptr_r(ufL_t offset);
    void set_ptr(ufL_t offset);

    upL_t write(const void* T, upL_t bytes);
    upL_t read(void* T, upL_t bytes);
    void flush();

    upL_t write_u16(const wchar_t* str);
    upL_t read_u16(wchar_t* str, upL_t L);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
