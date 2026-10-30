#include <TFile.h>
#include <TNtuple.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TF1.h>
#include <TChain.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <string>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TMath.h>

void compute_twoTrackDca(std::vector<float> pcax_vec, std::vector<float> pcay_vec, std::vector<float> pcaz_vec, std::vector<float> px_vec, std::vector<float> py_vec, std::vector<float> pz_vec, std::vector<float> phi_vec, std::vector<float> &two_track_dcaxy_vec, std::vector<float> &two_track_dcaz_vec, std::vector<float> &two_track_ptsum_vec, std::vector<float> &two_track_psum_vec, std::vector<float> &two_track_phidiff_vec, TH2D* h_pt_corr)
{
  if(pcax_vec.size() < 2)
    return;
  ROOT::Math::XYZVectorF z_unit(0,0,1);
  for(int ii = 0; ii< pcax_vec.size()-1;ii++)
  {
    ROOT::Math::XYZVectorF trackdir1(px_vec.at(ii), py_vec.at(ii), pz_vec.at(ii));
    ROOT::Math::XYZVectorF trackpca1(pcax_vec.at(ii), pcay_vec.at(ii), pcaz_vec.at(ii));
    float pt1 = ROOT::Math::sqrt(pow(px_vec.at(ii),2)+ pow(py_vec.at(ii),2));
    float pmag1 = ROOT::Math::sqrt(trackdir1.Dot(trackdir1));
    float phi1 = phi_vec.at(ii);
    for(int jj = ii+1; jj<pcax_vec.size();jj++)
    {
      float pt2 = ROOT::Math::sqrt(pow(px_vec.at(jj),2)+ pow(py_vec.at(jj),2));
      float phi2 = phi_vec.at(jj);
      h_pt_corr->Fill(pt1,pt2);
      ROOT::Math::XYZVectorF trackdir2(px_vec.at(jj), py_vec.at(jj), pz_vec.at(jj));
      float pmag2 = ROOT::Math::sqrt(trackdir2.Dot(trackdir2));
      if(fabs(pmag1-pmag2)>0.1)
        continue;
      ROOT::Math::XYZVectorF trackpca2(pcax_vec.at(jj), pcay_vec.at(jj), pcaz_vec.at(jj));  
      ROOT::Math::XYZVectorF trackdiffxy(pcax_vec.at(jj)-pcax_vec.at(ii),pcay_vec.at(jj)-pcay_vec.at(ii),0.);
      ROOT::Math::XYZVectorF trackdiffz(0,0,pcaz_vec.at(jj)-pcaz_vec.at(ii));
      ROOT::Math::XYZVectorF dirCrossDirUnit = trackdir1.Cross(trackdir2).Unit();
      float dcaxy_out = dirCrossDirUnit.Dot(trackdiffxy);
      float dcaz_out = dirCrossDirUnit.Dot(trackdiffz);
      ROOT::Math::XYZVectorF ptvec = (trackdir1+trackdir2).Cross(z_unit);
      //float pt_out = ROOT::Math::sqrt(ptvec.Dot(ptvec)/2);
      float p_out = ROOT::Math::sqrt((pmag1+pmag2)/2);
      //two_track_dcaxy_vec.push_back(dirCrossDirUnit.Dot(trackpca2-trackpca1));
      two_track_dcaxy_vec.push_back(dcaxy_out);
      two_track_dcaz_vec.push_back(dcaz_out);
      //two_track_ptsum_vec.push_back(pt_out);
      two_track_ptsum_vec.push_back((pt1+pt2)/2);
      two_track_psum_vec.push_back(p_out);
      two_track_phidiff_vec.push_back(phi1-phi2);
      //std::cout<<"pt1: "<<pt1<<"  , pt2: "<<pt2<<std::endl;
      //std::cout<<"trackdiffxy: "<<trackdiffxy<<"  trackdiffz: "<<trackdiffz<<"  dirCrossDirUnit: "<<dirCrossDirUnit<<std::endl;
      //std::cout<<"dcaxy: "<<dcaxy_out<<"  dcaz: "<<dcaz_out<<"  dca: "<<dirCrossDirUnit.Dot(trackpca2-trackpca1)<<std::endl;
    }
  }
}

void plot_twoTrackDca_map_Greg()
{
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);

    TChain *resid_base = new TChain("residualtree");

    for(int i=0;i<999;++i)
    {	      
      char name[200];
      sprintf(name, "/sphenix/tg/tg01/hf/gregoryottino/silicon_tpc_matching/cluster_seeds_si_79516-%i.root_resid.root",i);
      std::cout << "Adding " << name << std::endl;
      resid_base->Add(name);
    }

    TTreeReader reader(resid_base);
    TTreeReaderValue<Float_t> pcax(reader,"pcax");
    TTreeReaderValue<Float_t> pcay(reader,"pcay");
    TTreeReaderValue<Float_t> pcaz(reader,"pcaz");
    TTreeReaderValue<Float_t> px(reader,"px");
    TTreeReaderValue<Float_t> py(reader,"py");
    TTreeReaderValue<Float_t> pz(reader,"pz");
    TTreeReaderValue<Float_t> pt(reader,"pt");
    TTreeReaderValue<Float_t> phi(reader,"phi");
    TTreeReaderValue<Float_t> vx(reader,"vx");
    TTreeReaderValue<Int_t> nmaps(reader,"nmaps");
    TTreeReaderValue<Int_t> nintt(reader,"nintt");
    TTreeReaderValue<Int_t> event(reader,"event");
    TTreeReaderValue<Int_t> crossing(reader,"crossing");

    TH2D *h_two_track_pt_dcaxy = new TH2D("h_two_track_pt_dcaxy","h_two_track_pt_dcaxy", 150, 0.0, 3.0, 400, -0.1, 0.1);
    TH2D *h_two_track_phi_dcaxy = new TH2D("h_two_track_phi_dcaxy","h_two_track_phi_dcaxy", 150, -3.4, 3.4, 400, -0.1, 0.1);
    TH2D *h_two_track_pt_dcaz = new TH2D("h_two_track_pt_dcaz","h_two_track_pt_dcaz", 150, 0.0, 3.0, 400, -0.1, 0.1);
    TH2D *h_two_track_p_dcaxy = new TH2D("h_two_track_p_dcaxy","h_two_track_p_dcaxy", 150, 0.0, 3.0, 400, -0.1, 0.1);
    TH2D *h_two_track_p_dcaz = new TH2D("h_two_track_p_dcaz","h_two_track_p_dcaz", 150, 0.0, 3.0, 400, -0.1, 0.1);
    TH2D *h_two_track_pt_corr = new TH2D("h_two_track_pt_corr","h_two_track_pt_corr", 100, 0.0, 3.0, 100, 0., 3.);
    //TH2D *h_vtx_pt_dcaxy = new TH2D("h_vtx_pt_dcaxy","h_vtx_pt_dcaxy", 100, 0.0, 2.0, 100, -0.1, 0.1);
    //TH2D *h_vtx_pt_dcaz = new TH2D("h_vtx_pt_dcaz","h_vtx_pt_dcaz", 100, 0.0, 2.0, 100, -0.1, 0.1);

    int event_cur = -1;

    std::map<int, std::vector<float>> pcax_map;
    std::map<int, std::vector<float>> pcay_map;
    std::map<int, std::vector<float>> pcaz_map;
    std::map<int, std::vector<float>> px_map;
    std::map<int, std::vector<float>> py_map;
    std::map<int, std::vector<float>> pz_map;
    std::map<int, std::vector<float>> phi_map;
    std::vector<float> two_track_dcaxy_vec;
    std::vector<float> two_track_dcaz_vec;
    std::vector<float> two_track_ptsum_vec;
    std::vector<float> two_track_psum_vec;
    std::vector<float> two_track_phidiff_vec;

    while (reader.Next()) {
      if(*nmaps<2||*nintt<1||*pt<.2||fabs(*vx)<0.0000001)
        continue;
      //new bunch crossing, dump previous crossing into hists
        std::cout<<"event: "<<*event<<"   : event cur: "<<event_cur<<std::endl;
      if(event_cur!=*event)
      {
        std::cout<<"test"<<std::endl;
        event_cur = *event;
        for(const auto& [key, value] : pcax_map)
        {
          compute_twoTrackDca(pcax_map[key], pcay_map[key], pcaz_map[key], px_map[key], py_map[key], pz_map[key], phi_map[key], two_track_dcaxy_vec, two_track_dcaz_vec, two_track_ptsum_vec,two_track_psum_vec,two_track_phidiff_vec, h_two_track_pt_corr);
          for(int ii = 0; ii<two_track_dcaxy_vec.size();ii++)
          {
            h_two_track_pt_dcaxy->Fill(two_track_ptsum_vec.at(ii),two_track_dcaxy_vec.at(ii));
            h_two_track_phi_dcaxy->Fill(two_track_phidiff_vec.at(ii),two_track_dcaxy_vec.at(ii));
            h_two_track_pt_dcaz->Fill(two_track_ptsum_vec.at(ii),two_track_dcaz_vec.at(ii));
            h_two_track_p_dcaxy->Fill(two_track_psum_vec.at(ii),two_track_dcaxy_vec.at(ii));
            h_two_track_p_dcaz->Fill(two_track_psum_vec.at(ii),two_track_dcaz_vec.at(ii));
          }
          two_track_dcaxy_vec.clear();
          two_track_ptsum_vec.clear();
          two_track_dcaz_vec.clear();
          two_track_psum_vec.clear();
          two_track_phidiff_vec.clear();
        }
        pcax_map.clear();
        pcay_map.clear();
        pcaz_map.clear();
        px_map.clear();
        py_map.clear();
        pz_map.clear();
        phi_map.clear();
      }
      pcax_map[*crossing].push_back(*pcax);
      pcay_map[*crossing].push_back(*pcay);
      pcaz_map[*crossing].push_back(*pcaz);
      px_map[*crossing].push_back(*px);
      py_map[*crossing].push_back(*py);
      pz_map[*crossing].push_back(*pz);
      phi_map[*crossing].push_back(*phi);      
    }

    TFile *outputFile = TFile::Open("twoTrackDca_map_out_si.root", "RECREATE");
    h_two_track_pt_dcaxy->Write();
    h_two_track_pt_dcaz->Write();
    h_two_track_p_dcaxy->Write();
    h_two_track_p_dcaz->Write();
    h_two_track_pt_corr->Write();
    h_two_track_phi_dcaxy->Write();
    outputFile->Close();
}

