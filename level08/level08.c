#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

void log_wrapper(FILE *log_file, const char *prefix, char **message)
{
	char buffer[256];
	int newline_pos;

	strcpy(buffer, prefix);
	snprintf(buffer + strlen(buffer), 254 - strlen(buffer), "%s", *message);
	
	newline_pos = strcspn(buffer, "\n");
	buffer[newline_pos] = 0;
	
	fprintf(log_file, "LOG: %s\n", buffer);
}

int main(int argc, char **argv)
{
	FILE *backup_log;
	FILE *source_file;
	char backup_path[100] = "./backups/";
	int backup_fd;
	char buffer;

	if (argc != 2) {
		printf("Usage: %s filename\n", argv[0]);
		return 1;
	}

	backup_log = fopen("./backups/.log", "a");
	if (!backup_log) {
		printf("ERROR: Failed to open %s\n", "./backups/.log");
		exit(1);
	}

	log_wrapper(backup_log, "Starting back up: ", &argv[1]);

	source_file = fopen(argv[1], "r");
	if (!source_file) {
		printf("ERROR: Failed to open %s\n", argv[1]);
		exit(1);
	}

	strncat(backup_path, argv[1], 99);
	backup_fd = open(backup_path, O_CREAT | O_WRONLY | O_EXCL, 0660);
	if (backup_fd < 0) {
		printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
		exit(1);
	}

	while ((buffer = fgetc(source_file)) != EOF) {
		write(backup_fd, &buffer, 1);
	}

	log_wrapper(backup_log, "Finished back up ", &argv[1]);
	
	fclose(source_file);
	close(backup_fd);
	return 0;
}