/*
 * ArgumentParser.hpp
 *
 *  Created on: 17 may. 2018
 *      Author: Neo
 */

#ifndef ARGUMENTPARSER_ARGUMENTPARSER_HPP_
#define ARGUMENTPARSER_ARGUMENTPARSER_HPP_

#include <string>
#include <vector>
#include "../Interfaces/IError.hpp"

namespace Input { // Caution!! The three next elements MUST have the same size
	// Order between flags and LABELS/DEFAULT_VALUES arrays must be coherent
	enum Type {TSV = 0, CSV};
	const std::vector< std::string > TYPE_LABEL = {"tsv", "csv"};
	const std::vector< std::string > DELIMETER_LABEL = {"\t", ","};
}


class ArgumentParser : public IError {
private:
	ArgumentParser();
	virtual ~ArgumentParser();
	std::string m_inputFilePath;
	Input::Type m_inputType;
	std::string m_inputTypeStr;
	std::string m_delimeter;

	std::string m_outputFilePath;
	std::string m_configFilePath;
	std::string m_xsdFilePath;

	std::string createXSDFilePath();
	std::string getPath(const std::string& filePath);
	std::string getFileName(const std::string& filePath);
	std::string getFileExtension(const std::string& fileName);
	std::string trimFileExtension(const std::string& fileName);
	bool fileExists(const std::string& filePath);
	void setFormat(const std::string& format);
	std::string getFormat(Input::Type format);
	void guessFormat(const std::string& filePath);
	void error(const std::string& what);
public:
	ArgumentParser(ArgumentParser const&) = delete;
	void operator=(ArgumentParser const&) = delete;
	static ArgumentParser& getInstance();

	void parseArguments(int argc, char** argv);
	const std::string& getConfigFilePath() const;
	const std::string& getInputFilePath() const;
	const std::string& getOutputFilePath() const;
	const std::string& getXSDFilePath() const;
	Input::Type getInputType() const;
	const std::string& getDelimeter() const;
	void setDelimeter(const std::string& delimeter);
	const std::string& getInputTypeStr() const;
	void setInputTypeStr(const std::string& inputTypeStr);
};

#endif /* ARGUMENTPARSER_ARGUMENTPARSER_HPP_ */
