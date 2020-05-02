CC=g++
OBJDIR=build
BINDIR=bin
BIN=hhfi

build: checkdir build/main.o
	$(CC) -o $(BINDIR)/$(BIN) $(OBJDIR)/*.o

run: build
	./$(BINDIR)/$(BIN)

build/main.o: src/main.h src/main.cpp
	$(CC) -c src/main.cpp -o $(OBJDIR)/main.o

clean:
	rm build/*
	rm bin/hhfi

checkdir:
	if ! [ -d "$(OBJDIR)" ]; then mkdir $(OBJDIR); fi
	if ! [ -d "$(BINDIR)" ]; then mkdir $(BINDIR); fi