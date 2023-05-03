/*
 * IConverter.h
 *
 *  Created on: 14 may. 2018
 *      Author: Neo
 */

#ifndef INTERFACES_ICONVERTER_HPP_
#define INTERFACES_ICONVERTER_HPP_

#include <string>
#include <fstream>
#include <libxml/tree.h>

class IConverter {
public:
  // Empty virtual destructor for proper cleanup
  virtual ~IConverter() {}

  virtual void load(const std::string& inputPath) = 0;
  virtual void convertToXML() = 0;
  virtual void save(const std::string& outputPath) = 0;
  virtual xmlDocPtr getXMLOutputDoc() const = 0;
};



#endif /* INTERFACES_ICONVERTER_HPP_ */
