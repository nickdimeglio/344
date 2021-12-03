setup:
	gcc -std=gnu99 main.c cmd.c smallsh.c smallsh_signals.c -o smallsh 

clean:
	rm smallsh

