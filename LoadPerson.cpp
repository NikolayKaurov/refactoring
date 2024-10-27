#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

using namespace std;

enum class DBLogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

struct ConnectionSettings {
    string_view name;
    int timeout = 0;
    bool allow_exceptions = false;
    DBLogLevel log_level = DBLogLevel::DEBUG;

    ConnectionSettings& SetName(string_view db_name) {
        name = db_name;
        return *this;
    }

    ConnectionSettings& SetTimeout(int db_timeout) {
        timeout = db_timeout;
        return *this;
    }

    ConnectionSettings& SetAllowExceptions(bool db_allow_exceptions) {
        allow_exceptions = db_allow_exceptions;
        return *this;
    }

    ConnectionSettings& SetLogLevel(DBLogLevel db_log_level) {
        log_level = db_log_level;
        return *this;
    }
};

struct Range {
    int min;
    int max;
};

struct Person {
    Person(string name, int age)
        : name_(std::move(name))
        , age_(age) {
    }

    string name_;
    int age_;
};

class DBQuery {
public:
    explicit DBQuery(string query)
        : query_(std::move(query)) {
    }

private:
    string query_;
};

class DBHandler {
public:
    bool IsOK() {
        return true;
    }

    string Quote(string_view filter) {
        return {};
    }

    template<typename key, typename value>
    vector<pair<key, value>> LoadRows(const DBQuery& query) {
        return {{"Name"s, 18}};
    }

};

class DBConnector {
public:
    DBConnector(bool db_allow_exceptions, DBLogLevel db_log_level)
        : db_allow_exceptions_(db_allow_exceptions)
        , db_log_level_(db_log_level) {
    }

    DBHandler ConnectTmp(string_view db_name, int timeout) {
        return {};
    }

    DBHandler Connect(string_view db_name, int timeout) {
        return {};
    }

private:
    bool db_allow_exceptions_;
    DBLogLevel db_log_level_;
};

vector<Person> LoadPersons(ConnectionSettings settings, Range range, string_view name_filter) {
    auto [db_name, db_connection_timeout, db_allow_exceptions, db_log_level] = settings;
    auto [min_age, max_age] = range;

    DBConnector connector(db_allow_exceptions, db_log_level);

    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    } else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age]: db.LoadRows<string, int>(query)) {
        persons.emplace_back(std::move(name), age);
    }

    return persons;
}

int main() {
    auto persons = LoadPersons(
        ConnectionSettings()
            .SetName("Name"sv)
            .SetAllowExceptions(false)
            .SetLogLevel(DBLogLevel::DEBUG)
            .SetTimeout(1000),
        Range{18, 45},
        "filter"sv);

    cout << persons.at(0).name_ << ' ' << persons.at(0).age_ << endl;
}