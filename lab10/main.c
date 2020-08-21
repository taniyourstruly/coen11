
#include "list.h"

/*declare AND initialize*/

struct array waitingInfo[4];


pthread_mutex_t mutex;


/*only main*/
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

