/*
 * Option.cpp
 *
 *  Created on: 16 may. 2018
 *      Author: Neo
 */

#include "Option.hpp"
#include <iostream>

Option::Option() {}

Option::Option(const std::string& key) { setKey(key); }

Option::~Option() {}

const std::string& Option::getKey() const {
	return m_key;
}

void Option::setKey(const std::string& key) {
	m_key = key;
}

const std::vector<std::string>& Option::getValues() const {
	return m_vValues;
}


void Option::addValue(const std::string& value) {
	m_vValues.push_back(value);
}

void Option::print() {
	for(std::string  str : m_vValues) {
		std::cout << str << std::endl;
	}
}

void Option::copyVector(const std::vector<std::string>& valuesVector) {
	m_vValues = valuesVector;
}

const unsigned int Option::getValuesSize() const {
	return m_vValues.size();
}
