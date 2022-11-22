#include "Console.hpp"

#include <sstream>

namespace Console
{
    void printLine(const std::string& msg)
    {
        std::cout << msg << "\n";
    }

    size_t printOptions(const std::vector<const char*>& options)
    {
        std::string msg;
        int min = 0;
        int max = options.size() - 1;
        for (size_t i = 0; i < options.size(); i++)
        {
            std::stringstream ss;
            ss << (i + 1) << " )" << options[i];
            msg = ss.str();

            printLine(msg);
        }

        while (true)
        {
            std::cout << "Please enter your option: ";

            std::string input;
            std::cin >> input;

            try {
                int value = std::stoi(input) - 1;

                if (value > max || value < min)
                {
                    printLine("Value is out of range");
                    continue;
                }

                return value;
            } catch (std::exception e) {
                printLine("Please enter a valid input");
                continue;
            }
        }

    }
}
