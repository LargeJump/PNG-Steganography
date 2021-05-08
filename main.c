
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lodepng.h"
#include "stegapng.h"


typedef enum{HELP, READ, WRITE} Option;
Option checkOptions(char* opt){
   if(strcmp(opt, "-r") == 0){
       return READ;
   }
   else if(strcmp(opt, "-w") == 0){
       return WRITE;
   }
   else{
       return HELP;
   }
}

bool insertMessage(char* message, const char* filename){
    Image* img = decodePNG(filename);
    insertString(message, strlen(message), img);

    encodePNG(img);
    freeImage(img);

    return true;
}

bool readMessage(const char* filename){
    Image* img = decodePNG(filename);
    char* message = extractString(img);

    printf("Message :\n%s\n", message);

    free(message);
    message = NULL;

    return true;
}

void helpMessage(){
    printf("\noptions :\n-r\t read\n-w\t write\n");
    printf("\nread : -r <filename>\n");
    printf("\nwrite: -w <filename> <message>\n\n");
}

int main(int argc, char* argv[]){
    if(argc < 2 || argc > 4){
        printf("Invalid input\n");
        helpMessage();
        return -1;
    }

    Option option = checkOptions(argv[1]);
    switch(option){
        case(READ):
            readMessage(argv[2]);
            break;
        case(WRITE):
            insertMessage(argv[3], argv[2]);
            break;
        case(HELP):
            helpMessage();
            break;
    }


    return 0;
}