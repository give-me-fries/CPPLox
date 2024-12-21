#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc > 2)
    {
        std::cout << "Usage: cpplox [script]\n";
        std::exit(64);
    }
    else if (argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        runPrompt()
    }

    return 0;
}