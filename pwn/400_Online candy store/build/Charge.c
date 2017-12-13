#include "Global.h"
#include "UserInput.h"

void charge(){
    unsigned int num,charge = 0;

    printf("Please select the amount to charge.\n");
    printf("0) 1\n");
    printf("1) 10\n");
    printf("2) 100\n");
    printf("3) 1000\n");
    printf("4) 10000\n");
    printf("5) 100000\n");

    num = retNumber(2);
    switch(num){
        case 0:
            charge += 1;
            break;
        case 1:
            charge += 10;
            break;
        case 2:
            charge += 100;
            break;
        case 3:
            charge += 1000;
            break;
        case 4:
            charge += 10000;
            break;
        case 5:
            charge += 100000;
            break;
    }

    gLoginAccount->bk = gLoginAccount->bk + charge;
    printf("%ld yen charged.\n",charge);
}
