#ifndef MISC_H
#define MISC_H

//#undef DEBUG
#define MYDEBUG 2
#ifdef MYDEBUG
    #define PRINT(...) fprintf(stderr, __VA_ARGS__)
//    #define PRINT(...) printf("checking\n");
#else
    #define PRINT(...) 
#endif


typedef struct ocr_input
{
    int rows;
    int cols;
    int iter;
    int iter_max;
    int payload[]; 
} ocr_input_t;



#endif // MISC_H


