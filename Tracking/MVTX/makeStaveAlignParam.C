#include <MicromegasDefs.h>
#include <trackbase/ActsGeometry.h>
#include <trackbase/InttDefs.h>
#include <trackbase/MvtxDefs.h>
#include <trackbase/TpcDefs.h>
#include <trackbase/TrkrDefs.h>

#include <g4mvtx/PHG4MvtxDefs.h>

#include <Geant4/G4Types.hh> // for G4double

#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <TText.h>

#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

bool verbose = false;
const int n_Layers = 3;
const int n_allstaves = 12 + 16 + 20;

std::map<std::pair<int, int>, std::tuple<double, double, double>> getIdealStavePosition() // key: (layer, stave), value: x, y, z
{
    std::map<std::pair<int, int>, std::tuple<double, double, double>> m_IdealStavePosition;
    for (int ilayer = 0; ilayer < n_Layers; ++ilayer)
    {
        int N_staves = PHG4MvtxDefs::mvtxdat[ilayer][PHG4MvtxDefs::kNStave];
        for (int iphi = 0; iphi < N_staves; iphi++)
        {
            std::pair<int, int> p_layerstave = std::make_pair(ilayer, iphi);
            G4double phistep = 2.0 * M_PI / N_staves;
            G4double phi0 = PHG4MvtxDefs::mvtxdat[ilayer][PHG4MvtxDefs::kPhi0];
            G4double phi_rotation = phi0 + (double)iphi * phistep;
            G4double layer_nominal_radius = PHG4MvtxDefs::mvtxdat[ilayer][PHG4MvtxDefs::kRmd];
            G4double x = layer_nominal_radius * cos(phi_rotation);
            G4double y = layer_nominal_radius * sin(phi_rotation);
            G4double z = 0.0;

            m_IdealStavePosition[p_layerstave] = std::make_tuple(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z));

            if (verbose)
                cout << "Ideal stave position for layer " << ilayer << " stave " << iphi << " x " << x << " y " << y << " z " << z << endl;
        }
    }

    return m_IdealStavePosition;
}

void dump2TextFile(std::map<std::pair<int, int>, std::tuple<double, double, double, double, double, double>> m, std::string filename)
{
    std::ofstream outfile(filename);
    if (!outfile.is_open())
    {
        std::cout << "\033[1;31mError: could not open file " << filename << "\033[0m" << std::endl;
        return;
    }
    else
    {
        for (const auto &layer_stave : m)
        {
            outfile << unsigned(layer_stave.first.first) << " " << unsigned(layer_stave.first.second) << " " << std::get<0>(layer_stave.second) << " " << std::get<1>(layer_stave.second) << " " << std::get<2>(layer_stave.second) << " " << std::get<3>(layer_stave.second) << " " << std::get<4>(layer_stave.second) << " " << std::get<5>(layer_stave.second) << std::endl;
        }
    }
    outfile.close();
}

void dump2CDBTree(std::map<std::pair<int, int>, std::tuple<double, double, double>> m, std::string filename)
{
    // TODO
}

std::tuple<float, float, float, float, float, float> Summary(std::map<std::pair<int, int>, std::tuple<double, double, double, double, double, double>> m_StaveShift)
{
    std::vector<float> v_dx, v_dy, v_dz;
    for (auto &layer_stave : m_StaveShift)
    {
        v_dx.push_back(std::get<3>(m_StaveShift[layer_stave.first]));
        v_dy.push_back(std::get<4>(m_StaveShift[layer_stave.first]));
        v_dz.push_back(std::get<5>(m_StaveShift[layer_stave.first]));
    }
    float avgx = 0, avgy = 0, avgz = 0, stdx = 0, stdy = 0, stdz = 0;
    double sumx = std::accumulate(v_dx.begin(), v_dx.end(), 0.0);
    double sumy = std::accumulate(v_dy.begin(), v_dy.end(), 0.0);
    double sumz = std::accumulate(v_dz.begin(), v_dz.end(), 0.0);
    avgx = sumx / v_dx.size();
    avgy = sumy / v_dy.size();
    avgz = sumz / v_dz.size();
    std::vector<double> diffx(v_dx.size()), diffy(v_dy.size()), diffz(v_dz.size());
    std::transform(v_dx.begin(), v_dx.end(), diffx.begin(), [avgx](double x) { return x - avgx; });
    std::transform(v_dy.begin(), v_dy.end(), diffy.begin(), [avgy](double y) { return y - avgy; });
    std::transform(v_dz.begin(), v_dz.end(), diffz.begin(), [avgz](double z) { return z - avgz; });
    double sq_sumx = std::inner_product(diffx.begin(), diffx.end(), diffx.begin(), 0.0);
    double sq_sumy = std::inner_product(diffy.begin(), diffy.end(), diffy.begin(), 0.0);
    double sq_sumz = std::inner_product(diffz.begin(), diffz.end(), diffz.begin(), 0.0);
    stdx = std::sqrt(sq_sumx / v_dx.size());
    stdy = std::sqrt(sq_sumy / v_dy.size());
    stdz = std::sqrt(sq_sumz / v_dz.size());

    cout << "Average shift in x: " << avgx << " +/- " << stdx << " mm" << endl;
    cout << "Average shift in y: " << avgy << " +/- " << stdy << " mm" << endl;
    cout << "Average shift in z: " << avgz << " +/- " << stdz << " mm" << endl;

    return std::make_tuple(avgx, avgy, avgz, stdx, stdy, stdz);
}

void drawHist(TH1F *h, float avg, TString yaxistitle, TString filename)
{
    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 500);
    c1->cd();
    h->SetFillColor(kBlue - 9);
    h->SetLineColor(kBlue - 2);
    h->SetLineWidth(1);
    h->GetYaxis()->SetTitle(yaxistitle);
    h->LabelsOption("v");
    h->Draw("hist");
    TLine *avg_x = new TLine(0, avg, n_allstaves, avg);
    avg_x->SetLineColor(kRed);
    avg_x->SetLineStyle(2);
    avg_x->SetLineWidth(2);
    avg_x->Draw();
    TText *tavgx = new TText();
    tavgx->SetTextSize(0.03);
    tavgx->SetTextAlign(31);
    tavgx->SetTextColor(kRed);
    tavgx->DrawText(h->GetXaxis()->GetXmax(), avg + 0.02, "Average");
    c1->SaveAs(filename + ".png");
    c1->SaveAs(filename + ".pdf");
}

void drawStaveShiftHist(std::map<std::pair<int, int>, std::tuple<double, double, double, double, double, double>> m_StaveShift)
{
    // histograms for visualization
    TH1F *hM_alpha = new TH1F("hM_alpha", "hM_alpha", n_allstaves, 0, n_allstaves);
    TH1F *hM_beta = new TH1F("hM_beta", "hM_beta", n_allstaves, 0, n_allstaves);
    TH1F *hM_gamma = new TH1F("hM_gamma", "hM_gamma", n_allstaves, 0, n_allstaves);
    TH1F *hM_x = new TH1F("hM_x", "hM_x", n_allstaves, 0, n_allstaves);
    TH1F *hM_y = new TH1F("hM_y", "hM_y", n_allstaves, 0, n_allstaves);
    TH1F *hM_z = new TH1F("hM_z", "hM_z", n_allstaves, 0, n_allstaves);

    int idx = 0;
    for (auto &layer_stave : m_StaveShift)
    {
        hM_alpha->SetBinContent(idx + 1, std::get<0>(layer_stave.second));
        hM_alpha->GetXaxis()->SetBinLabel(idx + 1, Form("L%d_%d", unsigned(layer_stave.first.first), unsigned(layer_stave.first.second)));
        hM_beta->SetBinContent(idx + 1, std::get<1>(layer_stave.second));
        hM_beta->GetXaxis()->SetBinLabel(idx + 1, Form("L%d_%d", unsigned(layer_stave.first.first), unsigned(layer_stave.first.second)));
        hM_gamma->SetBinContent(idx + 1, std::get<2>(layer_stave.second));
        hM_gamma->GetXaxis()->SetBinLabel(idx + 1, Form("L%d_%d", unsigned(layer_stave.first.first), unsigned(layer_stave.first.second)));
        hM_x->SetBinContent(idx + 1, std::get<3>(layer_stave.second));
        hM_x->GetXaxis()->SetBinLabel(idx + 1, Form("L%d_%d", unsigned(layer_stave.first.first), unsigned(layer_stave.first.second)));
        hM_y->SetBinContent(idx + 1, std::get<4>(layer_stave.second));
        hM_y->GetXaxis()->SetBinLabel(idx + 1, Form("L%d_%d", unsigned(layer_stave.first.first), unsigned(layer_stave.first.second)));
        hM_z->SetBinContent(idx + 1, std::get<5>(layer_stave.second));
        hM_z->GetXaxis()->SetBinLabel(idx + 1, Form("L%d_%d", unsigned(layer_stave.first.first), unsigned(layer_stave.first.second)));
        idx++;
    }

    std::tuple<float, float, float, float, float, float> summary = Summary(m_StaveShift); // avgx, avgy, avgz, stdx, stdy, stdz

    drawHist(hM_x, std::get<0>(summary), "Per-stave shift in x [mm]", "staveAlignmentParameters_x");
    drawHist(hM_y, std::get<1>(summary), "Per-stave shift in y [mm]", "staveAlignmentParameters_y");
    drawHist(hM_z, std::get<2>(summary), "Per-stave shift in z [mm]", "staveAlignmentParameters_z");
}

void drawStavePosition(std::map<std::pair<int, int>, std::tuple<double, double, double, double, double, double>> m_StaveShift)
{
    std::vector<std::vector<double>> v_x_ideal, v_y_ideal, v_x_missaligned, v_y_missaligned;
    for (int i = 0; i < n_Layers; ++i)
    {
        v_x_ideal.push_back(std::vector<double>());
        v_y_ideal.push_back(std::vector<double>());
        v_x_missaligned.push_back(std::vector<double>());
        v_y_missaligned.push_back(std::vector<double>());
    }

    std::map<std::pair<int, int>, std::tuple<double, double, double>> map_IdealStavePos = getIdealStavePosition();
    for (const auto &layer_stave : m_StaveShift)
    {
        int layer = layer_stave.first.first;
        int stave = layer_stave.first.second;
        double dx = std::get<3>(layer_stave.second) / 10.0; // cm to mm
        double dy = std::get<4>(layer_stave.second) / 10.0; // cm to mm
        double dz = std::get<5>(layer_stave.second) / 10.0; // cm to mm

        double x = std::get<0>(map_IdealStavePos[std::make_pair(layer, stave)]) / 10.0; // cm to mm
        double y = std::get<1>(map_IdealStavePos[std::make_pair(layer, stave)]) / 10.0; // cm to mm
        double z = std::get<2>(map_IdealStavePos[std::make_pair(layer, stave)]) / 10.0; // cm to mm

        double x_new = x + dx;
        double y_new = y + dy;
        double z_new = z + dz;

        v_x_ideal[layer].push_back(x);
        v_y_ideal[layer].push_back(y);
        v_x_missaligned[layer].push_back(x_new);
        v_y_missaligned[layer].push_back(y_new);

        if (verbose)
            std::cout << "Ideal stave position for layer " << layer << " stave " << stave << " x " << x << " y " << y << " z " << z << std::endl;
        if (verbose)
            std::cout << "Misaligned stave position for layer " << layer << " stave " << stave << " x " << x_new << " y " << y_new << " z " << z_new << std::endl;
    }

    for (int ilayer = 0; ilayer < n_Layers; ++ilayer)
    {
        v_x_ideal[ilayer].push_back(v_x_ideal[ilayer][0]);
        v_y_ideal[ilayer].push_back(v_y_ideal[ilayer][0]);
        v_x_missaligned[ilayer].push_back(v_x_missaligned[ilayer][0]);
        v_y_missaligned[ilayer].push_back(v_y_missaligned[ilayer][0]);
    }

    TGraph *g_ideal[n_Layers], *g_misaligned[n_Layers];
    for (int ilayer = 0; ilayer < n_Layers; ++ilayer)
    {
        g_ideal[ilayer] = new TGraph(v_x_ideal[ilayer].size(), &v_x_ideal[ilayer][0], &v_y_ideal[ilayer][0]);
        g_misaligned[ilayer] = new TGraph(v_x_missaligned[ilayer].size(), &v_x_missaligned[ilayer][0], &v_y_missaligned[ilayer][0]);
    }

    TCanvas *c = new TCanvas("c", "c", 800, 800);
    c->SetGrid();

    for (int i = 0; i < n_Layers; ++i)
    {
        g_ideal[2 - i]->SetMarkerColor(kRed + (2 - i));
        g_ideal[2 - i]->SetMarkerStyle(20 + i);
        g_ideal[2 - i]->SetLineColor(kRed + (2 - i));
        g_ideal[2 - i]->SetMarkerSize(1);
        if (i == 0)
        {
            g_ideal[2 - i]->Draw("ALP");
        }
        else
        {
            g_ideal[2 - i]->Draw("LP");
        }
        g_misaligned[2 - i]->SetMarkerColor(kBlue + (2 - i));
        g_misaligned[2 - i]->SetMarkerStyle(20 + i);
        g_misaligned[2 - i]->SetLineColor(kBlue + (2 - i));
        g_misaligned[2 - i]->SetMarkerSize(1);
        g_misaligned[2 - i]->Draw("LP");
    }

    TLegend *leg = new TLegend(0.18, 0.18, 0.3, 0.27);
    leg->SetTextSize(0.035);
    leg->AddEntry(g_ideal[0], "Ideal Geometry", "p");
    leg->AddEntry(g_misaligned[0], "Shifted Geometry", "p");
    leg->SetFillStyle(0);
    leg->Draw();

    g_ideal[2]->GetXaxis()->SetTitle("X [cm]");
    g_ideal[2]->GetYaxis()->SetTitle("Y [cm]");
    g_ideal[2]->GetXaxis()->SetLimits(-5.5, 5.5);
    g_ideal[2]->GetHistogram()->SetMaximum(5.5);
    g_ideal[2]->GetHistogram()->SetMinimum(-5.5);

    c->SaveAs("center.png");
    c->SaveAs("center.pdf");
}

void makeStaveAlignParam(std::string mvtxAlignmentParamsFile = "/sphenix/user/frawley/production/macros/TrackingProduction/july29_repo_localAlignmentParamsFile.txt", //
                         std::string outputFilename = "./MvtxStaveAlignmentParameters_Run2024.txt"                                                                    //
)
{
    std::map<TrkrDefs::hitsetkey, std::tuple<double, double, double, double, double, double>> m_ModGeoParams;                 // alpha, beta, gamma, dx, dy, dz
    std::map<std::pair<int, int>, std::vector<std::tuple<double, double, double, double, double, double>>> m_ModGeoParamsMap; // key: (layer, stave), value: vector of tuples of alpha, beta, gamma, dx, dy, dz
    std::map<std::pair<int, int>, std::tuple<double, double, double, double, double, double>> m_ModGeoParamsMapAverage;       // key: (layer, stave), value: tuple of average alpha, beta, gamma, dx, dy, dz

    std::ifstream file(mvtxAlignmentParamsFile);
    if (!file.is_open())
    {
        std::cout << "\033[1;31mError: could not open file " << mvtxAlignmentParamsFile << "\033[0m" << std::endl;
        return;
    }
    else
    {
        std::string line;
        while (std::getline(file, line))
        {
            // The first element of the line is the hitsetkey, the following six elemens are the alignment parameters alpha, beta, gamma, dx, dy, dz
            std::istringstream iss(line);
            TrkrDefs::hitsetkey hitsetkey;
            double alpha, beta, gamma, dx, dy, dz;
            if (!(iss >> hitsetkey >> alpha >> beta >> gamma >> dx >> dy >> dz))
            {
                std::cout << "\033[1;31mERROR - Could not read line " << line << "\033[0m" << std::endl;
                continue;
            }
            else
            {
                uint8_t layer = TrkrDefs::getLayer(hitsetkey);
                uint8_t stave = MvtxDefs::getStaveId(hitsetkey);
                uint8_t chip = MvtxDefs::getChipId(hitsetkey);
                std::pair layer_stave = std::make_pair(layer, stave);
                if (layer > 2)
                    continue;

                m_ModGeoParamsMap[layer_stave].push_back(std::make_tuple(alpha, beta, gamma, dx, dy, dz));

                if (m_ModGeoParamsMapAverage.find(layer_stave) == m_ModGeoParamsMapAverage.end())
                {
                    m_ModGeoParamsMapAverage[layer_stave] = std::make_tuple(alpha, beta, gamma, dx, dy, dz);
                }
                else
                {
                    std::get<0>(m_ModGeoParamsMapAverage[layer_stave]) += alpha;
                    std::get<1>(m_ModGeoParamsMapAverage[layer_stave]) += beta;
                    std::get<2>(m_ModGeoParamsMapAverage[layer_stave]) += gamma;
                    std::get<3>(m_ModGeoParamsMapAverage[layer_stave]) += dx;
                    std::get<4>(m_ModGeoParamsMapAverage[layer_stave]) += dy;
                    std::get<5>(m_ModGeoParamsMapAverage[layer_stave]) += dz;
                }

                if (verbose)
                    std::cout << "Read hitsetkey " << hitsetkey << " layer " << unsigned(layer) << " stave " << unsigned(stave) << " chip " << unsigned(chip) << " alpha " << alpha << " beta " << beta << " gamma " << gamma << " dx " << dx << " dy " << dy << " dz " << dz << std::endl;
            }
            m_ModGeoParams[hitsetkey] = std::make_tuple(alpha, beta, gamma, dx, dy, dz);
        }
    }

    for (auto &layer_stave : m_ModGeoParamsMapAverage)
    {
        std::get<0>(layer_stave.second) /= m_ModGeoParamsMap[layer_stave.first].size();
        std::get<1>(layer_stave.second) /= m_ModGeoParamsMap[layer_stave.first].size();
        std::get<2>(layer_stave.second) /= m_ModGeoParamsMap[layer_stave.first].size();
        std::get<3>(layer_stave.second) /= m_ModGeoParamsMap[layer_stave.first].size();
        std::get<4>(layer_stave.second) /= m_ModGeoParamsMap[layer_stave.first].size();
        std::get<5>(layer_stave.second) /= m_ModGeoParamsMap[layer_stave.first].size();
    }

    if (m_ModGeoParamsMap.size() != static_cast<size_t>(n_allstaves) || m_ModGeoParamsMapAverage.size() != static_cast<size_t>(n_allstaves))
    {
        std::cout << "\033[1;31mERROR - The number of staves in the maps m_ModGeoParamsMap and m_ModGeoParamsMapAverage is not equal to " << n_allstaves << ". Exiting.\033[0m" << std::endl;
        exit(1);
    }

    dump2TextFile(m_ModGeoParamsMapAverage, outputFilename);

    drawStaveShiftHist(m_ModGeoParamsMapAverage);

    drawStavePosition(m_ModGeoParamsMapAverage);
}