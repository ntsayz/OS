# Usage:
# make        # compile all binaries
# make clean  # remove all binaries, objects and pipes

.PHONY = all clean

CC = gcc                        # compiler to use

LINKERFLAG = -lm

SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

all: ${BINS} 

%: %.o
		@echo "Checking.."
		${CC} ${LINKERFLAG} $< -o $@

%.o: %.c
		@echo "Creating objects.."
		${CC} -c $<

clean:
		@echo "Cleaning up.."
		rm -rvf *.o ${BINS}
		find  . -name 'pipe*' -exec rm {} \;