
#include "list.h"

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
