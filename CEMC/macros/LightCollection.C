// $Id: $                                                                                             

/*!
 * \file LightCollection.C
 * \brief Construct the light collection data file for PHG4FullProjSpacalCellReco::LightCollectionModel
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <TMath.h>

#include <iostream>
#include <cassert>

using namespace std;

void
LightCollection()
{
//  gStyle->SetOptStat(0);

  TFile * fdata = TFile::Open("../LightCollection/Prototype2Module.xml",
      "recreate");

  TH2 * h2 = LoadMikePhippsLightGuideEff();

  h2->SetDirectory(NULL);
  fdata->cd();
  h2->Write();

  TH1 * h1 = LoadSCSF78Fiber();

  h1->SetDirectory(NULL);
  fdata->cd();
  h1->Write();

}

TH2 *
LoadMikePhippsLightGuideEff()
{
  TFile * fsrc = new TFile("contaminationFourNeighbor.root");
  assert(fsrc->IsOpen());

  TH2F * contamHisto = fsrc->GetObjectChecked("contamHisto", "TH2F");
  const int n = contamHisto->GetXaxis()->GetNbins() - 1; // remove one empty bins

  TH2F * data_grid_light_guide_efficiency =
      new TH2F("data_grid_light_guide_efficiency",
          "Extracted light collection efficiency from Mike Phipps;x positio fraction;y position fraction", //
          n, 0., 1., n, 0., 1.);
  TH1F * stat_data_grid_light_guide_efficiency = new TH1F(
      "stat_data_grid_light_guide_efficiency",
      "Efficiency statistics;Reletive Efficiency;Count of bins", 100, 0, 0.33);
  TH1F * stat_data_grid_light_guide_efficiency_normalized = new TH1F(
      "stat_data_grid_light_guide_efficiency_normalized",
      "Efficiency statistics;Reletive Efficiency;Count of bins", 100, 0, 2);

  for (int x = 1; x <= n; x++)
    {
      for (int y = 1; y <= n; y++)
        {
          data_grid_light_guide_efficiency->SetBinContent(x, y,
              contamHisto->GetBinContent(x + 1, y + 1));
        }
    }

  data_grid_light_guide_efficiency->Smooth(1, "k5b");
  data_grid_light_guide_efficiency->Rebin2D(2, 2);
  data_grid_light_guide_efficiency->Smooth(1, "k5a");

  for (int x = 1; x <= data_grid_light_guide_efficiency->GetNbinsX(); x++)
    {
      for (int y = 1; y <= data_grid_light_guide_efficiency->GetNbinsY(); y++)
        {
          double eff = data_grid_light_guide_efficiency->GetBinContent(x, y);

          //Fix the large variation relative to newer version concluded by Mike:
          //          Currrent sPHENIX LG
          //          Uniformity RMS:
          //          0.011 (% efficiency)
          //          Total Efficiency: 13.5%
          //          Surface area covered by sipm: 18.4 %
          eff = (eff + 0.0119 * 3) * 3;

          stat_data_grid_light_guide_efficiency->Fill(eff);
          data_grid_light_guide_efficiency->SetBinContent(x, y, eff);
        }
    }

  data_grid_light_guide_efficiency->Scale(
      1. / stat_data_grid_light_guide_efficiency->GetMean());

  for (int x = 1; x <= data_grid_light_guide_efficiency->GetNbinsX(); x++)
    {
      for (int y = 1; y <= data_grid_light_guide_efficiency->GetNbinsY(); y++)
        {
          double eff = data_grid_light_guide_efficiency->GetBinContent(x, y);
          stat_data_grid_light_guide_efficiency_normalized->Fill(eff);
        }
    }

  TCanvas * c2 = new TCanvas("LoadMikePhippsLightGuideEff",
      "LoadMikePhippsLightGuideEff", 1800, 600);
  c2->Divide(3, 1);

  c2->cd(1);
  contamHisto->Draw("colz");

  c2->cd(2);
  data_grid_light_guide_efficiency->Draw("colz");

  c2->cd(3);
  stat_data_grid_light_guide_efficiency_normalized->Draw();

  return data_grid_light_guide_efficiency;
}

TH1 *
LoadSCSF78Fiber()
{
  const double fiber_length = 13.5; // cm
  const double attenuation_length = 105; // cm, from Sean Stoll
  const double back_reflection = 0.3; // Rough estimation from Sean Stoll
  const int N = 100;

  TH1 * data_grid_fiber_trans =
      new TH1F("data_grid_fiber_trans",
          "SCSF-78 Fiber Transmission VS hit position in fiber;position in fiber (cm);Effective transmission",
          N, -fiber_length / 2, fiber_length / 2);

  for (int i = 1; i <= N; i++)
    {
      const double z = data_grid_fiber_trans->GetXaxis()->GetBinCenter(i);

      const double eff = (TMath::Exp(
          -(z + fiber_length / 2) / attenuation_length)
          + back_reflection
              * TMath::Exp(
                  -(fiber_length + fiber_length / 2 - z) / attenuation_length))
          / (1
              + back_reflection
                  * TMath::Exp(-fiber_length * 2 / attenuation_length));
      data_grid_fiber_trans->SetBinContent(i, eff);

//      cout << "LoadSCSF78Fiber - bin " << i << " @ z = " << z << " : eff = "
//          << eff << endl;
    }

  const double mean = data_grid_fiber_trans->GetSumOfWeights() / N;
  data_grid_fiber_trans->Scale(1. / mean);

  new TCanvas("LoadSCSF78Fiber", "LoadSCSF78Fiber");
  data_grid_fiber_trans->Draw();

  return data_grid_fiber_trans;
}
