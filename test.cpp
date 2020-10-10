#include "lexer.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

bool printComments = false;

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
    std::string testNumbers = readFile("testNumber.txt");
    std::cout << "testing numbers parsing:" << std::endl;
    std::cout << testNumbers;
    std::cout << std::endl;
    LexerResult lexerResult = parseCode(testNumbers);
    std::vector<Lex> lexems = lexerResult.lexems;
    printLexems(lexems);

    std::cout << std::endl;
    std::cout << std::endl;

    std::string testMixed = readFile("testMix.txt");
    std::cout << "testing mixed parsing:" << std::endl;
    std::cout << testMixed;
    std::cout << std::endl;
    lexerResult = parseCode(testMixed);
    lexems = lexerResult.lexems;
    printLexems(lexems);

    std::cout << std::endl;
    std::cout << std::endl;

    std::string testFibanaci = readFile("testFibanaci.txt");
    std::cout << "testing fibanaci parsing:" << std::endl;
    std::cout << testFibanaci;
    std::cout << std::endl;
    lexerResult = parseCode(testFibanaci);
    lexems = lexerResult.lexems;
    printLexems(lexems);
}