/*
 * Factory.hpp
 *
 *  Created on: 18 may. 2018
 *      Author: Neo
 */

#ifndef FACTORY_FACTORY_HPP_
#define FACTORY_FACTORY_HPP_

#include "../ArgumentParser/ArgumentParser.hpp"
#include "../Interfaces/IConverter.hpp"

class Factory {
private:
	Factory();
	virtual ~Factory();
public:
	static Factory& getInstance();
	Factory(Factory const&) = delete;
	void operator=(Factory const&) = delete;
	IConverter* createConverter(Input::Type format);
};

#endif /* FACTORY_FACTORY_HPP_ */
