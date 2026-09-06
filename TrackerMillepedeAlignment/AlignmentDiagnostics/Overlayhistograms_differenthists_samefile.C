#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLine.h"

struct HistInfo
{
    std::string name;
    bool use_y_range;
    double ymin;
    double ymax;
};

void Overlayhistograms_differenthists_samefile()
{
    //const char* filename="output_PlottingMacro_singleTrackQuantities_Simulation.root";

    const char* filename = "output_PlottingMacro_singleTrackQuantities_data_sionlytracks.root";
    const char* fileinfo = "p+p sPHENIX sionly tracks";
    const char* trackcuts = "TrackCuts: m_nmaps>=2 && m_nintt>=2 && m_ntpc>=0 && pt>=0.2 GeV/c";

    std::vector<HistInfo> histinfos = {
        {"Gaussian_Mean_of_dcaxy_vs_ptreco_allpid_passcuts",  true, -0.004, 0.004},
        {"Gaussian_Width_of_dcaxy_vs_ptreco_allpid_passcuts",  true,  0.0,   0.02},
        {"Gaussian_Mean_of_dcaxy_vs_preco_allpid_passcuts",    true, -0.004, 0.004},
        {"Gaussian_Width_of_dcaxy_vs_preco_allpid_passcuts",    true,  0.0,   0.02},
        {"Gaussian_Mean_of_dcaxy_vs_etareco_allpid_passcuts",   true, -0.015, 0.015},
        {"Gaussian_Width_of_dcaxy_vs_etareco_allpid_passcuts",  true,  0.0,   0.02},
        {"Gaussian_Mean_of_dcaxy_vs_phireco_allpid_passcuts",   true, -0.005, 0.005},
        {"Gaussian_Width_of_dcaxy_vs_phireco_allpid_passcuts",   true,  0.0,   0.02},

        {"Gaussian_Mean_of_dcaz_vs_ptreco_allpid_passcuts",     true, -0.004, 0.004},
        {"Gaussian_Width_of_dcaz_vs_ptreco_allpid_passcuts",     true,  0.0,   0.02},
        {"Gaussian_Mean_of_dcaz_vs_preco_allpid_passcuts",      true, -0.004, 0.004},
        {"Gaussian_Width_of_dcaz_vs_preco_allpid_passcuts",     true,  0.0,   0.02},
        {"Gaussian_Mean_of_dcaz_vs_etareco_allpid_passcuts",    true, -0.015, 0.015},
        {"Gaussian_Width_of_dcaz_vs_etareco_allpid_passcuts",    true,  0.0,   0.02},
        {"Gaussian_Mean_of_dcaz_vs_phireco_allpid_passcuts",    true, -0.005, 0.005},
        {"Gaussian_Width_of_dcaz_vs_phireco_allpid_passcuts",    true,  0.0,   0.02},
    };

    std::vector<std::string> charge_suffixes = {
        "_charge1",
        "_chargem1",
    };

    std::vector<std::string> legend_labels = {
        "charge = +1",
        "charge = -1"
    };

    if (histinfos.empty())
    {
        std::cout << "Error: histinfos is empty." << std::endl;
        return;
    }

    if (charge_suffixes.size() != legend_labels.size())
    {
        std::cout << "Error: charge_suffixes and legend_labels must have the same size." << std::endl;
        return;
    }

    gStyle->SetOptStat(0);

    TFile* infile = TFile::Open(filename, "READ");

    if (!infile || infile->IsZombie())
    {
        std::cout << "Error: could not open file " << filename << std::endl;
        return;
    }

    std::vector<int> colors = {
        kRed + 1,
        kBlue + 1,
        kGreen + 2,
        kMagenta + 1,
        kOrange + 7,
        kCyan + 2,
        kViolet + 1,
        kBlack
    };

    std::vector<int> line_styles = {
        1, 2, 3, 4, 5, 6, 7, 8
    };
    
    if (charge_suffixes.size() > colors.size() || charge_suffixes.size() > line_styles.size())
    {
        std::cout << "Error: " << charge_suffixes.size() << " suffixes but only "
                  << colors.size() << " colors and "
                  << line_styles.size() << " line styles defined.\n"
                  << "Add more entries to the colors/line_styles vectors and rerun." << std::endl;
        infile->Close();
        delete infile;
        return;
    }

    std::string clean_filename = filename;
    {
        const std::size_t slash_pos = clean_filename.find_last_of("/\\");
        if (slash_pos != std::string::npos)
        {
            clean_filename = clean_filename.substr(slash_pos + 1);
        }
        const std::string root_ext = ".root";
        if (clean_filename.size() > root_ext.size() &&
            clean_filename.substr(clean_filename.size() - root_ext.size()) == root_ext)
        {
            clean_filename = clean_filename.substr(0, clean_filename.size() - root_ext.size());
        }
    }
    const std::string output_name =
        "Output_overlayhistograms_differenthists_" + clean_filename + ".pdf";

    TCanvas* c = new TCanvas("DrawOverlaySet_canvas", "DrawOverlaySet_canvas", 900, 650);
    c->Print((output_name + "[").c_str());

    for (const HistInfo& histinfo : histinfos)
    {
        const std::string& basename = histinfo.name;
        std::vector<TH1*> histograms;
        std::vector<std::size_t> index_found;

        for (size_t i = 0; i < charge_suffixes.size(); ++i)
        {
            const std::string hist_name = basename + charge_suffixes[i];
            TH1* h = dynamic_cast<TH1*>(infile->Get(hist_name.c_str()));

            if (!h)
            {
                std::cout << "Warning: histogram \"" << hist_name
                          << "\" not found in file " << filename
                          << ". Skipping it." << std::endl;
                continue;
            }

            h->SetDirectory(nullptr);
            h->SetStats(0);
            histograms.push_back(h);
            index_found.push_back(i);
            //labels_found.push_back(legend_labels[i]);
        }

        if (histograms.empty())
        {
            std::cout << "Warning: none of the requested histograms were found for "
                      << basename << std::endl;
            continue;
        }

        for (size_t i = 0; i < histograms.size(); ++i)
        {
            TH1* h = histograms[i];

            const int color = colors[index_found[i]];
            const int style = line_styles[index_found[i]];

            h->SetLineColor(color);
            h->SetMarkerColor(color);
            h->SetMarkerStyle(20 + static_cast<int>(index_found[i]));
            h->SetLineStyle(style);
            h->SetLineWidth(2);
        }

        if (histinfo.use_y_range)
        {
            double axis_ymin = histinfo.ymin;
            double axis_ymax = histinfo.ymax;

            if (axis_ymax <= axis_ymin)
            {
                std::cout << "Warning: invalid y-range for " << basename
                          << " using fallback [0, 1]." << std::endl;
                axis_ymin = 0.0;
                axis_ymax = 1.0;
            }

            histograms[0]->SetMinimum(axis_ymin);
            histograms[0]->SetMaximum(axis_ymax);
        }
        
        c->Clear();
        c->SetLeftMargin(0.14);
        c->SetRightMargin(0.05);
        c->SetTopMargin(0.11);
        c->SetBottomMargin(0.12);
        c->cd();

        histograms[0]->SetTitle(basename.c_str());
        histograms[0]->Draw("E1");

        for (size_t i = 1; i < histograms.size(); ++i)
        {
            histograms[i]->Draw("E1 SAME");
        }
        
        TLine* zero_line = new TLine(
            histograms[0]->GetXaxis()->GetXmin(), 0,
            histograms[0]->GetXaxis()->GetXmax(), 0);
        zero_line->SetLineStyle(2);
        zero_line->SetLineColor(kGray + 1);
        zero_line->Draw("SAME");

        TLegend* leg = new TLegend(0.62, 0.68, 0.88, 0.88);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);

        for (size_t i = 0; i < histograms.size(); ++i)
        {
            leg->AddEntry(histograms[i], legend_labels[index_found[i]].c_str(), "lep");
        }

        leg->Draw();

        TLatex latex;
        latex.SetNDC();
        latex.SetTextSize(0.028);
        latex.DrawLatex(0.19, 0.92, trackcuts);
        latex.DrawLatex(0.18, 0.85, fileinfo);

        c->Modified();
        c->Update();
        c->Print(output_name.c_str());
        
        delete zero_line;
        delete leg;

        for (TH1* h : histograms)
        {
            delete h;
        }
    }

    c->Print((output_name + "]").c_str());
    delete c;
    infile->Close();
    delete infile;
}
