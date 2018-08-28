// $$Id: $$

#include <string>
#include <fstream>      // std::fstream
using namespace std;

void
Construct_Calib_Param_HCAL(
	const char* hcalin_MIP_peak ="calib_hcalin_hg.txt",
	const char* hcalout_MIP_peak ="calib_hcalout_hg.txt")
{

  cout << "Construct_Calib_Param() - Entry" << endl;
  gSystem->Load("libg4detectors.so");
  char *det_name[] = {"HCALIN_LG","HCALIN_HG","HCALOUT_LG","HCALOUT_HG"};

  // double sf[] = {0.0819254,0.032649}; //Sampling fractions
  double sf[] = {0.0631283,0.0338021}; //Sampling fractions from Songkyo Lee
  double ngains = 2;
  double gain_values[] = {32, 16};
  char *gain_tags[] = {"LG","HG"};
  TString name; 
  fstream f; 

  for(int idet=0; idet<2; idet++)
  {
   for(int igain=0; igain<2; igain++)
   {
    name = det_name[2*idet+igain];
    cout << name << endl;
    PHParameters * param = new PHParameters(name.Data());
    if(igain==0) param->set_double_param("calib_const_scale", gain_values[idet]);
    else param->set_double_param("calib_const_scale", 1.);
    param->set_int_param("use_chan_calibration", 1);
    param->set_string_param("description", "Calibration based on cosmic MIP scans");

    if(idet==0) f.open(hcalin_MIP_peak, ios_base::in);
    if(idet==1) f.open(hcalout_MIP_peak, ios_base::in);
    assert(f.is_open());
    int count = 0;
    while (!f.eof())
    {
      string line;
      getline(f, line);

      if (line.length() == 0)
        continue;

      int col = -1, row = -1;
      double mip = -1;

      stringstream sline(line);

      sline >> col >> row >> mip;

      cout << "col = " << col << ", row = " << row << ", mip = " << mip << endl;

      assert(col >= 0);
      assert(row >= 0);
      assert(mip > 0);

      string calib_const_name(Form("calib_const_column%d_row%d", col, row));

      mip *= (0.001/sf[idet]); //MeV to GeV and sf
      param->set_double_param(calib_const_name, mip);
      count++;

    }

  // storage
  param->Print();

//  param->WriteToFile("root", "./test_geom/");

  // this to solve an annoying problem for ROOT/XML regarding precision needed for Geant4
//  TBufferXML::SetFloatFormat("%.18e"); // match precision for double
  param->WriteToFile("xml", "../Calibration/");
  f.close();
  }

 }

}

void
Construct_Calib_Param_Constatnt_Test()
{
  cout << "Construct_Calib_Param() - Entry" << endl;

  gSystem->Load("libg4detectors.so");

  PHParameters * param = new PHParameters("CEMC");

  param->set_string_param("description",
      "Test file with same constant for all towers");

  // additional scale for the calibration constant
  // negative pulse -> positive with -1
  param->set_double_param("calib_const_scale", -1);

  // use channel by channel stuff
  param->set_int_param("use_chan_calibration", 1);

//  feed the numbers
  for (int col = 0; col < 8; col++)
    for (int row = 0; row < 8; row++)
      {

        string calib_const_name(Form("calib_const_column%d_row%d", col, row));

        param->set_double_param(calib_const_name, 1);

      }

  // storage
  //param->print();

//  param->WriteToFile("root", "./test_geom/");

  // this to solve an annoying problem for ROOT/XML regarding precision needed for Geant4
//  TBufferXML::SetFloatFormat("%.18e"); // match precision for double
  param->WriteToFile("xml", "../Calibration/");
//  param->WriteToFile("xml", "./");

}

