//
//  Global.h
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 30..
//

#ifndef Global_h
#define Global_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <signal.h>

#define CANDISECNT 10
#define SIGALARMTIME 30
#define ORDERCNT 10
#define STOCKMAX 5
#define ACCOUNTMAX 3
#define IDPWMAX 8
#define BOARDSIZE 1200

#define EMPTY 0
#define ADMIN 1
#define USER 3

typedef struct ORDER{
    char orderCode[8];
    unsigned int  orderNumber;
    char orderCandyName[8];
    int  candyCode;
};

typedef struct CANDIES {
    char candyName[8];
    unsigned int  orderNumber;
    int candyCode;
};

typedef struct STOCK{
    char candyName[8];
    unsigned int  candyNumber;
    int  candyPrice;
    char *candyDescription;
};

typedef struct IDPW{
    long empty[2];
    char id[IDPWMAX];
    char pw[IDPWMAX];
    long state;
    char description[88];
};

typedef struct ACCOUNT{
    long state;
    long number;
    struct IDPW *fd;
    long bk;
};

char *board;

unsigned int gOrderCnt;
unsigned int gStockCnt;
unsigned int gLoginFailCnt;

void AlarmHandle(int signo);

struct ORDER *gOrderList[ORDERCNT];
struct CANDIES *gCandies[CANDISECNT];
struct STOCK *gStock[STOCKMAX];
struct ACCOUNT *gLoginAccount;
struct ACCOUNT gAccount[ACCOUNTMAX];

#endif /* Global_h */
