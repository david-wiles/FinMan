#include <view/TableView.h>
#include <iostream>

void TableView::print()
{

}

void TableView::view(QueryResult *table)
{
    int col_len[table->get_column_names()->size()];
    int index = 0;
    int total_width = 0;

    // Print columns
    std::cout << "|   ";
    for (auto &col : *table->get_column_names()) {
        std::cout << col << "   |   ";
        int width = 7 + col.size();
        col_len[index] = width;
        total_width += width;
        ++index;
    }

    std::cout << std::endl;
    total_width += 4;
    std::cout << std::string(total_width, '_') << std::endl;

    // Print rows
    std::cout << "|   ";
    for (auto &row : *table->get_rows()) {
        index = 0;
        for (auto &val : row) {
            if (val.size() < col_len[index])
                std::cout << val << std::string(col_len[index] - val.size(), ' ') << "   |   ";
            else
                std::cout << val.substr(0, col_len[index] - 3) << ".." << " |   ";
            ++index;
        }
        std::cout << std::endl;
        std::cout << std::string(total_width, '_') << std::endl;
    }
}
