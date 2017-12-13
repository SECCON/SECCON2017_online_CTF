//
//  UserInput.c
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 26..
//

#include "Global.h"
#include "UserInput.h"

int UserInput(char *buf, int size){
    int result = 0;
    
    fflush(0);

    result = read(0,buf,size);
    return result;
}

unsigned int retNumber(unsigned len){
    char tmp[len];
    unsigned int ret = 0;
    
    fflush(0);

    read(0,tmp,len);
    ret = atoi(tmp);
    return ret;
}
