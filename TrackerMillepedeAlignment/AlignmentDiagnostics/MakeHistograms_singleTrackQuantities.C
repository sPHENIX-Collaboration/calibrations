#include "UsefulFunctions.h"
#include <TChain.h>

int MakeHistograms_singleTrackQuantities(const bool is_simulation_input=false){
    int vertex_ntracks_cut=3; float fabs_vx_cut=0.0000001; //event level cuts applied to all tracks
    int nmaps_cut=2, intt_cut=2, ntpc_cut=0; float pt_cut=0.2; float ptcutfordca_vsetaphi=0.8; float chisq_ndf_cut = 10; //track level cuts
    
//    TFile *File=new TFile("output_SimpleEvtGenerator/residuals_G4sPHENIX_SimpleEvtGenerator_all.root");
//    TFile *File=new TFile("residuals_G4sPHENIX_34930evts.root");
//    const char* description="Simple Event Generator (2 #pi^{+} + 2 #pi^{-} )";
//    const char* outputname="SimpleEvtGenerator";
//    const char* plots_outputdir="Plots";
//    const char* trackcuts_info="Cuts: m_nmaps>1 && m_nintt>0 && m_ntpc>25 && vertex_ntracks > 3";
//    float title_xstart=0.3, title_ystart=0.95;
    
        //TFile *File=new TFile("residuals_G4sPHENIX_3499.root");
        //TFile *File=new TFile("residuals_G4sPHENIX_Pythia_105kevts_withtruedca.root");
        //TTree *residualtree = (TTree*)File->Get("residualtree");
    
        //TChain *residualtree = new TChain("residualtree");
        //residualtree->Add("residuals_G4sPHENIX_3499.root");
//        residualtree->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_105kevts_withtruedca.root");
//        residualtree->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_175kevts_withtruedca_set*.root");
//        residualtree->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_200kevts_withtruedca_set*.root");

    char outputrootfilename[200]="defaultoutputname.root";
    char description[200]="description";
    char plots_outputdir[200]="Plots_current";
    char vertexcuts_info[200];
    snprintf(vertexcuts_info, sizeof(vertexcuts_info),
             "EventCuts: vertex_ntracks > %i, |vx|>=%g", vertex_ntracks_cut, fabs_vx_cut);
    
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
    
    float title_xstart=0.4, title_ystart=0.95;
    
    TChain *residualtree = new TChain("residualtree");
    if(is_simulation_input){
        snprintf(outputrootfilename, sizeof(outputrootfilename),"output_PlottingMacro_singleTrackQuantities_Simulation.root");
        snprintf(description, sizeof(description),"Pythia 8 Minbias (%s)",vertexcuts_info);
        snprintf(plots_outputdir, sizeof(plots_outputdir),"Plots_current");
        title_xstart=0.4, title_ystart=0.95;
        
        residualtree->Add("./output_Pythia_Simulation/SoftQCD_nonDiffractive/residuals_G4sPHENIX_minbiasPythia_withchisqbyndf_200kevts*.root");
        
        //        TChain *residualtree = new TChain("residualtree");
        //        residualtree->Add("./output_Pythia_Simulation/SoftQCD_nonDiffractive/residuals_G4sPHENIX_minbiasPythia_200kevts*.root");
    }
    else{
        snprintf(outputrootfilename, sizeof(outputrootfilename),"output_PlottingMacro_singleTrackQuantities_data_fulltracks.root");
        snprintf(description, sizeof(description),"sPHENIX (%s)",vertexcuts_info);
        snprintf(plots_outputdir, sizeof(plots_outputdir),"Plots_current/sPHENIX_data");
        title_xstart=0.4; title_ystart=0.95;
        
        char name[200];
        for(int i=0;i<999;++i)
        {
            //snprintf(name,sizeof(name), "/sphenix/tg/tg01/hf/gregoryottino/silicon_tpc_matching/cluster_seeds_all_79516-%i.root_resid.root",i);
            
            snprintf(name,sizeof(name), "/sphenix/tg/tg01/hf/gregoryottino/silicon_tpc_matching/output_baseAlignment/cluster_seeds_all_79516-%i.root_resid.root",i);
            //snprintf(name,sizeof(name), "/sphenix/tg/tg01/hf/gregoryottino/silicon_tpc_matching/output_baseAlignment/cluster_seeds_si_79516-%i.root_resid.root",i);
            //std::cout << "Adding " << name << std::endl;
            residualtree->Add(name);
        }
    }
        
    
    bool createpdffile=false;
    
    float dcaxy=0, dcaz=0, true_dcaxy=0, true_dcaz=0, px=0, py=0, pz=0, pt=0,eta=0,phi=0,vx=0,vy=0,vz=0, chisq=0, ndf=0; //quantities obtained from residualtree
    int nmaps=0, nmapsstate=0, nintt=0, ntpc=0, vertex_ntracks=0,vertex_crossing=0, has_truth_match=-1, truth_trackid=-1, charge=0;
    float dcaxy_min=-0.02, dcaxy_max=0.02, dcaz_min=-0.02, dcaz_max=0.02;
    float dcaxy_min_formeanwidthcalc=-0.02, dcaxy_max_formeanwidthcalc=0.02, dcaz_min_formeanwidthcalc=-0.02, dcaz_max_formeanwidthcalc=0.02, truth_match_fraction=0.0;
    //float pt_min=0.0, pt_max=5.5;
    //float p_min=0.0, p_max=5.5;
    float eta_min=-1.5, eta_max=1.5;
    float phi_min=-3.5, phi_max=3.5;
    //int nbins_pt=150, nbins_p=150,
    int nbins_eta=150, nbins_phi=150, nbins_dcaxy=400, nbins_dcaz=400, nbins_truedcaxy=400, nbins_truedcaz=400;
    
    std::vector<double> pt_bins    = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.75, 1.0, 1.5, 3.0, 5.5};
    std::vector<double> p_bins     = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.75, 1.0, 1.5, 3.0, 5.5};
    
    TF1* fgaus_dcaxy=new TF1("fgaus_dcaxy","gaus",dcaxy_min_formeanwidthcalc, dcaxy_max_formeanwidthcalc);
    TF1* fgaus_dcaz=new TF1("fgaus_dcaz","gaus",dcaz_min_formeanwidthcalc, dcaz_max_formeanwidthcalc);
    TF1* fgaus_nmaps = new TF1("fgaus_nmaps", "gaus", 0, 6);
    TF1* fgaus_chisq = new TF1("fgaus_chisq", "gaus", 0, 20);
    
    auto rename_fit_slices = [](TH1D* hMean, TH1D* hSigma, const char* base, const TString& tag)
    {
        if (hMean)
        {
            hMean->SetName(Form("Gaussian_Mean_of_%s_%s", base, tag.Data()));
        }
        if (hSigma)
        {
            hSigma->SetName(Form("Gaussian_Width_of_%s_%s", base, tag.Data()));
        }
    };
    
    //TF1 *fit1 = new TF1("fit1", "pol1");
    
    enum TrackSet
        {
          kAllTracks = 0,
          kPassCuts = 1,
          kFailnmapsCuts = 2,
          kFailinttCuts = 3,
          kFailntpcCuts = 4,
          kFailptCuts = 5,
          kNoTruthMatch=6,
          kLowTruthPurity=7,
          kPassCuts_NoChiSqCut=8,
          kPasscuts_Charge1=9,
          kPasscuts_Chargem1=10,
          kNTrackSets = 11
        };
    
    TString set_tag[kNTrackSets] = {
        "alltracks", "passcuts", Form("nmaps_lessthan_%g",nmaps_cut), Form("nintt_lessthan_%g",intt_cut), Form("ntpc_lessthan_%g",ntpc_cut), Form("pt_lessthan_%g",pt_cut), "No_truth_match", "truth_purity_lessthanp5","passcuts_nochisqcut","passcuts_charge1","passcuts_chargem1"};
    
    TString set_title[kNTrackSets] = {
        "alltracks", "passcuts", Form("nmaps_lessthan_%g",nmaps_cut), Form("nintt_lessthan_%g",intt_cut), Form("ntpc_lessthan_%g",ntpc_cut), Form("pt_lessthan_%g",pt_cut), "No_truth_match", "truth purity_lessthanp5","passcuts_nochisqcut","passcuts_charge1","passcuts_chargem1"};
    
    residualtree->SetBranchAddress("px", &px);
    residualtree->SetBranchAddress("py", &py);
    residualtree->SetBranchAddress("pz", &pz);
    residualtree->SetBranchAddress("pt", &pt);
    residualtree->SetBranchAddress("eta", &eta);
    residualtree->SetBranchAddress("phi", &phi);
    residualtree->SetBranchAddress("nmaps", &nmaps);
    residualtree->SetBranchAddress("nmapsstate", &nmapsstate);
    residualtree->SetBranchAddress("nintt", &nintt);
    residualtree->SetBranchAddress("ntpc", &ntpc);
    residualtree->SetBranchAddress("vertex_ntracks", &vertex_ntracks);
    residualtree->SetBranchAddress("dcaxy", &dcaxy);
    residualtree->SetBranchAddress("dcaz", &dcaz);
    residualtree->SetBranchAddress("vertex_crossing", &vertex_crossing);
    residualtree->SetBranchAddress("vx", &vx);
    residualtree->SetBranchAddress("vy", &vy);
    residualtree->SetBranchAddress("vz", &vz);
    residualtree->SetBranchAddress("charge", &charge);
    bool has_chisq_info = (residualtree->GetBranch("chisq") != nullptr && residualtree->GetBranch("ndf") != nullptr);
    if (has_chisq_info)
    {
        residualtree->SetBranchAddress("chisq", &chisq);
        residualtree->SetBranchAddress("ndf", &ndf);
    }
    else
    {
        std::cout << "[INFO] chisq / ndf branches NOT found - chi-square cut will be skipped.\n";
    }
     
    bool has_true_dca = (residualtree->GetBranch("true_dcaxy") != nullptr &&
                             residualtree->GetBranch("true_dcaz")  != nullptr);
        if (has_true_dca)
        {
            residualtree->SetBranchAddress("true_dcaxy", &true_dcaxy);
            residualtree->SetBranchAddress("true_dcaz",  &true_dcaz);
        }
        else
        {
            //std::cout << "[INFO] true_dcaxy / true_dcaz branches NOT found — skipping truth histograms." << std::endl;
        }
    
    bool has_truth_info = (residualtree->GetBranch("has_truth_match") != nullptr);
    if (has_truth_info) {
        residualtree->SetBranchAddress("has_truth_match",       &has_truth_match);
        residualtree->SetBranchAddress("truth_match_fraction",  &truth_match_fraction);
        residualtree->SetBranchAddress("truth_trackid",         &truth_trackid);
    }
    else
    {
        std::cout << "[INFO] has_truth_match / truth_match_fraction / truth_trackid branches "
                     "NOT found — kNoTruthMatch and kLowTruthPurity sets will be empty.\n";
    }
    
    TH1D* hEta_allpid[kNTrackSets] = {}; TH1D* hPhi_allpid[kNTrackSets] = {}; TH1D* hPt_allpid[kNTrackSets] = {}; TH1D* hp_allpid[kNTrackSets] = {};

    TH2D* hnmapsstate_vs_phi_allpid[kNTrackSets] = {}; TH1D* hnmapsstate_Mean_vs_phi_allpid[kNTrackSets] = {}; TH1D* hnmapsstate_Width_vs_phi_allpid[kNTrackSets] = {};
    
    TH2D* hchisqbyndf_vs_pt_allpid[kNTrackSets] = {}; TH1D* hchisqbyndf_Mean_vs_pt_allpid[kNTrackSets] = {}; TH1D* hchisqbyndf_Width_vs_pt_allpid[kNTrackSets] = {};
    
    //dcaxy as a function of reconstructed quantities
    TH2D* hdcaxy_vs_ptreco_allpid[kNTrackSets] = {};
    TH2D* hdcaxy_vs_preco_allpid[kNTrackSets] = {};
    TH2D* hdcaxy_vs_etareco_allpid[kNTrackSets] = {};
    TH2D* hdcaxy_vs_phireco_allpid[kNTrackSets] = {};

    TH1D* hdcaxy_Mean_vs_ptreco_allpid[kNTrackSets] = {};
    TH1D* hdcaxy_Mean_vs_preco_allpid[kNTrackSets] = {};
    TH1D* hdcaxy_Mean_vs_etareco_allpid[kNTrackSets] = {};
    TH1D* hdcaxy_Mean_vs_phireco_allpid[kNTrackSets] = {};

    TH1D* hdcaxy_Width_vs_ptreco_allpid[kNTrackSets] = {};
    TH1D* hdcaxy_Width_vs_preco_allpid[kNTrackSets] = {};
    TH1D* hdcaxy_Width_vs_etareco_allpid[kNTrackSets] = {};
    TH1D* hdcaxy_Width_vs_phireco_allpid[kNTrackSets] = {};
       
    //dcaz as a function of reconstructed quantities
    TH2D* hdcaz_vs_ptreco_allpid[kNTrackSets] = {};
    TH2D* hdcaz_vs_preco_allpid[kNTrackSets] = {};
    TH2D* hdcaz_vs_etareco_allpid[kNTrackSets] = {};
    TH2D* hdcaz_vs_phireco_allpid[kNTrackSets] = {};

    TH1D* hdcaz_Mean_vs_ptreco_allpid[kNTrackSets] = {};
    TH1D* hdcaz_Mean_vs_preco_allpid[kNTrackSets] = {};
    TH1D* hdcaz_Mean_vs_etareco_allpid[kNTrackSets] = {};
    TH1D* hdcaz_Mean_vs_phireco_allpid[kNTrackSets] = {};

    TH1D* hdcaz_Width_vs_ptreco_allpid[kNTrackSets] = {};
    TH1D* hdcaz_Width_vs_preco_allpid[kNTrackSets] = {};
    TH1D* hdcaz_Width_vs_etareco_allpid[kNTrackSets] = {};
    TH1D* hdcaz_Width_vs_phireco_allpid[kNTrackSets] = {};
    
    // true_dcaxy as a function of reconstructed quantities
    TH2D* htruedcaxy_vs_ptreco_allpid[kNTrackSets] = {};
    TH2D* htruedcaxy_vs_preco_allpid[kNTrackSets]  = {};
    TH2D* htruedcaxy_vs_etareco_allpid[kNTrackSets] = {};
    TH2D* htruedcaxy_vs_phireco_allpid[kNTrackSets] = {};

    TH1D* htruedcaxy_Mean_vs_ptreco_allpid[kNTrackSets]  = {};
    TH1D* htruedcaxy_Mean_vs_preco_allpid[kNTrackSets]   = {};
    TH1D* htruedcaxy_Mean_vs_etareco_allpid[kNTrackSets] = {};
    TH1D* htruedcaxy_Mean_vs_phireco_allpid[kNTrackSets] = {};

    TH1D* htruedcaxy_Width_vs_ptreco_allpid[kNTrackSets]  = {};
    TH1D* htruedcaxy_Width_vs_preco_allpid[kNTrackSets]   = {};
    TH1D* htruedcaxy_Width_vs_etareco_allpid[kNTrackSets] = {};
    TH1D* htruedcaxy_Width_vs_phireco_allpid[kNTrackSets] = {};

    // true_dcaz as a function of reconstructed quantities
    TH2D* htruedcaz_vs_ptreco_allpid[kNTrackSets]  = {};
    TH2D* htruedcaz_vs_preco_allpid[kNTrackSets]   = {};
    TH2D* htruedcaz_vs_etareco_allpid[kNTrackSets] = {};
    TH2D* htruedcaz_vs_phireco_allpid[kNTrackSets] = {};

    TH1D* htruedcaz_Mean_vs_ptreco_allpid[kNTrackSets]  = {};
    TH1D* htruedcaz_Mean_vs_preco_allpid[kNTrackSets]   = {};
    TH1D* htruedcaz_Mean_vs_etareco_allpid[kNTrackSets] = {};
    TH1D* htruedcaz_Mean_vs_phireco_allpid[kNTrackSets] = {};

    TH1D* htruedcaz_Width_vs_ptreco_allpid[kNTrackSets]  = {};
    TH1D* htruedcaz_Width_vs_preco_allpid[kNTrackSets]   = {};
    TH1D* htruedcaz_Width_vs_etareco_allpid[kNTrackSets] = {};
    TH1D* htruedcaz_Width_vs_phireco_allpid[kNTrackSets] = {};
    
    
    for (int iset = 0; iset < kNTrackSets; ++iset)
    {
        hEta_allpid[iset] = new TH1D(
          Form("hEta_allpid_%s", set_tag[iset].Data()),
          Form("#eta distribution_allpid, %s, %s;#eta;tracks", vertexcuts_info, set_title[iset].Data()), nbins_eta, eta_min, eta_max
        );
        hEta_allpid[iset]->SetStats(0);

        hPhi_allpid[iset] = new TH1D(
          Form("hPhi_allpid_%s", set_tag[iset].Data()),
          Form("#phi distribution_allpid, %s, %s;#phi;tracks", vertexcuts_info, set_title[iset].Data()),
          nbins_phi, phi_min, phi_max
        );
        hPhi_allpid[iset]->SetStats(0);

        hPt_allpid[iset] = new TH1D(
          Form("hPt_allpid_%s", set_tag[iset].Data()),
          Form("p_{T} distribution_allpid, %s, %s;p_{T} (GeV/c);tracks", vertexcuts_info, set_title[iset].Data()),
                                    pt_bins.size()-1, pt_bins.data()
        );
        hPt_allpid[iset]->SetStats(0);

        hp_allpid[iset] = new TH1D(
          Form("hp_allpid_%s", set_tag[iset].Data()),
          Form("p distribution_allpid, %s, %s;p (GeV/c);tracks", vertexcuts_info, set_title[iset].Data()),
                                   p_bins.size()-1, p_bins.data()
        );
        hp_allpid[iset]->SetStats(0);

        hnmapsstate_vs_phi_allpid[iset] = new TH2D(
          Form("hnmapsstate_vs_phi_allpid_%s", set_tag[iset].Data()),
          Form("nmapsstate vs #phi_allpid, %s, %s;#phi;nmapsstate", vertexcuts_info, set_title[iset].Data()),
          nbins_phi, phi_min, phi_max, 6, 0, 6
        );
        hnmapsstate_vs_phi_allpid[iset]->SetStats(0);
        
        hchisqbyndf_vs_pt_allpid[iset] = new TH2D(
          Form("hchisqbyndf_vs_pt_allpid_%s", set_tag[iset].Data()),
          Form("chisqbyndf vs pt_allpid, %s, %s;p_{T};chisq/ndf", vertexcuts_info, set_title[iset].Data()),
          pt_bins.size()-1, pt_bins.data(), 80, 0, 20
        );
        hchisqbyndf_vs_pt_allpid[iset]->SetStats(0);
        
        // dcaxy as a function of reconstructed quantities
          hdcaxy_vs_ptreco_allpid[iset] = new TH2D(
            Form("hdcaxy_vs_ptreco_allpid_%s", set_tag[iset].Data()),
            Form("dcaxy vs p_{T}, all PID, %s, %s;p_{T} (reco);dcaxy (cm)", vertexcuts_info, set_title[iset].Data()),
                                                   pt_bins.size()-1, pt_bins.data(), nbins_dcaxy, dcaxy_min, dcaxy_max
          );
          hdcaxy_vs_ptreco_allpid[iset]->SetStats(0);

          hdcaxy_vs_preco_allpid[iset] = new TH2D(
            Form("hdcaxy_vs_preco_allpid_%s", set_tag[iset].Data()),
            Form("dcaxy vs p, all PID, %s, %s;p (reco);dcaxy (cm)", vertexcuts_info, set_title[iset].Data()),
                                                  p_bins.size()-1, p_bins.data(), nbins_dcaxy, dcaxy_min, dcaxy_max
          );
          hdcaxy_vs_preco_allpid[iset]->SetStats(0);

          hdcaxy_vs_etareco_allpid[iset] = new TH2D(
            Form("hdcaxy_vs_etareco_allpid_%s", set_tag[iset].Data()),
            Form("dcaxy vs #eta, all PID, p_{T}>%g, %s, %s;#eta (reco);dcaxy (cm)", ptcutfordca_vsetaphi, vertexcuts_info, set_title[iset].Data()),
            nbins_eta, eta_min, eta_max, nbins_dcaxy, dcaxy_min, dcaxy_max
          );
          hdcaxy_vs_etareco_allpid[iset]->SetStats(0);

          hdcaxy_vs_phireco_allpid[iset] = new TH2D(
            Form("hdcaxy_vs_phireco_allpid_%s", set_tag[iset].Data()),
            Form("dcaxy vs #phi, all PID, p_{T}>%g,  %s, %s;#phi (reco);dcaxy (cm)", ptcutfordca_vsetaphi, vertexcuts_info, set_title[iset].Data()),
            nbins_phi, phi_min, phi_max, nbins_dcaxy, dcaxy_min, dcaxy_max
          );
          hdcaxy_vs_phireco_allpid[iset]->SetStats(0);

          // dcaz as a function of reconstructed quantities
          hdcaz_vs_ptreco_allpid[iset] = new TH2D(
            Form("hdcaz_vs_ptreco_allpid_%s", set_tag[iset].Data()),
            Form("dcaz vs p_{T}, all PID, %s, %s;p_{T} (reco);dcaz (cm)", vertexcuts_info, set_title[iset].Data()),
                                                  pt_bins.size()-1, pt_bins.data(), nbins_dcaz, dcaz_min, dcaz_max
          );
          hdcaz_vs_ptreco_allpid[iset]->SetStats(0);

          hdcaz_vs_preco_allpid[iset] = new TH2D(
            Form("hdcaz_vs_preco_allpid_%s", set_tag[iset].Data()),
            Form("dcaz vs p, all PID, %s, %s;p (reco);dcaz (cm)", vertexcuts_info, set_title[iset].Data()), p_bins.size()-1, p_bins.data(), nbins_dcaz, dcaz_min, dcaz_max);
          hdcaz_vs_preco_allpid[iset]->SetStats(0);

          hdcaz_vs_etareco_allpid[iset] = new TH2D(
            Form("hdcaz_vs_etareco_allpid_%s", set_tag[iset].Data()),
            Form("dcaz vs #eta, all PID, p_{T}>%g, %s, %s;#eta (reco);dcaz (cm)", ptcutfordca_vsetaphi, vertexcuts_info, set_title[iset].Data()),
            nbins_eta, eta_min, eta_max, nbins_dcaz, dcaz_min, dcaz_max
          );
          hdcaz_vs_etareco_allpid[iset]->SetStats(0);

          hdcaz_vs_phireco_allpid[iset] = new TH2D(
            Form("hdcaz_vs_phireco_allpid_%s", set_tag[iset].Data()),
            Form("dcaz vs #phi, all PID, p_{T}>%g, %s, %s;#phi (reco);dcaz (cm)", ptcutfordca_vsetaphi, vertexcuts_info, set_title[iset].Data()),
            nbins_phi, phi_min, phi_max, nbins_dcaz, dcaz_min, dcaz_max
          );
          hdcaz_vs_phireco_allpid[iset]->SetStats(0);
        
          // truth DCA histograms — allocated only when branches are present
          if (has_true_dca)
          {
            htruedcaxy_vs_ptreco_allpid[iset] = new TH2D(
              Form("htruedcaxy_vs_ptreco_allpid_%s", set_tag[iset].Data()),
              Form("true dcaxy vs p_{T}, all PID, %s, %s;p_{T} (reco);true_dcaxy (cm)", vertexcuts_info, set_title[iset].Data()), pt_bins.size()-1, pt_bins.data(), nbins_truedcaxy, dcaxy_min, dcaxy_max);
            htruedcaxy_vs_ptreco_allpid[iset]->SetStats(0);

            htruedcaxy_vs_preco_allpid[iset] = new TH2D(
              Form("htruedcaxy_vs_preco_allpid_%s", set_tag[iset].Data()),
              Form("true dcaxy vs p, all PID, %s, %s;p (reco);true_dcaxy (cm)", vertexcuts_info, set_title[iset].Data()), p_bins.size()-1, p_bins.data(), nbins_truedcaxy, dcaxy_min, dcaxy_max);
            htruedcaxy_vs_preco_allpid[iset]->SetStats(0);

            htruedcaxy_vs_etareco_allpid[iset] = new TH2D(
              Form("htruedcaxy_vs_etareco_allpid_%s", set_tag[iset].Data()),
              Form("true dcaxy vs #eta, all PID, p_{T}>%g, %s, %s;#eta (reco);true_dcaxy (cm)", ptcutfordca_vsetaphi, vertexcuts_info, set_title[iset].Data()),
              nbins_eta, eta_min, eta_max, nbins_truedcaxy, dcaxy_min, dcaxy_max
            );
            htruedcaxy_vs_etareco_allpid[iset]->SetStats(0);

            htruedcaxy_vs_phireco_allpid[iset] = new TH2D(
              Form("htruedcaxy_vs_phireco_allpid_%s", set_tag[iset].Data()),
              Form("true dcaxy vs #phi, all PID, p_{T}>%g, %s, %s;#phi (reco);true_dcaxy (cm)", ptcutfordca_vsetaphi, vertexcuts_info, set_title[iset].Data()),
              nbins_phi, phi_min, phi_max, nbins_truedcaxy, dcaxy_min, dcaxy_max
            );
            htruedcaxy_vs_phireco_allpid[iset]->SetStats(0);

            htruedcaz_vs_ptreco_allpid[iset] = new TH2D(
              Form("htruedcaz_vs_ptreco_allpid_%s", set_tag[iset].Data()),
              Form("true dcaz vs p_{T}, all PID, %s, %s;p_{T} (reco);true_dcaz (cm)", vertexcuts_info, set_title[iset].Data()),
                                                        pt_bins.size()-1, pt_bins.data(), nbins_truedcaz, dcaz_min, dcaz_max
            );
            htruedcaz_vs_ptreco_allpid[iset]->SetStats(0);

            htruedcaz_vs_preco_allpid[iset] = new TH2D(
              Form("htruedcaz_vs_preco_allpid_%s", set_tag[iset].Data()),
              Form("true dcaz vs p, all PID, %s, %s;p (reco);true_dcaz (cm)", vertexcuts_info, set_title[iset].Data()),
                                                       p_bins.size()-1, p_bins.data(), nbins_truedcaz, dcaz_min, dcaz_max
            );
            htruedcaz_vs_preco_allpid[iset]->SetStats(0);

            htruedcaz_vs_etareco_allpid[iset] = new TH2D(
              Form("htruedcaz_vs_etareco_allpid_%s", set_tag[iset].Data()),
              Form("true dcaz vs #eta, all PID, p_{T}>%g, %s, %s;#eta (reco);true_dcaz (cm)", ptcutfordca_vsetaphi, vertexcuts_info, set_title[iset].Data()),
              nbins_eta, eta_min, eta_max, nbins_truedcaz, dcaz_min, dcaz_max
            );
            htruedcaz_vs_etareco_allpid[iset]->SetStats(0);

            htruedcaz_vs_phireco_allpid[iset] = new TH2D(
              Form("htruedcaz_vs_phireco_allpid_%s", set_tag[iset].Data()),
              Form("true dcaz vs #phi, all PID, p_{T}>%g, %s, %s;#phi (reco);true_dcaz (cm)", ptcutfordca_vsetaphi, vertexcuts_info, set_title[iset].Data()),
              nbins_phi, phi_min, phi_max, nbins_truedcaz, dcaz_min, dcaz_max
            );
            htruedcaz_vs_phireco_allpid[iset]->SetStats(0);
          }
    }
    
    Long64_t n_entries=residualtree->GetEntries();
    if (n_entries == 0) {
        std::cerr << "[ERROR] No entries found in residualtree. "
                     "Check that the input files exist and match the pattern.\n";
        return 0;
    }
    
    for(Long64_t i=0;i<n_entries;i++){
        residualtree->GetEntry(i);
        
        if(vertex_ntracks <= vertex_ntracks_cut || fabs(vx)<fabs_vx_cut){continue;} //event level cuts should be true for all TrackSets
        
        float p   = p = std::sqrt(px*px + py*py + pz*pz);
        
        ////if(nmapsstate>=2||nintt>=1||pt>=0.2){
          const bool pass_nmaps = (nmaps >= nmaps_cut);
          const bool pass_intt = (nintt >= intt_cut);
          const bool pass_tpc = (ntpc >= ntpc_cut);
          const bool pass_pt = (pt >= pt_cut);
        
          bool pass_chisq_ndf = true;
          float chisq_ndf=0.0;
         
          if (has_chisq_info)
          {
            chisq_ndf = (ndf > 0.f) ? (chisq / ndf) :
            std::numeric_limits<float>::infinity();
            pass_chisq_ndf = (chisq_ndf < chisq_ndf_cut);
          }
        
          const bool no_truth_match   = has_truth_info && (has_truth_match < 1);
          const bool low_truth_purity = has_truth_info && (has_truth_match == 1 && truth_match_fraction < 0.5);
        
          const bool pass_selection =
              (pass_nmaps && pass_intt && pass_tpc && pass_pt && pass_chisq_ndf);
        
          const bool pass_selection_nochisqcut =
            (pass_nmaps && pass_intt && pass_tpc && pass_pt);

          bool fill_set[kNTrackSets] = {};

          fill_set[kAllTracks] = true;
          fill_set[kPassCuts] = pass_selection;
          fill_set[kFailnmapsCuts] = !pass_nmaps;
          fill_set[kFailinttCuts] = !pass_intt;
          fill_set[kFailntpcCuts] = !pass_tpc;
          fill_set[kFailptCuts] = !pass_pt;
          fill_set[kNoTruthMatch] = no_truth_match;
          fill_set[kLowTruthPurity] = low_truth_purity;
          fill_set[kPassCuts_NoChiSqCut] = pass_selection_nochisqcut;
          fill_set[kPasscuts_Charge1] = pass_selection && (charge==1);
          fill_set[kPasscuts_Chargem1] = pass_selection && (charge==-1);
        
          //All pid histograms
          for (int iset = 0; iset < kNTrackSets; ++iset)
          {
            if (!fill_set[iset])
            {
              continue;
            }
              
            hEta_allpid[iset]->Fill(eta);
            hPhi_allpid[iset]->Fill(phi);
            hPt_allpid[iset]->Fill(pt);
            hp_allpid[iset]->Fill(p);
            hnmapsstate_vs_phi_allpid[iset]->Fill(phi, nmapsstate);
            if (has_chisq_info){hchisqbyndf_vs_pt_allpid[iset]->Fill(pt, chisq_ndf);}
            
            hdcaxy_vs_ptreco_allpid[iset]->Fill(pt, dcaxy);
            hdcaxy_vs_preco_allpid[iset]->Fill(p, dcaxy);
            if(pt>=ptcutfordca_vsetaphi){hdcaxy_vs_etareco_allpid[iset]->Fill(eta, dcaxy);}
            if(pt>=ptcutfordca_vsetaphi){hdcaxy_vs_phireco_allpid[iset]->Fill(phi, dcaxy);}

            hdcaz_vs_ptreco_allpid[iset]->Fill(pt, dcaz);
            hdcaz_vs_preco_allpid[iset]->Fill(p, dcaz);
            if(pt>=ptcutfordca_vsetaphi){hdcaz_vs_etareco_allpid[iset]->Fill(eta, dcaz);}
            if(pt>=ptcutfordca_vsetaphi){hdcaz_vs_phireco_allpid[iset]->Fill(phi, dcaz);}
              
              // Fill truth-DCA histograms when branches are available
              if (has_true_dca)
              {
                htruedcaxy_vs_ptreco_allpid[iset]->Fill(pt,  true_dcaxy);
                htruedcaxy_vs_preco_allpid[iset] ->Fill(p,   true_dcaxy);
                if(pt>=ptcutfordca_vsetaphi){htruedcaxy_vs_etareco_allpid[iset]->Fill(eta, true_dcaxy);}
                if(pt>=ptcutfordca_vsetaphi){htruedcaxy_vs_phireco_allpid[iset]->Fill(phi, true_dcaxy);}

                htruedcaz_vs_ptreco_allpid[iset] ->Fill(pt,  true_dcaz);
                htruedcaz_vs_preco_allpid[iset]  ->Fill(p,   true_dcaz);
                if(pt>=ptcutfordca_vsetaphi){htruedcaz_vs_etareco_allpid[iset]->Fill(eta, true_dcaz);}
                if(pt>=ptcutfordca_vsetaphi){htruedcaz_vs_phireco_allpid[iset]->Fill(phi, true_dcaz);}
              }
          }
        
    }
    
    for (int iset = 0; iset < kNTrackSets; ++iset)
    {
        GetFitSlicesY(
          hnmapsstate_vs_phi_allpid[iset],
          fgaus_nmaps,
          hnmapsstate_Mean_vs_phi_allpid[iset],
          hnmapsstate_Width_vs_phi_allpid[iset],
          Form("hnmapsstate_vs_phi_allpid, %s %s", vertexcuts_info, set_tag[iset].Data()),
          "#phi",
          "nmapsstate"
        );
        rename_fit_slices(hnmapsstate_Mean_vs_phi_allpid[iset], hnmapsstate_Width_vs_phi_allpid[iset],
                          "hnmapsstate_vs_phi_allpid", set_tag[iset]);
        
        if (has_chisq_info){
            GetFitSlicesY(
                          hchisqbyndf_vs_pt_allpid[iset],
                          fgaus_chisq,
                          hchisqbyndf_Mean_vs_pt_allpid[iset],
                          hchisqbyndf_Width_vs_pt_allpid[iset],
                          Form("hchisqbyndf_vs_pt_allpid, %s %s", vertexcuts_info, set_tag[iset].Data()),
                          "p_{T}",
                          "chisq/ndf"
                          );
            rename_fit_slices(hchisqbyndf_Mean_vs_pt_allpid[iset], hchisqbyndf_Width_vs_pt_allpid[iset],
                              "hchisqbyndf_vs_pt_allpid", set_tag[iset]);
        }
        
        GetFitSlicesY(
          hdcaxy_vs_ptreco_allpid[iset],
          fgaus_dcaxy,
          hdcaxy_Mean_vs_ptreco_allpid[iset],
          hdcaxy_Width_vs_ptreco_allpid[iset],
          Form("dcaxy_vs_ptreco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
          "p_{T} (reco) (GeV/c)",
          "dcaxy (cm)"
        );
        rename_fit_slices(hdcaxy_Mean_vs_ptreco_allpid[iset], hdcaxy_Width_vs_ptreco_allpid[iset],
                          "dcaxy_vs_ptreco_allpid", set_tag[iset]);

        GetFitSlicesY(
          hdcaxy_vs_preco_allpid[iset],
          fgaus_dcaxy,
          hdcaxy_Mean_vs_preco_allpid[iset],
          hdcaxy_Width_vs_preco_allpid[iset],
          Form("dcaxy_vs_preco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
          "p (reco) (GeV/c)",
          "dcaxy (cm)"
        );
        rename_fit_slices(hdcaxy_Mean_vs_preco_allpid[iset], hdcaxy_Width_vs_preco_allpid[iset],
                          "dcaxy_vs_preco_allpid", set_tag[iset]);

        GetFitSlicesY(
          hdcaxy_vs_etareco_allpid[iset],
          fgaus_dcaxy,
          hdcaxy_Mean_vs_etareco_allpid[iset],
          hdcaxy_Width_vs_etareco_allpid[iset],
          Form("dcaxy_vs_etareco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
          "#eta (reco)",
          "dcaxy (cm)"
        );
        rename_fit_slices(hdcaxy_Mean_vs_etareco_allpid[iset], hdcaxy_Width_vs_etareco_allpid[iset],
                          "dcaxy_vs_etareco_allpid", set_tag[iset]);

        GetFitSlicesY(
          hdcaxy_vs_phireco_allpid[iset],
          fgaus_dcaxy,
          hdcaxy_Mean_vs_phireco_allpid[iset],
          hdcaxy_Width_vs_phireco_allpid[iset],
          Form("dcaxy_vs_phireco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
          "#phi (reco)",
          "dcaxy (cm)"
        );
        rename_fit_slices(hdcaxy_Mean_vs_phireco_allpid[iset], hdcaxy_Width_vs_phireco_allpid[iset],
                          "dcaxy_vs_phireco_allpid", set_tag[iset]);

        GetFitSlicesY(
          hdcaz_vs_ptreco_allpid[iset],
          fgaus_dcaz,
          hdcaz_Mean_vs_ptreco_allpid[iset],
          hdcaz_Width_vs_ptreco_allpid[iset],
          Form("dcaz_vs_ptreco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
          "p_{T} (reco) (GeV/c)",
          "dcaz (cm)"
        );
        rename_fit_slices(hdcaz_Mean_vs_ptreco_allpid[iset], hdcaz_Width_vs_ptreco_allpid[iset],
                          "dcaz_vs_ptreco_allpid", set_tag[iset]);

        GetFitSlicesY(
          hdcaz_vs_preco_allpid[iset],
          fgaus_dcaz,
          hdcaz_Mean_vs_preco_allpid[iset],
          hdcaz_Width_vs_preco_allpid[iset],
          Form("dcaz_vs_preco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
          "p (reco) (GeV/c)",
          "dcaz (cm)"
        );
        rename_fit_slices(hdcaz_Mean_vs_preco_allpid[iset], hdcaz_Width_vs_preco_allpid[iset],
                          "dcaz_vs_preco_allpid", set_tag[iset]);

        GetFitSlicesY(
          hdcaz_vs_etareco_allpid[iset],
          fgaus_dcaz,
          hdcaz_Mean_vs_etareco_allpid[iset],
          hdcaz_Width_vs_etareco_allpid[iset],
          Form("dcaz_vs_etareco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
          "#eta (reco)",
          "dcaz (cm)"
        );
        rename_fit_slices(hdcaz_Mean_vs_etareco_allpid[iset], hdcaz_Width_vs_etareco_allpid[iset],
                          "dcaz_vs_etareco_allpid", set_tag[iset]);

        GetFitSlicesY(
          hdcaz_vs_phireco_allpid[iset],
          fgaus_dcaz,
          hdcaz_Mean_vs_phireco_allpid[iset],
          hdcaz_Width_vs_phireco_allpid[iset],
          Form("dcaz_vs_phireco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
          "#phi (reco)",
          "dcaz (cm)"
        );
        rename_fit_slices(hdcaz_Mean_vs_phireco_allpid[iset], hdcaz_Width_vs_phireco_allpid[iset],
                          "dcaz_vs_phireco_allpid", set_tag[iset]);

        // Fit slices for truth-DCA histograms
        if (has_true_dca)
        {
          GetFitSlicesY(
            htruedcaxy_vs_ptreco_allpid[iset], fgaus_dcaxy,
            htruedcaxy_Mean_vs_ptreco_allpid[iset], htruedcaxy_Width_vs_ptreco_allpid[iset],
            Form("truedcaxy_vs_ptreco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
            "p_{T} (reco) (GeV/c)", "true_dcaxy (cm)"
          );
          rename_fit_slices(htruedcaxy_Mean_vs_ptreco_allpid[iset], htruedcaxy_Width_vs_ptreco_allpid[iset],
                            "truedcaxy_vs_ptreco_allpid", set_tag[iset]);
          GetFitSlicesY(
            htruedcaxy_vs_preco_allpid[iset], fgaus_dcaxy,
            htruedcaxy_Mean_vs_preco_allpid[iset], htruedcaxy_Width_vs_preco_allpid[iset],
            Form("truedcaxy_vs_preco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
            "p (reco) (GeV/c)", "true_dcaxy (cm)"
          );
          rename_fit_slices(htruedcaxy_Mean_vs_preco_allpid[iset], htruedcaxy_Width_vs_preco_allpid[iset],
                            "truedcaxy_vs_preco_allpid", set_tag[iset]);
          GetFitSlicesY(
            htruedcaxy_vs_etareco_allpid[iset], fgaus_dcaxy,
            htruedcaxy_Mean_vs_etareco_allpid[iset], htruedcaxy_Width_vs_etareco_allpid[iset],
            Form("truedcaxy_vs_etareco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
            "#eta (reco)", "true_dcaxy (cm)"
          );
          rename_fit_slices(htruedcaxy_Mean_vs_etareco_allpid[iset], htruedcaxy_Width_vs_etareco_allpid[iset],
                            "truedcaxy_vs_etareco_allpid", set_tag[iset]);
          GetFitSlicesY(
            htruedcaxy_vs_phireco_allpid[iset], fgaus_dcaxy,
            htruedcaxy_Mean_vs_phireco_allpid[iset], htruedcaxy_Width_vs_phireco_allpid[iset],
            Form("truedcaxy_vs_phireco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
            "#phi (reco)", "true_dcaxy (cm)"
          );
          rename_fit_slices(htruedcaxy_Mean_vs_phireco_allpid[iset], htruedcaxy_Width_vs_phireco_allpid[iset],
                            "truedcaxy_vs_phireco_allpid", set_tag[iset]);

          GetFitSlicesY(
            htruedcaz_vs_ptreco_allpid[iset], fgaus_dcaz,
            htruedcaz_Mean_vs_ptreco_allpid[iset], htruedcaz_Width_vs_ptreco_allpid[iset],
            Form("truedcaz_vs_ptreco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
            "p_{T} (reco) (GeV/c)", "true_dcaz (cm)"
          );
          rename_fit_slices(htruedcaz_Mean_vs_ptreco_allpid[iset], htruedcaz_Width_vs_ptreco_allpid[iset],
                            "truedcaz_vs_ptreco_allpid", set_tag[iset]);
          GetFitSlicesY(
            htruedcaz_vs_preco_allpid[iset], fgaus_dcaz,
            htruedcaz_Mean_vs_preco_allpid[iset], htruedcaz_Width_vs_preco_allpid[iset],
            Form("truedcaz_vs_preco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
            "p (reco) (GeV/c)", "true_dcaz (cm)"
          );
          rename_fit_slices(htruedcaz_Mean_vs_preco_allpid[iset], htruedcaz_Width_vs_preco_allpid[iset],
                            "truedcaz_vs_preco_allpid", set_tag[iset]);
          GetFitSlicesY(
            htruedcaz_vs_etareco_allpid[iset], fgaus_dcaz,
            htruedcaz_Mean_vs_etareco_allpid[iset], htruedcaz_Width_vs_etareco_allpid[iset],
            Form("truedcaz_vs_etareco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
            "#eta (reco)", "true_dcaz (cm)"
          );
          rename_fit_slices(htruedcaz_Mean_vs_etareco_allpid[iset], htruedcaz_Width_vs_etareco_allpid[iset],
                            "truedcaz_vs_etareco_allpid", set_tag[iset]);
          GetFitSlicesY(
            htruedcaz_vs_phireco_allpid[iset], fgaus_dcaz,
            htruedcaz_Mean_vs_phireco_allpid[iset], htruedcaz_Width_vs_phireco_allpid[iset],
            Form("truedcaz_vs_phireco_allpid, %s, %s", vertexcuts_info, set_title[iset].Data()),
            "#phi (reco)", "true_dcaz (cm)"
          );
          rename_fit_slices(htruedcaz_Mean_vs_phireco_allpid[iset], htruedcaz_Width_vs_phireco_allpid[iset],
                            "truedcaz_vs_phireco_allpid", set_tag[iset]);
        }
    }

    
    TFile *fout = new TFile(outputrootfilename, "RECREATE");
    
    for (int iset = 0; iset < kNTrackSets; ++iset)
    {
        hEta_allpid[iset]->Write();
        hPhi_allpid[iset]->Write();
        hPt_allpid[iset]->Write();
        hp_allpid[iset]->Write();
        
        hnmapsstate_vs_phi_allpid[iset]->Write();
        hnmapsstate_Mean_vs_phi_allpid[iset]->Write();
        hnmapsstate_Width_vs_phi_allpid[iset]->Write();
        
        hchisqbyndf_vs_pt_allpid[iset]->Write();
        if (has_chisq_info)
        {
            hchisqbyndf_Mean_vs_pt_allpid[iset]->Write();
            hchisqbyndf_Width_vs_pt_allpid[iset]->Write();
        }
        
        //dcaxy as a function of reconstructed quantities
        hdcaxy_vs_ptreco_allpid[iset]->Write();
        hdcaxy_Mean_vs_ptreco_allpid[iset]->Write();
        hdcaxy_Width_vs_ptreco_allpid[iset]->Write();
        
        hdcaxy_vs_preco_allpid[iset]->Write();
        hdcaxy_Mean_vs_preco_allpid[iset]->Write();
        hdcaxy_Width_vs_preco_allpid[iset]->Write();
        
        hdcaxy_vs_etareco_allpid[iset]->Write();
        hdcaxy_Mean_vs_etareco_allpid[iset]->Write();
        hdcaxy_Width_vs_etareco_allpid[iset]->Write();
        
        hdcaxy_vs_phireco_allpid[iset]->Write();
        hdcaxy_Mean_vs_phireco_allpid[iset]->Write();
        hdcaxy_Width_vs_phireco_allpid[iset]->Write();
        
        //dcaz as a function of reconstructed quantities
        hdcaz_vs_ptreco_allpid[iset]->Write();
        hdcaz_Mean_vs_ptreco_allpid[iset]->Write();
        hdcaz_Width_vs_ptreco_allpid[iset]->Write();
        
        hdcaz_vs_preco_allpid[iset]->Write();
        hdcaz_Mean_vs_preco_allpid[iset]->Write();
        hdcaz_Width_vs_preco_allpid[iset]->Write();
        
        hdcaz_vs_etareco_allpid[iset]->Write();
        hdcaz_Mean_vs_etareco_allpid[iset]->Write();
        hdcaz_Width_vs_etareco_allpid[iset]->Write();
        
        hdcaz_vs_phireco_allpid[iset]->Write();
        hdcaz_Mean_vs_phireco_allpid[iset]->Write();
        hdcaz_Width_vs_phireco_allpid[iset]->Write();
        
        // Write truth-DCA histograms when available
        if (has_true_dca)
        {
            htruedcaxy_vs_ptreco_allpid[iset]->Write();
            htruedcaxy_Mean_vs_ptreco_allpid[iset]->Write();
            htruedcaxy_Width_vs_ptreco_allpid[iset]->Write();

            htruedcaxy_vs_preco_allpid[iset]->Write();
            htruedcaxy_Mean_vs_preco_allpid[iset]->Write();
            htruedcaxy_Width_vs_preco_allpid[iset]->Write();

            htruedcaxy_vs_etareco_allpid[iset]->Write();
            htruedcaxy_Mean_vs_etareco_allpid[iset]->Write();
            htruedcaxy_Width_vs_etareco_allpid[iset]->Write();

            htruedcaxy_vs_phireco_allpid[iset]->Write();
            htruedcaxy_Mean_vs_phireco_allpid[iset]->Write();
            htruedcaxy_Width_vs_phireco_allpid[iset]->Write();

            htruedcaz_vs_ptreco_allpid[iset]->Write();
            htruedcaz_Mean_vs_ptreco_allpid[iset]->Write();
            htruedcaz_Width_vs_ptreco_allpid[iset]->Write();

            htruedcaz_vs_preco_allpid[iset]->Write();
            htruedcaz_Mean_vs_preco_allpid[iset]->Write();
            htruedcaz_Width_vs_preco_allpid[iset]->Write();

            htruedcaz_vs_etareco_allpid[iset]->Write();
            htruedcaz_Mean_vs_etareco_allpid[iset]->Write();
            htruedcaz_Width_vs_etareco_allpid[iset]->Write();

            htruedcaz_vs_phireco_allpid[iset]->Write();
            htruedcaz_Mean_vs_phireco_allpid[iset]->Write();
            htruedcaz_Width_vs_phireco_allpid[iset]->Write();
        }
    }
    
    fout->Close();
    cout<<Form("%s has been created",outputrootfilename)<<endl;
    if(createpdffile){
        SaveAllHistogramsToPdf(outputrootfilename);
    }
//    if(createCanvases){
//        
//        //dcaxy as a function of truth quantities
//        CreateandSaveCanvaswithPID(plots_outputdir,"particles_overlaidplots/canvas_dcaxyfits_vstruth",hdcaxy_Mean_vs_pttrue,hdcaxy_Width_vs_pttrue,hdcaxy_Mean_vs_ptrue,hdcaxy_Width_vs_ptrue,hdcaxy_Mean_vs_etatrue,hdcaxy_Width_vs_etatrue,hdcaxy_Mean_vs_phitrue,hdcaxy_Width_vs_phitrue, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, trackcuts_info, dcaxy_max_formeanwidthcalc);
//        
//        //dcaxy as a function of reconstructed quantities
//        CreateandSaveCanvaswithPID(plots_outputdir,"particles_overlaidplots/canvas_dcaxyfits_vsreco",hdcaxy_Mean_vs_ptreco,hdcaxy_Width_vs_ptreco,hdcaxy_Mean_vs_preco,hdcaxy_Width_vs_preco,hdcaxy_Mean_vs_etareco,hdcaxy_Width_vs_etareco,hdcaxy_Mean_vs_phireco,hdcaxy_Width_vs_phireco, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, trackcuts_info, dcaxy_max_formeanwidthcalc);
//        
//        //dcaz as a function of truth quantities
//        CreateandSaveCanvaswithPID(plots_outputdir,"particles_overlaidplots/canvas_dcazfits_vstruth",hdcaz_Mean_vs_pttrue,hdcaz_Width_vs_pttrue,hdcaz_Mean_vs_ptrue,hdcaz_Width_vs_ptrue,hdcaz_Mean_vs_etatrue,hdcaz_Width_vs_etatrue,hdcaz_Mean_vs_phitrue,hdcaz_Width_vs_phitrue, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, trackcuts_info, dcaz_max_formeanwidthcalc);
//        
//        //dcaz as a function of reconstructed quantities
//        CreateandSaveCanvaswithPID(plots_outputdir,"particles_overlaidplots/canvas_dcazfits_vsreco",hdcaz_Mean_vs_ptreco,hdcaz_Width_vs_ptreco,hdcaz_Mean_vs_preco,hdcaz_Width_vs_preco,hdcaz_Mean_vs_etareco,hdcaz_Width_vs_etareco,hdcaz_Mean_vs_phireco,hdcaz_Width_vs_phireco, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, trackcuts_info, dcaz_max_formeanwidthcalc);
//        
//        //truedcaxy as a function of truth quantities
//        CreateandSaveCanvaswithPID(plots_outputdir,"particles_overlaidplots/canvas_truedcaxyfits_vstruth",htruedcaxy_Mean_vs_pttrue,htruedcaxy_Width_vs_pttrue,htruedcaxy_Mean_vs_ptrue,htruedcaxy_Width_vs_ptrue,htruedcaxy_Mean_vs_etatrue,htruedcaxy_Width_vs_etatrue,htruedcaxy_Mean_vs_phitrue,htruedcaxy_Width_vs_phitrue, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, trackcuts_info, truedcaxy_max_formeanwidthcalc);
//        
//        //truedcaxy as a function of reconstructed quantities
//        CreateandSaveCanvaswithPID(plots_outputdir,"particles_overlaidplots/canvas_truedcaxyfits_vsreco",htruedcaxy_Mean_vs_ptreco,htruedcaxy_Width_vs_ptreco,htruedcaxy_Mean_vs_preco,htruedcaxy_Width_vs_preco,htruedcaxy_Mean_vs_etareco,htruedcaxy_Width_vs_etareco,htruedcaxy_Mean_vs_phireco,htruedcaxy_Width_vs_phireco, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, trackcuts_info, truedcaxy_max_formeanwidthcalc);
//        
//        //truedcaz as a function of truth quantities
//        CreateandSaveCanvaswithPID(plots_outputdir,"particles_overlaidplots/canvas_truedcazfits_vstruth",htruedcaz_Mean_vs_pttrue,htruedcaz_Width_vs_pttrue,htruedcaz_Mean_vs_ptrue,htruedcaz_Width_vs_ptrue,htruedcaz_Mean_vs_etatrue,htruedcaz_Width_vs_etatrue,htruedcaz_Mean_vs_phitrue,htruedcaz_Width_vs_phitrue, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, trackcuts_info, truedcaz_max_formeanwidthcalc);
//        
//        //truedcaz as a function of reconstructed quantities
//        CreateandSaveCanvaswithPID(plots_outputdir,"particles_overlaidplots/canvas_truedcazfits_vsreco",htruedcaz_Mean_vs_ptreco,htruedcaz_Width_vs_ptreco,htruedcaz_Mean_vs_preco,htruedcaz_Width_vs_preco,htruedcaz_Mean_vs_etareco,htruedcaz_Width_vs_etareco,htruedcaz_Mean_vs_phireco,htruedcaz_Width_vs_phireco, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, trackcuts_info, truedcaz_max_formeanwidthcalc);
//    }
    
    return 1;
    
}
