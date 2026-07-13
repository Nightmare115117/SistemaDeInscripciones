#pragma once
#include <vector>

template <typename T>
class Service {

public:

    virtual ~Service() = default;

    virtual std::vector<T> findAll() const = 0;
    virtual T findById(int id) const = 0;
    virtual int insert(const T& entidad) = 0;
    virtual bool update(const T& entidad) = 0;
    virtual bool remove(int id) = 0;

    virtual bool validate(const T& entidad) = 0;
    virtual bool exist(int id) = 0;
};