/* ParseException.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/11/2017
 * Last Modified    : 04/11/2017
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "ParseException.h"
//#include "ParseThrower.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char ParseException::TYPENAME[] = "ParseException";
ExceptionFactoryT<ParseException> ParseException_Instance;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
[[noreturn]] YM_NO_INLINE
void throw_ParseException(const char* message){
    throw ParseException(message);
}
[[noreturn]] YM_NO_INLINE
void throw_ParseException(const char* function, const char* message){
    throw ParseException(function, message);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}