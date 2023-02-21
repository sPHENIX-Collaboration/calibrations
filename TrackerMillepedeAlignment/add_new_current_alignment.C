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

int verbosity = 2;
void add_new_current_alignment(
  std::string existingfile = "localAlignmentParamsFile.txt",
  std::string newfile = "new_alignment_corrections.txt",
  std::string updatedfile = "updatedAlignmentParams.txt")
{
  std::cout << " Add new parameters to existing ones" << std::endl;
  
  ifstream fexisting(existingfile);
  if(!fexisting.is_open()) std::cout << "Unable to open existing params file" << std::endl;
  
  ifstream fnew(newfile);
  if(!fnew.is_open()) std::cout << "Unable to open new params file" << std::endl;
  
  ofstream fupdated(updatedfile);
  
  TrkrDefs::hitsetkey key_exist, key_new;
  float pars_exist[6], pars_new[6], pars_update[6];
  
  std::string line1, line2;
  std::streampos fnewpos = 0;

  while( getline(fexisting, line1) )
    {
      stringstream line_exist(line1);
      if(verbosity > 1) std::cout << "exist in: " << line_exist.str() << std::endl;
      line_exist >> key_exist;
      unsigned short layer_exist = TrkrDefs::getLayer(key_exist);
      if(verbosity > 1) 
	{
	  std::cout << "     layer_exist " << layer_exist << std::endl; 	  
	  if(layer_exist > 6 && layer_exist < 55)
	    {
	      unsigned short sector = TpcDefs::getSectorId(key_exist);
	      unsigned short side = TpcDefs::getSide(key_exist);
	      std::cout << "       sector " << sector << " side " << side << std::endl;
	    }	    
	  if(layer_exist > 54)
	    {
	      unsigned short tile = MicromegasDefs::getTileId(key_exist);	      
	      MicromegasDefs::SegmentationType seg = MicromegasDefs::getSegmentationType(key_exist);
	      int type = 0;
	      if(seg == MicromegasDefs::SegmentationType::SEGMENTATION_Z) type = 1;
	      std::cout << "       tile " << tile << " segmentation " << type << std::endl;
	    }
	}
      
      getline(fnew, line2);
      stringstream line_new(line2);
      if(verbosity > 1) std::cout << "new in: " << line_new.str() << std::endl;
      line_new >> key_new;
      unsigned short layer_new = TrkrDefs::getLayer(key_new);
      if(verbosity > 1) 
	{
	  std::cout << "     layer _new " << layer_new << std::endl; 	  
	  if(layer_new > 6 && layer_new < 55)
	    {
	      unsigned short sector = TpcDefs::getSectorId(key_new);
	      unsigned short side = TpcDefs::getSide(key_new);
	      std::cout << "       sector " << sector << " side " << side << std::endl;
	    }
	  if(layer_new > 54)
	    {
	      unsigned short tile = MicromegasDefs::getTileId(key_new);	      
	      MicromegasDefs::SegmentationType seg = MicromegasDefs::getSegmentationType(key_new);
	      int type = 0;
	      if(seg == MicromegasDefs::SegmentationType::SEGMENTATION_Z) type = 1;
	      std::cout << "       tile " << tile << " segmentation " << type << std::endl;
	    }
	}
      
      if(key_new != key_exist)
	{
	  std::cout << "ERROR: Mismatched keys: in layer " << layer_new << " existing key " << key_exist << " new key " << key_new << std::endl;
	  /// If the keys mismatch, then there may not have been a result
	  /// for this particular key. Skip it by going back one in fnew stream
	  fnew.seekg(fnewpos);

	  /// Then just use the existing parameters for this key
	  fupdated << key_exist << " " << pars_exist[0] << " "
		   << pars_exist[1] << " " << pars_exist[2] << " "
		   << pars_exist[3] << " " << pars_exist[4] << " "
		   << pars_exist[5] << std::endl;
	}

      fnewpos = fnew.tellg();

      line_exist >> pars_exist[0] >> pars_exist[1] >> pars_exist[2] >> pars_exist[3] >> pars_exist[4] >> pars_exist[5];
      line_new >> pars_new[0] >> pars_new[1] >> pars_new[2] >> pars_new[3] >> pars_new[4] >> pars_new[5];
      fupdated << key_exist << " "
	       << pars_exist[0] + pars_new[0]<< " " 
	       << pars_exist[1] + pars_new[1] << " " 
	       << pars_exist[2] + pars_new[2] << " " 
	       << pars_exist[3] + pars_new[3] << " " 
	       << pars_exist[4] + pars_new[4] << " "
	       << pars_exist[5] + pars_new[5]
	       << std::endl;
    }
  fupdated.close();
  
  // copy updatedAlignmentParams.txt to currentAlignmentParams.txt
  ostringstream command;
  command.str("");
  command << "mv " << updatedfile << " currentAlignmentParams.txt";
  gSystem->Exec(command.str().c_str());
  
  std::cout << "Done updating currentAlignmentParams.txt file" << std::endl;        
  
}
