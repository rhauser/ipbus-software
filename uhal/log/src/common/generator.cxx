
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/algorithm/string.hpp>

#include <uhal/log/log_configuration.hpp>


uint32_t gLogLevelCount ( 8 );


std::string gDivider (	"// " + std::string ( 150,'=' ) + "\n" +
						"// WARNING! This file is automatically generated! Do not modify it! Any changes will be overwritten!\n" +
						"// " + std::string ( 150,'=' ) + "\n" );



void fileHeaders ( std::ofstream& aHppFile , std::ofstream& aHxxFile , std::ofstream& aCppFile )
{
	aHppFile	<< "\n"
				<< "#ifndef _log_hpp_\n"
				<< "#define _log_hpp_\n"
				<< "\n"
				<< "#include <uhal/log/log_configuration.hpp>\n"
				<< "\n"
				<< "namespace uhal{\n"
				<< "\n"
				<< gDivider
				<< "\n";
	aHxxFile	<< "\n"
				<< "#include <uhal/log/log_inserters.hpp>\n"
				<< "\n"
				<< "namespace uhal{\n"
				<< "\n"
				<< gDivider
				<< "\n";
	aCppFile	<< "\n"
				<< "#include <uhal/log/log.hpp>\n"
				<< "\n"
				<< "namespace uhal{\n"
				<< "\n"
				<< gDivider
				<< "\n";
}



void fileContent ( std::ofstream& aHppFile , std::ofstream& aHxxFile , std::ofstream& aCppFile )
{
	std::stringstream lIfDefs , lEndIfs;
	std::vector< std::string > lLogLevels = uhal::log_configuration::getLogLevels();

	for ( std::vector< std::string >::iterator lIt = lLogLevels.begin() ; lIt != lLogLevels.end() ; ++lIt )
	{
		lIfDefs << "\t#ifndef LOGGING_EXCLUDE_" << boost::to_upper_copy ( *lIt ) << "\n";
		lEndIfs << "\t#endif\n";
		std::stringstream lTemplates;
		std::stringstream lArgs;
		std::stringstream lInstructions;

		for ( uint32_t i = 0 ; i!=MAX_NUM_ARGS ; ++i )
		{
			lTemplates << " typename T" << i << " ,";
			std::string lTemplatesStr ( lTemplates.str() );
			lTemplatesStr.resize ( lTemplatesStr.size()-1 );
			lArgs << " const T" << i << "& aArg" << i << " ,";
			std::string lArgsStr ( lArgs.str() );
			lArgsStr.resize ( lArgsStr.size()-1 );
			lInstructions << "\t\t\tlog_inserter( aArg" << i << " );\n";
			aHppFile	<< "template<" << lTemplatesStr << ">\n"
						<< "void log( const " <<*lIt << "& " << boost::to_upper_copy ( *lIt ) << " ," << lArgsStr << ");\n"
						<< "\n";
			aHxxFile	<< "template<" << lTemplatesStr << ">\n"
						<< "void log( const " <<*lIt << "& " << boost::to_upper_copy ( *lIt ) << " ," << lArgsStr << " )\n"
						<< "{\n"
						<< lIfDefs.str()
						<< "\t\tif( log_configuration::LoggingIncludes( " << boost::to_upper_copy ( *lIt ) << " ) ){\n"
						<< "\t\t\tlog_configuration::log_head( " << boost::to_upper_copy ( *lIt ) << " );\n"
						<< lInstructions.str()
						<< "\t\t\tlog_configuration::log_tail( " << boost::to_upper_copy ( *lIt ) << " );\n"
						<< "\t\t}\n"
						<< lEndIfs.str()
						<< "}\n"
						<< "\n";
		}

		aHppFile	<< gDivider
					<< "\n";
		aHxxFile	<< gDivider
					<< "\n";
	}
}


void fileFooters ( std::ofstream& aHppFile , std::ofstream& aHxxFile , std::ofstream& aCppFile )
{
	aHppFile	<< "}\n"
				<< "#include <uhal/log/log.hxx>\n"
				<< "#endif\n";
	aHxxFile	<< "}\n"
				<< "\n";
	aCppFile	<< "}\n"
				<< "\n";
}



int main ( int argc , char* argv[] )
{
	try
	{
		std::ofstream lHppFile ( "include/uhal/log/log.hpp" );

		if ( !lHppFile.is_open() )
		{
			std::cout << "Unable to open HPP file" << std::endl;
			return 1;
		}

		std::ofstream lHxxFile ( "include/uhal/log/log.hxx" );

		if ( !lHxxFile.is_open() )
		{
			std::cout << "Unable to open HXX file" << std::endl;
			return 1;
		}

		std::ofstream lCppFile ( "src/common/log.cpp" );

		if ( !lCppFile.is_open() )
		{
			std::cout << "Unable to open CPP file" << std::endl;
			return 1;
		}

		fileHeaders ( lHppFile , lHxxFile , lCppFile );
		fileContent ( lHppFile , lHxxFile , lCppFile );
		fileFooters ( lHppFile , lHxxFile , lCppFile );
		lHppFile.close();
		lHxxFile.close();
		lCppFile.close();
	}
	catch ( const std::exception& aExc )
	{
		std::cerr << "ERROR: Caught Exception : " << aExc.what() << std::endl;
		exit(1);
	}
}


