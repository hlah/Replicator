#ifndef _REPLICATOR_ACTION_HPP_
#define _REPLICATOR_ACTION_HPP_

#include <string>

typedef size_t ActionId;

class ActionEvent {
    public:
        // Action type
        enum class Type {
            ON,
            OFF,
            NONE,
        };

        ActionEvent( Type type, std::string name ) :
            _name{name},
            _type{type} {}

        // Return the action type
        Type type() const { return _type; };
        // The action name
        const std::string& name() const { return _name; };
    private:
        std::string _name;
        Type _type;
};

#endif // _REPLICATOR_ACTION_HPP_
