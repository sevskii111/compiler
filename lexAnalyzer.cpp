#include "lexer.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <map>

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

void writeLexems(std::vector<Lex> lexems)
{
    std::ofstream o("resultLexems.csv");
    o << "Line, Raw, Type, Value" << std::endl;
    for (int i = 0; i < lexems.size(); i++)
    {
        Lex lexem = lexems[i];
        if (lexem.type != LexTypes::SINGLE_LINE_COMMENT && lexem.type != LexTypes::MULTI_LINE_COMMENT)
        {
            o << lexem.line << "," << lexem.raw << "," << lexIdToName[lexem.type];
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
    std::ofstream o("resultKeywords.csv");
    o << "Keyword, index" << std::endl;
    for (std::pair<std::string, int> p : keywords)
    {
        o << p.first << ", " << p.second << std::endl;
    }
    o.close();
}

void writeIds(std::map<std::string, int> ids)
{
    std::ofstream o("resultIds.csv");
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

    std::string code = readFile(argv[1]);
    LexerResult lexerResult = parseCode(code);

    writeLexems(lexerResult.lexems);
    writeKeywords(lexerResult.keywordsMap);
    writeIds(lexerResult.idsMap);
}