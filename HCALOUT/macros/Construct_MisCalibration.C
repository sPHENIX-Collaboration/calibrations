// $Id: $

/*!
 * \file DrawEcal_BlockCalibration.C
 * \brief 
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TLine.h>
#include <TString.h>
#include <TTree.h>
#include <cassert>
#include <cmath>
//#include "SaveCanvas.C"
//#include "sPhenixStyle.C"

using namespace std;

void Construct_MisCalibration(
    const double mis_calibration_width = 0.03)
{
  const int nbin_eta = 11 * 2;
  const int nbin_phi = 64;
  const string detector("HCALOUT");

  cout << "Construct_DeadTowerMap() - nbin_eta = " << nbin_eta << ", nbin_phi = " << nbin_phi << ", mis_calibration_width = " << mis_calibration_width << endl;

  TRandom3 random(13454);

  //Save result
  gSystem->Load("libphparameter.so");

  PHParameters* param = new PHParameters(detector);

  param->set_string_param("description",
                          Form("%s tower-by-tower calibration with %.2f percent miscalibration",
                               detector.c_str(), mis_calibration_width * 100));

  // use channel by channel stuff
  param->set_int_param("use_chan_calibration", 1);

  for (int eta = 0; eta < nbin_eta; ++eta)
  {
    for (int phi = 0; phi < nbin_phi; ++phi)
    {
      assert(mis_calibration_width >= 0);
      const double mis_calibration = random.Gaus(1, mis_calibration_width);

      string calib_const_name(Form("calib_const_eta%d_phi%d", eta, phi));
      param->set_double_param(calib_const_name, mis_calibration);
    }
  }

  string dir_name(Form("../TowerCalib"));

  if (mis_calibration_width != 0)
  {
    dir_name += Form("_MisCalib%.0fPercent", mis_calibration_width * 100);
  }

  gSystem->mkdir(dir_name.c_str());

  param->WriteToFile("xml", dir_name + "/");
}
