

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

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
struct array waitingInfo[4];

int x;
#define CHECK_DEPT(x) (((x)>0)&&((x)<5))

pthread_mutex_t mutex;

void sleep();

//checks if name entered is a duplicate name
int check_duplicate(char nameEntered[])
{
	struct array *p = waitingInfo;
	NODE *q;
	int i;
	for (i = 0; i < 4; i++, p++)
	{
		q=waitingInfo[i].head;
		while(q!=NULL)
		{
			if (strcmp(nameEntered,q->names)==0)
			{
				printf("This name has already been added.\n");
				return 1;
			}
			q=q->next;
		}
	}
	return 0;
}

//lists out all of the names in the list
void list()
{
	int i;
	NODE *q;
	for ( i = 0 ; i < 4; i++)
	{
		q=waitingInfo[i].head;
		while (q!=NULL)
		{
			printf("%s\t\t %d  ",q->names, q->deptList);
			if (q->deptList==1)
			{
				printf("Fever %f\n", q->patientInfo.fever);

			}
			else if (q->deptList==2)
			{	
				printf("Type of pain %s\n",q->patientInfo.pain);
			}		
			else
			{
				printf("Days sick %d\n",q->patientInfo.daySick);
			}
			q=q->next;
		}	
	}
}

//shows the names and information of the department entered
void show_dept()
{
	int deptNum;
	scanf("%d",&deptNum);
	struct array *p;
	NODE *q;	
	if (!(CHECK_DEPT(deptNum)))
	{
		printf("Wrong department.\n");
		return;
	}
	//printf("test3\n");
	deptNum--;
	p=&waitingInfo[deptNum];
	q=p->head;
	while (q!=NULL)
	{	
	printf("%s\t %d \n", q->names, q->deptList);
			if (q->deptList==1)
			{
				printf("Fever %f\n", q->patientInfo.fever);
			}
			else if (q->deptList==2)
			{
				printf("Type of pain %s\n",q->patientInfo.pain);
			}		
			else
			{
				printf("Days sick %d\n",q->patientInfo.daySick);
			}
			q=q->next;
	//}
	//q=q->next;
	}
}


//shows the information of the name entered
void show_info()
{
	NODE *q;
	int i;
	struct array *p = waitingInfo;
	char nameEntered[20];
	scanf("%s",nameEntered);
	//q=p->head;
	for (i = 0; i < 4; i++, p++)
	{	
		//p=&waitingInfo[i];
		//printf("test 1\n");
		q=p->head;
		while (q!=NULL)
		{
			if ((strcmp(q->names,nameEntered))==0)		//if nameEntered equals name in the list
			{
				printf("What is the department?\t");
				printf("%d\n",q->deptList);
				if (q->deptList==1)
				{
					printf("Fever: \t");
					printf("%f\n",q->patientInfo.fever);
				}
				else if (q->deptList==2)
				{
					printf("Pain: \t");
					printf("%s\n",q->patientInfo.pain);
				}
			
				else
				{
					printf("Sick days: \t");
					printf("%d\n", q->patientInfo.daySick);
				}
			}
			q=q->next;
			//printf("test1\n");
		}
	}
}

//inserts a new name with department and info in the list
void insert(char* nameEntered, int deptList, union u patientInfo)
{
	NODE *p=(NODE *)malloc(sizeof(NODE));

	if (p == NULL)
	{
		return;
	}
	strcpy(p->names, nameEntered);
	p->deptList=deptList;
	p->patientInfo=patientInfo;
	p->next=NULL;
	pthread_mutex_lock(&mutex);
	if (waitingInfo[deptList-1].tail==NULL && waitingInfo[deptList-1].head==NULL)
	{
		waitingInfo[deptList-1].head=waitingInfo[deptList-1].tail=p;
	}
	else
	{
		
		waitingInfo[deptList-1].tail->next=p;
		waitingInfo[deptList-1].tail=p;
		pthread_mutex_unlock(&mutex);
		return;
	}
	pthread_mutex_unlock(&mutex);
}
//remove a patient from the list	
void move()
{
	int deptNum;
	scanf("%d",&deptNum);
	NODE *q;
	q=waitingInfo[deptNum-1].head;
	if (!(CHECK_DEPT(deptNum)))
	{
		printf("Wrong department.\n");
	}
	pthread_mutex_lock(&mutex);
	if (q!=NULL)
	{
		waitingInfo[deptNum-1].head=q->next;
		free(q);	
	}
	pthread_mutex_unlock(&mutex);
}


//reads from a text file
void read_tfile(char *fname, int key)
{ 
	FILE *fp = fopen(fname,"r");
	if (fp==NULL)
	{
		return;
	}
	char temp[10];
	int depts;
	union u myUnion;
	char string[100];
	char *p;
	//fseek(fp, 15, SEEK_SET); //skips over temp, depts, myUnion
	while (fgets(string, 100, fp)!=NULL)
	{
		p=string;
		while (*p!='\n')
		{
			*p = *p ^ key;
			p++;
		}
	//while (fscanf(fp, "%s %d", temp, &depts)==2)
		sscanf(string, "%s\t%d", temp, &depts);
		if (depts==1)
		{ 
			//printf("test 2\n");
 			sscanf(string, "%s\t%d\t%f\n", temp, &depts, &myUnion.fever);
		}
		else if (depts==2)
		{
			sscanf(string, "%s\t%d\t%s\n", temp, &depts, myUnion.pain);
		}
		else
		{
			sscanf(string, "%s\t%d\t%d\n", temp, &depts, &myUnion.daySick);
		}
		insert(temp, depts, myUnion);
	}
	printf("successful read\n");
	fclose(fp);
	return;
}

//writes from a text file
void save_tfile(char *fname, int key)
{ 
	int i;
	NODE *q;
	char string[100];
	char *p;
	FILE *fp = fopen(fname,"w");
	if (fp==NULL)
	{
		return;
	}
	//fprintf(fp, "Name\tDept\tInfo\n");
	for (i = 0; i<4; i++)
	{
		q=waitingInfo[i].head;
		while (q!=NULL)
		{
			if (q->deptList==1)	
			{
				//printf("test 1\n"); 
				sprintf(string, "%s\t%d\t%f\n", q->names,q->deptList,q->patientInfo.fever);
			}
			else if (q->deptList==2)
			{	
				sprintf(string, "%s\t%d\t%s\n",  q->names,q->deptList,q->patientInfo.pain);
			}
			else
			{	
				sprintf(string, "%s\t%d\t%d\n",  q->names,q->deptList,q->patientInfo.daySick);
			}
			p=string;
			while (*p!='\n')
			{
				*p = *p ^ key;
				p++;
			}
			fprintf(fp, "%s", string);
			q=q->next;
		}
	}
	printf("successful save\n");
	fclose(fp);
	return;
}

//reads from the binary file
void read_auto_save(char *fname) //change to binary file (fread)
{
	NODE temp;
	int r;
	pthread_mutex_lock(&mutex);
	FILE *fp = fopen(fname,"rb");
	if (fp==NULL)
	{
		pthread_mutex_unlock(&mutex);
		return;
	}
	while ((r=fread(&temp,sizeof(NODE),1,fp))>0)
	{
		
		printf ("%s\t%d\t", temp.names, temp.deptList);
			
			if (temp.deptList==1)	
			{ 
 				printf("%f\n", temp.patientInfo.fever);
			}
			else if (temp.deptList==2)
			{
				printf("%s\n", temp.patientInfo.pain);
			}
			else
			{
				printf("%d\n", temp.patientInfo.daySick);
			}
	}				
	fclose(fp);
	pthread_mutex_unlock(&mutex);
	return;
}


//deletes everything
void delete_All()
{
	NODE *q;
	int i;
	for (i = 0; i<4; i++)
	{
		q=waitingInfo[i].head;
		while(q!=NULL)
		{
			waitingInfo[i].head=q->next;
			free(q);
			q=waitingInfo[i].head;
		}
	}
}


//extra credit - change the department of one name to another
void change_dept()
{
	NODE *p, *q;
	char nameEntered[20];
	scanf("%s",nameEntered);
	printf("Type in current department.\n");
	int old_dept;
	scanf("%d",&old_dept);
	p=q=waitingInfo[old_dept-1].head;
	int flag;
	pthread_mutex_lock(&mutex);
	while (p!=NULL)
	{
		if ((strcmp(p->names,nameEntered))==0)		//if nameEntered equals name in the list
		{
			flag=1;
			break;
		}
		q=p;
		p=p->next;
	}
	if (flag == 0)
	{
		printf("Person not found.\n");
		pthread_mutex_unlock(&mutex);
		return;
	}
	else
	{
		if (p==waitingInfo[old_dept-1].head&&p==waitingInfo[old_dept-1].tail) //only one
		{
			waitingInfo[old_dept-1].head=waitingInfo[old_dept-1].tail=NULL;
		}
		if (p==waitingInfo[old_dept-1].head)
		{
			waitingInfo[old_dept-1].head=p->next;
		}
		else if (p==waitingInfo[old_dept-1].tail)
		{
			waitingInfo[old_dept-1].tail=q;
			q->next=NULL;
		}
		else
		{
			q->next=p->next;	
		}

		printf("Type in new department.\n");
		int new_dept;
		scanf("%d",&new_dept);

		if (!(CHECK_DEPT(new_dept)))
		{
			printf("Wrong department.\n");
		}
		if (new_dept == 1)	//if dept is 1, type in fever
		{
			printf("Type in fever temperature.\n");
			scanf("%f",&p->patientInfo.fever);
		}
		if (new_dept == 2)	//if dept is 2, type in pain
		{
			printf("Type in type of pain.\n");
			scanf("%s",p->patientInfo.pain);
		}
		if (new_dept == 3||new_dept == 4)	//if dept is 3 or 4, type in days sick
		{
			printf("Type in numbers of days sick.\n");
			scanf("%d",&p->patientInfo.daySick);
		}
		pthread_mutex_unlock(&mutex);
		insert(nameEntered, new_dept, p->patientInfo);
		free(p);
	}
}

void *auto_save(void *arg)
{
	int i;
	NODE *q;
	char *fileName = (char *)arg;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("Autosave\n");
		FILE*fp=fopen(fileName,"wb");
		if (fp==NULL)
		{
			pthread_mutex_unlock(&mutex);
			return NULL;
		}
		for (i = 0; i<4; i++)
		{
			q=waitingInfo[i].head;
			while (q!=NULL)
			{
 				fwrite(q, sizeof(NODE), 1, fp);
				q=q->next;
			}
		}
		fclose(fp);
		pthread_mutex_unlock(&mutex);
		sleep(15);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thread;

	pthread_mutex_init (&mutex, NULL);
	
	pthread_create(&thread,NULL,auto_save,(void *)argv[2]);
	
	int key = atoi(argv[3]);

	if (argc<3)
	{
		return 0;
	}

	read_tfile(argv[1], key);

	int input;
	do
	{
		printf("------------------------------------------------------\n");
		printf("Welcome to Urgent Care. Press 1 to add your name to the waiting list.\n");
		printf("Press 2 to show the list.\n");
		printf("Press 3 to remove the oldest item with a specific department.\n");
		printf("Press 4 to show the names in the list with a department.\n");
		printf("Press 5 to show info in a department.\n");
		printf("Press 6 to change department.\n");
		printf("Press 7 to see the auto-saved information.\n");
		printf("Press 0 to quit.\n");
		printf("------------------------------------------------------\n");
		scanf("%d",&input);
		{
			if (input == 1)
			{
				union u info;
				struct array *p = waitingInfo;
				
				printf("Type in name.\n");
				char nameEntered[20];
				scanf("%s",nameEntered);
				int dept;
				if (check_duplicate(nameEntered)==0)	//check if the name entered is not already in the list
				{
					printf("Type in dept.\n");
					scanf("%d",&dept);
					if (!(CHECK_DEPT(dept)))
					{
						printf("Wrong department.\n");
						continue;
					}
					else
					{
						if (dept == 1)	//if dept is 1, type in fever
						{
							printf("Type in fever temperature.\n");
							scanf("%f",&info.fever);
						}
						if (dept == 2)	//if dept is 2, type in pain
						{
							printf("Type in type of pain.\n");
							scanf("%s",info.pain);
						}
						if (dept == 3||dept == 4)	//if dept is 3 or 4, type in days sick
						{
							printf("Type in numbers of days sick.\n");
							scanf("%d",&info.daySick);
						}
					}
				}	
				//printf("Test 1");
				insert(nameEntered, dept, info);
			}	
			else if (input == 2)
			{
				list();
			}
			else if (input == 3)
			{
				printf("Enter a dept.\n ");
				move();
			}	
			else if (input == 4)
			{
				printf("Enter a dept.\n ");
				show_dept();
			}
			else if (input == 5)
			{
				printf("Enter a name.\n");
				show_info();
			}	
			else if (input == 6)
			{
				printf("Enter a name.\n");
				change_dept();
			}
			else if (input == 7)
			{
				read_auto_save(argv[2]);
			}
			else if (input == 0)
			{
				
				//printf("print\n");
				pthread_mutex_lock(&mutex);
				pthread_cancel(thread);
				pthread_mutex_unlock(&mutex);
				
				save_tfile(argv[1], key);
				delete_All();
			}
			else
			{
				printf("Error. Please enter 1, 2, 3, 4, 5, 6, 7 or 0. \n");
			}
		}
	}while(input !=0);
	return 0;
}

