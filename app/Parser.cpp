#include "Parser.h"

#include <sstream>
#include <iterator>

Parser::Parser()
{
}

std::vector<std::string> Parser::parseSpace(std::string str, bool cut)
{
    std::vector<std::string> strList;

    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf), end;

    std::vector<std::string> tokens(beg, end);

    for(auto& s: tokens) {
        if(cut) {
            s.erase(0,2);
        }

        strList.push_back(s);
    }

    return strList;
}
