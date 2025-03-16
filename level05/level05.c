#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	char buffer[100];
	int i = 0;

	fgets(buffer, sizeof(buffer), stdin);

	while (1)
	{
		if (i >= strlen(buffer))
		{
			printf("%s", buffer);
			exit(0);
		}

		if (buffer[i] > '@' && buffer[i] < '[')
		{
			buffer[i] += 32;
		}

		i++;
	}

	return 0;
}
