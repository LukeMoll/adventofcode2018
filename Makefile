CC=g++ 
# boo, no clang in UoY CSE
CFLAGS=-Wall -Wextra -Wpedantic -std=c++17 -g
# all the warnings, C++17, -g for stack traces

SRCDIR=src/
BINDIR=bin/
OBJDIR=obj/

DEPS = $(OBJDIR)aocutils.o

.PHONY: clean		
clean: 
	rm -f bin/* obj/*

.SECONDARY:
# don't delete object files

# general build rules: bin/% -> obj/%.o -> src/%.c
$(BINDIR)%: $(OBJDIR)%.o $(DEPS) $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(DEPS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

# folder dependencies
$(OBJDIR) $(BINDIR) $(SRCDIR):
	mkdir -p $@
