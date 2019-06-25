/*
** EPITECH PROJECT, 2021
** PSU_strace_2018
** Email: sid.djilali-saiah@epitech.eu
** Created by Sid Djilali Saiah,
*/

#include "strace.h"
#include "syscalls.h"

static int registers[SYSCALL_MAXARGS] = {RDI,
					RSI,
					RDX,
					R10,
					R8,
					R9};

static long	get_register_value(pid_t child, int pos)
{
	int	offset;
	long	value;

	offset = sizeof(long) * pos;
	value = ptrace(PTRACE_PEEKUSER, child, offset);
	return (value);
}

const char	*get_syscall_name(int num)
{
	t_entry		*ent;
	static char	buff[FUNC_MAX_LEN];

	if (num <= MAX_SYSCALL_NUM)
	{
		ent = &g_syscall[num];
		if (ent->name)
			return (ent->name);
	}
	snprintf(buff, sizeof(buff), "sys_%d", num);
	return (buff);
}

static void	display_syscall_args(pid_t child, int num, int mode)
{
	int		i;
	long		arg;
	int		argc = SYSCALL_MAXARGS;
	t_entry		*ent = NULL;
	enum e_arg_type	type;

	if ((num <= MAX_SYSCALL_NUM) && g_syscall[num].name)
	{
		ent = &g_syscall[num];
		argc = ent->nb_args;
	}
	i = 0;
	while (i < argc)
	{
		type = ent ? ent->args_value[i] : PTR;
		arg = get_register_value(child, registers[i]);
		display_arg(child, arg, mode, type);
		if (i++ != argc - 1)
			fprintf(stderr, ", ");
	}
}

void	display_syscall(pid_t child, int sysnum, long retval, int mode)
{
	t_entry	*ent;

	fprintf(stderr, "%s(", get_syscall_name(sysnum));
	display_syscall_args(child, sysnum, mode);
	fprintf(stderr, ") = ");
	ent = &g_syscall[sysnum];
	if (mode)
		fprintf(stderr, ent->retval == INT ? "%ld\n"
			: "0x%lx\n", retval);
	else
		retval ? fprintf(stderr, "0x%lx\n", retval)
			: fprintf(stderr, "0x0\n");
}
