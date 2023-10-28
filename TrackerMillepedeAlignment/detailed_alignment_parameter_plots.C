int get_tpc_region(int layer)
{
  int region = 0;
  if(layer > 22 && layer < 39)
    region = 1;
  if(layer > 38 && layer < 55)
    region = 2;

  return region;  
}

void detailed_alignment_parameter_plots()
{
  gStyle->SetOptStat(0);

  TLine zero(0.0, 0.0,180.0,0.0);
  zero.SetLineStyle(1);
  zero.SetLineColor(kGray);

  /*
static const std::array<double, 3> mvtx_clamshell_mean[2] ={
   0.0, 0.0, 0.0,
   0.2, -0.2, 0.3}; 
static const std::array<double,3> mvtx_stave_mean[3] = {
  0.0, 0.0, 0.0,                       // fixed
  -0.05, 0.04, -0.03,                 // dx, dy, dz 
  0.04, -0.06, 0.05 };      


// run20 INTT
static const std::array<double,3> intt_stave_mean[4] = {
  0.2, -0.15, 0.0,
  0.2, -0.15, 0.0,
  0.2, -0.15, 0.0,
  0.2, -0.15, 0.0 };
// the index here is tpc region (0-2)

// run 20 tpc
static const std::array<double,3> tpc_sector_mean[3] = {
  1, -1, 1.0,
  -1, 1, 1.0, 
  0, 0, 0 };

  TLine offset(0, -0.1, 100, -0.1); // dummy settings for now
  offset.SetLineStyle(1);
  offset.SetLineColor(kRed);
  */

  TLegend *lpd = new TLegend(0.15, 0.2, 0.88, 0.35, "", "NDC");
  lpd->SetBorderSize(1);
  lpd->SetFillColor(kWhite);
  lpd->SetFillStyle(1001);


  TCanvas *cmvtx = new TCanvas("cmvtx","MVTX layers 0-2",50,50,1200,800);
  cmvtx->Divide(6,3);
  TCanvas *cintt = new TCanvas("cintt","INTT layers 3-6",50,50,1200,800);
  cintt->Divide(6,4);
  TCanvas *ctpc = new TCanvas("ctpc","TPC layers 8, 18, 28, 36, 50",50,50,1200,800);
  ctpc->Divide(6,5);
  int this_layer[5] = {8,18,28,36,50};

  std::vector<std::string> finvec;
  std::vector<std::string> legvec;
  std::vector<int> col;

  /*
  finvec.push_back("run24_input.root");
  legvec.push_back( "Input");
  col.push_back(kRed); 
  */
  /*
  finvec.push_back("tmp.root");
  legvec.push_back( "ideal");
  col.push_back(kBlue); 
  */

  finvec.push_back("/sphenix/tg/tg01/hf/frawley/residuals1/kshort_proc_25.rootseedresiduals.root");
  legvec.push_back( "ideal");
  col.push_back(kBlue); 


  for(int ifile = 0; ifile < finvec.size(); ++ifile)
    {
      TFile *fin = new TFile(finvec[ifile].c_str());      
      
      for(unsigned int layer = 0; layer <3; ++layer)
	{
	  for(unsigned int ipar =0; ipar < 6; ++ipar)
	    {
	      TH2D *hpar;
	      
	      char name[100];
	      sprintf(name, "hpar_%i_%i", layer, ipar);
	      fin->GetObject(name, hpar);
	      cmvtx->cd(layer*6+ipar + 1);
	      hpar->SetMarkerStyle(20);
	      hpar->SetMarkerSize(0.3);
	      float xmax = 0;
	      if(layer == 0)
		{	
		  xmax = 120;
		}
	      else if(layer == 1)
		{
		  xmax = 150;
		}
	      else
		{
		  xmax = 180;
		}
	      
	      char hname[100];
	      sprintf(hname,"layer %i par %i", layer, ipar);
	      hpar->SetTitle(hname);
	      hpar->GetXaxis()->SetRangeUser(0, xmax);
	      hpar->SetMarkerColor(col[ifile]);
	      if(ipar > 2 && ipar < 5)
		hpar->GetYaxis()->SetRangeUser(-0.5, 0.5);

	      if(ifile == 0)
		{	      
		  hpar->Draw();
		  zero.DrawLine(0,0,xmax,0);
		}
	      else
		hpar->Draw("same");

	      if(layer == 0 && ipar == 1)
		lpd->AddEntry(hpar, legvec[ifile].c_str(),"p");

	      if(ifile == finvec.size() -1 && ipar == 0)
		{
		  lpd->Draw();
		}

	      /*
	      if(ipar > 2)
		{
		  
		  std::cout << "draw line for ipar " << ipar << " layer " << layer << " with " 
			    << mvtx_clamshell_mean[0][ipar-3] +  mvtx_stave_mean[layer][ipar-3] << std::endl;
		  offset.DrawLine(0, mvtx_clamshell_mean[0][ipar-3]+ mvtx_stave_mean[layer][ipar-3], xmax/2, 
				  mvtx_clamshell_mean[0][ipar-3] + mvtx_stave_mean[layer][ipar-3]);
		  
		  std::cout << "draw line for ipar " << ipar << " layer " << layer << " with " 
			    << mvtx_clamshell_mean[1][ipar-3] + mvtx_stave_mean[layer][ipar-3] << std::endl;
		  offset.DrawLine(xmax/2, mvtx_clamshell_mean[1][ipar-3]+ mvtx_stave_mean[layer][ipar-3], xmax, 
				  mvtx_clamshell_mean[1][ipar-3] + mvtx_stave_mean[layer][ipar-3]);
		  
		}
	      */
	      
	    }
	  
	} 
            
      for(unsigned int layer = 3; layer < 7; ++layer)
	{
	  for(unsigned int ipar =0; ipar < 6; ++ipar)
	    {
	      TH2D *hpar;
	      
	      char name[100];
	      sprintf(name, "hpar_%i_%i", layer, ipar);
	      fin->GetObject(name, hpar);
	      cintt->cd((layer-3)*6 + ipar + 1);
	      float xmax = 0;
	      if(layer < 5)
		xmax = 48;
	      else
		xmax = 64;
	      
	      char hname[100];
	      sprintf(hname,"layer %i par %i", layer, ipar);
	      hpar->SetTitle(hname);
	      hpar->GetXaxis()->SetRangeUser(0, xmax);
	      if(ipar > 2 && ipar < 5)
		hpar->GetYaxis()->SetRangeUser(-0.5, 0.5);
	      else if(ipar == 5)
		hpar->GetYaxis()->SetRangeUser(-10.0, 10.0);
	      hpar->SetMarkerStyle(20);
	      hpar->SetMarkerSize(0.3);
	      hpar->SetMarkerColor(col[ifile]);

	      if(ifile == 0)
		{	      
		  hpar->Draw();
		  zero.DrawLine(0,0,xmax,0);
		}
	      else
		hpar->Draw("same");

	      if(ifile == finvec.size() -1 && ipar == 0)
		lpd->Draw();

	      /*	      
	      if(ipar > 2)
		{
		  
		  std::cout << "draw line for ipar " << ipar << " layer " << layer << " with " << intt_stave_mean[layer-3][ipar-3] << std::endl;
		  
		  offset.DrawLine(0, intt_stave_mean[layer-3][ipar-3], xmax, intt_stave_mean[layer-3][ipar-3]);
		}
	      */
	      
	    }
	  
	} 
      
      
      for(unsigned int layer_index = 0; layer_index < 5; ++layer_index)
	{
	  for(unsigned int ipar =0; ipar < 6; ++ipar)
	    {
	      TH2D *hpar;
	      
	      char name[100];
	      sprintf(name, "hpar_%i_%i", this_layer[layer_index], ipar);
	      fin->GetObject(name, hpar);
	      ctpc->cd(layer_index*6 + ipar + 1);
	      float xmax =26;
	      
	      char hname[100];
	      sprintf(hname,"layer %i par %i", this_layer[layer_index], ipar);
	      hpar->SetTitle(hname);
	      hpar->GetXaxis()->SetRangeUser(0, xmax);
	      hpar->SetMarkerStyle(20);
	      hpar->SetMarkerSize(0.4);
	      hpar->SetMarkerColor(col[ifile]);

	      if(ifile == 0)
		{	      
		  hpar->Draw();
		  zero.DrawLine(0,0,xmax,0);
		}
	      else
		hpar->Draw("same");

	      if(ifile == finvec.size() -1 && ipar == 0)
		lpd->Draw();

	      /*	      
	      if(ipar > 2)
		{
		  int region  = get_tpc_region(this_layer[layer_index]);
		  std::cout << "draw line for ipar " << ipar << " layer " << this_layer[layer_index] << " with " << tpc_sector_mean[region][ipar-3] << std::endl;
		  
		  offset.DrawLine(0, tpc_sector_mean[region][ipar-3], xmax, tpc_sector_mean[region][ipar-3]);
		}
	      */
	      
	    }
	  
	} 
      
    }  
}
