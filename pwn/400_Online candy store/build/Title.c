//
//  Title.c
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 30..
//

#include "Title.h"
#include "Global.h"

void title(){
    printf("                                    _________ _________   _________                                         \n");
    printf("           /\\|\\/\\                   \\_   ___ \\\\_   ___ \\ /   _____/                   /\\|\\/\\                \n");
    printf("  ______  _)    (__   ______        /    \\  \\//    \\  \\/ \\_____  \\          ______   _)    (__   ______     \n");
    printf(" /_____/  \\_     _/  /_____/        \\     \\___\\     \\____/        \\        /_____/   \\_     _/  /_____/     \n");
    printf("            )    \\                   \\______  /\\______  /_______  /                    )    \\               \n");
    printf("            \\/\\|\\/                          \\/        \\/        \\/                     \\/\\|\\/               \n");
    printf("_________                              _________                    .___              .__                   \n");
    printf("\\_   ___ \\  ____   ____   ___________  \\_   ___ \\_____    ____    __| _/__.__.   _____|  |__   ____ ______  \n");
    printf("/    \\  \\/ /  _ \\ /    \\_/ __ \\_  __ \\ /    \\  \\/\\__  \\  /    \\  / __ <   |  |  /  ___/  |  \\ /  _ \\\\____ \\ \n");
    printf("\\     \\___(  <_> )   |  \\  ___/|  | \\/ \\     \\____/ __ \\|   |  \\/ /_/ |\\___  |  \\___ \\|   Y  (  <_> )  |_> >\n");
    printf(" \\______  /\\____/|___|  /\\___  >__|     \\______  (____  /___|  /\\____ |/ ____| /____  >___|  /\\____/|   __/ \n");
    printf("        \\/            \\/     \\/                \\/     \\/     \\/      \\/\\/           \\/     \\/       |__|    \n");
}

void Menu(){
    printf("\nMenu\n");
    printf("1.Stock\n");
    printf("2.Purchase\n");
    printf("3.Charge\n");
    if(gLoginAccount->state == ADMIN){
        printf("4.Order\n");
        printf("5.Account\n");
    }
    printf("9.Logout\n");
    printf("0.Exit\n");
    printf("\nHi,%s. You have %ld yen.\n",gLoginAccount->fd->id,gLoginAccount->bk);
}
