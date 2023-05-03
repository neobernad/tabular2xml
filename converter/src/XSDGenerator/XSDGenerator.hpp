/*
 * XSDGenerator.hpp
 *
 *  Created on: 18 may. 2018
 *      Author: Neo
 */

#ifndef XSDGENERATOR_XSDGENERATOR_HPP_
#define XSDGENERATOR_XSDGENERATOR_HPP_

#include "../Interfaces/IError.hpp"
#include <libxml/tree.h>
#include <vector>
#include <string>

class XSDGenerator : public IError {
private:
	const std::string XSD_PREFIX = "xsd";
	const std::string XSD_NAMESPACE = "http://www.w3.org/2001/XMLSchema";
	const std::string XSD_ELEMENT = "element";
	const std::string XSD_COMPLEX_TYPE = "complexType";
	const std::string XSD_SEQUENCE = "sequence";
	const std::string XSD_ATTR_NAME = "name";
	const std::string XSD_ATTR_TYPE = "type";

	std::vector< std::string > m_vNames;
	xmlDocPtr m_pXMLDoc = nullptr;
	xmlDocPtr m_pXSDDoc = nullptr;

	XSDGenerator();
	virtual ~XSDGenerator();

	void error(const std::string& what);
	void searchInChild(xmlNodePtr iterator, xmlNodePtr parent, xmlNsPtr ns_xsd);
	bool isValidName(const xmlChar* name);
	template<typename T> //https://stackoverflow.com/questions/5932391/determining-if-a-string-is-a-double
	bool is(const std::string& str);
	template<bool>
	bool is(const std::string& str); // Overwritten
	std::string getType(const xmlChar* type);
public:
	XSDGenerator(XSDGenerator const&) = delete;
	void operator=(XSDGenerator const&) = delete;
	static XSDGenerator& getInstance();
	void load(xmlDocPtr doc);
	void generateXSDSchema();
};

#endif /* XSDGENERATOR_XSDGENERATOR_HPP_ */
