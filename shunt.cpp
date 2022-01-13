#include <cstdint>

#include <vector>
#include <queue>
#include <stack>
#include <map>

#include <string>
#include <variant>
#include <functional>
#include <cmath>
#include <limits>

using namespace std::literals;
#include "thrower.h"
#include "print.h"


using Operator  = std::function<double(double,double)> ;
using Token     = std::variant<double, Operator >;
using RPN       = std::vector<Token>;


double power(double lhs, double rhs)
{
    return std::pow(lhs,rhs);
}

double NaN(double lhs, double rhs)
{
    return std::numeric_limits<double>::quiet_NaN();
}

struct Parse
{
    int         precedence;
    Operator    op;

    Parse(int precedence, Operator op) : precedence{precedence}, op{op}
    {}

};

std::map<char, Parse> precedence
{
    { '('   , {4, NaN} },
    { ')'   , {4, NaN} },
    { '^'   , {3, power} },
    { '*'   , {2, std::multiplies<double>{} }},
    { '/'   , {2, std::divides<double>{}    }},
    { '+'   , {1, std::plus<double>{}       }},
    { '-'   , {1, std::minus<double>{}      }},
};



double resolve(RPN  const  &tokens)
{                     
    std::stack<double>   numbers;

    auto pop = [&]
    {
        auto top=numbers.top();
        numbers.pop();
        return top;
    };


    for(auto const &token : tokens)
    {
        if(std::holds_alternative<double>(token))
        {
            numbers.push( std::get<double>(token));
        }
        else
        {
            if(numbers.size()<2)
            {
                throw_runtime_error(std::format("Size is {} instead of 2 or more",numbers.size()));
            }

            auto right     = pop();
            auto left      = pop();
            auto operation = std::get<Operator>(token);

            numbers.push (operation(left,right));
        }
    }

    if(numbers.size()!=1)
    {
        throw_runtime_error(std::format("Size is {} instead of 1",numbers.size()));
    }

    return numbers.top();
}


void testRPN()
{
    std::vector<Token>  sum        {  6.0, 9.0, std::plus<double>{} };
    std::vector<Token>  product    {  6.0, 9.0, std::multiplies<double>{} };
    std::vector<Token>  difference {  6.0, 9.0, std::minus<double>{} };
    std::vector<Token>  ratio      {  6.0, 9.0, std::divides<double>{} };
    std::vector<Token>  exponent   {  6.0, 9.0, power };
                                              
    print("sum        = {}\n",resolve(sum));
    print("product    = {}\n",resolve(product));
    print("difference = {}\n",resolve(difference));
    print("ratio      = {}\n",resolve(ratio));
    print("exponent   = {}\n",resolve(exponent));
}

bool isOperator(char c)
{
    return precedence.contains(c);    
}

auto shunt(std::vector<std::string> const &tokens)
{
    RPN                     rpn;
    std::stack<char>        operators;

    auto transferOperator = [&]
    {
        auto symbol = operators.top();
        operators.pop();

        switch(symbol)
        {
        case '+':
            rpn.push_back(std::plus<double>{});
            break;

        case '-':
            rpn.push_back(std::minus<double>{});
            break;



        }

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


int main(int argc, char const *argv[])
try
{
    std::vector<std::string>  args(argv+1,argv+argc);

    if(args.empty())
    {
        throw_runtime_error("No calculation");
    }

    auto rpn = shunt(args);

    print("Result = {}\n",resolve(rpn));
    

    return 0;
}
catch(std::exception const &e)
{
    print("Caught {}\n",e.what());
}