/* u32h8_forward_convert_Default.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/20/2011
 * Last Modified    : 01/23/2016
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "u32h8_forward_intrinsics_Default.h"
#include "u32h8.h"
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void u32r_to_h8r(char* T, const u32_t* A, upL_t AL){
    T += AL * 8;
    const u32_t* stopA = A + AL;

    while (A < stopA - 1){
        T -= 16;
        u32h8::convert_h8x2_forward_Default(T, A);
        A += 2;
    }
    while (A < stopA){
        T -= 8;
        u32h8::convert_h8x1_forward_Default(T, *A);
        A += 1;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
