#pragma once
#include <vector>
#include <string>

template <typename T, typename Id = int>
class Repository {

public:

    virtual ~Repository() = default;

    virtual std::vector<T> findAll() const = 0;
    virtual T findById(Id id) const = 0;
    virtual int insert(const T& entidad) = 0;
    virtual bool update(const T& entidad) = 0;
    virtual bool remove(Id id) = 0;
};

template <typename T>
class Repository<T, std::string> {
public:
    virtual ~Repository() = default;
    virtual std::vector<T> findAll() const = 0;
    virtual T findById(const std::string& id) const = 0;
    virtual T insert(const T& entidad) = 0;
    virtual bool update(const T& entidad) = 0;
    virtual bool remove(const std::string& id) = 0;
};