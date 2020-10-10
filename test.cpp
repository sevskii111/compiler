#include "lexer.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

bool printComments = true;

void printLexems(std::vector<Lex> lexems)
{
    for (int i = 0; i < lexems.size(); i++)
    {
        Lex lexem = lexems[i];
        if ((lexem.type != LexTypes::SINGLE_LINE_COMMENT && lexem.type != LexTypes::MULTI_LINE_COMMENT) || printComments)
        {
            std::cout << lexem.line << "\t" << lexem.raw << "\t" << lexIdToName[lexem.type] << "\t" << lexem.val << std::endl;
        }
    }
}

std::string readFile(std::string fileName)
{
    std::ifstream t(fileName);
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());

    return str;
}

int main()
{
    std::ifstream numbersFile("testNumber.txt");
    LexerResult lexerResult = parseCode(numbersFile);
    std::vector<Lex> lexems = lexerResult.lexems;
    printLexems(lexems);

    std::cout << std::endl;
    std::cout << std::endl;

    std::ifstream mixedFile("testMix.txt");
    lexerResult = parseCode(mixedFile);
    lexems = lexerResult.lexems;
    printLexems(lexems);

    std::cout << std::endl;
    std::cout << std::endl;

    std::ifstream fibanaciFile("testFibanaci.txt");
    lexerResult = parseCode(fibanaciFile);
    lexems = lexerResult.lexems;
    printLexems(lexems);
}