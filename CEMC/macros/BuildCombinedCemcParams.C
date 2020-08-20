// $Id: $

/*!
 * \file BuildCombinedCemcParams.C
 * \brief 
 * \author Spencer Griffith <spencer.griffith@colorado.edu>
 * \version $Revision:   $
 * \date $Date: $
 */

// Adapted from DrawEcal_BlockCalibration.C (Jin Huang <jhuang@bnl.gov>)
// Must manually load libphparameter.so before running in ROOT

#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TLine.h>
#include <TString.h>
#include <TTree.h>
#include <cassert>
#include <cmath>
#include <phparameter/PHParameters.h>
//#include "SaveCanvas.C"
//#include "sPhenixStyle.C"

using namespace std;

void BuildCombinedCemcParams(
    const TString infile =
        "/phenix/u/jinhuang/links/sPHENIX_work/prod_analysis/EMCal_photon_calib/original/EMCalAna.root",
    const double mis_calibration_width = 0.0, const double zero_suppression_thresh_GeV = 0.03)
{
  //  SetsPhenixStyle();
  TVirtualFitter::SetDefaultFitter("Minuit2");

  TRandom3 random(13454);

  TFile* f =
      TFile::Open(infile);

  assert(f);
  assert(f->IsOpen());

  TH1* EMCalAna_h_CEMC_BLOCK_ETotal = (TH1*)f->Get("EMCalAna_h_CEMC_BLOCK_ETotal");
  assert(EMCalAna_h_CEMC_BLOCK_ETotal);
  TH1* EMCalAna_h_CEMC_BLOCK_EVis = (TH1*)f->Get("EMCalAna_h_CEMC_BLOCK_EVis");
  assert(EMCalAna_h_CEMC_BLOCK_EVis);

  TH1* hSamplingFraction =
      (TH1*)EMCalAna_h_CEMC_BLOCK_EVis->Clone("hSamplingFraction");

  hSamplingFraction->Divide(EMCalAna_h_CEMC_BLOCK_ETotal);

  hSamplingFraction->SetTitle("CEMC visible sampling fraction, 4 GeV photon, |z|<10 cm; EMCal block ID; Sampling Fraction");
  hSamplingFraction->GetXaxis()->SetRangeUser(-.5, 47.5);
  hSamplingFraction->GetYaxis()->SetRangeUser(0.01, 0.03);
  hSamplingFraction->SetFillColor(kBlue - 3);
  hSamplingFraction->SetFillStyle(1001);

  TCanvas* c1 = new TCanvas("DrawEcal_BlockCalibration", "DrawEcal_BlockCalibration", 900, 600);
  c1->Divide(1, 1);
  int idx = 1;
  TPad* p;

  p = (TPad*) c1->cd(idx++);
  c1->Update();

  hSamplingFraction->Draw();

  TLegend* leg = new TLegend(.0, .7, .95, .93);
  leg->SetFillStyle(0);
  leg->AddEntry("", "#it{#bf{sPHENIX}}  Geant4 Simulation", "");
  leg->AddEntry("", "CEMC visible sampling fraction for 4 GeV photon, |z|<10 cm", "");
  //  leg->AddEntry("", "CEMC visible sampling fraction", "");
  //  leg->AddEntry("", "4 GeV photon, |z|<10 cm", "");
  leg->AddEntry("", "2017 projective SPACAL design", "");
  leg->AddEntry("", "1-D projective blocks for #21-26, rest are 2-D projective blocks", "");
  leg->Draw();

  //  SaveCanvas(c1, infile + "_" + TString(c1->GetName()), kTRUE);

  //Save result
  gSystem->Load("libg4detectors.so");

  PHParameters* param = new PHParameters("CEMC");

  param->set_string_param("description",
                          Form(
                              "2017 projective SPACAL design, Calibration based on CEMC visible sampling fraction for 4 GeV photon, source file: %s, pedestal central 0, pedestal width 8, GeV per ADC 0.002, zero suppression threshold %f",
                              infile.Data(), zero_suppression_thresh_GeV));

  // additional scale for the calibration constant
  // negative pulse -> positive with -1
  //  param->set_double_param("calib_const_scale", 1 / 0.02);  // overall scale the calibration constant to near 1.0
  const double calib_const_scale = 0.02;  // overall scale the calibration constant to near 1.0

  // use channel by channel stuff
  param->set_int_param("use_chan_calibration", 1);

  for (int eta = 0; eta < 96; ++eta)
  {
    for (int phi = 0; phi < 256; ++phi)
    {
      const int block_eta_id = eta / 2;

      assert(mis_calibration_width >= 0);
      const double mis_calibration = random.Gaus(1, mis_calibration_width);

      string calib_const_name(Form("calib_const_eta%d_phi%d", eta, phi));
      param->set_double_param(calib_const_name,
                              mis_calibration * calib_const_scale / hSamplingFraction->GetBinContent(hSamplingFraction->GetXaxis()->FindBin(block_eta_id)));

      // pedestal and GeV are static for now
      string pedestal_central_name(Form("PedCentral_ADC_eta%d_phi%d", eta, phi));
      param->set_double_param(pedestal_central_name, 0.0);

      string pedestal_width_name(Form("PedWidth_ADC_eta%d_phi%d", eta, phi));
      param->set_double_param(pedestal_width_name, 8.0);

      string GeV_per_ADC_name(Form("GeVperADC_eta%d_phi%d", eta, phi));
      param->set_double_param(GeV_per_ADC_name, 0.002); 
      // pedestal and GeV are static for now

      string zero_suppression_name(Form("ZS_ADC_eta%d_phi%d", eta, phi));
      param->set_double_param(zero_suppression_name,
                              (zero_suppression_thresh_GeV / ((param->get_double_param(GeV_per_ADC_name)) * (param->get_double_param(calib_const_name)))) + param->get_double_param(pedestal_central_name));
    }
  }

  string dir_name(Form("../TowerCalibCombinedParams_2020"));

  if (mis_calibration_width != 0)
  {
    dir_name += Form("_MisCalib%.0fPercent", mis_calibration_width * 100);
  }

  gSystem->mkdir(dir_name.c_str());

  param->WriteToFile("xml", dir_name + "/");
}
