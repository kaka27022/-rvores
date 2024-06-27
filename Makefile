DIR = implementacao

all.o: utilitarios.o main.o sequencial.o arvore_binaria.o arvoreB.o arvoreBs.o
	@gcc main.o utilitarios.o sequencial.o arvore_binaria.o arvoreB.o arvoreBs.o -o pesquisa
	@rm main.o utilitarios.o sequencial.o arvore_binaria.o arvoreB.o arvoreBs.o

main.o: main.c
	@gcc -c main.c -Wall

utilitarios.o: $(DIR)/utilitarios.c $(DIR)/utilitarios.h
	@gcc -c $(DIR)/utilitarios.c -Wall

sequencial.o: $(DIR)/sequencial.c $(DIR)/sequencial.h
	@gcc -c $(DIR)/sequencial.c -Wall

arvore_binaria.o: $(DIR)/arvore_binaria.c $(DIR)/arvore_binaria.h
	@gcc -c $(DIR)/arvore_binaria.c -Wall

arvoreB.o: $(DIR)/arvoreB.c $(DIR)/arvoreB.h
	@gcc -c $(DIR)/arvoreB.c -Wall

arvoreBs.o: $(DIR)/arvoreBs.c $(DIR)/arvoreBs.h
	@gcc -c $(DIR)/arvoreBs.c -Wall
