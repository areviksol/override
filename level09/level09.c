#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void secret_backdoor(void)
{
	char *string;
	
	fgets(&string, 0x80, stdin);
	system(&string);
	return;
}

void set_msg(char *arg1)
{
	char src[128];
	
	for (int i = 0; i < 128; i++) {
		src[i] = 0;
	}
	
	puts("Message: ");
	printf(">> ");
	fgets(src, 1024, stdin);
	strncpy(arg1, src, *(int32_t *)(arg1 + 0xb4));
	return;
}

void set_username(int64_t arg1)
{
	char s[16];
	int var_ch;
	
	for (int i = 0; i < 16; i++) {
		s[i] = 0;
	}
	
	puts("Username: ");
	printf(">> ");
	fgets(s, 128, stdin);
	
	for (var_ch = 0; var_ch < 41 && s[var_ch] != '\0'; var_ch++) {
		*(char *)(arg1 + 0x8c + var_ch) = s[var_ch];
	}
	
	printf("Welcome, %s\n", (char *)(arg1 + 0x8c));
	return;
}

void handle_msg(void)
{
	char buffer[128];
	int64_t *ptr = (int64_t *)buffer;
	
	for (int i = 0; i < 16; i++) {
		ptr[i] = 0;
	}
	
	set_username((int64_t)buffer);
	set_msg(buffer);
	
	puts(">: Msg sent!");
	return;
}

int main(void)
{
	puts("--------------------------------------------\n"\
		 "|   ~Welcome to l33t-m$n ~    v1337        |\n"\
		 "--------------------------------------------");
	handle_msg();
	return 0;
}