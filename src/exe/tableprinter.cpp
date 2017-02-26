#include "tableprinter.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>


TablePrinter::TablePrinter(const std::vector<std::size_t>& widths,
                           const std::vector<std::string>& names,
                           const std::vector<std::ios_base::fmtflags>& format,
                           const std::string& tableName):
    m_widths(widths),
    m_format(format)
{
    // Add missing formatting
    for(std::size_t k=format.size(); k<widths.size(); k++)
        m_format.push_back(std::ios::left | std::ios::dec);

    // Total width
    m_width = std::accumulate(m_widths.begin(), m_widths.end(), m_widths.size() + 1);

    if(tableName != "")
    {
        m_oss << std::string(m_width, '-') << "\n";
        m_oss << "|" << center(m_width-2, tableName) << "|\n";
        m_oss << "|" << std::string(m_width-2, '-') << "|\n";
    }

    if(names.size()>0)
    {
        // Separator line
        if(tableName=="")
            m_oss << std::string(m_width, '-') << "\n";

        // Names
        for(std::size_t k=0; k<m_widths.size(); k++)
        {
            if(k>=names.size())
                m_oss << "|" << std::string(m_widths[k], ' ');
            else
                m_oss << "|" << center(m_widths[k], names[k]);
        }
        m_oss << "|\n";


        // Separator line
        m_oss << "|" << std::string(m_width-2, '-') << "|\n";
    }

    // Separator line
    if(tableName == "" && names.size()==0)
        m_oss << std::string(m_width, '-') << "\n";
}

std::string TablePrinter::str() const
{
    return m_oss.str() + std::string(m_width, '-') + "\n";   ;
}


void TablePrinter::print() const
{
    std::cout << str();
}


std::string TablePrinter::center(size_t width, const std::string &str)
{
    std::size_t spaces = std::max(0, (int)width-(int)str.size());
    std::size_t w1 = spaces/2;
    std::size_t w2 = spaces-w1;

    return std::string(w1, ' ') + str + std::string(w2, ' ');
}

