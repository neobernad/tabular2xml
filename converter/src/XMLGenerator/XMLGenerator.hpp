/*
 * XMLGenerator.hpp
 *
 *  Created on: 17 may. 2018
 *      Author: Neo
 */

#ifndef XMLGENERATOR_XMLGENERATOR_HPP_
#define XMLGENERATOR_XMLGENERATOR_HPP_

#include "../Interfaces/IError.hpp"
#include "../Tokenizer/Tokenizer.hpp"
#include <libxml2/libxml/tree.h>
#include <string>
#include <vector>

class XMLGenerator : public IError {
private:
	Tokenizer m_tokenizer;
	XMLGenerator();
	virtual ~XMLGenerator();
	void error(const std::string& what);
	std::vector< std::string > preprocessField(const std::string& field, const std::string& header);
public:
	static XMLGenerator instance;
	XMLGenerator(XMLGenerator const&) = delete;
	void operator=(XMLGenerator const&) = delete;
	static XMLGenerator& getInstance();

	/// It turns a string into a free XML escape character string. Modified by reference.
	void toValidXMLString(std::string& str);
	/// Replaces invalid XML characters into underscore "_". Modified by reference.
	void replaceInvalidXMLChars(std::string& str);
	xmlDocPtr createXMLDoc();
	void freeXMLDoc(xmlDocPtr doc);
	xmlNodePtr createXMLNode(std::vector < std::string >& data, std::vector < std::string >& attributes);
	void addNodeToDoc(xmlDocPtr doc, xmlNodePtr node);
};

#endif /* XMLGENERATOR_XMLGENERATOR_HPP_ */
