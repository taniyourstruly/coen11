

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

/*do not initialize global variables*/

union u
{
	float fever;
	char pain[20];
	int daySick;
};
#define NODE struct node
struct node
{
	char names[20];
	int deptList;
	union u patientInfo;
	NODE *next;
};
struct array
{
	NODE *head;
	NODE *tail;	
};
extern struct array waitingInfo[4];

int x;
#define CHECK_DEPT(x) (((x)>0)&&((x)<5))

extern pthread_mutex_t mutex;

extern void sleep();

extern int check_duplicate(char *);
extern void insert(char* nameEntered, int deptList, union u patientInfo);
extern void move();
extern void change_dept();
//extern void delete_All(NODE *p);
extern void list();
extern void show_dept();
extern void show_info();
extern void read_auto_save(char *fname);
extern void save_tfile(char *fname, int key);
extern void read_tfile(char *fname, int key); 
extern void delete_All(void);
extern void *auto_save(void *arg);
