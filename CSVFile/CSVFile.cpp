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


class Exception
{
protected:
    std::string m_error;
public:
    Exception(std::string error) : m_error(error) {}

    const char* getError() const { return m_error.c_str(); }
};


class IndexError : public Exception
{
public:
    IndexError(std::string error) : Exception(error) {}
};


class FileError : public Exception
{
public:
    FileError(std::string error) : Exception(error) {}
};


// We are trying out variants to solve the problem of arbitrary data types
// coming from CSV fields.
using Cell = std::variant<double, std::string>;

struct CellPrint
{
    std::ostream& m_out;

    CellPrint(std::ostream& out)
        : m_out{out}
    {}

    void operator()(int i) const { 
        m_out << i; 
    }
    void operator()(long l) const { 
        m_out << l; 
    }
    void operator()(double f) const { 
        m_out << f; 
    }
    void operator()(const std::string& s) const { 
        m_out << s; 
    }
};


class CSVRow
{
private:
protected:
    std::vector<Cell> m_fields{};
public:
    CSVRow() {
        // std::cout << "CSVRow()...\n";
    }

    CSVRow(std::string &strRow) {
        if (strRow.length() == 0) return;

        std::stringstream ssRow{strRow};
        std::string field;

        while(std::getline(ssRow, field, '\t')) {
            m_fields.push_back(getField(field));
        }
    }

	~CSVRow() {
		// std::cerr << "CSVRow cleaned up\n";
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

    int size() const {
        return m_fields.size();
    }

    Cell& operator[] (int index) {
        return m_fields[index];
    }

    const Cell& operator[] (int index) const {
        return m_fields[index];
    }

	friend std::ostream& operator<<(std::ostream &out, const CSVRow &row) {
		return row.print(out);
	}

    virtual std::ostream& print(std::ostream& out) const {
        std::vector<Cell>::const_iterator it;
        it = m_fields.cbegin();

		out << "(";
        while (it + 1 < m_fields.cend())  // stop just 1 before the end
        {
            std::visit(CellPrint{out}, *it);
            out << ", ";
            ++it;
        }
        std::visit(CellPrint{out}, *it);
        out << ")";

		return out;
	}
};

class CSVColumn : public CSVRow
{
private:
public:
    CSVColumn(const std::vector<CSVRow>& rows, int index) {
        int max_len{max_length(rows)};

        if (index < 0) {
            index = max_len + index;
        }

        if (index >= max_len) {
            throw IndexError{"IndexError: column number too big!"};
        }
        else if (index < 0) {
            throw IndexError{"IndexError: column number too small!"};
        }

        std::vector<CSVRow>::const_iterator it;
        it = rows.cbegin();

        while (it < rows.cend()) {
            if (index >= (*it).size()) {
                m_fields.push_back("");
            }
            else {
                m_fields.push_back((*it)[index]);
            }
            ++it;
        }
    }

	~CSVColumn() {
		// std::cerr << "CSVColumn cleaned up\n";
	}

    int max_length(const std::vector<CSVRow> &rows) {
        int max_len = 0;
        std::vector<CSVRow>::const_iterator it;
        it = rows.cbegin();

        while (it < rows.cend()) {
            if (max_len < (*it).size()) {
                max_len = (*it).size();
            }
            ++it;
        }

        return max_len;
    }

	friend std::ostream& operator<<(std::ostream &out, const CSVColumn &col) {
		return col.print(out);
	}

    virtual std::ostream& print(std::ostream& out) const {
        std::vector<Cell>::const_iterator it;
        it = m_fields.cbegin();

		out << "(";
        while (it + 1 < m_fields.cend())  // stop just 1 before the end
        {
            std::visit(CellPrint{out}, *it);
            out << ", ";
            ++it;
        }
        std::visit(CellPrint{out}, *it);
        out << ")";

		return out;
	}
};

class CSVFile
{
private:
    std::vector<CSVRow> m_rows{};
public:
    CSVFile(std::string filePath) {
        std::ifstream inFile{filePath};

        if (!inFile) {
            throw FileError{"FileException: Could not open file for reading!"};
        }

        for(int count=0; inFile;) {
            std::string strLine;
            std::getline(inFile, strLine);

            if (strLine.length() > 0) {
                m_rows.push_back(strLine);
            }
        }
    }

	~CSVFile() {
		std::cerr << "CSVFile cleaned up\n";
	}

    CSVRow getRow(int index) {
        if (index < 0) {
            index = m_rows.size() + index;
        }

        if (index >= static_cast<int>(m_rows.size())) {
            throw IndexError{"IndexError: row number too big!"};
        }
        else if (index < 0) {
            throw IndexError{"IndexError: row number too small!"};
        }

        return m_rows[index];
    }

    CSVColumn getColumn(int index) {
        return CSVColumn(m_rows, index);
    }

    Cell getCell(int row, int column) {
        CSVRow csvRow{getRow(row)};

        if (column < 0) {
            column = csvRow.size() + column;
        }

        if (column >= static_cast<int>(csvRow.size())) {
            throw IndexError{"IndexError: column number too big!"};
        }
        else if (column < 0) {
            throw IndexError{"IndexError: column number too small!"};
        }

        return csvRow[column];
    }

	friend std::ostream& operator<<(std::ostream &out, const CSVFile &csvFile) {
		return csvFile.print(out);
	}

    virtual std::ostream& print(std::ostream& out) const {
        std::vector<CSVRow>::const_iterator it;
        it = m_rows.cbegin();

		out << "(";
        while (it + 1 < m_rows.cend()) {  // stop just 1 before the end
            out << *it++ << ", ";
        }
        out << *it << ")";

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




