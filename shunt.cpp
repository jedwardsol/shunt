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

std::map<std::string, Parse> parse
{
    { "^^"  , {3, static_cast<OpPointer>(std::pow)} },             // ^ is an escape character in cmd
    { "^"   , {3, static_cast<OpPointer>(std::pow)} },
    { "*"   , {2, std::multiplies<double>{} }},
    { "/"   , {2, std::divides<double>{}    }},
    { "+"   , {1, std::plus<double>{}       }},
    { "-"   , {1, std::minus<double>{}      }},


};




bool isOperator(std::string const &c)
{
    return parse.contains(c);    
}

bool isOpen(std::string const &c)
{
    return c == "(";    
}

bool isClose(std::string const &c)
{
    return c == ")";    
}



RPN shunt(std::vector<std::string> const &tokens)
{
    RPN                         rpn;
    std::stack<std::string>     operators;

    auto transferOperator = [&]
    {
        auto symbol = operators.top();
        operators.pop();

        rpn.push_back( parse[symbol].op);
    };

    for(auto const &token : tokens)
    {
        if(isOpen(token))
        {
            operators.push(token);
        }
        else if(isClose(token))
        {
            while(   !operators.empty()
                  && !isOpen(operators.top()))
            {
                transferOperator();
            }

            if(operators.empty())
            {
                throw_runtime_error("Mismatched parentheses");
            }

            operators.pop();

        }
        else if(isOperator(token))
        {
            while(   !operators.empty()
                  && !isOpen(operators.top())
                  &&  parse[operators.top()].precedence >= parse[token].precedence)
            {
                transferOperator();
            }
            
            operators.push(token);
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

