#ifndef _REPLICATOR_TEXTURE_H_
#define _REPLICATOR_TEXTURE_H_

#include "glad/glad.h"

#include "entt/entt.hpp"

#include <string>

class Texture {
    public:
        Texture( const std::string& path );
        ~Texture();
        operator GLuint () const { return _texture_id; }
    private:
        int _width, _height, _channels;
        GLuint _texture_id;
};

class TextureLoader final: public entt::resource_loader<TextureLoader, Texture> {
    public:
        std::shared_ptr<Texture> load( const std::string& texture_path ) const;
};

class TextureException : public std::exception {
    public:
        TextureException( const std::string& msg ) : _msg{msg} { }
        const char* what() const throw() { return _msg.c_str(); }

    private:
        const std::string _msg;
};

#endif // _REPLICATOR_TEXTURE_H_
