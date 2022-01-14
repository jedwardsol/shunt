#pragma once


#include <type_traits>
#include <functional>
#include <variant>
#include <vector>
#include <string>

using OpProto   = double(double,double);
using OpPointer = std::add_pointer_t<OpProto>;

using Operator  = std::function<OpProto> ;
using Token     = std::variant<double, Operator >;
using RPN       = std::vector<Token>;


RPN     shunt(std::vector<std::string> const &tokens);

double  resolve(RPN  const  &tokens);
void    testRPN();
