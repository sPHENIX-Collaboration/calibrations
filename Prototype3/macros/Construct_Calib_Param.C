// $$Id: $$

#include <string>
#include <fstream>      // std::fstream
#include <TNtuple.h>
#include <TFile.h>
#include <string>
#include <stdint.h>
#include <fstream>

using namespace std;

void
Construct_Calib_Param(const char * filename_MIP_peak =
    "mipPeak_set1_sim_cor.txt", const char * filename_shower_calib =
    "ShowerCalibFit_CablibConst.dat"
//    "output_set3.txt"
    )
{

  cout << "Construct_Calib_Param() - Entry" << endl;

  gSystem->Load("libg4detectors.so");

  PHG4Parameters * param = new PHG4Parameters("CEMC");

  param->set_string_param("description",
      Form(
          "Calibration based on MIP scans from Michael Skoby (input file %s), and electron shower calibration (input file %s)",
          filename_MIP_peak, filename_shower_calib));

  // additional scale for the calibration constant
  // negative pulse -> positive with -1
  param->set_double_param("calib_const_scale", -1. * 16); // calibrated with high gain but produce the constant for low gain with x16 gain difference

  // use channel by channel stuff
  param->set_int_param("use_chan_calibration", 1);

  //  Recalibration constant
  map<double, double> _recalib_const;

  fstream fcalib(filename_shower_calib, ios_base::in);
  assert(fcalib.is_open());
  while (!fcalib.eof())
    {
      string line1;

      getline(fcalib, line1);

      cout << filename_shower_calib << " get line " << line1 << endl;

      istringstream ssline(line1);

      int col = -1;
      int row = -1;
      double calib = 0;

      ssline >> col >> row >> calib;

      if (!ssline.fail())
        {

          cout << filename_shower_calib << " - recalibration constant  " << col
              << "," << row << " = " << calib << endl;

          _recalib_const[col * 10 + row] = calib;
        }

    }

  //  From: Michael Skoby [mailto:mskoby@umich.edu]
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

      const double recalib_const = _recalib_const[col * 10 + row];

      cout << "col = " << col << ", row = " << row << ", mip = " << mip
          << ", recalib = " << recalib_const << endl;

      assert(col >= 0);
      assert(row >= 0);
      assert(mip > 0);

      string calib_const_name(Form("calib_const_column%d_row%d", col, row));

      const double expected_mip = 0.3777 / 8; //expected MIP in GeV
      expected_mip *= -0.9666 / 1.582 * 2 * 6 / 8 * (8. / 8.11);
      param->set_double_param(calib_const_name, expected_mip / mip * recalib_const);
      count++;

    }

  cout << "Get " << count << " record from " << filename_MIP_peak << endl;

  // storage
//  param->print();

//  param->WriteToFile("root", "./test_geom/");

  // this to solve an annoying problem for ROOT/XML regarding precision needed for Geant4
//  TBufferXML::SetFloatFormat("%.18e"); // match precision for double
  param->WriteToFile("xml", "../Calibration/");
//  param->WriteToFile("xml", "./");

}

