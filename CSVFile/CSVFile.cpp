//============================================================================
// Name        : CSVFile.cpp
// Author      : James L. Makela
// Version     : 0.0.1
// Copyright   : LGPL v3.0
// Description : Simple start to parsing content in a CSV file
//               (comma separated values)
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>
#include <string>

#include "CmdOptionParser.hpp"

// this is just to make printing stuff a bit more concise
// everything else in std, we can call explicitly
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

// We are trying out variants to solve the problem of arbitrary data types
// coming from CSV fields.  Nothing usable yet.
using Cell = std::variant<int, double, std::string>;


int main(int argc, const char *argv[])
{
    CmdOptionParser options(argc, argv);

    const std::string &filePath = options.getCmdOption("-f");

    if (filePath.empty()) {
        // could not find the file
        std::string cmd(argv[0]);

        cout << "file not specified!\n"
             << "Usage: "
             << cmd.substr(cmd.rfind("/") + 1)
             << " -f <filename>\n";

        return 1;
    }

    cout << "opening file: " << filePath << "\n";

    std::ifstream inFile{filePath};

    if (!inFile)
    {
        // Print an error and exit
        cerr << "Error, " << filePath << " could not be opened for reading!"
             << std::endl;
        return 1;
    }

    while (inFile) {
        std::string strLine;

        std::getline(inFile, strLine);
        std::stringstream ssLine{strLine};
        std::string field;

        while(std::getline(ssLine, field, '\t')) {
            std::stringstream ssField{field};

            double numField{};
            std::string strField;

            ssField >> numField;

            if (!ssField.fail()) {
                cout << numField << ", ";
            }
            else {
                ssField.clear();
                ssField.str(field);
                std::getline(ssField, strField, '\t');

                cout << strField << ", ";
            }
        }

        cout << "\n\n";
    }

    std::variant<int, float> v, w;

    v = 12; // v contains int

    // MacOS < 10.14 does not support std::get
    int i = std::get<int>(v);
    w = std::get<int>(v);
    w = std::get<0>(v); // same effect as the previous line

    w = v; // same effect as the previous line

    return 0;
}




