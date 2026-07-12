#pragma once

class Entity {

    int id;

public:
    virtual ~Entity() = default;
    explicit Entity(int id);
    int getId() const;

    void setId(int id);
};