#ifndef OCR_H_
#define OCR_H_


typedef struct ocr_input
{
    uint8_t rows;
    uint8_t cols;
    int payload[]; 
} ocr_input_t;


#endif // OCR_H_