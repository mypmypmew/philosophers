CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lpthread

# Source files and output
SRCS = main.c simulation.c philo_init.c philo_routines.c utils.c utils_time.c utils_routine.c
OBJS = $(SRCS:.c=.o)
TARGET = philo

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re