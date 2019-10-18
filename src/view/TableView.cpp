#include <view/TableView.h>
#include <iostream>

void TableView::print()
{

}

// Display a query result as a table
void TableView::view(QueryResult *table)
{
    int col_len[table->get_column_names()->size()];
    int index = 0;
    int total_width = 0;

    // Print columns
    std::cout << "|   ";
    for (auto &col : *table->get_column_names()) {
        col_len[index] = 7 + col.size();

        for (int i = 0; i < table->row_count(); ++i) {
            int width = 7 + table->get_row(i)->at(index).size();
            if (width > col_len[index])
                col_len[index] = width;
        }

        total_width += col_len[index];

        if ((col.size() + 7) < col_len[index])
            std::cout << col << std::string((col_len[index] - 4) - col.size(), ' ') << "|   ";
        else
            std::cout << col << "   |   ";
        ++index;
    }

    std::cout << std::endl;
    total_width++;
    std::cout << std::string(total_width, '-') << std::endl;

    // Print rows
    std::cout << "|   ";
    for (auto &row : *table->get_rows()) {
        index = 0;
        for (auto &val : row) {
            if ((val.size() + 7) < col_len[index])
                std::cout << val << std::string((col_len[index] - 4) - val.size(), ' ') << "|   ";
            else
                std::cout << val << "   |   ";
            ++index;
        }
        std::cout << std::endl;
        std::cout << std::string(total_width, '-') << std::endl;
    }
}
