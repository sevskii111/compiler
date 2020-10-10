all:
	g++ lexer.cpp lexAnalyzer.cpp -o ./lexAnalyzer.out
	g++ lexer.cpp test.cpp -o ./test.out