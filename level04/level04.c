
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	pid_t child_pid;
	int status;
	int traced_pid;

	child_pid = fork();

	if (child_pid == 0)
	{
		prctl(PR_SET_PDEATHSIG, SIGKILL);
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		puts("Give me some shellcode, k");
		char buffer[128];
		gets(buffer);
	}
	else
	{
		while (1)
		{
			wait(&status);

			if (WIFEXITED(status) || WIFSIGNALED(status))
			{
				puts("child is exiting...");
				return 0;
			}

			traced_pid = ptrace(PTRACE_PEEKUSER, child_pid, 44, NULL);
			if (traced_pid == 11)
			{
				puts("no exec() for you");
				kill(child_pid, SIGKILL);
				break;
			}
		}
	}
	return 0;
}