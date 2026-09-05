#pragma once
#include <string>
#include <vector>

template <typename T>

class Cache {
public:

  static bool exist(const std::string& key);
  static bool isValid(const std::string& key);
  static std::vector<T> get(const std::string& key);
  static void set(const std::string& key, const std::vector<T>& entity);

};