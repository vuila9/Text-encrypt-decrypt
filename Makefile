a1:			JohnNguyenA1.c
			gcc -o a1 JohnNguyenA1.c

valgrind:		a1
			valgrind --leak-check=yes ./a1
