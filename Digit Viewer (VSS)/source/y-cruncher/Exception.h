/* Exception.h - Exceptions
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 08/17/2013
 * 
 * 
 *  This file implements the basic exception and error class used by all of
 *  y-cruncher.
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
#ifndef _yc_Exception_H
#define _yc_Exception_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <exception>
void ymo_print_astr(const char *str);
void ymo_print_wstr(const wchar_t *str);
void ymo_set_color_R(int intense);
void ymo_set_color_W();
void ymo_error(const char *msg);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YM_TERMINATE_THREAD_BARRIER     "FATAL ERROR: Unhandled exception in child thread."
#define YM_TERMINATE_CCPP_BARRIER       "FATAL ERROR: Unhandled exception in C/C++ barrier."
#define YM_EXCEPTION_GENERIC_EXCEPTION  "Generic Exception"
class ym_exception : public std::exception{
public:
    int code;

private:
    //  This object is movable, but not copyable.
//    ym_exception(const ym_exception&)
//    ym_exception& operator=(const ym_exception&);
public:
    ym_exception(ym_exception &&x)
        : code(x.code)
        , str(x.str)
        , astr(std::move(x.astr))
        , wstr(std::move(x.wstr))
        , name(x.name)
    {}
    ym_exception& operator=(ym_exception &&x);
    virtual ~ym_exception() throw (){}

    ym_exception(int code_,const char *str_ = NULL)
        : code(code_)
        , str(str_)
        , name(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(const char *str_)
        : code(0)
        , str(str_)
        , name(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(std::string str_,int code_ = 0)
        : code (code_)
        , str(NULL)
        , astr(std::move(str_))
        , name(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(std::wstring str_,int code_ = 0)
        : code (code_)
        , str(NULL)
        , wstr(std::move(str_))
        , name(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(const char *astr_,std::wstring wstr_,int code_ = 0)
        : code (code_)
        , str(astr_)
        , wstr(std::move(wstr_))
        , name(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}
    ym_exception(std::string astr_,std::wstring wstr_,int code_ = 0)
        : code (code_)
        , str(NULL)
        , astr(std::move(astr_))
        , wstr(std::move(wstr_))
        , name(YM_EXCEPTION_GENERIC_EXCEPTION)
    {}

    void print() const{
        ymo_set_color_R(1);
        ymo_print_astr("\n\nException Encountered: ");
        ymo_print_astr(name);
        ymo_print_astr("\nError Code: ");
        ymo_print_astr(std::to_string(code).c_str());
        ymo_print_astr("\n");
        if (str != NULL){
            ymo_print_astr(str);
        }
        if (astr.size() != 0){
            ymo_print_astr("\n");
            ymo_print_astr(astr.c_str());
        }
        if (wstr.size() != 0){
            ymo_print_astr("\n");
            ymo_print_wstr(wstr.c_str());
        }
        ymo_print_astr("\n\n");
        ymo_set_color_W();
    }

private:
    const char *str;
    std::string astr;
    std::wstring wstr;

protected:
    const char *name;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ym_error : public std::exception{
public:
    ym_error(ym_exception e_,const char *msg = NULL)
        : e(std::move(e_))
        , astr(msg)
    {}

private:
    //  This object is movable, but not copyable.
//    ym_error(const ym_error&)
//    ym_error& operator=(const ym_error&);
public:
    ym_error(ym_error &&x)
        : e(std::move(x.e))
    {}
    ym_error& operator=(ym_error &&x);
    virtual ~ym_error() throw (){}

    void print() const{
        ymo_set_color_R(1);
        ymo_print_astr("\n\nAn unrecoverable error has occurred.\n\n");

        ymo_print_astr("Attached Exception:");
        e.print();

        if (astr != NULL){
            ymo_set_color_R(1);
            ymo_print_astr(astr);
            ymo_print_astr("\n\n");
            ymo_set_color_W();
        }
    }

private:
    ym_exception e;
    const char *astr;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
