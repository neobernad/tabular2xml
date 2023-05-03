/*
 * Option.h
 *
 *  Created on: 16 may. 2018
 *      Author: Neo
 */

#ifndef CONFIGURATOR_OPTION_OPTION_HPP_
#define CONFIGURATOR_OPTION_OPTION_HPP_

#include <string>
#include <vector>

class Option {
private:
	std::string m_key;
	std::vector < std::string > m_vValues;
public:
	Option();
	Option(const std::string& key);
	virtual ~Option();
	const std::string& getKey() const;
	void setKey(const std::string& key);
	const std::vector<std::string>& getValues() const;
	const unsigned int getValuesSize() const;
	void addValue(const std::string& value);
	void copyVector(const std::vector < std::string >& valuesVector);
	void print();
};

#endif /* CONFIGURATOR_OPTION_OPTION_HPP_ */
