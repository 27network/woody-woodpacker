#include <stdio.h>
#include <unistd.h>

int main(void)
{
	while (1)
	{
		printf("Waiting 5 seconds...\n");
		sleep(5);
	}
}
