run: output
	./out 
output: lexer.c parser.c hashtable.c hash.c color.c set.c test.c
	gcc -c lexer.c
	gcc -c parser.c
	gcc -c hashtable.c
	gcc -c hash.c
	gcc -c color.c
	gcc -c set.c
	gcc lexer.o parser.o hashtable.o hash.o color.o set.o test.c -o out

clean:
	rm -vf *.o
