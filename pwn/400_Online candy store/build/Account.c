//
//  Account.c
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 30..
//

#include "Global.h"
#include "UserInput.h"

void addAccount(unsigned int state){
    unsigned int i, empty = 1;

    for(i=0;(i < ACCOUNTMAX) && empty;i++){
        if(gAccount[i].state == EMPTY){
            empty = 0;
            gAccount[i].state = state;
            gAccount[i].number = i + 1;
            gAccount[i].fd = malloc(sizeof(struct IDPW));

            gAccount[i].fd->state = 1;
            printf("\nEnter your New ID.\n");
            UserInput(gAccount[i].fd->id, IDPWMAX);
            printf("Enter your New Password.\n");
            UserInput(gAccount[i].fd->pw, IDPWMAX);
            printf("Enter your profile.\n");
            UserInput(gAccount[i].fd->description, 88);

            gAccount[i].bk = 10000;
        }
    }

    if(empty){
        printf("Could not add user.\n");
    }
}

void delAccount(){
    unsigned int num,i;

    printf("\nAccount list\n");
    for(i=0;i < ACCOUNTMAX;i++){
        if(gAccount[i].state != EMPTY){
            printf("%d) %s\n",gAccount[i].number, gAccount[i].fd->id);
        }        
    }

    printf("\nPlease enter the number of the account you want to delete\n");
    num = retNumber(2);

    if(0 < num && num <= ACCOUNTMAX){

        num -=1;

        if(gAccount[num].state == USER){
            gAccount[num].state = 0;
            gAccount[num].fd->state = 0;
            printf("The account(%s) has been deleted.\n",gAccount[num].fd->id);
            memset(gAccount[num].fd,0,sizeof(struct IDPW));
            free(gAccount[num].fd);

            long tmp = gAccount[num].fd;
            gAccount[num].fd = tmp - 16;
        }else{
            printf("You can not delete the account.\n");
        }
    }
}

unsigned int login(){
    int i;
    char id[8],pw[8];

    memset(id,0,8);
    memset(pw,0,8);

    printf("\nEnter your ID.\n> ");
    UserInput(id,8);
 
    printf("Enter your Password.\n> ");
    UserInput(pw,8);

    for(i=0;i<ACCOUNTMAX;i++){
        if(gAccount[i].state != EMPTY){
            if(strlen(id) == strlen(gAccount[i].fd->id)){
                if(!strncmp(gAccount[i].fd->id,id,strlen(gAccount[i].fd->id))){
                    if(strlen(pw) == strlen(gAccount[i].fd->pw)){
                        if(!strncmp(gAccount[i].fd->pw,pw,strlen(gAccount[i].fd->pw))){
                            gLoginAccount = &gAccount[i];
                            printf("\nHi, %s",gLoginAccount->fd->id);
                            return 1;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

void logout(){
    printf("\nAre you sure you want to sign out?\n");
    printf("0) Yes\n1) No\n");
            
    if(retNumber(2) == 0){
        gLoginAccount = 0;
        printf("logout\n");
    }
}

void changePW(){
    unsigned int num,i;
    char tmp[IDPWMAX];

    printf("\nAccount list\n");
    for(i=0;i < ACCOUNTMAX;i++){
        if(gAccount[i].state != EMPTY){
            printf("%ld) %s\n",gAccount[i].number, gAccount[i].fd->id);
        }        
    }

    printf("\nPlease enter the number of the account you want to change PW\n");
    num = retNumber(2);

    if(0 < num && num <= ACCOUNTMAX){
        num -=1;
        if(gAccount[num].fd != EMPTY){
            if(gAccount[num].fd->state != EMPTY){
                printf("Enter your New Password.\n");
                UserInput(gAccount[num].fd->pw, IDPWMAX);
            }
        }
    }   
}

void Account(){
    int cnt;
    unsigned int control = 1;
    char funcList[3][22] = {"Delete account", "Change password","Exit"};

    while(control){
        printf("\nAccount.\n");
        
        for(cnt =0;cnt < 3;cnt++){
            printf("%d) %s\n",cnt+1,funcList[cnt]);
        }
        
        printf("Command : ");
        switch (retNumber(2)) {
            case 1:
                delAccount();
                break;
            case 2:
		changePW();
                break;
            case 3:
		control = 0;
                break;
            default:
                break;
        }
    }
}
