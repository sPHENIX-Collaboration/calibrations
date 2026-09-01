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
#include <vector>
#include <map>
#include <cmath>
#include <TVector2.h>
#include <Math/Vector3D.h>
#include <algorithm>
#include <random>
#include <numeric>
#include <set>
#include <tuple>
#include <TSystem.h>

#include "UsefulFunctions.h"

void compute_twoTrackDca(const std::vector<float>& pcax_vec, const std::vector<float>& pcay_vec, const std::vector<float>& pcaz_vec, const std::vector<float>& px_vec, const std::vector<float>& py_vec, const std::vector<float>& pz_vec, const std::vector<float>& eta_vec, const std::vector<float>& phi_vec, const std::vector<float>& dcaxy_vec, const std::vector<float>& dcaz_vec, std::vector<float> &two_track_dcaxy_vec, std::vector<float> &two_track_dcaz_vec, std::vector<float> &two_track_ptavg_vec, std::vector<float> &two_track_pavg_vec, std::vector<float> &two_track_etadiff_vec, std::vector<float> &two_track_phidiff_vec, TH2D* h_pt_corr, TH3D *h_etadiff_two_track_dcaxy_pt, TH3D *h_etadiff_two_track_dcaz_pt, TH3D *h_phidiff_two_track_dcaxy_pt, TH3D *h_phidiff_two_track_dcaz_pt, TH3D* h_eta1_eta2_two_track_dcaxy, TH3D* h_eta1_eta2_two_track_dcaz, TH3D* h_phi1_phi2_two_track_dcaxy, TH3D* h_phi1_phi2_two_track_dcaz, TH3D* h_etadiff_phidiff_two_track_dcaxy, TH3D* h_etadiff_phidiff_two_track_dcaz, TH3D* h_etadiff_phidiff_two_track_dcaxy3d, TH3D* h_etadiff_phidiff_two_track_dcaz3d, TH2D* h_dcaxy1_dcaxy2_diffvs_2trackdcaxy,TH2D* h_dcaz1_dcaz2_diffvs_2trackdcaz); 

int vertex_ntracks_cut=3; float fabs_vx_cut=0.0000001; //event level cuts applied to all tracks
int nmaps_cut=2, intt_cut=1, ntpc_cut=26; float pt_cut=0.2; float pt_cut_etaphitwotrackdca3dhists=0.8; float chisq_ndf_cut = 10; //track level cuts, pt_cut_etaphitwotrackdca3dhists is used to filter tracks used in two track dca histograms except for the h_etadiff_two_track_dcaxy_pt histograms as they already show pt information
float pdiffpair_cut=0.1; //track pair level cuts

void MakeHistograms_twoTrack_and_vertex_distributions(const bool is_simulation_input=false)
{
    bool createpdffile=false;
    float dcaxy_onesiderange_formeanwidthcalc=0.01, dcaz_onesiderange_formeanwidthcalc=0.01,
        deltavertex_onesiderange_formeanwidthcalc=0.01;
    
    char outputrootfilename[200]="defaultoutputname.root";
    char description[200]="description";
    char plots_outputdir[200]=".";
    char vertexcuts_info[200];
    snprintf(vertexcuts_info, sizeof(vertexcuts_info),
             "VertexCuts: vertex_ntracks > %i, |vx|>=%g", vertex_ntracks_cut, fabs_vx_cut);
    
    char trackcuts_info[200];
    if(is_simulation_input){
        snprintf(trackcuts_info, sizeof(trackcuts_info),
                 "TrackCuts: m_nmaps>=%i && m_nintt>=%i && m_ntpc>=%i && pt>=%g && chisq_ndf< %g",
                 nmaps_cut, intt_cut, ntpc_cut, pt_cut, chisq_ndf_cut);
    }
    else{
        snprintf(trackcuts_info, sizeof(trackcuts_info),
                 "TrackCuts: m_nmaps>=%i && m_nintt>=%i && m_ntpc>=%i && pt>=%g",
                 nmaps_cut, intt_cut, ntpc_cut, pt_cut);
    }
    
    char trackpaircuts_info[200];
    snprintf(trackpaircuts_info, sizeof(trackpaircuts_info),
             "PairCuts: |p_{1}-p_{2}|<= %g", pdiffpair_cut);
    
    float title_xstart=0.4, title_ystart=0.95;
    
    TChain *resid_base = new TChain("residualtree");
    if(is_simulation_input){
        snprintf(outputrootfilename, sizeof(outputrootfilename),"twoTrackDca_map_out_Simulation.root");
        snprintf(description, sizeof(description),"Pythia 8 (%s)",vertexcuts_info);
        snprintf(plots_outputdir, sizeof(plots_outputdir),"./Plots_current/twoTrackDCA_and_DeltavertexPlots/Simulation");
        title_xstart=0.4; title_ystart=0.95;
        
        resid_base->Add("./output_Pythia_Simulation/SoftQCD_nonDiffractive/residuals_G4sPHENIX_minbiasPythia_withchisqbyndf_200kevts*.root");
        
        //resid_base->Add("./output_Pythia_Simulation/SoftQCD_nonDiffractive/residuals_G4sPHENIX_minbiasPythia_200kevts*.root");
       
        //resid_base->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_105kevts_withtruedca.root");
        //resid_base->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_175kevts*.root");
        //resid_base->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_200kevts_withtruedca_set*.root");
        //resid_base->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_200kevts_withtruedca_set*.root");
    }
    else{
        snprintf(outputrootfilename, sizeof(outputrootfilename),"twoTrackDca_map_out_data.root");
        snprintf(description, sizeof(description),"sPHENIX (%s)",vertexcuts_info);
        snprintf(plots_outputdir, sizeof(plots_outputdir),"./Plots_current/twoTrackDCA_and_DeltavertexPlots/Data");
        title_xstart=0.4; title_ystart=0.95;
        
        char name[200];
        for(int i=0;i<999;++i)
        {
            //snprintf(name,sizeof(name), "/sphenix/tg/tg01/hf/gregoryottino/silicon_tpc_matching/cluster_seeds_all_79516-%i.root_resid.root",i);
            //snprintf(name,sizeof(name), "/sphenix/tg/tg01/hf/gregoryottino/silicon_tpc_matching/output_baseAlignment/cluster_seeds_all_79516-%i.root_resid.root",i);
            snprintf(name,sizeof(name), "/sphenix/tg/tg01/hf/gregoryottino/silicon_tpc_matching/output_baseAlignment/cluster_seeds_si_79516-%i.root_resid.root",i);
            //std::cout << "Adding " << name << std::endl;
            resid_base->Add(name);
        }
    }
    float pt_min=0.0, pt_max=3.0;
    float p_min=0.0, p_max=3.0;
    float phi_min=-3.4, phi_max=3.4;
    float eta_min=-3.0, eta_max=3.0;
    float dcaxy_min=-0.2, dcaxy_max=0.2, dcaz_min=-0.2, dcaz_max=0.2;
    float nvertices_min=-0.5, nvertices_max=20.5;
    float vxy_min=-0.2, vxy_max=0.2;
    float pcaxy_min=-0.2, pcaxy_max=0.2;
    int nbins_vxy=800, nbins_pcaxy=800;
    
    int nbins_pt=150, nbins_p=150, nbins_eta=150, nbins_phi=150, nbins_dcaxy=800, nbins_dcaz=800, nbins_nvertices=20;
    
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);
    
    
    TF1 fgaus_dcaxy("fgaus_dcaxy","gaus",-dcaxy_onesiderange_formeanwidthcalc, dcaxy_onesiderange_formeanwidthcalc);
    TF1 fgaus_dcaz("fgaus_dcaz","gaus",-dcaz_onesiderange_formeanwidthcalc, dcaz_onesiderange_formeanwidthcalc);
    TF1 fgaus_deltavertex_vs_nTracks(
        "fgaus_deltavertex_vs_nTracks", "gaus",
        -deltavertex_onesiderange_formeanwidthcalc,
        deltavertex_onesiderange_formeanwidthcalc
      );

      TF1 fgaus_deltavertex_vs_nwotrackcutsTracks(
        "fgaus_deltavertex_vs_nwotrackcutsTracks", "gaus",
        -deltavertex_onesiderange_formeanwidthcalc,
        deltavertex_onesiderange_formeanwidthcalc
      );
    
    if (resid_base->GetEntries() == 0) {
      std::cerr << "No entries found in residualtree." << std::endl;
      return;
    }
    
    resid_base->LoadTree(0);
      TTree* first_tree = resid_base->GetTree();
    
    TTreeReader reader(resid_base);
    TTreeReaderValue<Float_t> pcax(reader,"pcax");
    TTreeReaderValue<Float_t> pcay(reader,"pcay");
    TTreeReaderValue<Float_t> pcaz(reader,"pcaz");
    TTreeReaderValue<Float_t> dcaxy(reader,"dcaxy");
    TTreeReaderValue<Float_t> dcaz(reader,"dcaz");
    TTreeReaderValue<Float_t> px(reader,"px");
    TTreeReaderValue<Float_t> py(reader,"py");
    TTreeReaderValue<Float_t> pz(reader,"pz");
    TTreeReaderValue<Float_t> pt(reader,"pt");
    TTreeReaderValue<Float_t> eta(reader,"eta");
    TTreeReaderValue<Float_t> phi(reader,"phi");
    TTreeReaderValue<Int_t> nmapsstate(reader,"nmapsstate");
    TTreeReaderValue<Int_t> nmaps(reader,"nmaps");
    TTreeReaderValue<Int_t> nintt(reader,"nintt");
    TTreeReaderValue<Int_t> ntpc(reader,"ntpc");
    TTreeReaderValue<Int_t> vertex_ntracks(reader,"vertex_ntracks");
    TTreeReaderValue<Int_t> run(reader,"run");
    TTreeReaderValue<Int_t> segment(reader,"segment");
    TTreeReaderValue<Int_t> event(reader,"event");
    TTreeReaderValue<Int_t> crossing(reader,"crossing");
    TTreeReaderValue<Float_t> vx(reader,"vx");
    TTreeReaderValue<Float_t> vy(reader,"vy");
    TTreeReaderValue<Float_t> vz(reader,"vz");
    TTreeReaderValue<UInt_t> vertexid(reader, "vertexid");
    TTreeReaderValue<Float_t>* true_vx = nullptr;
    TTreeReaderValue<Float_t>* true_vy = nullptr;
    TTreeReaderValue<Float_t>* true_vz = nullptr;
    TTreeReaderValue<Float_t>* chisq = nullptr;
    TTreeReaderValue<Float_t>* ndf = nullptr;
    
    if (is_simulation_input)
    {
        true_vx = new TTreeReaderValue<Float_t>(reader, "true_vx");
        true_vy = new TTreeReaderValue<Float_t>(reader, "true_vy");
        true_vz = new TTreeReaderValue<Float_t>(reader, "true_vz");
        chisq = new TTreeReaderValue<Float_t>(reader, "chisq");
        ndf = new TTreeReaderValue<Float_t>(reader, "ndf");
    }
    
    
    TH2D *h_two_track_pt_dcaxy = new TH2D("h_two_track_pt_dcaxy","h_two_track_pt_dcaxy", nbins_pt, pt_min, pt_max, nbins_dcaxy,dcaxy_min,dcaxy_max);
    h_two_track_pt_dcaxy->SetTitle("Two track dcaxy vs p_{T};(p_{T,1}+p_{T,2})/2;two_track_dcaxy");
    TH2D *h_two_track_pt_dcaz = new TH2D("h_two_track_pt_dcaz","h_two_track_pt_dcaz", nbins_pt, pt_min, pt_max, nbins_dcaz,dcaz_min,dcaz_max);
    h_two_track_pt_dcaz->SetTitle("Two track dcaz vs p_{T};(p_{T,1}+p_{T,2})/2;two_track_dcaz");
    
    TH2D *h_two_track_p_dcaxy = new TH2D("h_two_track_p_dcaxy","h_two_track_p_dcaxy", nbins_p, p_min, p_max, nbins_dcaxy,dcaxy_min,dcaxy_max);
    h_two_track_p_dcaxy->SetTitle("Two track dcaxy vs p;(p_{1}+p_{2})/2;two_track_dcaxy");
    TH2D *h_two_track_p_dcaz = new TH2D("h_two_track_p_dcaz","h_two_track_p_dcaz", nbins_p, p_min, p_max, nbins_dcaz,dcaz_min,dcaz_max);
    h_two_track_p_dcaz->SetTitle("Two track dcaz vs p;(p_{1}+p_{2})/2;two_track_dcaz");
    
    //TH2D *h_vtx_pt_dcaxy = new TH2D("h_vtx_pt_dcaxy","h_vtx_pt_dcaxy", 100, 0.0, 2.0, 100, -0.005,0.005);
    //TH2D *h_vtx_pt_dcaz = new TH2D("h_vtx_pt_dcaz","h_vtx_pt_dcaz", 100, 0.0, 2.0, 100, -0.005,0.005);
    
    TH2D *h_two_track_eta_dcaxy = new TH2D("h_two_track_eta_dcaxy","h_two_track_eta_dcaxy", nbins_eta, eta_min,eta_max, nbins_dcaxy,dcaxy_min,dcaxy_max);
    h_two_track_eta_dcaxy->SetTitle("Two_track_eta_dcaxy;#eta_{1}-#eta_{2};two_track_dcaxy");
    
    TH2D *h_two_track_phi_dcaxy = new TH2D("h_two_track_phi_dcaxy","h_two_track_phi_dcaxy", nbins_phi, phi_min,phi_max, nbins_dcaxy,dcaxy_min,dcaxy_max);
    h_two_track_phi_dcaxy->SetTitle("Two_track_phi_dcaxy;#phi_{1}-#phi_{2};two_track_dcaxy");
    
    TH3D *h_pcax_pcay_pt = new TH3D("h_pcax_pcay_pt", "PCA y vs PCA x;pca_{x} (cm);pca_{y} (cm);p_{T} (GeV/c)", nbins_pcaxy,
      pcaxy_min, pcaxy_max, nbins_pcaxy, pcaxy_min, pcaxy_max,nbins_pt, pt_min, pt_max);

    TH2D *h_vx_vy = new TH2D("h_vx_vy", "Reco vertex y vs x;v_{x} (cm);v_{y} (cm)", nbins_vxy, vxy_min,
      vxy_max, nbins_vxy, vxy_min, vxy_max);
    
    TH2D *h_two_track_pt_corr = new TH2D("h_two_track_pt_corr","h_two_track_pt_corr", nbins_pt, pt_min, pt_max, nbins_pt, pt_min, pt_max);
    h_two_track_pt_corr->SetTitle("Two_track_pt_corr before p selection;p_{T,1};p_{T,2}");
    
    TH2D *h_nmapsstate_phi = new TH2D("h_nmapsstate_phi","nmapsstate vs phi;#phi;nmapsstate", nbins_phi,phi_min,phi_max, 8, -0.5, 7.5);
    
    TH2D *h_fractrackssatisfyingcuts = new TH2D("h_fractrackssatisfyingcuts",Form("h_fractrackssatisfyingcuts: %s;tracks passing vertex-level cuts;100 #times selected/all (per event)", trackcuts_info), 31,-0.5,30.5, 202,-0.5,100.5);
    
    TH1I* h_nvertices = new TH1I("h_nvertices",
                                "Number of vertices passing vertex level cuts (per event);N_{vertices};Events",
                                nbins_nvertices, nvertices_min, nvertices_max);
    
    TH1I* h_vertex_ntracks = new TH1I("h_vertex_ntracks", "vertex_ntracks (TrackFitter cuts) for each reconstructed vertex;N_{tracks}^{vertex};Vertices", 101, -0.5, 100.5);
    
    TH1I* h_vertex_wocuts_ntracks = new TH1I("h_vertex_wocuts_ntracks", "Tracks wocuts associated with each reconstructed vertex;N_{tracks}^{vertex};Vertices", 101, -0.5, 100.5);

    TH1I* h_vertex_selected_ntracks = new TH1I("h_vertex_selected_ntracks", "Selected tracks per reconstructed vertex;N_{tracks}^{selected};Vertices", 101, -0.5, 100.5);
    
    TH3D *h_etadiff_two_track_dcaxy_pt = new TH3D("h_etadiff_two_track_dcaxy_pt","h_etadiff_two_track_dcaxy_pt", nbins_eta, eta_min,eta_max, nbins_dcaxy, dcaxy_min,dcaxy_max, 10, pt_min,pt_max);
    h_etadiff_two_track_dcaxy_pt->SetTitle("h_etadiff_two_track_dcaxy_pt;#eta_{1}-#eta_{2};two_track_dcaxy;p_{T}");
    
    TH3D *h_etadiff_two_track_dcaz_pt = new TH3D("h_etadiff_two_track_dcaz_pt","h_etadiff_two_track_dcaz_pt", nbins_eta, eta_min,eta_max, nbins_dcaz, dcaz_min,dcaz_max, 10, pt_min,pt_max);
    h_etadiff_two_track_dcaz_pt->SetTitle("h_etadiff_two_track_dcaz_pt;#eta_{1}-#eta_{2};two_track_dcaz;p_{T}");
    
    
    TH3D *h_phidiff_two_track_dcaxy_pt = new TH3D("h_phidiff_two_track_dcaxy_pt","h_phidiff_two_track_dcaxy_pt", nbins_phi, phi_min,phi_max, nbins_dcaxy, dcaxy_min,dcaxy_max, 10, pt_min,pt_max);
    h_phidiff_two_track_dcaxy_pt->SetTitle("h_phidiff_two_track_dcaxy_pt;#phi_{1}-#phi_{2};two_track_dcaxy;p_{T}");
    
    TH3D *h_phidiff_two_track_dcaz_pt = new TH3D("h_phidiff_two_track_dcaz_pt","h_phidiff_two_track_dcaz_pt", nbins_phi, phi_min,phi_max, nbins_dcaz, dcaz_min,dcaz_max, 10, pt_min,pt_max);
    h_phidiff_two_track_dcaz_pt->SetTitle("h_phidiff_two_track_dcaz_pt;#phi_{1}-#phi_{2};two_track_dcaz;p_{T}");
    
    TH3D *h_eta1_eta2_two_track_dcaxy = new TH3D("h_eta1_eta2_two_track_dcaxy","h_eta1_eta2_two_track_dcaxy", nbins_eta, eta_min,eta_max, nbins_eta, eta_min,eta_max, nbins_dcaxy, dcaxy_min,dcaxy_max);
    h_eta1_eta2_two_track_dcaxy->SetTitle("h_eta1_eta2_two_track_dcaxy;#eta_{1}; #eta_{2};two_track_dcaxy");
    
    TH3D *h_eta1_eta2_two_track_dcaz = new TH3D("h_eta1_eta2_two_track_dcaz","h_eta1_eta2_two_track_dcaz", nbins_eta, eta_min,eta_max, nbins_eta, eta_min,eta_max, nbins_dcaz, dcaz_min,dcaz_max);
    h_eta1_eta2_two_track_dcaz->SetTitle("h_eta1_eta2_two_track_dcaz;#eta_{1}; #eta_{2};two_track_dcaz");
    
    TH3D *h_phi1_phi2_two_track_dcaxy = new TH3D("h_phi1_phi2_two_track_dcaxy","h_phi1_phi2_two_track_dcaxy", nbins_phi, phi_min,phi_max, nbins_phi, phi_min,phi_max, nbins_dcaxy, dcaxy_min,dcaxy_max);
    h_phi1_phi2_two_track_dcaxy->SetTitle("h_phi1_phi2_two_track_dcaxy;#phi_{1}; #phi_{2};two_track_dcaxy");
    
    TH3D *h_phi1_phi2_two_track_dcaz = new TH3D("h_phi1_phi2_two_track_dcaz","h_phi1_phi2_two_track_dcaz", nbins_phi, phi_min,phi_max, nbins_phi, phi_min,phi_max, nbins_dcaz, dcaz_min,dcaz_max);
    h_phi1_phi2_two_track_dcaz->SetTitle("h_phi1_phi2_two_track_dcaz;#phi_{1}; #phi_{2};two_track_dcaz");
    
    TH3D *h_etadiff_phidiff_two_track_dcaxy = new TH3D("h_etadiff_phidiff_two_track_dcaxy","h_etadiff_phidiff_two_track_dcaxy", nbins_eta, eta_min,eta_max, nbins_phi, phi_min,phi_max, nbins_dcaxy, dcaxy_min,dcaxy_max);
    h_etadiff_phidiff_two_track_dcaxy->SetTitle("h_etadiff_phidiff_two_track_dcaxy;#eta_{1}-#eta_{2}; #phi_{1}-#phi_{2};two_track_dcaxy");
    
    TH3D *h_etadiff_phidiff_two_track_dcaz = new TH3D("h_etadiff_phidiff_two_track_dcaz","h_etadiff_phidiff_two_track_dcaz", nbins_eta, eta_min,eta_max, nbins_phi, phi_min,phi_max, nbins_dcaz, dcaz_min,dcaz_max);
    h_etadiff_phidiff_two_track_dcaz->SetTitle("h_etadiff_phidiff_two_track_dcaz;#eta_{1}-#eta_{2}; #phi_{1}-#phi_{2};two_track_dcaz");
    
    TH3D *h_etadiff_phidiff_two_track_dcaxy3d = new TH3D("h_etadiff_phidiff_two_track_dcaxy3d","h_etadiff_phidiff_two_track_dcaxy3d", nbins_eta, eta_min,eta_max, nbins_phi, phi_min,phi_max, nbins_dcaxy, dcaxy_min,dcaxy_max);
    h_etadiff_phidiff_two_track_dcaxy3d->SetTitle("h_etadiff_phidiff_two_track_dcaxy3d;#eta_{1}-#eta_{2}; #phi_{1}-#phi_{2};two_track_dcaxy");
    
    TH3D *h_etadiff_phidiff_two_track_dcaz3d = new TH3D("h_etadiff_phidiff_two_track_dcaz3d","h_etadiff_phidiff_two_track_dcaz3d", nbins_eta, eta_min,eta_max, nbins_phi, phi_min,phi_max, nbins_dcaz, dcaz_min,dcaz_max);
    h_etadiff_phidiff_two_track_dcaz3d->SetTitle("h_etadiff_phidiff_two_track_dcaz3d;#eta_{1}-#eta_{2}; #phi_{1}-#phi_{2};two_track_dcaz");
    
    
    
    TH2D *h_dcaxy1_dcaxy2_diffvs_2trackdcaxy = new TH2D("h_dcaxy1_dcaxy2_diffvs_2trackdcaxy","h_dcaxy1_dcaxy2_diffvs_2trackdcaxy", 100, -0.02, 0.02, 50, -0.01, 0.01);
    h_dcaxy1_dcaxy2_diffvs_2trackdcaxy->SetTitle("h_dcaxy1_dcaxy2_diffvs_2trackdcaxy;dcaxy_{1}-dcaxy_{2};twotrack_dcaxy");
    
    TH2D *h_dcaz1_dcaz2_diffvs_2trackdcaz = new TH2D("h_dcaz1_dcaz2_diffvs_2trackdcaz","h_dcaz1_dcaz2_diffvs_2trackdcaz", 100, -0.02, 0.02, 50, -0.01, 0.01);
    h_dcaz1_dcaz2_diffvs_2trackdcaz->SetTitle("h_dcaz1_dcaz2_diffvs_2trackdcaz;dcaz_{1}-dcaz_{2};twotrack_dcaz");
    
    TH2D *h_truevx_truevy = nullptr;

      TH2D *h_deltavxy_vs_nTracks = nullptr;
      TH2D *h_deltavx_vs_nTracks = nullptr;
      TH2D *h_deltavy_vs_nTracks = nullptr;
      TH2D *h_deltavz_vs_nTracks = nullptr;

      TH2D *h_deltavxy_vs_nwotrackcutsTracks = nullptr;
      TH2D *h_deltavx_vs_nwotrackcutsTracks = nullptr;
      TH2D *h_deltavy_vs_nwotrackcutsTracks = nullptr;
      TH2D *h_deltavz_vs_nwotrackcutsTracks = nullptr;

      if (is_simulation_input)
      {
        h_truevx_truevy = new TH2D(
          "h_truevx_truevy",
          "Truth vertex y vs x;true v_{x} (cm);true v_{y} (cm)",
          nbins_vxy, vxy_min, vxy_max,
          nbins_vxy, vxy_min, vxy_max
        );

        h_deltavxy_vs_nTracks = new TH2D(
          "h_deltavxy_vs_nTracks",
          "#Delta v_{xy} vs nTracks;nTracks of vertex passing selection cuts;#Delta v_{xy} (cm)",
          65, -0.5, 65.5, 100, -0.008, 0.008
        );

        h_deltavx_vs_nTracks = new TH2D(
          "h_deltavx_vs_nTracks",
          "#Delta v_{x} vs nTracks;nTracks of vertex passing selection cuts;#Delta v_{x} (cm)",
          65, -0.5, 65.5, 100, -0.008, 0.008
        );

        h_deltavy_vs_nTracks = new TH2D(
          "h_deltavy_vs_nTracks",
          "#Delta v_{y} vs nTracks;nTracks of vertex passing selection cuts;#Delta v_{y} (cm)",
          65, -0.5, 65.5, 100, -0.008, 0.008
        );

        h_deltavz_vs_nTracks = new TH2D(
          "h_deltavz_vs_nTracks",
          "#Delta v_{z} vs nTracks;nTracks of vertex passing selection cuts;#Delta v_{z} (cm)",
          65, -0.5, 65.5, 100, -0.008, 0.008
        );

        h_deltavxy_vs_nwotrackcutsTracks = new TH2D(
          "h_deltavxy_vs_nwotrackcutsTracks",
          "#Delta v_{xy} vs nwotrackcutsTracks;tracks passing vertex-level cuts before track-level cuts;#Delta v_{xy} (cm)",
          65, -0.5, 65.5, 100, -0.008, 0.008
        );

        h_deltavx_vs_nwotrackcutsTracks = new TH2D(
          "h_deltavx_vs_nwotrackcutsTracks",
          "#Delta v_{x} vs nwotrackcutsTracks;tracks before selection cuts;#Delta v_{x} (cm)",
          65, -0.5, 65.5, 100, -0.008, 0.008
        );

        h_deltavy_vs_nwotrackcutsTracks = new TH2D(
          "h_deltavy_vs_nwotrackcutsTracks",
          "#Delta v_{y} vs nwotrackcutsTracks;tracks before selection cuts;#Delta v_{y} (cm)",
          65, -0.5, 65.5, 100, -0.008, 0.008
        );

        h_deltavz_vs_nwotrackcutsTracks = new TH2D(
          "h_deltavz_vs_nwotrackcutsTracks",
          "#Delta v_{z} vs nwotrackcutsTracks;tracks before selection cuts;#Delta v_{z} (cm)",
          65, -0.5, 65.5, 100, -0.008, 0.008
        );
      }

    bool have_event = false;
    int run_cur = -1, segment_cur = -1, event_cur = -1, nwotrackcutsTracks=0;
    
    std::map<int, std::vector<float>> pcax_map;
    std::map<int, std::vector<float>> pcay_map;
    std::map<int, std::vector<float>> pcaz_map;
    std::map<int, std::vector<float>> px_map;
    std::map<int, std::vector<float>> py_map;
    std::map<int, std::vector<float>> pz_map;
    std::map<int, std::vector<float>> vx_map;
    std::map<int, std::vector<float>> vy_map;
    std::map<int, std::vector<float>> vz_map;
    std::map<int, std::vector<float>> eta_map;
    std::map<int, std::vector<float>> phi_map;
    std::map<int, std::vector<float>> dcaxy_map;
    std::map<int, std::vector<float>> dcaz_map;
    std::vector<float> two_track_dcaxy_vec;
    std::vector<float> two_track_dcaz_vec;
    std::vector<float> two_track_ptavg_vec;
    std::vector<float> two_track_pavg_vec;
    std::vector<float> two_track_etadiff_vec;
    std::vector<float> two_track_phidiff_vec;
    
    struct VertexInfo
      {
        ROOT::Math::XYZVectorF reco_vertex;
        ROOT::Math::XYZVectorF truth_vertex;
        int associated_ntracks = 0; // from reconstruction: vertex->size_tracks()
        int wocuts_ntracks = 0;     //tracks passing vertex level cuts without filtering for track level cuts
        int selected_ntracks = 0;   // tracks passing this macro's cuts
      };

      std::map<unsigned int, VertexInfo> vertex_map;
    
    auto FlushEvent = [&]() -> void
    {
        if (!have_event){ return;}

        int nTracks = 0;
        for (const auto& [cross_key, vec] : pcax_map){
            nTracks += vec.size();
        }
        if (nwotrackcutsTracks > 0){ h_fractrackssatisfyingcuts->Fill(nwotrackcutsTracks, 100.0*double(nTracks)/double(nwotrackcutsTracks));}
        
        if (!vertex_map.empty())
        {
            int nvertices = static_cast<int>(vertex_map.size());
            h_nvertices->Fill(nvertices);

            for (const auto& vertex_entry : vertex_map)
            {
              const auto& vtx_info = vertex_entry.second;
              h_vertex_ntracks->Fill(vtx_info.associated_ntracks);
              h_vertex_wocuts_ntracks->Fill(vtx_info.wocuts_ntracks);
              h_vertex_selected_ntracks->Fill(vtx_info.selected_ntracks);
              h_vx_vy->Fill(vtx_info.reco_vertex.X(), vtx_info.reco_vertex.Y());
                
              if (is_simulation_input){
                    h_truevx_truevy->Fill(vtx_info.truth_vertex.X(), vtx_info.truth_vertex.Y());
                    
                    ROOT::Math::XYZVectorF delta_vertex =
                    vtx_info.reco_vertex - vtx_info.truth_vertex;
                    
                    h_deltavxy_vs_nTracks->Fill(vtx_info.selected_ntracks, delta_vertex.Rho());
                    h_deltavx_vs_nTracks->Fill(vtx_info.selected_ntracks, delta_vertex.X());
                    h_deltavy_vs_nTracks->Fill(vtx_info.selected_ntracks, delta_vertex.Y());
                    h_deltavz_vs_nTracks->Fill(vtx_info.selected_ntracks, delta_vertex.Z());
                    
                    
                    h_deltavxy_vs_nwotrackcutsTracks->Fill(vtx_info.wocuts_ntracks, delta_vertex.Rho());
                    h_deltavx_vs_nwotrackcutsTracks->Fill(vtx_info.wocuts_ntracks, delta_vertex.X());
                    h_deltavy_vs_nwotrackcutsTracks->Fill(vtx_info.wocuts_ntracks, delta_vertex.Y());
                    h_deltavz_vs_nwotrackcutsTracks->Fill(vtx_info.wocuts_ntracks, delta_vertex.Z());
              }
            }
        }
        
        if(!pcax_map.empty())
        {
          for (const auto& [cross_key, value] : pcax_map)
          {
            two_track_dcaxy_vec.clear();
            two_track_dcaz_vec.clear();
            two_track_ptavg_vec.clear();
            two_track_pavg_vec.clear();
            two_track_etadiff_vec.clear();
            two_track_phidiff_vec.clear();
            
            compute_twoTrackDca(pcax_map.at(cross_key), pcay_map.at(cross_key), pcaz_map.at(cross_key),
                                px_map.at(cross_key), py_map.at(cross_key), pz_map.at(cross_key),eta_map.at(cross_key), phi_map.at(cross_key),dcaxy_map.at(cross_key),dcaz_map.at(cross_key),
                                two_track_dcaxy_vec, two_track_dcaz_vec,
                                two_track_ptavg_vec, two_track_pavg_vec, two_track_etadiff_vec,
                                two_track_phidiff_vec, h_two_track_pt_corr,
                                h_etadiff_two_track_dcaxy_pt, h_etadiff_two_track_dcaz_pt, h_phidiff_two_track_dcaxy_pt, h_phidiff_two_track_dcaz_pt, h_eta1_eta2_two_track_dcaxy, h_eta1_eta2_two_track_dcaz, h_phi1_phi2_two_track_dcaxy, h_phi1_phi2_two_track_dcaz, h_etadiff_phidiff_two_track_dcaxy, h_etadiff_phidiff_two_track_dcaz,
                                    h_etadiff_phidiff_two_track_dcaxy3d, h_etadiff_phidiff_two_track_dcaz3d,
                                h_dcaxy1_dcaxy2_diffvs_2trackdcaxy,h_dcaz1_dcaz2_diffvs_2trackdcaz);

            for (size_t ii = 0; ii < two_track_dcaxy_vec.size(); ++ii)
            {
                h_two_track_pt_dcaxy->Fill(two_track_ptavg_vec[ii], two_track_dcaxy_vec[ii]);
                h_two_track_eta_dcaxy->Fill(two_track_etadiff_vec[ii], two_track_dcaxy_vec[ii]);
                h_two_track_phi_dcaxy->Fill(two_track_phidiff_vec[ii], two_track_dcaxy_vec[ii]);
                h_two_track_pt_dcaz->Fill(two_track_ptavg_vec[ii], two_track_dcaz_vec[ii]);
                h_two_track_p_dcaxy->Fill(two_track_pavg_vec[ii], two_track_dcaxy_vec[ii]);
                h_two_track_p_dcaz->Fill(two_track_pavg_vec[ii], two_track_dcaz_vec[ii]);
            }
            
          }
            
        }
        pcax_map.clear();
        pcay_map.clear();
        pcaz_map.clear();
        px_map.clear();
        py_map.clear();
        pz_map.clear();
        vx_map.clear();
        vy_map.clear();
        vz_map.clear();
        eta_map.clear();
        phi_map.clear();
        dcaxy_map.clear();
        dcaz_map.clear();
        vertex_map.clear();
        
        
        nwotrackcutsTracks=0;
        have_event = false;
    };
    
    while (reader.Next())
    {
        if (!have_event)
        {
            run_cur = *run;
            segment_cur = *segment;
            event_cur = *event;
            have_event = true;
        }
        
        //else if (event_cur != *event)
        else if (run_cur != *run || segment_cur != *segment || event_cur != *event)
        {
            FlushEvent();
            run_cur = *run;
            segment_cur = *segment;
            event_cur = *event;
            have_event = true;
        }
        
        if(*vertex_ntracks <= vertex_ntracks_cut||std::fabs(*vx)<fabs_vx_cut){continue;} //event level cuts should be applied to all tracks
        
        nwotrackcutsTracks=nwotrackcutsTracks+1;
        
        ROOT::Math::XYZVectorF truth_vertex={std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()};

          if (is_simulation_input)
          {
            truth_vertex = ROOT::Math::XYZVectorF(**true_vx, **true_vy, **true_vz);
          }

        
        auto vertex_entry = vertex_map.emplace(
            *vertexid,
            VertexInfo{ROOT::Math::XYZVectorF(*vx, *vy, *vz),
                truth_vertex,
                *vertex_ntracks, 0, 0}
          );
        auto it = vertex_entry.first;
        
        it->second.wocuts_ntracks++;
        
        //if(*nmapsstate < 2 || *nintt < 1 || *pt < 0.2) continue;
        //if(*nmaps<=1 || *nintt<=0 || *ntpc<=25 || *pt <= 0.2){ continue;}
        //if(*nmaps<=1 || *nintt<=0 || *ntpc<=25 || *vertex_ntracks <= 3){ continue;}
        
        bool pass_chisq_ndf = true;
        float chisq_ndf=0.0;
       
        if (is_simulation_input)
        {
          chisq_ndf = ((**ndf) > 0.0) ? ((**chisq) / (**ndf)) : 0.0;
          std::numeric_limits<float>::infinity();
          pass_chisq_ndf = (chisq_ndf < chisq_ndf_cut);
        }
        
        if( (*nmaps<nmaps_cut) || (*nintt<intt_cut) || (*ntpc < ntpc_cut)|| (*pt<pt_cut) || !pass_chisq_ndf) {continue;}
        
        //std::cout<<"event: "<<*event<<" event cut="<<event_cur<<std::endl;
        
        it->second.selected_ntracks++;

        h_nmapsstate_phi->Fill(*phi,*nmapsstate);
        h_pcax_pcay_pt->Fill(*pcax, *pcay,*pt);

        pcax_map[*crossing].push_back(*pcax);
        pcay_map[*crossing].push_back(*pcay);
        pcaz_map[*crossing].push_back(*pcaz);
        px_map[*crossing].push_back(*px);
        py_map[*crossing].push_back(*py);
        pz_map[*crossing].push_back(*pz);
        vx_map[*crossing].push_back(*vx);
        vy_map[*crossing].push_back(*vy);
        vz_map[*crossing].push_back(*vz);
        eta_map[*crossing].push_back(*eta);
        phi_map[*crossing].push_back(*phi);
        dcaxy_map[*crossing].push_back(*dcaxy);
        dcaz_map[*crossing].push_back(*dcaz);
    }
    
    FlushEvent();
    
    //Mean and width of two_track_pt_dcaxy
    TH1D* h_two_track_dcaxy_Mean_vs_pt=nullptr; TH1D* h_two_track_dcaxy_Width_vs_pt=nullptr; TH1D* h_two_track_dcaz_Mean_vs_pt=nullptr; TH1D* h_two_track_dcaz_Width_vs_pt=nullptr;
    
    TH1D* h_two_track_dcaxy_Mode_vs_pt=nullptr; TH1D* h_two_track_dcaxy_FWHM_vs_pt=nullptr; TH1D* h_two_track_dcaxy_RMS_vs_pt=nullptr; TH1D* h_two_track_dcaz_Mode_vs_pt=nullptr; TH1D* h_two_track_dcaz_FWHM_vs_pt=nullptr; TH1D* h_two_track_dcaz_RMS_vs_pt=nullptr;
    
    TH1D* h_two_track_dcaxy_Mean_vs_p=nullptr; TH1D* h_two_track_dcaxy_Width_vs_p=nullptr; TH1D* h_two_track_dcaz_Mean_vs_p=nullptr; TH1D* h_two_track_dcaz_Width_vs_p=nullptr;
    
    TH1D* h_two_track_dcaxy_Mode_vs_p=nullptr; TH1D* h_two_track_dcaxy_FWHM_vs_p=nullptr; TH1D* h_two_track_dcaxy_RMS_vs_p=nullptr; TH1D* h_two_track_dcaz_Mode_vs_p=nullptr; TH1D* h_two_track_dcaz_FWHM_vs_p=nullptr; TH1D* h_two_track_dcaz_RMS_vs_p=nullptr;
    
    TH1D *h_deltavxy_Mean_vs_nTracks = nullptr, *h_deltavxy_Width_vs_nTracks = nullptr;
    TH1D *h_deltavx_Mean_vs_nTracks = nullptr, *h_deltavx_Width_vs_nTracks = nullptr;
    TH1D *h_deltavy_Mean_vs_nTracks = nullptr, *h_deltavy_Width_vs_nTracks = nullptr;
    TH1D *h_deltavz_Mean_vs_nTracks = nullptr, *h_deltavz_Width_vs_nTracks = nullptr;

    TH1D *h_deltavxy_Mean_vs_nwotrackcutsTracks = nullptr, *h_deltavxy_Width_vs_nwotrackcutsTracks = nullptr;
    TH1D *h_deltavx_Mean_vs_nwotrackcutsTracks = nullptr, *h_deltavx_Width_vs_nwotrackcutsTracks = nullptr;
    TH1D *h_deltavy_Mean_vs_nwotrackcutsTracks = nullptr, *h_deltavy_Width_vs_nwotrackcutsTracks = nullptr;
    TH1D *h_deltavz_Mean_vs_nwotrackcutsTracks = nullptr, *h_deltavz_Width_vs_nwotrackcutsTracks = nullptr;
    
    TH2D *h_eta1_eta2_two_track_dcaxy_Mean = nullptr, *h_eta1_eta2_two_track_dcaz_Mean = nullptr;
    TH2D *h_eta1_eta2_two_track_dcaxy_Width = nullptr, *h_eta1_eta2_two_track_dcaz_Width = nullptr;
    
    TH2D *h_eta1_eta2_two_track_dcaxy_Mode = nullptr, *h_eta1_eta2_two_track_dcaz_Mode = nullptr;
    TH2D *h_eta1_eta2_two_track_dcaxy_FWHM = nullptr, *h_eta1_eta2_two_track_dcaz_FWHM = nullptr;
    TH2D *h_eta1_eta2_two_track_dcaxy_RMS = nullptr, *h_eta1_eta2_two_track_dcaz_RMS = nullptr;
    
    
    TH2D *h_phi1_phi2_two_track_dcaxy_Mean = nullptr, *h_phi1_phi2_two_track_dcaz_Mean = nullptr;
    TH2D *h_phi1_phi2_two_track_dcaxy_Width = nullptr, *h_phi1_phi2_two_track_dcaz_Width = nullptr;
    
    
    TH2D *h_phi1_phi2_two_track_dcaxy_Mode = nullptr, *h_phi1_phi2_two_track_dcaz_Mode = nullptr;
    TH2D *h_phi1_phi2_two_track_dcaxy_FWHM = nullptr, *h_phi1_phi2_two_track_dcaz_FWHM = nullptr;
    TH2D *h_phi1_phi2_two_track_dcaxy_RMS = nullptr, *h_phi1_phi2_two_track_dcaz_RMS = nullptr;
    
    TH2D *h_etadiff_phidiff_two_track_dcaxy_Mean = nullptr, *h_etadiff_phidiff_two_track_dcaz_Mean = nullptr;
    TH2D *h_etadiff_phidiff_two_track_dcaxy_Width = nullptr, *h_etadiff_phidiff_two_track_dcaz_Width = nullptr;
    
    TH2D *h_etadiff_phidiff_two_track_dcaxy_Mode = nullptr, *h_etadiff_phidiff_two_track_dcaz_Mode = nullptr;
    TH2D *h_etadiff_phidiff_two_track_dcaxy_FWHM = nullptr, *h_etadiff_phidiff_two_track_dcaz_FWHM = nullptr;
    TH2D *h_etadiff_phidiff_two_track_dcaxy_RMS = nullptr, *h_etadiff_phidiff_two_track_dcaz_RMS = nullptr;
    
    TH2D *h_etadiff_phidiff_two_track_dcaxy3d_Mean = nullptr, *h_etadiff_phidiff_two_track_dcaz3d_Mean = nullptr;
    TH2D *h_etadiff_phidiff_two_track_dcaxy3d_Width = nullptr, *h_etadiff_phidiff_two_track_dcaz3d_Width = nullptr;
    
    TH2D *h_etadiff_phidiff_two_track_dcaxy3d_Mode = nullptr, *h_etadiff_phidiff_two_track_dcaz3d_Mode = nullptr;
    TH2D *h_etadiff_phidiff_two_track_dcaxy3d_FWHM = nullptr, *h_etadiff_phidiff_two_track_dcaz3d_FWHM = nullptr;
    TH2D *h_etadiff_phidiff_two_track_dcaxy3d_RMS = nullptr, *h_etadiff_phidiff_two_track_dcaz3d_RMS = nullptr;
    
    GetFitSlicesY(h_two_track_pt_dcaxy,&fgaus_dcaxy,h_two_track_dcaxy_Mean_vs_pt,h_two_track_dcaxy_Width_vs_pt,"Two_track_pt_dcaxy","(p_{T,1}+p_{T,2})/2","two_track_dcaxy");
    GetFitSlicesY(h_two_track_pt_dcaz,&fgaus_dcaz,h_two_track_dcaz_Mean_vs_pt,h_two_track_dcaz_Width_vs_pt,"Two_track_pt_dcaz","(p_{T,1}+p_{T,2})/2","two_track_dcaz");
    
    GetModeFWHMRMS_Y(h_two_track_pt_dcaxy,h_two_track_dcaxy_Mode_vs_pt,h_two_track_dcaxy_FWHM_vs_pt,h_two_track_dcaxy_RMS_vs_pt,3,"Two_track_pt_dcaxy","(p_{T,1}+p_{T,2})/2","two_track_dcaxy");
    GetModeFWHMRMS_Y(h_two_track_pt_dcaz,h_two_track_dcaz_Mode_vs_pt,h_two_track_dcaz_FWHM_vs_pt,h_two_track_dcaz_RMS_vs_pt,3,"Two_track_pt_dcaz","(p_{T,1}+p_{T,2})/2","two_track_dcaz");
    
    GetFitSlicesY(h_two_track_p_dcaxy,&fgaus_dcaxy,h_two_track_dcaxy_Mean_vs_p,h_two_track_dcaxy_Width_vs_p,"Two_track_p_dcaxy","(p_{1}+p_{2})/2","two_track_dcaxy");
    GetFitSlicesY(h_two_track_p_dcaz,&fgaus_dcaz,h_two_track_dcaz_Mean_vs_p,h_two_track_dcaz_Width_vs_p,"Two_track_p_dcaz","(p_{1}+p_{2})/2","two_track_dcaz");
    
    GetModeFWHMRMS_Y(h_two_track_p_dcaxy,h_two_track_dcaxy_Mode_vs_p,h_two_track_dcaxy_FWHM_vs_p,h_two_track_dcaxy_RMS_vs_p,3,"Two_track_p_dcaxy","(p_{1}+p_{2})/2","two_track_dcaxy");
    GetModeFWHMRMS_Y(h_two_track_p_dcaz,h_two_track_dcaz_Mode_vs_p,h_two_track_dcaz_FWHM_vs_p,h_two_track_dcaz_RMS_vs_p,3,"Two_track_p_dcaz","(p_{1}+p_{2})/2","two_track_dcaz");
    
    FitTH3ZSlices(h_eta1_eta2_two_track_dcaxy, &fgaus_dcaxy,
                  h_eta1_eta2_two_track_dcaxy_Mean, h_eta1_eta2_two_track_dcaxy_Width,
                          /*par_mean=*/1, /*par_width=*/2,
                          1, "QSRN", /*rebin_x=*/int(nbins_eta/15), /*rebin_y=*/int(nbins_eta/15), /*rebin_z=*/4);
    
    FitTH3ZSlices(h_eta1_eta2_two_track_dcaz, &fgaus_dcaz,
                  h_eta1_eta2_two_track_dcaz_Mean, h_eta1_eta2_two_track_dcaz_Width,
                          /*par_mean=*/1, /*par_width=*/2,
                          1, "QSRN", /*rebin_x=*/int(nbins_eta/15), /*rebin_y=*/int(nbins_eta/15), /*rebin_z=*/4);
    
    FitTH3ZSlices(h_phi1_phi2_two_track_dcaxy, &fgaus_dcaxy,
                  h_phi1_phi2_two_track_dcaxy_Mean, h_phi1_phi2_two_track_dcaxy_Width,
                          /*par_mean=*/1, /*par_width=*/2,
                          1, "QSRN", /*rebin_x=*/int(nbins_phi/10), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/4);
    
    FitTH3ZSlices(h_phi1_phi2_two_track_dcaz, &fgaus_dcaz,
                  h_phi1_phi2_two_track_dcaz_Mean, h_phi1_phi2_two_track_dcaz_Width,
                          /*par_mean=*/1, /*par_width=*/2,
                          1, "QSRN", /*rebin_x=*/int(nbins_phi/10), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/4);
    
    FitTH3ZSlices_GetFWHMRMS(h_eta1_eta2_two_track_dcaxy,
                  h_eta1_eta2_two_track_dcaxy_Mode, h_eta1_eta2_two_track_dcaxy_FWHM, h_eta1_eta2_two_track_dcaxy_RMS,/*min_entries*/ 1,
                          /*rebin_x=*/int(nbins_eta/15), /*rebin_y=*/int(nbins_eta/15), /*rebin_z=*/5);
    
    FitTH3ZSlices_GetFWHMRMS(h_eta1_eta2_two_track_dcaz,
                  h_eta1_eta2_two_track_dcaz_Mode, h_eta1_eta2_two_track_dcaz_FWHM, h_eta1_eta2_two_track_dcaz_RMS,/*min_entries*/ 1,
                          /*rebin_x=*/int(nbins_eta/15), /*rebin_y=*/int(nbins_eta/15), /*rebin_z=*/5);
    
    FitTH3ZSlices_GetFWHMRMS(h_phi1_phi2_two_track_dcaxy,
                  h_phi1_phi2_two_track_dcaxy_Mode, h_phi1_phi2_two_track_dcaxy_FWHM, h_phi1_phi2_two_track_dcaxy_RMS,/*min_entries*/ 1,
                          /*rebin_x=*/int(nbins_phi/10), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/5);
    
    FitTH3ZSlices_GetFWHMRMS(h_phi1_phi2_two_track_dcaz,
                  h_phi1_phi2_two_track_dcaz_Mode, h_phi1_phi2_two_track_dcaz_FWHM, h_phi1_phi2_two_track_dcaz_RMS,/*min_entries*/ 1,
                          /*rebin_x=*/int(nbins_phi/10), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/5);
    
    FitTH3ZSlices(h_etadiff_phidiff_two_track_dcaxy, &fgaus_dcaxy,
                  h_etadiff_phidiff_two_track_dcaxy_Mean, h_etadiff_phidiff_two_track_dcaxy_Width,
                          /*par_mean=*/1, /*par_width=*/2,
                          1, "QSRN", /*rebin_x=*/int(nbins_eta/15), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/4);
    
    FitTH3ZSlices(h_etadiff_phidiff_two_track_dcaz, &fgaus_dcaz,
                  h_etadiff_phidiff_two_track_dcaz_Mean, h_etadiff_phidiff_two_track_dcaz_Width,
                          /*par_mean=*/1, /*par_width=*/2,
                          1, "QSRN", /*rebin_x=*/int(nbins_eta/15), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/4);
    
    FitTH3ZSlices(h_etadiff_phidiff_two_track_dcaxy3d, &fgaus_dcaxy,
                  h_etadiff_phidiff_two_track_dcaxy3d_Mean, h_etadiff_phidiff_two_track_dcaxy3d_Width, //dcaxy 3d is not centered around 0 so a Gaussian fit is not appropriate
                          /*par_mean=*/1, /*par_width=*/2,
                          1, "QSRN", /*rebin_x=*/int(nbins_eta/15), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/4);
    
    FitTH3ZSlices(h_etadiff_phidiff_two_track_dcaz3d, &fgaus_dcaz,
                  h_etadiff_phidiff_two_track_dcaz3d_Mean, h_etadiff_phidiff_two_track_dcaz3d_Width, //dcaxy 3d is not centered around 0 so a Gaussian fit is not appropriate
                          /*par_mean=*/1, /*par_width=*/2,
                          1, "QSRN", /*rebin_x=*/int(nbins_eta/15), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/4);
    
    FitTH3ZSlices_GetFWHMRMS(h_etadiff_phidiff_two_track_dcaxy,
                          h_etadiff_phidiff_two_track_dcaxy_Mode, h_etadiff_phidiff_two_track_dcaxy_FWHM, h_etadiff_phidiff_two_track_dcaxy_RMS,/*min_entries*/ 1,
                          /*rebin_x=*/int(nbins_eta/10), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/5);
    
    FitTH3ZSlices_GetFWHMRMS(h_etadiff_phidiff_two_track_dcaz,
                          h_etadiff_phidiff_two_track_dcaz_Mode, h_etadiff_phidiff_two_track_dcaz_FWHM,
                             h_etadiff_phidiff_two_track_dcaz_RMS,/*min_entries*/ 1,
                          /*rebin_x=*/int(nbins_eta/10), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/5);
    
    FitTH3ZSlices_GetFWHMRMS(h_etadiff_phidiff_two_track_dcaxy3d,
                          h_etadiff_phidiff_two_track_dcaxy3d_Mode, h_etadiff_phidiff_two_track_dcaxy3d_FWHM, h_etadiff_phidiff_two_track_dcaxy3d_RMS,/*min_entries*/ 1,
                          /*rebin_x=*/int(nbins_eta/10), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/5);
    
    FitTH3ZSlices_GetFWHMRMS(h_etadiff_phidiff_two_track_dcaz3d,
                          h_etadiff_phidiff_two_track_dcaz3d_Mode, h_etadiff_phidiff_two_track_dcaz3d_FWHM,
                             h_etadiff_phidiff_two_track_dcaz3d_RMS,/*min_entries*/ 1,
                          /*rebin_x=*/int(nbins_eta/10), /*rebin_y=*/int(nbins_phi/10), /*rebin_z=*/5);
    
    if (is_simulation_input){
        GetFitSlicesY(h_deltavxy_vs_nTracks,&fgaus_deltavertex_vs_nTracks,h_deltavxy_Mean_vs_nTracks,h_deltavxy_Width_vs_nTracks,"deltavxy_vs_nTracks","nTracks","deltavxy");
        GetFitSlicesY(h_deltavx_vs_nTracks,&fgaus_deltavertex_vs_nTracks,h_deltavx_Mean_vs_nTracks,h_deltavx_Width_vs_nTracks,"deltavx_vs_nTracks","nTracks","deltavx");
        GetFitSlicesY(h_deltavy_vs_nTracks,&fgaus_deltavertex_vs_nTracks,h_deltavy_Mean_vs_nTracks,h_deltavy_Width_vs_nTracks,"deltavy_vs_nTracks","nTracks","deltavy");
        GetFitSlicesY(h_deltavz_vs_nTracks,&fgaus_deltavertex_vs_nTracks,h_deltavz_Mean_vs_nTracks,h_deltavz_Width_vs_nTracks,"deltavz_vs_nTracks","nTracks","deltavz");
        
        GetFitSlicesY(h_deltavxy_vs_nwotrackcutsTracks,&fgaus_deltavertex_vs_nwotrackcutsTracks,h_deltavxy_Mean_vs_nwotrackcutsTracks,h_deltavxy_Width_vs_nwotrackcutsTracks,"deltavxy_vs_nwotrackcutsTracks","nwotrackcutsTracks","deltavxy");
        GetFitSlicesY(h_deltavx_vs_nwotrackcutsTracks,&fgaus_deltavertex_vs_nwotrackcutsTracks,h_deltavx_Mean_vs_nwotrackcutsTracks,h_deltavx_Width_vs_nwotrackcutsTracks,"deltavx_vs_nwotrackcutsTracks","nwotrackcutsTracks","deltavx");
        GetFitSlicesY(h_deltavy_vs_nwotrackcutsTracks,&fgaus_deltavertex_vs_nwotrackcutsTracks,h_deltavy_Mean_vs_nwotrackcutsTracks,h_deltavy_Width_vs_nwotrackcutsTracks,"deltavy_vs_nwotrackcutsTracks","nwotrackcutsTracks","deltavy");
        GetFitSlicesY(h_deltavz_vs_nwotrackcutsTracks,&fgaus_deltavertex_vs_nwotrackcutsTracks,h_deltavz_Mean_vs_nwotrackcutsTracks,h_deltavz_Width_vs_nwotrackcutsTracks,"deltavz_vs_nwotrackcutsTracks","nwotrackcutsTracks","deltavz");
    }
    
    TFile *outputFile = TFile::Open(outputrootfilename, "RECREATE");
    h_two_track_pt_dcaxy->Write();
    h_two_track_dcaxy_Mean_vs_pt->Write();
    h_two_track_dcaxy_Width_vs_pt->Write();
    h_two_track_dcaxy_Mode_vs_pt->Write();
    h_two_track_dcaxy_FWHM_vs_pt->Write();
    h_two_track_dcaxy_RMS_vs_pt->Write();
    
    h_two_track_pt_dcaz->Write();
    h_two_track_dcaz_Mean_vs_pt->Write();
    h_two_track_dcaz_Width_vs_pt->Write();
    h_two_track_dcaz_Mode_vs_pt->Write();
    h_two_track_dcaz_FWHM_vs_pt->Write();
    h_two_track_dcaz_RMS_vs_pt->Write();
    
    h_two_track_p_dcaxy->Write();
    h_two_track_dcaxy_Mean_vs_p->Write();
    h_two_track_dcaxy_Width_vs_p->Write();
    h_two_track_dcaxy_Mode_vs_p->Write();
    h_two_track_dcaxy_FWHM_vs_p->Write();
    h_two_track_dcaxy_RMS_vs_p->Write();
    
    h_two_track_p_dcaz->Write();
    h_two_track_dcaz_Mean_vs_p->Write();
    h_two_track_dcaz_Width_vs_p->Write();
    h_two_track_dcaz_Mode_vs_p->Write();
    h_two_track_dcaz_FWHM_vs_p->Write();
    h_two_track_dcaz_RMS_vs_p->Write();
    
    h_two_track_pt_corr->Write();
    h_nmapsstate_phi->Write();
    
    h_pcax_pcay_pt->Write();
    h_vx_vy->Write();
    
    h_two_track_eta_dcaxy->Write();
    h_two_track_phi_dcaxy->Write();
    
    h_nvertices->Write();
    h_vertex_ntracks->Write();
    h_vertex_selected_ntracks->Write();
    h_vertex_wocuts_ntracks->Write();
    h_fractrackssatisfyingcuts->Write();
    
    if (is_simulation_input){
        h_truevx_truevy->Write();
        
        h_deltavxy_vs_nTracks->Write();
        h_deltavxy_Mean_vs_nTracks->Write();
        h_deltavxy_Width_vs_nTracks->Write();
        
        h_deltavxy_vs_nwotrackcutsTracks->Write();
        h_deltavxy_Mean_vs_nwotrackcutsTracks->Write();
        h_deltavxy_Width_vs_nwotrackcutsTracks->Write();
        
        h_deltavx_vs_nwotrackcutsTracks->Write();
        h_deltavx_Mean_vs_nwotrackcutsTracks->Write();
        h_deltavx_Width_vs_nwotrackcutsTracks->Write();
        
        h_deltavy_vs_nwotrackcutsTracks->Write();
        h_deltavy_Mean_vs_nwotrackcutsTracks->Write();
        h_deltavy_Width_vs_nwotrackcutsTracks->Write();
        
        h_deltavz_vs_nwotrackcutsTracks->Write();
        h_deltavz_Mean_vs_nwotrackcutsTracks->Write();
        h_deltavz_Width_vs_nwotrackcutsTracks->Write();
        
        h_deltavx_vs_nTracks->Write();
        h_deltavx_Mean_vs_nTracks->Write();
        h_deltavx_Width_vs_nTracks->Write();
        
        h_deltavy_vs_nTracks->Write();
        h_deltavy_Mean_vs_nTracks->Write();
        h_deltavy_Width_vs_nTracks->Write();
        
        h_deltavz_vs_nTracks->Write();
        h_deltavz_Mean_vs_nTracks->Write();
        h_deltavz_Width_vs_nTracks->Write();
    }
    
    h_etadiff_two_track_dcaxy_pt->Write();
    h_etadiff_two_track_dcaz_pt->Write();
    h_phidiff_two_track_dcaxy_pt->Write();
    h_phidiff_two_track_dcaz_pt->Write();
    
    h_eta1_eta2_two_track_dcaxy->Write();
    h_eta1_eta2_two_track_dcaxy_Mean->Write();
    h_eta1_eta2_two_track_dcaxy_Width->Write();
    h_eta1_eta2_two_track_dcaxy_Mode->Write();
    h_eta1_eta2_two_track_dcaxy_FWHM->Write();
    h_eta1_eta2_two_track_dcaxy_RMS->Write();
    
    h_eta1_eta2_two_track_dcaz->Write();
    h_eta1_eta2_two_track_dcaz_Mean->Write();
    h_eta1_eta2_two_track_dcaz_Width->Write();
    h_eta1_eta2_two_track_dcaz_Mode->Write();
    h_eta1_eta2_two_track_dcaz_FWHM->Write();
    h_eta1_eta2_two_track_dcaz_RMS->Write();
    
    h_phi1_phi2_two_track_dcaxy->Write();
    h_phi1_phi2_two_track_dcaxy_Mean->Write();
    h_phi1_phi2_two_track_dcaxy_Width->Write();
    h_phi1_phi2_two_track_dcaxy_Mode->Write();
    h_phi1_phi2_two_track_dcaxy_FWHM->Write();
    h_phi1_phi2_two_track_dcaxy_RMS->Write();
    
    h_phi1_phi2_two_track_dcaz->Write();
    h_phi1_phi2_two_track_dcaz_Mean->Write();
    h_phi1_phi2_two_track_dcaz_Width->Write();
    h_phi1_phi2_two_track_dcaz_Mode->Write();
    h_phi1_phi2_two_track_dcaz_FWHM->Write();
    h_phi1_phi2_two_track_dcaz_RMS->Write();
    
    h_etadiff_phidiff_two_track_dcaxy->Write();
    h_etadiff_phidiff_two_track_dcaxy_Mean->Write();
    h_etadiff_phidiff_two_track_dcaxy_Width->Write();
    h_etadiff_phidiff_two_track_dcaxy_Mode->Write();
    h_etadiff_phidiff_two_track_dcaxy_FWHM->Write();
    h_etadiff_phidiff_two_track_dcaxy_RMS->Write();
    
    h_etadiff_phidiff_two_track_dcaz->Write();
    h_etadiff_phidiff_two_track_dcaz_Mean->Write();
    h_etadiff_phidiff_two_track_dcaz_Width->Write();
    h_etadiff_phidiff_two_track_dcaz_Mode->Write();
    h_etadiff_phidiff_two_track_dcaz_FWHM->Write();
    h_etadiff_phidiff_two_track_dcaz_RMS->Write();
    
    h_etadiff_phidiff_two_track_dcaxy3d->Write();
    h_etadiff_phidiff_two_track_dcaxy3d_Mean->Write();
    h_etadiff_phidiff_two_track_dcaxy3d_Width->Write(); //dcaxy 3d is not centered around 0 so a Gaussian fit is not appropriate
    h_etadiff_phidiff_two_track_dcaxy3d_Mode->Write();
    h_etadiff_phidiff_two_track_dcaxy3d_FWHM->Write();
    h_etadiff_phidiff_two_track_dcaxy3d_RMS->Write();
    
    h_etadiff_phidiff_two_track_dcaz3d->Write();
    h_etadiff_phidiff_two_track_dcaz3d_Mean->Write();
    h_etadiff_phidiff_two_track_dcaz3d_Width->Write(); //dcaxy 3d is not centered around 0 so a Gaussian fit is not appropriate
    h_etadiff_phidiff_two_track_dcaz3d_Mode->Write();
    h_etadiff_phidiff_two_track_dcaz3d_FWHM->Write();
    h_etadiff_phidiff_two_track_dcaz3d_RMS->Write();
    
    
    h_dcaxy1_dcaxy2_diffvs_2trackdcaxy->Write();
    h_dcaz1_dcaz2_diffvs_2trackdcaz->Write();
    
    outputFile->Close();
    std::cout<<Form("%s has been created",outputrootfilename)<<std::endl;
    
    if(createpdffile){
        SaveAllHistogramsToPdf(outputrootfilename);
    }
//    if(createCanvases==true){
//        gSystem->mkdir(plots_outputdir, true);
//        
//        CreateandSaveCanvaswithPID(plots_outputdir,"canvas_twotrackdcaxydcazdists_vs_pandpt",h_two_track_pt_dcaxy,h_two_track_p_dcaxy,h_two_track_pt_dcaz,h_two_track_p_dcaz,title_xstart, title_ystart, description, trackcuts_info, dcaxy_onesiderange_formeanwidthcalc);
//        
//        CreateandSaveCanvaswithPID(plots_outputdir, "canvas_twotrackdcavsmomentum_meanwidthplots", h_two_track_dcaxy_Mean_vs_pt, h_two_track_dcaxy_Width_vs_pt, h_two_track_dcaz_Mean_vs_pt, h_two_track_dcaz_Width_vs_pt, h_two_track_dcaxy_Mean_vs_p, h_two_track_dcaxy_Width_vs_p, h_two_track_dcaz_Mean_vs_p, h_two_track_dcaz_Width_vs_p, title_xstart, title_ystart, description, trackcuts_info, dcaxy_onesiderange_formeanwidthcalc);
//        
//        CreateandSaveCanvaswithPID(plots_outputdir, "canvas_twotrackdcavsmomentum_ModeFWHMplots", h_two_track_dcaxy_Mode_vs_pt, h_two_track_dcaxy_FWHM_vs_pt, h_two_track_dcaz_Mode_vs_pt, h_two_track_dcaz_FWHM_vs_pt, h_two_track_dcaxy_Mode_vs_p, h_two_track_dcaxy_FWHM_vs_p, h_two_track_dcaz_Mode_vs_p, h_two_track_dcaz_FWHM_vs_p, title_xstart, title_ystart, description, trackcuts_info, 0.0);
//    }
}

void compute_twoTrackDca(const std::vector<float>& pcax_vec, const std::vector<float>& pcay_vec, const std::vector<float>& pcaz_vec, const std::vector<float>& px_vec, const std::vector<float>& py_vec, const std::vector<float>& pz_vec, const std::vector<float>& eta_vec, const std::vector<float>& phi_vec, const std::vector<float>& dcaxy_vec, const std::vector<float>& dcaz_vec, std::vector<float> &two_track_dcaxy_vec, std::vector<float> &two_track_dcaz_vec, std::vector<float> &two_track_ptavg_vec, std::vector<float> &two_track_pavg_vec, std::vector<float> &two_track_etadiff_vec, std::vector<float> &two_track_phidiff_vec, TH2D* h_pt_corr, TH3D *h_etadiff_two_track_dcaxy_pt, TH3D *h_etadiff_two_track_dcaz_pt, TH3D *h_phidiff_two_track_dcaxy_pt, TH3D *h_phidiff_two_track_dcaz_pt, TH3D* h_eta1_eta2_two_track_dcaxy, TH3D* h_eta1_eta2_two_track_dcaz, TH3D* h_phi1_phi2_two_track_dcaxy, TH3D* h_phi1_phi2_two_track_dcaz, TH3D* h_etadiff_phidiff_two_track_dcaxy, TH3D* h_etadiff_phidiff_two_track_dcaz, TH3D* h_etadiff_phidiff_two_track_dcaxy3d, TH3D* h_etadiff_phidiff_two_track_dcaz3d, TH2D* h_dcaxy1_dcaxy2_diffvs_2trackdcaxy,TH2D* h_dcaz1_dcaz2_diffvs_2trackdcaz)
{
  const size_t nTracks = pcax_vec.size();
  if (nTracks < 2) return;
    
  std::vector<size_t> idx(nTracks);
  std::iota(idx.begin(), idx.end(), 0);

  static std::mt19937 rng(std::random_device{}());
  std::shuffle(idx.begin(), idx.end(), rng);
    
  ROOT::Math::XYZVectorF z_unit(0,0,1);
  for(size_t ia = 0; ia< pcax_vec.size()-1;++ia)
  {
    size_t ii = idx[ia];
    ROOT::Math::XYZVectorF trackdir1(px_vec.at(ii), py_vec.at(ii), pz_vec.at(ii));
    ROOT::Math::XYZVectorF trackpca1(pcax_vec.at(ii), pcay_vec.at(ii), pcaz_vec.at(ii));
    float pt1 = std::sqrt(pow(px_vec.at(ii),2)+ pow(py_vec.at(ii),2));
    float pmag1 = std::sqrt(trackdir1.Dot(trackdir1));
    float eta1 = eta_vec.at(ii);
    float phi1 = phi_vec.at(ii);
      
    for(size_t ja = ia+1; ja<pcax_vec.size();ja++)
    {
      size_t jj = idx[ja];
      float pt2 = std::sqrt(pow(px_vec.at(jj),2)+ pow(py_vec.at(jj),2));
      float eta2 = eta_vec.at(jj);
      float phi2 = phi_vec.at(jj);
      h_pt_corr->Fill(pt1,pt2);
      ROOT::Math::XYZVectorF trackdir2(px_vec.at(jj), py_vec.at(jj), pz_vec.at(jj));
      float pmag2 = std::sqrt(trackdir2.Dot(trackdir2));
        
        
      if(std::fabs(pmag1-pmag2)>pdiffpair_cut){ continue;}
        //std::cout<<"phi1="<<phi1<<", phi2="<<phi2<<std::endl;
      //if(std::fabs(pmag1-pmag2)>0.1 || (std::fabs(TVector2::Phi_mpi_pi(phi1-phi2))<0.15 && std::fabs(eta1-eta2)<0.04)){ continue;}
      ROOT::Math::XYZVectorF trackpca2(pcax_vec.at(jj), pcay_vec.at(jj), pcaz_vec.at(jj));
      ROOT::Math::XYZVectorF trackdiffxy(pcax_vec.at(jj)-pcax_vec.at(ii),pcay_vec.at(jj)-pcay_vec.at(ii),0.);
      ROOT::Math::XYZVectorF trackdiffz(0,0,pcaz_vec.at(jj)-pcaz_vec.at(ii));
      ROOT::Math::XYZVectorF crossVec = trackdir1.Cross(trackdir2);
      float crossMag = crossVec.R();
      if (crossMag < 1e-6) continue;
      ROOT::Math::XYZVectorF dirCrossDirUnit = crossVec * (1.0f / crossMag);
      float twotrackdcaxy_out = dirCrossDirUnit.Dot(trackdiffxy);
      float twotrackdcaz_out = dirCrossDirUnit.Dot(trackdiffz);
      ROOT::Math::XYZVectorF ptvec = (trackdir1+trackdir2).Cross(z_unit);
      //float pt_out = std::sqrt(ptvec.Dot(ptvec)/2);
        
        ROOT::Math::XYZVectorF u1 = trackdir1.Unit();
        ROOT::Math::XYZVectorF u2 = trackdir2.Unit();
        float c = u1.Dot(u2);
        ROOT::Math::XYZVectorF dp = trackpca2 - trackpca1;

        float a = u1.Dot(dp);
        float b = u2.Dot(dp);

        float denom = 1.0f - c*c;
        if (std::fabs(denom) <= 1e-6) {continue;}
            float s = (a - c*b) / denom;
            float t = (c*a - b) / denom;
            
            ROOT::Math::XYZVectorF v1 = trackpca1 + s*u1;
            ROOT::Math::XYZVectorF v2 = trackpca2 + t*u2;
            ROOT::Math::XYZVectorF twotrackdca3d_vec= v2-v1;
            float twotrackdcaxy_3d_out=twotrackdca3d_vec.Rho();
            float twotrackdcaz_3d_out=twotrackdca3d_vec.Z();
            
            //             ROOT::Math::XYZVectorF vertex = 0.5f * (v1 + v2);
            //
            //        std::cout<<"dcaxy1="<<dcaxy_vec.at(ii)<<", dcaxy2="<<dcaxy_vec.at(jj)<<", twotrackdcaxy_out="<<twotrackdcaxy_out<<std::endl;
            //        std::cout<<"dcaz1="<<dcaz_vec.at(ii)<<", dcaz2="<<dcaz_vec.at(jj)<<", twotrackdcaz_out="<<twotrackdcaz_out<<std::endl;
            //
            //        std::cout << "pcaxy1=" << std::sqrt(trackpca1.X()*trackpca1.X() + trackpca1.Y()*trackpca1.Y())
            //               << ", pcaxy2=" << std::sqrt(trackpca2.X()*trackpca2.X() + trackpca2.Y()*trackpca2.Y())
            //               << ", pcaz1_z=" << trackpca1.Z()
            //               << ", pcaz2_z=" << trackpca2.Z()
            //               << std::endl;
            //        std::cout << "estimated vertex = ("
            //                  << vertex.X() << ", " << vertex.Y() << ", " << vertex.Z() << ")"
            //                  << std::endl<<std::endl;
    
      h_dcaxy1_dcaxy2_diffvs_2trackdcaxy->Fill((dcaxy_vec.at(ii)-dcaxy_vec.at(jj)),twotrackdcaxy_out);
      h_dcaz1_dcaz2_diffvs_2trackdcaz->Fill((dcaz_vec.at(ii)-dcaz_vec.at(jj)),twotrackdcaz_out);
      //two_track_dcaxy_vec.push_back(dirCrossDirUnit.Dot(trackpca2-trackpca1));
      two_track_dcaxy_vec.push_back(twotrackdcaxy_out);
      two_track_dcaz_vec.push_back(twotrackdcaz_out);
      //two_track_ptavg_vec.push_back(pt_out);
      two_track_ptavg_vec.push_back((pt1+pt2)/2);
      two_track_pavg_vec.push_back((pmag1+pmag2)/2.0);
      two_track_etadiff_vec.push_back(eta1-eta2);
      two_track_phidiff_vec.push_back(TVector2::Phi_mpi_pi(phi1-phi2));
      //std::cout<<"trackdiffxy: "<<trackdiffxy<<"  trackdiffz: "<<trackdiffz<<"  dirCrossDirUnit: "<<dirCrossDirUnit<<std::endl;
      //std::cout<<"dcaxy: "<<twotrackdcaxy_out<<"  dcaz: "<<twotrackdcaz_out<<"  dca: "<<dirCrossDirUnit.Dot(trackpca2-trackpca1)<<std::endl;
        if(pt1 >= pt_cut_etaphitwotrackdca3dhists &&pt2 >= pt_cut_etaphitwotrackdca3dhists){
            h_eta1_eta2_two_track_dcaxy->Fill(eta1, eta2, twotrackdcaxy_out);
            h_eta1_eta2_two_track_dcaz->Fill(eta1, eta2, twotrackdcaz_out);
            
            h_phi1_phi2_two_track_dcaxy->Fill(phi1, phi2, twotrackdcaxy_out);
            h_phi1_phi2_two_track_dcaz->Fill(phi1, phi2, twotrackdcaz_out);
            
            h_etadiff_phidiff_two_track_dcaxy->Fill((eta1-eta2),TVector2::Phi_mpi_pi(phi1-phi2),twotrackdcaxy_out);
            h_etadiff_phidiff_two_track_dcaz->Fill((eta1-eta2),TVector2::Phi_mpi_pi(phi1-phi2),twotrackdcaz_out);
            
            h_etadiff_phidiff_two_track_dcaxy3d->Fill((eta1-eta2),TVector2::Phi_mpi_pi(phi1-phi2),twotrackdcaxy_3d_out);
            h_etadiff_phidiff_two_track_dcaz3d->Fill((eta1-eta2),TVector2::Phi_mpi_pi(phi1-phi2),twotrackdcaz_3d_out);
        }
            
            h_etadiff_two_track_dcaxy_pt->Fill((eta1-eta2), twotrackdcaxy_out,(pt1+pt2)/2.0);
            h_etadiff_two_track_dcaz_pt->Fill((eta1-eta2), twotrackdcaz_out,(pt1+pt2)/2.0);
            
            h_phidiff_two_track_dcaxy_pt->Fill(TVector2::Phi_mpi_pi(phi1-phi2), twotrackdcaxy_out,(pt1+pt2)/2.0);
            h_phidiff_two_track_dcaz_pt->Fill(TVector2::Phi_mpi_pi(phi1-phi2), twotrackdcaz_out,(pt1+pt2)/2.0);
        
    }
  }
}
