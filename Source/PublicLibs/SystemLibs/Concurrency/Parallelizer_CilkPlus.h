/* Parallelizer_CilkPlus.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/19/2017
 * Last Modified    : 02/19/2017
 * 
 */

#pragma once
#ifndef ymp_Concurrency_Parallelizer_CilkPlus_H
#define ymp_Concurrency_Parallelizer_CilkPlus_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <cilk/cilk_api.h>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/Exceptions/Exception.h"
#include "PublicLibs/BasicLibs/Concurrency/BasicParallelizer.h"
namespace ymp{
namespace BasicFrameworks{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class CilkPlus : public BasicParallelizer{
public:
    virtual void run_in_parallel(BasicAction& a0, BasicAction& a1) override{
        _Cilk_spawn run(a0, 0);
        a1.run();
    }
    virtual void run_in_parallel(BasicAction& action, upL_t si, upL_t ei) override{
        #pragma cilk grainsize = 1
        _Cilk_for (upL_t c = si; c < ei; c++){
            run(action, c);
        }
    }

private:
    //  COMPILER-BUG-ICC: ICE if this function gets inlined.
    YM_NO_INLINE static void run(BasicAction& action, upL_t index){
        try{
            action.run(index);
        }catch (Exception& e){
            e.print();
            Console::Quit(1);
        }catch (std::exception& e){
            Console::Warning(e.what());
            Console::Quit(1);
        }catch (...){
            Console::Warning(EXCEPTION_THREAD_BARRIER);
            Console::Quit(1);
        }
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
