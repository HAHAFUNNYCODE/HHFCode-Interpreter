CC=g++
OBJDIR=build
BINDIR=bin
BIN=hhfi

build: checkdir $(OBJDIR)/main.o $(OBJDIR)/parser.o $(OBJDIR)/interpreter.o
	$(CC) -o $(BINDIR)/$(BIN) $(OBJDIR)/*.o

run: build
	./$(BINDIR)/$(BIN)

$(OBJDIR)/main.o: src/main.h src/main.cpp
	$(CC) -c src/main.cpp -o $(OBJDIR)/main.o -I.

$(OBJDIR)/interpreter.o: src/interpreter.h src/interpreter.cpp
	$(CC) -c src/interpreter.cpp -o $(OBJDIR)/interpreter.o -I.

$(OBJDIR)/parser.o: src/parser.h src/parser.cpp
	$(CC) -c src/parser.cpp -o $(OBJDIR)/parser.o -I. 

clean:
	rm build/*
	rm bin/hhfi

checkdir:
	if ! [ -d "$(OBJDIR)" ]; then mkdir $(OBJDIR); fi
	if ! [ -d "$(BINDIR)" ]; then mkdir $(BINDIR); fi