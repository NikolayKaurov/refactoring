#include <array>
#include <stdexcept>
#include <string>

using namespace std;

struct DateTime {
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
};

enum class DateTimeValidity {
    OK,
    YEAR_SMALL,
    YEAR_BIG,
    MONTH_SMALL,
    MONTH_BIG,
    DAY_SMALL,
    DAY_BIG,
    HOUR_SMALL,
    HOUR_BIG,
    MINUTE_SMALL,
    MINUTE_BIG,
    SECOND_SMALL,
    SECOND_BIG
};

DateTimeValidity CheckDateTimeValidity(const DateTime& dt) {
    if (dt.year < 1) {
        return DateTimeValidity::YEAR_SMALL;
    }
    if (dt.year > 9999) {
        return DateTimeValidity::YEAR_BIG;
    }

    if (dt.month < 1) {
        return DateTimeValidity::MONTH_SMALL;
    }
    if (dt.month > 12) {
        return DateTimeValidity::MONTH_BIG;
    }

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dt.day < 1) {
        return DateTimeValidity::DAY_SMALL;
    }
    if (dt.day > month_lengths[dt.month - 1]) {
        return DateTimeValidity::DAY_BIG;
    }

    if (dt.hour < 0) {
        return DateTimeValidity::HOUR_SMALL;
    }
    if (dt.hour > 23) {
        return DateTimeValidity::HOUR_BIG;
    }

    if (dt.minute < 0) {
        return DateTimeValidity::MINUTE_SMALL;
    }
    if (dt.minute > 59) {
        return DateTimeValidity::MINUTE_BIG;
    }

    if (dt.second < 0) {
        return DateTimeValidity::SECOND_SMALL;
    }
    if (dt.second > 59) {
        return DateTimeValidity::SECOND_BIG;
    }

    return DateTimeValidity::OK;
}

int main() {
    DateTimeValidity v = CheckDateTimeValidity({});
}