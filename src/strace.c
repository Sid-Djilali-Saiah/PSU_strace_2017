/*
** EPITECH PROJECT, 2021
** PSU_strace_2018
** Email: sid.djilali-saiah@epitech.eu
** Created by Sid Djilali Saiah,
*/

#include "strace.h"

int	launch_child(int argc, char **argv, int mode)
{
	char	*args[argc + 1];
	int	i;

	i = -1;
	while (++i < argc)
		args[i] = argv[i + 1 + mode];
	args[i] = NULL;
	ptrace(PTRACE_TRACEME);
	kill(getpid(), SIGSTOP);
	if (execvp(argv[1 + mode], args) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	attach_to_process(pid_t pid)
{
	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1)
	{
		perror("Ptrace attach: ");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

inline static int	signal_status_handler(int wait_status)
{
	return ((WSTOPSIG(wait_status) == SIGTRAP
		|| WSTOPSIG(wait_status) == SIGSTOP)
		&& WIFSTOPPED(wait_status));
}

int	trace(pid_t pid, int mode)
{
	struct user_regs_struct	r;
	unsigned short		instr_code;
	int			wait_status;

	waitpid(pid, &wait_status, 0);
	while (signal_status_handler(wait_status)) {
		if (ptrace(PTRACE_GETREGS, pid, NULL, &r))
			perror("ptrace");
		instr_code = ptrace(PTRACE_PEEKTEXT, pid, r.rip, NULL);
		if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1)
			perror("ptrace");
		waitpid(pid, &wait_status, 0);
		if (signal_status_handler(wait_status)) {
			if (ptrace(PTRACE_GETREGS, pid, NULL, &r))
				perror("ptrace");
			if (instr_code == SYSCALL_CODE)
				display_syscall(pid, r.orig_rax, r.rax, mode);
		} else if (WIFSTOPPED(wait_status)) {
			siginfo_t	sig;
			ptrace(PTRACE_GETSIGINFO, pid, NULL, &sig);
			printf("--- %s {si_signo=%s, si_code=%d, "
					"si_addr=%p} ---\n",
				signames[WSTOPSIG(wait_status)],
				signames[WSTOPSIG(wait_status)], sig.si_code,
				sig.si_addr);
			ptrace(PTRACE_CONT, pid, NULL, NULL);
		}
	}
	return (wait_status);
}
