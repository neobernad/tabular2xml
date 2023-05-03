/*
 * IError.hpp
 *
 *  Created on: 17 may. 2018
 *      Author: Neo
 */

#ifndef INTERFACES_IERROR_HPP_
#define INTERFACES_IERROR_HPP_

#include<string>

class IError {
public:
  // Empty virtual destructor for proper cleanup
  virtual ~IError() {}

  virtual void error(const std::string& what) = 0;
};



#endif /* INTERFACES_IERROR_HPP_ */
