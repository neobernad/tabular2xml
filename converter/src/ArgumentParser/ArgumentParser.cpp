/*
 * ArgumentParser.cpp
 *
 *  Created on: 17 may. 2018
 *      Author: Neo
 */

#include "ArgumentParser.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <boost/program_options.hpp>
#include <sys/stat.h>
#include <libxml2/libxml/xmlversion.h>
#include <boost/version.hpp>

namespace po = boost::program_options;

ArgumentParser::ArgumentParser() {
	m_inputType = Input::Type::TSV;
}

ArgumentParser::~ArgumentParser() {}

ArgumentParser& ArgumentParser::getInstance() {
	static ArgumentParser instance;
	return instance;
}

const std::string& ArgumentParser::getConfigFilePath() const {
	return m_configFilePath;
}

const std::string& ArgumentParser::getInputFilePath() const {
	return m_inputFilePath;
}

std::string ArgumentParser::createXSDFilePath() {
	std::string path = getPath(getOutputFilePath()); // Solving output path
	std::string fileName = trimFileExtension(getFileName(getOutputFilePath())); // Output file name trimmed (no extension)
	return path + (path.empty()? "":"/") + fileName + ".xsd";
}

std::string ArgumentParser::getPath(const std::string& filePath) {
	size_t n = filePath.rfind('/', filePath.length());
	if (n != std::string::npos) {
		return(filePath.substr(0, n));
	}
	return std::string();
}

std::string ArgumentParser::getFileName(const std::string& filePath) {
	if (filePath.empty()) {
		return std::string();
	}
	size_t n = filePath.rfind('/', filePath.length());
	if (n != std::string::npos) {
	  return(filePath.substr(n+1, filePath.length() - n));
	}
	return filePath;
}

std::string ArgumentParser::trimFileExtension(const std::string& fileName) {
	if (fileName.empty()) {
		return std::string();
	}
	size_t lastindex = fileName.find_last_of(".");
	return fileName.substr(0, lastindex);
}

void ArgumentParser::error(const std::string& what) {
	throw std::runtime_error( "ArgumentParser - Error: " + what );
}

bool ArgumentParser::fileExists(const std::string& filePath) {
	std::ifstream f(filePath.c_str());
	if ( f.good() ) {
		struct stat s;
		if( stat(filePath.c_str(),&s) == 0 ) {
		    if( s.st_mode & S_IFDIR ) {
		        return false;
		    }
		    else if( s.st_mode & S_IFREG ) {
		    	return true;
		    }
		    else {
		    	return false;
		    }
		}
		else {
		    error( "Cannot stat file '" + filePath + "'" );
		}
	}
	return false;
}

const std::string& ArgumentParser::getOutputFilePath() const {
	return m_outputFilePath;
}

void ArgumentParser::parseArguments(int argc, char** argv) {
	std::string inputType;
	try {
		po::options_description desc("Allowed options");
		std::string configDesc = "(optional) the configuration file whose parameters are:\n"
				"\t- col-names=[column1,column2,...] List of column names\n"
				"\t- col-values=[-,*,...] List of default values for each column, if data is missing\n"
				"\t- has-header=[true|false] If true, it reads the first line as column names\n"
				"\t- item-tag=[string] XML tag which wraps the rows data from the input file\n"
				"\t- root-tag=[string] XML tag which wraps the whole data from the input file\n"
				"\t- col-division=[string] List of column names to be split by col-divider delimiter\n"
				"\t- col-divider=[\"string\"] Data delimiter. Default value is \",\"\n"
				"\tAdditional info: The character '#' can be used to make comments\n"
		;
		desc.add_options()
			("help,h", "produces help message")
			("input,i", po::value<std::string>(&m_inputFilePath), "the input file to transform to XML")
			("output,o", po::value<std::string>(&m_outputFilePath), "(optional) the output file (XML) where the transformation is stored (make sure the output directory exists)")
			("format,f", po::value<std::string>(&inputType), "(optional) the input file format [tsv, csv]")
			("config,c", po::value<std::string>(&m_configFilePath), configDesc.c_str())
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << std::endl;
			std::cout << "  Library versions:" << std::endl;
			std::cout << "  - LIBXML: " << LIBXML_DOTTED_VERSION << std::endl;
			std::cout << "  - LIBBOOST: " << BOOST_LIB_VERSION << std::endl;
			std::cout << "  - LIBBOOST/PROGRAM_OPTIONS: " << BOOST_PROGRAM_OPTIONS_VERSION << std::endl;
			std::cout << std::endl << "  Example run: converter -i path/file.tsv -o path/example.xml -c path/config.cfg" << std::endl;
			exit(0);
		}

		if (vm.count("input")) {
			if (!fileExists(m_inputFilePath)) {
				error( "Input file '" + m_inputFilePath + "' is a directory or does not exist" );
			}
		} else {
			std::string programName = argv[0];
			error( "An input file must be set, use '" +
					getFileName(programName) + " -h' to show up help menu" );
		}

		if (!vm.count("output")) {
			std::cout << "No output path detected. Default output file is '";
			std::string inputfileName = getFileName(m_inputFilePath);
			m_outputFilePath = trimFileExtension(inputfileName) + ".xml";
			std::cout << m_outputFilePath << "' in the current working directory" << std::endl;
		}

		if (vm.count("config")) {
			if (!fileExists(m_configFilePath)) {
				error( "Configuration file '" + m_configFilePath + "' is a directory or does not exist" );
			}
		} else {
			std::cout << "No configuration file loaded, using default parameters" << std::endl;
		}

		if (vm.count("format")) {
			setFormat(inputType);
		} else {
			guessFormat(m_inputFilePath);
		}

		m_xsdFilePath = createXSDFilePath();

		/*
		 * Print info
		 */
		std::cout << "------------------------------------------------" << std::endl;
		std::cout << "\tInput file: " << m_inputFilePath << std::endl;
		std::cout << "\tInput format: " << getFormat(getInputType()) << std::endl;
		std::cout << "\tConfiguration file: " << (m_configFilePath.empty()? "None":m_configFilePath) << std::endl;
		std::cout << "------------------------------------------------" << std::endl;
	}
	catch(std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	catch(...) {
		std::cerr << "Exception of unknown type! :(" << std::endl;
		exit(1);
	}
}

void ArgumentParser::setFormat(const std::string& format) {
	if ( format == Input::TYPE_LABEL[Input::TSV] ) {
		m_inputType = Input::TSV;
		m_inputTypeStr = Input::TYPE_LABEL[Input::TSV];
	} else if ( format == Input::TYPE_LABEL[Input::CSV] ) {
		m_inputType = Input::CSV;
		m_inputTypeStr = Input::TYPE_LABEL[Input::CSV];
	} else {
		error( "Unknown input format '" + format + "'" );
	}
}

void ArgumentParser::guessFormat(const std::string& filePath) {
	std::string extension = getFileExtension(getFileName(filePath));
	//TODO: Maybe execute tolower(extension) before setFormat()?
	setFormat(extension);
}

std::string ArgumentParser::getFileExtension(const std::string& fileName) {
	if (fileName.empty()) {
		return std::string();
	}
	size_t lastindex = fileName.rfind(".");
	return fileName.substr(lastindex+1);
}

std::string ArgumentParser::getFormat(Input::Type format) {
	switch (format) {
		case Input::Type::TSV:
			setDelimeter("\t");
			return Input::TYPE_LABEL[Input::Type::TSV];
			break;
		case Input::Type::CSV:
			setDelimeter(",");
			return Input::TYPE_LABEL[Input::Type::CSV];
			break;
		default:
			std::cerr << "Unknown format '" << getInputTypeStr() << "'" << std::endl;
			exit(1);
//			return "unknown";
			break;
	}
}

const std::string& ArgumentParser::getDelimeter() const {
	return m_delimeter;
}

const std::string& ArgumentParser::getInputTypeStr() const {
	return m_inputTypeStr;
}

void ArgumentParser::setInputTypeStr(const std::string& inputTypeStr) {
	m_inputTypeStr = inputTypeStr;
}

void ArgumentParser::setDelimeter(const std::string& delimeter) {
	m_delimeter = delimeter;
}

Input::Type ArgumentParser::getInputType() const {
	return m_inputType;
}

const std::string& ArgumentParser::getXSDFilePath() const {
	return m_xsdFilePath;
}
