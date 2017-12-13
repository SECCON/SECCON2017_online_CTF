// gcc election.c -Wl,-z,relro,-z,now -o election
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 32

enum LEVEL {LV_STND=1, LV_VOTE, LV_RESLT};

struct candidate {
	char *name;
	struct candidate *next;
	unsigned votes;
};

struct candidate *list = NULL;
enum LEVEL lv = LV_STND;
int inv_votes = 0;

int menu(void);
void stand(void);
void vote(void);
void result(void);
struct candidate *add_list(char *name);
struct candidate *find_list(char *name);
int getnline(char *buf, int size);
int getint(void);

__attribute__((constructor))
void init(void){
	setbuf(stdout, NULL);
	srand(time(NULL));
}

int main(void){
	int n;

	printf("*** Election ***\n");

	add_list("Tatsumi");
	add_list("Shinonome");
	add_list("Ojima");

	while(n = menu()){
		switch(n){
			case 1:
				stand();
				break;
			case 2:
				vote();
				break;
			case 3:
				result();
				break;
			default:
				puts("Invalid input...");
		}
		puts("done.");
	}

	printf("Thank you!!");
}

int menu(void){
	int n;

	printf( "\n"
		"1. stand\n"
		"2. vote\n"
		"3. result\n"
		"0. eat chocolate\n"
		">> ");

	n = getint();

	putchar('\n');
	return n;
}

void stand(void){
	char buf[BUF_SIZE];

	if(lv > LV_STND){
		printf("The candidacy has already closed.\n");
		return;
	}

	printf("Enter the name.\n>> ");
	getnline(buf, sizeof(buf));
	add_list(buf);
}

void vote(void){
	struct {
		char buf[BUF_SIZE];
		struct candidate *p;
		char v;
	} _;
	_.v = 1;

	if(lv > LV_VOTE){
		printf("The voting has already closed.\n");
		return;
	}

	lv = LV_VOTE;

	printf("Show candidates? (Y/n) ");
	getnline(_.buf, sizeof(_.buf));
	if(strcasecmp(_.buf, "n")){
		struct candidate *p;
		printf("Candidates:\n");
		for(p=list; p; p=p->next)
			printf("* %s\n", p->name);
	}

	printf("Enter the name of the candidate.\n>> ");
	getnline(_.buf, sizeof(_.buf));

	if(!strcasecmp(_.buf, "oshima")){
		if(!(_.p = find_list("ojima")))
			return;

		printf(	"I'm not 'Oshima', but 'Ojima'!\n"
			"Would you modify the name and re-vote?\n>> ");
		getnline(_.buf, sizeof(_));
		if(!strcasecmp(_.buf, "yes"))
			_.p->votes += _.v;
	}
	else if(_.p = find_list(_.buf))
		_.p->votes += _.v;
	else{
		printf("'%s' is invalid vote :(\n", _.buf);
		inv_votes++;
	}
}

void result(void){
	struct candidate *p;

	lv = LV_RESLT;

	for(p=list; p; p=p->next)
		printf("%11s : %d\n", p->name, p->votes);
	printf("%11s : %d\n", "INVALID", inv_votes);
}

struct candidate *add_list(char *name){
	struct candidate *p;

	p = malloc(sizeof(struct candidate));
	p->name = strdup(name);
	p->votes = rand()%100;
	p->next = list;
	list = p;
}

struct candidate *find_list(char *name){
	struct candidate *p;

	for(p=list; p; p=p->next)
		if(!strcasecmp(name, p->name))
			return p;
	return NULL;
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
