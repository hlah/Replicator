#ifndef _REPLICATOR_BOX_H_
#define _REPLICATOR_BOX_H_

#include "transform.hpp"

#include "glm/vec3.hpp"

#include <limits>
#include <string>


class Box {
    public:
        // reversed infinite box (identity element for box sum);
        Box() : _p1{std::numeric_limits<float>::infinity()}, _p2{-std::numeric_limits<float>::infinity()} {}
        // box with min corner at the origin and the other given
        Box(glm::vec3 p2) : _p1{0.0}, _p2{p2} {}
        // box with given corners
        Box(glm::vec3 p1, glm::vec3 p2) : _p1{p1}, _p2{p2} {}

        // get box width
        inline float width() const { return _p2.x - _p1.x; }
        inline float height() const { return _p2.y - _p1.y; }
        inline float length() const { return _p2.z - _p1.z; }

        // get corners
        inline const auto& min() const { return _p1; };
        inline const auto& max() const { return _p2; };

        // scale box
        inline void scale( float s )  { _p1 *= s; _p2 *= s; }

        // Box addition, returns the smallest box that containg both boxes
        Box operator+( const Box& other ) const;
        void operator+=( const Box& other );

        std::string to_string();

        static Box bounding_box( const entt::registry&, entt::entity entity );

    private:
        // min corner
        glm::vec3 _p1;
        // max corner
        glm::vec3 _p2;

};

Box operator*( const glm::mat4& transform, const Box& box ); 


#endif // _REPLICATOR_BOX_H_
