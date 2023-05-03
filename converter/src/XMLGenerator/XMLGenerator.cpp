/*
 * XMLGenerator.cpp
 *
 *  Created on: 17 may. 2018
 *      Author: Neo
 */

#include "XMLGenerator.hpp"
#include "../Configurator/Configurator.hpp"
#include <iostream>

XMLGenerator::XMLGenerator() {
	if (Configurator::getInstance().existsKey(Configuration::flags::COL_DIVISION)) {
		std::string delimeter = Configurator::getInstance().getOptionValueFirst(Configuration::flags::COL_DIVIDER);
		m_tokenizer.setDelimiter(delimeter);
	}
}

XMLGenerator::~XMLGenerator() {}

XMLGenerator& XMLGenerator::getInstance() {
	static XMLGenerator instance;
	return instance;
}

void XMLGenerator::error(const std::string& what) {
	throw std::runtime_error( "XMLGenerator - Error: " + what );
}

std::vector< std::string > XMLGenerator::preprocessField(const std::string& field, const std::string& header) {
	Configurator& configurator = Configurator::getInstance();
	std::vector< std::string > fieldVector;
	if (configurator.optionHasValue(Configuration::flags::COL_DIVISION, header)) { // The column "header" is in the col-division list?
		StringVector fieldSplitted = m_tokenizer.split(field);
		std::for_each(fieldSplitted.begin(), fieldSplitted.end(), [&fieldVector](std::string& str){
			fieldVector.push_back(str);
		});
	} else {
		fieldVector.push_back(field);
	}
	return fieldVector;
}

void XMLGenerator::toValidXMLString(std::string& str) {
	for (std::string separator : { " ", "/", "\\" }) {
		boost::replace_all(str, separator, "_");
	}
}
void XMLGenerator::replaceInvalidXMLChars(std::string& str) {
	std::string buffer;
	    buffer.reserve(str.size());
	    for(size_t pos = 0; pos != str.size(); ++pos) {
	        switch(str[pos]) {
	            case '&':  buffer.append("&amp;");       break;
	            case '\"': buffer.append("&quot;");      break;
	            case '\'': buffer.append("&apos;");      break;
	            case '<':  buffer.append("&lt;");        break;
	            case '>':  buffer.append("&gt;");        break;
	            default:   buffer.append(&str[pos], 1); break;
	        }
	    }
	    str.swap(buffer);
}

xmlDocPtr XMLGenerator::createXMLDoc() {
	std::string rootTag =
			Configurator::getInstance().
			getOptionValueFirst(Configuration::flags::ROOT_TAG);
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlDocSetRootElement(doc, xmlNewNode(NULL, BAD_CAST rootTag.c_str()));
	return doc;
}

xmlNodePtr XMLGenerator::createXMLNode(std::vector<std::string>& fields,
		std::vector<std::string>& header) {
	if ( fields.empty() ) {
		error( "Cannot create an XML node without fields" );
	}
	if ( header.empty() ) {
		error( "Cannot create an XML node without attributes" );
	}

	Configuration::flags itemTag = Configuration::flags::ITEM_TAG;
	Configuration::flags colValues = Configuration::flags::COL_VALUES;
	Configurator& configurator = Configurator::getInstance();
	std::string itemTag_str = configurator.getOptionValueFirst(itemTag);

	xmlNodePtr node = xmlNewNode(NULL, BAD_CAST itemTag_str.c_str());

	for (std::size_t i = 0; i < fields.size(); i++) {
		std::string tag =
			i < header.size() ? header[i] : "col_"+std::to_string(i);
		toValidXMLString(tag); // Clean strings 'Gene name/A' -> 'Gene_name_A'
		std::vector < std::string > fieldVector = preprocessField(fields[i], header[i]);
		for(std::string  fieldData : fieldVector) {
			if ( fieldData.empty() ) {
				// GetOptionValueAtIndex will return an user defined value at position "i"
				// or the default value for 'COL_VALUES' flag.
				fieldData = configurator.getOptionValueAtIndex(colValues, i);
			}
			replaceInvalidXMLChars(fieldData);
			xmlNewChild(node, NULL, BAD_CAST tag.c_str(),
					BAD_CAST fieldData.c_str());
		}
	}

	return node;

}

void XMLGenerator::freeXMLDoc(xmlDocPtr doc) {
	xmlFreeDoc(doc);
}

void XMLGenerator::addNodeToDoc(xmlDocPtr doc, xmlNodePtr node) {
	xmlAddChild(xmlDocGetRootElement(doc), node);
}
