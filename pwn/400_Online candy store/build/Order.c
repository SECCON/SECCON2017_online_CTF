//
//  Order.c
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 30..
//

#include "Global.h"
#include "Candy.h"
#include "UserInput.h"
#include "Order.h"

void orderList(){
    int cnt;
    
    if(gOrderCnt){
        printf("\n=*= Order list =*=\n");
        for(cnt = 0; cnt < gOrderCnt;cnt++){
            printf("\nOrder code  : %s\n",gOrderList[cnt]->orderCode);
            printf("Order count : %d\n",gOrderList[cnt]->orderNumber);
            printf("Order candy : %s\n",gOrderList[cnt]->orderCandyName);
            printf("Candy code  : %d\n",gOrderList[cnt]->candyCode);
        }
    }else{
        printf("You have never ordered a product.\n");
    }
}

int getOrderNum(){
    int seed;
    seed = time(NULL);
    srand(seed);
    return rand()% 10;
}        

unsigned int choiceCandy(){
    getCandy();
    printf("\nPlease pick up the candies to order.\n>");
    return retNumber(2);
}

void addToOrderList(){
    unsigned int candyNum;
    char strOrdNum[8];
    
    if(gOrderCnt < ORDERCNT){
        candyNum = choiceCandy();
        if(candyNum < CANDISECNT){
            struct ORDER *newOrder = malloc(sizeof(struct ORDER));

            sprintf(strOrdNum,"%d", getOrderNum());
            strncpy(newOrder->orderCode, strOrdNum, 1);
            newOrder->orderNumber = gCandies[candyNum]->orderNumber;
            strncpy(newOrder->orderCandyName,gCandies[candyNum]->candyName,sizeof(newOrder->orderCandyName));
            newOrder->candyCode = gCandies[candyNum]->candyCode;

            gOrderList[gOrderCnt] = newOrder;
            gOrderCnt++;

            orderList();
        }else{
            printf("Please enter a number between 0 and 9\n");
        }
    }else{
        printf("You can not order candy anymore.\n");
    }
}

int checkCancel(unsigned int num){
    int cnt;

    for(cnt = 0; cnt < CANDISECNT; cnt++){
        if(gOrderList[num]->candyCode == gCandies[cnt]->candyCode){
            if(gOrderList[num]->orderNumber == gCandies[cnt]->orderNumber){
                return 1;
            }else{
		return 0;
            }
        } 
    }
    return 0;
}

void reSort(int cnt){
    int add;

    free(gOrderList[cnt]);
    

    if(cnt < gOrderCnt){
        for(add = 0; cnt + add < gOrderCnt; add++){
            gOrderList[cnt + add] = gOrderList[cnt + add + 1];
        }
    }
    gOrderList[gOrderCnt] = 0;
    gOrderCnt--;
}

void orderCancel(){
    int cnt; 
    int state = 0;
    int orderCnt = 0;
    int cancelableCnt = 0;
    int cancelableList[10];

    if(gOrderCnt){
        for(cnt = 0; cnt < gOrderCnt;cnt++){
            if(checkCancel(cnt)){
                cancelableList[orderCnt] = cnt;
                orderCnt++;

		printf("\n=*= Cancelable order (Order number : %d) =*=\n", cnt);               
                printf("Order code: %s\n",gOrderList[cnt]->orderCode);
                printf("Order count : %d\n",gOrderList[cnt]->orderNumber);
                printf("Order candy : %s\n",gOrderList[cnt]->orderCandyName);
                printf("Candy code: %d\n",gOrderList[cnt]->candyCode);
            }
        }

        if(orderCnt){
            unsigned int canNum = retNumber(2);

            for(cnt = 0;cnt < orderCnt;cnt++){
                if(canNum == cancelableList[cnt]){
                    reSort(canNum);
                }
            }
        }
    }else{
        printf("You have never ordered a product.\n");
    }
}

int getStockNum(unsigned int num){
    int i;

    if(gStockCnt){
        for(i = 0; i < gStockCnt;i++){
            if(strncmp(gStock[i]->candyName,gOrderList[num]->orderCandyName,8) == 0){
                return (i + 1);
            }
        }
    }
    return 0;
}

void orderCandy(){
    int num, cnt, input;

    if(gOrderCnt){
        orderList();
        printf("\nWould you like to order these candies?\n");
        printf("0) Yes, 1) No\n");

        input = retNumber(2);
	if(input == 0){
            for(cnt = 0; cnt < gOrderCnt; cnt++){

                num = getStockNum(cnt);
                if(num){
                    gStock[num-1]->candyNumber += gOrderList[cnt]->orderNumber;
                }else{
                    if(gStockCnt < STOCKMAX){
                        printf("\nEnter information about newly added candy.\n");
			struct STOCK *newStock = malloc(sizeof(struct STOCK));

                        strncpy(newStock->candyName,gOrderList[cnt]->orderCandyName,sizeof(newStock->candyName));
                        newStock->candyNumber = gOrderList[cnt]->orderNumber;

		        printf("Enter the price of %s candy.\n",newStock->candyName);
                        newStock->candyPrice = retNumber(5);

                        printf("Enter a description of the %s candy.\n",newStock->candyName);
                        newStock->candyDescription = malloc(124);
                        UserInput(newStock->candyDescription,124);

                        gStock[gStockCnt] = newStock;
                        gStockCnt++;
                    }else{
                        printf("The warehouse is full. Your new order can not be completed.\n");
                    }
                }
            }

            for(;gOrderCnt > 0;gOrderCnt--){
	        free(gOrderList[gOrderCnt - 1]);
                gOrderList[gOrderCnt - 1] = 0;
            }

        }
    }else{
        printf("You have never ordered a product.\n");
    }
}

void orderMenu(){
    int cnt;
    unsigned int control = 1;
    char funcList[5][22] = {"Order List", "Add to Order List","Cancel one’s order.","Order candy","Exit"};

    while(control){
        printf("\nOrder candy.\n");
        
        for(cnt =0;cnt < 5;cnt++){
            printf("%d) %s\n",cnt+1,funcList[cnt]);
        }
        
        printf("Command : ");
        switch (retNumber(2)) {
            case 1:
                orderList();
                break;
            case 2:
		addToOrderList();
                break;
            case 3:
		orderCancel();
                break;
            case 4:
		orderCandy();
		break;
	    case 5:
		control = 0;
                break;
            default:
                break;
        }
    }
}

