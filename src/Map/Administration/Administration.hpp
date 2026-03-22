#ifndef ADMINISTRATION_HPP
#define ADMINISTRATION_HPP

#include <map>

#include <QString>

class City;
class Country;

class Administration
{
private:
    const QString name, name_ascii, code;
    const Country* const country;

    std::map<QString, City*> cities;
public:
    Administration(const QString& _name, const QString& _name_ascii, const QString& _code, const Country* _country = nullptr);
    ~Administration();

    const City* getCity(const QString& name) const;
    void addCity(City* const city);

    inline const QString& getName() const { return this->name; };
    inline const QString& getNameAscii() const { return this->name_ascii; };
    inline const QString& getCode() const { return this->code; };
    inline const Country* getCountry() const { return this->country; };
    inline const std::map<QString, City*>& getCities() const { return cities; };
};

#endif // ADMINISTRATION_HPP
