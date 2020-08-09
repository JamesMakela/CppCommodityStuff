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

#include "CSVFile.h"


CSVRow::CSVRow(std::string &strRow) {
    if (strRow.length() == 0) return;

    std::stringstream ssRow{strRow};
    std::string field;

    while(std::getline(ssRow, field, '\t')) {
        m_fields.push_back(getField(field));
    }
}

Cell CSVRow::getField(std::string &field) {
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

std::ostream& CSVRow::print(std::ostream& out) const {
    std::vector<Cell>::const_iterator it;
    it = this->m_fields.cbegin();

    out << "(";
    while (it + 1 < this->m_fields.cend())  // stop just 1 before the end
    {
        std::visit(CellPrint{out}, *it);
        out << ", ";
        ++it;
    }
    std::visit(CellPrint{out}, *it);
    out << ")";

    return out;
}





CSVColumn::CSVColumn(const std::vector<CSVRow>& rows, int index) {
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

int CSVColumn::max_length(const std::vector<CSVRow> &rows) {
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

std::ostream& CSVColumn::print(std::ostream& out) const {
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





CSVFile::CSVFile(std::string filePath) {
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

CSVRow CSVFile::getRow(int index) {
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

CSVColumn CSVFile::getColumn(int index) {
    return CSVColumn(m_rows, index);
}

Cell CSVFile::getCell(int row, int column) {
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

std::ostream& CSVFile::print(std::ostream& out) const {
    std::vector<CSVRow>::const_iterator it;
    it = m_rows.cbegin();

    out << "(";
    while (it + 1 < m_rows.cend()) {  // stop just 1 before the end
        out << *it++ << ", ";
    }
    out << *it << ")";

    return out;
}
