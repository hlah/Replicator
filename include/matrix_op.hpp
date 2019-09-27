#ifndef _REPLICATOR_MATRIX_HPP_
#define _REPLICATOR_MATRIX_HPP_

#include "glm/mat4x4.hpp"

// This header defines common matrix operations
namespace matrix_op {

    // matrix scale operation
    glm::mat4 scale( float sx, float sy, float sz );
    glm::mat4 scale( float s );

    // matrix translation operation
    glm::mat4 translation( float tx, float ty, float tz );


    // orthographic projection matrix
    glm::mat4 orthographic( 
            float left, 
            float right, 
            float bottom, 
            float top, 
            float far, 
            float near 
    );

    // perspective projection matrix
    glm::mat4 perspective( float fov, float aspect_ratio, float near, float far ); 

}


#endif // _REPLICATOR_MATRIX_HPP_
