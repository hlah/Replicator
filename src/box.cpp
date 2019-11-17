#include "box.hpp"

#include "hierarchy.hpp"

#include <algorithm>
#include <sstream>

Box Box::operator+( const Box& other ) const {
    return Box{
        { std::min( _p1.x, other._p1.x ), std::min( _p1.y, other._p1.y ), std::min( _p1.z, other._p1.z ) },
        { std::max( _p2.x, other._p2.x ), std::max( _p2.y, other._p2.y ), std::max( _p2.z, other._p2.z ) },
    };
}

void Box::operator+=( const Box& other ) {
    _p1 = { std::min( _p1.x, other._p1.x ), std::min( _p1.y, other._p1.y ), std::min( _p1.z, other._p1.z ) };
    _p2 = { std::max( _p2.x, other._p2.x ), std::max( _p2.y, other._p2.y ), std::max( _p2.z, other._p2.z ) };
}

std::string Box::to_string() {
    std::stringstream strs;
    strs << "x: " << _p1.x << ":" << _p2.x
       << " y: " << _p1.y << ":" << _p2.y
       << " z: " << _p1.z << ":" << _p2.z;
    return strs.str();
}
