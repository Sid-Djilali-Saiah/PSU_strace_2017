/*
** EPITECH PROJECT, 2021
** PSU_strace_2018
** Email: sid.djilali-saiah@epitech.eu
** Created by Sid Djilali Saiah,
*/

#ifndef STRACE_H_
#define STRACE_H_

#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#define FUNC_MAX_LEN 50
#define EXIT_SUCCESS 0
#define MAX_SYSCALL_NUM 331
#define SYSCALL_CODE 0x050F
#define SYSCALL_MAXARGS 6
#define EXIT_FAILURE 1

extern char *signames[];

enum 	e_arg_type
{
	INT,
	PTR,
	STR
};

typedef struct		s_entry
{
	enum e_arg_type	retval;
	const char	*name;
	enum e_arg_type	args_value[SYSCALL_MAXARGS];
	int		nb_args;
}			t_entry;

void	display_syscall(pid_t child, int sysnum, long retval, int mode);
void	display_arg(pid_t child, long arg, int mode, enum e_arg_type type);
int	display_exit(int status, int mode);

int	launch_child(int argc, char **argv, int mode);
int	attach_to_process(pid_t pid);
int	trace(pid_t pid, int mode);

#endif /*!STRACE_H_*/
