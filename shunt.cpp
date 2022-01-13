#include <cstdint>

#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <variant>
#include <memory>
#include <functional>
#include <cmath>

using namespace std::literals;
#include "thrower.h"
#include "print.h"


using Operation = std::function<double(double,double)> ;
using Token    = std::variant<double, Operation >;

double rpn(std::vector<Token> const  &tokens)
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
        if(token.index()==0)
        {
            numbers.push( std::get<0>(token));
        }
        else
        {
            if(numbers.size()<2)
            {
                throw_runtime_error(std::format("Size is {} instead of 2 or more",numbers.size()));
            }

            auto right     = pop();
            auto left      = pop();
            auto operation = std::get<1>(token);

            numbers.push (operation(left,right));
        }
    }

    if(numbers.size()!=1)
    {
        throw_runtime_error(std::format("Size is {} instead of 1",numbers.size()));
    }

    return numbers.top();
}

double power(double lhs, double rhs)
{
    return std::pow(lhs,rhs);
}

void testRPN()
{
    std::vector<Token>  sum        {  6.0, 9.0, std::plus<double>{} };
    std::vector<Token>  product    {  6.0, 9.0, std::multiplies<double>{} };
    std::vector<Token>  difference {  6.0, 9.0, std::minus<double>{} };
    std::vector<Token>  ratio      {  6.0, 9.0, std::divides<double>{} };
    std::vector<Token>  exponent   {  6.0, 9.0, power };
                                              
    print("sum        = {}\n",rpn(sum));
    print("product    = {}\n",rpn(product));
    print("difference = {}\n",rpn(difference));
    print("ratio      = {}\n",rpn(ratio));
    print("exponent   = {}\n",rpn(exponent));
}

int main(int argc, char const *argv[])
try
{
    testRPN();

    return 0;
}
catch(std::exception const &e)
{
    print("Caught {}\n",e.what());
}