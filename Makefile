Sudoku: Sudoku.c
	gcc -std=c99 -o Sudoku.o Sudoku.c
clean: 
	rm -f Sudoku.o
debug: Sudoku.c
	gcc -std=c99 -g -o Sudoku.o Sudoku.c
