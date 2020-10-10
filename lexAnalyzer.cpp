#include "lexer.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <map>

void writeLexems(std::vector<Lex> lexems)
{
    std::ofstream o("resultLexems.csv", std::ofstream::binary);
    o << "Line, Raw, Type, Value" << std::endl;
    for (int i = 0; i < lexems.size(); i++)
    {
        Lex lexem = lexems[i];
        if (lexem.type != LexTypes::SINGLE_LINE_COMMENT && lexem.type != LexTypes::MULTI_LINE_COMMENT)
        {
            std::string escapedRaw = lexem.raw;
            o << lexem.line << ", " << escapedRaw << "," << lexIdToName[lexem.type];
            if (lexem.val != -1)
            {
                o << "," << lexem.val;
            }
            o << std::endl;
        }
    }
    o.close();
}

std::map<std::string, int> keywordsMap;
std::map<std::string, int> idsMap;

void writeKeywords(std::map<std::string, int> keywords)
{
    std::ofstream o("resultKeywords.csv", std::ofstream::binary);
    o << "Keyword, index" << std::endl;
    for (std::pair<std::string, int> p : keywords)
    {
        o << p.first << ", " << p.second << std::endl;
    }
    o.close();
}

void writeIds(std::map<std::string, int> ids)
{
    std::ofstream o("resultIds.csv", std::ofstream::binary);
    o << "ID, index" << std::endl;
    for (std::pair<std::string, int> p : ids)
    {
        o << p.first << ", " << p.second << std::endl;
    }
    o.close();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Please pass file name as parametr";
        return -1;
    }

    std::ifstream file(argv[1]);
    LexerResult lexerResult = parseCode(file);

    writeLexems(lexerResult.lexems);
    writeKeywords(lexerResult.keywordsMap);
    writeIds(lexerResult.idsMap);
}