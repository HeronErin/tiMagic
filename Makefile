CC      = gcc
CFLAGS  = -g
RM      = rm -f


default: TIM


TIM: src/main.c
	$(CC) $(CFLAGS) -o TIM src/main.c

run: clean TIM
	./TIM

clean:
	-$(RM) TIM
