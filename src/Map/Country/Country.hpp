#ifndef COUNTRY_HPP
#define COUNTRY_HPP

#include "../Administration/Administration.hpp"

#include <map>

#include <QString>

class Country
{
private:
    const QString name, iso2, iso3;

    std::map<QString, Administration*> administrations;
public:
    Country(const QString& _name, const QString& _iso2, const QString& _iso3);
    ~Country();

    inline const QString& getName() const { return this->name; };
    inline const QString& getIso2() const { return this->iso2; };
    inline const QString& getIso3() const { return this->iso3; };
    inline const std::map<QString, Administration*>& getAdministrations() const { return this->administrations; };

    Administration* getAdministration(const QString& adminName) const;
    void addAdministration(Administration* const administration);
};

#endif // COUNTRY_HPP
