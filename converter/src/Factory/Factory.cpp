/*
 * Factory.cpp
 *
 *  Created on: 18 may. 2018
 *      Author: Neo
 */

#include "Factory.hpp"
#include "../Converter/Converter.hpp"

Factory::Factory() {}

Factory::~Factory() {}

Factory& Factory::getInstance() {
	static Factory instance;
	return instance;
}

IConverter* Factory::createConverter(Input::Type format) {
	/*
	 * I could simply return "return new Converter(Input::DELIMETER_LABEL[format]);"
	 * But I leave it this way just in case in the future we need some kind of
	 * special actions depending on the input (e.g preprocessing)
	 */
	switch (format) {
		case Input::Type::TSV:
			return new Converter(Input::DELIMETER_LABEL[Input::Type::TSV]);
			break;
		case Input::Type::CSV:
			return new Converter(Input::DELIMETER_LABEL[Input::Type::CSV]);
			break;
		default:
			return nullptr;
			break;
	}
}
