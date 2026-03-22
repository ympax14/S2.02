#ifndef PATH_HPP
#define PATH_HPP

#include "../City/City.hpp"

class Path
{
private:
    const City* const from, *const to;
    const int weight;
public:
    Path(const City* const &_from, const City* const &_to, const int& _weight = 0);
    ~Path();

    inline const City* getFrom() const { return this->from; };
    inline const City* getTo() const { return this->to; };
    inline const int& getWeight() const { return this->weight; };
};

#endif // PATH_HPP
