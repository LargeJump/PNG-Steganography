#include "stegapng.h"
#include "lodepng.h"

#include <stdio.h>
#include <stdlib.h>


//allocate memory to store information of image 
Image* createImage(){
    Image* newImage = malloc(sizeof(Image));
    newImage->image  = NULL;
    newImage->width  = 0;
    newImage->height = 0;

    return newImage;
}

//free image and remove pointers
void freeImage(Image* img){
    free(img->image);
    img->image = NULL; // removes dangling pointer

    free(img);
    img = NULL; //removes dangling pointer
}


void printPNG(const char* filename) {
    unsigned error;
    unsigned char* image = 0;
    unsigned width, height;

    error = lodepng_decode32_file(&image, &width, &height, filename);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

    for(int i=0; i < 30; i++){
        printf("%x", image[i]);
    }
    free(image);
}

Image* decodePNG(const char* filename) {
    unsigned error;
    unsigned char* image = 0;
    unsigned width, height;

    error = lodepng_decode32_file(&image, &width, &height, filename);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

    Image* tmpImage = createImage();
    tmpImage->image = image;
    tmpImage->width = width;
    tmpImage->height = height;

    return tmpImage;
    //rmbr to free image when not using anymore
}

void encodePNG(Image* img){
    unsigned error = lodepng_encode32_file("out.png", img->image, img->width, img->height);
    if(error){
        printf("error saving image\n");
    } else{
        printf("Image Saved\n");
    }
}

//insert character c at beggining of pointer rgba
//c will use be inserted into 4 bytes of rgba
void insertChar(char c, unsigned char* rgba){
    //max num of bytes to use from rgba
    int maxBytes = 4;
    int shift, byte = 8;
    for(int i=0; i<maxBytes; i++ ){
        rgba[i] &= 0xfc;// reset last 2 bits 1111 1100 & rgba -> xxxx xx00
        shift = byte - ((i+1) * 2);//get shift size
        rgba[i] = ((c >> shift) & 0x03) | rgba[i];//set last 2 bits
    }
}

//use first 8 bytes of pixels to store size of message in 2 bytes (int)
void insertStringSize(int n, unsigned char* image){
    char fByte = 0, sByte = 0;
    int byte = 8;
    fByte = n >> byte;
    sByte = n;
    insertChar(fByte, &image[0]);
    insertChar(sByte, &image[4]);
}

void insertString(char* str, int n, Image* img){
    //seperate img into sections to store str
    //4 img bytes = 1 char

    //reserve first 8 bytes to store 2 byte int
    insertStringSize(n, img->image);

    int imgMax = img->width * img->height;
    for(int i = 0, j = 8; i < n  && j < imgMax; i++, j+=4){
        insertChar(str[i], &img->image[j]);
    }

}

//extract 1 char from 4 bytes of data
char extractChar(unsigned char* rgba){
    int max = 4; 
    char byte = 0;
    for(int i=0; i < max; i++){
        byte <<= 2;
        byte += (rgba[i] & 0x03);
    }
    return byte;
}

//check the first 8 bytes to find size of message string stored in png as (2 bytes)
//return size of string message
int extractStringSize(unsigned char* image){
    int n;
    n = extractChar(&image[0]);
    n <<= 8;
    n += extractChar(&image[4]);
    return n;
}

//extract message from png image (img)
//return string that has to be dealocated using free()
char* extractString(Image* img){
    int n = extractStringSize(img->image);
    char* str = malloc(sizeof(char)*n); str[n] = '\0'; // remember to free
    //need safeguard for size of image too 
    for(int i=0, j=8; i < n; i++, j+=4){
        str[i] = extractChar(&img->image[j]);
    }

    return str;
}

