/*
 * Tokenizer.h
 *
 *  Created on: 14 may. 2018
 *      Author: Neo
 */

#ifndef TOKENIZER_TOKENIZER_HPP_
#define TOKENIZER_TOKENIZER_HPP_

#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>

class Tokenizer {
private:
	typedef boost::tokenizer< boost::escaped_list_separator<char> > boost_tokenizer;
	typedef boost::escaped_list_separator<char> boost_delimeter_list;
	boost_tokenizer* m_pTokenizer = nullptr;
	boost_delimeter_list* m_pDelimters = nullptr;

public:
	Tokenizer();
	Tokenizer(const std::string& delimiter);
	virtual ~Tokenizer();
	std::vector<std::string> split(const std::string& data);
	void setDelimiter(const std::string& delimiter);

};

#endif /* TOKENIZER_TOKENIZER_HPP_ */
