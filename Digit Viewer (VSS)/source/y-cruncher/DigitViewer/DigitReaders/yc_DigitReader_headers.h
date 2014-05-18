/* yc_DigitReader_headers.h - File Reader Object Interface
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/06/2012
 * Last Modified    : 07/28/2013
 * 
 * 
 *  This is an abstract class that represents a digit reader that streams digits
 *  from a source.
 * 
 *  This the parent class of:
 *      -   TextReader  (reads the simple .txt files of plain-text digits)
 *      -   YCDReader   (reads the compressed digit .ycd format files)
 * 
 *  Each of these classes have their own constructors. But once created they
 *  share this common interface for all digit reading/streaming purposes.
 */

#pragma once
#ifndef _yc_DigitReader_headers_H
#define _yc_DigitReader_headers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include <memory>
#include "../DigitViewer_globals.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YC_DIGITREADER_DEFAULT_BUFFER   ((ym_pL)64 << 20)
class DigitReader{
public:
    DigitReader();
    virtual ~DigitReader    (){}

    /*  Iterator methods
     * 
     *  The following 3 methods act as iterators for streaming digits.
     * 
     *  Note that these iterator methods require an internal buffer that is
     *  instantiated upon first use. If these are never used, the buffer is
     *  is never made. So this no memory will be wasted if you only intend to
     *  use the raw read() method.
     */

    //  Sets the digit position of this reader.
    void    set_pos     (ym_uL offset);

    //  Returns the next digit.
    char    next        ();

    //  Returns the next N digits. (Reads "digits" digits and stores them into "str".)
    //  For large N, this function is faster than calling "next()" N times.
    void    read        (char *str,ym_pL digits);



    //  Prints out the contents of the reader. (For debugging)
    virtual void        print           () const = 0;

    //  Returns the radix of the digits. (10 or 16)
    virtual int         get_radix       () const = 0;

    //  Returns the # of digits in this digit stream. (0 = unknown)
    virtual ym_fL       get_digits      () const = 0;

    //  Enable/Disable raw output.
    virtual void        set_raw         (bool raw) = 0;

    //  Checks to see if the range [start, end) is accessible.
    virtual bool        check_range     (ym_uL start,ym_uL end) = 0;

    //  Returns the first few digits.
    virtual std::string get_first_digits(ym_pL L) = 0;


    /*  Reads N digits starting from an offset of "pos".
     * 
     *  Note that this read function does not do internal buffering of the 
     *  decompressed digits. It reads directly from the disk into a raw buffer.
     *  Then it formats/decompresses them directly to "str".
     * 
     *  Since this function does not touch the internal buffer, it will not
     *  invalidate the iterator state.
     * 
     *  For performance reasons, the best way to use this function is:
     *      -   Grab the entire desired region of digits with a single call.
     *      -   Stream a very large number of digits with multiple calls where
     *          "digits" is large. (> ~50,000,000)
     * 
     *  This function should NOT be used to stream digits a little at a time.
     *  Use the other read() function for that purpose.
     * 
     * 
     *  Each call to this function usually involves multiple disk seeks and a
     *  large number of integer divisions. So it has very high calling overhead.
     * 
     *  But once this function is called, it has the highest steady-state
     *  conversion rate of all the read functions. So its main purpose is to
     *  stream a very large number of digits.
     * 
     *  While this function will work correctly for any value "digits", it is
     *  most efficient when "digits > ~50,000,000" - subject to the latency
     *  and sequential bandwidth of the hard drive.
     */
    virtual void        read            (ym_uL pos,char *str,ym_pL digits) = 0;

private:
    DigitReader(const DigitReader&);
    void operator=(const DigitReader&);

protected:

    //  Digit Buffer
    std::unique_ptr<char[]> buffer;
    ym_pL buffer_L;
    
    //  Iterator
    ym_uL iter_f_offset;
    ym_pL iter_b_offset;

    //  Reload the buffer
    //  If the subclass supports unknown # of digits (get_digits() == 0),
    //  this it MUST override this function.
    virtual void reload();

    //  Clear the buffer
    void clear_buffer();
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline char DigitReader::next(){
    //  This is a VERY performance critical function call.
    //  So put the implementation here to help the compiler inline it.

    //  Buffer is empty
    if (iter_b_offset == buffer_L){
        reload();
    }
    
    iter_f_offset++;
    return buffer[iter_b_offset++];
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Digit Viewer Factory
//      Call this on any digit file and it will return the digit viewer for it.
std::unique_ptr<DigitReader> OpenDigitFile(
    std::wstring path,
    bool raw = false,
    ym_pL buffer_size = YC_DIGITREADER_DEFAULT_BUFFER
);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
