#pragma once

template <typename Id = int>
class Entity {

    Id id;

public:
    virtual ~Entity() = default;
    explicit Entity(Id id = Id{}) : id(id) {}
    const Id& getId() const { return id; }

    void setId(const Id& value) { id = value; }
};