
#include <stack>

#include "shunt.h"
#include "print.h"
#include "thrower.h"


class Resolver
{
public:
    
    double  result()
    {
        if(numbers.size()!=1)
        {
            throw_runtime_error(std::format("Size is {} instead of 1",numbers.size()));
        }

        return numbers.top();
    }

    double pop()
    {
        auto top=numbers.top();
        numbers.pop();
        return top;
    };

    void operator()(double const &number)
    {
        numbers.push( number);
    }

    void operator()(Operator const &op)
    {
        if(numbers.size()<2)
        {
            throw_runtime_error(std::format("Size is {} instead of 2 or more",numbers.size()));
        }

        auto right     = pop();
        auto left      = pop();

        numbers.push (op(left,right));
    }


private:
    std::stack<double>   numbers;
};


double resolve(RPN  const  &tokens)
{                     
    Resolver    resolver;

    for(auto const &token : tokens)
    {
        std::visit(resolver,token);
    }

    return resolver.result();
}


void testRPN()
{
    std::vector<Token>  sum        {  6.0, 9.0, std::plus<double>{} };
    std::vector<Token>  product    {  6.0, 9.0, std::multiplies<double>{} };
    std::vector<Token>  difference {  6.0, 9.0, std::minus<double>{} };
    std::vector<Token>  ratio      {  6.0, 9.0, std::divides<double>{} };
    std::vector<Token>  exponent   {  5.0, 2.0, static_cast<OpPointer>(std::pow)};
                                              
    print("sum        = {}\n",resolve(sum));
    print("product    = {}\n",resolve(product));
    print("difference = {}\n",resolve(difference));
    print("ratio      = {}\n",resolve(ratio));
    print("exponent   = {}\n",resolve(exponent));
}
