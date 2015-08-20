/* FileIO_WinAPI.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/05/2010
 * Last Modified    : 07/26/2011
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <errno.h>
#include <windows.h>
#include <vector>
#include "../CompilerSettings.h"
#include "../ConsoleIO/Label.h"
#include "FileException.h"
#include "FileIO_WinAPI.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CompileOptions(){
    Console::println_labelm("File IO", "WinAPI", 'G');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const upL_t BLOCK_SIZE = (upL_t)1 << 30;
upL_t SetFileLengthFailures = 0;
int GetLastErrorCode(){
    return GetLastError();
}
void PrintLastError(){
    DWORD errorcode = GetLastError();
    Console::print_labelc("Windows Error Code", errorcode);
    Console::print("    ");
    switch (errorcode){
        case ERROR_ACCESS_DENIED:
            Console::print("Access Denied");
            break;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            Console::print("Not Enough Memory");
            break;
        case ERROR_DISK_FULL:
        case ERROR_HANDLE_DISK_FULL:
            Console::print("Disk is Full");
            break;
        case ERROR_WRITE_FAULT:
            Console::print("Write Fault");
            break;
        case ERROR_READ_FAULT:
            Console::print("Read Fault");
            break;
        case ERROR_CRC:
            Console::print("Cyclic Redundancy Check - Possible Hardware Failure");
            break;
        default:
            Console::print("Unknown Error, See:\nhttp://msdn.microsoft.com/en-us/library/ms681381(VS.85).aspx");
    }
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
void MakeDirectory(const wchar_t* path){
    _wmkdir(path);
}
void RenameFile(const wchar_t* oldname, const wchar_t* newname){
    while (1){
        if (!_wrename(oldname, newname))
            return;

        errno_t err;
        _get_errno(&err);

        if (err == EEXIST){
    //        Console::println("\nWarning: Overwriting existing Checkpoint Swap File...");
        }else{
            Console::Warning("Unable to rename file.", true);
            Console::println_labelc("Error Code", err);
            Console::println(newname);
            Console::println();
            Console::println("Re-attempting...");
            Console::SetColor('w');
            continue;
        }

        if (_wremove(newname)){
            _get_errno(&err);
            Console::Warning("Unable to delete file.", true);
            Console::println_labelc("Error Code", err);
            Console::println();
            Console::println("Re-attempting...");
            Console::SetColor('w');
            continue;
        }

        if (_wrename(oldname, newname)){
            _get_errno(&err);
            Console::Warning("Unable to rename file.", true);
            Console::println_labelc("Error Code", err);
            Console::println(newname);
            Console::println();
            Console::println("Re-attempting...");
            Console::SetColor('w');
            continue;
        }
    }
}
ufL_t GetFileSize(const wchar_t* path){
    WIN32_FIND_DATAW filedata;
    HANDLE data = FindFirstFileW(path, &filedata);
    if (data == INVALID_HANDLE_VALUE){
        Console::Warning("Path does not exist.");
        Console::Quit(1);
    }

    ufL_t size = ((ufL_t)filedata.nFileSizeHigh << 32) + filedata.nFileSizeLow;
    
    if (!FindClose(data)){
        Console::Warning("Unable to close file.");
        Console::Quit(1);
    }

    return size;
}
bool FileExists(const wchar_t* path){
    FILE *file;
    if (_wfopen_s(&file, path, L"rb"))
        return false;
    fclose(file);
    return true;
}
void RemoveFile(const wchar_t* path){
    _wremove(path);
}
bool DirectoryIsWritable(const wchar_t* directory){
    //Returns:
    // - 0  if directory is writable
    // - 1  if directory is not writable

    //  Build the path
    std::wstring path;

    //  Push the base path
    if (directory != NULL && directory[0] != L'\0'){
        path += directory;

        //  Ensure the path ends with a slash.
        wchar_t last = path.back();
        if (last != L'/' && last != L'\\')
            path += L'/';
    }

    //  Push the name
    path += L"pathcheck.ysf";

    HANDLE file = CreateFileW(
        path.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_FLAG_WRITE_THROUGH,
//        NULL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE)
        return false;
    else{
        while (!CloseHandle(file)){
            Console::Warning("Unable to close file.");
            Console::Quit(1);
        }
    }

    while (_wremove(path.c_str())){
        Console::Warning("Unable to delete file.", true);
        Console::println();
        Console::println("Re-attempting...");
        Console::Pause('w');
        continue;
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////
BasicFile::BasicFile(BasicFile&& x)
    : path(std::move(x.path))
    , filehandle(std::move(x.filehandle))
{
    x.path.clear();
}
void BasicFile::operator=(BasicFile&& x){
    path = std::move(x.path);
    filehandle = std::move(x.filehandle);
    x.path.clear();
}
BasicFile::BasicFile(const wchar_t* path, bool retry){
    //  Copy the path
    this->path = path;

    do{
        filehandle = CreateFileW(
            path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            NULL,
            NULL
        );

        if (filehandle != INVALID_HANDLE_VALUE)
            break;

        if (!retry){
            FileException(GetLastError(), nullptr, path, "Unable to open file.").fire();
        }

        Console::Warning("Unable to open file.", true);
        Console::println(path);
        PrintLastError();
        Console::println();
        Console::println("Re-attempting...");
        Console::Pause('w');
    }while (1);
}
BasicFile::BasicFile(ufL_t bytes, const wchar_t* path, bool retry){
    //  Copy the path
    this->path = path;

    do{
        filehandle = CreateFileW(
            path,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_ALWAYS,
            FILE_FLAG_WRITE_THROUGH,
            NULL
        );

        if (filehandle != INVALID_HANDLE_VALUE){
            LARGE_INTEGER t;
            t.QuadPart = (LONGLONG)bytes;
            SetFilePointerEx(filehandle, t, NULL, FILE_BEGIN);
            SetEndOfFile(filehandle);
            break;
        }

        if (!retry){
            FileException(GetLastError(), nullptr, path, "Unable to create file.").fire();
        }

        Console::Warning("Unable to create file.", true);
        Console::println(path);
        PrintLastError();
        Console::println();
        Console::println("Re-attempting...");
        Console::Pause('w');
    }while (1);
}
void BasicFile::Close(){
    if (!IsOpen())
        return;

    while (!CloseHandle(filehandle)){
        Console::Warning("Unable to close file.");
        Console::Quit(1);
    }

    path.clear();
}
void BasicFile::Delete(){
    if (!IsOpen())
        return;

    while (!CloseHandle(filehandle)){
        Console::Warning("Unable to close file.", true);
        PrintLastError();
        Console::println();
        Console::println("Re-attempting...");
        Console::Pause('w');
        continue;
    }

    RemoveFile(path.c_str());
    path.clear();
}
void BasicFile::ForceDelete(){
    if (!IsOpen())
        return;
    CloseHandle(filehandle);
    _wremove(path.c_str());
    path.clear();
}
void BasicFile::Rename(const wchar_t* name){
    if (!IsOpen())
        return;

    while (!CloseHandle(filehandle)){
        Console::Warning("Unable to close file.", true);
        PrintLastError();
        Console::println();
        Console::println("Re-attempting...");
        Console::Pause('w');
        continue;
    }

    RenameFile(path.c_str(), name);

    path.clear();
    *this = BasicFile(name);
}
////////////////////////////////////////////////////////////////////////////////
void BasicFile::set_ptr(ufL_t offset){
    LARGE_INTEGER t;
    t.QuadPart = (LONGLONG)offset;
    if (!SetFilePointerEx(filehandle, t, NULL, FILE_BEGIN)){
        PrintLastError();
        Console::Warning("Unable to move file pointer.");
        Console::Quit(1);
    }
}
upL_t BasicFile::write(const void* T, upL_t bytes){

    upL_t total_written = 0;

    DWORD bytes_written;
    while (bytes > BLOCK_SIZE){
        int ret = !WriteFile(filehandle, T, (DWORD)BLOCK_SIZE, &bytes_written, NULL);
        if (ret || bytes_written != BLOCK_SIZE){
            total_written += bytes_written;
            return total_written;
        }
        total_written += BLOCK_SIZE;
        bytes -= BLOCK_SIZE;
        T = (void*)((upL_t)T + BLOCK_SIZE);
    }

    WriteFile(filehandle, T, (DWORD)bytes, &bytes_written, NULL);
    total_written += bytes_written;
    return total_written;
}
upL_t BasicFile::read(void* T, upL_t bytes){
//    Console::println("asdf");

    upL_t total_read = 0;

    DWORD bytes_read;
    while (bytes > BLOCK_SIZE){
        int ret = !ReadFile(filehandle, T, (DWORD)BLOCK_SIZE, &bytes_read, NULL);
        if (ret || bytes_read != BLOCK_SIZE){
            total_read += bytes_read;
            return total_read;
        }
        total_read += BLOCK_SIZE;
        bytes -= BLOCK_SIZE;
        T = (void*)((upL_t)T + BLOCK_SIZE);
    }

    ReadFile(filehandle, T, (DWORD)bytes, &bytes_read, NULL);
    total_read += bytes_read;
    return total_read;
}
void BasicFile::flush(){
    FlushFileBuffers(filehandle);
}
upL_t BasicFile::write_u16(const wchar_t* str){
    //  Writes as a UTF16 string.
    upL_t length = wcslen(str);

    if (sizeof(wchar_t) == sizeof(u16_t)){
        upL_t bytes = write(str, length * sizeof(wchar_t));
        return bytes / sizeof(wchar_t);
    }else{
        std::vector<u16_t> buffer(length);
        for (upL_t c = 0; c < length; c++){
            buffer[c] = str[c];
        }
        upL_t bytes = write(&buffer[0], length);
        return bytes / sizeof(u16_t);
    }
}
upL_t BasicFile::read_u16(wchar_t* str, upL_t L){
    //  Reads as a UTF16 string.
    memset(str, 0, L * sizeof(wchar_t));

    if (sizeof(wchar_t) == sizeof(u16_t)){
        upL_t bytes = read(str, L * sizeof(wchar_t));
        return bytes / sizeof(wchar_t);
    }else{
        std::vector<u16_t> buffer(L);
        upL_t bytes = read(&buffer[0], L * sizeof(u16_t));
        for (upL_t c = 0; c < L; c++){
            str[c] = buffer[c];
        }
        return bytes / sizeof(u16_t);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
