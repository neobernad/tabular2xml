/*
 * CommandLineParser.h
 *
 *  Created on: 15 may. 2018
 *      Author: Neo
 */

#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include "Option/Option.hpp"
#include "../Interfaces/IError.hpp"
#include "../Tokenizer/Tokenizer.hpp"
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string/predicate.hpp> //boost::starts_with
#include <boost/algorithm/string.hpp> // boost::trim

typedef std::vector < std::string > StringVector;
typedef std::map< std::string, Option > OptionMap;

namespace Configuration { // Caution!! The three next elements MUST have the same size
	// Order between flags and LABELS/DEFAULT_VALUES arrays must be coherent
	enum flags {
		COL_NAMES = 0,
		COL_VALUES,
		HAS_HEADER,
		ITEM_TAG,
		ROOT_TAG,
		COL_DIVISION,
		COL_DIVIDER
	};

	const StringVector LABELS = {
		"col-names",	// COL_NAMES
		"col-values",	// COL_VALUES
		"has-header",	// HAS_HEADER
		"item-tag",		// ITEM_TAG
		"root-tag",		// ROOT_TAG
		"col-division",	//COL_DIVISION
		"col-divider"	//COL_DIVIDER
	};
	const StringVector DEFAULT_VALUES = {
		"column", 	// col-names
		"",			// col-values
		"true",		// has-header
		"Element",	// item-tag
		"Data",		// root-tag
		"",			// col-division
		","			// col-divider
	};
}

class Configurator : public IError {
private:
	const std::string COMMENT_DELIMITER = "#";
	const std::string CONFIG_DELIMITER = ",";

	Tokenizer m_tokenizer;
	Configurator();
	virtual ~Configurator();

	OptionMap m_optionMap;
	void error(const std::string& what);

	bool isComment(std::string& str);
	void avoidComments(std::string& str);
public:
	// Singleton methods
	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.
	Configurator(Configurator const&) = delete;
	void operator=(Configurator const&) = delete;
	static Configurator& getInstance();

	void readConfiguration(const std::string& filePath);
	void checkIntegrity();
	void storeConfiguration(const std::string& key, const std::string& value);
	void addOption(Configuration::flags flag, const std::string& key, StringVector& valuesVector);
	Option* getOption(const std::string& key);
	Option* getOption(Configuration::flags flag);
	bool existsKey(const std::string& key);
	bool existsKey(Configuration::flags flag);
	bool optionHasValue(Configuration::flags flag, const std::string& value);
	std::string getConfigurationLabel(Configuration::flags flag);
	std::string getConfigurationDefaultValue(Configuration::flags flag);
	StringVector getOptionValues(const std::string& key);
	StringVector getOptionValues(Configuration::flags flag);
	std::string getOptionValueFirst(Configuration::flags flag);
	std::string getOptionValueAtIndex(Configuration::flags flag, unsigned int index);

	void printConfiguration();
};

#endif /* CONFIGURATOR_H_ */
