/*
 * Tokenizer.cpp
 *
 *  Created on: 14 may. 2018
 *      Author: Neo
 */

#include "Tokenizer.hpp"

#include <iostream>

Tokenizer::Tokenizer() {
}

Tokenizer::Tokenizer(const std::string& delimiter) {
	m_pDelimters = new boost_delimeter_list("", // Do not let quoted arguments escape themselves
			delimiter, // Split depending on delimiter
			"\"\'"); // Let it have quoted arguments (e.g: "Ex sample", 'hello'
}

Tokenizer::~Tokenizer() {
	if (m_pDelimters)
		delete m_pDelimters;
	if (m_pTokenizer)
		delete m_pTokenizer;
//	std::cout << "Bye!" << std::endl;
}

std::vector<std::string> Tokenizer::split(const std::string& data) {
	if (!m_pDelimters) {
		std::cerr << "Error: no delimiter set for this tokenizer" << std::endl;
		exit(1);
	}
	m_pTokenizer = new boost_tokenizer(data, (*m_pDelimters));
	std::vector<std::string> result { m_pTokenizer->begin(), m_pTokenizer->end() };
	for (std::string& tok : result) {
		boost::trim(tok); // Trims leading and trailing spaces
	}
	return result;
}

void Tokenizer::setDelimiter(const std::string& delimiter) {
	if (m_pDelimters)
		delete m_pDelimters;
	m_pDelimters = new boost_delimeter_list("", // Do not let quoted arguments escape themselves
			delimiter, // Split depending on delimiter
			"\"\'"); // Let it have quoted arguments (e.g: "Ex sample"
}
