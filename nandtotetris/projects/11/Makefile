CC := gcc
CFLAGS := -g -Wall -Wextra -v
main: ./JackAnalyzer.c
	$(RM) JackAnalyzer
	$(CC) ./VMWriter.c ./SymbolTable.c ./JackTokenizer.c ./CompilationEngine.c ./JackAnalyzer.c -o ./JackAnalyzer $(CFLAGS)
clear:
	$(RM) JackAnalyzer
