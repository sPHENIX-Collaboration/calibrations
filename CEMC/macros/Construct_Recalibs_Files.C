#include <string>
#include <fstream>
#include <TFile.h>
#include <string>

using namespace std;
const int nbins = 16;

void Construct_Recalibs_Files()
{

  gSystem->Load("libg4detectors.so");

  PHG4Parameters *param = new PHG4Parameters("CEMC_RECALIB");

  param->set_string_param("description",
			  Form("Position based recalibrations for CEMC showers from Joe Osborn, inputfiles located at /calibrations/CEMC/PositionRecalibration/"));
  
  //the calibrations are made in 16 by 16 bins for a 2x2 cemc block
  //each row of values is for a bin in eta, with 16 bins of phi
  double calibrations[nbins][nbins];
  ifstream stream;
  stream.open("../PositionRecalibration/LO_positiondependent_calibs_phot.txt");

  cout<<"reading"<<endl;
  if(stream.is_open()){
    int row = 0;
    while(row!=nbins){
      int col=0;
      while(col!=nbins){
	double value;
	stream>>value;
	calibrations[row][col]=value;

	col++;
      }
      row++;
    }
      
  }
  else
    cout<<"no recalibs open, can't do anything"<<endl;

  cout<<"setting params"<<endl;
  for(int row=0; row<nbins; row++){
    for(int col=0; col<nbins; col++){
      
      string recalib_const_name(Form("recalib_const_eta%i_phi%i",row,col));
      param->set_double_param(recalib_const_name,calibrations[row][col]);

    }
  }

  param->set_int_param("number_of_bins",nbins);

  cout<<"write to xml file, located at the path below"<<endl;
  param->WriteToFile("xml","../");

}
