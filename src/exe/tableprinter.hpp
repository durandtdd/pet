#ifndef TABLEPRINTER_HPP
#define TABLEPRINTER_HPP

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>


/**
 * @brief Helper class printing a table
 *
 * The table is made from column of different sizes defined in the constructor
 * It can include optionally column names and a table name
 *
 * Row are added with the function addRow()
 *
 * When all rows were added, the user can get the string with str()
 *
 * Integers are printed as hex values
 *
 *
 * Example:
 * TablePrinter tp({6, 5, 7}, {"Col 1", "Col 2", "Col 3"}, "Name")
 * tp.addRow("Row 1", 3.14, 5);
 * tp.addRow("Row 2", "N/A", 45);
 * tp.addRow("Row 3");
 * std::cout << tp.str() << std::endl;
 *
 * ----------------------
 * |        Name        |
 * |--------------------|
 * |Col 1 |Col 2| Col 3 |
 * |--------------------|
 * | Row 1| 3.14|      5|
 * | Row 2|  N/A|     2d|
 * | Row 3|     |       |
 * ----------------------
 */
class TablePrinter
{
    public:
        /**
         * @brief Construct a TablePrinter
         * @param widths Column widths
         * @param names Column names
         * @param tableName Table name
         * @param format Column formats
         */
        TablePrinter(const std::vector<std::size_t>& widths,
                     const std::vector<std::string>& names = std::vector<std::string>(),
                     const std::vector<std::ios::fmtflags>& format = std::vector<int>(),
                     const std::string& tableName = "");

        /**
         *  @brief Add a row to the table
         *  @param Values to print in the table
         */
        template<typename T, typename... Args>
        void addRow(T first, Args... others);

        /**
         * @brief Return a string with the printed table
         * @return String
         */
        std::string str() const;

        /**
         * @brief Print the table to stdout
         */
        void print() const;


    private:
        /**
         * @brief Add values in columns
         * @param Values
         */
        template<typename T, typename... Args>
        void addValue(std::size_t idx, T value, Args... others);

        template<typename T>
        void addValue(std::size_t idx, T value);

        /**
         * @brief Center a string
         * @param width Output string width
         * @param str String to center
         * @return Centered string
         */
        std::string center(std::size_t width, const std::string& str);

        /**
         * @brief Print formatted value to stream
         * @param idx Column index
         * @param value Value to print
         */
        template<typename T>
        void TablePrinter::addValueToStream(size_t idx, T value);


    private:
        /** Column widths */
        std::vector<std::size_t> m_widths;

        /** Total table width */
        std::size_t m_width;

        /** Columns format */
        std::vector<int> m_format;

        /** Stream to write the table */
        std::ostringstream m_oss;
};


/*============================================================================*/
template<typename T, typename... Args>
void TablePrinter::addRow(T first, Args... others)
{
    addValue(0, first, others...);
}


template<typename T, typename... Args>
void TablePrinter::addValue(std::size_t idx, T value, Args... others)
{
    // Add value if enough columns
    if(idx<m_widths.size())
    {
        addValueToStream(idx, value);

        // Recursive call
        addValue(idx+1, others...);
    }
}


template<typename T>
void TablePrinter::addValue(std::size_t idx, T value)
{
    // Current value (if enough columns)
    if(idx<m_widths.size())
        addValueToStream(idx, value);

    // Add blank columns for missing values
    idx++;
    for(; idx<m_widths.size(); idx++)
        m_oss << "|" << std::string(m_widths[idx], ' ');

    // Finish
    m_oss << "|\n";
}

template<typename T>
void TablePrinter::addValueToStream(size_t idx, T value)
{
    m_oss << "|";
    m_oss << std::setw(m_widths[idx]);
    m_oss.flags(m_format[idx]);
    m_oss << value;
}


#endif // TABLEPRINTER_HPP



