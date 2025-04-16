#pragma once
#include <string>

class DataInterface {
 private:
  std::string fileName;

 public:
  DataInterface(std::string fileName) : fileName(fileName) {};

  virtual std::string loadFromFile() = 0;
  virtual void saveToFile(std::string data) const = 0;
};