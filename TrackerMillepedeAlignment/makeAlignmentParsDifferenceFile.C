#include <trackbase/TrkrDefs.h>
#include <trackbase/MvtxDefs.h>
#include <trackbase/InttDefs.h>
#include <trackbase/TpcDefs.h>
#include <MicromegasDefs.h>
#include <trackbase/ActsGeometry.h>

#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void makeAlignmentParsDifferenceFile()
{

  // this version is for interactive testing only
  int verbosity = 2;

  // Subtract the updated alignment corrections file from the original misalignments file
  
  std::cout << " Add new parameters to existing ones" << std::endl;
  
  ifstream foriginal("run20_input_misalignments.txt");
  if(!foriginal.is_open()) std::cout << "Unable to open original misalignment params file" << std::endl;
  
  ifstream fupdated("sumAlignmentParams_run20_iter1.txt");
  if(!fupdated.is_open()) std::cout << "Unable to open updated params file" << std::endl;
  
  ofstream fdifference("differenceAlignmentParams_run20_iter1.txt");
  
  TrkrDefs::hitsetkey key_original, key_updated;
  float pars_original[6], pars_updated[6];
  
  std::string line1, line2;
  
  while( getline(foriginal, line1) )
    {
      stringstream line_original(line1);
      if(verbosity > 1) std::cout << "original in: " << line_original.str() << std::endl;
      line_original >> key_original;
      getline(fupdated, line2);
      stringstream line_updated(line2);
      if(verbosity > 1) std::cout << "updated  in: " << line_updated.str() << std::endl;
      line_updated >> key_updated;
      if(key_updated != key_original)
	std::cout << "ERROR: Mismatched keys: " << key_original << "   " << key_updated << std::endl;
      
      line_original >> pars_original[0] >> pars_original[1] >> pars_original[2] >> pars_original[3] >> pars_original[4] >> pars_original[5];
      line_updated >> pars_updated[0] >> pars_updated[1] >> pars_updated[2] >> pars_updated[3] >> pars_updated[4] >> pars_updated[5];
      fdifference << key_original << " "
		  << pars_original[0] - pars_updated[0]<< " " 
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

