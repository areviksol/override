#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	int iVar1;
	int64_t iVar3;
	char **ppcVar4;
	char **var_128h;
	int var_11ch;
	char *filename;
	char *ptr;
	char format[100];
	int var_14h;
	FILE *stream;

	var_14h = 0;
	stream = (FILE *)fopen("/home/users/level03/.pass", "r");
	if (stream == (FILE *)0x0)
	{
		fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
		exit(1);
	}
	var_14h = fread(&ptr, 1, 0x29, stream);
	iVar3 = strcspn(&ptr, 0x29);

	if (var_14h != 0x29)
	{
		fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
		fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
		exit(1);
	}

	fclose(stream);
	puts("===== [ Secure Access System v1.0 ] =====");
	puts("| You must login to access this system. |");
	printf("--[ Username: ");
	fgets(&format, 100, stdin);
	iVar3 = strcspn(&format, "\n");
	format[iVar3] = 0;
	printf("--[ Password: ");
	fgets(&filename, 100, stdin);
	iVar3 = strcspn(&filename, "\n");
	format[iVar3] = 0;
	puts("*****************************************");
	iVar1 = strncmp(&ptr, &filename, 0x29);

	if (iVar1 != 0)
	{
		printf(&format);
		puts(" does not have access!");
		exit(1);
	}
	printf("Greetings, %s!\n", &format);
	system("/bin/sh");
	return 0;
}