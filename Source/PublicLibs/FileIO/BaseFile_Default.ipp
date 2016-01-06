/* BaseFile_Default.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 12/30/2015
 * Last Modified    : 12/30/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/StringTools/Unicode.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "BaseFile_Default.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  BaseFile
bool BaseFile::open(std::string path){
    close();

    filehandle = fopen(path.c_str(), "rb");
    if (filehandle == nullptr)
        return false;

    this->path = std::move(path);
    return true;
}
bool BaseFile::create(std::string path, ufL_t bytes){
    close();

    filehandle = fopen(path.c_str(), "wb+");
    if (filehandle == nullptr)
        return false;

    if (bytes != 0){
        //  TODO
    }

    this->path = std::move(path);
    return true;
}
void BaseFile::close(bool delete_file){
    if (path.empty())
        return;

    fclose(filehandle);

    if (delete_file){
        remove(path.c_str());
    }

    path.clear();
}
bool BaseFile::set_ptr(ufL_t offset){
#ifdef _MSC_VER
    return _fseeki64(filehandle, offset, SEEK_SET) == 0;
#else
    return fseeko(filehandle, offset, SEEK_SET) == 0;
#endif
}
void BaseFile::flush(){
    fflush(filehandle);
}
upL_t BaseFile::read(void* T, upL_t bytes){
    return fread(T, 1, (size_t)bytes, filehandle);
}
upL_t BaseFile::write(const void* T, upL_t bytes){
    return fwrite(T, 1, (size_t)bytes, filehandle);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
