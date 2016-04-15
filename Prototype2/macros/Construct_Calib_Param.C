// $$Id: $$

#include <string>
#include <fstream>      // std::fstream
using namespace std;

void
Construct_Calib_Param(const char * filename_MIP_peak =
    "output_set2_v3.txt"
//    "output_set3.txt"
    )
{

  cout << "Construct_Calib_Param() - Entry" << endl;

  gSystem->Load("libg4detectors.so");

  PHG4Parameters * param = new PHG4Parameters("CEMC");

  param->set_string_param("description", Form("Calibration based on MIP scans from Michael Skoby, input file %s",filename_MIP_peak));

  // additional scale for the calibration constant
  // negative pulse -> positive with -1
  param->set_double_param("calib_const_scale", -1. * 16); // calibrated with high gain but produce the constant for low gain with x16 gain difference

  // use channel by channel stuff
  param->set_int_param("use_chan_calibration", 1);

  //  From: Michael Skoby [mailto:mskoby@umich.edu]
  //  Sent: Wednesday, April 13, 2016 5:29 PM
  //  To: Huang, Jin <jhuang@bnl.gov>
  //  Subject: EMCal MIP Fits
  //
  //  Text file with Column, Row, Peak: mskoby/emcal/output.txt
  //  Code: mskoby/emcal/getMipPed.C
  fstream f(filename_MIP_peak, ios_base::in);
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

      const double expected_mip = 0.3777 / 8; //expected MIP in GeV
      expected_mip *= 0.9666/1.582;
      param->set_double_param(calib_const_name, expected_mip / mip);
      count++;

    }

  cout << "Get " << count << " record from " << filename_MIP_peak << endl;

  // storage
  param->print();

//  param->WriteToFile("root", "./test_geom/");

  // this to solve an annoying problem for ROOT/XML regarding precision needed for Geant4
//  TBufferXML::SetFloatFormat("%.18e"); // match precision for double
  param->WriteToFile("xml", "../Calibration/");
//  param->WriteToFile("xml", "./");

}

void
Construct_Calib_Param_Constatnt_Test()
{
  cout << "Construct_Calib_Param() - Entry" << endl;

  gSystem->Load("libg4detectors.so");

  PHG4Parameters * param = new PHG4Parameters("CEMC");

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
  param->print();

//  param->WriteToFile("root", "./test_geom/");

  // this to solve an annoying problem for ROOT/XML regarding precision needed for Geant4
//  TBufferXML::SetFloatFormat("%.18e"); // match precision for double
  param->WriteToFile("xml", "../Calibration/");
//  param->WriteToFile("xml", "./");

}

