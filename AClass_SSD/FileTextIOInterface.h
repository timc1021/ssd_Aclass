#pragma once
#include <string>

class FileTextIOInterface {
 protected:
  std::string fileName;

 public:
  FileTextIOInterface(std::string fileName) : fileName(fileName) {};

  virtual std::string loadFromFile() = 0;
  virtual void saveToFile(std::string data) const = 0;
};