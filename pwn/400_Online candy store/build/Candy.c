//
//  Candy.c
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 30..
//

#include "Global.h"
#include "Candy.h"

void setCandy(){
    int cnt;
    char candiesNames[10][8] = {"Red","Orange","Yellow","Green","Blue","Indigo","Purple","Black","White","Pink"};
    int candiesValues[10] = {10,10,10,10,10,10,10,10,10,10};

    for (cnt = 0; cnt < CANDISECNT; cnt++) {
        gCandies[cnt] = malloc(sizeof(struct CANDIES));
        strncpy(gCandies[cnt]->candyName, candiesNames[cnt],sizeof(gCandies[cnt]->candyName));
        gCandies[cnt]->orderNumber = candiesValues[cnt];
        gCandies[cnt]->candyCode = cnt;
    }
}

void getCandy(){
    int cnt;
    printf("=*= Candies =*=\n");
    for(cnt = 0; cnt < CANDISECNT; cnt++) {
        printf("\nCandy code  : %d\n",gCandies[cnt]->candyCode);
        printf("Candy taste : %s\n",gCandies[cnt]->candyName);
        printf("Order number : %d\n",gCandies[cnt]->orderNumber);
        printf("Candy code  : %d\n",gCandies[cnt]->candyCode);
    }
}
