#include "Route.hpp"

Route::Route() {}

Route::~Route() = default;

void Route::addSegment(const Path& p) {
    this->segments.push_back(p);
}

int Route::getTotalWeight() const {
    int total = 0;

    for (const Path& p : this->segments)
        total += p.getWeight();

    return total;
}