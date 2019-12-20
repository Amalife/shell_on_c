CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
CINCLUDES = -I ./includes

NAME = Shredder

CFILES = $(wildcard srcs/*.c)

OBJECTS = $(patsubst %.c, %.o, $(CFILES))

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC)  $(CFLAGS) -o $@ $(OBJECTS)

%.o : %.c
	$(CC) $(CFLAGS) $(CINCLUDES) -o $@ -c $^

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

