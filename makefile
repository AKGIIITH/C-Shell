CC = gcc
CFLAGS = -Wall -g


SRCS = main.c display.c utils.c input.c hop.c execution.c reveal.c log.c proclore.c seek.c alias.c global.c display_activity.c signal_handler.c iMan.c

OBJS = $(SRCS:.c=.o)
TARGET = myshell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# Optional rule to run the shell (if desired)
run: $(TARGET)
	./$(TARGET)
