#include "box.hpp"

#include "hierarchy.hpp"

#include <algorithm>
#include <sstream>

Box Box::operator+( const Box& other ) const {
    return Box{
        { std::min( _p1.x, other._p1.x ), std::min( _p1.y, other._p1.y ), std::min( _p1.z, other._p1.z ) },
        { std::max( _p2.x, other._p2.x ), std::max( _p2.y, other._p2.y ), std::max( _p2.z, other._p1.z ) },
    };
}

void Box::operator+=( const Box& other ) {
    _p1 = { std::min( _p1.x, other._p1.x ), std::min( _p1.y, other._p1.y ), std::min( _p1.z, other._p1.z ) };
    _p2 = { std::max( _p2.x, other._p2.x ), std::max( _p2.y, other._p2.y ), std::max( _p2.z, other._p2.z ) };
}

Box operator*( const Transform& transform, const Box& box ) {
    // transform all corners
    auto c0 = transform.local_matrix() * glm::vec4{box._p1.x, box._p1.y, box._p1.z, 1.0};
    auto c1 = transform.local_matrix() * glm::vec4{box._p1.x, box._p1.y, box._p2.z, 1.0};
    auto c2 = transform.local_matrix() * glm::vec4{box._p1.x, box._p2.y, box._p1.z, 1.0};
    auto c3 = transform.local_matrix() * glm::vec4{box._p1.x, box._p2.y, box._p2.z, 1.0};
    auto c4 = transform.local_matrix() * glm::vec4{box._p2.x, box._p1.y, box._p1.z, 1.0};
    auto c5 = transform.local_matrix() * glm::vec4{box._p2.x, box._p1.y, box._p2.z, 1.0};
    auto c6 = transform.local_matrix() * glm::vec4{box._p2.x, box._p2.y, box._p1.z, 1.0};
    auto c7 = transform.local_matrix() * glm::vec4{box._p2.x, box._p2.y, box._p2.z, 1.0};
    glm::vec3 new_p1 = {
        std::min( { c0.x, c1.x, c2.x, c3.x, c4.x, c5.x, c6.x, c7.x } ),
        std::min( { c0.y, c1.y, c2.y, c3.y, c4.y, c5.y, c6.y, c7.y } ),
        std::min( { c0.z, c1.z, c2.z, c3.z, c4.z, c5.z, c6.z, c7.z } )
    };
    glm::vec3 new_p2 = {
        std::max( { c0.x, c1.x, c2.x, c3.x, c4.x, c5.x, c6.x, c7.x } ),
        std::max( { c0.y, c1.y, c2.y, c3.y, c4.y, c5.y, c6.y, c7.y } ),
        std::max( { c0.z, c1.z, c2.z, c3.z, c4.z, c5.z, c6.z, c7.z } )
    };

    return Box{ new_p1, new_p2 };
}

std::string Box::to_string() {
    std::stringstream strs;
    strs << "x: " << _p1.x << ":" << _p2.x
       << " y: " << _p1.y << ":" << _p2.y
       << " z: " << _p1.z << ":" << _p2.z;
    return strs.str();
}
