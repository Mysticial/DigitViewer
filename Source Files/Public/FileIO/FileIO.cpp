#include "FileIO.h"

#ifdef _WIN32
#include "FileIO_WinAPI.ipp"
#else
#include "FileIO_Default.ipp"
#endif
