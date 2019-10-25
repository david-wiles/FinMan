#include <db/SQLite3QueryBuilder.h>
#include <db/SQLite3Instance.h>
#include <model/Account.h>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include "model/Transaction.h"


bool Transaction::create(const std::vector<std::string> &vals)
{
    if (vals.size() != 6)
        return false;

    // TODO validation

    auto query = new SQLite3QueryBuilder("transaction");
    query
    ->insert({"type", "amount", "from_acct", "to_acct", "date", "description"})
    ->values({vals});

    bool err = SQLite3Instance::getInstance()->query(query) == nullptr;
    delete query;

    if (!err) {

        std::time_t rawtime;
        std::tm tt{};

        std::time(&rawtime);
        strptime(vals.at(4).c_str(), "%Y-%m-%d %H:%M:%S", &tt);
        std::time_t transaction_time = mktime(&tt);

        // Update amounts in account if transaction date is before or equals current time
        if (difftime(transaction_time, rawtime) >= 0) {
            try {

                // Update from account
                if (!vals.at(2).empty()) {
                    auto from_query = new SQLite3QueryBuilder("account");
                    Account from(from_query->where(std::make_pair("acct_num", vals.at(2))));
                    auto obj = from.get();
                    int new_amt = stoi(obj->get_row(0)->at(3)) - stoi(vals.at(1));
                    from.update({std::make_pair("balance", std::to_string(new_amt))});
                    delete from_query;
                }

                // Update to account
                if (!vals.at(3).empty()) {
                    auto to_query = new SQLite3QueryBuilder("account");
                    Account to(to_query->where(std::make_pair("acct_num", vals.at(3))));
                    auto obj = to.get();
                    int new_amt = stoi(obj->get_row(0)->at(3)) + stoi(vals.at(1));
                    to.update({std::make_pair("balance", std::to_string(new_amt))});
                    delete to_query;
                }

            } catch (std::runtime_error &e) {
                return false;
            }
        }

    } else
        return false;

    return true;

}

void Transaction::del()
{
    // Not allowed
}
