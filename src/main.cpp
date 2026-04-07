/*
  main.cpp - CLI entry point for the resistor color code calculator.
  Part of Rosito Bench — a collection of pure C++17 CLI engineering utilities.

  Usage:
    resistor -v <value> -p <tolerance>   Convert a value to color bands
    resistor -c <color,color,...>        Convert color bands to a value

  Created by Alex Rosito, April 2026.
  Valley Glen, Los Angeles, California.

  Released under the MIT License.
  © Alex Rosito
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include <map>
#include "resistor.h"

// Maps CLI flags to integer codes for use in switch statements
const std::map<std::string, int> flagMap = {
    {"-v", 0},
    {"-p", 1},
    {"-c", 2}
};

void printHelp()
{
    std::cout << "\nUsage: resistor [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -v    Value of the resistor in ohms (e.g.: 1000, 4700, 2.2k, 1M)\n";
    std::cout << "  -p    Tolerance in percentage (e.g.: 5, 1, 0.5)\n";
    std::cout << "  -c    Color code (e.g.: red,brown,black,gold)\n";
    std::cout << "  -h    Show this help\n\n";
    std::cout << "Examples:\n";
    std::cout << "  resistor -v 1000 -p 5\n";
    std::cout << "  resistor -v 1000 -p 1\n";
    std::cout << "  resistor -c red,brown,black,gold\n";
    std::cout << "  resistor -c red,brown,black,black,brown\n";
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printHelp();
        return 0;
    }

    // ---------------------------------------------------------------------------
    // First pass: detect which flags are present
    // ---------------------------------------------------------------------------
    bool vArg = false;
    bool cArg = false;
    bool pArg = false;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg.size() > 2)
            arg = arg.substr(0, 2);
        if (arg == "-v") vArg = true;
        if (arg == "-p") pArg = true;
        if (arg == "-c") cArg = true;
        if (arg == "-h") { printHelp(); return 0; }
    }

    // ---------------------------------------------------------------------------
    // Validation: check for invalid flag combinations
    // -c and -v are mutually exclusive
    // -c and -p are mutually exclusive
    // -v without -p is invalid (tolerance is required)
    // ---------------------------------------------------------------------------
    if (cArg && pArg || cArg && vArg || vArg && !pArg)
    {
        std::cerr << "Invalid parameters format\n" << std::endl;
        printHelp();
        return -1;
    }

    // ---------------------------------------------------------------------------
    // Second pass: extract values and execute
    // ---------------------------------------------------------------------------
    std::string arg, flag, value;
    double tolerance     = 0.0;
    double resistorValue = 0.0;
    std::vector<std::string> colorValues;

    for (int i = 1; i < argc; i++)
    {
        arg = argv[i];

        // Handle flags with value attached (e.g., -v220 or -cbrown,black,green,gold)
        if (arg.size() > 2)
        {
            flag  = arg.substr(0, 2);
            value = arg.substr(2);
        }
        else
        {
            // Handle flags with value separated by space (e.g., -v 220)
            flag = argv[i];
            if (i + 1 >= argc)
            {
                std::cerr << "Missing value after " << flag << std::endl;
                return -1;
            }
            std::string nextArg = argv[i + 1];
            if (nextArg[0] == '-')
            {
                std::cerr << "Invalid parameters format\n" << std::endl;
                printHelp();
                return -1;
            }
            value = argv[++i];
        }

        switch (flagMap.at(flag))
        {
        case 0: // -v: parse resistor value
            try
            {
                resistorValue = parseValue(value);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
                return 1;
            }
            break;

        case 1: // -p: parse tolerance
            try
            {
                tolerance = parseTolerance(value);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
                return 1;
            }
            break;

        case 2: // -c: parse color bands
            try
            {
                colorValues = parseColors(value);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
                return 1;
            }
            break;
        }
    }

    // ---------------------------------------------------------------------------
    // Output
    // ---------------------------------------------------------------------------

    // Color bands to value
    if (cArg)
    {
        if (colorValues.size() == 4)
            std::cout << "The resistor value is:  " << fourBandColorsToValue(colorValues) << std::endl;
        if (colorValues.size() == 5)
            std::cout << "The resistor value is:  " << fiveBandColorsToValue(colorValues) << std::endl;
    }

    // Value to color bands
    if (vArg)
        std::cout << valueToColors(resistorValue, tolerance) << std::endl;

    return 0;
}
