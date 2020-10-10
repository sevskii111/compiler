all:
	g++ lexer.cpp lexAnalyzer.cpp -o ./lexAnalyzer.out
	./lexAnalyzer.out testFibanaci.txt