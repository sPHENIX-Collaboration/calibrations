#include "UsefulFunctions.h"

int MakeHistograms_withPID_Simulation(){
//    TFile *File=new TFile("output_SimpleEvtGenerator/residuals_G4sPHENIX_SimpleEvtGenerator_all.root");
//    TFile *File=new TFile("residuals_G4sPHENIX_34930evts.root");
//    const char* description="Simple Event Generator (2 #pi^{+} + 2 #pi^{-} )";
//    const char* outputname="SimpleEvtGenerator";
//    const char* outputdir="Plots";
//    const char* cuts_info="Cuts: m_nmaps>1 && m_nintt>0 && m_ntpc>25 && vertex_ntracks > 3";
//    float title_xstart=0.3, title_ystart=0.95;
    
        //TFile *File=new TFile("residuals_G4sPHENIX_3499.root");
        //TFile *File=new TFile("residuals_G4sPHENIX_Pythia_105kevts_withtruedca.root");
        //TTree *residualtree = (TTree*)File->Get("residualtree");
    
        const char* outputrootfilename="output_PlottingMacro_withPID_Simulation_subset.root";
    
        TChain *residualtree = new TChain("residualtree");
        residualtree->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_105kevts_withtruedca.root");
        //residualtree->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_175kevts_withtruedca_set*.root");
        //residualtree->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_200kevts_withtruedca_set*.root");
    
        const char* description="Pythia 8 Simulation";
        const char* outputname="Pythia8";
        const char* outputdir="Plots_current";
        //const char* outputdir="TestingPath";
        const char* cuts_info="Cuts: m_nmaps>1 && m_nintt>0 && m_ntpc>25 && pt>=0.2 && vertex_ntracks > 3";
        //const char* cuts_info="Cuts: nmapsstate>=2 && m_nintt>=1 && pt>=0.2";
        float title_xstart=0.4, title_ystart=0.95;
        bool createCanvases=false;
    
    
    
    float dcaxy=0, dcaz=0, true_dcaxy=0,true_dcaz=0, px=0, py=0, pz=0, pt=0,eta=0,phi=0,vx=0,vy=0,vz=0; //quantities obtained from residualtree
    int nmaps=0, nmapsstate=0, nintt=0, ntpc=0, vertex_ntracks=0,vertex_crossing=0, gflavor=0;
    float true_px=0, true_py=0, true_pz=0, true_vx=0, true_vy=0, true_vz=0, true_t=0;
    float dcaxy_min=-0.01, dcaxy_max=0.01, dcaz_min=-0.01, dcaz_max=0.01;
    float truedcaxy_min=-0.01, truedcaxy_max=0.01, truedcaz_min=-0.01, truedcaz_max=0.01;
    float dcaxy_min_formeanwidthcalc=-0.01, dcaxy_max_formeanwidthcalc=0.01, dcaz_min_formeanwidthcalc=-0.01, dcaz_max_formeanwidthcalc=0.01;
    float truedcaxy_min_formeanwidthcalc=-0.01, truedcaxy_max_formeanwidthcalc=0.01, truedcaz_min_formeanwidthcalc=-0.01, truedcaz_max_formeanwidthcalc=0.01;
    float ptRes_min_formeanwidthcalc=-0.03, ptRes_max_formeanwidthcalc=0.03, pRes_min_formeanwidthcalc=-0.03, pRes_max_formeanwidthcalc=0.03;
    float pt_min=0.0, pt_max=5.5;
    float p_min=0.0, p_max=5.5;
    float eta_min=-1, eta_max=1;
    float phi_min=-3.4, phi_max=3.4;
    float ptRes_min=-0.03, ptRes_max=0.03;
    float pRes_min=-0.03, pRes_max=0.03;
    
    int nbins_pt=150, nbins_p=150, nbins_eta=150, nbins_phi=150, nbins_dcaxy=400, nbins_dcaz=400, nbins_truedcaxy=400, nbins_truedcaz=400, nbins_ptRes=100, nbins_pRes=100;
    
    TF1* fgaus_dcaxy=new TF1("fgaus_dcaxy","gaus",dcaxy_min_formeanwidthcalc, dcaxy_max_formeanwidthcalc);
    TF1* fgaus_dcaz=new TF1("fgaus_dcaz","gaus",dcaz_min_formeanwidthcalc, dcaz_max_formeanwidthcalc);
    TF1* fgaus_truedcaxy=new TF1("fgaus_truedcaxy","gaus",truedcaxy_min_formeanwidthcalc, truedcaxy_max_formeanwidthcalc);
    TF1* fgaus_truedcaz=new TF1("fgaus_truedcaz","gaus",truedcaz_min_formeanwidthcalc, truedcaz_max_formeanwidthcalc);
    TF1* fgaus_ptRes = new TF1("fgaus_ptRes", "gaus", ptRes_min_formeanwidthcalc, ptRes_max_formeanwidthcalc);
    TF1* fgaus_pRes = new TF1("fgaus_pRes", "gaus", pRes_min_formeanwidthcalc, pRes_max_formeanwidthcalc);
    TF1* fgaus_nmaps = new TF1("fgaus_nmaps", "gaus", 0, 6);
    
    
    //TF1 *fit1 = new TF1("fit1", "pol1");
    
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
    residualtree->SetBranchAddress("true_dcaxy", &true_dcaxy);
    residualtree->SetBranchAddress("true_dcaz", &true_dcaz);
    residualtree->SetBranchAddress("vertex_crossing", &vertex_crossing);
    residualtree->SetBranchAddress("vx", &vx);
    residualtree->SetBranchAddress("vy", &vy);
    residualtree->SetBranchAddress("vz", &vz);
    residualtree->SetBranchAddress("gflavor", &gflavor);
    residualtree->SetBranchAddress("true_px", &true_px);
    residualtree->SetBranchAddress("true_py", &true_py);
    residualtree->SetBranchAddress("true_pz", &true_pz);
    residualtree->SetBranchAddress("true_vx", &true_vx);
    residualtree->SetBranchAddress("true_vy", &true_vy);
    residualtree->SetBranchAddress("true_vz", &true_vz);
    residualtree->SetBranchAddress("true_t", &true_t);
    
    
    
    const int nParticles=6;
    
    std::vector<std::string> particles;
    std::vector<int> particles_PID;
    std::vector<double> particles_mass;
    
    particles.push_back("pi+");
    particles_PID.push_back(211);
    particles_mass.push_back(0.13957039);
    
    particles.push_back("pi-");
    particles_PID.push_back(-211);
    particles_mass.push_back(0.13957039);
    
    particles.push_back("K+");
    particles_PID.push_back(321);
    particles_mass.push_back(0.493696);
    
    particles.push_back("K-");
    particles_PID.push_back(-321);
    particles_mass.push_back(0.493696);
    
    particles.push_back("p");
    particles_PID.push_back(2212);
    particles_mass.push_back(0.93827208816);
    
    particles.push_back("#bar{p}");
    particles_PID.push_back(-2212);
    particles_mass.push_back(0.93827208816);
    
    TH1D* hEta_allparticles; TH1D* hPhi_allparticles; TH1D* hPt_allparticles; TH1D* hp_allparticles;
    
    std::vector<TH1D*> hEta(nParticles); std::vector<TH1D*> hPhi(nParticles); std::vector<TH1D*> hPt(nParticles); std::vector<TH1D*> hp(nParticles);
    
    TH2D* hnmapsstate_vs_phi_allparticles;
    TH1D* hnmapsstate_Mean_vs_phi_allparticles; TH1D* hnmapsstate_Width_vs_phi_allparticles;
    
    std::vector<TH2D*> hnmapsstate_vs_phi(nParticles);
    std::vector<TH1D*> hnmapsstate_Mean_vs_phi(nParticles); std::vector<TH1D*> hnmapsstate_Width_vs_phi(nParticles);
    
    //dcaxy as a function of truth quantities
    TH2D* hdcaxy_vs_pttrue_allparticles; TH2D* hdcaxy_vs_ptrue_allparticles; TH2D* hdcaxy_vs_etatrue_allparticles; TH2D* hdcaxy_vs_phitrue_allparticles;
    
    TH1D* hdcaxy_Mean_vs_pttrue_allparticles; TH1D* hdcaxy_Mean_vs_ptrue_allparticles; TH1D* hdcaxy_Mean_vs_etatrue_allparticles; TH1D* hdcaxy_Mean_vs_phitrue_allparticles;
    
    TH1D* hdcaxy_Width_vs_pttrue_allparticles; TH1D* hdcaxy_Width_vs_ptrue_allparticles; TH1D* hdcaxy_Width_vs_etatrue_allparticles; TH1D* hdcaxy_Width_vs_phitrue_allparticles;
    
    std::vector<TH2D*> hdcaxy_vs_pttrue(nParticles); std::vector<TH2D*> hdcaxy_vs_ptrue(nParticles); std::vector<TH2D*> hdcaxy_vs_etatrue(nParticles); std::vector<TH2D*> hdcaxy_vs_phitrue(nParticles);
    
    std::vector<TH1D*> hdcaxy_Mean_vs_pttrue(nParticles); std::vector<TH1D*> hdcaxy_Mean_vs_ptrue(nParticles); std::vector<TH1D*> hdcaxy_Mean_vs_etatrue(nParticles); std::vector<TH1D*> hdcaxy_Mean_vs_phitrue(nParticles);
    
    std::vector<TH1D*> hdcaxy_Width_vs_pttrue(nParticles); std::vector<TH1D*> hdcaxy_Width_vs_ptrue(nParticles); std::vector<TH1D*> hdcaxy_Width_vs_etatrue(nParticles); std::vector<TH1D*> hdcaxy_Width_vs_phitrue(nParticles);
    
    //dcaxy as a function of reconstructed quantities
    TH2D* hdcaxy_vs_ptreco_allparticles; TH2D* hdcaxy_vs_preco_allparticles; TH2D* hdcaxy_vs_etareco_allparticles; TH2D* hdcaxy_vs_phireco_allparticles;
    
    TH1D* hdcaxy_Mean_vs_ptreco_allparticles; TH1D* hdcaxy_Mean_vs_preco_allparticles; TH1D* hdcaxy_Mean_vs_etareco_allparticles; TH1D* hdcaxy_Mean_vs_phireco_allparticles;
    
    TH1D* hdcaxy_Width_vs_ptreco_allparticles; TH1D* hdcaxy_Width_vs_preco_allparticles; TH1D* hdcaxy_Width_vs_etareco_allparticles; TH1D* hdcaxy_Width_vs_phireco_allparticles;
    
    std::vector<TH2D*> hdcaxy_vs_ptreco(nParticles); std::vector<TH2D*> hdcaxy_vs_preco(nParticles); std::vector<TH2D*> hdcaxy_vs_etareco(nParticles); std::vector<TH2D*> hdcaxy_vs_phireco(nParticles);
    
    std::vector<TH1D*> hdcaxy_Mean_vs_ptreco(nParticles); std::vector<TH1D*> hdcaxy_Mean_vs_preco(nParticles); std::vector<TH1D*> hdcaxy_Mean_vs_etareco(nParticles); std::vector<TH1D*> hdcaxy_Mean_vs_phireco(nParticles);
    
    std::vector<TH1D*> hdcaxy_Width_vs_ptreco(nParticles); std::vector<TH1D*> hdcaxy_Width_vs_preco(nParticles); std::vector<TH1D*> hdcaxy_Width_vs_etareco(nParticles); std::vector<TH1D*> hdcaxy_Width_vs_phireco(nParticles);
    
    //dcaz as a function of truth quantities
    TH2D* hdcaz_vs_pttrue_allparticles; TH2D* hdcaz_vs_ptrue_allparticles; TH2D* hdcaz_vs_etatrue_allparticles; TH2D* hdcaz_vs_phitrue_allparticles;
    
    TH1D* hdcaz_Mean_vs_pttrue_allparticles; TH1D* hdcaz_Mean_vs_ptrue_allparticles; TH1D* hdcaz_Mean_vs_etatrue_allparticles; TH1D* hdcaz_Mean_vs_phitrue_allparticles;
    
    TH1D* hdcaz_Width_vs_pttrue_allparticles; TH1D* hdcaz_Width_vs_ptrue_allparticles; TH1D* hdcaz_Width_vs_etatrue_allparticles; TH1D* hdcaz_Width_vs_phitrue_allparticles;
    
    std::vector<TH2D*> hdcaz_vs_pttrue(nParticles); std::vector<TH2D*> hdcaz_vs_ptrue(nParticles); std::vector<TH2D*> hdcaz_vs_etatrue(nParticles); std::vector<TH2D*> hdcaz_vs_phitrue(nParticles);
    
    std::vector<TH1D*> hdcaz_Mean_vs_pttrue(nParticles); std::vector<TH1D*> hdcaz_Mean_vs_ptrue(nParticles); std::vector<TH1D*> hdcaz_Mean_vs_etatrue(nParticles); std::vector<TH1D*> hdcaz_Mean_vs_phitrue(nParticles);
    
    std::vector<TH1D*> hdcaz_Width_vs_pttrue(nParticles); std::vector<TH1D*> hdcaz_Width_vs_ptrue(nParticles); std::vector<TH1D*> hdcaz_Width_vs_etatrue(nParticles); std::vector<TH1D*> hdcaz_Width_vs_phitrue(nParticles);
    
    
    
    //dcaz as a function of reconstructed quantities
    TH2D* hdcaz_vs_ptreco_allparticles; TH2D* hdcaz_vs_preco_allparticles; TH2D* hdcaz_vs_etareco_allparticles; TH2D* hdcaz_vs_phireco_allparticles;
    
    TH1D* hdcaz_Mean_vs_ptreco_allparticles; TH1D* hdcaz_Mean_vs_preco_allparticles; TH1D* hdcaz_Mean_vs_etareco_allparticles; TH1D* hdcaz_Mean_vs_phireco_allparticles;
    
    TH1D* hdcaz_Width_vs_ptreco_allparticles; TH1D* hdcaz_Width_vs_preco_allparticles; TH1D* hdcaz_Width_vs_etareco_allparticles; TH1D* hdcaz_Width_vs_phireco_allparticles;
    
    std::vector<TH2D*> hdcaz_vs_ptreco(nParticles); std::vector<TH2D*> hdcaz_vs_preco(nParticles); std::vector<TH2D*> hdcaz_vs_etareco(nParticles); std::vector<TH2D*> hdcaz_vs_phireco(nParticles);
    
    std::vector<TH1D*> hdcaz_Mean_vs_ptreco(nParticles); std::vector<TH1D*> hdcaz_Mean_vs_preco(nParticles); std::vector<TH1D*> hdcaz_Mean_vs_etareco(nParticles); std::vector<TH1D*> hdcaz_Mean_vs_phireco(nParticles);
    
    std::vector<TH1D*> hdcaz_Width_vs_ptreco(nParticles); std::vector<TH1D*> hdcaz_Width_vs_preco(nParticles); std::vector<TH1D*> hdcaz_Width_vs_etareco(nParticles); std::vector<TH1D*> hdcaz_Width_vs_phireco(nParticles);
    
    //truedcaxy as a function of truth quantities
    TH2D* htruedcaxy_vs_pttrue_allparticles; TH2D* htruedcaxy_vs_ptrue_allparticles; TH2D* htruedcaxy_vs_etatrue_allparticles; TH2D* htruedcaxy_vs_phitrue_allparticles;
    
    TH1D* htruedcaxy_Mean_vs_pttrue_allparticles; TH1D* htruedcaxy_Mean_vs_ptrue_allparticles; TH1D* htruedcaxy_Mean_vs_etatrue_allparticles; TH1D* htruedcaxy_Mean_vs_phitrue_allparticles;
    
    TH1D* htruedcaxy_Width_vs_pttrue_allparticles; TH1D* htruedcaxy_Width_vs_ptrue_allparticles; TH1D* htruedcaxy_Width_vs_etatrue_allparticles; TH1D* htruedcaxy_Width_vs_phitrue_allparticles;
    
    std::vector<TH2D*> htruedcaxy_vs_pttrue(nParticles); std::vector<TH2D*> htruedcaxy_vs_ptrue(nParticles); std::vector<TH2D*> htruedcaxy_vs_etatrue(nParticles); std::vector<TH2D*> htruedcaxy_vs_phitrue(nParticles);
    
    std::vector<TH1D*> htruedcaxy_Mean_vs_pttrue(nParticles); std::vector<TH1D*> htruedcaxy_Mean_vs_ptrue(nParticles); std::vector<TH1D*> htruedcaxy_Mean_vs_etatrue(nParticles); std::vector<TH1D*> htruedcaxy_Mean_vs_phitrue(nParticles);
    
    std::vector<TH1D*> htruedcaxy_Width_vs_pttrue(nParticles); std::vector<TH1D*> htruedcaxy_Width_vs_ptrue(nParticles); std::vector<TH1D*> htruedcaxy_Width_vs_etatrue(nParticles); std::vector<TH1D*> htruedcaxy_Width_vs_phitrue(nParticles);
    
    //truedcaxy as a function of reconstructed quantities
    TH2D* htruedcaxy_vs_ptreco_allparticles; TH2D* htruedcaxy_vs_preco_allparticles; TH2D* htruedcaxy_vs_etareco_allparticles; TH2D* htruedcaxy_vs_phireco_allparticles;
    
    TH1D* htruedcaxy_Mean_vs_ptreco_allparticles; TH1D* htruedcaxy_Mean_vs_preco_allparticles; TH1D* htruedcaxy_Mean_vs_etareco_allparticles; TH1D* htruedcaxy_Mean_vs_phireco_allparticles;
    
    TH1D* htruedcaxy_Width_vs_ptreco_allparticles; TH1D* htruedcaxy_Width_vs_preco_allparticles; TH1D* htruedcaxy_Width_vs_etareco_allparticles; TH1D* htruedcaxy_Width_vs_phireco_allparticles;
    
    std::vector<TH2D*> htruedcaxy_vs_ptreco(nParticles); std::vector<TH2D*> htruedcaxy_vs_preco(nParticles); std::vector<TH2D*> htruedcaxy_vs_etareco(nParticles); std::vector<TH2D*> htruedcaxy_vs_phireco(nParticles);
    
    std::vector<TH1D*> htruedcaxy_Mean_vs_ptreco(nParticles); std::vector<TH1D*> htruedcaxy_Mean_vs_preco(nParticles); std::vector<TH1D*> htruedcaxy_Mean_vs_etareco(nParticles); std::vector<TH1D*> htruedcaxy_Mean_vs_phireco(nParticles);
    
    std::vector<TH1D*> htruedcaxy_Width_vs_ptreco(nParticles); std::vector<TH1D*> htruedcaxy_Width_vs_preco(nParticles); std::vector<TH1D*> htruedcaxy_Width_vs_etareco(nParticles); std::vector<TH1D*> htruedcaxy_Width_vs_phireco(nParticles);
    
    //truedcaz as a function of truth quantities
    TH2D* htruedcaz_vs_pttrue_allparticles; TH2D* htruedcaz_vs_ptrue_allparticles; TH2D* htruedcaz_vs_etatrue_allparticles; TH2D* htruedcaz_vs_phitrue_allparticles;
    
    TH1D* htruedcaz_Mean_vs_pttrue_allparticles; TH1D* htruedcaz_Mean_vs_ptrue_allparticles; TH1D* htruedcaz_Mean_vs_etatrue_allparticles; TH1D* htruedcaz_Mean_vs_phitrue_allparticles;
    
    TH1D* htruedcaz_Width_vs_pttrue_allparticles; TH1D* htruedcaz_Width_vs_ptrue_allparticles; TH1D* htruedcaz_Width_vs_etatrue_allparticles; TH1D* htruedcaz_Width_vs_phitrue_allparticles;
    
    std::vector<TH2D*> htruedcaz_vs_pttrue(nParticles); std::vector<TH2D*> htruedcaz_vs_ptrue(nParticles); std::vector<TH2D*> htruedcaz_vs_etatrue(nParticles); std::vector<TH2D*> htruedcaz_vs_phitrue(nParticles);
    
    std::vector<TH1D*> htruedcaz_Mean_vs_pttrue(nParticles); std::vector<TH1D*> htruedcaz_Mean_vs_ptrue(nParticles); std::vector<TH1D*> htruedcaz_Mean_vs_etatrue(nParticles); std::vector<TH1D*> htruedcaz_Mean_vs_phitrue(nParticles);
    
    std::vector<TH1D*> htruedcaz_Width_vs_pttrue(nParticles); std::vector<TH1D*> htruedcaz_Width_vs_ptrue(nParticles); std::vector<TH1D*> htruedcaz_Width_vs_etatrue(nParticles); std::vector<TH1D*> htruedcaz_Width_vs_phitrue(nParticles);
    
    //truedcaz as a function of reconstructed quantities
    TH2D* htruedcaz_vs_ptreco_allparticles; TH2D* htruedcaz_vs_preco_allparticles; TH2D* htruedcaz_vs_etareco_allparticles; TH2D* htruedcaz_vs_phireco_allparticles;
    
    TH1D* htruedcaz_Mean_vs_ptreco_allparticles; TH1D* htruedcaz_Mean_vs_preco_allparticles; TH1D* htruedcaz_Mean_vs_etareco_allparticles; TH1D* htruedcaz_Mean_vs_phireco_allparticles;
    
    TH1D* htruedcaz_Width_vs_ptreco_allparticles; TH1D* htruedcaz_Width_vs_preco_allparticles; TH1D* htruedcaz_Width_vs_etareco_allparticles; TH1D* htruedcaz_Width_vs_phireco_allparticles;
    
    std::vector<TH2D*> htruedcaz_vs_ptreco(nParticles); std::vector<TH2D*> htruedcaz_vs_preco(nParticles); std::vector<TH2D*> htruedcaz_vs_etareco(nParticles); std::vector<TH2D*> htruedcaz_vs_phireco(nParticles);
    
    std::vector<TH1D*> htruedcaz_Mean_vs_ptreco(nParticles); std::vector<TH1D*> htruedcaz_Mean_vs_preco(nParticles); std::vector<TH1D*> htruedcaz_Mean_vs_etareco(nParticles); std::vector<TH1D*> htruedcaz_Mean_vs_phireco(nParticles);
    
    std::vector<TH1D*> htruedcaz_Width_vs_ptreco(nParticles); std::vector<TH1D*> htruedcaz_Width_vs_preco(nParticles); std::vector<TH1D*> htruedcaz_Width_vs_etareco(nParticles); std::vector<TH1D*> htruedcaz_Width_vs_phireco(nParticles);
    
    //hptRes as a function of truth quantities
    TH2D* hptRes_vs_pttrue_allparticles; TH2D* hptRes_vs_ptrue_allparticles; TH2D* hptRes_vs_etatrue_allparticles; TH2D* hptRes_vs_phitrue_allparticles;
    
    std::vector<TH2D*>
    hptRes_vs_pttrue(nParticles); std::vector<TH2D*>
    hptRes_vs_ptrue(nParticles); std::vector<TH2D*>
    hptRes_vs_etatrue(nParticles); std::vector<TH2D*>
    hptRes_vs_phitrue(nParticles);
    
    std::vector<TH1D*>
    hptRes_Mean_vs_pttrue(nParticles); std::vector<TH1D*>
    hptRes_Mean_vs_ptrue(nParticles); std::vector<TH1D*>
    hptRes_Mean_vs_etatrue(nParticles); std::vector<TH1D*>
    hptRes_Mean_vs_phitrue(nParticles);
    
    std::vector<TH1D*>
    hptRes_Width_vs_pttrue(nParticles); std::vector<TH1D*>
    hptRes_Width_vs_ptrue(nParticles); std::vector<TH1D*>
    hptRes_Width_vs_etatrue(nParticles); std::vector<TH1D*>
    hptRes_Width_vs_phitrue(nParticles);
    
    //hptRes as a function of reconstructed quantities
    TH2D* hptRes_vs_ptreco_allparticles; TH2D* hptRes_vs_preco_allparticles; TH2D* hptRes_vs_etareco_allparticles; TH2D* hptRes_vs_phireco_allparticles;
    
    std::vector<TH2D*>
    hptRes_vs_ptreco(nParticles); std::vector<TH2D*>
    hptRes_vs_preco(nParticles); std::vector<TH2D*>
    hptRes_vs_etareco(nParticles); std::vector<TH2D*>
    hptRes_vs_phireco(nParticles);
    
    std::vector<TH1D*>
    hptRes_Mean_vs_ptreco(nParticles); std::vector<TH1D*>
    hptRes_Mean_vs_preco(nParticles); std::vector<TH1D*>
    hptRes_Mean_vs_etareco(nParticles); std::vector<TH1D*>
    hptRes_Mean_vs_phireco(nParticles);
    
    std::vector<TH1D*>
    hptRes_Width_vs_ptreco(nParticles); std::vector<TH1D*>
    hptRes_Width_vs_preco(nParticles); std::vector<TH1D*>
    hptRes_Width_vs_etareco(nParticles); std::vector<TH1D*>
    hptRes_Width_vs_phireco(nParticles);
    
    //hpRes as a function of truth quantities
    TH2D* hpRes_vs_pttrue_allparticles; TH2D* hpRes_vs_ptrue_allparticles; TH2D* hpRes_vs_etatrue_allparticles; TH2D* hpRes_vs_phitrue_allparticles;
    
    std::vector<TH2D*>
    hpRes_vs_pttrue(nParticles); std::vector<TH2D*>
    hpRes_vs_ptrue(nParticles); std::vector<TH2D*>
    hpRes_vs_etatrue(nParticles); std::vector<TH2D*>
    hpRes_vs_phitrue(nParticles);
    
    std::vector<TH1D*>
    hpRes_Mean_vs_pttrue(nParticles); std::vector<TH1D*>
    hpRes_Mean_vs_ptrue(nParticles); std::vector<TH1D*>
    hpRes_Mean_vs_etatrue(nParticles); std::vector<TH1D*>
    hpRes_Mean_vs_phitrue(nParticles);
    
    std::vector<TH1D*>
    hpRes_Width_vs_pttrue(nParticles); std::vector<TH1D*>
    hpRes_Width_vs_ptrue(nParticles); std::vector<TH1D*>
    hpRes_Width_vs_etatrue(nParticles); std::vector<TH1D*>
    hpRes_Width_vs_phitrue(nParticles);
    
    //hpRes as a function of reconstructed quantities
    TH2D* hpRes_vs_ptreco_allparticles; TH2D* hpRes_vs_preco_allparticles; TH2D* hpRes_vs_etareco_allparticles; TH2D* hpRes_vs_phireco_allparticles;
    
    std::vector<TH2D*>
    hpRes_vs_ptreco(nParticles); std::vector<TH2D*>
    hpRes_vs_preco(nParticles); std::vector<TH2D*>
    hpRes_vs_etareco(nParticles); std::vector<TH2D*>
    hpRes_vs_phireco(nParticles);
    
    std::vector<TH1D*>
    hpRes_Mean_vs_ptreco(nParticles); std::vector<TH1D*>
    hpRes_Mean_vs_preco(nParticles); std::vector<TH1D*>
    hpRes_Mean_vs_etareco(nParticles); std::vector<TH1D*>
    hpRes_Mean_vs_phireco(nParticles);
    
    std::vector<TH1D*>
    hpRes_Width_vs_ptreco(nParticles); std::vector<TH1D*>
    hpRes_Width_vs_preco(nParticles); std::vector<TH1D*>
    hpRes_Width_vs_etareco(nParticles); std::vector<TH1D*>
    hpRes_Width_vs_phireco(nParticles);
    
    //Defining Histograms
    hEta_allparticles=new TH1D("hEta_allparticles","#eta distribution_allparticles;#eta;tracks",150,-1.5,1.5);
    hEta_allparticles->SetStats(0);
    hPhi_allparticles=new TH1D("hPhi_allparticles","#phi distribution_allparticles;#phi;tracks",30,phi_min, phi_max);
    hPhi_allparticles->SetStats(0);
    hPt_allparticles=new TH1D("hPt_allparticles","p_{T} distribution_allparticles;p_{T} (GeV/c);tracks",200,0,10);
    hPt_allparticles->SetStats(0);
    hp_allparticles=new TH1D("hp_allparticles","p distribution_allparticles;p (GeV/c);tracks",200,0,10);
    hp_allparticles->SetStats(0);
    
    hnmapsstate_vs_phi_allparticles=new TH2D("hnmapsstate_vs_phi_allparticles","hnmapsstate_vs_phi_allparticles;#phi;nmapsstate",nbins_phi,phi_min,phi_max,6,0,6);
    hnmapsstate_vs_phi_allparticles->SetStats(0);
     
    
    //dcaxy as a function of truth quantities
    hdcaxy_vs_pttrue_allparticles=new TH2D("hdcaxy_vs_pttrue_allparticles","hdcaxy_vs_pttrue;p_{T} (true) (GeV/c);dcaxy  (cm)",nbins_pt,pt_min,pt_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
    hdcaxy_vs_pttrue_allparticles->SetStats(0);
    
    hdcaxy_vs_ptrue_allparticles=new TH2D("hdcaxy_vs_ptrue_allparticles","hdcaxy_vs_ptrue;p (true) (GeV/c);dcaxy  (cm)",nbins_p,p_min,p_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
    hdcaxy_vs_ptrue_allparticles->SetStats(0);
    
    hdcaxy_vs_etatrue_allparticles=new TH2D("hdcaxy_vs_etatrue_allparticles","hdcaxy_vs_etatrue;#eta (true);dcaxy  (cm)",nbins_eta,eta_min,eta_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
    hdcaxy_vs_etatrue_allparticles->SetStats(0);
    
    hdcaxy_vs_phitrue_allparticles=new TH2D("hdcaxy_vs_phitrue_allparticles","hdcaxy_vs_phitrue;#phi (true);dcaxy  (cm)",nbins_phi,phi_min,phi_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
    hdcaxy_vs_phitrue_allparticles->SetStats(0);
    
    //dcaxy as a function of reconstructed quantities
    hdcaxy_vs_ptreco_allparticles=new TH2D("hdcaxy_vs_ptreco_allparticles","hdcaxy_vs_ptreco;p_{T} (reco);dcaxy  (cm)",nbins_pt,pt_min,pt_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
    hdcaxy_vs_ptreco_allparticles->SetStats(0);
    
    hdcaxy_vs_preco_allparticles=new TH2D("hdcaxy_vs_preco_allparticles","hdcaxy_vs_preco;p (reco);dcaxy  (cm)",nbins_p,p_min,p_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
    hdcaxy_vs_preco_allparticles->SetStats(0);
    
    hdcaxy_vs_etareco_allparticles=new TH2D("hdcaxy_vs_etareco_allparticles","hdcaxy_vs_etareco;#eta (reco);dcaxy  (cm)",nbins_eta,eta_min,eta_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
    hdcaxy_vs_etareco_allparticles->SetStats(0);
    
    hdcaxy_vs_phireco_allparticles=new TH2D("hdcaxy_vs_phireco_allparticles","hdcaxy_vs_phireco;#phi (reco);dcaxy  (cm)",nbins_phi,phi_min,phi_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
    hdcaxy_vs_phireco_allparticles->SetStats(0);
    
    //dcaz as a function of truth quantities
    hdcaz_vs_pttrue_allparticles=new TH2D("hdcaz_vs_pttrue_allparticles","hdcaz_vs_pttrue;p_{T} (true) (GeV/c);dcaz  (cm)",nbins_pt,pt_min,pt_max,nbins_dcaz,dcaz_min,dcaz_max);
    hdcaz_vs_pttrue_allparticles->SetStats(0);
    
    hdcaz_vs_ptrue_allparticles=new TH2D("hdcaz_vs_ptrue_allparticles","hdcaz_vs_ptrue;p (true) (GeV/c);dcaz  (cm)",nbins_p,p_min,p_max,nbins_dcaz,dcaz_min,dcaz_max);
    hdcaz_vs_ptrue_allparticles->SetStats(0);
    
    hdcaz_vs_etatrue_allparticles=new TH2D("hdcaz_vs_etatrue_allparticles","hdcaz_vs_etatrue;#eta (true);dcaz  (cm)",nbins_eta,eta_min,eta_max,nbins_dcaz,dcaz_min,dcaz_max);
    hdcaz_vs_etatrue_allparticles->SetStats(0);
    
    hdcaz_vs_phitrue_allparticles=new TH2D("hdcaz_vs_phitrue_allparticles","hdcaz_vs_phitrue;#phi (true);dcaz  (cm)",nbins_phi,phi_min,phi_max,nbins_dcaz,dcaz_min,dcaz_max);
    hdcaz_vs_phitrue_allparticles->SetStats(0);
    
    //dcaz as a function of reconstructed quantities
    hdcaz_vs_ptreco_allparticles=new TH2D("hdcaz_vs_ptreco_allparticles","hdcaz_vs_ptreco;p_{T} (reco);dcaz  (cm)",nbins_pt,pt_min,pt_max,nbins_dcaz,dcaz_min,dcaz_max);
    hdcaz_vs_ptreco_allparticles->SetStats(0);
    
    hdcaz_vs_preco_allparticles=new TH2D("hdcaz_vs_preco_allparticles","hdcaz_vs_preco;p (reco);dcaz  (cm)",nbins_p,p_min,p_max,nbins_dcaz,dcaz_min,dcaz_max);
    hdcaz_vs_preco_allparticles->SetStats(0);
    
    hdcaz_vs_etareco_allparticles=new TH2D("hdcaz_vs_etareco_allparticles","hdcaz_vs_etareco;#eta (reco);dcaz  (cm)",nbins_eta,eta_min,eta_max,nbins_dcaz,dcaz_min,dcaz_max);
    hdcaz_vs_etareco_allparticles->SetStats(0);
    
    hdcaz_vs_phireco_allparticles=new TH2D("hdcaz_vs_phireco_allparticles","hdcaz_vs_phireco;#phi (reco);dcaz  (cm)",nbins_phi,phi_min,phi_max,nbins_dcaz,dcaz_min,dcaz_max);
    hdcaz_vs_phireco_allparticles->SetStats(0);
    
    //truedcaxy as a function of truth quantities
    htruedcaxy_vs_pttrue_allparticles=new TH2D("htruedcaxy_vs_pttrue_allparticles","htruedcaxy_vs_pttrue;p_{T} (true) (GeV/c);truedcaxy  (cm)",nbins_pt,pt_min,pt_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
    htruedcaxy_vs_pttrue_allparticles->SetStats(0);
    
    htruedcaxy_vs_ptrue_allparticles=new TH2D("htruedcaxy_vs_ptrue_allparticles","htruedcaxy_vs_ptrue;p (true) (GeV/c);truedcaxy  (cm)",nbins_p,p_min,p_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
    htruedcaxy_vs_ptrue_allparticles->SetStats(0);
    
    htruedcaxy_vs_etatrue_allparticles=new TH2D("htruedcaxy_vs_etatrue_allparticles","htruedcaxy_vs_etatrue;#eta (true);truedcaxy  (cm)",nbins_eta,eta_min,eta_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
    htruedcaxy_vs_etatrue_allparticles->SetStats(0);
    
    htruedcaxy_vs_phitrue_allparticles=new TH2D("htruedcaxy_vs_phitrue_allparticles","htruedcaxy_vs_phitrue;#phi (true);truedcaxy  (cm)",nbins_phi,phi_min,phi_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
    htruedcaxy_vs_phitrue_allparticles->SetStats(0);
    
    //truedcaxy as a function of reconstructed quantities
    htruedcaxy_vs_ptreco_allparticles=new TH2D("htruedcaxy_vs_ptreco_allparticles","htruedcaxy_vs_ptreco;p_{T} (reco);truedcaxy  (cm)",nbins_pt,pt_min,pt_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
    htruedcaxy_vs_ptreco_allparticles->SetStats(0);
    
    htruedcaxy_vs_preco_allparticles=new TH2D("htruedcaxy_vs_preco_allparticles","htruedcaxy_vs_preco;p (reco);truedcaxy  (cm)",nbins_p,p_min,p_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
    htruedcaxy_vs_preco_allparticles->SetStats(0);
    
    htruedcaxy_vs_etareco_allparticles=new TH2D("htruedcaxy_vs_etareco_allparticles","htruedcaxy_vs_etareco;#eta (reco);truedcaxy  (cm)",nbins_eta,eta_min,eta_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
    htruedcaxy_vs_etareco_allparticles->SetStats(0);
    
    htruedcaxy_vs_phireco_allparticles=new TH2D("htruedcaxy_vs_phireco_allparticles","htruedcaxy_vs_phireco;#phi (reco);truedcaxy  (cm)",nbins_phi,phi_min,phi_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
    htruedcaxy_vs_phireco_allparticles->SetStats(0);
    
    //truedcaz as a function of truth quantities
    htruedcaz_vs_pttrue_allparticles=new TH2D("htruedcaz_vs_pttrue_allparticles","htruedcaz_vs_pttrue;p_{T} (true) (GeV/c);truedcaz  (cm)",nbins_pt,pt_min,pt_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
    htruedcaz_vs_pttrue_allparticles->SetStats(0);
    
    htruedcaz_vs_ptrue_allparticles=new TH2D("htruedcaz_vs_ptrue_allparticles","htruedcaz_vs_ptrue;p (true) (GeV/c);truedcaz  (cm)",nbins_p,p_min,p_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
    htruedcaz_vs_ptrue_allparticles->SetStats(0);
    
    htruedcaz_vs_etatrue_allparticles=new TH2D("htruedcaz_vs_etatrue_allparticles","htruedcaz_vs_etatrue;#eta (true);truedcaz  (cm)",nbins_eta,eta_min,eta_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
    htruedcaz_vs_etatrue_allparticles->SetStats(0);
    
    htruedcaz_vs_phitrue_allparticles=new TH2D("htruedcaz_vs_phitrue_allparticles","htruedcaz_vs_phitrue;#phi (true);truedcaz  (cm)",nbins_phi,phi_min,phi_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
    htruedcaz_vs_phitrue_allparticles->SetStats(0);
    
    //truedcaz as a function of reconstructed quantities
    htruedcaz_vs_ptreco_allparticles=new TH2D("htruedcaz_vs_ptreco_allparticles","htruedcaz_vs_ptreco;p_{T} (reco);truedcaz  (cm)",nbins_pt,pt_min,pt_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
    htruedcaz_vs_ptreco_allparticles->SetStats(0);
    
    htruedcaz_vs_preco_allparticles=new TH2D("htruedcaz_vs_preco_allparticles","htruedcaz_vs_preco;p (reco);truedcaz  (cm)",nbins_p,p_min,p_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
    htruedcaz_vs_preco_allparticles->SetStats(0);
    
    htruedcaz_vs_etareco_allparticles=new TH2D("htruedcaz_vs_etareco_allparticles","htruedcaz_vs_etareco;#eta (reco);truedcaz  (cm)",nbins_eta,eta_min,eta_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
    htruedcaz_vs_etareco_allparticles->SetStats(0);
    
    htruedcaz_vs_phireco_allparticles=new TH2D("htruedcaz_vs_phireco_allparticles","htruedcaz_vs_phireco;#phi (reco);truedcaz  (cm)",nbins_phi,phi_min,phi_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
    htruedcaz_vs_phireco_allparticles->SetStats(0);
    
    for (int i=0;i<nParticles;i++){
        hEta[i]=new TH1D(Form("hEta_%s",particles[i].c_str()),Form("#eta distribution_%s;#eta;tracks",particles[i].c_str()),nbins_eta,eta_min,eta_max);
        hEta[i]->SetStats(0);
        hPhi[i]=new TH1D(Form("hPhi_%s",particles[i].c_str()),Form("#phi distribution_%s;#phi;tracks",particles[i].c_str()),nbins_phi,phi_min, phi_max);
        hPhi[i]->SetStats(0);
        hPt[i]=new TH1D(Form("hPt_%s",particles[i].c_str()),Form("p_{T} distribution_%s;p_{T};tracks",particles[i].c_str()),nbins_pt,pt_min,pt_max);
        hPt[i]->SetStats(0);
        hp[i]=new TH1D(Form("hp_%s",particles[i].c_str()),Form("p distribution_%s;p;tracks",particles[i].c_str()),nbins_p,p_min,p_max);
        hp[i]->SetStats(0);
        
        hnmapsstate_vs_phi[i]=new TH2D(Form("hnmapsstate_vs_phi_%s",particles[i].c_str()),Form("hnmapsstate_vs_phi;#phi;nmapsstate_%s",particles[i].c_str()),nbins_phi,phi_min,phi_max,6,0,6);
        hnmapsstate_vs_phi[i]->SetStats(0);
         
        //dcaxy as a function of truth quantities
        hdcaxy_vs_pttrue[i]=new TH2D(Form("hdcaxy_vs_pttrue_%s",particles[i].c_str()),Form("hdcaxy_vs_pttrue_%s;p_{T} (true) (GeV/c);dcaxy  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        hdcaxy_vs_pttrue[i]->SetStats(0);
        
        hdcaxy_vs_ptrue[i]=new TH2D(Form("hdcaxy_vs_ptrue_%s",particles[i].c_str()),Form("hdcaxy_vs_ptrue_%s;p (true) (GeV/c);dcaxy  (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        hdcaxy_vs_ptrue[i]->SetStats(0);
        
        hdcaxy_vs_etatrue[i]=new TH2D(Form("hdcaxy_vs_etatrue_%s",particles[i].c_str()),Form("hdcaxy_vs_etatrue_%s;#eta (true);dcaxy  (cm)",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        hdcaxy_vs_etatrue[i]->SetStats(0);
        
        hdcaxy_vs_phitrue[i]=new TH2D(Form("hdcaxy_vs_phitrue_%s",particles[i].c_str()),Form("hdcaxy_vs_phitrue_%s;#phi (true);dcaxy  (cm)",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        hdcaxy_vs_phitrue[i]->SetStats(0);
        
        //dcaxy as a function of reconstructed quantities
        hdcaxy_vs_ptreco[i]=new TH2D(Form("hdcaxy_vs_ptreco_%s",particles[i].c_str()),Form("hdcaxy_vs_ptreco_%s;p_{T} (reco);dcaxy  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        hdcaxy_vs_ptreco[i]->SetStats(0);
        
        hdcaxy_vs_preco[i]=new TH2D(Form("hdcaxy_vs_preco_%s",particles[i].c_str()),Form("hdcaxy_vs_preco_%s;p (reco);dcaxy  (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        hdcaxy_vs_preco[i]->SetStats(0);
        
        hdcaxy_vs_etareco[i]=new TH2D(Form("hdcaxy_vs_etareco_%s",particles[i].c_str()),Form("hdcaxy_vs_etareco_%s;#eta (reco);dcaxy  (cm)",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        hdcaxy_vs_etareco[i]->SetStats(0);
        
        hdcaxy_vs_phireco[i]=new TH2D(Form("hdcaxy_vs_phireco_%s",particles[i].c_str()),Form("hdcaxy_vs_phireco_%s;#phi (reco);dcaxy  (cm)",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        hdcaxy_vs_phireco[i]->SetStats(0);
        
        //dcaz as a function of truth quantities
        hdcaz_vs_pttrue[i]=new TH2D(Form("hdcaz_vs_pttrue_%s",particles[i].c_str()),Form("hdcaz_vs_pttrue_%s;p_{T} (true) (GeV/c);dcaz  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_dcaz,dcaz_min,dcaz_max);
        hdcaz_vs_pttrue[i]->SetStats(0);
        
        hdcaz_vs_ptrue[i]=new TH2D(Form("hdcaz_vs_ptrue_%s",particles[i].c_str()),Form("hdcaz_vs_ptrue_%s;p (true) (GeV/c);dcaz  (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_dcaz,dcaz_min,dcaz_max);
        hdcaz_vs_ptrue[i]->SetStats(0);
        
        hdcaz_vs_etatrue[i]=new TH2D(Form("hdcaz_vs_etatrue_%s",particles[i].c_str()),Form("hdcaz_vs_etatrue_%s;#eta (true);dcaz  (cm)",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_dcaz,dcaz_min,dcaz_max);
        hdcaz_vs_etatrue[i]->SetStats(0);
        
        hdcaz_vs_phitrue[i]=new TH2D(Form("hdcaz_vs_phitrue_%s",particles[i].c_str()),Form("hdcaz_vs_phitrue_%s;#phi (true);dcaz  (cm)",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_dcaz,dcaz_min,dcaz_max);
        hdcaz_vs_phitrue[i]->SetStats(0);
        
        //dcaz as a function of reconstructed quantities
        hdcaz_vs_ptreco[i]=new TH2D(Form("hdcaz_vs_ptreco_%s",particles[i].c_str()),Form("hdcaz_vs_ptreco_%s;p_{T} (reco);dcaz  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_dcaz,dcaz_min,dcaz_max);
        hdcaz_vs_ptreco[i]->SetStats(0);
        
        hdcaz_vs_preco[i]=new TH2D(Form("hdcaz_vs_preco_%s",particles[i].c_str()),Form("hdcaz_vs_preco_%s;p (reco);dcaz  (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_dcaz,dcaz_min,dcaz_max);
        hdcaz_vs_preco[i]->SetStats(0);
        
        hdcaz_vs_etareco[i]=new TH2D(Form("hdcaz_vs_etareco_%s",particles[i].c_str()),Form("hdcaz_vs_etareco_%s;#eta (reco);dcaz  (cm)",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_dcaz,dcaz_min,dcaz_max);
        hdcaz_vs_etareco[i]->SetStats(0);
        
        hdcaz_vs_phireco[i]=new TH2D(Form("hdcaz_vs_phireco_%s",particles[i].c_str()),Form("hdcaz_vs_phireco_%s;#phi (reco);dcaz  (cm)",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_dcaz,dcaz_min,dcaz_max);
        hdcaz_vs_phireco[i]->SetStats(0);
        
        
        //        hdcaxy_vs_pt[i]=new TH2D(Form("hdcaxy_vs_pt_%s",particles[i].c_str()),Form("hdcaxy_vs_pt_%s;pt;dcaxy  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        //        hdcaxy_vs_pt[i]->SetStats(0);
        //        hdcaxy_vs_p[i]=new TH2D(Form("hdcaxy_vs_p_%s",particles[i].c_str()),Form("hdcaxy_vs_p_%s;p;dcaxy (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_dcaxy,dcaxy_min,dcaxy_max);
        //        hdcaxy_vs_p[i]->SetStats(0);
        //
        //        hdcaz_vs_pt[i]=new TH2D(Form("hdcaz_vs_pt_%s",particles[i].c_str()),Form("hdcaz_vs_pt_%s;pt;dcaz  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_dcaz,dcaz_min,dcaz_max);
        //        hdcaz_vs_pt[i]->SetStats(0);
        //        hdcaz_vs_p[i]=new TH2D(Form("hdcaz_vs_p_%s",particles[i].c_str()),Form("hdcaz_vs_p_%s;p;dcaz (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_dcaz,dcaz_min,dcaz_max);
        //        hdcaz_vs_p[i]->SetStats(0);
        
        //truedcaxy as a function of truth quantities
        htruedcaxy_vs_pttrue[i]=new TH2D(Form("htruedcaxy_vs_pttrue_%s",particles[i].c_str()),Form("htruedcaxy_vs_pttrue_%s;p_{T} (true) (GeV/c);truedcaxy  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
        htruedcaxy_vs_pttrue[i]->SetStats(0);
        
        htruedcaxy_vs_ptrue[i]=new TH2D(Form("htruedcaxy_vs_ptrue_%s",particles[i].c_str()),Form("htruedcaxy_vs_ptrue_%s;p (true) (GeV/c);truedcaxy  (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
        htruedcaxy_vs_ptrue[i]->SetStats(0);
        
        htruedcaxy_vs_etatrue[i]=new TH2D(Form("htruedcaxy_vs_etatrue_%s",particles[i].c_str()),Form("htruedcaxy_vs_etatrue_%s;#eta (true);truedcaxy  (cm)",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
        htruedcaxy_vs_etatrue[i]->SetStats(0);
        
        htruedcaxy_vs_phitrue[i]=new TH2D(Form("htruedcaxy_vs_phitrue_%s",particles[i].c_str()),Form("htruedcaxy_vs_phitrue_%s;#phi (true);truedcaxy  (cm)",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
        htruedcaxy_vs_phitrue[i]->SetStats(0);
        
        //truedcaxy as a function of reconstructed quantities
        htruedcaxy_vs_ptreco[i]=new TH2D(Form("htruedcaxy_vs_ptreco_%s",particles[i].c_str()),Form("htruedcaxy_vs_ptreco_%s;p_{T} (reco);truedcaxy  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
        htruedcaxy_vs_ptreco[i]->SetStats(0);
        
        htruedcaxy_vs_preco[i]=new TH2D(Form("htruedcaxy_vs_preco_%s",particles[i].c_str()),Form("htruedcaxy_vs_preco_%s;p (reco);truedcaxy  (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
        htruedcaxy_vs_preco[i]->SetStats(0);
        
        htruedcaxy_vs_etareco[i]=new TH2D(Form("htruedcaxy_vs_etareco_%s",particles[i].c_str()),Form("htruedcaxy_vs_etareco_%s;#eta (reco);truedcaxy  (cm)",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
        htruedcaxy_vs_etareco[i]->SetStats(0);
        
        htruedcaxy_vs_phireco[i]=new TH2D(Form("htruedcaxy_vs_phireco_%s",particles[i].c_str()),Form("htruedcaxy_vs_phireco_%s;#phi (reco);truedcaxy  (cm)",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_truedcaxy,truedcaxy_min,truedcaxy_max);
        htruedcaxy_vs_phireco[i]->SetStats(0);
        
        //truedcaz as a function of truth quantities
        htruedcaz_vs_pttrue[i]=new TH2D(Form("htruedcaz_vs_pttrue_%s",particles[i].c_str()),Form("htruedcaz_vs_pttrue_%s;p_{T} (true) (GeV/c);truedcaz  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
        htruedcaz_vs_pttrue[i]->SetStats(0);
        
        htruedcaz_vs_ptrue[i]=new TH2D(Form("htruedcaz_vs_ptrue_%s",particles[i].c_str()),Form("htruedcaz_vs_ptrue_%s;p (true) (GeV/c);truedcaz  (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
        htruedcaz_vs_ptrue[i]->SetStats(0);
        
        htruedcaz_vs_etatrue[i]=new TH2D(Form("htruedcaz_vs_etatrue_%s",particles[i].c_str()),Form("htruedcaz_vs_etatrue_%s;#eta (true);truedcaz  (cm)",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
        htruedcaz_vs_etatrue[i]->SetStats(0);
        
        htruedcaz_vs_phitrue[i]=new TH2D(Form("htruedcaz_vs_phitrue_%s",particles[i].c_str()),Form("htruedcaz_vs_phitrue_%s;#phi (true);truedcaz  (cm)",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
        htruedcaz_vs_phitrue[i]->SetStats(0);
        
        //truedcaz as a function of reconstructed quantities
        htruedcaz_vs_ptreco[i]=new TH2D(Form("htruedcaz_vs_ptreco_%s",particles[i].c_str()),Form("htruedcaz_vs_ptreco_%s;p_{T} (reco);truedcaz  (cm)",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
        htruedcaz_vs_ptreco[i]->SetStats(0);
        
        htruedcaz_vs_preco[i]=new TH2D(Form("htruedcaz_vs_preco_%s",particles[i].c_str()),Form("htruedcaz_vs_preco_%s;p (reco);truedcaz  (cm)",particles[i].c_str()),nbins_p,p_min,p_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
        htruedcaz_vs_preco[i]->SetStats(0);
        
        htruedcaz_vs_etareco[i]=new TH2D(Form("htruedcaz_vs_etareco_%s",particles[i].c_str()),Form("htruedcaz_vs_etareco_%s;#eta (reco);truedcaz  (cm)",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
        htruedcaz_vs_etareco[i]->SetStats(0);
        
        htruedcaz_vs_phireco[i]=new TH2D(Form("htruedcaz_vs_phireco_%s",particles[i].c_str()),Form("htruedcaz_vs_phireco_%s;#phi (reco);truedcaz  (cm)",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_truedcaz,truedcaz_min,truedcaz_max);
        htruedcaz_vs_phireco[i]->SetStats(0);
        
        //ptRes as a function of truth quantities
        hptRes_vs_pttrue[i]=new TH2D(Form("hptRes_vs_pttrue_%s",particles[i].c_str()),Form("hptRes_vs_pttrue_%s;p_{T} (true) (GeV/c);#frac{p_{T}(reco)-p_{T}(true)}{p_{T}(true)}",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_ptRes,ptRes_min,ptRes_max);
        hptRes_vs_pttrue[i]->SetStats(0);
        
        hptRes_vs_ptrue[i]=new TH2D(Form("hptRes_vs_ptrue_%s",particles[i].c_str()),Form("hptRes_vs_ptrue_%s;p (true) (GeV/c);#frac{p_{T}(reco)-p_{T}(true)}{p_{T}(true)}",particles[i].c_str()),nbins_p,p_min,p_max,nbins_ptRes,ptRes_min,ptRes_max);
        hptRes_vs_ptrue[i]->SetStats(0);
        
        hptRes_vs_etatrue[i]=new TH2D(Form("hptRes_vs_etatrue_%s",particles[i].c_str()),Form("hptRes_vs_etatrue_%s;#eta (true);#frac{p_{T}(reco)-p_{T}(true)}{p_{T}(true)}",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_ptRes,ptRes_min,ptRes_max);
        hptRes_vs_etatrue[i]->SetStats(0);
        
        hptRes_vs_phitrue[i]=new TH2D(Form("hptRes_vs_phitrue_%s",particles[i].c_str()),Form("hptRes_vs_phitrue_%s;#phi (true);#frac{p_{T}(reco)-p_{T}(true)}{p_{T}(true)}",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_ptRes,ptRes_min,ptRes_max);
        hptRes_vs_phitrue[i]->SetStats(0);
        
        
        //ptRes as a function of reconstructed quantities
        hptRes_vs_ptreco[i]=new TH2D(Form("hptRes_vs_ptreco_%s",particles[i].c_str()),Form("hptRes_vs_ptreco_%s;p_{T} (reco);#frac{p_{T}(reco)-p_{T}(true)}{p_{T}(true)}",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_ptRes,ptRes_min,ptRes_max);
        hptRes_vs_ptreco[i]->SetStats(0);
        
        hptRes_vs_preco[i]=new TH2D(Form("hptRes_vs_preco_%s",particles[i].c_str()),Form("hptRes_vs_preco_%s;p (reco);#frac{p_{T}(reco)-p_{T}(true)}{p_{T}(true)}",particles[i].c_str()),nbins_p,p_min,p_max,nbins_ptRes,ptRes_min,ptRes_max);
        hptRes_vs_preco[i]->SetStats(0);
        
        hptRes_vs_etareco[i]=new TH2D(Form("hptRes_vs_etareco_%s",particles[i].c_str()),Form("hptRes_vs_etareco_%s;#eta (reco);#frac{p_{T}(reco)-p_{T}(true)}{p_{T}(true)}",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_ptRes,ptRes_min,ptRes_max);
        hptRes_vs_etareco[i]->SetStats(0);
        
        hptRes_vs_phireco[i]=new TH2D(Form("hptRes_vs_phireco_%s",particles[i].c_str()),Form("hptRes_vs_phireco_%s;#phi (reco);#frac{p_{T}(reco)-p_{T}(true)}{p_{T}(true)}",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_ptRes,ptRes_min,ptRes_max);
        hptRes_vs_phireco[i]->SetStats(0);
        
        //pRes as a function of truth quantities
        hpRes_vs_pttrue[i]=new TH2D(Form("hpRes_vs_pttrue_%s",particles[i].c_str()),Form("hpRes_vs_pttrue_%s;p_{T} (true) (GeV/c);#frac{p(reco)-p(true)}{p(true)}",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_pRes,pRes_min,pRes_max);
        hpRes_vs_pttrue[i]->SetStats(0);
        
        hpRes_vs_ptrue[i]=new TH2D(Form("hpRes_vs_ptrue_%s",particles[i].c_str()),Form("hpRes_vs_ptrue_%s;p (true) (GeV/c);#frac{p(reco)-p(true)}{p(true)}",particles[i].c_str()),nbins_p,p_min,p_max,nbins_pRes,pRes_min,pRes_max);
        hpRes_vs_ptrue[i]->SetStats(0);
        
        hpRes_vs_etatrue[i]=new TH2D(Form("hpRes_vs_etatrue_%s",particles[i].c_str()),Form("hpRes_vs_etatrue_%s;#eta (true);#frac{p(reco)-p(true)}{p(true)}",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_pRes,pRes_min,pRes_max);
        hpRes_vs_etatrue[i]->SetStats(0);
        
        hpRes_vs_phitrue[i]=new TH2D(Form("hpRes_vs_phitrue_%s",particles[i].c_str()),Form("hpRes_vs_phitrue_%s;#phi (true);#frac{p(reco)-p(true)}{p(true)}",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_pRes,pRes_min,pRes_max);
        hpRes_vs_phitrue[i]->SetStats(0);
        
        
        //pRes as a function of reconstructed quantities
        hpRes_vs_ptreco[i]=new TH2D(Form("hpRes_vs_ptreco_%s",particles[i].c_str()),Form("hpRes_vs_ptreco_%s;p_{T} (reco);#frac{p(reco)-p(true)}{p(true)}",particles[i].c_str()),nbins_pt,pt_min,pt_max,nbins_pRes,pRes_min,pRes_max);
        hpRes_vs_ptreco[i]->SetStats(0);
        
        hpRes_vs_preco[i]=new TH2D(Form("hpRes_vs_preco_%s",particles[i].c_str()),Form("hpRes_vs_preco_%s;p (reco);#frac{p(reco)-p(true)}{p(true)}",particles[i].c_str()),nbins_p,p_min,p_max,nbins_pRes,pRes_min,pRes_max);
        hpRes_vs_preco[i]->SetStats(0);
        
        hpRes_vs_etareco[i]=new TH2D(Form("hpRes_vs_etareco_%s",particles[i].c_str()),Form("hpRes_vs_etareco_%s;#eta (reco);#frac{p(reco)-p(true)}{p(true)}",particles[i].c_str()),nbins_eta,eta_min,eta_max,nbins_pRes,pRes_min,pRes_max);
        hpRes_vs_etareco[i]->SetStats(0);
        
        hpRes_vs_phireco[i]=new TH2D(Form("hpRes_vs_phireco_%s",particles[i].c_str()),Form("hpRes_vs_phireco_%s;#phi (reco);#frac{p(reco)-p(true)}{p(true)}",particles[i].c_str()),nbins_phi,phi_min,phi_max,nbins_pRes,pRes_min,pRes_max);
        hpRes_vs_phireco[i]->SetStats(0);
    }
    
    TVector3 momentum3vector;
    TVector3 true_momentum3vector;
    
    Long64_t n_entries=residualtree->GetEntries();
    for(Long64_t i=0;i<n_entries;i++){
        residualtree->GetEntry(i);
        int particle_index=-1;
        for(int particle_no=0;particle_no<nParticles; particle_no++){
            if(particles_PID[particle_no]==gflavor){
                particle_index=particle_no;
                break;
            }
        }
        
        
        if(nmaps>1 && nintt>0 && ntpc>25 && pt>=0.2 && vertex_ntracks > 3){
            
        //if(nmapsstate>=2||nintt>=1||pt>=0.2){
              
            momentum3vector.SetXYZ(px, py, pz);
            float p   = momentum3vector.Mag();
            
            float true_pt=0,true_p=0,true_eta=0,true_phi=0, ptres=0, pres=0;
            if (gflavor != 0){
                true_momentum3vector.SetXYZ(true_px, true_py, true_pz);
                true_pt  = (float) true_momentum3vector.Pt();
                true_p   = (float) true_momentum3vector.Mag();
                true_eta = (float) true_momentum3vector.Eta();
                true_phi = (float) true_momentum3vector.Phi();
                
                ptres=(pt-true_pt)/true_pt;
                pres=(p-true_p)/true_p;
            }
            else{
                true_momentum3vector.SetXYZ(0, 0, 0);
                true_pt  = 0;
                true_p   = 0;
                true_eta = 0;
                true_phi = 0;
            }
            
             
            
            
            //All particles histograms
            hEta_allparticles->Fill(eta);
            hPhi_allparticles->Fill(phi);
            hPt_allparticles->Fill(pt);
            hp_allparticles->Fill(p);
            hnmapsstate_vs_phi_allparticles->Fill(phi,nmapsstate);
            
            hdcaxy_vs_ptreco_allparticles->Fill(pt,dcaxy);
            hdcaxy_vs_preco_allparticles->Fill(p,dcaxy);
            hdcaxy_vs_etareco_allparticles->Fill(eta,dcaxy);
            hdcaxy_vs_phireco_allparticles->Fill(phi,dcaxy);
            
            hdcaz_vs_ptreco_allparticles->Fill(pt,dcaz);
            hdcaz_vs_preco_allparticles->Fill(p,dcaz);
            hdcaz_vs_etareco_allparticles->Fill(eta,dcaz);
            hdcaz_vs_phireco_allparticles->Fill(phi,dcaz);
            
            htruedcaxy_vs_ptreco_allparticles->Fill(pt,true_dcaxy);
            htruedcaxy_vs_preco_allparticles->Fill(p,true_dcaxy);
            htruedcaxy_vs_etareco_allparticles->Fill(eta,true_dcaxy);
            htruedcaxy_vs_phireco_allparticles->Fill(phi,true_dcaxy);
            
            htruedcaz_vs_ptreco_allparticles->Fill(pt,true_dcaz);
            htruedcaz_vs_preco_allparticles->Fill(p,true_dcaz);
            htruedcaz_vs_etareco_allparticles->Fill(eta,true_dcaz);
            htruedcaz_vs_phireco_allparticles->Fill(phi,true_dcaz);
            
            
            if (gflavor != 0){
                
                hdcaxy_vs_pttrue_allparticles->Fill(true_pt,dcaxy);
                hdcaxy_vs_ptrue_allparticles->Fill(true_p,dcaxy);
                hdcaxy_vs_etatrue_allparticles->Fill(true_eta,dcaxy);
                hdcaxy_vs_phitrue_allparticles->Fill(true_phi,dcaxy);
                
                hdcaz_vs_pttrue_allparticles->Fill(true_pt,dcaz);
                hdcaz_vs_ptrue_allparticles->Fill(true_p,dcaz);
                hdcaz_vs_etatrue_allparticles->Fill(true_eta,dcaz);
                hdcaz_vs_phitrue_allparticles->Fill(true_phi,dcaz);
                
                htruedcaxy_vs_pttrue_allparticles->Fill(true_pt,true_dcaxy);
                htruedcaxy_vs_ptrue_allparticles->Fill(true_p,true_dcaxy);
                htruedcaxy_vs_etatrue_allparticles->Fill(true_eta,true_dcaxy);
                htruedcaxy_vs_phitrue_allparticles->Fill(true_phi,true_dcaxy);
                
                htruedcaz_vs_pttrue_allparticles->Fill(true_pt,true_dcaz);
                htruedcaz_vs_ptrue_allparticles->Fill(true_p,true_dcaz);
                htruedcaz_vs_etatrue_allparticles->Fill(true_eta,true_dcaz);
                htruedcaz_vs_phitrue_allparticles->Fill(true_phi,true_dcaz);
            }
            
            //PID Specific histograms
            if (particle_index < 0){ continue;}
            
//            TLorentzVector momentum4vector;
//            TLorentzVector true_momentum4vector;
//            
//            momentum4vector.SetXYZM(px, py, pz, particles_mass[particle_index]);
//            true_momentum4vector.SetXYZM(true_px, true_py, true_pz, particles_mass[particle_index]);
           
            hEta[particle_index]->Fill(eta);
            hPhi[particle_index]->Fill(phi);
            hPt[particle_index]->Fill(pt);
            hp[particle_index]->Fill(p);
            hnmapsstate_vs_phi[particle_index]->Fill(phi,nmapsstate);
            
            hdcaxy_vs_ptreco[particle_index]->Fill(pt,dcaxy);
            hdcaxy_vs_preco[particle_index]->Fill(p,dcaxy);
            hdcaxy_vs_etareco[particle_index]->Fill(eta,dcaxy);
            hdcaxy_vs_phireco[particle_index]->Fill(phi,dcaxy);
            
            hdcaz_vs_ptreco[particle_index]->Fill(pt,dcaz);
            hdcaz_vs_preco[particle_index]->Fill(p,dcaz);
            hdcaz_vs_etareco[particle_index]->Fill(eta,dcaz);
            hdcaz_vs_phireco[particle_index]->Fill(phi,dcaz);
            
            htruedcaxy_vs_ptreco[particle_index]->Fill(pt,true_dcaxy);
            htruedcaxy_vs_preco[particle_index]->Fill(p,true_dcaxy);
            htruedcaxy_vs_etareco[particle_index]->Fill(eta,true_dcaxy);
            htruedcaxy_vs_phireco[particle_index]->Fill(phi,true_dcaxy);
            
            htruedcaz_vs_ptreco[particle_index]->Fill(pt,true_dcaz);
            htruedcaz_vs_preco[particle_index]->Fill(p,true_dcaz);
            htruedcaz_vs_etareco[particle_index]->Fill(eta,true_dcaz);
            htruedcaz_vs_phireco[particle_index]->Fill(phi,true_dcaz);
            
            if (gflavor != 0){
                
                hdcaxy_vs_pttrue[particle_index]->Fill(true_pt,dcaxy);
                hdcaxy_vs_ptrue[particle_index]->Fill(true_p,dcaxy);
                hdcaxy_vs_etatrue[particle_index]->Fill(true_eta,dcaxy);
                hdcaxy_vs_phitrue[particle_index]->Fill(true_phi,dcaxy);
                
                hdcaz_vs_pttrue[particle_index]->Fill(true_pt,dcaz);
                hdcaz_vs_ptrue[particle_index]->Fill(true_p,dcaz);
                hdcaz_vs_etatrue[particle_index]->Fill(true_eta,dcaz);
                hdcaz_vs_phitrue[particle_index]->Fill(true_phi,dcaz);
                
                htruedcaxy_vs_pttrue[particle_index]->Fill(true_pt,true_dcaxy);
                htruedcaxy_vs_ptrue[particle_index]->Fill(true_p,true_dcaxy);
                htruedcaxy_vs_etatrue[particle_index]->Fill(true_eta,true_dcaxy);
                htruedcaxy_vs_phitrue[particle_index]->Fill(true_phi,true_dcaxy);
                
                htruedcaz_vs_pttrue[particle_index]->Fill(true_pt,true_dcaz);
                htruedcaz_vs_ptrue[particle_index]->Fill(true_p,true_dcaz);
                htruedcaz_vs_etatrue[particle_index]->Fill(true_eta,true_dcaz);
                htruedcaz_vs_phitrue[particle_index]->Fill(true_phi,true_dcaz);
            }
            
            
            
            //            if(pt>pt_min && pt<pt_max && dcaxy>dcaxy_min && dcaxy<dcaxy_max){
            //                hdcaxy_vs_pttrue[particle_index]->Fill(true_pt,dcaxy);
            //                hdcaxy_vs_ptrue[particle_index]->Fill(true_p,dcaxy);
            //                hdcaxy_vs_etatrue[particle_index]->Fill(true_eta,dcaxy);
            //                hdcaxy_vs_phitrue[particle_index]->Fill(true_phi,dcaxy);
            //
            //                hdcaxy_vs_ptreco[particle_index]->Fill(pt,dcaxy);
            //                hdcaxy_vs_preco[particle_index]->Fill(p,dcaxy);
            //                hdcaxy_vs_etareco[particle_index]->Fill(eta,dcaxy);
            //                hdcaxy_vs_phireco[particle_index]->Fill(phi,dcaxy);
            //
            //            }
            //            if(p>p_min && p<p_max && dcaxy>dcaxy_min && dcaxy<dcaxy_max){
            //                hdcaxy_vs_p[particle_index]->Fill(p,dcaxy);
            //            }
            //
            //            if(pt>pt_min && pt<pt_max && dcaz>dcaz_min && dcaz<dcaz_max){
            //                hdcaz_vs_pt[particle_index]->Fill(pt,dcaz);
            //            }
            //            if(p>p_min && p<p_max && dcaz>dcaz_min && dcaz<dcaz_max){
            //                hdcaz_vs_p[particle_index]->Fill(p,dcaz);
            //            }
            
             
            hptRes_vs_ptreco[particle_index]->Fill(pt,ptres);
            hptRes_vs_preco[particle_index]->Fill(p,ptres);
            hptRes_vs_etareco[particle_index]->Fill(eta,ptres);
            hptRes_vs_phireco[particle_index]->Fill(phi,ptres);
            
            
            hpRes_vs_ptreco[particle_index]->Fill(pt,pres);
            hpRes_vs_preco[particle_index]->Fill(p,pres);
            hpRes_vs_etareco[particle_index]->Fill(eta,pres);
            hpRes_vs_phireco[particle_index]->Fill(phi,pres);
            
            if (gflavor != 0){
                hptRes_vs_pttrue[particle_index]->Fill(true_pt,ptres);
                hptRes_vs_ptrue[particle_index]->Fill(true_p,ptres);
                hptRes_vs_etatrue[particle_index]->Fill(true_eta,ptres);
                hptRes_vs_phitrue[particle_index]->Fill(true_phi,ptres);
                
                hpRes_vs_pttrue[particle_index]->Fill(true_pt,pres);
                hpRes_vs_ptrue[particle_index]->Fill(true_p,pres);
                hpRes_vs_etatrue[particle_index]->Fill(true_eta,pres);
                hpRes_vs_phitrue[particle_index]->Fill(true_phi,pres);
            }
            
        }
    }
    
    GetFitSlicesY(hnmapsstate_vs_phi_allparticles, fgaus_nmaps, hnmapsstate_Mean_vs_phi_allparticles, hnmapsstate_Width_vs_phi_allparticles, "hnmapsstate_vs_phi_allparticles", "#phi", "nmapsstate");
    
    GetFitSlicesY(hdcaxy_vs_pttrue_allparticles, fgaus_dcaxy, hdcaxy_Mean_vs_pttrue_allparticles, hdcaxy_Width_vs_pttrue_allparticles, "dcaxy_vs_pttrue_allparticles", "p_{T} (true) (GeV/c)", "dcaxy (cm)");
    GetFitSlicesY(hdcaxy_vs_ptrue_allparticles, fgaus_dcaxy, hdcaxy_Mean_vs_ptrue_allparticles, hdcaxy_Width_vs_ptrue_allparticles,"dcaxy_vs_ptrue_allparticles", "p (true) (GeV/c)", "dcaxy (cm)");
    GetFitSlicesY(hdcaxy_vs_etatrue_allparticles, fgaus_dcaxy, hdcaxy_Mean_vs_etatrue_allparticles, hdcaxy_Width_vs_etatrue_allparticles,"dcaxy_vs_etatrue_allparticles", "#eta (true)", "dcaxy (cm)");
    GetFitSlicesY(hdcaxy_vs_phitrue_allparticles, fgaus_dcaxy, hdcaxy_Mean_vs_phitrue_allparticles, hdcaxy_Width_vs_phitrue_allparticles,"dcaxy_vs_phitrue_allparticles", "#phi (true)", "dcaxy (cm)");
    
    GetFitSlicesY(hdcaxy_vs_ptreco_allparticles, fgaus_dcaxy, hdcaxy_Mean_vs_ptreco_allparticles, hdcaxy_Width_vs_ptreco_allparticles, "dcaxy_vs_ptreco_allparticles", "p_{T} (reco) (GeV/c)", "dcaxy (cm)");
    GetFitSlicesY(hdcaxy_vs_preco_allparticles, fgaus_dcaxy, hdcaxy_Mean_vs_preco_allparticles, hdcaxy_Width_vs_preco_allparticles,"dcaxy_vs_preco_allparticles", "p (reco) (GeV/c)", "dcaxy (cm)");
    GetFitSlicesY(hdcaxy_vs_etareco_allparticles, fgaus_dcaxy, hdcaxy_Mean_vs_etareco_allparticles, hdcaxy_Width_vs_etareco_allparticles,"dcaxy_vs_etareco_allparticles", "#eta (reco)", "dcaxy (cm)");
    GetFitSlicesY(hdcaxy_vs_phireco_allparticles, fgaus_dcaxy, hdcaxy_Mean_vs_phireco_allparticles, hdcaxy_Width_vs_phireco_allparticles,"dcaxy_vs_phireco_allparticles", "#phi (reco)", "dcaxy (cm)");
    
    GetFitSlicesY(hdcaz_vs_pttrue_allparticles, fgaus_dcaz, hdcaz_Mean_vs_pttrue_allparticles, hdcaz_Width_vs_pttrue_allparticles, "dcaz_vs_pttrue_allparticles", "p_{T} (true) (GeV/c)", "dcaz (cm)");
    GetFitSlicesY(hdcaz_vs_ptrue_allparticles, fgaus_dcaz, hdcaz_Mean_vs_ptrue_allparticles, hdcaz_Width_vs_ptrue_allparticles,"dcaz_vs_ptrue_allparticles", "p (true) (GeV/c)", "dcaz (cm)");
    GetFitSlicesY(hdcaz_vs_etatrue_allparticles, fgaus_dcaz, hdcaz_Mean_vs_etatrue_allparticles, hdcaz_Width_vs_etatrue_allparticles,"dcaz_vs_etatrue_allparticles", "#eta (true)", "dcaz (cm)");
    GetFitSlicesY(hdcaz_vs_phitrue_allparticles, fgaus_dcaz, hdcaz_Mean_vs_phitrue_allparticles, hdcaz_Width_vs_phitrue_allparticles,"dcaz_vs_phitrue_allparticles", "#phi (true)", "dcaz (cm)");
    
    GetFitSlicesY(hdcaz_vs_ptreco_allparticles, fgaus_dcaz, hdcaz_Mean_vs_ptreco_allparticles, hdcaz_Width_vs_ptreco_allparticles, "dcaz_vs_ptreco_allparticles", "p_{T} (reco) (GeV/c)", "dcaz (cm)");
    GetFitSlicesY(hdcaz_vs_preco_allparticles, fgaus_dcaz, hdcaz_Mean_vs_preco_allparticles, hdcaz_Width_vs_preco_allparticles,"dcaz_vs_preco_allparticles", "p (reco) (GeV/c)", "dcaz (cm)");
    GetFitSlicesY(hdcaz_vs_etareco_allparticles, fgaus_dcaz, hdcaz_Mean_vs_etareco_allparticles, hdcaz_Width_vs_etareco_allparticles,"dcaz_vs_etareco_allparticles", "#eta (reco)", "dcaz (cm)");
    GetFitSlicesY(hdcaz_vs_phireco_allparticles, fgaus_dcaz, hdcaz_Mean_vs_phireco_allparticles, hdcaz_Width_vs_phireco_allparticles,"dcaz_vs_phireco_allparticles", "#phi (reco)", "dcaz (cm)");
    
    
    GetFitSlicesY(htruedcaxy_vs_pttrue_allparticles, fgaus_truedcaxy, htruedcaxy_Mean_vs_pttrue_allparticles, htruedcaxy_Width_vs_pttrue_allparticles, "truedcaxy_vs_pttrue_allparticles", "p_{T} (true) (GeV/c)", "truedcaxy (cm)");
    GetFitSlicesY(htruedcaxy_vs_ptrue_allparticles, fgaus_truedcaxy, htruedcaxy_Mean_vs_ptrue_allparticles, htruedcaxy_Width_vs_ptrue_allparticles,"truedcaxy_vs_ptrue_allparticles", "p (true) (GeV/c)", "truedcaxy (cm)");
    GetFitSlicesY(htruedcaxy_vs_etatrue_allparticles, fgaus_truedcaxy, htruedcaxy_Mean_vs_etatrue_allparticles, htruedcaxy_Width_vs_etatrue_allparticles,"truedcaxy_vs_etatrue_allparticles", "#eta (true)", "truedcaxy (cm)");
    GetFitSlicesY(htruedcaxy_vs_phitrue_allparticles, fgaus_truedcaxy, htruedcaxy_Mean_vs_phitrue_allparticles, htruedcaxy_Width_vs_phitrue_allparticles,"truedcaxy_vs_phitrue_allparticles", "#phi (true)", "truedcaxy (cm)");
    
    GetFitSlicesY(htruedcaxy_vs_ptreco_allparticles, fgaus_truedcaxy, htruedcaxy_Mean_vs_ptreco_allparticles, htruedcaxy_Width_vs_ptreco_allparticles, "truedcaxy_vs_ptreco_allparticles", "p_{T} (reco) (GeV/c)", "truedcaxy (cm)");
    GetFitSlicesY(htruedcaxy_vs_preco_allparticles, fgaus_truedcaxy, htruedcaxy_Mean_vs_preco_allparticles, htruedcaxy_Width_vs_preco_allparticles,"truedcaxy_vs_preco_allparticles", "p (reco) (GeV/c)", "truedcaxy (cm)");
    GetFitSlicesY(htruedcaxy_vs_etareco_allparticles, fgaus_truedcaxy, htruedcaxy_Mean_vs_etareco_allparticles, htruedcaxy_Width_vs_etareco_allparticles,"truedcaxy_vs_etareco_allparticles", "#eta (reco)", "truedcaxy (cm)");
    GetFitSlicesY(htruedcaxy_vs_phireco_allparticles, fgaus_truedcaxy, htruedcaxy_Mean_vs_phireco_allparticles, htruedcaxy_Width_vs_phireco_allparticles,"truedcaxy_vs_phireco_allparticles", "#phi (reco)", "truedcaxy (cm)");
    
    GetFitSlicesY(htruedcaz_vs_pttrue_allparticles, fgaus_truedcaz, htruedcaz_Mean_vs_pttrue_allparticles, htruedcaz_Width_vs_pttrue_allparticles, "truedcaz_vs_pttrue_allparticles", "p_{T} (true) (GeV/c)", "truedcaz (cm)");
    GetFitSlicesY(htruedcaz_vs_ptrue_allparticles, fgaus_truedcaz, htruedcaz_Mean_vs_ptrue_allparticles, htruedcaz_Width_vs_ptrue_allparticles,"truedcaz_vs_ptrue_allparticles", "p (true) (GeV/c)", "truedcaz (cm)");
    GetFitSlicesY(htruedcaz_vs_etatrue_allparticles, fgaus_truedcaz, htruedcaz_Mean_vs_etatrue_allparticles, htruedcaz_Width_vs_etatrue_allparticles,"truedcaz_vs_etatrue_allparticles", "#eta (true)", "truedcaz (cm)");
    GetFitSlicesY(htruedcaz_vs_phitrue_allparticles, fgaus_truedcaz, htruedcaz_Mean_vs_phitrue_allparticles, htruedcaz_Width_vs_phitrue_allparticles,"truedcaz_vs_phitrue_allparticles", "#phi (true)", "truedcaz (cm)");
    
    GetFitSlicesY(htruedcaz_vs_ptreco_allparticles, fgaus_truedcaz, htruedcaz_Mean_vs_ptreco_allparticles, htruedcaz_Width_vs_ptreco_allparticles,"truedcaz_vs_ptreco_allparticles", "p_{T} (reco) (GeV/c)", "truedcaz (cm)");
    GetFitSlicesY(htruedcaz_vs_preco_allparticles, fgaus_truedcaz, htruedcaz_Mean_vs_preco_allparticles, htruedcaz_Width_vs_preco_allparticles,"truedcaz_vs_preco_allparticles", "p (reco) (GeV/c)", "truedcaz (cm)");
    GetFitSlicesY(htruedcaz_vs_etareco_allparticles, fgaus_truedcaz, htruedcaz_Mean_vs_etareco_allparticles, htruedcaz_Width_vs_etareco_allparticles,"truedcaz_vs_etareco_allparticles", "#eta (reco)", "truedcaz (cm)");
    GetFitSlicesY(htruedcaz_vs_phireco_allparticles, fgaus_truedcaz, htruedcaz_Mean_vs_phireco_allparticles, htruedcaz_Width_vs_phireco_allparticles,"truedcaz_vs_phireco_allparticles", "#phi (reco)", "truedcaz (cm)");
    
    
    for(int particle_no=0;particle_no<nParticles;particle_no++){
        
        GetFitSlicesY(hnmapsstate_vs_phi[particle_no], fgaus_nmaps, hnmapsstate_Mean_vs_phi[particle_no], hnmapsstate_Width_vs_phi[particle_no], Form("hnmapsstate_vs_phi_%s",particles[particle_no].c_str()), "#phi", "nmapsstate");
        
        GetFitSlicesY(hdcaxy_vs_pttrue[particle_no], fgaus_dcaxy, hdcaxy_Mean_vs_pttrue[particle_no], hdcaxy_Width_vs_pttrue[particle_no], Form("dcaxy_vs_pttrue_%s",particles[particle_no].c_str()), "p_{T} (true) (GeV/c)", "dcaxy (cm)");
        GetFitSlicesY(hdcaxy_vs_ptrue[particle_no], fgaus_dcaxy, hdcaxy_Mean_vs_ptrue[particle_no], hdcaxy_Width_vs_ptrue[particle_no],Form("dcaxy_vs_ptrue_%s",particles[particle_no].c_str()), "p (true) (GeV/c)", "dcaxy (cm)");
        GetFitSlicesY(hdcaxy_vs_etatrue[particle_no], fgaus_dcaxy, hdcaxy_Mean_vs_etatrue[particle_no], hdcaxy_Width_vs_etatrue[particle_no],Form("dcaxy_vs_etatrue_%s",particles[particle_no].c_str()), "#eta (true)", "dcaxy (cm)");
        GetFitSlicesY(hdcaxy_vs_phitrue[particle_no], fgaus_dcaxy, hdcaxy_Mean_vs_phitrue[particle_no], hdcaxy_Width_vs_phitrue[particle_no],Form("dcaxy_vs_phitrue_%s",particles[particle_no].c_str()), "#phi (true)", "dcaxy (cm)");
        
        GetFitSlicesY(hdcaxy_vs_ptreco[particle_no], fgaus_dcaxy, hdcaxy_Mean_vs_ptreco[particle_no], hdcaxy_Width_vs_ptreco[particle_no], Form("dcaxy_vs_ptreco_%s",particles[particle_no].c_str()), "p_{T} (reco) (GeV/c)", "dcaxy (cm)");
        GetFitSlicesY(hdcaxy_vs_preco[particle_no], fgaus_dcaxy, hdcaxy_Mean_vs_preco[particle_no], hdcaxy_Width_vs_preco[particle_no],Form("dcaxy_vs_preco_%s",particles[particle_no].c_str()), "p (reco) (GeV/c)", "dcaxy (cm)");
        GetFitSlicesY(hdcaxy_vs_etareco[particle_no], fgaus_dcaxy, hdcaxy_Mean_vs_etareco[particle_no], hdcaxy_Width_vs_etareco[particle_no],Form("dcaxy_vs_etareco_%s",particles[particle_no].c_str()), "#eta (reco)", "dcaxy (cm)");
        GetFitSlicesY(hdcaxy_vs_phireco[particle_no], fgaus_dcaxy, hdcaxy_Mean_vs_phireco[particle_no], hdcaxy_Width_vs_phireco[particle_no],Form("dcaxy_vs_phireco_%s",particles[particle_no].c_str()), "#phi (reco)", "dcaxy (cm)");
        
        GetFitSlicesY(hdcaz_vs_pttrue[particle_no], fgaus_dcaz, hdcaz_Mean_vs_pttrue[particle_no], hdcaz_Width_vs_pttrue[particle_no], Form("dcaz_vs_pttrue_%s",particles[particle_no].c_str()), "p_{T} (true) (GeV/c)", "dcaz (cm)");
        GetFitSlicesY(hdcaz_vs_ptrue[particle_no], fgaus_dcaz, hdcaz_Mean_vs_ptrue[particle_no], hdcaz_Width_vs_ptrue[particle_no],Form("dcaz_vs_ptrue_%s",particles[particle_no].c_str()), "p (true) (GeV/c)", "dcaz (cm)");
        GetFitSlicesY(hdcaz_vs_etatrue[particle_no], fgaus_dcaz, hdcaz_Mean_vs_etatrue[particle_no], hdcaz_Width_vs_etatrue[particle_no],Form("dcaz_vs_etatrue_%s",particles[particle_no].c_str()), "#eta (true)", "dcaz (cm)");
        GetFitSlicesY(hdcaz_vs_phitrue[particle_no], fgaus_dcaz, hdcaz_Mean_vs_phitrue[particle_no], hdcaz_Width_vs_phitrue[particle_no],Form("dcaz_vs_phitrue_%s",particles[particle_no].c_str()), "#phi (true)", "dcaz (cm)");
        
        GetFitSlicesY(hdcaz_vs_ptreco[particle_no], fgaus_dcaz, hdcaz_Mean_vs_ptreco[particle_no], hdcaz_Width_vs_ptreco[particle_no], Form("dcaz_vs_ptreco_%s",particles[particle_no].c_str()), "p_{T} (reco) (GeV/c)", "dcaz (cm)");
        GetFitSlicesY(hdcaz_vs_preco[particle_no], fgaus_dcaz, hdcaz_Mean_vs_preco[particle_no], hdcaz_Width_vs_preco[particle_no],Form("dcaz_vs_preco_%s",particles[particle_no].c_str()), "p (reco) (GeV/c)", "dcaz (cm)");
        GetFitSlicesY(hdcaz_vs_etareco[particle_no], fgaus_dcaz, hdcaz_Mean_vs_etareco[particle_no], hdcaz_Width_vs_etareco[particle_no],Form("dcaz_vs_etareco_%s",particles[particle_no].c_str()), "#eta (reco)", "dcaz (cm)");
        GetFitSlicesY(hdcaz_vs_phireco[particle_no], fgaus_dcaz, hdcaz_Mean_vs_phireco[particle_no], hdcaz_Width_vs_phireco[particle_no],Form("dcaz_vs_phireco_%s",particles[particle_no].c_str()), "#phi (reco)", "dcaz (cm)");
        
        
        GetFitSlicesY(htruedcaxy_vs_pttrue[particle_no], fgaus_truedcaxy, htruedcaxy_Mean_vs_pttrue[particle_no], htruedcaxy_Width_vs_pttrue[particle_no], Form("truedcaxy_vs_pttrue_%s",particles[particle_no].c_str()), "p_{T} (true) (GeV/c)", "truedcaxy (cm)");
        GetFitSlicesY(htruedcaxy_vs_ptrue[particle_no], fgaus_truedcaxy, htruedcaxy_Mean_vs_ptrue[particle_no], htruedcaxy_Width_vs_ptrue[particle_no],Form("truedcaxy_vs_ptrue_%s",particles[particle_no].c_str()), "p (true) (GeV/c)", "truedcaxy (cm)");
        GetFitSlicesY(htruedcaxy_vs_etatrue[particle_no], fgaus_truedcaxy, htruedcaxy_Mean_vs_etatrue[particle_no], htruedcaxy_Width_vs_etatrue[particle_no],Form("truedcaxy_vs_etatrue_%s",particles[particle_no].c_str()), "#eta (true)", "truedcaxy (cm)");
        GetFitSlicesY(htruedcaxy_vs_phitrue[particle_no], fgaus_truedcaxy, htruedcaxy_Mean_vs_phitrue[particle_no], htruedcaxy_Width_vs_phitrue[particle_no],Form("truedcaxy_vs_phitrue_%s",particles[particle_no].c_str()), "#phi (true)", "truedcaxy (cm)");
        
        GetFitSlicesY(htruedcaxy_vs_ptreco[particle_no], fgaus_truedcaxy, htruedcaxy_Mean_vs_ptreco[particle_no], htruedcaxy_Width_vs_ptreco[particle_no], Form("truedcaxy_vs_ptreco_%s",particles[particle_no].c_str()), "p_{T} (reco) (GeV/c)", "truedcaxy (cm)");
        GetFitSlicesY(htruedcaxy_vs_preco[particle_no], fgaus_truedcaxy, htruedcaxy_Mean_vs_preco[particle_no], htruedcaxy_Width_vs_preco[particle_no],Form("truedcaxy_vs_preco_%s",particles[particle_no].c_str()), "p (reco) (GeV/c)", "truedcaxy (cm)");
        GetFitSlicesY(htruedcaxy_vs_etareco[particle_no], fgaus_truedcaxy, htruedcaxy_Mean_vs_etareco[particle_no], htruedcaxy_Width_vs_etareco[particle_no],Form("truedcaxy_vs_etareco_%s",particles[particle_no].c_str()), "#eta (reco)", "truedcaxy (cm)");
        GetFitSlicesY(htruedcaxy_vs_phireco[particle_no], fgaus_truedcaxy, htruedcaxy_Mean_vs_phireco[particle_no], htruedcaxy_Width_vs_phireco[particle_no],Form("truedcaxy_vs_phireco_%s",particles[particle_no].c_str()), "#phi (reco)", "truedcaxy (cm)");
        
        GetFitSlicesY(htruedcaz_vs_pttrue[particle_no], fgaus_truedcaz, htruedcaz_Mean_vs_pttrue[particle_no], htruedcaz_Width_vs_pttrue[particle_no], Form("truedcaz_vs_pttrue_%s",particles[particle_no].c_str()), "p_{T} (true) (GeV/c)", "truedcaz (cm)");
        GetFitSlicesY(htruedcaz_vs_ptrue[particle_no], fgaus_truedcaz, htruedcaz_Mean_vs_ptrue[particle_no], htruedcaz_Width_vs_ptrue[particle_no],Form("truedcaz_vs_ptrue_%s",particles[particle_no].c_str()), "p (true) (GeV/c)", "truedcaz (cm)");
        GetFitSlicesY(htruedcaz_vs_etatrue[particle_no], fgaus_truedcaz, htruedcaz_Mean_vs_etatrue[particle_no], htruedcaz_Width_vs_etatrue[particle_no],Form("truedcaz_vs_etatrue_%s",particles[particle_no].c_str()), "#eta (true)", "truedcaz (cm)");
        GetFitSlicesY(htruedcaz_vs_phitrue[particle_no], fgaus_truedcaz, htruedcaz_Mean_vs_phitrue[particle_no], htruedcaz_Width_vs_phitrue[particle_no],Form("truedcaz_vs_phitrue_%s",particles[particle_no].c_str()), "#phi (true)", "truedcaz (cm)");
        
        GetFitSlicesY(htruedcaz_vs_ptreco[particle_no], fgaus_truedcaz, htruedcaz_Mean_vs_ptreco[particle_no], htruedcaz_Width_vs_ptreco[particle_no],Form("truedcaz_vs_ptreco_%s",particles[particle_no].c_str()), "p_{T} (reco) (GeV/c)", "truedcaz (cm)");
        GetFitSlicesY(htruedcaz_vs_preco[particle_no], fgaus_truedcaz, htruedcaz_Mean_vs_preco[particle_no], htruedcaz_Width_vs_preco[particle_no],Form("truedcaz_vs_preco_%s",particles[particle_no].c_str()), "p (reco) (GeV/c)", "truedcaz (cm)");
        GetFitSlicesY(htruedcaz_vs_etareco[particle_no], fgaus_truedcaz, htruedcaz_Mean_vs_etareco[particle_no], htruedcaz_Width_vs_etareco[particle_no],Form("truedcaz_vs_etareco_%s",particles[particle_no].c_str()), "#eta (reco)", "truedcaz (cm)");
        GetFitSlicesY(htruedcaz_vs_phireco[particle_no], fgaus_truedcaz, htruedcaz_Mean_vs_phireco[particle_no], htruedcaz_Width_vs_phireco[particle_no],Form("truedcaz_vs_phireco_%s",particles[particle_no].c_str()), "#phi (reco)", "truedcaz (cm)");
        
        
        GetFitSlicesY(hptRes_vs_pttrue[particle_no], fgaus_ptRes, hptRes_Mean_vs_pttrue[particle_no], hptRes_Width_vs_pttrue[particle_no], Form("ptRes_vs_pttrue_%s",particles[particle_no].c_str()), "p_{T} (true) (GeV/c)", "(p_{T}-p_{T}(true))/p_{T}(true)");
        GetFitSlicesY(hptRes_vs_ptrue[particle_no], fgaus_ptRes, hptRes_Mean_vs_ptrue[particle_no], hptRes_Width_vs_ptrue[particle_no], Form("ptRes_vs_ptrue_%s",particles[particle_no].c_str()), "p (true) (GeV/c)", "(p_{T}-p_{T}(true))/p_{T}(true)");
        GetFitSlicesY(hptRes_vs_etatrue[particle_no], fgaus_ptRes, hptRes_Mean_vs_etatrue[particle_no], hptRes_Width_vs_etatrue[particle_no], Form("ptRes_vs_etatrue_%s",particles[particle_no].c_str()), "#eta (true)", "(p_{T}-p_{T}(true))/p_{T}(true)");
        GetFitSlicesY(hptRes_vs_phitrue[particle_no], fgaus_ptRes, hptRes_Mean_vs_phitrue[particle_no], hptRes_Width_vs_phitrue[particle_no], Form("ptRes_vs_phitrue_%s",particles[particle_no].c_str()), "#phi (true)", "(p_{T}-p_{T}(true))/p_{T}(true)");
        
        GetFitSlicesY(hptRes_vs_ptreco[particle_no], fgaus_ptRes, hptRes_Mean_vs_ptreco[particle_no], hptRes_Width_vs_ptreco[particle_no], Form("ptRes_vs_ptreco_%s",particles[particle_no].c_str()), "p_{T} (reco) (GeV/c)", "(p_{T}-p_{T}(true))/p_{T}(true)");
        GetFitSlicesY(hptRes_vs_preco[particle_no], fgaus_ptRes, hptRes_Mean_vs_preco[particle_no], hptRes_Width_vs_preco[particle_no], Form("ptRes_vs_preco_%s",particles[particle_no].c_str()), "p (reco) (GeV/c)", "(p_{T}-p_{T}(true))/p_{T}(true)");
        GetFitSlicesY(hptRes_vs_etareco[particle_no], fgaus_ptRes, hptRes_Mean_vs_etareco[particle_no], hptRes_Width_vs_etareco[particle_no], Form("ptRes_vs_etareco_%s",particles[particle_no].c_str()), "#eta (reco)", "(p_{T}-p_{T}(true))/p_{T}(true)");
        GetFitSlicesY(hptRes_vs_phireco[particle_no], fgaus_ptRes, hptRes_Mean_vs_phireco[particle_no], hptRes_Width_vs_phireco[particle_no], Form("ptRes_vs_phireco_%s",particles[particle_no].c_str()), "#phi (reco)", "(p_{T}-p_{T}(true))/p_{T}(true)");
        
        
        GetFitSlicesY(hpRes_vs_pttrue[particle_no], fgaus_pRes, hpRes_Mean_vs_pttrue[particle_no], hpRes_Width_vs_pttrue[particle_no], Form("pRes_vs_pttrue_%s",particles[particle_no].c_str()), "p_{T} (true) (GeV/c)", "(p-p(true))/p(true)");
        GetFitSlicesY(hpRes_vs_ptrue[particle_no], fgaus_pRes, hpRes_Mean_vs_ptrue[particle_no], hpRes_Width_vs_ptrue[particle_no], Form("pRes_vs_ptrue_%s",particles[particle_no].c_str()), "p (true) (GeV/c)", "(p-p(true))/p(true)");
        GetFitSlicesY(hpRes_vs_etatrue[particle_no], fgaus_pRes, hpRes_Mean_vs_etatrue[particle_no], hpRes_Width_vs_etatrue[particle_no], Form("pRes_vs_etatrue_%s",particles[particle_no].c_str()), "#eta (true)", "(p-p(true))/p(true)");
        GetFitSlicesY(hpRes_vs_phitrue[particle_no], fgaus_pRes, hpRes_Mean_vs_phitrue[particle_no], hpRes_Width_vs_phitrue[particle_no], Form("pRes_vs_phitrue_%s",particles[particle_no].c_str()), "#phi (true)", "(p-p(true))/p(true)");
        
        GetFitSlicesY(hpRes_vs_ptreco[particle_no], fgaus_pRes, hpRes_Mean_vs_ptreco[particle_no], hpRes_Width_vs_ptreco[particle_no], Form("pRes_vs_ptreco_%s",particles[particle_no].c_str()), "p_{T} (reco) (GeV/c)", "(p-p(true))/p(true)");
        GetFitSlicesY(hpRes_vs_preco[particle_no], fgaus_pRes, hpRes_Mean_vs_preco[particle_no], hpRes_Width_vs_preco[particle_no], Form("pRes_vs_preco_%s",particles[particle_no].c_str()), "p (reco) (GeV/c)", "(p-p(true))/p(true)");
        GetFitSlicesY(hpRes_vs_etareco[particle_no], fgaus_pRes, hpRes_Mean_vs_etareco[particle_no], hpRes_Width_vs_etareco[particle_no], Form("pRes_vs_etareco_%s",particles[particle_no].c_str()), "#eta (reco)", "(p-p(true))/p(true)");
        GetFitSlicesY(hpRes_vs_phireco[particle_no], fgaus_pRes, hpRes_Mean_vs_phireco[particle_no], hpRes_Width_vs_phireco[particle_no], Form("pRes_vs_phireco_%s",particles[particle_no].c_str()), "#phi (reco)", "(p-p(true))/p(true)");
        
    }
    
    TFile *fout = new TFile(outputrootfilename, "RECREATE");
    
    hEta_allparticles->Write();
    hPhi_allparticles->Write();
    hPt_allparticles->Write();
    hp_allparticles->Write();
    
    hnmapsstate_vs_phi_allparticles->Write();
    hnmapsstate_Mean_vs_phi_allparticles->Write();
    hnmapsstate_Width_vs_phi_allparticles->Write();
    
    //dcaxy as a function of truth quantities
    hdcaxy_vs_pttrue_allparticles->Write();
    hdcaxy_Mean_vs_pttrue_allparticles->Write();
    hdcaxy_Width_vs_pttrue_allparticles->Write();
    
    hdcaxy_vs_ptrue_allparticles->Write();
    hdcaxy_Mean_vs_ptrue_allparticles->Write();
    hdcaxy_Width_vs_ptrue_allparticles->Write();
    
    hdcaxy_vs_etatrue_allparticles->Write();
    hdcaxy_Mean_vs_etatrue_allparticles->Write();
    hdcaxy_Width_vs_etatrue_allparticles->Write();
    
    hdcaxy_vs_phitrue_allparticles->Write();
    hdcaxy_Mean_vs_phitrue_allparticles->Write();
    hdcaxy_Width_vs_phitrue_allparticles->Write();
    
    //dcaxy as a function of reconstructed quantities
    hdcaxy_vs_ptreco_allparticles->Write();
    hdcaxy_Mean_vs_ptreco_allparticles->Write();
    hdcaxy_Width_vs_ptreco_allparticles->Write();
    
    hdcaxy_vs_preco_allparticles->Write();
    hdcaxy_Mean_vs_preco_allparticles->Write();
    hdcaxy_Width_vs_preco_allparticles->Write();
    
    hdcaxy_vs_etareco_allparticles->Write();
    hdcaxy_Mean_vs_etareco_allparticles->Write();
    hdcaxy_Width_vs_etareco_allparticles->Write();
    
    hdcaxy_vs_phireco_allparticles->Write();
    hdcaxy_Mean_vs_phireco_allparticles->Write();
    hdcaxy_Width_vs_phireco_allparticles->Write();
    
    //dcaz as a function of truth quantities
    hdcaz_vs_pttrue_allparticles->Write();
    hdcaz_Mean_vs_pttrue_allparticles->Write();
    hdcaz_Width_vs_pttrue_allparticles->Write();
    
    hdcaz_vs_ptrue_allparticles->Write();
    hdcaz_Mean_vs_ptrue_allparticles->Write();
    hdcaz_Width_vs_ptrue_allparticles->Write();
    
    hdcaz_vs_etatrue_allparticles->Write();
    hdcaz_Mean_vs_etatrue_allparticles->Write();
    hdcaz_Width_vs_etatrue_allparticles->Write();
    
    hdcaz_vs_phitrue_allparticles->Write();
    hdcaz_Mean_vs_phitrue_allparticles->Write();
    hdcaz_Width_vs_phitrue_allparticles->Write();
    
    //dcaz as a function of reconstructed quantities
    hdcaz_vs_ptreco_allparticles->Write();
    hdcaz_Mean_vs_ptreco_allparticles->Write();
    hdcaz_Width_vs_ptreco_allparticles->Write();
    
    hdcaz_vs_preco_allparticles->Write();
    hdcaz_Mean_vs_preco_allparticles->Write();
    hdcaz_Width_vs_preco_allparticles->Write();
    
    hdcaz_vs_etareco_allparticles->Write();
    hdcaz_Mean_vs_etareco_allparticles->Write();
    hdcaz_Width_vs_etareco_allparticles->Write();
    
    hdcaz_vs_phireco_allparticles->Write();
    hdcaz_Mean_vs_phireco_allparticles->Write();
    hdcaz_Width_vs_phireco_allparticles->Write();
    
    //truedcaxy as a function of truth quantities
    htruedcaxy_vs_pttrue_allparticles->Write();
    htruedcaxy_Mean_vs_pttrue_allparticles->Write();
    htruedcaxy_Width_vs_pttrue_allparticles->Write();
    
    htruedcaxy_vs_ptrue_allparticles->Write();
    htruedcaxy_Mean_vs_ptrue_allparticles->Write();
    htruedcaxy_Width_vs_ptrue_allparticles->Write();
    
    htruedcaxy_vs_etatrue_allparticles->Write();
    htruedcaxy_Mean_vs_etatrue_allparticles->Write();
    htruedcaxy_Width_vs_etatrue_allparticles->Write();
    
    htruedcaxy_vs_phitrue_allparticles->Write();
    htruedcaxy_Mean_vs_phitrue_allparticles->Write();
    htruedcaxy_Width_vs_phitrue_allparticles->Write();
    
    //truedcaxy as a function of reconstructed quantities
    htruedcaxy_vs_ptreco_allparticles->Write();
    htruedcaxy_Mean_vs_ptreco_allparticles->Write();
    htruedcaxy_Width_vs_ptreco_allparticles->Write();
    
    htruedcaxy_vs_preco_allparticles->Write();
    htruedcaxy_Mean_vs_preco_allparticles->Write();
    htruedcaxy_Width_vs_preco_allparticles->Write();
    
    htruedcaxy_vs_etareco_allparticles->Write();
    htruedcaxy_Mean_vs_etareco_allparticles->Write();
    htruedcaxy_Width_vs_etareco_allparticles->Write();
    
    htruedcaxy_vs_phireco_allparticles->Write();
    htruedcaxy_Mean_vs_phireco_allparticles->Write();
    htruedcaxy_Width_vs_phireco_allparticles->Write();
    
    //truedcaz as a function of truth quantities
    htruedcaz_vs_pttrue_allparticles->Write();
    htruedcaz_Mean_vs_pttrue_allparticles->Write();
    htruedcaz_Width_vs_pttrue_allparticles->Write();
    
    htruedcaz_vs_ptrue_allparticles->Write();
    htruedcaz_Mean_vs_ptrue_allparticles->Write();
    htruedcaz_Width_vs_ptrue_allparticles->Write();
    
    htruedcaz_vs_etatrue_allparticles->Write();
    htruedcaz_Mean_vs_etatrue_allparticles->Write();
    htruedcaz_Width_vs_etatrue_allparticles->Write();
    
    htruedcaz_vs_phitrue_allparticles->Write();
    htruedcaz_Mean_vs_phitrue_allparticles->Write();
    htruedcaz_Width_vs_phitrue_allparticles->Write();
    
    //truedcaz as a function of reconstructed quantities
    htruedcaz_vs_ptreco_allparticles->Write();
    htruedcaz_Mean_vs_ptreco_allparticles->Write();
    htruedcaz_Width_vs_ptreco_allparticles->Write();
    
    htruedcaz_vs_preco_allparticles->Write();
    htruedcaz_Mean_vs_preco_allparticles->Write();
    htruedcaz_Width_vs_preco_allparticles->Write();
    
    htruedcaz_vs_etareco_allparticles->Write();
    htruedcaz_Mean_vs_etareco_allparticles->Write();
    htruedcaz_Width_vs_etareco_allparticles->Write();
    
    htruedcaz_vs_phireco_allparticles->Write();
    htruedcaz_Mean_vs_phireco_allparticles->Write();
    htruedcaz_Width_vs_phireco_allparticles->Write();
    
    for(int particle_no=0;particle_no<nParticles;particle_no++){
        
        hEta[particle_no]->Write();
        hPhi[particle_no]->Write();
        hPt[particle_no]->Write();
        hp[particle_no]->Write();
        
        hnmapsstate_vs_phi[particle_no]->Write();
        hnmapsstate_Mean_vs_phi[particle_no]->Write();
        hnmapsstate_Width_vs_phi[particle_no]->Write();
        
        //dcaxy as a function of truth quantities
        hdcaxy_vs_pttrue[particle_no]->Write();
        hdcaxy_Mean_vs_pttrue[particle_no]->Write();
        hdcaxy_Width_vs_pttrue[particle_no]->Write();
        
        hdcaxy_vs_ptrue[particle_no]->Write();
        hdcaxy_Mean_vs_ptrue[particle_no]->Write();
        hdcaxy_Width_vs_ptrue[particle_no]->Write();
        
        hdcaxy_vs_etatrue[particle_no]->Write();
        hdcaxy_Mean_vs_etatrue[particle_no]->Write();
        hdcaxy_Width_vs_etatrue[particle_no]->Write();
        
        hdcaxy_vs_phitrue[particle_no]->Write();
        hdcaxy_Mean_vs_phitrue[particle_no]->Write();
        hdcaxy_Width_vs_phitrue[particle_no]->Write();
        
        //dcaxy as a function of reconstructed quantities
        hdcaxy_vs_ptreco[particle_no]->Write();
        hdcaxy_Mean_vs_ptreco[particle_no]->Write();
        hdcaxy_Width_vs_ptreco[particle_no]->Write();
        
        hdcaxy_vs_preco[particle_no]->Write();
        hdcaxy_Mean_vs_preco[particle_no]->Write();
        hdcaxy_Width_vs_preco[particle_no]->Write();
        
        hdcaxy_vs_etareco[particle_no]->Write();
        hdcaxy_Mean_vs_etareco[particle_no]->Write();
        hdcaxy_Width_vs_etareco[particle_no]->Write();
        
        hdcaxy_vs_phireco[particle_no]->Write();
        hdcaxy_Mean_vs_phireco[particle_no]->Write();
        hdcaxy_Width_vs_phireco[particle_no]->Write();
        
        //dcaz as a function of truth quantities
        hdcaz_vs_pttrue[particle_no]->Write();
        hdcaz_Mean_vs_pttrue[particle_no]->Write();
        hdcaz_Width_vs_pttrue[particle_no]->Write();
        
        hdcaz_vs_ptrue[particle_no]->Write();
        hdcaz_Mean_vs_ptrue[particle_no]->Write();
        hdcaz_Width_vs_ptrue[particle_no]->Write();
        
        hdcaz_vs_etatrue[particle_no]->Write();
        hdcaz_Mean_vs_etatrue[particle_no]->Write();
        hdcaz_Width_vs_etatrue[particle_no]->Write();
        
        hdcaz_vs_phitrue[particle_no]->Write();
        hdcaz_Mean_vs_phitrue[particle_no]->Write();
        hdcaz_Width_vs_phitrue[particle_no]->Write();
        
        //dcaz as a function of reconstructed quantities
        hdcaz_vs_ptreco[particle_no]->Write();
        hdcaz_Mean_vs_ptreco[particle_no]->Write();
        hdcaz_Width_vs_ptreco[particle_no]->Write();
        
        hdcaz_vs_preco[particle_no]->Write();
        hdcaz_Mean_vs_preco[particle_no]->Write();
        hdcaz_Width_vs_preco[particle_no]->Write();
        
        hdcaz_vs_etareco[particle_no]->Write();
        hdcaz_Mean_vs_etareco[particle_no]->Write();
        hdcaz_Width_vs_etareco[particle_no]->Write();
        
        hdcaz_vs_phireco[particle_no]->Write();
        hdcaz_Mean_vs_phireco[particle_no]->Write();
        hdcaz_Width_vs_phireco[particle_no]->Write();
        
        //truedcaxy as a function of truth quantities
        htruedcaxy_vs_pttrue[particle_no]->Write();
        htruedcaxy_Mean_vs_pttrue[particle_no]->Write();
        htruedcaxy_Width_vs_pttrue[particle_no]->Write();
        
        htruedcaxy_vs_ptrue[particle_no]->Write();
        htruedcaxy_Mean_vs_ptrue[particle_no]->Write();
        htruedcaxy_Width_vs_ptrue[particle_no]->Write();
        
        htruedcaxy_vs_etatrue[particle_no]->Write();
        htruedcaxy_Mean_vs_etatrue[particle_no]->Write();
        htruedcaxy_Width_vs_etatrue[particle_no]->Write();
        
        htruedcaxy_vs_phitrue[particle_no]->Write();
        htruedcaxy_Mean_vs_phitrue[particle_no]->Write();
        htruedcaxy_Width_vs_phitrue[particle_no]->Write();
        
        //truedcaxy as a function of reconstructed quantities
        htruedcaxy_vs_ptreco[particle_no]->Write();
        htruedcaxy_Mean_vs_ptreco[particle_no]->Write();
        htruedcaxy_Width_vs_ptreco[particle_no]->Write();
        
        htruedcaxy_vs_preco[particle_no]->Write();
        htruedcaxy_Mean_vs_preco[particle_no]->Write();
        htruedcaxy_Width_vs_preco[particle_no]->Write();
        
        htruedcaxy_vs_etareco[particle_no]->Write();
        htruedcaxy_Mean_vs_etareco[particle_no]->Write();
        htruedcaxy_Width_vs_etareco[particle_no]->Write();
        
        htruedcaxy_vs_phireco[particle_no]->Write();
        htruedcaxy_Mean_vs_phireco[particle_no]->Write();
        htruedcaxy_Width_vs_phireco[particle_no]->Write();
        
        //truedcaz as a function of truth quantities
        htruedcaz_vs_pttrue[particle_no]->Write();
        htruedcaz_Mean_vs_pttrue[particle_no]->Write();
        htruedcaz_Width_vs_pttrue[particle_no]->Write();
        
        htruedcaz_vs_ptrue[particle_no]->Write();
        htruedcaz_Mean_vs_ptrue[particle_no]->Write();
        htruedcaz_Width_vs_ptrue[particle_no]->Write();
        
        htruedcaz_vs_etatrue[particle_no]->Write();
        htruedcaz_Mean_vs_etatrue[particle_no]->Write();
        htruedcaz_Width_vs_etatrue[particle_no]->Write();
        
        htruedcaz_vs_phitrue[particle_no]->Write();
        htruedcaz_Mean_vs_phitrue[particle_no]->Write();
        htruedcaz_Width_vs_phitrue[particle_no]->Write();
        
        //truedcaz as a function of reconstructed quantities
        htruedcaz_vs_ptreco[particle_no]->Write();
        htruedcaz_Mean_vs_ptreco[particle_no]->Write();
        htruedcaz_Width_vs_ptreco[particle_no]->Write();
        
        htruedcaz_vs_preco[particle_no]->Write();
        htruedcaz_Mean_vs_preco[particle_no]->Write();
        htruedcaz_Width_vs_preco[particle_no]->Write();
        
        htruedcaz_vs_etareco[particle_no]->Write();
        htruedcaz_Mean_vs_etareco[particle_no]->Write();
        htruedcaz_Width_vs_etareco[particle_no]->Write();
        
        htruedcaz_vs_phireco[particle_no]->Write();
        htruedcaz_Mean_vs_phireco[particle_no]->Write();
        htruedcaz_Width_vs_phireco[particle_no]->Write();
        
        //ptRes as a function of truth quantities
        hptRes_vs_pttrue[particle_no]->Write();
        hptRes_Mean_vs_pttrue[particle_no]->Write();
        hptRes_Width_vs_pttrue[particle_no]->Write();
        
        hptRes_vs_ptrue[particle_no]->Write();
        hptRes_Mean_vs_ptrue[particle_no]->Write();
        hptRes_Width_vs_ptrue[particle_no]->Write();
        
        hptRes_vs_etatrue[particle_no]->Write();
        hptRes_Mean_vs_etatrue[particle_no]->Write();
        hptRes_Width_vs_etatrue[particle_no]->Write();
        
        hptRes_vs_phitrue[particle_no]->Write();
        hptRes_Mean_vs_phitrue[particle_no]->Write();
        hptRes_Width_vs_phitrue[particle_no]->Write();
        
        //ptRes as a function of reconstructed quantities
        hptRes_vs_ptreco[particle_no]->Write();
        hptRes_Mean_vs_ptreco[particle_no]->Write();
        hptRes_Width_vs_ptreco[particle_no]->Write();
        
        hptRes_vs_preco[particle_no]->Write();
        hptRes_Mean_vs_preco[particle_no]->Write();
        hptRes_Width_vs_preco[particle_no]->Write();
        
        hptRes_vs_etareco[particle_no]->Write();
        hptRes_Mean_vs_etareco[particle_no]->Write();
        hptRes_Width_vs_etareco[particle_no]->Write();
        
        hptRes_vs_phireco[particle_no]->Write();
        hptRes_Mean_vs_phireco[particle_no]->Write();
        hptRes_Width_vs_phireco[particle_no]->Write();
        
        //pRes as a function of truth quantities
        hpRes_vs_pttrue[particle_no]->Write();
        hpRes_Mean_vs_pttrue[particle_no]->Write();
        hpRes_Width_vs_pttrue[particle_no]->Write();
        
        hpRes_vs_ptrue[particle_no]->Write();
        hpRes_Mean_vs_ptrue[particle_no]->Write();
        hpRes_Width_vs_ptrue[particle_no]->Write();
        
        hpRes_vs_etatrue[particle_no]->Write();
        hpRes_Mean_vs_etatrue[particle_no]->Write();
        hpRes_Width_vs_etatrue[particle_no]->Write();
        
        hpRes_vs_phitrue[particle_no]->Write();
        hpRes_Mean_vs_phitrue[particle_no]->Write();
        hpRes_Width_vs_phitrue[particle_no]->Write();
        
        //pRes as a function of reconstructed quantities
        hpRes_vs_ptreco[particle_no]->Write();
        hpRes_Mean_vs_ptreco[particle_no]->Write();
        hpRes_Width_vs_ptreco[particle_no]->Write();
        
        hpRes_vs_preco[particle_no]->Write();
        hpRes_Mean_vs_preco[particle_no]->Write();
        hpRes_Width_vs_preco[particle_no]->Write();
        
        hpRes_vs_etareco[particle_no]->Write();
        hpRes_Mean_vs_etareco[particle_no]->Write();
        hpRes_Width_vs_etareco[particle_no]->Write();
        
        hpRes_vs_phireco[particle_no]->Write();
        hpRes_Mean_vs_phireco[particle_no]->Write();
        hpRes_Width_vs_phireco[particle_no]->Write();
    }
    
    fout->Close();
    cout<<Form("%s has been created",outputrootfilename)<<endl;
    
    if(createCanvases){
        
        //dcaxy as a function of truth quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_dcaxyfits_vstruth",hdcaxy_Mean_vs_pttrue,hdcaxy_Width_vs_pttrue,hdcaxy_Mean_vs_ptrue,hdcaxy_Width_vs_ptrue,hdcaxy_Mean_vs_etatrue,hdcaxy_Width_vs_etatrue,hdcaxy_Mean_vs_phitrue,hdcaxy_Width_vs_phitrue, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, cuts_info, dcaxy_max_formeanwidthcalc);
        
        //dcaxy as a function of reconstructed quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_dcaxyfits_vsreco",hdcaxy_Mean_vs_ptreco,hdcaxy_Width_vs_ptreco,hdcaxy_Mean_vs_preco,hdcaxy_Width_vs_preco,hdcaxy_Mean_vs_etareco,hdcaxy_Width_vs_etareco,hdcaxy_Mean_vs_phireco,hdcaxy_Width_vs_phireco, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, cuts_info, dcaxy_max_formeanwidthcalc);
        
        //dcaz as a function of truth quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_dcazfits_vstruth",hdcaz_Mean_vs_pttrue,hdcaz_Width_vs_pttrue,hdcaz_Mean_vs_ptrue,hdcaz_Width_vs_ptrue,hdcaz_Mean_vs_etatrue,hdcaz_Width_vs_etatrue,hdcaz_Mean_vs_phitrue,hdcaz_Width_vs_phitrue, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, cuts_info, dcaz_max_formeanwidthcalc);
        
        //dcaz as a function of reconstructed quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_dcazfits_vsreco",hdcaz_Mean_vs_ptreco,hdcaz_Width_vs_ptreco,hdcaz_Mean_vs_preco,hdcaz_Width_vs_preco,hdcaz_Mean_vs_etareco,hdcaz_Width_vs_etareco,hdcaz_Mean_vs_phireco,hdcaz_Width_vs_phireco, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, cuts_info, dcaz_max_formeanwidthcalc);
        
        //truedcaxy as a function of truth quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_truedcaxyfits_vstruth",htruedcaxy_Mean_vs_pttrue,htruedcaxy_Width_vs_pttrue,htruedcaxy_Mean_vs_ptrue,htruedcaxy_Width_vs_ptrue,htruedcaxy_Mean_vs_etatrue,htruedcaxy_Width_vs_etatrue,htruedcaxy_Mean_vs_phitrue,htruedcaxy_Width_vs_phitrue, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, cuts_info, truedcaxy_max_formeanwidthcalc);
        
        //truedcaxy as a function of reconstructed quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_truedcaxyfits_vsreco",htruedcaxy_Mean_vs_ptreco,htruedcaxy_Width_vs_ptreco,htruedcaxy_Mean_vs_preco,htruedcaxy_Width_vs_preco,htruedcaxy_Mean_vs_etareco,htruedcaxy_Width_vs_etareco,htruedcaxy_Mean_vs_phireco,htruedcaxy_Width_vs_phireco, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, cuts_info, truedcaxy_max_formeanwidthcalc);
        
        //truedcaz as a function of truth quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_truedcazfits_vstruth",htruedcaz_Mean_vs_pttrue,htruedcaz_Width_vs_pttrue,htruedcaz_Mean_vs_ptrue,htruedcaz_Width_vs_ptrue,htruedcaz_Mean_vs_etatrue,htruedcaz_Width_vs_etatrue,htruedcaz_Mean_vs_phitrue,htruedcaz_Width_vs_phitrue, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, cuts_info, truedcaz_max_formeanwidthcalc);
        
        //truedcaz as a function of reconstructed quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_truedcazfits_vsreco",htruedcaz_Mean_vs_ptreco,htruedcaz_Width_vs_ptreco,htruedcaz_Mean_vs_preco,htruedcaz_Width_vs_preco,htruedcaz_Mean_vs_etareco,htruedcaz_Width_vs_etareco,htruedcaz_Mean_vs_phireco,htruedcaz_Width_vs_phireco, particles, -0.5e-3, 0.5e-3, 0, 0.005, title_xstart, title_ystart, description, cuts_info, truedcaz_max_formeanwidthcalc);
        
        
        //ptRes as a function of truth quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_ptResfits_vstrue",hptRes_Mean_vs_pttrue,hptRes_Width_vs_pttrue,hptRes_Mean_vs_ptrue,hptRes_Width_vs_ptrue,hptRes_Mean_vs_etatrue,hptRes_Width_vs_etatrue,hptRes_Mean_vs_phitrue,hptRes_Width_vs_phitrue,particles, -0.02, 0.005, 0, 0.02, title_xstart, title_ystart, description, cuts_info, ptRes_max_formeanwidthcalc);
        
        //ptRes as a function of reconstructed quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_ptResfits_vsreco",hptRes_Mean_vs_ptreco,hptRes_Width_vs_ptreco,hptRes_Mean_vs_preco,hptRes_Width_vs_preco,hptRes_Mean_vs_etareco,hptRes_Width_vs_etareco,hptRes_Mean_vs_phireco,hptRes_Width_vs_phireco,particles, -0.02, 0.005, 0, 0.02, title_xstart, title_ystart, description, cuts_info, ptRes_max_formeanwidthcalc);
        
        //pRes as a function of truth quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_pResfits_vstrue",hpRes_Mean_vs_pttrue,hpRes_Width_vs_pttrue,hpRes_Mean_vs_ptrue,hpRes_Width_vs_ptrue,hpRes_Mean_vs_etatrue,hpRes_Width_vs_etatrue,hpRes_Mean_vs_phitrue,hpRes_Width_vs_phitrue,particles, -0.02, 0.005, 0, 0.02, title_xstart, title_ystart, description, cuts_info, pRes_max_formeanwidthcalc);
        
        //pRes as a function of reconstructed quantities
        CreateandSaveCanvaswithPID(outputdir,"particles_overlaidplots/canvas_pResfits_vsreco",hpRes_Mean_vs_ptreco,hpRes_Width_vs_ptreco,hpRes_Mean_vs_preco,hpRes_Width_vs_preco,hpRes_Mean_vs_etareco,hpRes_Width_vs_etareco,hpRes_Mean_vs_phireco,hpRes_Width_vs_phireco,particles, -0.02, 0.005, 0, 0.02, title_xstart, title_ystart, description, cuts_info, pRes_max_formeanwidthcalc);
        
        
        
        
        
        for(int particle_no=0;particle_no<nParticles;particle_no++){
            
            //dcaxy as a function of truth quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_dcaxydists_vstruth_%s",particles[particle_no].c_str()),hdcaxy_vs_pttrue[particle_no],hdcaxy_vs_ptrue[particle_no],hdcaxy_vs_etatrue[particle_no],hdcaxy_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, dcaxy_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_dcaxyfits_vstruth_%s",particles[particle_no].c_str()),hdcaxy_Mean_vs_pttrue[particle_no],hdcaxy_Width_vs_pttrue[particle_no],hdcaxy_Mean_vs_ptrue[particle_no],hdcaxy_Width_vs_ptrue[particle_no],hdcaxy_Mean_vs_etatrue[particle_no],hdcaxy_Width_vs_etatrue[particle_no],hdcaxy_Mean_vs_phitrue[particle_no],hdcaxy_Width_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, dcaxy_max_formeanwidthcalc);
            
            //dcaxy as a function of reconstructed quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_dcaxydists_vsreco_%s",particles[particle_no].c_str()),hdcaxy_vs_ptreco[particle_no],hdcaxy_vs_preco[particle_no],hdcaxy_vs_etareco[particle_no],hdcaxy_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, dcaxy_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_dcaxyfits_vsreco_%s",particles[particle_no].c_str()),hdcaxy_Mean_vs_ptreco[particle_no],hdcaxy_Width_vs_ptreco[particle_no],hdcaxy_Mean_vs_preco[particle_no],hdcaxy_Width_vs_preco[particle_no],hdcaxy_Mean_vs_etareco[particle_no],hdcaxy_Width_vs_etareco[particle_no],hdcaxy_Mean_vs_phireco[particle_no],hdcaxy_Width_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, dcaxy_max_formeanwidthcalc);
            
            //dcaz as a function of truth quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_dcazdists_vstruth_%s",particles[particle_no].c_str()),hdcaz_vs_pttrue[particle_no],hdcaz_vs_ptrue[particle_no],hdcaz_vs_etatrue[particle_no],hdcaz_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, dcaz_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_dcazfits_vstruth_%s",particles[particle_no].c_str()),hdcaz_Mean_vs_pttrue[particle_no],hdcaz_Width_vs_pttrue[particle_no],hdcaz_Mean_vs_ptrue[particle_no],hdcaz_Width_vs_ptrue[particle_no],hdcaz_Mean_vs_etatrue[particle_no],hdcaz_Width_vs_etatrue[particle_no],hdcaz_Mean_vs_phitrue[particle_no],hdcaz_Width_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, dcaz_max_formeanwidthcalc);
            
            //dcaz as a function of reconstructed quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_dcazdists_vsreco_%s",particles[particle_no].c_str()),hdcaz_vs_ptreco[particle_no],hdcaz_vs_preco[particle_no],hdcaz_vs_etareco[particle_no],hdcaz_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, dcaz_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_dcazfits_vsreco_%s",particles[particle_no].c_str()),hdcaz_Mean_vs_ptreco[particle_no],hdcaz_Width_vs_ptreco[particle_no],hdcaz_Mean_vs_preco[particle_no],hdcaz_Width_vs_preco[particle_no],hdcaz_Mean_vs_etareco[particle_no],hdcaz_Width_vs_etareco[particle_no],hdcaz_Mean_vs_phireco[particle_no],hdcaz_Width_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, dcaz_max_formeanwidthcalc);
            
            //truedcaxy as a function of truth quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_truedcaxydists_vstruth_%s",particles[particle_no].c_str()),htruedcaxy_vs_pttrue[particle_no],htruedcaxy_vs_ptrue[particle_no],htruedcaxy_vs_etatrue[particle_no],htruedcaxy_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, truedcaxy_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_truedcaxyfits_vstruth_%s",particles[particle_no].c_str()),htruedcaxy_Mean_vs_pttrue[particle_no],htruedcaxy_Width_vs_pttrue[particle_no],htruedcaxy_Mean_vs_ptrue[particle_no],htruedcaxy_Width_vs_ptrue[particle_no],htruedcaxy_Mean_vs_etatrue[particle_no],htruedcaxy_Width_vs_etatrue[particle_no],htruedcaxy_Mean_vs_phitrue[particle_no],htruedcaxy_Width_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, truedcaxy_max_formeanwidthcalc);
            
            //truedcaxy as a function of reconstructed quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_truedcaxydists_vsreco_%s",particles[particle_no].c_str()),htruedcaxy_vs_ptreco[particle_no],htruedcaxy_vs_preco[particle_no],htruedcaxy_vs_etareco[particle_no],htruedcaxy_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, truedcaxy_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_truedcaxyfits_vsreco_%s",particles[particle_no].c_str()),htruedcaxy_Mean_vs_ptreco[particle_no],htruedcaxy_Width_vs_ptreco[particle_no],htruedcaxy_Mean_vs_preco[particle_no],htruedcaxy_Width_vs_preco[particle_no],htruedcaxy_Mean_vs_etareco[particle_no],htruedcaxy_Width_vs_etareco[particle_no],htruedcaxy_Mean_vs_phireco[particle_no],htruedcaxy_Width_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, truedcaxy_max_formeanwidthcalc);
            
            //truedcaz as a function of truth quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_truedcazdists_vstruth_%s",particles[particle_no].c_str()),htruedcaz_vs_pttrue[particle_no],htruedcaz_vs_ptrue[particle_no],htruedcaz_vs_etatrue[particle_no],htruedcaz_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, truedcaz_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_truedcazfits_vstruth_%s",particles[particle_no].c_str()),htruedcaz_Mean_vs_pttrue[particle_no],htruedcaz_Width_vs_pttrue[particle_no],htruedcaz_Mean_vs_ptrue[particle_no],htruedcaz_Width_vs_ptrue[particle_no],htruedcaz_Mean_vs_etatrue[particle_no],htruedcaz_Width_vs_etatrue[particle_no],htruedcaz_Mean_vs_phitrue[particle_no],htruedcaz_Width_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, truedcaz_max_formeanwidthcalc);
            
            //truedcaz as a function of reconstructed quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_truedcazdists_vsreco_%s",particles[particle_no].c_str()),htruedcaz_vs_ptreco[particle_no],htruedcaz_vs_preco[particle_no],htruedcaz_vs_etareco[particle_no],htruedcaz_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, truedcaz_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_truedcazfits_vsreco_%s",particles[particle_no].c_str()),htruedcaz_Mean_vs_ptreco[particle_no],htruedcaz_Width_vs_ptreco[particle_no],htruedcaz_Mean_vs_preco[particle_no],htruedcaz_Width_vs_preco[particle_no],htruedcaz_Mean_vs_etareco[particle_no],htruedcaz_Width_vs_etareco[particle_no],htruedcaz_Mean_vs_phireco[particle_no],htruedcaz_Width_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, truedcaz_max_formeanwidthcalc);
            
            //ptRes as a function of truth quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_ptResdists_vstruth_%s",particles[particle_no].c_str()),hptRes_vs_pttrue[particle_no],hptRes_vs_ptrue[particle_no],hptRes_vs_etatrue[particle_no],hptRes_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, ptRes_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_ptResfits_vstruth_%s",particles[particle_no].c_str()),hptRes_Mean_vs_pttrue[particle_no],hptRes_Width_vs_pttrue[particle_no],hptRes_Mean_vs_ptrue[particle_no],hptRes_Width_vs_ptrue[particle_no],hptRes_Mean_vs_etatrue[particle_no],hptRes_Width_vs_etatrue[particle_no],hptRes_Mean_vs_phitrue[particle_no],hptRes_Width_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, ptRes_max_formeanwidthcalc);
            
            //ptRes as a function of reconstructed quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_ptResdists_vsreco_%s",particles[particle_no].c_str()),hptRes_vs_ptreco[particle_no],hptRes_vs_preco[particle_no],hptRes_vs_etareco[particle_no],hptRes_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, ptRes_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_ptResfits_vsreco_%s",particles[particle_no].c_str()),hptRes_Mean_vs_ptreco[particle_no],hptRes_Width_vs_ptreco[particle_no],hptRes_Mean_vs_preco[particle_no],hptRes_Width_vs_preco[particle_no],hptRes_Mean_vs_etareco[particle_no],hptRes_Width_vs_etareco[particle_no],hptRes_Mean_vs_phireco[particle_no],hptRes_Width_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, ptRes_max_formeanwidthcalc);
            
            //pRes as a function of truth quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_pResdists_vstruth_%s",particles[particle_no].c_str()),hpRes_vs_pttrue[particle_no],hpRes_vs_ptrue[particle_no],hpRes_vs_etatrue[particle_no],hpRes_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, pRes_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_pResfits_vstruth_%s",particles[particle_no].c_str()),hpRes_Mean_vs_pttrue[particle_no],hpRes_Width_vs_pttrue[particle_no],hpRes_Mean_vs_ptrue[particle_no],hpRes_Width_vs_ptrue[particle_no],hpRes_Mean_vs_etatrue[particle_no],hpRes_Width_vs_etatrue[particle_no],hpRes_Mean_vs_phitrue[particle_no],hpRes_Width_vs_phitrue[particle_no],title_xstart, title_ystart, description, cuts_info, pRes_max_formeanwidthcalc);
            
            //pRes as a function of recontructed quantities
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_pResdists_vsreco_%s",particles[particle_no].c_str()),hpRes_vs_ptreco[particle_no],hpRes_vs_preco[particle_no],hpRes_vs_etareco[particle_no],hpRes_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, pRes_max_formeanwidthcalc);
            
            CreateandSaveCanvaswithPID(outputdir,Form("canvas_pResfits_vsreco_%s",particles[particle_no].c_str()),hpRes_Mean_vs_ptreco[particle_no],hpRes_Width_vs_ptreco[particle_no],hpRes_Mean_vs_preco[particle_no],hpRes_Width_vs_preco[particle_no],hpRes_Mean_vs_etareco[particle_no],hpRes_Width_vs_etareco[particle_no],hpRes_Mean_vs_phireco[particle_no],hpRes_Width_vs_phireco[particle_no],title_xstart, title_ystart, description, cuts_info, pRes_max_formeanwidthcalc);
        }
         
    }
    
    return 1;
    
}
