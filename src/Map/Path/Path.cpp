#include "Path.hpp"

Path::Path(const City* const &_from, const City* const &_to, const int& _weight) :
    from(_from),
    to(_to),
    weight(_weight)
{}

Path::~Path() = default;
