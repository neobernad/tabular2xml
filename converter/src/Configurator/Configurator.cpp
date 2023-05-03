/*
 * Configurator.cpp
 *
 *  Created on: 15 may. 2018
 *      Author: Neo
 */

#include "Configurator.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>



Configurator::Configurator() {
	m_tokenizer.setDelimiter(CONFIG_DELIMITER);
}

Configurator::~Configurator() {}

Configurator& Configurator::getInstance() {
	static Configurator instance; // Guaranteed to be destroyed.
	// Instantiated on first use.
	return instance;
}

void Configurator::readConfiguration(const std::string& filePath) {
	std::ifstream configFile(filePath);

	if (!configFile.is_open()) {
		std::cerr << "Could not load configuration file '" << filePath << "'" << std::endl;
		exit(1);
	}
	std::stringstream configStream;
	configStream << configFile.rdbuf();
	configFile.close();
	try {
		std::string line;
		unsigned int lineNumber = 0;
		while( std::getline(configStream >> std::ws, line)) { // >> std::ws read only filled lines, not empty ones
			avoidComments(line);
			if (line.size() == 0) { // Happens when line is a comment from the begining
				continue;
			}
			std::istringstream lineStream(line);
			boost::trim(line);
			std::string key;
			if( std::getline(lineStream, key, '=') ) {
				boost::trim(key);
				if (key.empty()) {
					error("Key is empty on line '" + std::to_string(lineNumber) + "'");
				}
				std::string value;
				if( std::getline(lineStream, value) ) {
					boost::trim(value);
					if (value.empty()) {
						error("Value is empty on line '" + std::to_string(lineNumber) + "'");
					}
					storeConfiguration(key, value);
				} else {
					error("Could not parse configuration file, invalid key '" + key + "' ('=' expected)");
				}
			} else {
			  error("Could not parse configuration file on line '" + std::to_string(lineNumber) + "'");
			}
			lineNumber++;
		} // endl while
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	checkIntegrity();
}

void Configurator::storeConfiguration(const std::string& key,
		const std::string& value) {
	StringVector valuesVector = m_tokenizer.split(value);
//	boost::split(valuesVector, value,boost::is_any_of(",")); // split by comma
	//Delete empty entries (e.g) a,b,c, <- last comma without value produces an empty string in valuesVector
	valuesVector.erase(
	std::remove_if(
		valuesVector.begin(),
		valuesVector.end(),
	[](std::string const& str) { return str.empty(); }),
	valuesVector.end());
	if ( valuesVector.size() == 0 ) {
		error("Parameter '" + key + "' is empty after preprocessing.\n"
				"If you are using the configuration delimiter '" + CONFIG_DELIMITER + "' "
						"wrap it around with \"\" or \'\' quotes");
	}
	// Trim leading and trailing spaces
	std::for_each(valuesVector.begin(), valuesVector.end(), [](std::string& str){boost::trim(str);});

	int configFlag = 0;
	int end = Configuration::LABELS.size();
	for ( ; configFlag < end; configFlag++ ) {
	   Configuration::flags flag = static_cast<Configuration::flags>(configFlag);
	   if ( key == Configuration::LABELS[flag] ) { //TODO: Maybe we need specific treatment according to the key
		   addOption(flag, key, valuesVector);
		   break;
	   }
	}
	if (configFlag == end) { //Not inserted -> unknown configuration parameter
		error("Key '" + key + "' is not recognized");
	}
}

void Configurator::error(const std::string& what) {
	throw std::runtime_error("Configurator - Error: " + what);
}

void Configurator::checkIntegrity() {
	// Check whether the configuration is valid
	Option* colnames = getOption(Configuration::flags::COL_NAMES);
	Option* colvalues = getOption(Configuration::flags::COL_VALUES);
	if ( colnames && colvalues ) {
		if ( colnames->getValuesSize() != colvalues->getValuesSize() ) {
			error("Configuration '" +  getConfigurationLabel(Configuration::flags::COL_NAMES) + "' and '"
					+ getConfigurationLabel(Configuration::flags::COL_VALUES) + "' have different sizes");
		}
	}
}

std::string Configurator::getConfigurationLabel(Configuration::flags flag) {
	return Configuration::LABELS[flag];
}

std::string Configurator::getConfigurationDefaultValue(
		Configuration::flags flag) {
	return Configuration::DEFAULT_VALUES[flag];
}

bool Configurator::existsKey(const std::string& key) {
	OptionMap::iterator it = m_optionMap.find(key);
	if ( it == m_optionMap.end() ) {
		return false;
	} else {
		return true;
	}
}

bool Configurator::existsKey(Configuration::flags flag) {
	std::string key = getConfigurationLabel(flag);
	return existsKey(key);
}

StringVector Configurator::getOptionValues(const std::string& key) {
	Option* option = getOption(key);
	if (option) {
		return option->getValues();
	}
	return StringVector();
}

void Configurator::addOption(Configuration::flags flag, const std::string& key,
		StringVector& valuesVector) {
	if (existsKey(flag)) {
		error("Configuration parameter '" + key + "' is duplicated");
	}
	Option op(key);
	op.copyVector(valuesVector);
	m_optionMap[key] = op;
}

StringVector Configurator::getOptionValues(Configuration::flags flag) {
	std::string key = getConfigurationLabel(flag);
	return getOptionValues(key);

}

Option* Configurator::getOption(const std::string& key) {
	OptionMap::iterator it = m_optionMap.find(key);
	if ( it == m_optionMap.end() ) {
		return nullptr;
	} else {
		return &(it->second);
	}
}

Option* Configurator::getOption(Configuration::flags flag) {
	std::string key = getConfigurationLabel(flag);
	return getOption(key);
}

bool Configurator::optionHasValue(Configuration::flags flag,
		const std::string& value) {
	Option* option = getOption(flag);
	if (option) { // User specified an explicit parameter 'key'
		StringVector values = option->getValues();
		return (std::find(values.begin(), values.end(), value) != values.end());
	} else { // User did not set this parameter, use default one
		return Configuration::DEFAULT_VALUES[flag] == value;
	}
}

std::string Configurator::getOptionValueFirst(Configuration::flags flag) {
	if (existsKey(flag)) { // Did the user set a specific value for this flag?
		StringVector optionValues = getOptionValues(flag);
		if (optionValues.size() > 0) {
			return optionValues[0];
		} else {
			error("Index '0' is out of bounds for flag: "
					+ Configuration::LABELS[flag]);
			return std::string();
		}
	} else {
		return Configuration::DEFAULT_VALUES[flag];
	}
}

/**
 * Returns the first value the user specified, or the default value for that flag
 */
std::string Configurator::getOptionValueAtIndex(Configuration::flags flag,
		unsigned int index) {
	if (existsKey(flag)) { // Did the user set a specific value for this flag?
		StringVector optionValues = getOptionValues(flag);
		if (index < optionValues.size()) {
			return optionValues[index];
		} else {
			error("Index '" + std::to_string(index) + "' is out of bounds"
					+ " for flag: " + Configuration::LABELS[flag]);
			return std::string();
		}
	} else {
		return Configuration::DEFAULT_VALUES[flag];
	}
}

void Configurator::avoidComments(std::string& str) {
	if (isComment(str)) {
		str = std::string(); // Clean string
	} else { // Check for a comment along the line
		std::string::size_type n = str.find(COMMENT_DELIMITER);
		if (n != std::string::npos)
			str.erase(n);
	}
}


bool Configurator::isComment(std::string& str) {
	return boost::starts_with(str, COMMENT_DELIMITER);
}

void Configurator::printConfiguration() {
	OptionMap::iterator it = m_optionMap.begin();
	for(; it != m_optionMap.end(); it++) {
		std::cout << "Key: " << it->first << std::endl;
		Option op = it->second;
		std::cout << "Values:" << std::endl;
		for(std::string  str : op.getValues()) {
			std::cout << "\t" << str << std::endl;
		}
	}

}
