#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"

struct HistInfo
{
  std::string name;
  bool normalize;
  bool logy;
  bool use_y_range;
  double ymin;
  double ymax;
};

std::string make_safe_object_name(const std::string& input)
{
  std::string safe_name;
  safe_name.reserve(input.size());

  for (const char ch : input)
  {
    safe_name.push_back(std::isalnum(static_cast<unsigned char>(ch)) ? ch : '_');
  }

  if (safe_name.empty())
  {
    safe_name = "hist";
  }

  return safe_name;
}

TH1* clone_histogram_from_file(
    TFile* input,
    const std::string& hist_name,
    const std::string& clone_name)
{
  if (!input)
  {
    throw std::runtime_error("Invalid input file handle while loading " + hist_name);
  }

  TObject* obj = input->Get(hist_name.c_str());
  if (!obj)
  {
    throw std::runtime_error(
        "Could not find histogram " + hist_name + " in " + input->GetName());
  }

  TH1* hist = dynamic_cast<TH1*>(obj);
  if (!hist)
  {
    throw std::runtime_error(
        "Object " + hist_name + " in " + input->GetName() + " is not a TH1");
  }

  if (hist->GetDimension() != 1)
  {
    throw std::runtime_error(
        "Histogram " + hist_name + " in " + input->GetName() + " is not 1D");
  }

  TH1* clone = dynamic_cast<TH1*>(hist->Clone(clone_name.c_str()));
  if (!clone)
  {
    throw std::runtime_error(
        "Could not clone histogram " + hist_name + " from " + input->GetName());
  }

  clone->SetDirectory(nullptr);
  clone->SetStats(0);
  return clone;
}

bool normalize_histogram(TH1* hist)
{
  if (!hist)
  {
    return false;
  }

  const double integral = hist->Integral();
  if (integral <= 0.0)
  {
    return false;
  }

  if (hist->GetSumw2N() == 0)
  {
    hist->Sumw2();
  }

  hist->Scale(1.0 / integral);
  return true;
}

bool apply_configured_y_range(const HistInfo& histinfo, TH1* frame)
{
  if (!frame || !histinfo.use_y_range)
  {
    return false;
  }

  double ymin = histinfo.ymin;
  double ymax = histinfo.ymax;

  if (histinfo.logy)
  {
    if (ymax <= 0.0)
    {
      return false;
    }

    ymin = std::max(ymin, 1e-12);
    if (ymax <= ymin)
    {
      return false;
    }
  }
  else if (ymax <= ymin)
  {
    return false;
  }

  frame->SetMinimum(ymin);
  frame->SetMaximum(ymax);
  return true;
}

bool set_overlay_y_range(const std::vector<TH1*>& histograms, const bool logy)
{
  if (histograms.empty())
  {
    return false;
  }

  double ymin = std::numeric_limits<double>::infinity();
  double ymax = -std::numeric_limits<double>::infinity();
  double min_positive = std::numeric_limits<double>::infinity();

  for (const TH1* hist : histograms)
  {
    if (!hist)
    {
      continue;
    }

    for (int bin = 1; bin <= hist->GetNbinsX(); ++bin)
    {
      const double content = hist->GetBinContent(bin);
      ymin = std::min(ymin, content);
      ymax = std::max(ymax, content);

      if (content > 0.0)
      {
        min_positive = std::min(min_positive, content);
      }
    }
  }

  if (!std::isfinite(ymin) || !std::isfinite(ymax))
  {
    return false;
  }

  TH1* first = histograms.front();
  if (!first)
  {
    return false;
  }

  if (logy)
  {
    if (!std::isfinite(min_positive))
    {
      return false;
    }

    first->SetMinimum(std::max(min_positive * 0.5, 1e-12));
    first->SetMaximum((ymax > 0.0) ? 10.0 * ymax : 1.0);
    return true;
  }

  if (ymax <= ymin)
  {
    if (ymax == 0.0)
    {
      ymin = 0.0;
      ymax = 1.0;
    }
    else
    {
      const double pad = 0.2 * std::fabs(ymax);
      ymin -= pad;
      ymax += pad;
    }
  }
  else
  {
    const double pad = 0.15 * (ymax - ymin);
    ymin = (ymin >= 0.0) ? 0.0 : ymin - pad;
    ymax += pad;
  }

  first->SetMinimum(ymin);
  first->SetMaximum(ymax);
  return true;
}

void Overlayhistograms_samehists_differentfiles(const char* input_dir = ".")
{
  const std::vector<HistInfo> histinfos = {
      {"Gaussian_Mean_of_dcaxy_vs_ptreco_allpid_passcuts",  false, false, true, -0.004, 0.004},
      {"Gaussian_Mean_of_dcaxy_vs_preco_allpid_passcuts",   false, false, true, -0.004, 0.004},
      {"Gaussian_Mean_of_dcaxy_vs_etareco_allpid_passcuts", false, false, true, -0.015, 0.015},
      {"Gaussian_Mean_of_dcaxy_vs_phireco_allpid_passcuts", false, false, true, -0.005, 0.005},
      {"Gaussian_Width_of_dcaxy_vs_ptreco_allpid_passcuts",  false, false, true, 0.0, 0.02},
      {"Gaussian_Width_of_dcaxy_vs_preco_allpid_passcuts",   false, false, true, 0.0, 0.02},
      {"Gaussian_Width_of_dcaxy_vs_etareco_allpid_passcuts", false, false, true, 0.0, 0.02},
      {"Gaussian_Width_of_dcaxy_vs_phireco_allpid_passcuts", false, false, true, 0.0, 0.02},
      {"Gaussian_Mean_of_dcaz_vs_ptreco_allpid_passcuts",  false, false, true, -0.004, 0.004},
      {"Gaussian_Mean_of_dcaz_vs_preco_allpid_passcuts",   false, false, true, -0.004, 0.004},
      {"Gaussian_Mean_of_dcaz_vs_etareco_allpid_passcuts", false, false, true, -0.015, 0.015},
      {"Gaussian_Mean_of_dcaz_vs_phireco_allpid_passcuts", false, false, true, -0.005, 0.005},
      {"Gaussian_Width_of_dcaz_vs_ptreco_allpid_passcuts",  false, false, true, 0.0, 0.02},
      {"Gaussian_Width_of_dcaz_vs_preco_allpid_passcuts",   false, false, true, 0.0, 0.02},
      {"Gaussian_Width_of_dcaz_vs_etareco_allpid_passcuts", false, false, true, 0.0, 0.02},
      {"Gaussian_Width_of_dcaz_vs_phireco_allpid_passcuts", false, false, true, 0.0, 0.02},
      // Example for raw distributions:
      // {"h_ptreco_allpid_passcuts", true,  true,  false, 0.0, 0.0},
      // {"h_nmaps",                  false, false, true,  0.0, 5000.0},
  };

  std::string resolved_input_dir =
      (input_dir && input_dir[0]) ? std::string(input_dir) : ".";
  if (resolved_input_dir.size() > 1 &&
      (resolved_input_dir.back() == '/' || resolved_input_dir.back() == '\\'))
  {
    resolved_input_dir.pop_back();
  }

  const std::vector<std::string> filenames = {
      resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_data_fulltracks.root",
      resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_data_sionlytracks.root",
      resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_Simulation_withchisqbyndfcut.root",
      resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_Simulation_withoutchisqbyndfcut.root"};

  const std::vector<std::string> fileinfos = {
      "Data fulltracks",
      "Data Si-only tracks",
      "Simulation with #chi^{2}/ndf < 10",
      "Simulation without #chi^{2}/ndf cut"};

  const std::string output_pdf =
      "Output_overlayhistograms_samehists_differentfiles.pdf";

  if (histinfos.empty())
  {
    std::cout << "Error: histinfos is empty." << std::endl;
    return;
  }

  if (filenames.empty())
  {
    std::cout << "Error: filenames is empty." << std::endl;
    return;
  }

  if (filenames.size() != fileinfos.size())
  {
    std::cout << "Error: filenames and fileinfos must have the same size." << std::endl;
    return;
  }

  gStyle->SetOptStat(0);

  std::vector<TFile*> input_files(filenames.size(), nullptr);
  for (std::size_t i = 0; i < filenames.size(); ++i)
  {
    input_files[i] = TFile::Open(filenames[i].c_str(), "READ");
    if (!input_files[i] || input_files[i]->IsZombie())
    {
      std::cout << "Warning: could not open file " << filenames[i]
                << ". It will be skipped." << std::endl;
      if (input_files[i])
      {
        input_files[i]->Close();
        delete input_files[i];
      }
      input_files[i] = nullptr;
    }
  }

  TCanvas* canvas = new TCanvas("Overlayhistograms_samehists_differentfiles_canvas",
                                "Overlayhistograms_samehists_differentfiles_canvas",
                                1200, 900);
  canvas->SetLeftMargin(0.13);
  canvas->SetRightMargin(0.05);
  canvas->SetTopMargin(0.11);
  canvas->SetBottomMargin(0.12);

  const std::vector<int> colors = {
      kRed + 1,
      kBlue + 1,
      kBlack,
      kGreen + 2,
      kMagenta + 1,
      kOrange + 7,
      kCyan + 2,
      kViolet + 1};

  const std::vector<int> marker_styles = {
      20, 21, 22, 23, 29, 33, 34, 47};

  const std::vector<int> line_styles = {
      1, 2, 3, 4, 5, 6, 7, 8};

  bool opened_pdf = false;
  std::size_t pages_written = 0;

  for (const HistInfo& histinfo : histinfos)
  {
    std::vector<TH1*> histograms;
    std::vector<std::string> labels;

    for (std::size_t file_index = 0; file_index < input_files.size(); ++file_index)
    {
      TFile* input = input_files[file_index];
      if (!input)
      {
        continue;
      }

      try
      {
        const std::string clone_name =
            make_safe_object_name(histinfo.name) + "_" + std::to_string(file_index);
        TH1* hist = clone_histogram_from_file(input, histinfo.name, clone_name);

        if (histinfo.normalize && !normalize_histogram(hist))
        {
          std::cout << "Warning: could not normalize histogram " << histinfo.name
                    << " from " << filenames[file_index]
                    << " because its integral is non-positive. Skipping it."
                    << std::endl;
          delete hist;
          continue;
        }

        hist->SetLineColor(colors[file_index % colors.size()]);
        hist->SetMarkerColor(colors[file_index % colors.size()]);
        hist->SetMarkerStyle(marker_styles[file_index % marker_styles.size()]);
        hist->SetLineStyle(line_styles[file_index % line_styles.size()]);
        hist->SetLineWidth(2);

        histograms.push_back(hist);
        labels.push_back(fileinfos[file_index]);
      }
      catch (const std::exception& e)
      {
        std::cout << "Warning: " << e.what() << std::endl;
        // No hist pointer to delete here; clone_histogram_from_file either
        // returns a valid pointer or throws before returning.
        continue;
      }
    }

    if (histograms.empty())
    {
      std::cout << "Warning: no valid histograms were found for " << histinfo.name
                << ". Skipping this page." << std::endl;
      continue;
    }

    canvas->Clear();
    canvas->cd();
    canvas->SetLogy(histinfo.logy);
    histograms.front()->SetTitle(histinfo.name.c_str());

    if (histinfo.normalize)
    {
      histograms.front()->GetYaxis()->SetTitle("Normalized entries");
    }

    const bool has_configured_range = histinfo.use_y_range;
    const bool y_range_ok = has_configured_range
        ? apply_configured_y_range(histinfo, histograms.front())
        : set_overlay_y_range(histograms, histinfo.logy);

    if (!y_range_ok)
    {
      std::cout << "Warning: could not determine a valid y-range for "
                << histinfo.name
                << (has_configured_range ? " using the configured limits."
                                         : " using automatic scaling.")
                << " Skipping this page." << std::endl;
      for (TH1* hist : histograms)
      {
        delete hist;
      }
      continue;
    }

    histograms.front()->Draw("E1");
    for (std::size_t i = 1; i < histograms.size(); ++i)
    {
      histograms[i]->Draw("E1 SAME");
    }

    const double legend_y2 = 0.88;
    const double entry_height = 0.055;
    const double legend_y1 = legend_y2 - entry_height * static_cast<double>(histograms.size());
    if (legend_y1 < 0.15)
    {
      std::cout << "Warning: legend for " << histinfo.name
                << " may overflow the plot area (" << histograms.size()
                << " entries). Consider reducing the number of overlaid files."
                << std::endl;
    }
    TLegend legend(0.58, std::max(0.15, legend_y1), 0.94, legend_y2);
    legend.SetBorderSize(0);
    legend.SetFillStyle(0);
    for (std::size_t i = 0; i < histograms.size(); ++i)
    {
      legend.AddEntry(histograms[i], labels[i].c_str(), "lep");
    }
    legend.Draw();

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.028);
    latex.DrawLatex(0.14, 0.92, histinfo.normalize ? "Unit-area normalized" : "");
    //latex.DrawLatex(0.14, 0.88, histinfo.logy ? "Logarithmic y-axis" : "Linear y-axis");

    canvas->Modified();
    canvas->Update();

    if (!opened_pdf)
    {
      canvas->Print((output_pdf + "[").c_str());
      opened_pdf = true;
    }
    canvas->Print(output_pdf.c_str());
    ++pages_written;

    for (TH1* hist : histograms)
    {
      delete hist;
    }
  }

  if (opened_pdf)
  {
    canvas->Print((output_pdf + "]").c_str());
    std::cout << "Wrote " << pages_written << " page(s) to "
              << output_pdf << std::endl;
  }
  else
  {
    std::cout << "Warning: no output pages were written to "
              << output_pdf << std::endl;
  }

  delete canvas;

  for (TFile* input : input_files)
  {
    if (!input)
    {
      continue;
    }
    input->Close();
    delete input;
  }
}
