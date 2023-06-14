R__LOAD_LIBRARY(libintt.so)
#include <intt/InttFelixMapObsolete.h>

#include <iostream>

void InttFelixMapExample()
{
	int felix = 0;
	int felix_channel = 13;

	struct INTT_Felix::Ladder_s ldr_strct;

	//	Struct defined in the INTT_Felix namespace
	//	from the intt/InttFelixMap.h header
	//	from the libintt.so shared library
	//	included at the beginning of the macro
	//
	//	it contains:
	//	int barrel
	//	int layer
	//	int ladder

	INTT_Felix::FelixMap(felix, felix_channel, ldr_strct);

	//	This method passes ldr_struct by reference
	//	and mutates its barrel, layer, and ladder feilds
	//	so it's the ladder connected to Felix felix
	//	with felix channel felix_channel and returns EXIT_SUCCESS
	//	(0) upon success

	//	If felix, felix_channel do not correspond to actual ports
	//	the function sets the fields of ldr_strct to -1
	//	and returns EXIT_FAILURE (1)

	//	I have not implemented the reverse mapping

	//std::cout << "Felix: " << felix << std::endl;
	//std::cout << "Felix Channel: " << felix_channel << std::endl;

	//std::cout << "\tbarrel: " << ldr_strct.barrel << std::endl;
	//std::cout << "\tlayer: " << ldr_strct.layer << std::endl;
	//std::cout << "\tladder: " << ldr_strct.ladder << std::endl;
	
	felix = 0;
	for(felix_channel = 0; felix_channel < 14; ++felix_channel)
	{
		INTT_Felix::FelixMap(felix, felix_channel, ldr_strct);
		std::cout << "lyr: " << ldr_strct.barrel * 2 + ldr_strct.layer << std::endl;
		std::cout << "ldr: " << ldr_strct.ladder << std::endl;
		std::cout << std::endl;
	}
}

