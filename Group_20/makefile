
output: codegen.c symboltable.c semantic.c ast.c lexer.c parser.c hashtable.c hash.c color.c set.c driver.c stack.c nary_tree.c treefiles.c
	gcc -c codegen.c
	gcc -c symboltable.c
	gcc -c semantic.c
	gcc -c ast.c
	gcc -c stack.c
	gcc -c nary_tree.c
	gcc -c lexer.c
	gcc -c parser.c
	gcc -c hashtable.c
	gcc -c hash.c
	gcc -c color.c
	gcc -c set.c
	gcc -c treefiles.c
	gcc codegen.o treefiles.o symboltable.o semantic.o ast.o stack.o nary_tree.o lexer.o parser.o hashtable.o hash.o color.o set.o driver.c -o stage1exe
	rm -vf *.o out *~

