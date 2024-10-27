#include <map>
#include <string>
#include <vector>

using namespace std;

using Language = string;

class Json;

struct City {
    string name;
    string iso_code;
    string phone_code;
    string country_name;
    string country_iso_code;
    string country_time_zone;
    vector<Language> languages;
};

struct Country {
    string name;
    string iso_code;
    string phone_code;
    string country_name;
    string country_iso_code;
    string time_zone;
    vector<Language> languages;
};

using Object = map<string, Json>;

class Json {
public:
    [[nodiscard]] vector<Json> AsList() const {
        return {};
    }

    [[nodiscard]] Object AsObject() const {
        return {{"name"s,       {}},
                {"iso_code"s,   {}},
                {"phone_code"s, {}},
                {"time_zone"s,  {}},
                {"languages"s,  {}},
                {"cities"s,     {}}};
    }

    [[nodiscard]] string AsString() const {
        return {};
    }
};

template<typename value>
value FromString(const string& arg) {
    return {};
}

// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const Country& country) {
    for (const auto& city_json: json.AsList()) {
        const auto& city_obj = city_json.AsObject();

        cities.push_back({city_obj.at("name"s).AsString(),
                          city_obj.at("iso_code"s).AsString(),
                          country.phone_code + city_obj.at("phone_code"s).AsString(),
                          country.name,
                          country.iso_code,
                          country.time_zone,
                          country.languages});
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json: json.AsList()) {
        const auto& country_obj = country_json.AsObject();

        countries.push_back({
                                country_obj.at("name"s).AsString(),
                                country_obj.at("iso_code"s).AsString(),
                                country_obj.at("phone_code"s).AsString(),
                                country_obj.at("time_zone"s).AsString(),
                            });

        Country& country = countries.back();

        for (const auto& lang_obj: country_obj.at("languages"s).AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }

        ParseCitySubjson(cities, country_obj.at("cities"s), country);
    }
}

int main() {
    vector<Country> countries;
    vector<City> cities;

    ParseCountryJson(countries, cities, Json{});
}