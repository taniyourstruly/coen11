#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int correct = 0;
	srand((int) time (NULL));
	for (int x = 0; x < 10; x++)
	{
		int divisor = rand()%12+1;
		int quotient = rand() % 13;
		int dividend = divisor * quotient;
		printf ("dividend = %d, divisor = %d\n", dividend, divisor);
		int answer;
		scanf("%d", &answer);
		if (answer == quotient)
		{
			printf ("right\n");
			correct++;
		}
		else
		{
			printf ("wrong\n");
			print ("quotient = %d\n", quotient);
		}
	}
	printf ("number correct = %d\n", correct);
}
