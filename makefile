output: tetris.o 
	gcc tetris.o -o output -lncurses

main.o: tetris.c
	gcc -c tetris.c 

clean: 
	rm *.o output debug

debug: tetris.o
	gcc -g tetris.c -o debug -lncurses