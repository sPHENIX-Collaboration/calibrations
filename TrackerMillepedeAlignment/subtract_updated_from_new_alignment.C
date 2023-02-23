#include <trackbase/TrkrDefs.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int verbosity = 2;
void subtract_updated_from_new_alignment(
  std::string originalmisalignmentfile = "originalMisalignmentParamsFile.txt",
  std::string updatedfilename = "currentAlignmentParams.txt",
  std::string differencefilename = "differenceAlignmentParams.txt")
{

  std::cout << " Add new parameters to existing ones" << std::endl;
  
  ifstream foriginal(originalmisalignmentfile);
  if(!foriginal.is_open()) std::cout << "Unable to open original misalignment params file" << std::endl;
  
  ifstream fupdated(updatedfilename);
  if(!fupdated.is_open()) std::cout << "Unable to open updated params file" << std::endl;
  
  ofstream fdifference(differencefilename);
  
  TrkrDefs::hitsetkey key_original, key_updated;
  float pars_original[6], pars_updated[6];
  
  std::string line1, line2;

  std::streampos fnewpos = 0;
  
  while( getline(foriginal, line1) )
    {
      stringstream line_original(line1);
      if(verbosity > 1) std::cout << "original in: " << line_original.str() << std::endl;
      line_original >> key_original;
      getline(fupdated, line2);
      stringstream line_updated(line2);
      if(verbosity > 1) std::cout << "updated  in: " << line_updated.str() << std::endl;
      line_updated >> key_updated;

      line_original >> pars_original[0] >> pars_original[1] >> pars_original[2] >> pars_original[3] >> pars_original[4] >> pars_original[5];
      line_updated >> pars_updated[0] >> pars_updated[1] >> pars_updated[2] >> pars_updated[3] >> pars_updated[4] >> pars_updated[5];

      if(key_updated != key_original)
	{
	  std::cout << "ERROR: Mismatched keys: " << key_original << "   " << key_updated << std::endl;
	  fdifference << key_original << " " << pars_original[0] << " " 
		   << pars_original[1] << " " << pars_original[2] << " "
		   << pars_original[3] << " " << pars_original[4] << " "
		   << pars_original[5] << std::endl;
	  fupdated.seekg(fnewpos);

	}
      
      fnewpos = fupdated.tellg();

      fdifference << key_original << " "
		  << pars_original[0] - pars_updated[0] << " " 
		  << pars_original[1] - pars_updated[1] << " " 
		  << pars_original[2] - pars_updated[2] << " " 
		  << pars_original[3] - pars_updated[3] << " " 
		  << pars_original[4] - pars_updated[4] << " "
		  << pars_original[5] - pars_updated[5]
		  << std::endl;
    }
  
  fdifference.close();
  
  std::cout << "Done making difference file " << std::endl;  
  
}
