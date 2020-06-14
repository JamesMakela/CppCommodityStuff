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

// We are trying out variants to solve the problem of arbitrary data types
// coming from CSV fields.  Nothing usable yet.
using Cell = std::variant<int, long, double, std::string>;

struct CellPrintVisitor1
{
    std::ostream& m_out;
    std::string m_sep{};

    CellPrintVisitor1(std::ostream& out, std::string sep=", ")
        : m_out{out}, m_sep{sep}
    {}

    void operator()(int i) const { 
        m_out << i << m_sep; 
    }
    void operator()(long l) const { 
        m_out << l << m_sep; 
    }
    void operator()(double f) const { 
        m_out << f << m_sep; 
    }
    void operator()(const std::string& s) const { 
        m_out << s << m_sep; 
    }
};


// helper constant for CellPrintVisitor2
template<class> inline constexpr bool always_false_v = false;

auto CellPrintVisitor2 = [](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, int>)
        std::cout << "int with value " << arg << '\n';
    else if constexpr (std::is_same_v<T, long>)
        std::cout << "long with value " << arg << '\n';
    else if constexpr (std::is_same_v<T, double>)
        std::cout << "double with value " << arg << '\n';
    else if constexpr (std::is_same_v<T, std::string>)
        std::cout << "std::string with value " << std::quoted(arg) << '\n';
    else 
        static_assert(always_false_v<T>, "non-exhaustive visitor!");
};


class CSVRow
{
private:
    std::vector<Cell> fields{};
public:
    CSVRow(std::string &strRow) {
        if (strRow.length() == 0) return;

        std::stringstream ssRow{strRow};
        std::string field;

        while(std::getline(ssRow, field, '\t')) {
            fields.push_back(getField(field));
        }
    }

    Cell getField(std::string &field) {
        double numField{};
        std::string strField{};

        std::stringstream ssField{field};
        ssField >> numField;

        if (ssField.fail()) {
            ssField.clear();
            ssField.str(field);
            std::getline(ssField, strField, '\t');
            //ssField >> strField;

            return strField;
        }
        else {
            return numField;
        }
    }

	friend std::ostream& operator<<(std::ostream &out, const CSVRow &row) {
		return row.print(out);
	}

    virtual std::ostream& print(std::ostream& out) const {
        std::vector<Cell>::const_iterator it;
        it = fields.cbegin();

		out << "(";
        while (it + 1 < fields.cend())  // stop just 1 before the end
        {
            std::visit(CellPrintVisitor1{out},
                       *it);
            ++it;
        }
        std::visit(CellPrintVisitor1{out, ")"}, *it);

		return out;
	}
};


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

    std::cout << "opening file: " << filePath << "\n";

    std::ifstream inFile{filePath};

    if (!inFile) {
        // Print an error and exit
        std::cerr << "Error, " << filePath
                  << " could not be opened for reading!"
                  << std::endl;
        return 1;
    }

    for(int count=0; inFile;) {
        std::string strLine;
        std::getline(inFile, strLine);

        if (strLine.length() > 0) {
            CSVRow csvRow{strLine};

            std::cout << ++count << ": " << csvRow << "\n";
        }

    }
    std::cout << "finished!...\n";

    return 0;
}




