/*
 * Converter.h
 *
 *  Created on: 14 may. 2018
 *      Author: Neo
 */

#ifndef CONVERTER_CONVERTER_HPP_
#define CONVERTER_CONVERTER_HPP_

#include <fstream>
#include <sstream>

#include "../Configurator/Configurator.hpp"
#include "../Interfaces/IConverter.hpp"
#include "../Tokenizer/Tokenizer.hpp"
#include <libxml/tree.h>

class Converter: public IConverter {
protected:
	std::ifstream* m_pInputFile = nullptr;
	xmlDocPtr m_pOutputDoc = nullptr;
	std::stringstream m_inputStream;
	Tokenizer m_tokenizer;

	StringVector createHeader(StringVector& headerData, const std::string& line, bool* lineIsAHeader);
	StringVector parseHeader(StringVector& headerData);
	void setUpTokenizer(const std::string& delimeter);
	std::string getConverterType();
public:
	Converter(const std::string& delimeter);
	virtual ~Converter();
	//IConverter.h
	void load(const std::string& inputPath) override;
	void convertToXML() override; // Conversion depends on the type of children converter
	void save(const std::string& outputPath) override;
	xmlDocPtr getXMLOutputDoc() const override;
};

#endif /* CONVERTER_CONVERTER_HPP_ */
