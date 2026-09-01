#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TString.h>
#include <TMath.h>

#include <iostream>
#include <vector>

void PlotTruthTrackQuantities(
    TString infile = "input.root",
    TString treename = "truthtree")
{
    gStyle->SetOptStat(0);

    //----------------------------------------------------------
    // cuts
    //----------------------------------------------------------

    float nmaps_cut = 1;
    float intt_cut  = 0;
    float tpc_cut   = 25;
    float pt_cut    = 0.2;

    //----------------------------------------------------------
    // open input
    //----------------------------------------------------------

    TFile *fin = TFile::Open(infile);

    if (!fin || fin->IsZombie())
    {
        std::cout << "Cannot open file!" << std::endl;
        return;
    }

    TTree *tree = (TTree*)fin->Get(treename);

    if (!tree)
    {
        std::cout << "Cannot find tree: " << treename << std::endl;
        return;
    }

    //----------------------------------------------------------
    // output ROOT file
    //----------------------------------------------------------

    TFile *fout =
        new TFile("TruthTrackPlots.root", "RECREATE");

    //----------------------------------------------------------
    // branches
    //----------------------------------------------------------

    int event;
    int gtrackid;
    int gflavor_truth;
    int gprimary;

    float gpx;
    float gpy;
    float gpz;

    float gpt;
    float geta;
    float gphi;

    float gvx;
    float gvy;
    float gvz;
    float gvt;

    int gnmaps;
    int gnintt;
    int gntpc;
    int gnmms;

    int gmatched_reco_trackid;

    tree->SetBranchAddress("event", &event);
    tree->SetBranchAddress("gtrackid", &gtrackid);
    tree->SetBranchAddress("gflavor_truth", &gflavor_truth);
    tree->SetBranchAddress("gprimary", &gprimary);

    tree->SetBranchAddress("gpx", &gpx);
    tree->SetBranchAddress("gpy", &gpy);
    tree->SetBranchAddress("gpz", &gpz);

    tree->SetBranchAddress("gpt", &gpt);
    tree->SetBranchAddress("geta", &geta);
    tree->SetBranchAddress("gphi", &gphi);

    tree->SetBranchAddress("gvx", &gvx);
    tree->SetBranchAddress("gvy", &gvy);
    tree->SetBranchAddress("gvz", &gvz);
    tree->SetBranchAddress("gvt", &gvt);

    tree->SetBranchAddress("gnmaps", &gnmaps);
    tree->SetBranchAddress("gnintt", &gnintt);
    tree->SetBranchAddress("gntpc", &gntpc);
    tree->SetBranchAddress("gnmms", &gnmms);

    tree->SetBranchAddress(
        "gmatched_reco_trackid",
        &gmatched_reco_trackid);

    //----------------------------------------------------------
    // categories
    //----------------------------------------------------------

    enum HistIndex
    {
        kAllTracks = 0,
        kPassAllCuts,
        kFailnmapsCuts,
        kFailinttCuts,
        kFailTPCcuts,
        kFailpTcuts,
        kMatchedReco,
        kUnmatchedReco,
        kNsets
    };

    TString labels[kNsets] =
    {
        "alltracks",
        "passing trackcuts",
        "failing nmaps cut",
        "failing INTT cut",
        "failing TPC cut",
        "failing pT cut",
        "matched reco tracks",
        "unmatched reco tracks"
    };

    int colors[kNsets] =
    {
        kBlue+1,
        kOrange+7,
        kGreen+2,
        kMagenta+1,
        kRed+1,
        kBlack,
        kCyan+2,
        kGray+2
    };

    //----------------------------------------------------------
    // histograms
    //----------------------------------------------------------

    TH1D *h_eta[kNsets];
    TH1D *h_pt[kNsets];
    TH1D *h_p[kNsets];
    TH1D *h_phi[kNsets];

    for (int i = 0; i < kNsets; ++i)
    {
        h_eta[i] = new TH1D(
            Form("h_eta_%d", i),
            ";#eta;tracks",
            100, -1.2, 1.2);

        h_pt[i] = new TH1D(
            Form("h_pt_%d", i),
            ";p_{T} [GeV/c];tracks",
            100, 0, 10);

        h_p[i] = new TH1D(
            Form("h_p_%d", i),
            ";p [GeV/c];tracks",
            100, 0, 20);

        h_phi[i] = new TH1D(
            Form("h_phi_%d", i),
            ";#phi;tracks",
            100, -TMath::Pi(), TMath::Pi());

        h_eta[i]->SetLineColor(colors[i]);
        h_pt[i]->SetLineColor(colors[i]);
        h_p[i]->SetLineColor(colors[i]);
        h_phi[i]->SetLineColor(colors[i]);

        h_eta[i]->SetLineWidth(2);
        h_pt[i]->SetLineWidth(2);
        h_p[i]->SetLineWidth(2);
        h_phi[i]->SetLineWidth(2);
    }

    //----------------------------------------------------------
    // event loop
    //----------------------------------------------------------

    Long64_t nentries = tree->GetEntries();

    for (Long64_t i = 0; i < nentries; ++i)
    {
        tree->GetEntry(i);

        //------------------------------------------------------
        // momentum
        //------------------------------------------------------

        float p = std::sqrt(
            gpx*gpx +
            gpy*gpy +
            gpz*gpz);

        //------------------------------------------------------
        // cuts
        //------------------------------------------------------

        bool pass_nmaps = (gnmaps > nmaps_cut);
        bool pass_intt  = (gnintt > intt_cut);
        bool pass_tpc   = (gntpc > tpc_cut);
        bool pass_pt    = (gpt >= pt_cut);

        bool pass_all =
            pass_nmaps &&
            pass_intt &&
            pass_tpc &&
            pass_pt;

        //------------------------------------------------------
        // matching
        //------------------------------------------------------

        bool matched_reco =
            (gmatched_reco_trackid >= 0);

        bool unmatched_reco =
            !matched_reco;

        //------------------------------------------------------
        // category filling
        //------------------------------------------------------

        bool fill_set[kNsets] = {false};

        fill_set[kAllTracks]      = true;
        fill_set[kPassAllCuts]    = pass_all;
        fill_set[kFailnmapsCuts]  = !pass_nmaps;
        fill_set[kFailinttCuts]   = !pass_intt;
        fill_set[kFailTPCcuts]    = !pass_tpc;
        fill_set[kFailpTcuts]     = !pass_pt;
        fill_set[kMatchedReco]    = matched_reco;
        fill_set[kUnmatchedReco]  = unmatched_reco;

        //------------------------------------------------------
        // fill
        //------------------------------------------------------

        for (int ih = 0; ih < kNsets; ++ih)
        {
            if (!fill_set[ih]) continue;

            h_eta[ih]->Fill(geta);
            h_pt[ih]->Fill(gpt);
            h_p[ih]->Fill(p);
            h_phi[ih]->Fill(gphi);
        }
    }

    //----------------------------------------------------------
    // draw helper
    //----------------------------------------------------------

    auto DrawHistSet =
    [&](TH1D* hists[kNsets],
        TString cname,
        TString title)
    {
        TCanvas *c =
            new TCanvas(cname, cname, 1200, 900);

        double maxy = 0;

        for (int i = 0; i < kNsets; ++i)
        {
            if (hists[i]->GetMaximum() > maxy)
                maxy = hists[i]->GetMaximum();
        }

        hists[0]->SetMaximum(1.15 * maxy);
        hists[0]->SetMinimum(0.0);
        hists[0]->SetTitle(title);

        hists[0]->Draw("HIST");

        for (int i = 1; i < kNsets; ++i)
        {
            hists[i]->Draw("HIST SAME");
        }

        TLegend *leg =
            new TLegend(0.58, 0.55, 0.90, 0.90);

        for (int i = 0; i < kNsets; ++i)
        {
            leg->AddEntry(hists[i], labels[i], "l");
        }

        leg->SetBorderSize(1);
        leg->Draw();

        //------------------------------------------------------
        // save to ROOT file
        //------------------------------------------------------

        fout->cd();

        c->Write();
    };

    //----------------------------------------------------------
    // draw canvases
    //----------------------------------------------------------

    DrawHistSet(
        h_eta,
        "truth_eta_distributions",
        "#eta distributions_truthtracks");

    DrawHistSet(
        h_pt,
        "truth_pt_distributions",
        "p_{T} distributions_truthtracks");

    DrawHistSet(
        h_p,
        "truth_p_distributions",
        "p distributions_truthtracks");

    DrawHistSet(
        h_phi,
        "truth_phi_distributions",
        "#phi distributions_truthtracks");

    //----------------------------------------------------------
    // write histograms
    //----------------------------------------------------------

    fout->cd();

    for (int i = 0; i < kNsets; ++i)
    {
        h_eta[i]->Write();
        h_pt[i]->Write();
        h_p[i]->Write();
        h_phi[i]->Write();
    }

    //----------------------------------------------------------
    // close
    //----------------------------------------------------------

    fout->Close();

    std::cout << std::endl;
    std::cout << "Saved output to TruthTrackPlots.root"
              << std::endl;
}
