
output: lexer.c parser.c hashtable.c hash.c color.c set.c driver.c stack.c nary_tree.c
	gcc -c stack.c
	gcc -c nary_tree.c
	gcc -c lexer.c
	gcc -c parser.c
	gcc -c hashtable.c
	gcc -c hash.c
	gcc -c color.c
	gcc -c set.c
	gcc stack.o nary_tree.o lexer.o parser.o hashtable.o hash.o color.o set.o driver.c -o stage1exe
	rm -vf *.o out *~

