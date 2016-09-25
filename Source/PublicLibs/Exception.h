/* Exception.h - Exceptions
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 09/20/2015
 * 
 *      This current exception system is a rats nest and needs to be completely
 *  redone in a better way. But for now, it's better than nothing.
 * 
 * 
 *  This file implements the basic exception and error class used by all of
 *  y-cruncher and related projects.
 * 
 *  The exception class has a bunch of constructors for convenience.
 *  Every module defines its own set of sub-class exceptions and can catch and
 *  handle them as desired.
 * 
 *  The error class is meant to not be caught. This is reserved for errors that
 *  can never be safely recovered. The typical use-case is for a critical
 *  operation to throw an exception leaving the program in an inconsistent
 *  state that cannot be repaired. This exception is them caught and then
 *  wrapped into the error class and rethrown to be propagated to the top of the
 *  program.
 */

#pragma once
#ifndef ymp_Exception_H
#define ymp_Exception_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <memory>
#include <exception>
#include "CompilerSettings.h"
#include "ConsoleIO/Label.h"
//#include "ErrorHandling.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YM_TERMINATE_THREAD_BARRIER     "FATAL ERROR: Unhandled exception in child thread."
#define YM_TERMINATE_CCPP_BARRIER       "FATAL ERROR: Unhandled exception in C/C++ barrier."
#define YM_EXCEPTION_GENERIC_EXCEPTION  "Generic Exception"
class ym_exception : public std::exception{
public:
    int m_code;

private:
    //  COMPILER-BUG-VS:    //  Throw using move-semantecs.
    //  Visual Studio is unable to throw objects using move-semantecs.
    //  https://connect.microsoft.com/VisualStudio/feedback/details/688012/vc10-compiler-cannot-throw-move-only-objects

    //  This object is movable, but not copyable.
//    ym_exception(const ym_exception&) = delete;
//    ym_exception& operator=(const ym_exception&) = delete;
public:
    //ym_exception(ym_exception&& x)
    //    : code(x.code)
    //    , str(x.str)
    //    , astr(std::move(x.astr))
    //    , wstr(std::move(x.wstr))
    //    , name(x.name)
    //{}
    //ym_exception& operator=(ym_exception&& x);
    virtual ~ym_exception() noexcept{}

    ym_exception(int code, const char* str = NULL)
        : m_code(code)
        , m_str(str)
        , m_message(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(const char* str)
        : m_code(0)
        , m_str(str)
        , m_message(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(std::string str, int code = 0)
        : m_code(code)
        , m_str(nullptr)
        , m_astr(std::move(str))
        , m_message(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(std::wstring str, int code = 0)
        : m_code(code)
        , m_str(nullptr)
        , m_wstr(std::move(str))
        , m_message(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(const char* astr, std::wstring wstr, int code = 0)
        : m_code(code)
        , m_str(astr)
        , m_wstr(std::move(wstr))
        , m_message(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(std::string astr, std::wstring wstr, int code = 0)
        : m_code(code)
        , m_str(nullptr)
        , m_astr(std::move(astr))
        , m_wstr(std::move(wstr))
        , m_message(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}

    void print() const{
        Console::println("\n");
        Console::println_labelc("Exception Encountered", m_message, 'R');
        Console::print("Error Code: ");
        Console::println(std::to_string(m_code));
        if (m_location != nullptr){
            Console::print("Location: ");
            Console::println(m_location);
        }
        if (m_str != NULL){
            Console::println(m_str);
        }
        if (m_astr.size() != 0){
            Console::println(m_astr);
        }
        if (m_wstr.size() != 0){
            Console::println(m_wstr);
        }
        Console::println("\n");
        Console::SetColor('w');
    }

    virtual const char* what() const noexcept override{
        if (m_str != nullptr)
            return m_str;
        return m_astr.c_str();
    }

protected:
    const char* m_str;
    std::string m_astr;
    std::wstring m_wstr;

protected:
    int m_level = 3;

    const char* m_message;
    const char* m_location = nullptr;

    ym_exception()
        : m_str(nullptr)
    {}

public:
    void fire() const{
        if (m_level >= 3){
            print();
            fire_this();
        }if (m_level >= 2){
            fire_this();
        }else if (m_level >= 1){
            print();
            Console::Quit(1);
        }
    }

private:
    virtual void fire_this() const{
        throw *this;
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ym_error : public std::exception{
public:
    ym_error(ym_exception e_, const char* msg = NULL)
        : e(std::move(e_))
        , astr(msg)
    {}

private:
    //  This object is movable, but not copyable.
//    ym_error(const ym_error&) = delete;
//    ym_error& operator=(const ym_error&) = delete;
public:
//    ym_error(ym_error&& x)
//        : e(std::move(x.e))
//    {}
//    ym_error& operator=(ym_error&& x);
    virtual ~ym_error() noexcept{}

    void print() const{
        Console::SetColor('R');
        Console::println("\n\nAn unrecoverable error has occurred.\n");

        Console::print("Attached Exception:");
        e.print();

        if (astr != NULL){
            Console::println(astr, 'R');
            Console::println();
            Console::SetColor('w');
        }
    }

private:
    ym_exception e;
    const char* astr;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
