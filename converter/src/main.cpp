/*
 * main.cpp
 *
 *  Created on: 14 may. 2018
 *      Author: Neo
 *      Version: 1.0
 *      Libraries: boost libxml2
 *      Flags: -lxml2 -lboost_program_options
 */

#include <iostream>
#include <string>

#include "Configurator/Configurator.hpp"
#include "Interfaces/IConverter.hpp"
#include "Factory/Factory.hpp"
#include "ArgumentParser/ArgumentParser.hpp"
#include "XSDGenerator/XSDGenerator.hpp"

int main(int argc, char** argv) {
	ArgumentParser::getInstance().parseArguments(argc, argv);
	std::string configFile = ArgumentParser::getInstance().getConfigFilePath();
	if (!configFile.empty()) {
		Configurator::getInstance().readConfiguration(ArgumentParser::getInstance().getConfigFilePath());
	}

	IConverter* converter = Factory::getInstance().createConverter(ArgumentParser::getInstance().getInputType());
	std::string inputFormat = ArgumentParser::getInstance().getInputTypeStr();

	converter->load(ArgumentParser::getInstance().getInputFilePath());
	std::cout << "Converting from '" + inputFormat + "' to XML..." << std::endl;
	converter->convertToXML();
	converter->save(ArgumentParser::getInstance().getOutputFilePath());
	try {
		XSDGenerator::getInstance().load(converter->getXMLOutputDoc());
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}
	delete converter;
	XSDGenerator::getInstance().generateXSDSchema();
	return 0;

}

