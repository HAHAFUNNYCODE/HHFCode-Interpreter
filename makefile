CC=g++
OBJDIR=build
OBJS := $(addprefix $(OBJDIR)/,main.o lexer.o lexemes.o timer.o)
BINDIR=bin
BIN=hhfi
STD=c++11

vpath %.cpp src src/lexer src/misc
vpath %.h src src/lexer src/misc

$(BINDIR)/$(BIN): $(OBJDIR) $(BINDIR) $(OBJS)
	$(CC) $(OBJDIR)/*.o -o $@

run: $(BINDIR)/$(BIN)
	./$(BINDIR)/$(BIN)

test: $(BINDIR)/$(BIN)
	./$(BINDIR)/$(BIN) testfile.txt

$(OBJDIR)/%.o: %.cpp %.h
	$(CC) -c $< -o $@ -std=$(STD)

clean: $(OBJDIR) $(BINDIR)
	rm -r $(OBJDIR) $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)