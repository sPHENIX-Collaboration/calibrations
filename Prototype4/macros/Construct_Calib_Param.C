// $$Id: $$

#include <TFile.h>
#include <TNtuple.h>
#include <stdint.h>
#include <fstream>  // std::fstream
#include <fstream>
#include <string>
#include <string>

using namespace std;

void Construct_Calib_Param(const char* filename_shower_calib =
                               "4thPositionScan_ShowerCalibFit_CablibConst.dat"
                           //    "output_set3.txt"
                           )
{
  cout << "Construct_Calib_Param() - Entry" << endl;

  gSystem->Load("libphparameter");

  PHParameters* param = new PHParameters("CEMC");

  param->set_string_param("description",
                          Form(
                              "Calibration based on electron shower calibration (input file %s)",
                              filename_shower_calib));

  // additional scale for the calibration constant
  // negative pulse -> positive with -1
  param->set_double_param("calib_const_scale", 8. / 3000);  // leading order calibration

  // use channel by channel stuff
  param->set_int_param("use_chan_calibration", 1);

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

      string calib_const_name(Form("calib_const_column%d_row%d", col, row));
      param->set_double_param(calib_const_name, calib);
    }
  }

  // this to solve an annoying problem for ROOT/XML regarding precision needed for Geant4
  //  TBufferXML::SetFloatFormat("%.18e"); // match precision for double
  param->WriteToFile("xml", "../Calibration/");
  //  param->WriteToFile("xml", "./");
}
