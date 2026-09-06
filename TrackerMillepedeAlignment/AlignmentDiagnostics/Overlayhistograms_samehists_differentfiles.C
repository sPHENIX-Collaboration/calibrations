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
#include "TGraphErrors.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"

struct HistInfo
{
  std::string name;
  std::string title;
  bool normalize;
  bool logy;
  bool use_y_range;
  double ymin;
  double ymax;
  std::string trackcuts;
  bool draw_as_lines;  // false: points with errors; true: connected lines
  bool use_x_range;
  double xmin;
  double xmax;

  HistInfo(
      const std::string& input_name,
      const std::string& input_title,
      const bool input_normalize,
      const bool input_logy,
      const bool input_use_y_range,
      const double input_ymin,
      const double input_ymax,
      const std::string& input_trackcuts,
      const bool input_draw_as_lines,
      const bool input_use_x_range = false,
      const double input_xmin = 0.0,
      const double input_xmax = 0.0)
      : name(input_name)
      , title(input_title)
      , normalize(input_normalize)
      , logy(input_logy)
      , use_y_range(input_use_y_range)
      , ymin(input_ymin)
      , ymax(input_ymax)
      , trackcuts(input_trackcuts)
      , draw_as_lines(input_draw_as_lines)
      , use_x_range(input_use_x_range)
      , xmin(input_xmin)
      , xmax(input_xmax)
  {
  }
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


TGraphErrors* make_shifted_graph(
    const TH1* hist,
    const double shift_fraction,
    const std::string& graph_name)
{
  if (!hist)
  {
    return nullptr;
  }

  const int nbins = hist->GetNbinsX();
  TGraphErrors* graph = new TGraphErrors(nbins);
  graph->SetName(graph_name.c_str());
  graph->SetTitle(hist->GetTitle());

  for (int bin = 1; bin <= nbins; ++bin)
  {
    const double bin_width = hist->GetXaxis()->GetBinWidth(bin);
    const double x = hist->GetXaxis()->GetBinCenter(bin)
                   + shift_fraction * bin_width;
    const double y = hist->GetBinContent(bin);
    //const double ex = 0.5 * bin_width;
    const double ex = 0;
    const double ey = hist->GetBinError(bin);

    graph->SetPoint(bin - 1, x, y);
    graph->SetPointError(bin - 1, ex, ey);
  }

  // Keep the line color synchronized with the marker color so switching
  // between point and line drawing does not change the dataset color.
  graph->SetLineColor(hist->GetMarkerColor());
  graph->SetMarkerColor(hist->GetMarkerColor());
  graph->SetMarkerStyle(hist->GetMarkerStyle());
  graph->SetMarkerSize(hist->GetMarkerSize());
  graph->SetLineStyle(hist->GetLineStyle());
  graph->SetLineWidth(hist->GetLineWidth());

  return graph;
}

bool normalize_histogram(TH1* hist)
{
  if (!hist)
  {
    return false;
  }

  if (hist->GetSumw2N() == 0)
  {
    hist->Sumw2();
  }

  const int first_bin = 1;
  const int last_bin = hist->GetNbinsX();

  // Total number of entries, excluding underflow and overflow.
  const double total_entries =
      hist->Integral(first_bin, last_bin);

  if (!std::isfinite(total_entries) || total_entries <= 0.0)
  {
    return false;
  }

  // Divide by total entries and by each bin width.
  hist->Scale(1.0 / total_entries, "width");

  // Verify that the total histogram area is one.
  const double area_after =
      hist->Integral(first_bin, last_bin, "width");

  std::cout << hist->GetName()
            << ": normalized area = "
            << area_after << std::endl;

  return std::isfinite(area_after) &&
         std::fabs(area_after - 1.0) < 1.0e-10;
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

bool apply_configured_x_range(const HistInfo& histinfo, TH1* frame)
{
  if (!frame || !histinfo.use_x_range)
  {
    return false;
  }

  if (!std::isfinite(histinfo.xmin) || !std::isfinite(histinfo.xmax) ||
      histinfo.xmax <= histinfo.xmin)
  {
    return false;
  }

  frame->GetXaxis()->SetRangeUser(histinfo.xmin, histinfo.xmax);
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
  

  std::string resolved_input_dir =
      (input_dir && input_dir[0]) ? std::string(input_dir) : ".";
  if (resolved_input_dir.size() > 1 &&
      (resolved_input_dir.back() == '/' || resolved_input_dir.back() == '\\'))
  {
    resolved_input_dir.pop_back();
  }

  //const char* common_text = "12/14 run3pp allSiSeeds ACTS 79516";
  const char* common_text = "12/14 run3pp ACTS 79516";
  const char* default_trackcuts = "Cuts: vertex_ntracks_cut>3, m_nmaps>=3 && m_nintt>=2 && m_ntpc>=0 && pt>=0.8 GeV/c";
  const char* trackcuts_fordcaxyvsetaphi = "Cuts: vertex_ntracks_cut>3, m_nmaps>=3 && m_nintt>=2 && m_ntpc>=0 && pt>=0.8 GeV/c";
  
  //const char* trackcuts = "Cuts: vertex_ntracks_cut>3, m_nmaps>=3 && m_nintt>=2 && m_ntpc>=0 && pt>=0.8 GeV/c";

  const std::vector<std::string> filenames = {
      resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_data_TPCMatchedSiSeeds_ACTS_baselinealignment_79516_ptgthan0p8.root",
      resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_data_TPCMatchedSiSeeds_ACTS_iter91_79516_ptgthan0p8.root",
      resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_data_millepederesiduals_iteration139_79516.root",
      resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_Simulation_ptgthan0p8.root"
      //resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_data_cluster_seeds_si_79516.root",
      //resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_data_ACTS_79516.root",
      //resolved_input_dir + "/output_PlottingMacro_singleTrackQuantities_Simulation.root"
    };

  const std::vector<std::string> fileinfos = {
       "Baseline CDB Alignment",
       "TPC Matched Si seeds Iter 91",
       "Millepede calculated residuals Iter 139",
       //"fulltracks"
      //"Data TPC matched Si tracks",
      //"Data only Si information",
      "Simulation"
    };

  const std::string output_pdf =
      "Output_overlayhistograms_samehists_differentfiles.pdf";

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
  // The configured title is drawn explicitly with TLatex below because the
  // histogram is used only as an axis frame via Draw("AXIS").
  gStyle->SetOptTitle(0);

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
  canvas->SetTopMargin(0.16);
  canvas->SetBottomMargin(0.12);

  const std::vector<HistInfo> histinfos = {
      // Last field: draw_as_lines (false = points with errors, true = lines).
      {"Gaussian_Mean_of_dcaxy_vs_ptreco_allpid_passcuts","<dcaxy> (Gaussian Fit) vs p_{T,reco}",  false, false, true, -0.008, 0.004,default_trackcuts, false},
      {"Gaussian_Mean_of_dcaxy_vs_preco_allpid_passcuts","<dcaxy> (Gaussian Fit) vs p_{reco}",   false, false, true, -0.008, 0.004,default_trackcuts, false},
      {"Gaussian_Mean_of_dcaxy_vs_etareco_allpid_passcuts","<dcaxy> (Gaussian Fit) vs #eta_{reco}", false, false, true, -0.015, 0.01,trackcuts_fordcaxyvsetaphi, false},
      {"Gaussian_Mean_of_dcaxy_vs_phireco_allpid_passcuts","<dcaxy> (Gaussian Fit) vs #phi_{T,reco}", false, false, true, -0.015, 0.015,trackcuts_fordcaxyvsetaphi, false},
      {"Gaussian_Width_of_dcaxy_vs_ptreco_allpid_passcuts","#sigma(dcaxy) (Gaussian Fit) vs p_{T,reco}",  false, false, true, 0.0, 0.025,default_trackcuts, false},
      {"Gaussian_Width_of_dcaxy_vs_preco_allpid_passcuts","#sigma(dcaxy) (Gaussian Fit) vs p_{reco}",   false, false, true, 0.0, 0.025,default_trackcuts, false},
      {"Gaussian_Width_of_dcaxy_vs_etareco_allpid_passcuts","#sigma(dcaxy) (Gaussian Fit) vs #eta_{T,reco}", false, false, true, 0.0, 0.04,trackcuts_fordcaxyvsetaphi, false},
      {"Gaussian_Width_of_dcaxy_vs_phireco_allpid_passcuts","#sigma(dcaxy) (Gaussian Fit) vs #phi_{T,reco}", false, false, true, 0.0, 0.04,trackcuts_fordcaxyvsetaphi, false},

      {"Gaussian_Mean_of_dcaz_vs_ptreco_allpid_passcuts","<dcaz> (Gaussian Fit) vs p_{T,reco}",  false, false, true, -0.0005, 0.0015,default_trackcuts, false},
      {"Gaussian_Mean_of_dcaz_vs_preco_allpid_passcuts","<dcaz> (Gaussian Fit) vs p_{reco}",   false, false, true, -0.0005, 0.0015,default_trackcuts, false},
      {"Gaussian_Mean_of_dcaz_vs_etareco_allpid_passcuts","<dcaz> (Gaussian Fit) vs #eta_{reco}", false, false, true, -0.015, 0.015,trackcuts_fordcaxyvsetaphi, false},
      {"Gaussian_Mean_of_dcaz_vs_phireco_allpid_passcuts","<dcaz> (Gaussian Fit) vs #phi_{reco}", false, false, true, -0.005, 0.008,trackcuts_fordcaxyvsetaphi, false},
      {"Gaussian_Width_of_dcaz_vs_ptreco_allpid_passcuts","#sigma(dcaz) (Gaussian Fit) vs p_{T,reco}",  false, false, true, 0.0, 0.018,default_trackcuts, false},
      {"Gaussian_Width_of_dcaz_vs_preco_allpid_passcuts","#sigma(dcaz) (Gaussian Fit) vs p_{reco}",   false, false, true, 0.0, 0.018,default_trackcuts, false},
      {"Gaussian_Width_of_dcaz_vs_etareco_allpid_passcuts","#sigma(dcaz) (Gaussian Fit) vs #eta_{reco}", false, false, true, 0.0, 0.025,trackcuts_fordcaxyvsetaphi, false},
      {"Gaussian_Width_of_dcaz_vs_phireco_allpid_passcuts","#sigma(dcaz) (Gaussian Fit) vs #phi_{reco}", false, false, true, 0.0, 0.025,trackcuts_fordcaxyvsetaphi, false},
      // Example for raw distributions:
       {"hEta_allpid_passcuts", "#eta distribution",false,  false,  false, 0.00000001, 10,default_trackcuts, false},
       {"hPt_allpid_passcuts", "p_{T} distribution", false,  false,  false, 0.0, 0.0,default_trackcuts, false},
       {"hp_allpid_passcuts", "p distribution", false,  false,  false, 0.0, 0.0,default_trackcuts, false},
       {"hPhi_allpid_passcuts", "#phi distribution", false,  false,  true, 0.0, 14e3,default_trackcuts, false},
      // {"h_nmaps",                  false, false, true,  0.0, 5000.0},

      {"htrackresidualx_MVTX_layer0","htrackresidualx_MVTX_layer0", true, false, false, 0, 10e7,default_trackcuts, true, true, -0.005,0.005},
      {"htrackresidualx_MVTX_layer1","htrackresidualx_MVTX_layer1", true, false, false, 0, 10e7,default_trackcuts, true, true, -0.005,0.005},
      {"htrackresidualx_MVTX_layer2","htrackresidualx_MVTX_layer2", true, false, false, 0, 10e7,default_trackcuts, true, true, -0.005,0.005},
      {"htrackresidualx_INTT_layers34_combined","htrackresidualx_INTT_layers34_combined", true, false, false, 0, 10e7,default_trackcuts, true, true, -0.05,0.05},
      {"htrackresidualx_INTT_layers56_combined","htrackresidualx_INTT_layers56_combined", true, false, false, 0, 10e7,default_trackcuts, true, true, -0.05,0.05},
      
      {"htrackresidualz_MVTX_layer0","htrackresidualz_MVTX_layer0", true, false, false, 0, 10e7,default_trackcuts, true, true, -0.005,0.005},
      {"htrackresidualz_MVTX_layer1","htrackresidualz_MVTX_layer1", true, false, false, 0, 10e7,default_trackcuts, true, true, -0.005,0.005},
      {"htrackresidualz_MVTX_layer2","htrackresidualz_MVTX_layer2", true, false, false, 0, 10e7,default_trackcuts, true, true, -0.005,0.005},
      {"htrackresidualz_INTT_layers34_combined","htrackresidualz_INTT_layers34_combined", true, false, false, 0, 10e7,default_trackcuts, true, true, -1.5, 1.5},
      {"htrackresidualz_INTT_layers56_combined","htrackresidualz_INTT_layers56_combined", true, false, false, 0, 10e7,default_trackcuts, true, true, -1.5, 1.5}
  };

  if (histinfos.empty())
  {
    std::cout << "Error: histinfos is empty." << std::endl;
    return;
  }

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

    std::vector<TGraphErrors*> graphs;
    graphs.reserve(histograms.size());

    // Adjacent datasets are shifted symmetrically around each bin center.
    // The shift is expressed as a fraction of the local bin width.
    const double shift_step = 0.01;
    const double graph_center =
        0.5 * static_cast<double>(histograms.size() - 1);

    for (std::size_t i = 0; i < histograms.size(); ++i)
    {
      const double shift_fraction = histinfo.draw_as_lines
          ? 0.0
          : (static_cast<double>(i) - graph_center) * shift_step;

      TGraphErrors* graph = make_shifted_graph(
          histograms[i],
          shift_fraction,
          make_safe_object_name(histinfo.name)
              + "_shifted_graph_" + std::to_string(i));

      if (graph)
      {
        graphs.push_back(graph);
      }
    }

    if (graphs.size() != histograms.size())
    {
      std::cout << "Warning: could not construct all shifted graphs for "
                << histinfo.name << ". Skipping this page." << std::endl;
      for (TGraphErrors* graph : graphs)
      {
        delete graph;
      }
      for (TH1* hist : histograms)
      {
        delete hist;
      }
      continue;
    }

    canvas->Clear();
    canvas->cd();
    canvas->SetLogy(histinfo.logy);
    //histograms.front()->SetTitle(histinfo.name.c_str());
    histograms.front()->SetTitle(histinfo.title.c_str());
    
    if (histinfo.normalize)
    {
      histograms.front()->GetYaxis()->SetTitle("Normalized entries");
    }

    if (histinfo.use_x_range &&
        !apply_configured_x_range(histinfo, histograms.front()))
    {
      std::cout << "Warning: invalid configured x-range for "
                << histinfo.name << ". Skipping this page." << std::endl;
      for (TGraphErrors* graph : graphs)
      {
        delete graph;
      }
      for (TH1* hist : histograms)
      {
        delete hist;
      }
      continue;
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
      for (TGraphErrors* graph : graphs)
      {
        delete graph;
      }
      for (TH1* hist : histograms)
      {
        delete hist;
      }
      continue;
    }

    // Draw only the histogram axes/frame. The data points themselves are
    // drawn from shifted TGraphErrors objects below.
    histograms.front()->Draw("AXIS");
    for (TGraphErrors* graph : graphs)
    {
      graph->Draw(histinfo.draw_as_lines ? "L SAME" : "P E1 SAME");
    }

    const double legend_y2 = 0.8;
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
    for (std::size_t i = 0; i < graphs.size(); ++i)
    {
      legend.AddEntry(graphs[i], labels[i].c_str(),
                      histinfo.draw_as_lines ? "l" : "lep");
    }
    legend.Draw();

    TLatex latex;
    latex.SetNDC();

    // Draw the title explicitly so it is visible when the histogram itself is
    // drawn only as an axis frame.
    latex.SetTextAlign(23);
    latex.SetTextSize(0.038);
    latex.DrawLatex(0.50, 0.95, histinfo.title.c_str());

    latex.SetTextAlign(13);
    latex.SetTextSize(0.028);

    //latex.DrawLatex(0.13, 0.87, trackcuts);
    if (!histinfo.trackcuts.empty())
    {
        latex.DrawLatex(0.13, 0.87, histinfo.trackcuts.c_str());
    }
    latex.DrawLatex(0.17, 0.78, common_text);
    
    latex.DrawLatex(0.14, 0.7, histinfo.normalize ? "Unit-area normalized" : "");
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

    for (TGraphErrors* graph : graphs)
    {
      delete graph;
    }

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
