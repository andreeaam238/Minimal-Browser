CC = gcc
CFLAG = -Wall -Wextra -g 
EXE = tema2
CODE = lista.c coada.c stiva.c utils.c browser.c main.c

build: $(EXE)

$(EXE): $(CODE) 
	$(CC) $(CFLAG) $(CODE) -o $(EXE)

clean: $(EXE)
	rm -f $(EXE)
