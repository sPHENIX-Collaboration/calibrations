#include <trackbase/TrkrDefs.h>
#include <trackbase/MvtxDefs.h>
#include <trackbase/InttDefs.h>
#include <trackbase/TpcDefs.h>
#include <MicromegasDefs.h>
#include <trackbase/ActsGeometry.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <string>

R__LOAD_LIBRARY(libtrack_io.so)
R__LOAD_LIBRARY(libmicromegas.so)

unsigned int getSensor(TrkrDefs::hitsetkey hitsetkey)
{
  unsigned int sensor = 0;
  unsigned int trkrid = TrkrDefs::getTrkrId(hitsetkey);

  if(trkrid == TrkrDefs::mvtxId)
    {
      unsigned int staveid = MvtxDefs::getStaveId(hitsetkey);
      unsigned int chipid = MvtxDefs::getChipId(hitsetkey);
      sensor = staveid*9 + chipid;
      std::cout << " staveid " << staveid << " chipid " << chipid << std::endl;
    }
  else if(trkrid == TrkrDefs::inttId)
    {
      unsigned int ladderzid = InttDefs::getLadderZId(hitsetkey);
      unsigned int ladderphiid = InttDefs::getLadderPhiId(hitsetkey);
      sensor = ladderzid*4 + ladderphiid;      
    }
  else if(trkrid == TrkrDefs::tpcId)
    {
      unsigned int sector = TpcDefs::getSectorId(hitsetkey);
      unsigned int side = TpcDefs::getSide(hitsetkey);

      sensor = side * 12 + sector; 
    }
  else
    {
      // trkrid == TrkrDefs::micromegasId
      unsigned int tile = MicromegasDefs::getTileId(hitsetkey);
      sensor = tile;
    }

  return sensor;

}

void plot_alignment_residuals(std::string inputfilename,
			      std::string outfilename)
{
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

  // read in the alignment parameters file and make histograms for each layer
  // There is one entry in the file for every surface in the detector

  //ifstream fin("differenceLocalAlignmentParamsFile.txt");
  //ifstream fin("localAlignmentParamsFile.txt");
  //ifstream fin("new_alignment_corrections.txt");
  ifstream fin(inputfilename);
  if(!fin.is_open()) std::cout << "Unable to open input alignment params file" << std::endl;
  
  TH2D *hpar[57][6];
  for(int ilayer=0;ilayer<57;++ilayer)
    {
      for(int ipar = 0; ipar < 6; ++ipar)
	{
	  double range = 0.5;  // mm
	  double range_angles = 0.003;  // rad
	  if(ipar < 3) range = range_angles;
	  //if(ilayer > 6 && (ipar > 2 && ipar < 5)) range = 0.01;
	  //if(ilayer > 6 && (ipar == 5)) range = 0.1;
	  //if( (ilayer > 2 && ilayer < 7) && (ipar == 5) ) range = 0.4;
	  
	  char name[500];
	  char title[500];
	  sprintf(name,"hpar_%i_%i", ilayer, ipar);
	  if(ilayer < 3)  sprintf(title,"MVTX parameter %i", ipar);
	  else if (ilayer > 2 && ilayer < 7) sprintf(title,"INTT parameter %i", ipar);
	  else if (ilayer > 6 && ilayer < 55) sprintf(title,"TPC parameter %i", ipar);
	  else  sprintf(title,"MMS parameters %i", ipar);
	  
	  hpar[ilayer][ipar] = new TH2D(name, title, 600, 0, 200, 200, -range, +range);  // sensor number, parameter range
	  
	  hpar[ilayer][ipar]->GetXaxis()->SetNdivisions(504);
	  hpar[ilayer][ipar]->GetXaxis()->SetLabelSize(0.05);
	  hpar[ilayer][ipar]->GetXaxis()->SetTitleSize(0.05);
	  if(ipar < 3)
	    hpar[ilayer][ipar]->GetXaxis()->SetTitle("radians");
	  else
	    hpar[ilayer][ipar]->GetXaxis()->SetTitle("mm");	    	    
	}
    }
  
  std::string line;
  float pars[6];      
  TrkrDefs::hitsetkey hitsetkey;
  while( getline(fin, line) )
    {
      stringstream line_in(line);
      std::cout << "line in: " << line_in.str() << std::endl;
      line_in >> hitsetkey;
      line_in >> pars[0] >> pars[1] >> pars[2] >> pars[3] >> pars[4] >> pars[5];
      
      // extract sensor # from hitsetkey
      unsigned int trkrid = TrkrDefs::getTrkrId(hitsetkey);
      unsigned int layer = TrkrDefs::getLayer(hitsetkey);
      unsigned int sensor = getSensor(hitsetkey);
      std::cout << "      layer " << layer << " trkrid " << trkrid << " sensor " << sensor << std::endl;
      for(int ipar=0;ipar<6;++ipar)
	{
	  hpar[layer][ipar]->Fill(sensor, pars[ipar]);
	}
    }
  
  // make plots
  
  TCanvas *cmvtx = new TCanvas("mvtx","mvtx",100,100,1600,800);
  cmvtx->Divide(3,2);
  for(int ipar=0;ipar<6;++ipar)
    {
      cmvtx->cd(ipar+1);
      hpar[0][ipar]->Add(hpar[1][ipar]);
      hpar[0][ipar]->Add(hpar[2][ipar]);
      
      TH1D *hpar_combined = hpar[0][ipar]->ProjectionY();
      if(ipar < 3) 
	hpar_combined->GetXaxis()->SetTitle("rad");
      else
	hpar_combined->GetXaxis()->SetTitle("mm");
      
      hpar_combined->DrawCopy();
    }
  
  TCanvas *cintt = new TCanvas("intt","intt",150,150,1600,800);
  cintt->Divide(3,2);
  for(int ipar=0;ipar<6;++ipar)
    {
      cintt->cd(ipar+1);
      hpar[3][ipar]->Add(hpar[4][ipar]);
      hpar[3][ipar]->Add(hpar[5][ipar]);
      hpar[3][ipar]->Add(hpar[6][ipar]);
      
      TH1D *hpar_combined = hpar[3][ipar]->ProjectionY();
      if(ipar < 3) 
	hpar_combined->GetXaxis()->SetTitle("rad");
      else
	hpar_combined->GetXaxis()->SetTitle("mm");
      
      hpar_combined->DrawCopy();
    }
  
  TCanvas *ctpc = new TCanvas("tpc","tpc",200,200,1600,800);
  ctpc->Divide(3,2);
  for(int ipar=0;ipar<6;++ipar)
    {
      ctpc->cd(ipar+1);
      for(int i=8; i< 55; ++i)
	{
	  hpar[7][ipar]->Add(hpar[i][ipar]);
	}
      
      TH1D *hpar_combined = hpar[7][ipar]->ProjectionY();
      if(ipar < 3) 
	hpar_combined->GetXaxis()->SetTitle("rad");
      else
	hpar_combined->GetXaxis()->SetTitle("mm");
      
      hpar_combined->DrawCopy();
    }
  
  
  TFile *file = new TFile(outfilename.c_str(),"RECREATE");
  cmvtx->Write();
  cintt->Write();
  ctpc->Write();
  file->Write();
  file->Close();
}

      /*
  int nladders_layer[7] = {12, 16, 20, 12, 12, 16, 16};


  TH1D *hdev[7][6];
  for(int i=0;i<7;++i)
    {
      for(int ipar = 0; ipar < 6; ++ipar)
	{
	  double range;
	  if(ipar < 3) 
	    range = 0.001;
	  else
	    range = 0.001;

	  if(i > 2 && ipar == 5)
	    range = 0.10;

	  char hname[50];
	  sprintf(hname,"layer_%i_par_%i", i, ipar);
	  hdev[i][ipar] = new TH1D(hname, hname, 100,-range, range);
	  hdev[i][ipar]->GetXaxis()->SetNdivisions(504);
	  hdev[i][ipar]->GetXaxis()->SetLabelSize(0.05);
	  hdev[i][ipar]->GetXaxis()->SetTitleSize(0.05);
	  if(ipar < 3)
	    hdev[i][ipar]->GetXaxis()->SetTitle("radians");
	  else
	    hdev[i][ipar]->GetXaxis()->SetTitle("cm");	    
	}
    }

  int label = 0;
  float align = 0.0;
  float dummy = 0.0;
  std::string line;

  float results[7][20][6] = {0};  // layer, stave/ladder, parameter

  getline(fin,line);
  std::cout << line << std::endl;  // no data in first line
  int nstaves = 12+16+20+12+12+16+16;

  for(int i=0; i< nstaves; ++i)
    {
      for(int ipar=0;ipar<6;++ipar)
	{
	  fin >> label >> align >> dummy >> dummy;
	  
	  // decode the labels
	  int layer = label / 1000000;
	  int stave = (label - layer *1000000) / 10000;
	  int par = ipar;
	  
	  results[layer][stave][par] = align;
	  std::cout << " i " << i << " layer " << layer << " stave " << stave << " par " << par << " align " << align << std::endl;

	  hdev[layer][ipar]->Fill(align);
	}
    }

  // MVTX angles
  TCanvas *cmvtx_angles = new TCanvas("cmvtx_angles", " mvtx angles" , 0,0,2200,1200);
  cmvtx_angles->Divide(3,3);

  for(int i=0; i<3; ++i)
    {
      for(int ipar = 0; ipar < 3; ++ipar)
	{
	  cmvtx_angles->cd(i*3 + ipar + 1);
	  hdev[i][ipar]->Draw();
	}
    }

  // MVTX positions
  TCanvas *cmvtx_pos = new TCanvas("cmvtx_pos", " mvtx positions" , 60,60,2000,1200);
  cmvtx_pos->Divide(3,3);

  for(int i=0; i<3; ++i)
    {
      for(int ipar = 0; ipar < 3; ++ipar)
	{
	  cmvtx_pos->cd(i*3 + ipar + 1);
	  hdev[i][ipar+3]->Draw();
	}
    }

  // INTT angles
  TCanvas *cintt_angles = new TCanvas("cintt_angles", " INTT angles" , 120, 120,2000,1200);
  cintt_angles->Divide(3,4);
  
  for(int i=3; i<7; ++i)
    {
      for(int ipar = 0; ipar < 3; ++ipar)
	{
	  cintt_angles->cd((i-3)*3 + ipar + 1);
	  hdev[i][ipar]->Draw();
	}
    }  
  // INTT positions
  TCanvas *cintt_pos = new TCanvas("cintt_pos", " intt positions" , 180,180,2000,1200);
  cintt_pos->Divide(4,3);
  
  for(int i=3; i<7; ++i)
    {
      for(int ipar = 0; ipar < 3; ++ipar)
	{
	  cintt_pos->cd(ipar*4  + (i-3) + 1);
	  hdev[i][ipar+3]->Draw();
	}
    }  

  // TPC angles




  // TPC positions



  float result_staves[7][20] = {0};;
  std::cout << "Results summary:" << std::endl;
  for(int layer = 0; layer < 7; ++layer)
    {
      std::cout << "  layer " << layer << std::endl;

      for(int stave = 0; stave < nladders_layer[layer]; ++stave)
	{
	  std::cout << "       stave " << stave << std::endl;

	  for(int ipar = 0; ipar < 6; ++ipar)
	    {
	      std::cout << "          parameter " << ipar << " result " << results[layer][stave][ipar] << std::endl;
	      result_staves[layer][ipar] +=  results[layer][stave][ipar];
	    }
	}
    }
      */
  /*
  std::cout << "float deltas[7][6] = {" << std::endl;
  for(int layer = 0; layer < 7; ++layer)
    {
      std::cout << " {";
      for(int ipar = 0; ipar < 5; ++ipar)
	{
	  result_staves[layer][ipar] /= (float) nladders_layer[layer];      
	  std::cout <<  result_staves[layer][ipar] << ", ";
	}
      std::cout <<  result_staves[layer][5];
      if(layer < 6)  
	std::cout << "}," << std::endl;
      else
	std::cout << "} };" << std::endl;
    }
  */
  

