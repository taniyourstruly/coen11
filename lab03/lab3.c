
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int counter = 0;
struct waitingList
{
	char names[20];
	int deptList;
};
struct waitingList urgentCare[10];
struct waitingList* waitingInfo;

void insert();
void list();
void move();
void show_dept();
int check_duplicate(char nameEntered[]);

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
		printf("Press 0 to quit.\n");
		printf("------------------------------------------------------\n");
		scanf("%d",&input);
		{
			if (input == 1)
			{
				if (counter>=10)
				{
					printf("You cannot add to the list.\n ");
				}
				else
				{
					insert();
				}
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
			else if (input == 0)
			{
				printf("You are quitting the list.\n ");
				return 0;
			}
			else
			{
				printf("Error. Please enter 1, 2, 3, 4, or 0.\n ");
	
			}

		}
	}while(input !=0);

	return 0;
}

void insert()
{
	printf("Type in name.\n ");
	char nameEntered[20];
	scanf("%s",nameEntered);
	//printf("Type in dept.\n");
	int dept;
	if (check_duplicate(nameEntered)==0) 	//check if the name entered is not already in the list
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
			urgentCare[counter].deptList=dept;
			strcpy(urgentCare[counter].names,nameEntered);
		}
	//urgentCare[counter].deptList=dept;	//add dept number
	//strcpy(urgentCare[counter].names,nameEntered);	//add name
	counter++;
	}
}

void list()
{
	waitingInfo = urgentCare;
	for (int i = 0; i < counter; i++, waitingInfo++)
	{
		printf("%s\t\t %d \n ",waitingInfo->names,waitingInfo->deptList);
	}
}

void move()
{
	int deptNum;
	scanf("%d",&deptNum);
	waitingInfo = urgentCare;
	struct waitingList* temp;
	for (int i = 0; i < counter; i++, waitingInfo++)
	{
		if(waitingInfo->deptList==deptNum)
		{
			temp = waitingInfo+1;	//temporary struct, one in front of waitingInfo
			int c;
			for (c = i-1; c < counter-1; c++, waitingInfo++, temp++)	//start from index before the position and replace
			{
				*waitingInfo=*temp;	//replace the info in the array with the next one
			}
			counter--;
			return;  
		}
	}
}

void show_dept()
{
	int deptNum;
	scanf("%d",&deptNum);
	int i;
	waitingInfo = urgentCare;
	for (i = 0; i< counter; i++,waitingInfo++)
	{
		if (waitingInfo->deptList==deptNum)
		{
			printf("%s\t %d \n", waitingInfo->names, waitingInfo->deptList);
		}
	}
	return;
}

int check_duplicate(char nameEntered[])
{
	waitingInfo = urgentCare;
	for (int i = 0; i < counter; i++, waitingInfo++)
	{
		if (strcmp(nameEntered,waitingInfo->names)==0)
		{
			printf("This name has already been added.\n");
			return 1;
		}
	}
	return 0;
}

