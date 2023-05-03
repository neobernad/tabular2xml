/*
 * Converter.cpp
 *
 *  Created on: 14 may. 2018
 *      Author: Neo
 */

#include "Converter.hpp"

#include <iostream>
#include "../XMLGenerator/XMLGenerator.hpp"
#include <libxml/xmlsave.h>

Converter::Converter(const std::string& delimeter) {
	setUpTokenizer(delimeter);
}

Converter::~Converter() {
	if (m_pInputFile) {
		delete m_pInputFile;
		m_pInputFile = nullptr;
	}

	if (m_pOutputDoc) {
		XMLGenerator::getInstance().freeXMLDoc(m_pOutputDoc);
		m_pOutputDoc = nullptr;
	}
}

void Converter::load(const std::string& inputPath) {
	m_pInputFile = new std::ifstream(inputPath);
	if (!m_pInputFile) {
		std::cerr << "Could not load file '" << inputPath << "'" << std::endl;
		exit(0);
	}
	m_inputStream << m_pInputFile->rdbuf();
	m_pInputFile->close();
	m_pOutputDoc = XMLGenerator::getInstance().createXMLDoc();
	std::cout << "File '" << inputPath << "' has been loaded" << std::endl;
}

void Converter::setUpTokenizer(const std::string& delimeter) {
	m_tokenizer.setDelimiter(delimeter);
}

xmlDocPtr Converter::getXMLOutputDoc() const {
	return m_pOutputDoc;
}

void Converter::save(const std::string& outputPath) {
	//TODO: Check if outputPath is a valid path (dir. exists?)
	if (!m_pOutputDoc) {
		std::cerr << "No document loaded, please call load() first"
				<< std::endl;
		exit(1);
	}
	m_inputStream.clear();
	xmlSaveCtxt *ctxt = xmlSaveToFilename(outputPath.c_str(), "UTF-8",
			XML_SAVE_FORMAT | XML_SAVE_NO_EMPTY);
	if (!ctxt || xmlSaveDoc(ctxt, m_pOutputDoc) < 0 || xmlSaveClose(ctxt) < 0) {
		std::cerr << "Error: Could not save file to '" << outputPath << "'"
				<< std::endl;
		exit(1);
	}
	std::cout << "XML data saved in '" << outputPath << "'" << std::endl;
//	XMLGenerator::getInstance().freeXMLDoc(m_pOutputDoc); // We need the document later so we can create the XSD schema
}

void Converter::convertToXML() {
	if (m_inputStream.rdbuf()->in_avail() == 0 || !m_pOutputDoc) {
		std::cerr << "Input file has been not loaded yet or it is empty"
				<< std::endl;
		exit(1);
	}

	std::string line;

	while (std::getline(m_inputStream, line) && line.empty()); // Read first line;

	// Create a header
	bool advanceLine = false;
	StringVector header = createHeader(header, line, &advanceLine);
	if (advanceLine) {
		std::getline(m_inputStream, line); // Advance to next line so that do-while works properly
	}
	// /Create a header

	long numLine = 0;
	do {
		if (!line.empty()) {
			StringVector fields = m_tokenizer.split(line);
			xmlNodePtr node = XMLGenerator::getInstance().createXMLNode(fields,
					header);
			XMLGenerator::getInstance().addNodeToDoc(m_pOutputDoc, node);
		}
		numLine++;
	} while (std::getline(m_inputStream, line));
}

StringVector Converter::createHeader(StringVector& headerData,
		const std::string& line, bool *lineIsAHeader) {
	StringVector header;
	Configuration::flags hasHeader = Configuration::flags::HAS_HEADER;
	Configuration::flags colNames = Configuration::flags::COL_NAMES;

	if (Configurator::getInstance().optionHasValue(hasHeader, "true")) {
		header = m_tokenizer.split(line); // first line represents the header
		header = parseHeader(header);
		*lineIsAHeader = true;
	} else { // No header, use default header then
		// careful here 'line' represents the first line of data!
		unsigned int colSize = m_tokenizer.split(line).size();
		for (unsigned int i = 0; i < colSize; i++) {
			header.push_back(
					Configuration::DEFAULT_VALUES[colNames] + "_"
							+ std::to_string(i));
		}
	}

	// Check if the users set their own column names (col-names param)
	if (Configurator::getInstance().existsKey(colNames)) {
		header.clear();
		StringVector columnNames = Configurator::getInstance().getOptionValues(
				colNames);
		for (std::string column : columnNames) {
			header.push_back(column);
		}
	}

	// Clean strings 'Gene name/A' -> 'Gene_name_A'
//	std::for_each(header.begin(), header.end(),
//			[this](std::string& str) {XMLGenerator::getInstance().toXMLTag(str);});
	return header;
}

/*
 * If a header has an empty value, this method will replace the empty value
 * for a default one defined in Configuration::DEFAULT_VALUES
 */
StringVector Converter::parseHeader(StringVector& headerData) {
	StringVector copyHeader = headerData;
	for (unsigned int i = 0; i < copyHeader.size(); i++) {
		std::string& str = copyHeader[i]; // Reference to str, so it also changes in copyHeader
		if (str.empty()) {
			str = Configuration::DEFAULT_VALUES[Configuration::flags::COL_NAMES]
					+ "_" + std::to_string(i);
		}
	}
	return copyHeader;
}
