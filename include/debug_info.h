#ifndef DEBUG_INF
#define DEBUG_INF

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(arg) arg
#else
    #define ON_DEBUG(arg)
#endif

#endif
