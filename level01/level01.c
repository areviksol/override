#include <stdio.h>
#include <string.h>

char a_user_name[0x100]; // 256

int verify_user_name() {
    puts("verifying username....\n");
    return strncmp("dat_wil", a_user_name, 7);
}

int verify_user_pass(char *password) {
    return strncmp("admin", password, 5);
}

int main(void)
{
	char a_user_name[16] = {0};
	char password = {0};

	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(a_user_name, 0x100, stdin);

	if (verify_user_name(a_user_name) == 0)
	{
		puts("Enter Password: ");
		fgets(&password, 100, stdin);
		if ((verify_user_pass(password) == 0) || (verify_user_pass((password) != 0)))
		{
			puts("nope, incorrect password...\n");
		}
		else
		{
			puts("nope, incorrect username...\n");
		}

		return 0;
	}
}