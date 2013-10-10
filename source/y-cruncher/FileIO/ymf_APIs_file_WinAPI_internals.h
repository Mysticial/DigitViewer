/* ymf_APIs_file_WinAPI_internals.h - File IO - WinAPI
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/05/2010
 * Last Modified    : 07/26/2011
 * 
 */

#pragma once
#ifndef _ymf_APIs_file_WinAPI_internals_H
#define _ymf_APIs_file_WinAPI_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "../ConsoleIO/ymo_APIs_cio_headers.h"
#include "../Strings/ymb_APIs_StringTools_headers.h"
#include "../Strings/ymb_APIs_StringBuilder_headers.h"
#include "ymf_APIs_file_WinAPI_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void yma_APIs_file(){
    ymo_print_label("File IO:   \t","WinAPI",'G');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ym_pL ym_file_BLOCK_SIZE = (ym_pL)1 << 30;
ym_pL ym_file_set_file_length_fails = 0;
void ym_file_print_error(){
    DWORD errorcode = GetLastError();
    ymo_print_astr("Windows Error Code: ");
    ymo_print_uL((ym_uL)errorcode);
    ymo_print_astr("    ");
    switch (errorcode){
        case ERROR_ACCESS_DENIED:
            ymo_print_astr("Access Denied");
            break;
        case ERROR_NOT_ENOUGH_MEMORY:
            ymo_print_astr("Not Enough Memory");
            break;
        case ERROR_OUTOFMEMORY:
            ymo_print_astr("Not Enough Memory");
            break;
        case ERROR_DISK_FULL:
            ymo_print_astr("Disk is Full");
            break;
        case ERROR_HANDLE_DISK_FULL:
            ymo_print_astr("Disk is Full");
            break;
        case ERROR_WRITE_FAULT:
            ymo_print_astr("Write Fault");
            break;
        case ERROR_READ_FAULT:
            ymo_print_astr("Read Fault");
            break;
        case ERROR_CRC:
            ymo_print_astr("Cyclic Redundancy Check - Possible Hardware Failure");
            break;
        default:
            ymo_print_astr("Unknown Error, See:\nhttp://msdn.microsoft.com/en-us/library/ms681381(VS.85).aspx");
    }
    ymo_print_astr("\n");
}
int ym_file_get_error_code(){
    return GetLastError();
}
////////////////////////////////////////////////////////////////////////////////
void ym_file_makedir(const wchar_t *path){
    _wmkdir(path);
}
void ym_file_rename_pp(const wchar_t *oldname,const wchar_t *newname){
    while (1){
        if (!_wrename(oldname,newname))
            return;

        errno_t err;
        _get_errno(&err);

        if (err == EEXIST){
    //        ymo_warning(L"Warning: Overwriting existing Checkpoint Swap File...\n");
        }else{
            ymo_print_astr("\nError Code = ");
            ymo_print_uL(err);
            ymo_warning("Unable to rename file.");
            ymo_print_wstr(newname);
            ymo_print_astr("\n\n\n\n");
            ymo_print_astr("Re-attempting...");
            continue;
        }

        if (_wremove(newname)){
            _get_errno(&err);
            ymo_print_astr("\nError Code = ");
            ymo_print_uL(err);
            ymo_warning("Unable to delete file.");
            ymo_print_astr("\n\n");
            ymo_print_astr("Re-attempting...");
            continue;
        }

        if (_wrename(oldname,newname)){
            _get_errno(&err);
            ymo_print_astr("\nError Code = ");
            ymo_print_uL(err);
            ymo_warning("Unable to rename file.");
            ymo_print_astr("\n\n");
            ymo_print_wstr(newname);
            ymo_print_astr("\n\n");
            ymo_print_astr("Re-attempting...");
            continue;
        }
    }
}
ym_fL ym_file_getsize(const wchar_t *path){
    WIN32_FIND_DATAW filedata;
    HANDLE data = FindFirstFileW(path,&filedata);
    if (data == INVALID_HANDLE_VALUE)
        ymo_error("Path does not exist.");

    ym_fL size = ((ym_fL)filedata.nFileSizeHigh << 32) + filedata.nFileSizeLow;
    
    if (!FindClose(data))
        ymo_error("Unable to close file.");

    return size;
}
int ym_file_exist(const wchar_t *path){
    FILE *file;
    if (_wfopen_s(&file,path,L"rb"))
        return 0;
    fclose(file);
    return 1;
}
void ym_file_delete_p(const wchar_t *path){
    _wremove(path);
    //while (_wremove(path)){
    //    ymo_set_color_R(1);
    //    ymo_print_astr("\n");
    //    ymo_print_astr("Unable to delete file: ");
    //    ymo_print_wstr(path);
    //    ymo_print_astr("\n\n\n");
    //    ymo_print_astr("Re-attempting...");
    //    ymo_print_astr("\n");
    //    ymo_set_color_W();
    //    ym_pause();
    //    continue;
    //}
}
int ym_file_check_directory(ym_heap *ah,const wchar_t *directory){
    //Returns:
    // - 0  if directory is writable
    // - 1  if directory is not writable

    //  Build the path
    ym_sbw path;
    ym_sbw_init(ah,&path);

    //  Push the base path
    if (directory != NULL && directory[0] != L'\0'){
        ym_sbw_push_wstr(ah,&path,directory);

        //  Ensure the path ends with a slash.
        wchar_t last = path.buffer[path.len - 1];
        if (last != L'/' && last != L'\\')
            ym_sbw_push_char(ah,&path,L'/');
    }

    //  Push the name
    ym_sbw_push_wstr(ah,&path,L"pathcheck.ysf");
    ym_sbw_push_char(ah,&path,L'\0');

    HANDLE file = CreateFileW(
        path.buffer,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_FLAG_WRITE_THROUGH,
//        NULL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE)
        return 1;
    else{
        while (!CloseHandle(file)){
            ymo_set_color_R(1);
            ymo_error("Unable to close file.");
//            ym_file_print_error();
//            ymo_print_wstr(L"\n");
//            ymo_print_wstr(ymo_str_Reattempting);
//            ymo_print_wstr(L"\n");
//            ymo_set_color_W();
//            ym_pause();
//            continue;
        }
    }

    while (_wremove(path.buffer)){
        ymo_set_color_R(1);
        ymo_print_astr("\n");
        ymo_print_astr("Unable to delete file.");
        ymo_print_astr("\n\n\n");
        ymo_print_astr("Re-attempting...");
        ymo_print_astr("\n");
        ymo_set_color_W();
        ym_pause();
        continue;
    }
    
    ym_sbw_kill(&path);
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
int ym_file_open_r(ym_heap *ah,ym_file *file,const wchar_t *path){

    //  Copy the path
    file->path = ymb_str_w_copy_t(ah,path);

    file->filehandle = CreateFileW(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        NULL,
        NULL
    );

    if (file->filehandle == INVALID_HANDLE_VALUE){
        ym_ah_free(file->path);
        return 1;
    }

    return 0;
}
void ym_file_open(ym_heap *ah,ym_file *file,const wchar_t *path){

    //  Copy the path
    file->path = ymb_str_w_copy_t(ah,path);

    do{
        file->filehandle = CreateFileW(
            path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            NULL,
            NULL
        );
        if (file->filehandle == INVALID_HANDLE_VALUE){
            ymo_print_astr("\n");
            ymo_set_color_R(1);
            ymo_print_astr("Unable to open or create file.");
            ymo_print_astr("\n\n");
            ymo_print_wstr(path);
            ymo_print_astr("\n");
            ym_file_print_error();
            ymo_print_astr("\n");
            ymo_print_astr("Re-attempting...");
            ymo_print_astr("\n");
            ymo_set_color_W();
            ym_pause();
            continue;
        }

        break;
    }while (1);
}
void ym_file_openw(ym_heap *ah,ym_file *file,const wchar_t *path){

    //  Copy the path
    file->path = ymb_str_w_copy_t(ah,path);

    do{
        file->filehandle = CreateFileW(
            path,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            NULL,
            NULL
        );
        if (file->filehandle == INVALID_HANDLE_VALUE){
            ymo_print_astr("\n");
            ymo_set_color_R(1);
            ymo_print_astr("Unable to open or create file.");
            ymo_print_astr("\n\n");
            ymo_print_wstr(path);
            ymo_print_astr("\n");
            ym_file_print_error();
            ymo_print_astr("\n");
            ymo_print_astr("Re-attempting...");
            ymo_print_astr("\n");
            ymo_set_color_W();
            ym_pause();
            continue;
        }

        break;
    }while (1);
}
void ym_file_close(ym_file *file){
    if (file == NULL || file->path == NULL)
        return;

    while (!CloseHandle(file->filehandle)){
        ymo_set_color_R(1);
        ymo_error("Unable to close file.");
//        ym_file_print_error();
//        ymo_print_wstr(L"\n");
//        ymo_print_wstr(ymo_str_Reattempting);
//        ymo_print_wstr(L"\n");
//        ymo_set_color_W();
//        ym_pause();
//        continue;
    }

    ym_ah_free(file->path);
}
int ym_file_create_r(ym_heap *ah,ym_file *file,ym_fL bytes,const wchar_t *path){

    //  Copy the path
    file->path = ymb_str_w_copy_t(ah,path);

    file->filehandle = CreateFileW(
        path,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_FLAG_WRITE_THROUGH,
        NULL
    );
    if (file->filehandle == INVALID_HANDLE_VALUE){
        ym_ah_free(file->path);
        return 1;
    }

    LARGE_INTEGER t;
    t.QuadPart = (LONGLONG)bytes;
    SetFilePointerEx(file->filehandle,t,NULL,FILE_BEGIN);
    SetEndOfFile(file->filehandle);

    return 0;
}
void ym_file_create(ym_heap *ah,ym_file *file,ym_fL bytes,const wchar_t *path){

    //  Copy the path
    file->path = ymb_str_w_copy_t(ah,path);

    do{
        file->filehandle = CreateFileW(
            path,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_ALWAYS,
            FILE_FLAG_WRITE_THROUGH,
            NULL
        );
        if (file->filehandle == INVALID_HANDLE_VALUE){
            ymo_set_color_R(1);
            ymo_print_astr("\n");
            ymo_print_astr("Unable to create file.");
            ymo_print_astr("\n\n");
            ym_file_print_error();
            ymo_print_astr("\n");
            ymo_print_astr("Re-attempting...");
            ymo_print_astr("\n");
            ymo_set_color_W();
            ym_pause();
            continue;
        }
        break;
    }while (1);

    do{
        LARGE_INTEGER t;
        t.QuadPart = (LONGLONG)bytes;
        if (!SetFilePointerEx(file->filehandle,t,NULL,FILE_BEGIN)){
            ymo_set_color_R(1);
            ymo_print_astr("\n");
            ymo_print_astr("Unable to move file pointer.");
            ymo_print_astr("\n\n");
            ym_file_print_error();
            ymo_print_astr("\n");
            ymo_print_astr("Re-attempting...");
            ymo_print_astr("\n");
            ymo_set_color_W();
            ym_pause();
            continue;
        }
        if (!SetEndOfFile(file->filehandle)){
            ymo_print_astr("\n");
            ymo_print_astr("bytes = ");
            ymo_print_uL_commas(bytes,0);
            ymo_print_astr("\n\n");
            ymo_set_color_R(1);
            ymo_print_astr("Unable to allocate swap file.");
            ymo_print_astr("\n\n");
            ym_file_print_error();
            ymo_print_astr("\n");
            ymo_print_astr("Re-attempting...");
            ymo_print_astr("\n");
            ymo_set_color_W();
            ym_pause();
            continue;
        }

        break;
    }while (1);
}
void ym_file_delete(ym_file *file){
    if (file == NULL || file->path == NULL)
        return;

    while (!CloseHandle(file->filehandle)){
        ymo_set_color_R(1);
        ymo_warning("Unable to close file.");
        ym_file_print_error();
        ymo_print_astr("\n");
        ymo_print_astr("Re-attempting...");
        ymo_print_astr("\n");
        ymo_set_color_W();
        ym_pause();
        continue;
    }

    ym_file_delete_p(file->path);
    ym_ah_free(file->path);
}
void ym_file_force_delete(ym_file *file){
    if (file == NULL || file->path == NULL)
        return;

    CloseHandle(file->filehandle);

    _wremove(file->path);

    ym_ah_free(file->path);
}
void ym_file_rename(ym_heap *ah,ym_file *file,const wchar_t *path){
    while (!CloseHandle(file->filehandle)){
        ymo_set_color_R(1);
        ymo_warning("Unable to close file.");
        ym_file_print_error();
        ymo_print_astr("\n");
        ymo_print_astr("Re-attempting...");
        ymo_print_astr("\n");
        ymo_set_color_W();
        ym_pause();
        continue;
    }

    ym_file_rename_pp(file->path,path);

    ym_ah_free(file->path);
    ym_file_open(ah,file,path);
}
////////////////////////////////////////////////////////////////////////////////
void ym_file_setptr(ym_file *file,ym_fL offset){
    LARGE_INTEGER t;
    t.QuadPart = (LONGLONG)offset;
    if (!SetFilePointerEx(file->filehandle,t,NULL,FILE_BEGIN)){
        ym_file_print_error();
        ymo_error("Unable to move file pointer.");
    }
}
ym_fL ym_file_getptr(ym_file *file){
    LARGE_INTEGER zero;
    zero.QuadPart = 0;
    LARGE_INTEGER ptr;
    if (!SetFilePointerEx(file->filehandle,zero,&ptr,FILE_CURRENT)){
        ym_file_print_error();
        ymo_error("Unable to move file pointer.");
    }
    return ptr.QuadPart;
}
ym_pL ym_file_wr(ym_file *file,const void *T,ym_pL bytes){

    ym_pL total_written = 0;

    DWORD bytes_written;
    while (bytes > ym_file_BLOCK_SIZE){
        int ret = !WriteFile(file->filehandle,T,(DWORD)ym_file_BLOCK_SIZE,&bytes_written,NULL);
        if (ret || bytes_written != ym_file_BLOCK_SIZE){
            total_written += bytes_written;
            return total_written;
        }
        total_written += ym_file_BLOCK_SIZE;
        bytes -= ym_file_BLOCK_SIZE;
        T = (void *)((ym_pL)T + ym_file_BLOCK_SIZE);
    }

    WriteFile(file->filehandle,T,(DWORD)bytes,&bytes_written,NULL);
    total_written += bytes_written;
    return total_written;
}
ym_pL ym_file_rd(ym_file *file,void *T,ym_pL bytes){

    ym_pL total_read = 0;

    DWORD bytes_read;
    while (bytes > ym_file_BLOCK_SIZE){
        int ret = !ReadFile(file->filehandle,T,(DWORD)ym_file_BLOCK_SIZE,&bytes_read,NULL);
        if (ret || bytes_read != ym_file_BLOCK_SIZE){
            total_read += bytes_read;
            return total_read;
        }
        total_read += ym_file_BLOCK_SIZE;
        bytes -= ym_file_BLOCK_SIZE;
        T = (void *)((ym_pL)T + ym_file_BLOCK_SIZE);
    }

    ReadFile(file->filehandle,T,(DWORD)bytes,&bytes_read,NULL);
    total_read += bytes_read;
    return total_read;
}
void ym_file_flush(ym_file *file){
    FlushFileBuffers(file->filehandle);
}
ym_pL ym_file_wr_u16(ym_heap *ah,ym_file *file,const wchar_t *str){
    //  Writes as a UTF16 string.
    ym_pL length = ymb_str_w_len(str);

    if (sizeof(wchar_t) == sizeof(ym_u16)){
        ym_pL bytes = ym_file_wr(file,str,length * sizeof(wchar_t));
        return bytes / sizeof(wchar_t);
    }else{
        ym_u16 *buffer = (ym_u16*)ym_ah_malloc(ah,length * sizeof(ym_u16),0);
        for (ym_pL c = 0; c < length; c++){
            buffer[c] = str[c];
        }
        ym_pL bytes = ym_file_wr(file,buffer,length);
        ym_ah_free(buffer);
        return bytes / sizeof(ym_u16);
    }
}
ym_pL ym_file_rd_u16(ym_heap *ah,ym_file *file,wchar_t *str,ym_pL L){
    //  Reads as a UTF16 string.
    memset(str,0,L * sizeof(wchar_t));

    if (sizeof(wchar_t) == sizeof(ym_u16)){
        ym_pL bytes = ym_file_rd(file,str,L * sizeof(wchar_t));
        return bytes / sizeof(wchar_t);
    }else{
        ym_u16 *buffer = (ym_u16*)ym_ah_malloc(ah,L * sizeof(ym_u16),0);
        ym_pL bytes = ym_file_rd(file,buffer,L * sizeof(ym_u16));
        for (ym_pL c = 0; c < L; c++){
            str[c] = buffer[c];
        }
        ym_ah_free(buffer);
        return bytes / sizeof(ym_u16);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
