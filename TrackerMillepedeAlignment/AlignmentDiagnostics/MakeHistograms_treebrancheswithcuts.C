#include <TChain.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TString.h>
#include <iostream>

void MakeHistograms_treebrancheswithcuts(
    int startIndex = 0,
    int endIndex = 249,
    const char* filePrefix = "collision_79516",
    const char* treeName = "clustertree",
    const char* xBranch = "m_scluslx",
    const char* yBranch = "m_scluslz",
    const char* cut = "m_staveid>0"
)
{
    TChain chain(treeName);

    for (int i = startIndex; i <= endIndex; i++) {
        TString filename = Form("%s-%d.root_resid.root", filePrefix, i);
        int added = chain.Add(filename);

        if (added == 0) {
            std::cout << "Warning: could not add " << filename << std::endl;
        } else {
            std::cout << "Added " << filename << std::endl;
        }
    }

    TH2D* hXY = new TH2D(
        "hXY",
        Form("x vs y for %s;x;y;Counts",cut),
        100, -1, 1,
        100, -1, 1
    );

    TString drawExpr = Form("%s:%s >> hXY", yBranch, xBranch);
    TString cutExpr  = Form("%s", cut);

    chain.Draw(drawExpr, cutExpr, "COLZ");

    gStyle->SetOptStat(0);

    TCanvas* c1 = new TCanvas("c1", "x vs y", 800, 700);
    hXY->Draw("COLZ");

    c1->SaveAs("x_vs_y_zcut.png");
    c1->SaveAs("x_vs_y_zcut.pdf");

    TFile* outFile = new TFile("x_vs_y_zcut.root", "RECREATE");
    hXY->Write();
    c1->Write();
    outFile->Close();
}