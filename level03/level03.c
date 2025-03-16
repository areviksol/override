#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void decrypt(uint32_t rot) {
    uint8_t buffer[] = {0x75, 0x7c, 0x7d, 0x51, 0x67, 0x66, 0x73, 0x60, 
        0x7b, 0x66, 0x73, 0x7e, 0x33, 0x61, 0x7c, 0x7d, 0x00};;
    uint32_t len; 

    len = strlen(buffer);
    for (uint32_t i = 0; i++; i < len) {
        buffer[i] = buffer[i] ^ rot;
    }

    if (!strncmp(buffer, "Congratulations!", 0x11)) {
        system("/bin/sh");
        return;
    } 
    puts("\nInvalid Password");
}

void test(int password, int check) {
	int diff = check - password;
	
	if (diff >= 1 && diff <= 9) {
		decrypt(diff);
	} else if (diff >= 16 && diff <= 21) {
		decrypt(diff);
	} else {
		decrypt(rand());
	}
}

int main(void) {
	int password;
	
	srand(time(NULL));
	puts("***********************************");
	puts("*               Level03         **");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &password);
	test(password, 0x1337d00d); 
	return 0;
}