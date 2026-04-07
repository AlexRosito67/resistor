/*
  resistor.h - Resistor color code calculator library for Rosito Bench.
  Supports bidirectional conversion between resistor values and color bands
  for both 4-band and 5-band resistors. Includes ANSI terminal color output.

  Part of Rosito Bench — a collection of pure C++17 CLI engineering utilities.
  No dependencies. No frameworks. No runtime overhead.

  Created by Alex Rosito, April 2026.
  Valley Glen, Los Angeles, California.

  Released under the MIT License.
  © Alex Rosito
*/

#ifndef RESISTOR_H
#define RESISTOR_H

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <iomanip>
#include <cmath>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Color tables
// ---------------------------------------------------------------------------

// Valid tolerance colors for 5-band resistors (last band)
const std::vector<std::string> fiveBandsResistorTolerances = {
    "brown", "red", "green", "blue", "violet", "grey"
};

// Valid tolerance colors for 4-band resistors (last band)
const std::vector<std::string> fourBandsResistorTolerances = {
    "gold", "silver"
};

// All valid resistor band colors
const std::vector<std::string> validColors = {
    "black", "brown", "red", "orange", "yellow",
    "green", "blue", "violet", "grey", "white", "gold", "silver"
};

// Maps color names to their significant digit values (0–9)
const std::map<std::string, int> colorDigits = {
    {"black", 0}, {"brown", 1}, {"red",    2}, {"orange", 3}, {"yellow", 4},
    {"green", 5}, {"blue",  6}, {"violet", 7}, {"grey",   8}, {"white",  9}
};

// Maps color names to their multiplier values
const std::map<std::string, double> colorMultipliers = {
    {"black",  1.0},       {"brown",  10.0},      {"red",    100.0},
    {"orange", 1000.0},    {"yellow", 10000.0},   {"green",  100000.0},
    {"blue",   1000000.0}, {"violet", 10000000.0},
    {"grey",   0.01},      {"white",  0.1},
    {"gold",   0.1},       {"silver", 0.01}
};

// Maps color names to their tolerance percentage values
const std::map<std::string, double> colorTolerances = {
    {"gold",   5.0},  {"silver", 10.0},
    {"brown",  1.0},  {"red",    2.0},
    {"green",  0.5},  {"blue",   0.25},
    {"violet", 0.1},  {"grey",   0.05}
};

// Maps color names to their ANSI terminal escape sequences for colored output
const std::map<std::string, std::string> ansiColors = {
    {"black",  "\033[48;2;30;30;30mbk\033[0m"},
    {"brown",  "\033[48;2;139;69;19mbn\033[0m"},
    {"red",    "\033[48;2;255;0;0mrd\033[0m"},
    {"orange", "\033[48;2;255;165;0mog\033[0m"},
    {"yellow", "\033[48;2;255;255;0mye\033[0m"},
    {"green",  "\033[48;2;0;128;0mgn\033[0m"},
    {"blue",   "\033[48;2;0;0;255mbl\033[0m"},
    {"violet", "\033[48;2;148;0;211mvt\033[0m"},
    {"grey",   "\033[48;2;128;128;128mgy\033[0m"},
    {"white",  "\033[48;2;255;255;255mwh\033[0m"},
    {"gold",   "\033[48;2;255;215;0mgl\033[0m"},
    {"silver", "\033[48;2;192;192;192msr\033[0m"}
};

// ---------------------------------------------------------------------------
// Formatting
// ---------------------------------------------------------------------------

/*
  Formats a resistor value and tolerance into a human-readable string.
  Applies engineering suffixes (K, M) when appropriate.
  Includes the valid measurement range based on the tolerance.
*/
std::string formatedResistorValue(double resistorValue, double tolerance)
{
    bool isK = false;
    bool isM = false;

    if (resistorValue >= 1000.0 && resistorValue < 1000000.0)
    {
        isK = true;
        resistorValue /= 1000.0;
    }
    else if (resistorValue >= 1000000.0)
    {
        isM = true;
        resistorValue /= 1000000.0;
    }

    std::string suffix = (isK ? "K" : isM ? "M" : "");

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1)
        << resistorValue << suffix << "\u03A9  +/- " << tolerance << "%\n"
        << "Valid range: "
        << resistorValue * (1 - tolerance / 100) << suffix << "\u03A9"
        << " - "
        << resistorValue * (1 + tolerance / 100) << suffix << "\u03A9\n"
        << "If the measured value is outside this range, return the resistors lot.\n"
        << "You've been scammed!\n";

    return oss.str();
}

// ---------------------------------------------------------------------------
// Color code to value
// ---------------------------------------------------------------------------

/*
  Converts a 4-band resistor color code into a formatted value string.
  Band order: digit1, digit2, multiplier, tolerance.
*/
std::string fourBandColorsToValue(std::vector<std::string> &colorsFound)
{
    double digit1    = colorDigits.at(colorsFound.at(0));
    double digit2    = colorDigits.at(colorsFound.at(1));
    double multiplier = colorMultipliers.at(colorsFound.at(2));
    double tolerance  = colorTolerances.at(colorsFound.at(3));
    double resistorValue = (digit1 * 10 + digit2) * multiplier;
    return formatedResistorValue(resistorValue, tolerance);
}

/*
  Converts a 5-band resistor color code into a formatted value string.
  Band order: digit1, digit2, digit3, multiplier, tolerance.
*/
std::string fiveBandColorsToValue(std::vector<std::string> &colorsFound)
{
    double digit1    = colorDigits.at(colorsFound.at(0));
    double digit2    = colorDigits.at(colorsFound.at(1));
    double digit3    = colorDigits.at(colorsFound.at(2));
    double multiplier = colorMultipliers.at(colorsFound.at(3));
    double tolerance  = colorTolerances.at(colorsFound.at(4));
    double resistorValue = (digit1 * 100 + digit2 * 10 + digit3) * multiplier;
    return formatedResistorValue(resistorValue, tolerance);
}

// ---------------------------------------------------------------------------
// Value to color code
// ---------------------------------------------------------------------------

/*
  Searches for matching values in a map and appends the corresponding
  color name keys to the output container.
  Template parameter T allows use with both int and double maps.
*/
template <typename T>
void lookForColors(const std::vector<T> &digit,
                   std::vector<std::string> &myContainer,
                   const std::map<std::string, T> &myMap)
{
    for (size_t i = 0; i < digit.size(); i++)
        for (const auto &pair : myMap)
            if (pair.second == digit.at(i))
                myContainer.push_back(pair.first);
}

/*
  Converts a numeric resistor value and tolerance into an ANSI color-band
  representation for terminal output.

  Determines the number of bands from the tolerance:
    gold (5%) and silver (10%) → 4 bands
    all precision tolerances   → 5 bands

  Finds the multiplier by repeatedly dividing the value by 10 until the
  quotient falls within the valid digit range. Extracts the significant
  digits, looks up each color, and returns a formatted ANSI string.
*/
std::string valueToColors(double value, double tolerance)
{
    // Determine number of bands from tolerance
    bool fourBands = (tolerance == 5.0 || tolerance == 10.0);

    double multiplier = 1.0;
    int exponent = 0;
    std::vector<int> digit(fourBands ? 2 : 3);

    if (value > 9.0)
    {
        // Divide until the value falls in the correct digit range
        if (fourBands)
            while (!(value > 9.0 && value < 100.0)) { value /= 10.0; exponent++; }
        else
            while (!(value > 99.0 && value < 1000.0)) { value /= 10.0; exponent++; }

        // Calculate multiplier from exponent
        for (int i = 1; i <= exponent; i++)
            multiplier *= 10.0;

        // Extract significant digits
        if (fourBands)
        {
            digit[0] = (int)value / 10;
            digit[1] = (int)value % 10;
        }
        else
        {
            digit[0] = (int)value / 100;
            digit[1] = ((int)value % 100) / 10;
            digit[2] = (int)value % 10;
        }
    }
    else
    {
        // Values below 10Ω (e.g., 4R7 = 4.7Ω)
        digit[0] = (int)value;
        digit[1] = (int)(value * 10) % 10;
    }

    // Build vectors for multiplier and tolerance lookup
    std::vector<double> parsedMultiplier(1, (double)multiplier);
    std::vector<double> parsedTolerance(1, (double)tolerance);

    // Look up colors for digits, multiplier, and tolerance
    std::vector<std::string> resistorColors;
    lookForColors(digit, resistorColors, colorDigits);
    lookForColors(parsedMultiplier, resistorColors, colorMultipliers);
    lookForColors(parsedTolerance, resistorColors, colorTolerances);

    // Build ANSI output string
    std::string result = "\nYour resistor's color code should look like this:  ";
    for (const auto &color : resistorColors)
    {
        auto ansiIt = ansiColors.find(color);
        if (ansiIt != ansiColors.end())
        {
            result += "\033[48;2;180;180;180m \033[0m";
            result += ansiIt->second;
        }
    }
    result += "\033[48;2;180;180;180m \033[0m\n";

    return result;
}

// ---------------------------------------------------------------------------
// Parsing
// ---------------------------------------------------------------------------

/*
  Parses a comma-separated resistor color string and validates its format.
  Accepts 4 or 5 bands. Converts all color names to lowercase.
  Validates the last band against the appropriate tolerance list.
  Validates all other bands against the full valid color list.
*/
std::vector<std::string> parseColors(const std::string &colorsStr)
{
    try
    {
        char comma = ',';
        size_t commaOccurrences = std::count(colorsStr.begin(), colorsStr.end(), comma);

        if (commaOccurrences < 3 || commaOccurrences > 4)
            throw std::invalid_argument("Invalid colors format: " + colorsStr);

        std::vector<std::string> colorsFound;
        std::stringstream ss(colorsStr);
        std::string value;

        // Split by comma and convert to lowercase
        while (std::getline(ss, value, ','))
        {
            std::transform(value.begin(), value.end(), value.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            colorsFound.push_back(value);
        }

        // Validate the last band (tolerance)
        bool isValid = false;
        const auto &toleranceList = (commaOccurrences == 3)
                                    ? fourBandsResistorTolerances
                                    : fiveBandsResistorTolerances;

        for (const std::string &t : toleranceList)
            if (colorsFound.back() == t) { isValid = true; break; }

        if (!isValid)
            throw std::invalid_argument("Invalid colors format: " + colorsStr);

        // Validate all bands except the last
        for (size_t idx = 0; idx < colorsFound.size() - 1; idx++)
        {
            isValid = false;
            for (const std::string &validColor : validColors)
                if (colorsFound.at(idx) == validColor) { isValid = true; break; }

            if (!isValid)
                throw std::invalid_argument("Invalid colors format: " + colorsStr);
        }

        return colorsFound;
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument("Invalid colors format: " + colorsStr);
    }
}

/*
  Parses a tolerance string and validates it against the standard
  resistor tolerance values. Accepts an optional trailing percent sign.
  Uses epsilon comparison to avoid floating-point precision issues.
*/
double parseTolerance(const std::string &toleranceStr)
{
    try
    {
        const double validTolerances[] = {0.05, 0.1, 0.25, 0.5, 1.0, 2.0, 5.0, 10.0};

        // Remove optional percent sign
        std::string modifiableStr = toleranceStr;
        size_t idx = modifiableStr.find('%');
        if (idx != std::string::npos)
            modifiableStr.replace(idx, 1, "");

        double tolerance = std::stod(modifiableStr);

        // Validate against standard tolerance values
        bool isValid = false;
        for (double v : validTolerances)
            if (std::abs(tolerance - v) < 0.001) { isValid = true; break; }

        if (!isValid)
            throw std::invalid_argument("Invalid tolerance value: " + toleranceStr);

        return tolerance;
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument("Invalid tolerance format: " + toleranceStr);
    }
}

/*
  Parses a resistor value string and converts engineering notation into
  a numeric double. Supports standard suffixes:
    R or r → ×1      (e.g., 4R7 = 4.7Ω)
    K or k → ×1000   (e.g., 2K2 = 2200Ω)
    M or m → ×1000000
  Also supports values with the suffix replacing the decimal point (e.g., 2k2).
*/
double parseValue(const std::string &valueStr)
{
    try
    {
        std::string modifiableStr = valueStr;
        size_t idx;

        // Parse the numeric portion up to the first non-numeric character
        double value = std::stod(modifiableStr, &idx);
        double multiplier = 1.0;

        if (idx != modifiableStr.length())
        {
            char suffix = std::tolower(modifiableStr[idx]);
            switch (suffix)
            {
                case 'r': multiplier = 1.0;   break;
                case 'k': multiplier = 1e3;   break;
                case 'm': multiplier = 1e6;   break;
                default:
                    throw std::invalid_argument("Invalid suffix: " + std::string(1, suffix));
            }

            // If digits follow the suffix (e.g., 2k2), replace suffix with decimal point
            if (idx + 1 < modifiableStr.length())
            {
                modifiableStr.replace(idx, 1, ".");
                value = std::stod(modifiableStr, &idx);
            }
        }

        return value * multiplier;
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument("Invalid value format: " + valueStr);
    }
}

#endif // RESISTOR_H
