#include "geometry/box.hpp"

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

Box operator*( const glm::mat4& transform, const Box& box ) {
    auto c0 = transform * glm::vec4{ box.min().x, box.min().y, box.min().z, 1.0 };
    auto c1 = transform * glm::vec4{ box.min().x, box.min().y, box.max().z, 1.0 };
    auto c2 = transform * glm::vec4{ box.min().x, box.max().y, box.min().z, 1.0 };
    auto c3 = transform * glm::vec4{ box.min().x, box.max().y, box.max().z, 1.0 };
    auto c4 = transform * glm::vec4{ box.max().x, box.min().y, box.min().z, 1.0 };
    auto c5 = transform * glm::vec4{ box.max().x, box.min().y, box.max().z, 1.0 };
    auto c6 = transform * glm::vec4{ box.max().x, box.max().y, box.min().z, 1.0 };
    auto c7 = transform * glm::vec4{ box.max().x, box.max().y, box.max().z, 1.0 };

    auto min_x = std::min( { c0.x, c1.x, c2.x, c3.x, c4.x, c5.x, c6.x, c7.x } );
    auto min_y = std::min( { c0.y, c1.y, c2.y, c3.y, c4.y, c5.y, c6.y, c7.y } );
    auto min_z = std::min( { c0.z, c1.z, c2.z, c3.z, c4.z, c5.z, c6.z, c7.z } );

    auto max_x = std::max( { c0.x, c1.x, c2.x, c3.x, c4.x, c5.x, c6.x, c7.x } );
    auto max_y = std::max( { c0.y, c1.y, c2.y, c3.y, c4.y, c5.y, c6.y, c7.y } );
    auto max_z = std::max( { c0.z, c1.z, c2.z, c3.z, c4.z, c5.z, c6.z, c7.z } );

    return Box{ {min_x, min_y, min_z}, {max_x, max_y, max_z} };
}
