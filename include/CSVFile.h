//============================================================================
// Name        : CSVFile.h
// Author      : James L. Makela
// Version     : 0.0.1
// Copyright   : LGPL v3.0
// Description : Simple start to parsing content in a CSV file
//               (comma separated values)
//============================================================================

#ifndef __CSVFILE_H__
#define __CSVFILE_H__

#include <variant>
#include <vector>
#include <string>

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

	~CSVRow() {
		// std::cerr << "CSVRow cleaned up\n";
	}

    CSVRow(std::string &strRow);

    Cell getField(std::string &field);

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

    virtual std::ostream& print(std::ostream& out) const;
};


class CSVColumn : public CSVRow
{
private:
public:
    CSVColumn(const std::vector<CSVRow>& rows, int index);

	~CSVColumn() {
		// std::cerr << "CSVColumn cleaned up\n";
	}

    int max_length(const std::vector<CSVRow> &rows);

	friend std::ostream& operator<<(std::ostream &out, const CSVColumn &col) {
		return col.print(out);
	}

    virtual std::ostream& print(std::ostream& out) const;
};


class CSVFile
{
private:
    std::vector<CSVRow> m_rows{};
public:
    CSVFile(std::string filePath);

	~CSVFile() {
		std::cerr << "CSVFile cleaned up\n";
	}

    CSVRow getRow(int index);
    CSVColumn getColumn(int index);
    Cell getCell(int row, int column);

	friend std::ostream& operator<<(std::ostream &out, const CSVFile &csvFile) {
		return csvFile.print(out);
	}

    virtual std::ostream& print(std::ostream& out) const;
};


#endif // __CSVFILE_H__
