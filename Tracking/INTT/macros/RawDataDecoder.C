#ifndef RAW_DATA_DECDODER_C
#define RAW_DATA_DECDODER_C

#include <intt/InttRawDataDecoder.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
//#include <fun4all/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <phool/recoConsts.h>

#include <iostream>
#include <string>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libintt.so)

std::string default_ifile = "/sphenix/user/jbertaux/commissioning/root_files/run_00008126.root";
std::string default_ofile = "/sphenix/user/jbertaux/commissioning/root_files/foo.root";

void RawDataDecoder(std::string i_filename=default_ifile, std::string o_filename=default_ofile)
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
	//Fun4AllInputManager* in = new Fun4AllPrdfInputManager("PRDF");
	Fun4AllInputManager* in = new Fun4AllDstInputManager("DST");
	in->fileopen(i_filename.c_str());
	se->registerInputManager(in);

	//ana
	SubsysReco* intt_decoder = new InttRawDataDecoder("INTTRAWDATADECODER");
	se->registerSubsystem(intt_decoder);

	//output
	Fun4AllOutputManager* out = new Fun4AllDstOutputManager("DST", o_filename.c_str());
	out->AddEventSelector("INTTRAWDATADECODER");
	out->AddNode("DST");
	se->registerOutputManager(out);

	se->run();
	se->End();

	delete se;
}

#endif//INTT_RAW_DATA_DECDODER_C
