#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char names[10][20];
int deptList[10];
int counter = 0;

void insert();
void list();

int main()
{
	printf("Press 1 to add name and dept number.\n ");
	printf("Press 2 to show the list.\n ");
	printf("Press 0 to quit.\n ");
	int input;
	while(1)	//infinite loop
	{	
		printf("Enter a number.\n ");
		scanf("%d",&input);
		if (input == 1)
		{
			//printf("about to insert.");
			if (counter>10)		//check if the list is already full
			{
				printf("You cannot add to the list.\n ");
				return 0;
			}
			insert();
		}
		else if (input == 2)
		{
			list();
		}
		else if (input == 0)
		{
			printf("You are quitting the list.\n ");
			return 0;
		}
		else
		{
			printf("Error. Please enter 1, 2, or 0.\n ");
		}
	}
}

void insert()
{
	char name[20];
	printf("Type a name.");
        scanf("%s",name);
	for (int i = 0; i < counter; i++)	//go through array and compare if the name is already in the array
	{
		if (strcmp(name,names[i])==0)
		{
			printf("This name has already been added.\n ");			
			return;
		}	 
	}
	int dept;
	printf("Type in dept.\n ");
	scanf("%d",&dept);

	if (dept >= 1 && dept <= 4)	//check if dept number is entered correctly
	{
		deptList[counter]=dept;		//add dept number to dept array
		strcpy(names[counter],name);	//add name to array
		counter++;	//move to the next space in the array
	}
	else
	{
		printf("Sorry. Incorrect dept number.");
		return;
	}	
				
}

void list()
{
	for (int i = 0; i < counter; i++)
	{ 	
		printf("%s  %d\n ", names[i], deptList[i]);
	}	 
}

