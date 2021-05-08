

//-------------------

/*
* used to store information regarding uncompressed
* png image 
*/
typedef struct{
    unsigned char* image;
    unsigned width;
    unsigned height;
} Image;

Image* createImage();

void freeImage();

//-------------------

Image* decodePNG(const char* filename);

void encodePNG(Image* img);

void insertChar(char c, unsigned char* rgba);

void insertStringSize(int n, unsigned char* image);

void insertString(char* str, int n, Image* img);

char extractChar(unsigned char* rgba);

int extractStringSize(unsigned char* image);

char* extractString(Image* img);


//-------------------