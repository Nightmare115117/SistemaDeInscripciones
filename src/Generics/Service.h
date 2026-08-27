#pragma once
#include <vector>
#include <string>

template <typename T, typename Repository, typename Id = int>
class Service {

protected:

    Repository& repo;

public:
    explicit Service(Repository& repo) : repo(repo) {}
    virtual ~Service() = default;

    virtual std::vector<T> findAll() const = 0;
    virtual T findById(Id id) const = 0;
    virtual int insert(const T& entidad) = 0;
    virtual bool update(const T& entidad) = 0;
    virtual bool remove(Id id) = 0;

    virtual bool validate(const T& entidad) = 0;
};

template <typename T, typename Repository>
class Service<T, Repository, std::string> {
protected:
    Repository& repo;
public:
    explicit Service(Repository& repository) : repo(repository) {}
    virtual ~Service() = default;
    virtual std::vector<T> findAll() const = 0;
    virtual T findById(const std::string& id) const = 0;
    virtual T insert(const T& entidad) = 0;
    virtual bool update(const T& entidad) = 0;
    virtual bool remove(const std::string& id) = 0;
};