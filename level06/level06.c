#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

#define PTRACE_TRACEME 0

int auth(char *login, int serial)
{
	int len;
	unsigned long hash;

	len = strcspn(login, "\n");
	login[len] = '\0';
	len = strnlen(login, 32);

	if (len < 6)
		return 1;

	if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
	{
		puts("You should not debug this");
		return 1;
	}

	hash = (login[3] ^ 0x1337) + 0x5eeded;
	for (int i = 0; i < len; i++)
	{

		if (login[i] <= ' ')
			return 1;
		hash += (login[i] ^ hash) % 0x539;
	}

	return (serial != hash);
}

int main(int argc, char **argv)
{
	char login[32];
	int serial;

	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");
	printf("-> Enter Login: ");
	fgets(login, 32, stdin);
	puts("***********************************");
	puts("***** NEW ACCOUNT DETECTED ********");
	puts("***********************************");
	printf("-> Enter Serial: ");
	scanf("%d", &serial);

	if (auth(login, serial) == 0)
	{
		puts("Authenticated!");
		system("/bin/sh");
	}

	return 1;
}