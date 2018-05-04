
#ifndef OCR_TYPE_H_
#define OCR_TYPE_H_


typedef struct ocr_input
{
    int rows;
    int cols;
    int iter;
    int payload[]; 
} ocr_input_t;



#endif // OCR_TYPE_H_
