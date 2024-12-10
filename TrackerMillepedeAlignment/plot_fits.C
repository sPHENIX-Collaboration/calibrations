#ifndef PLOT_FITS_C
#define PLOT_FITS_C

#include <TFile.h>
#include <TNtuple.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TF1.h>
#include <TChain.h>
#include <TStyle.h>

#include <iostream>
#include <filesystem>
#include <string>

void plot_fits()
{
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);


  TCut trcut("");
  //TCut phicut("track_phi < -0.5");
  TCut phicut("");
  TCut lyrcut_intt("layer > 2 && layer < 7");
  TCut lyrcut_mvtx("layer < 3");
  TCut lyrcut_tpc("layer > 6 && layer < 55");

  TChain *ntp = new TChain("ntp");
  TChain *track_ntp = new TChain("track_ntp");

  for (auto const& entry : std::filesystem::directory_iterator{"dat/"}) {
    if (!entry.is_regular_file()) continue;

	std::string name{entry.path().filename()};
	if (name.find("helical_ntuple.root") == std::string::npos) continue;
	name = entry.path();

    std::cout << "Adding " << name << std::endl;
    ntp->Add(name.c_str());
    track_ntp->Add(name.c_str());
  }

  TH2D *hglob = new TH2D("hglob","global y:x",200, -5, 5, 200, -5, 5);
  TH2D *hfit = new TH2D("hfit","glob fity:fitx",200, -5, 5, 200, -5, 5);
  TH2D *hsens = new TH2D("hsens","glob sensy:sensx",200, -5, 5, 200, -5, 5);
  TH2D *hpoint = new TH2D("hpoint","glob pcay:pcax",200, -5, 5, 200, -5, 5);
  TH2D *htang = new TH2D("htang","glob tangy:tangx",200, -5, 5, 200, -5, 5);

  double dX = 1.0;
  double dY= 1.0;
  double dx = 1.5;
  double dy = 1.5;
  double vdX = 1.5;
  double vdY = 5.0;

  bool silicon = false;
  if(silicon) {
    dX = 0.06;
    dY = 0.06;
    dx = 0.5;
    dy = 0.5;
    vdX = 0.5;
    vdY = 0.5;
  }

  TCanvas *c1 = new TCanvas("c1","",50,300,1600,1600);
  c1->Divide(2,2);

  c1->cd(1);
  TH2D *hXb = new TH2D("hXb","Cluster X residual vs phi TPC",200,-3.5,3.5,200,-dX,dX);
  ntp->Draw("fitX-X:phi>>hXb",trcut && lyrcut_tpc);
  hXb->GetXaxis()->SetTitle("phi (rad)");
  hXb->DrawCopy("colz");

  c1->cd(2);
  TH2D *hYb = new TH2D("hYb","Cluster Y residual vs phi TPC",200,-3.5,3.5,200,-4,4);
  ntp->Draw("fitY-Y:phi>>hYb", trcut && lyrcut_tpc);
  hYb->GetXaxis()->SetTitle("phi (rad)");
  hYb->DrawCopy("colz");

  c1->cd(3);
  TH1D *hX1b = new TH1D("hX1b","Cluster X residual TPC",200,-dX, dX);
  ntp->Draw("fitX-X>>hX1b",trcut && lyrcut_tpc);
  hX1b->GetXaxis()->SetTitle("X residual (cm)");
  hX1b->DrawCopy("colz");

  //  TF1 *fg5 = new TF1("fg5","gaus",-0.6,0.6);
  // hX1a->Fit(fg5,"R");

  c1->cd(4);
  TH1D *hY1b = new TH1D("hY1b","Cluster Y residual TPC",200,-4,4);
  ntp->Draw("fitY-Y>>hY1b",trcut && lyrcut_tpc);
  hY1b->GetXaxis()->SetTitle("Y residual (cm)");
  hY1b->DrawCopy("colz");
  c1->SaveAs("plot_fits_c1.png");

  TCanvas *c2 = new TCanvas("c2","",50,300,1600,1600);
  c2->Divide(2,2);

  c2->cd(1);
  TH2D *hXa = new TH2D("hXa","Cluster X residual vs phi INTT",200,-3.5,3.5,200,-dX,dX);
  ntp->Draw("fitX-X:phi>>hXa",trcut && lyrcut_intt);
  hXa->GetXaxis()->SetTitle("phi (rad)");
  hXa->DrawCopy("colz");

  c2->cd(2);
  TH2D *hYa = new TH2D("hYa","Cluster Y residual vs phi INTT",200,-3.5,3.5,200,-4,4);
  ntp->Draw("fitY-Y:phi>>hYa", trcut && lyrcut_intt);
  hYa->GetXaxis()->SetTitle("phi (rad)");
  hYa->DrawCopy("colz");

  c2->cd(3);
  TH1D *hX1a = new TH1D("hX1a","Cluster X residual INTT",200,-dX, dX);
  ntp->Draw("fitX-X>>hX1a",trcut && lyrcut_intt);
  hX1a->GetXaxis()->SetTitle("X residual (cm)");
  hX1a->DrawCopy("colz");

  //  TF1 *fg5 = new TF1("fg5","gaus",-0.6,0.6);
  // hX1a->Fit(fg5,"R");

  c2->cd(4);
  TH1D *hY1a = new TH1D("hY1a","Cluster Y residual INTT",200,-4,4);
  ntp->Draw("fitY-Y>>hY1a",trcut && lyrcut_intt);
  hY1a->GetXaxis()->SetTitle("Y residual (cm)");
  hY1a->DrawCopy("colz");
  c2->SaveAs("plot_fits_c2.png");

  TCanvas *c3 = new TCanvas("c3","",50,300,1600,1600);
  c3->Divide(2,2);

  c3->cd(1);
  TH2D *hX = new TH2D("hX","Cluster X residual vs phi MVTX",200,-3.5,3.5,200,-dX,dX);
  ntp->Draw("fitX-X:phi>>hX",trcut && lyrcut_mvtx);
  hX->GetXaxis()->SetTitle("phi (rad)");
  hX->DrawCopy("colz");

  c3->cd(2);
  TH2D *hY = new TH2D("hY","Cluster Y residual vs phi MVTX",200,-3.5,3.5,200,-3*dY,3*dY);
  ntp->Draw("fitY-Y:phi>>hY", trcut && lyrcut_mvtx);
  hY->GetXaxis()->SetTitle("phi (rad)");
  hY->DrawCopy("colz");

  c3->cd(3);
  TH1D *hX1 = new TH1D("hX1","Cluster X residual MVTX",500,-dX, dX);
  ntp->Draw("fitX-X>>hX1",trcut && lyrcut_mvtx);
  hX1->GetXaxis()->SetTitle("X residual (cm)");
  hX1->DrawCopy("colz");

  TF1 *fg3 = new TF1("fg3","gaus",-0.2,0.2);
  //hX1->Fit(fg3,"R");

  c3->cd(4);
  TH1D *hY1 = new TH1D("hY1","Cluster Y residual MVTX",500,-3*dY, 3*dY);
  ntp->Draw("fitY-Y>>hY1",trcut && lyrcut_mvtx);
  hY1->GetXaxis()->SetTitle("Y residual (cm)");
  hY1->DrawCopy("colz");

  TF1 *fg4 = new TF1("fg4","gaus",-0.2,0.2);
  //hY1->Fit(fg4,"R");
  c3->SaveAs("plot_fits_c3.png");

  TCanvas  *c5 = new TCanvas("c5","",50,200,1600,1200);
  c5->Divide(3,2);

  c5->cd(1);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.13);
  TH2D *hvyx = new TH2D("hvyx","event vertex y:x",200, -dx, dx, 200, -dy, dy);
  track_ntp->Draw("event_yvtx:event_xvtx >> hvyx",phicut);
  hvyx->GetXaxis()->SetTitle("vertex x (cm)");
  hvyx->GetYaxis()->SetTitle("vertex y (cm)");
  hvyx->DrawCopy("colz");

  c5->cd(2);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.12);
  TH2D *hvryx = new TH2D("hvryx","event vertex residual Y:X",200, -vdX, vdX, 200, -vdY, vdY);
  track_ntp->Draw("residual_y:residual_x >> hvryx",phicut);
  hvryx->GetXaxis()->SetTitle("vertex X residual (cm)");
  hvryx->GetYaxis()->SetTitle("vertex Y residual (cm)");
  hvryx->DrawCopy("colz");

  c5->cd(3);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.12);
  TH1D *hvres = new TH1D("hvres","vertex X residual ", 200, -vdX, vdX);
  track_ntp->Draw("residual_x  >> hvres", phicut);
  hvres->GetXaxis()->SetTitle("vertex X residual (cm)");
  hvres->DrawCopy();

  TF1 *fg1 = new TF1("fg1","gaus",-0.3,0.3);
  //hvres->Fit(fg1,"R");

  c5->cd(4);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.12);
  TH1D *hz = new TH1D("hz","vertex Y residual",200,-vdY,vdY);
  track_ntp->Draw("residual_y>> hz", phicut);
  hz->GetXaxis()->SetTitle("vertex Y residual (cm)");
  hz->DrawCopy();

  TF1 *fg2 = new TF1("fg2","gaus",-0.045,0.045);
  //hz->Fit(fg2,"R");

  c5->cd(5);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.12);
  TH2D *hz1 = new TH2D("hz1","vertex X residual vs track phi",200,-3.5, 3.5, 200, -vdX, vdX);
  track_ntp->Draw("residual_x:track_phi >> hz1", phicut);
  hz1->GetYaxis()->SetTitle("vertex X residual (cm)");
  hz1->GetXaxis()->SetTitle("track phi (rad)");
  hz1->DrawCopy("colz");

  c5->cd(6);
  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.12);
  TH2D *hz2 = new TH2D("hz2","vertex Y residual vs track phi",200,-3.5, 3.5, 200, -vdY, vdY);
  //track_ntp->Draw("residual_y:track_zvtx >> hz2", phicut);
  track_ntp->Draw("residual_y:track_phi >> hz2", phicut);
  hz2->GetYaxis()->SetTitle("vertex Y residual (cm)");
  hz2->GetXaxis()->SetTitle("track phi (rad)");
  hz2->DrawCopy("colz");
  c5->SaveAs("plot_fits_c5.png");

  /*
  TCanvas *c6 = new TCanvas("c6","vtx X local derivatives", 40,40,1200,1200);
  c6->Divide(2,2);
  c6->cd(1);
  TH2D *hdxdxys = new TH2D("hdxdxys","hdxdxys vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dXdXYs:track_phi >>hdxdxys");
  hdxdxys->DrawCopy();

  c6->cd(2);
  TH2D *hdxdy0 = new TH2D("hdxdy0","hdxdy0 vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dXdY0:track_phi >>hdxdy0");
  hdxdy0->DrawCopy();

  c6->cd(3);
  TH2D *hdxdzs = new TH2D("hdxdzs","hdxdZs vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dXdZs:track_phi >>hdxdzs");
  hdxdzs->DrawCopy();

  c6->cd(4);
  TH2D *hdxdz0 = new TH2D("hdxdz0","hdxdZ0 vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dXdZ0:track_phi >>hdxdz0");
  hdxdz0->DrawCopy();
  */

  /*
  TCanvas *c7 = new TCanvas("c7","vtx Y local derivatives", 40,40,1200,1200);
  c7->Divide(2,2);
  c7->cd(1);
  TH2D *hdydxys = new TH2D("hdydxys","hdydxys vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dYdXYs:track_phi >>hdydxys");
  hdydxys->DrawCopy();

  c7->cd(2);
  TH2D *hdydy0 = new TH2D("hdydy0","hdydy0 vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dYdY0:track_phi >>hdydy0");
  hdydy0->DrawCopy();

  c7->cd(3);
  TH2D *hdydzs = new TH2D("hdydzs","hdydZs vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dYdZs:track_phi >>hdydzs");
  hdydzs->DrawCopy();

  c7->cd(4);
  TH2D *hdydz0 = new TH2D("hdydz0","hdydZ0 vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dYdZ0:track_phi >>hdydz0");
  hdydz0->DrawCopy();
  */
  /*
  TCanvas *c8 = new TCanvas("c8","vtx global derivatives", 40,40,1200,800);
  c8->Divide(3,2);

  c8->cd(1);
  TH2D *hdxdx = new TH2D("hdxdx","hdxdx vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dXdx:track_phi >>hdxdx");
  hdxdx->DrawCopy();

  c8->cd(2);
  TH2D *hdxdy = new TH2D("hdxdy","hdxdy vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dXdy:track_phi >>hdxdy");
  hdxdy->DrawCopy();

  c8->cd(3);
  TH2D *hdxdz = new TH2D("hdxdz","hdxdz vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dXdz:track_phi >>hdxdz");
  hdxdz->DrawCopy();

  c8->cd(4);
  TH2D *hdydx = new TH2D("hdydx","hdydx vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dYdx:track_phi >>hdydx");
  hdydx->DrawCopy();

  c8->cd(5);
  TH2D *hdydy = new TH2D("hdydy","hdydy vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dYdy:track_phi >>hdydy");
  hdydy->DrawCopy();

  c8->cd(6);
  TH2D *hdydz = new TH2D("hdydz","hdydz vs phi",200,-3.5,3.5,200,-2,2);
  track_ntp->Draw("dYdz:track_phi >>hdydz");
  hdydz->DrawCopy();
  */
  /*
  TCanvas *c9 = new TCanvas("c9","",50,50,800,800);
  
  track_ntp->Draw("track_phi","abs(track_xvtx) < 0.1 && abs(track_yvtx) < 0.1");
  */


}

#endif//PLOT_FITS_C
