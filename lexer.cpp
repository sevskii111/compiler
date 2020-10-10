#include "lexer.h"

const std::vector<std::string>
    keywords = {"if", "for", "in", "of", "return"};

const std::set<char> space = {' ', '\n', '\t', '\0', '\r'};
const std::set<char> asgnStart = {'=', '*', '/', '+', '-'};
const std::set<char> deliStart = {';', '(', ')', '{', '}', '>', '<', '&', '|'};
const std::set<char> es = {'e', 'E'};

bool isSpace(char c)
{
    return space.count(c);
}

bool isCorrectIdStart(char c)
{
    return c == '$' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isCorrectIdPart(char c)
{
    return c == '$' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isDigit(c);
}

bool isAsgnStart(char c)
{
    return asgnStart.count(c);
}

bool isDeliStart(char c)
{
    return deliStart.count(c);
}

bool isCommentStart(char c)
{
    return c == '/';
}

bool isE(char c)
{
    return es.count(c);
}

bool isSign(char c)
{
    return c == '-' || c == '+';
}

Lex::Lex(int _line, LexTypes _type, std::string _raw, double _dval = -1)
{
    line = _line;
    type = _type;
    raw = _raw;
    val = _dval;
}

enum PosInNum
{
    NONE,
    SIGN,
    WHOLE_PART,
    DOT,
    FRAC_PART,
    E,
    E_PART
};

double calcNumberValue(double numberValue, bool isNumNegative, int eValue = 0, bool isENegative = false)
{
    if (isNumNegative)
    {
        numberValue = -numberValue;
    }
    if (isENegative)
    {
        eValue = -eValue;
    }
    return numberValue * pow(10, eValue);
}

char lookahead(std::istream &code)
{
    return code.peek();
}

std::map<std::string, int> makeKeywordsMap()
{
    std::map<std::string, int> result;
    for (int i = 0; i < keywords.size(); i++)
    {
        result[keywords[i]] = i;
    }
    return result;
}

LexerResult parseCode(std::istream &code)
{
    int start = code.tellg();

    int line_num = 1;
    std::string buff;
    LexTypes state = LexTypes::H;

    std::vector<Lex> lexems;
    std::map<std::string, int> keywordsMap = makeKeywordsMap();
    std::map<std::string, int> idsMap;

    PosInNum numberState = PosInNum::NONE;
    double numberValue = 0;
    int eValue = 0;
    unsigned int numberPointAfterDot = 1;
    bool isNumNegative = false;
    bool isENegative = false;

    char c = code.get();

    bool isScanFinished = false;
    bool isLastStep = false;

    while (!isScanFinished)
    {
        bool makeStep = true;
        switch (state)
        {
        case LexTypes::ERR:
            lexems.push_back(Lex(line_num, LexTypes::ERR, buff));
            state = LexTypes::H;
            break;

        case LexTypes::H:
            if (isSpace(c))
                ;
            else
            {
                buff = c;
                if (isCorrectIdStart(c))
                {
                    state = LexTypes::ID;
                }
                else if (isDigit(c))
                {
                    state = LexTypes::NUM;
                    numberValue = c - '0';
                    eValue = 0;
                    numberPointAfterDot = 1;
                    isNumNegative = false;
                    isENegative = false;
                    numberState = PosInNum::WHOLE_PART;
                }
                else if (c == '.')
                {
                    state = LexTypes::NUM;
                    numberValue = 0;
                    eValue = 0;
                    numberPointAfterDot = 1;
                    isNumNegative = false;
                    isENegative = false;
                    numberState = PosInNum::DOT;
                }
                else if (c == '"')
                {
                    state = LexTypes::STR;
                }
                else if (isAsgnStart(c))
                {
                    if (c == '=')
                    {
                        lexems.push_back(Lex(line_num, LexTypes::ASGN, buff));
                        state = LexTypes::H;
                    }
                    else
                    {
                        char next = lookahead(code);
                        if (next == '=')
                        {
                            buff += next;
                            code.get();
                            lexems.push_back(Lex(line_num, LexTypes::ASGN, buff));
                        }
                        else
                        {
                            if (isSign(c))
                            {
                                numberState = PosInNum::SIGN;
                                numberValue = 0;
                                eValue = 0;
                                numberPointAfterDot = 1;
                                isNumNegative = c == '-';
                                isENegative = false;
                                state = LexTypes::NUM;
                            }
                            else if (isCommentStart(c))
                            {
                                char next = lookahead(code);
                                if (next == '/')
                                {
                                    state = LexTypes::SINGLE_LINE_COMMENT;
                                }
                                else if (next = '*')
                                {
                                    state = LexTypes::MULTI_LINE_COMMENT;
                                }
                                else
                                {
                                    state = LexTypes::ERR;
                                }
                            }
                            else
                            {
                                state = LexTypes::ERR;
                            }
                        }
                    }
                }
                else if (isDeliStart(c))
                {
                    char next = lookahead(code);
                    if ((c == '<' && next == '=') || (c == '>' && next == '=') || (c == '&' && next == '&') || (c == '|' && next == '|'))
                    {
                        buff += next;
                        code.get();
                    }
                    lexems.push_back(Lex(line_num, LexTypes::DELI, buff));
                }
                else
                {
                    state = LexTypes::ERR;
                }
            }
            break;
        case SINGLE_LINE_COMMENT:
            if (c == '\n')
            {
                lexems.push_back(Lex(line_num, LexTypes::SINGLE_LINE_COMMENT, buff));
                state = LexTypes::H;
            }
            else
            {
                buff += c;
            }
        case MULTI_LINE_COMMENT:
            if (c == '*' && lookahead(code) == '/')
            {
                lexems.push_back(Lex(line_num, LexTypes::MULTI_LINE_COMMENT, buff));
                code.get();
                state = LexTypes::H;
            }
            else
            {
                buff += c;
            }
            break;

        case STR:
            if (c == '\\' && lookahead(code) == '\"')
            {
                buff += '\"';
                code.get();
            }
            else
            {
                buff += c;
                if (c == '"')
                {
                    lexems.push_back(Lex(line_num, LexTypes::STR, buff));
                    state = LexTypes::H;
                }
            }
            break;

        case NUM:
            switch (numberState)
            {
            case SIGN:
                if (isDigit(c))
                {
                    numberState = PosInNum::WHOLE_PART;
                    numberValue = c - '0';
                    buff += c;
                }
                else if (c == '.')
                {
                    numberState = PosInNum::DOT;
                    buff += c;
                }
                else
                {
                    lexems.push_back(Lex(line_num, LexTypes::ERR, buff));
                    makeStep = false;
                    state = LexTypes::H;
                }
                break;

            case WHOLE_PART:
                if (c == '.')
                {
                    numberState = PosInNum::DOT;
                    buff += c;
                }
                else if (isE(c))
                {
                    numberState = PosInNum::E;
                    buff += c;
                }
                else if (isDigit(c))
                {
                    buff += c;
                    numberValue = numberValue * 10 + (c - '0');
                }
                else
                {
                    lexems.push_back(Lex(line_num, LexTypes::NUM, buff, calcNumberValue(numberValue, isNumNegative)));
                    makeStep = false;
                    state = LexTypes::H;
                }

                break;

            case DOT:
                if (isDigit(c))
                {
                    numberState = PosInNum::FRAC_PART;
                    numberValue += 1.0 * (c - '0') / pow(10, numberPointAfterDot);
                    numberPointAfterDot += 1;
                    buff += c;
                }
                else if (isE(c))
                {
                    if (buff.length() > 1 && isDigit(buff[buff.length() - 2]))
                    {
                        numberState = PosInNum::E;
                        buff += c;
                    }
                    else
                    {
                        lexems.push_back(Lex(line_num, LexTypes::ERR, buff));
                        makeStep = false;
                        state = LexTypes::H;
                    }
                }
                else
                {
                    if (buff.length() > 1 && isDigit(buff[buff.length() - 2]))
                    {
                        lexems.push_back(Lex(line_num, LexTypes::NUM, buff, calcNumberValue(numberValue, isNumNegative)));
                        makeStep = false;
                        state = LexTypes::H;
                    }
                    else
                    {
                        lexems.push_back(Lex(line_num, LexTypes::ERR, buff));
                        makeStep = false;
                        state = LexTypes::H;
                    }
                }
                break;

            case FRAC_PART:
                if (isE(c))
                {
                    numberState = PosInNum::E;
                    buff += c;
                }
                else if (isDigit(c))
                {
                    numberValue += 1.0 * (c - '0') / pow(10, numberPointAfterDot);
                    numberPointAfterDot += 1;
                    buff += c;
                }
                else
                {
                    lexems.push_back(Lex(line_num, LexTypes::NUM, buff, calcNumberValue(numberValue, isNumNegative)));
                    makeStep = false;
                    state = LexTypes::H;
                }
                break;

            case E:
                if (isDigit(c))
                {
                    buff += c;
                    eValue = c - '0';
                    numberState = PosInNum::E_PART;
                }
                else if (c == '-')
                {
                    buff += c;
                    isENegative = true;
                }
                else if (c == '+')
                {
                    buff += c;
                    isENegative = false;
                }
                else
                {
                    lexems.push_back(Lex(line_num, LexTypes::ERR, buff));
                    makeStep = false;
                    state = LexTypes::H;
                }
                break;
            case E_PART:
                if (isDigit(c))
                {
                    buff += c;
                    eValue = eValue * 10 + (c - '0');
                    numberState = PosInNum::E_PART;
                }
                else
                {
                    lexems.push_back(Lex(line_num, LexTypes::NUM, buff, calcNumberValue(numberValue, isNumNegative, eValue, isENegative)));
                    makeStep = false;
                    state = LexTypes::H;
                }
                break;

            default:
                state = LexTypes::ERR;
                break;
            }
            break;

        case ID:
            if (isCorrectIdPart(c))
            {
                buff += c;
            }
            else
            {
                if (keywordsMap.count(buff))
                {
                    lexems.push_back(Lex(line_num, LexTypes::KEYWORD, buff, keywordsMap[buff]));
                }
                else if (idsMap.count(buff))
                {
                    lexems.push_back(Lex(line_num, LexTypes::ID, buff, idsMap[buff]));
                }
                else
                {
                    int newIdPos = idsMap.size();
                    lexems.push_back(Lex(line_num, LexTypes::ID, buff, newIdPos));
                    idsMap[buff] = newIdPos;
                }
                makeStep = false;
                state = LexTypes::H;
            }

            break;

        default:
            break;
        }

        if (makeStep)
        {
            if (c == '\n')
                line_num++;
            if (isLastStep)
            {
                isScanFinished = true;
            }
            else
            {
                c = code.get();
                if (code.eof())
                {
                    c = '\0';
                    isLastStep = true;
                }
            }
        }
    }

    LexerResult result;
    result.idsMap = idsMap;
    result.keywordsMap = keywordsMap;
    result.lexems = lexems;

    code.clear();
    code.seekg(start);

    return result;
}