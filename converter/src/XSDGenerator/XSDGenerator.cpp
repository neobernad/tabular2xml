/*
 * XSDGenerator.cpp
 *
 *  Created on: 18 may. 2018
 *      Author: Neo
 */

#include "XSDGenerator.hpp"
#include <iostream>
#include "../XMLGenerator/XMLGenerator.hpp"
#include "../ArgumentParser/ArgumentParser.hpp"
#include <sstream>
#include <algorithm>
#include <stdexcept>


XSDGenerator::XSDGenerator() {}

XSDGenerator::~XSDGenerator() {
	if (m_pXMLDoc) {
		XMLGenerator::getInstance().freeXMLDoc(m_pXMLDoc);
		m_pXMLDoc = nullptr;
	}
	if (m_pXSDDoc) {
		XMLGenerator::getInstance().freeXMLDoc(m_pXSDDoc);
		m_pXSDDoc = nullptr;
	}
}

XSDGenerator& XSDGenerator::getInstance() {
	static XSDGenerator instance;
	return instance;
}

void XSDGenerator::load(xmlDocPtr doc) {
	if (!doc) {
		error( "Input document is null" );
	}
	m_pXMLDoc = xmlCopyDoc(doc, 1);
	// TODO Integrade XSDGenerator in Converter::save()
	// so that we save xmlCopyDoc call
}

void XSDGenerator::error(const std::string& what) {
	throw std::runtime_error( "XSDGenerator - Error: " + what );
}

void XSDGenerator::searchInChild(xmlNodePtr iterator, xmlNodePtr parent, xmlNsPtr ns_xsd) {
	xmlNodePtr children = iterator->children;
	while (children) {
		if (children->type == XML_ELEMENT_NODE && isValidName(children->name)) {
			xmlNodePtr element = xmlNewNode(ns_xsd, BAD_CAST XSD_ELEMENT.c_str());
			xmlNewProp(element, BAD_CAST XSD_ATTR_NAME.c_str(), children->name);
			if (xmlChildElementCount(children) != 0) { // Is leaf node? 0 == yes != 0 no
				xmlNodePtr complexType = xmlNewNode(ns_xsd, BAD_CAST XSD_COMPLEX_TYPE.c_str());
				xmlNodePtr sequence = xmlNewNode(ns_xsd, BAD_CAST XSD_SEQUENCE.c_str());
				xmlAddChild(parent, element);
				xmlAddChild(element, complexType);
				xmlAddChild(complexType, sequence);
				searchInChild(children, sequence, ns_xsd);
			} else { // Leaf node
				std::string type = getType(xmlNodeGetContent(children));
				xmlNewProp(element, BAD_CAST XSD_ATTR_TYPE.c_str(), BAD_CAST type.c_str());
				xmlAddChild(parent, element);
			}
		}
		children = xmlNextElementSibling(children);
	}
}

bool XSDGenerator::isValidName(const xmlChar* name) {
	std::string nameStr = std::string((char*) name);
	if (std::find(m_vNames.begin(), m_vNames.end(), nameStr) != m_vNames.end()) {
		return false;
	}
	m_vNames.push_back(nameStr);
	return true;
}

template <>
bool XSDGenerator::is<bool>(const std::string& str) {
	std::string copyStr = str;
	std::transform(copyStr.begin(), copyStr.end(), copyStr.begin(), ::tolower);
    if (copyStr == "true" || copyStr == "false" || copyStr == "yes" || copyStr == "no") {
    	return true;
    }
    return false;
}

template <typename T>
bool XSDGenerator::is(const std::string& str) {
    std::istringstream iss(str);
    T x;
    return iss >> std::noskipws >> x && iss.eof();
}

std::string XSDGenerator::getType(const xmlChar* type) {
	std::string typeStr = std::string((char*) type);
	if (is<bool>(typeStr)) {
		return "xsd:boolean";
	}
	if (is<int>(typeStr)) {
		return "xsd:integer";
	}
	if (is<float>(typeStr)) {
		return "xsd:decimal";
	}
	if (is<double>(typeStr)) {
		return "xsd:decimal";
	}
	return "xsd:string";
}

void XSDGenerator::generateXSDSchema() {
	if (!m_pXMLDoc) {
		std::cerr << "No document loaded, please call load() first" << std::endl;
		exit(1);
	}
	m_pXSDDoc = XMLGenerator::getInstance().createXMLDoc();

	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "schema");
	xmlNsPtr ns_xsd = xmlNewNs(root_node,
		BAD_CAST XSD_NAMESPACE.c_str(),
		BAD_CAST XSD_PREFIX.c_str());
	xmlNodePtr iterator = xmlDocGetRootElement(m_pXMLDoc); // Iterator will move along the XML file
	while (iterator && isValidName(iterator->name)) {
		xmlNodePtr element = xmlNewNode(ns_xsd, BAD_CAST XSD_ELEMENT.c_str());
		xmlNodePtr complexType = xmlNewNode(ns_xsd, BAD_CAST XSD_COMPLEX_TYPE.c_str());
		xmlNodePtr sequence = xmlNewNode(ns_xsd, BAD_CAST XSD_SEQUENCE.c_str());
		xmlNewProp(element, BAD_CAST XSD_ATTR_NAME.c_str(), iterator->name);
		xmlAddChild(root_node, element);
		xmlAddChild(element, complexType);
		xmlAddChild(complexType, sequence);
		searchInChild(iterator, sequence, ns_xsd);
		iterator = xmlNextElementSibling(iterator);
	}
	xmlDocSetRootElement(m_pXSDDoc, root_node);
	xmlSetNs(root_node, ns_xsd); //xsd:schema
	std::string outputPath = ArgumentParser::getInstance().getXSDFilePath();
	xmlSaveFormatFileEnc(outputPath.c_str(),
			m_pXSDDoc, "UTF-8", 1);
	std::cout << "XSD data saved in '" << outputPath << "'" << std::endl;
}
