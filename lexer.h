#pragma once

#include <string>
#include <cmath>
#include <vector>
#include <set>
#include <map>

enum LexTypes
{
    H,
    ID,
    KEYWORD,
    NUM,
    STR,
    ASGN,
    DELI,
    ERR,
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT
};

static std::map<LexTypes, std::string> lexIdToName = {
    {LexTypes::ID, "ID"},
    {LexTypes::KEYWORD, "Keyword"},
    {LexTypes::NUM, "Number"},
    {LexTypes::STR, "String"},
    {LexTypes::ASGN, "Assignator"},
    {LexTypes::DELI, "Delimiter"},
    {LexTypes::ERR, "Unrecognized"},
    {LexTypes::SINGLE_LINE_COMMENT, "Single line comment"},
    {LexTypes::MULTI_LINE_COMMENT, "Multi line comment"}};

class Lex
{
public:
    int line;
    LexTypes type;
    double val = -1;
    std::string raw;

    Lex(int _line, LexTypes _type, std::string _raw, double _dval);
};

class LexerResult
{
public:
    std::vector<Lex> lexems;
    std::map<std::string, int> keywordsMap;
    std::map<std::string, int> idsMap;
};

LexerResult parseCode(std::string code);