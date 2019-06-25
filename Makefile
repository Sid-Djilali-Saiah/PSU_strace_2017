##
## EPITECH PROJECT, 2021
## PSU_strace_2018
## File description:
## Makefile
##

NAME	= strace

CC	= gcc

RM	= rm -f

SRCS	=   ./main.c \
            ./src/strace.c \
            ./src/display.c \
            ./src/trace.c

OBJS	= $(SRCS:.c=.o)

CFLAGS = -I./inc/
CFLAGS += -W -Wall -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
