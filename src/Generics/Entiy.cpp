#include "Entity.h"

Entity::Entity(int id) : id(id) {}

int Entity::getId() const {
    return id;
}

void Entity::setId(int id) {
    this->id = id;
}