// gcc secure_keymanager.c -o secure_keymanager
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#define BUF_SIZE 64
#define KEYS 8

int menu(void);
void change_master(void);
int check_account(void);
void add_key(void);
void show_key(void);
void edit_key(void);
void remove_key(void);
int getnline(char *buf, int size);
int getint(void);

struct Entry {
	char title[0x20];
	char key[];
};

struct Entry *key_list[KEYS];
char key_map[KEYS];
char account[0x10];
char master[0x10];

__attribute__((constructor))
void init(void){
	setbuf(stdout, NULL);
}

int main(void){
	int n;

	puts(
" _____                            _   __          ___  ___                                  \n"
"/  ___|                          | | / /          |  \\/  |                                  \n"
"\\ `--.  ___  ___ _   _ _ __ ___  | |/ /  ___ _   _| .  . | __ _ _ __   __ _  __ _  ___ _ __ \n"
" `--. \\/ _ \\/ __| | | | '__/ _ \\ |    \\ / _ \\ | | | |\\/| |/ _` | '_ \\ / _` |/ _` |/ _ \\ '__|\n"
"/\\__/ /  __/ (__| |_| | | |  __/ | |\\  \\  __/ |_| | |  | | (_| | | | | (_| | (_| |  __/ |   \n"
"\\____/ \\___|\\___|\\__,_|_|  \\___| \\_| \\_/\\___|\\__, \\_|  |_/\\__,_|_| |_|\\__,_|\\__, |\\___|_|   \n"
"                                              __/ |                          __/ |          \n"
"                                             |___/                          |___/           \n"
);

	printf("Set Your Account Name >> ");
	read(STDIN_FILENO, account, sizeof(account));
	printf("Set Your Master Pass >> ");
	read(STDIN_FILENO, master, sizeof(master));

	while(n = menu()){
		switch(n){
			case 1:
				add_key();
				break;
			case 2:
				show_key();
				break;
			case 3:
				edit_key();
				break;
			case 4:
				remove_key();
				break;
			case 9:
				change_master();
				break;
			default:
				puts("Invalid input...");
		}
		puts("done.");
	}

	return 0;
}

int menu(void){
	int n;
	printf( "\n"
		"1. add\n"
		"2. show\n"
		"3. edit\n"
		"4. remove\n"
		"9. change master pass\n"
		"0. exit\n"
		">> ");

	n = getint();

	puts("");
	return n;
}

void change_master(void){
	if(!check_account())
		return;

	printf("Set New Master Pass >> ");
	read(STDIN_FILENO, master, sizeof(master));
}

int check_account(void){
	char buf[BUF_SIZE];

	printf("Input Account Name >> ");
	read(STDIN_FILENO, buf, sizeof(buf));
	if(strcmp(account, buf)){
		printf("Account '%s' does not exist...\n", buf);
		return 0;
	}

	if(!strlen(master))
		return 1;

	printf("Input Master Pass >> ");
	read(STDIN_FILENO, buf, sizeof(buf));

	if(!strcmp(master, buf))
		return 1;

	printf("Wrong Pass...\n");
	return 0;
}

void add_key(void){
	int i, size;
	struct Entry *e;

	for(i=0; i<KEYS && key_map[i]; i++);
	if(i>=KEYS){
		puts("can't add key any more...");
		return;
	}

	puts("ADD KEY");
	printf("Input key length...");
	size = getint();
	if(!(e = malloc(offsetof(struct Entry, key) + size))){
		puts("can not allocate...");
		return;
	}
	
	printf("Input title...");
	getnline(e->title, sizeof(e->title));
	printf("Input key...");
	getnline(e->key, size);
	key_list[i] = e;
	key_map[i] = 1;
}

void show_key(void){
	int i;
	
	printf("id : Title / Key\n");
	for(i=0; i<KEYS; i++)
		if(key_map[i])
			printf("%02d : ***SECRET*** / ***SECRET***\n", i);
}

void edit_key(void){
	int id;

	puts("EDIT KEY");
	if(!check_account())
		return;

	printf("Input id to edit...");
	id = getint();

	if(id < 0 || id > KEYS-1)
		puts("out of length");
	else if(!key_map[id])
		puts("not exits...");
	else{
		printf("Input new key...");
		getnline(key_list[id]->key, malloc_usable_size(key_list[id]) - offsetof(struct Entry, key));
	}
}

void remove_key(void){
	int id;

	puts("REMOVE KEY");
	if(!check_account())
		return;

	printf("Input id to remove...");
	id = getint();

	if(id < 0 || id > KEYS-1)
		puts("out of length");
	else if(!key_list[id])
		puts("not exits...");
	else{
		free(key_list[id]);
		key_map[id] = 0;
	}
}

int getnline(char *buf, int size){
	char *lf;
	int n;

	if(size < 1 || !(n = read(STDIN_FILENO, buf, size-1)))
		return 0;

	buf[n] = '\0';
	if((lf=strchr(buf,'\n')))
		*lf='\0';

	return n;
}

int getint(void){
	char buf[BUF_SIZE]={0};

	getnline(buf, sizeof(buf));
	return atoi(buf);
}

