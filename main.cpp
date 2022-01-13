#include <vector>
#include <string>
#include <exception>


#include"shunt.h"
#include"print.h"
#include"thrower.h"


int main(int argc, char const *argv[])
try
{
//  testRPN();

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