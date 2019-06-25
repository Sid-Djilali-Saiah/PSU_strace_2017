/*
** EPITECH PROJECT, 2021
** PSU_strace_2018
** Email: sid.djilali-saiah@epitech.eu
** Created by Sid Djilali Saiah,
*/

#include "strace.h"

int	display_exit(int exit_status, int mode)
{
	if (WIFEXITED(exit_status))
	{
		fprintf(stderr, mode ? "exit_group(%d) = ?\n"
			: "exit_group(0x%x) = ?\n", WEXITSTATUS(exit_status));
		fprintf(stderr, "+++ exited with %d +++\n",
			WEXITSTATUS(exit_status));
	}
	else
	{
		printf("+++ killed by %s (core dumped) +++\n",
			signames[WSTOPSIG(exit_status)]);
		printf("%s (core dumped)\n",
			strsignal(WSTOPSIG(exit_status)));
	}
	return (EXIT_SUCCESS);
}

static char	*read_string(pid_t child, long addr)
{
	long	read = 0;
	long	tmp;
	char	*val = malloc(sizeof(char) * 4906);

	if (!val)
		return (NULL);
	while (1)
	{
		if (read + sizeof(tmp) > 4096)
			val = realloc(val, 8192);
		tmp = ptrace(PTRACE_PEEKDATA, child, addr + read);
		if (errno != 0) {
			val[read] = 0;
			break;
		}
		memcpy(val + read, &tmp, sizeof(tmp));
		if (memchr(&tmp, 0, sizeof(tmp)) != NULL)
			break;
		read += sizeof(tmp);
	}
	return (val);
}

void	display_arg(pid_t child, long arg, int mode, enum e_arg_type type)
{
	char	*str;
	int	h;

	if (mode && type == INT)
		fprintf(stderr, "%ld", arg);
	else if (mode && type == STR)
	{
		str = read_string(child, arg);
		h = -1;
		fprintf(stderr, "\"");
		while (str[++h] && h < 32)
			fprintf(stderr, isprint(str[h]) ? "%c"
				: "\\%o", str[h]);
		h >= 32 ? fprintf(stderr, "\"...") : fprintf(stderr, "\"");
		free(str);
	}
	else arg ? fprintf(stderr, "0x%lx", arg)
		: fprintf(stderr, (mode ? "NULL" : "0x0"));
}
