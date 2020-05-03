CC=g++
OBJDIR=build
BINDIR=bin
BIN=hhfi
STD=c++11

build: checkdir $(OBJDIR)/main.o $(OBJDIR)/parser.o $(OBJDIR)/interpreter.o $(OBJDIR)/lexer.o
	$(CC) -o $(BINDIR)/$(BIN) $(OBJDIR)/*.o -std=$(STD)

force:
	$(CC) -o $(BINDIR)/$(BIN) $(OBJDIR)/*.o -std=$(STD)

run: build
	./$(BINDIR)/$(BIN)

$(OBJDIR)/main.o: src/main.h src/main.cpp
	$(CC) -c src/main.cpp -o $(OBJDIR)/main.o -std=$(STD)

$(OBJDIR)/interpreter.o: src/interpreter.h src/interpreter.cpp
	$(CC) -c src/interpreter.cpp -o $(OBJDIR)/interpreter.o -std=$(STD)

$(OBJDIR)/parser.o: src/parser.h src/parser.cpp
	$(CC) -c src/parser.cpp -o $(OBJDIR)/parser.o -std=$(STD)

$(OBJDIR)/lexer.o: src/lexer/lexer.h src/lexer/lexer.cpp src/lexer/lexemes.h src/lexer/lexemes.cpp src/lexer/lexemestream.h
	$(CC) -c src/lexer/lexer.cpp -o $(OBJDIR)/lexer.o -std=$(STD)
	$(CC) -c src/lexer/lexemes.cpp -o $(OBJDIR)/lexemes.o -std=$(STD)

clean:
	rm build/*
	rm bin/hhfi

checkdir:
	if ! [ -d "$(OBJDIR)" ]; then mkdir $(OBJDIR); fi
	if ! [ -d "$(BINDIR)" ]; then mkdir $(BINDIR); fi