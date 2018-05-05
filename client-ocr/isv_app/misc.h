#ifndef MISC_H
#define MISC_H

#undef DEBUG
//#define MYDEBUG 2
#ifdef MYDEBUG
    #define PRINT(...) printf( __VA_ARGS__)
//    #define PRINT(...) printf("checking\n");
#else
    #define PRINT(...) 
#endif

#endif /* MISC_H */
