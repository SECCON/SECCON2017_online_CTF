//
//  Purchase.c
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 30..
//
#include "Global.h"
#include "UserInput.h"
#include "Board.h"

void reSortStock(int cnt){
    int add;

    free(gStock[cnt]->candyDescription);
    free(gStock[cnt]);

    if(cnt < gStockCnt){
        for(add = 0; cnt + add < gStockCnt; add++){
            gStock[cnt + add] = gStock[cnt + add + 1];
        }
    }

    gStock[gStockCnt] = 0;
    gStockCnt--;
    printf("The candy has just sold out.\n");
}

void purchase(){
    unsigned int candyCode, candyNum;

    if(gStockCnt){
        printf("Please enter the code number of the candy to be purchased.\n");
        candyCode = retNumber(3);
        if(candyCode < gStockCnt){
            printf("Please enter the number of the candy to purchase.\n");
            candyNum = retNumber(3);
            if(gStock[candyCode]->candyNumber >= candyNum){
                if(gStock[candyCode]->candyPrice * candyNum <= gLoginAccount->bk){
                    gStock[candyCode]->candyNumber -= candyNum;
                
                    if(!gStock[candyCode]->candyNumber){
		        printf("Thank you for your purchase.(%ld)\n",gStock[candyCode]->candyPrice * candyNum);
                        reSortStock(candyCode);
                        setBoard();
                    }
                }else{
                    printf("You do not have enough money.(%ld)\n",gStock[candyCode]->candyPrice * candyNum);
                }
            }else if(gStock[candyCode]->candyNumber < candyNum){
                printf("There is not enough stock.\n");
            }
        }
    }else{
        printf("We have not any candy.\n");
    }
}
