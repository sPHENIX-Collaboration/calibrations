#ifndef RAW_DATA_CONVERTER_C
#define RAW_DATA_CONVERTER_C

#include <intt/InttRawDataConverter.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllInputManager.h>

#include <phool/recoConsts.h>

#include <iostream>
#include <string>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libintt.so)

std::string default_ifile = "/sphenix/lustre01/sphnxpro/commissioning/INTT/beam/beam_intt1-00009419-0000.evt";
std::string default_ofile = "/sphenix/user/jbertaux/commissioning/root_files/foo.root";

void RawDataConverter(std::string i_filename=default_ifile, std::string o_filename=default_ofile)
{
	if(i_filename.empty())
	{
		std::cout << "No input (.evt) file specified in first argument" << std::endl;
		std::cout << "Exiting" << std::endl;
		return;
	}
	if(o_filename.empty())
	{
		std::cout << "No output (.root) file specified in second argument" << std::endl;
		std::cout << "Exiting" << std::endl;
		return;
	}

	Fun4AllServer* se = Fun4AllServer::instance();

	//input
	Fun4AllInputManager* in = new Fun4AllPrdfInputManager("PRDF");
	in->fileopen(i_filename.c_str());
	se->registerInputManager(in);

	//ana
	SubsysReco* intt_converter = new InttRawDataConverter();
	se->registerSubsystem(intt_converter);

	//output
	se->run();
	((InttRawDataConverter*)intt_converter)->WriteOutputFile(o_filename);
	se->End();

	delete se;
}

#endif//INTT_RAW_DATA_CONVERTER_C
