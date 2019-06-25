/*
** EPITECH PROJECT, 2021
** PSU_strace_2018
** Email: sid.djilali-saiah@epitech.eu
** Created by Sid Djilali Saiah,
*/

#include "strace.h"

char *signames[] = 	{"INVALID",
	"SIGHUP",
	"SIGINT",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGABRT",
	"SIGBUS",
	"SIGFPE",
	"SIGKILL",
	"SIGUSR1",
	"SIGSEGV",
	"SIGUSR2",
	"SIGPIPE",
	"SIGALRM",
	"SIGTERM",
	"SIGSTKFLT",
	"SIGCHLD",
	"SIGCONT",
	"SIGSTOP",
	"SIGTSTP",
	"SIGTTIN",
	"SIGTTOU",
	"SIGURG",
	"SIGXCPU",
	"SIGXFSZ",
	"SIGVTALRM",
	"SIGPROF",
	"SIGWINCH",
	"SIGPOLL",
	"SIGPWR",
	"SIGSYS",
	NULL};

void		show_usage(int argc, char **av)
{
	if (argc < 2)
	{

		fprintf(stderr,
			"Usage : %s [-s] [-p <pid>|<command>]\n", av[0]);
		exit(1);
	}
}

int		main(int argc, char **av)
{
	pid_t	pid;
	int	mode = 0;

	show_usage(argc, av);
	if (!strcmp(av[1], "-s"))
		mode = 1;
	if (av[1 + mode] && !strcmp(av[1 + mode], "-p") && av[2 + mode]) {
		if (attach_to_process((pid = (pid_t)atoi(av[2 + mode]))))
			return (EXIT_FAILURE);
	}
	else {
		if (!av[1 + mode])
			return (fprintf(stderr,
				"Invalid option for -s\n") || 1);
		pid = fork();
		if (pid == 0)
			return (launch_child(argc, av, mode));
	}
	return (display_exit(trace(pid, mode), mode));
}