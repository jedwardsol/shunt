#include <cstdint>
#include <limits>
#include <cmath>

#include <vector>
#include <queue>
#include <stack>
#include <map>

#include <string>

#include "thrower.h"
#include "print.h"
#include "shunt.h"



double NaN(double lhs, double rhs)
{
    return std::numeric_limits<double>::quiet_NaN();
}

struct Parse
{
    int         precedence{};
    Operator    op;

};

std::map<char, Parse> precedence
{
    { '^'   , {3, static_cast<OpPointer>(std::pow)} },
    { '*'   , {2, std::multiplies<double>{} }},
    { '/'   , {2, std::divides<double>{}    }},
    { '+'   , {1, std::plus<double>{}       }},
    { '-'   , {1, std::minus<double>{}      }},
};




bool isOperator(char c)
{
    return precedence.contains(c);    
}

RPN shunt(std::vector<std::string> const &tokens)
{
    RPN                     rpn;
    std::stack<char>        operators;

    auto transferOperator = [&]
    {
        auto symbol = operators.top();
        operators.pop();

        rpn.push_back( precedence[symbol].op);
    };



    for(auto const &token : tokens)
    {
        auto const symbol = token[0];

        if(isOperator(symbol))
        {
            
            operators.push(symbol);
        }
        else
        {
            rpn.push_back(std::stod(token));
        }
    }


    while(!operators.empty())
    {
        transferOperator();
    }


    return      rpn;
}

