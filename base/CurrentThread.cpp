#include "CurrentThread.h"

namespace TCB
{
    namespace CurrentThread
    {
        thread_local int t_cachedTid = 0;
        thread_local char t_tidString[32] = {0};
        thread_local std::size_t t_tidLength = 0;
    } // namespace CurrentThread
    
} // namespace TCB
