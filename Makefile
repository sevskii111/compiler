all:
	g++  -std=gnu++11 lexer.cpp lexAnalyzer.cpp -o ./lexAnalyzer.out
	g++  -std=gnu++11 lexer.cpp test.cpp -o ./test.out