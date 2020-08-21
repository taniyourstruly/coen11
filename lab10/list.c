
#include "list.h"

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
