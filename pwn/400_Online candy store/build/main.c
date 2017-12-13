//
//  main.c
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 26..
//

#include "Global.h"
#include "Candy.h"
#include "Stock.h"
#include "Account.h"
#include "Charge.h"

__attribute__((constructor)) void addAdmin(void)
{
    gAccount[0].state = ADMIN;
    gAccount[0].number = 1;
    gAccount[0].fd = malloc(sizeof(struct IDPW));
    gAccount[0].bk = 880;

    strncpy(gAccount[0].fd->id,"Admin",sizeof(gAccount[0].fd->id));
    strncpy(gAccount[0].fd->pw,"admin",sizeof(gAccount[0].fd->pw));
    strncpy(gAccount[0].fd->description,"I'm Lazenca.0x0\nWebsite is \"https://www.lazenca.net/\"",sizeof(gAccount[0].fd->description));
}

int main(void){
    int state = 1;

    signal(SIGALRM, AlarmHandle);
    alarm(SIGALARMTIME);

    title();
    setCandy();

    gOrderCnt = 0;
    gLoginFailCnt = 0;

    while(!gLoginAccount){
        if(login()){
            gLoginFailCnt = 0;
            while(state && gLoginAccount){
	        Menu();
	        printf("Command : ");
                switch (retNumber(2)) {
                    case 1:
		        printStock();
                        break;
                    case 2:
		        purchase();
		        break;
                    case 3:
                        charge();
                        break;
	            case 4:
                        if(gLoginAccount->state == ADMIN){
		            orderMenu();
                        }
		        break;
                    case 5:
                        if(gLoginAccount->state == ADMIN){
		            Account();
                        }
		        break;
                    case 9:
                        logout();
                        break;
                    case 0:
                        state = 0;
                        break;    
                    default:
                        break;
                }
            }
        }else if(gLoginFailCnt == 2){
            exit(1);
        }else{
            gLoginFailCnt++;
            printf("\nCreate an account?\n");
            printf("0) Yes\n1) No\n");
            
            if(retNumber(2) == 0){
                addAccount(3);
            }
        }
        
    } 
    return 0; 
}
