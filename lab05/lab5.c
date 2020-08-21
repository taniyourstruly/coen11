#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
NODE *head = NULL;
NODE *tail = NULL;

int check_duplicate(char nameEntered[]);

void insert()
{	
	NODE *temp;
	temp = (NODE *)malloc(sizeof(NODE));

	printf("Type in name.\n");
	char nameEntered[20];
	scanf("%s",nameEntered);
	int dept;
	if (check_duplicate(nameEntered)==0)	//check if the name entered is not already in the list
	{
		printf("Type in dept.\n");
		scanf("%d",&dept);
		if (dept<1||dept>4)		//check if dept number entered is valid
		{
			printf("Sorry. Incorrect dept number.\n");
			return;
		}
		else
		{
			strcpy(temp->names,nameEntered);
			temp->deptList=dept;
			if (temp->deptList == 1)	//if dept is 1, type in fever
			{
				printf("Type in fever temperature.\n");
				scanf("%f",&temp->patientInfo.fever);
			}
			if (temp->deptList == 2)	//if dept is 2, type in pain
			{
				printf("Type in type of pain.\n");
				scanf("%s",temp->patientInfo.pain);
			}
			if (temp->deptList == 3||temp->deptList == 4)	//if dept is 3 or 4, type in days sick
			{
				printf("Type in numbers of days sick.\n");
				scanf("%d",&temp->patientInfo.daySick);
			}
		}	
		if (head == NULL)	//if there is nothing in head, make temp into head
		{
			head=tail=temp;
			temp->next=NULL;
		}
		if (tail!=NULL)		//add temp the linked list
		{
			tail->next=temp;
			tail=temp;
			tail->next=NULL;
		}
	}
}

int check_duplicate(char nameEntered[])
{
	NODE *p;
	p=head;
	while(p!=NULL)
	{
		//printf("Name entered: %s\n",nameEntered);
		//printf("Name in list: %s\n",p->names);
		if (strcmp(nameEntered,p->names)==0)
		{
			printf("This name has already been added.\n");
			return 1;
		}
	p=p->next;
	}
	return 0;
}

void list()
{
	NODE *p;
	p=head;
	while (p!=NULL)
	{
		printf("%s\t\t %d \n ",p->names, p->deptList);
		if (p->deptList==1)
		{
			printf("Fever %f\n", p->patientInfo.fever);
		}
		else if (p->deptList==2)
		{
			printf("Type of pain %s\n",p->patientInfo.pain);
		}		
		else
		{
			printf("Days sick %d\n",p->patientInfo.daySick);
		}
		p=p->next;
	}	
}

void show_dept()
{
	int deptNum;
	scanf("%d",&deptNum);
	NODE *p;
	p=head;
	while (p!=NULL)
	{
		if (p->deptList==deptNum)
		{
			printf("%s\t %d \n", p->names, p->deptList);
			if (p->deptList==1)
			{
				printf("Fever %f\n", p->patientInfo.fever);
			}
			else if (p->deptList==2)
			{
				printf("Type of pain %s\n",p->patientInfo.pain);
			}		
			else
			{
				printf("Days sick %d\n",p->patientInfo.daySick);
			}
		}
	p=p->next;
	}
}

void show_info()
{
	NODE *p;
	p=head;
	char nameEntered[20];
	scanf("%s",nameEntered);
	while (p!=NULL)
	{
		if ((strcmp(p->names,nameEntered))==0)		//if nameEntered equals name in the list
		{
			printf("What is the department?\t");
			printf("%d\n",p->deptList);
			if (p->deptList==1)
			{
				printf("Fever: \t");
				printf("%f\n",p->patientInfo.fever);
			}
			else if (p->deptList==2)
			{
				printf("Pain: \t");
				printf("%s\n",p->patientInfo.pain);
			}
			else
			{
				printf("Sick days: \t");
				printf("%d\n", p->patientInfo.daySick);
			}
		}
	p=p->next;
	}
}

void move()
{
	int deptNum;
	scanf("%d",&deptNum);
	NODE *p,*q;
	p=q=head;
	int flag = 0;
	while (p!=NULL)
	{
		if (p->deptList==deptNum)	//set a flag if int in deptList is deptNum entered
		{
			flag = 1;
			break;				
		}
		q=p;		//move p and q along the list
		p=p->next;
	}
	if (flag==1)
	{
		if (p==head&&p==tail)		//only one node in the list
		{
			head=NULL;
			tail=NULL;
		}	
		else if (p==head)	//delete first node
		{
			head=p->next;
		}
		else if (p==tail)	//delete last node
		{
			tail=q;
			q->next=NULL;
		}
		else			//delete in the middle
		{
			q->next=p->next;	
		}
		free(p);
	}	
	else
	{
		printf("Department not found.\n");
	}
}

int main()
{
	int input;
	do
	{
		printf("------------------------------------------------------\n");
		printf("Welcome to Urgent Care. Please add your name to the waiting list.\n");
		printf("Press 1 to add names and dept number.\n");
		printf("Press 2 to show the list.\n");
		printf("Press 3 to remove the oldest item with a specific department.\n");
		printf("Press 4 to show the names in the list with a department.\n");
		printf("Press 5 to show info in a department.\n");
		printf("Press 0 to quit.\n");
		printf("------------------------------------------------------\n");
		scanf("%d",&input);
		{
			if (input == 1)
			{
				insert();
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
			else if (input == 0)
			{
				printf("You are quitting the list.\n ");
				return 0;
			}
			else
			{
				printf("Error. Please enter 1, 2, 3, 4, 5 or 0. \n");
			}
		}
	}while(input !=0);
	return 0;
}
