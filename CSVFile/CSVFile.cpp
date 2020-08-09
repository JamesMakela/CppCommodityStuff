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
#include <iomanip>

#include "CmdOptionParser.hpp"
#include "CSVFile.h"


int main(int argc, const char *argv[])
{
    CmdOptionParser options(argc, argv);

    const std::string &filePath = options.getCmdOption("-f");

    if (filePath.empty()) {
        // could not find the file
        std::string cmd(argv[0]);

        std::cout << "file not specified!\n"
                  << "Usage: "
                  << cmd.substr(cmd.rfind("/") + 1)
                  << " -f <filename>\n";

        return 1;
    }

    std::cout << "opening CSVFile: " << filePath << "\n";
    try {
        CSVFile csvFile{filePath};

        std::cout << csvFile << "\n";  // check that we can print it out

        // normal index into the rows
        std::cout << "\nRow 10: " << csvFile.getRow(10) << "\n";

        // last index into the rows
        std::cout << "\nRow 1496: " << csvFile.getRow(1496) << "\n";

        // negative index into the rows (should be same as last index)
        std::cout << "\nRow -1: " << csvFile.getRow(-1) << "\n";

        // normal index into the columns
        std::cout << "\nColumn 1: " << csvFile.getColumn(1) << "\n";

        // last index into the columns
        std::cout << "\nColumn 158: " << csvFile.getColumn(158) << "\n";

        // negative index into the columns (should be same as last index)
        std::cout << "\nColumn -1: " << csvFile.getColumn(-1) << "\n";

        // Cell value
        std::cout << "\nCell (0, 0): ";
        std::visit(CellPrint{std::cout}, csvFile.getCell(0, 0));
        std::cout << "\n";

        // Cell value
        std::cout << "\nCell (1, 1): ";
        std::visit(CellPrint{std::cout}, csvFile.getCell(1, 1));
        std::cout << "\n";

        // negative Cell value
        std::cout << "\nCell (-1, -1): ";
        std::visit(CellPrint{std::cout}, csvFile.getCell(-1, -1));
        std::cout << "\n";

        try {
            // row index too big
            std::cout << "\nRow 1497: " << csvFile.getRow(1497) << "\n";
        }
        catch (const IndexError &exc){
            std::cout << exc.getError() << "\n";
        }

        try {
            // negative row index too big
            std::cout << "\nRow -1498: " << csvFile.getRow(-1498) << "\n";
        }
        catch (const IndexError &exc){
            std::cout << exc.getError() << "\n";
        }

        try {
            // column index too big
            std::cout << "\nColumn 159: " << csvFile.getColumn(159) << "\n";
        }
        catch (const IndexError &exc){
            std::cout << exc.getError() << "\n";
        }

        try {
            // negative column index too big
            std::cout << "\nColumn -160: " << csvFile.getColumn(-160) << "\n";
        }
        catch (const IndexError &exc){
            std::cout << exc.getError() << "\n";
        }

        try {
            // Cell row too big
            std::cout << "\nCell (1497, 1): ";
            std::visit(CellPrint{std::cout}, csvFile.getCell(1497, 1));
            std::cout << "\n";
        }
        catch (const IndexError &exc){
            std::cout << exc.getError() << "\n";
        }

        try {
            // Cell column too big
            std::cout << "\nCell (1, 159): ";
            std::visit(CellPrint{std::cout}, csvFile.getCell(1, 159));
            std::cout << "\n";
        }
        catch (const IndexError &exc){
            std::cout << exc.getError() << "\n";
        }

        try {
            // negative Cell row too big
            std::cout << "\nCell (-1498, 1): ";
            std::visit(CellPrint{std::cout}, csvFile.getCell(-1498, 1));
            std::cout << "\n";
        }
        catch (const IndexError &exc){
            std::cout << exc.getError() << "\n";
        }

        try {
            // negative Cell column too big
            std::cout << "\nCell (1, -160): ";
            std::visit(CellPrint{std::cout}, csvFile.getCell(1, -160));
            std::cout << "\n";
        }
        catch (const IndexError &exc){
            std::cout << exc.getError() << "\n";
        }


    }
    catch (const FileError &exc) {
        std::cout << exc.getError() << "\n";
        exit(1);
    }

    std::cout << "finished!...\n";

    return 0;
}




