#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STORAGE_SIZE 100

unsigned int get_unum() {
	unsigned int num;
	scanf("%u", &num);
	return num;
}

int read_number(int *data) {
	unsigned int index;
	printf(" Index: ");
	index = get_unum();
	printf(" Number at data[%u] is %u\n", index, data[index]);
	return 0;
}

int store_number(int *data) {
	unsigned int number, index;
	
	printf(" Number: ");
	number = get_unum();
	printf(" Index: ");
	index = get_unum();
	
	if ((index % 3 == 0) || ((number >> 24) == 0xb7)) {
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return 1;
	}
	
	data[index] = number;
	return 0;
}

int main(int argc, char **argv, char **envp) {
	int data[STORAGE_SIZE] = {0};
	char command[20] = {0};
	int ret;
	
	// Clear argv and envp
	for (int i = 0; argv[i]; i++)
		memset(argv[i], 0, strlen(argv[i]));
	for (int i = 0; envp[i]; i++)
		memset(envp[i], 0, strlen(envp[i]));
	
	puts("----------------------------------------------------\n"
		 "  Welcome to wil's crappy number storage service!   \n"
		 "----------------------------------------------------\n"
		 " Commands:                                          \n"
		 "    store - store a number into the data storage    \n"
		 "    read  - read a number from the data storage     \n"
		 "    quit  - exit the program                        \n"
		 "----------------------------------------------------\n"
		 "   wil has reserved some storage :>                 \n"
		 "----------------------------------------------------\n");
	
	while (1) {
		printf("Input command: ");
		ret = 1;
		fgets(command, sizeof(command), stdin);
		command[strcspn(command, "\n")] = 0;
		
		if (strcmp(command, "store") == 0)
			ret = store_number(data);
		else if (strcmp(command, "read") == 0)
			ret = read_number(data);
		else if (strcmp(command, "quit") == 0)
			break;
			
		if (ret == 0)
			printf(" Completed %s command successfully\n", command);
		else
			printf(" Failed to do %s command\n", command);
			
		memset(command, 0, sizeof(command));
	}
	
	return 0;
}