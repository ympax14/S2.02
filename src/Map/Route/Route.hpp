#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "../Path/Path.hpp"

#include <vector>

class Route
{
private:
    std::vector<Path> segments;
public:
    Route();
    ~Route();

    inline const std::vector<Path>& getSegments() const { return this->segments; }

    void addSegment(const Path& p);
    int getTotalWeight() const;
};

#endif // ROUTE_HPP
