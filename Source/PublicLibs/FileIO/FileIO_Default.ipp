/* FileIO_Default.ipp
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
#include <vector>
#include "../ConsoleIO/Label.h"
#include "../StringTools/ToString.h"
#include "FileException.h"
#include "FileIO_Default.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CompileOptions(){
    Console::println_labelm("File IO", "Default", 'R');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
upL_t SetFileLengthFailures = 0;
int GetLastErrorCode(){
    return errno;
}
void PrintLastError(){
    Console::println_labelc("errno", errno);
}
////////////////////////////////////////////////////////////////////////////////
void MakeDirectory(const wchar_t* path){
    std::string path_a = StringTools::w_to_a_direct(path);
#ifdef _MSC_VER
    _mkdir(path_a.c_str());
#else
    mkdir(path_a.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}
void RenameFile(const wchar_t* oldname, const wchar_t* newname){
    std::string path_o = StringTools::w_to_a_direct(oldname);
    std::string path_n = StringTools::w_to_a_direct(newname);

    while (rename(path_o.c_str(), path_n.c_str())){
        Console::Warning("Unable to rename file.", true);
        Console::println(newname);
        Console::println();
        Console::println("Re-attempting...");
        Console::Pause('w');
    }
}
ufL_t GetFileSize(const wchar_t* path){
    BasicFile file(path);

    char buf;

    ufL_t L;
    ufL_t H = 1;

    //  Find upper bound for file size.
    while (1){
        if (file.set_ptr_r(H) != 0 || file.read(&buf, 1) == 0){
            break;
        }
        H <<= 1;
    }
    L = H >> 1;

    //  Binary Search the file size
    while (H - L > 1){
        ufL_t m = (L + H) >> 1;
        if (file.set_ptr_r(m) != 0 || file.read(&buf, 1) == 0){
            H = m;
        }else{
            L = m;
        }
    }

    if (L == 0){
        if (file.set_ptr_r(0) != 0 || file.read(&buf, 1) == 0){
            return 0;
        }
    }

    return H;
}
bool FileExists(const wchar_t* path){
    std::string path_a = StringTools::w_to_a_direct(path);

    FILE *file = fopen(path_a.c_str(), "rb");
    if (file == NULL)
        return false;

    fclose(file);
    return true;
}
void RemoveFile(const wchar_t* path){
    std::string path_a = StringTools::w_to_a_direct(path);
    remove(path_a.c_str());
}
bool DirectoryIsWritable(const wchar_t* directory){
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

    try{
        BasicFile file(0, path.c_str(), false);
        file.ForceDelete();
    }catch (FileException&){
        return false;
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
        std::string path_a = StringTools::w_to_a_direct(path);
        filehandle = fopen(path_a.c_str(), "rb");

        if (filehandle != NULL)
            break;

        if (!retry){
            FileException(errno, nullptr, path, "Unable to open file.").fire();
        }

        Console::Warning("Unable to open file.", true);
        Console::println();
        Console::println(path);
        PrintLastError();
        Console::println();
        Console::print("Re-attempting...");
        Console::println();
        Console::SetColor('w');
        Console::Pause();
    }while (1);
}
BasicFile::BasicFile(ufL_t bytes, const wchar_t* path, bool retry){
    //  Copy the path
    this->path = path;

    do{
        std::string path_a = StringTools::w_to_a_direct(path);
        filehandle = fopen(path_a.c_str(), "wb+");

        if (filehandle != NULL)
            break;

        if (!retry){
            FileException(errno, nullptr, path, "Unable to create file.").fire();
        }

        Console::Warning("Unable to create file.", true);
        Console::println();
        Console::println(path);
        PrintLastError();
        Console::println();
        Console::print("Re-attempting...");
        Console::println();
        Console::SetColor('w');
        Console::Pause();
    }while (1);
}
void BasicFile::Close(){
    if (!IsOpen())
        return;

    while (fclose(filehandle)){
        Console::Warning("Unable to close file.");
        Console::Quit(1);
    }

    path.clear();
}
void BasicFile::Delete(){
    if (!IsOpen())
        return;

    while (fclose(filehandle)){
        Console::Warning("Unable to close file.", true);
        Console::Quit(1);
        Console::println();
        Console::println("Re-attempting...");
        Console::SetColor('w');
        Console::Pause();
        continue;
    }

    RemoveFile(path.c_str());
    path.clear();
}
void BasicFile::ForceDelete(){
    if (!IsOpen())
        return;

    fclose(filehandle);

    std::string path_a = StringTools::w_to_a_direct(path);
    remove(path_a.c_str());

    path.clear();
}
void BasicFile::Rename(const wchar_t* name){
    while (fclose(filehandle)){
        Console::Warning("Unable to close file.", true);
        Console::Quit(1);
        Console::println();
        Console::print("Re-attempting...");
        Console::println();
        Console::SetColor('w');
        Console::Pause();
        continue;
    }

    RenameFile(path.c_str(), name);

    path.clear();
    *this = BasicFile(name);
}
////////////////////////////////////////////////////////////////////////////////
int BasicFile::set_ptr_r(ufL_t offset){
#ifdef _MSC_VER
    return _fseeki64(filehandle, offset, SEEK_SET);
#else
    return fseeko(filehandle, offset, SEEK_SET);
#endif
//    fpos_t pos = (fpos_t)offset;
//    return fsetpos(file->filehandle, &pos);
}
void BasicFile::set_ptr(ufL_t offset){
    if (set_ptr_r(offset)){
        PrintLastError();
        Console::Warning("Unable to move file pointer.");
        Console::Quit(1);
    }
}
upL_t BasicFile::write(const void* T, upL_t bytes){
    return fwrite(T, 1, (size_t)bytes, filehandle);
}
upL_t BasicFile::read(void* T, upL_t bytes){
    return fread(T, 1, (size_t)bytes, filehandle);
}
void BasicFile::flush(){
    fflush(filehandle);
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
        upL_t bytes = write(&buffer[0], length * sizeof(u16_t));
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
