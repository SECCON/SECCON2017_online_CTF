//
//  Stock.c
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 30..
//

#include "Global.h"

void printStock(){
    int i;
    if(gStockCnt){
        for(i = 0;i < gStockCnt;i++){
            printf("\n=*= Candy %d =*=\n",i);
            printf("Name of candy        : %s\n",gStock[i]->candyName);
            printf("Number of candy      : %d\n",gStock[i]->candyNumber);
            printf("Price of candy       : %d\n",gStock[i]->candyPrice);
            printf("Description of candy : %s\n",gStock[i]->candyDescription);
        }
    }else{
        printf("We have not any candy.\n");
    }
}
