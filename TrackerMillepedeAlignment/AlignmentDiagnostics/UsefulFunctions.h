#pragma once
#include <TH1.h>
#include <TH1D.h>
#include <TH2.h>
#include <TH2D.h>
#include <TProfile2D.h>
#include <TH3D.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TAxis.h>
#include <TArrayD.h>
#include <TFitResultPtr.h>
#include <TLatex.h>
#include <TCollection.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TKey.h>
#include <TLine.h>
#include <TPad.h>
#include <TEfficiency.h>
#include <TCanvas.h>
#include <TH3.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TMath.h>


#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <TString.h>

void CleanupHistogram(TH1* h, int nx, const std::vector<char>& hasSlice, double maxErr = 1e9, double minErr = 0.0, bool useContentRange = false, double ymin = -1e9, double ymax = 1e9){
    for (int i = 1; i <= h->GetNbinsX(); ++i) {
        const double y  = h->GetBinContent(i);
        const double ey = h->GetBinError(i);

        bool bad = false;

        // No uncertainty / non-finite / absurd uncertainty
        if (!(ey > minErr) || !std::isfinite(ey) || ey > maxErr) bad = true;

        // Optional content sanity checks
        if (useContentRange) {
            if (!std::isfinite(y) || y == 0 || y < ymin || y > ymax) bad = true;
        }

        if (bad) {
            h->SetBinContent(i, 0.0);
            h->SetBinError(i, 0.0);
        }
    }

    for (int ix = 1; ix <= nx; ++ix) {
        if (!hasSlice[ix]) {
            h->SetBinContent(ix, 0.0);
            h->SetBinError(ix, 0.0);
        }
    }
}

void GetFitSlicesY(TH2D* h, TF1* fittingfunc, TH1D*& hMean, TH1D*& hSigma, const char* title, const char* xlabel, const char*ylabel)
{
    hMean = nullptr;
    hSigma = nullptr;

    if (!h || !fittingfunc) return;

    int nx = h->GetNbinsX();
    int ny = h->GetNbinsY();

    std::vector<char> hasSlice(nx + 1, 0);
    for (int ix = 1; ix <= nx; ++ix) {
        for (int iy = 1; iy <= ny; ++iy) {
            if (h->GetBinContent(ix, iy) != 0.0) {
                hasSlice[ix] = 1;
                break;
            }
        }
    }
    
    TObjArray arr;
    h->FitSlicesY(fittingfunc, 1, nx, 0, "QNR", &arr);

    if (arr.At(1)) {
        hMean = (TH1D*)((TH1D*)arr.At(1))->Clone(Form("Gaussian_Mean_of_%s",title));
        hMean->SetDirectory(0);
        hMean->SetName(Form("Gaussian_Mean_of_%s", title));
        hMean->SetTitle(Form("Gaussian Mean of %s; %s; <%s>", title, xlabel,ylabel));
        hMean->SetStats(0);

        CleanupHistogram(hMean, nx, hasSlice);
        
    }

    if (arr.At(2)) {
        hSigma = (TH1D*)((TH1D*)arr.At(2))->Clone(Form("Gaussian_Width_of_%s",title));
        hSigma->SetDirectory(0);
        hSigma->SetName(Form("Gaussian_Width_of_%s", title));
        hSigma->SetTitle(Form("Gaussian width of %s; %s; #sigma (%s)", title, xlabel,ylabel));
        hSigma->SetStats(0);

        CleanupHistogram(hSigma, nx, hasSlice);
    }
}

void GetModeFWHMRMS_Y(TH2D* h2_in, TH1D*& h1_mode, TH1D*& h1_fwhm, TH1D*& h1_rms,
                      int n_bins_comb, const char* title,
                      const char* xlabel, const char* ylabel)
{
    h1_mode = nullptr;
    h1_fwhm = nullptr;
    h1_rms  = nullptr;

    // ── Input validation ──────────────────────────────────────────────────────
    // [minor] null guard was absent in the original
    if (!h2_in) {
        std::cerr << "[GetModeFWHMRMS_Y] ERROR: null TH2D pointer.\n";
        return;
    }
    if (n_bins_comb <= 0) {
        std::cerr << "[GetModeFWHMRMS_Y] ERROR: n_bins_comb must be >= 1.\n";
        return;
    }

    const int nBinsX = h2_in->GetNbinsX();
    TAxis*    xax    = h2_in->GetXaxis();

    if (nBinsX % n_bins_comb != 0) {
        std::cerr << "[GetModeFWHMRMS_Y] ERROR: n_bins_comb=" << n_bins_comb
                  << " does not divide nBinsX=" << nBinsX << " evenly. "
                  << "The last " << (nBinsX % n_bins_comb)
                  << " column(s) would be silently dropped.\n";
        return;
    }

    const int n_iter = nBinsX / n_bins_comb;

    std::vector<double> edges(n_iter + 1);
    for (int ii = 0; ii < n_iter; ++ii)
        edges[ii] = xax->GetBinLowEdge(ii * n_bins_comb + 1);
    edges[n_iter] = xax->GetBinUpEdge(n_iter * n_bins_comb);

    // ── Allocate output histograms ─────────────────────────────────────────────
    h1_mode = new TH1D(Form("h1_mode_%s", h2_in->GetName()),
                       Form("Mode_of_%s;%s;Mode(%s)", title, xlabel, ylabel),
                       n_iter, edges.data());
    h1_fwhm = new TH1D(Form("h1_fwhm_%s", h2_in->GetName()),
                       Form("FWHM_of_%s;%s;FWHM(%s)", title, xlabel, ylabel),
                       n_iter, edges.data());
    h1_rms  = new TH1D(Form("h1_rms_%s",  h2_in->GetName()),
                       Form("RMS_of_%s;%s;RMS(%s)", title, xlabel, ylabel),
                       n_iter, edges.data());

    h1_mode->SetDirectory(nullptr);
    h1_fwhm->SetDirectory(nullptr);
    h1_rms ->SetDirectory(nullptr);
    h1_mode->SetStats(0);
    h1_fwhm->SetStats(0);
    h1_rms ->SetStats(0);

    // ── Main loop ──────────────────────────────────────────────────────────────
    for (int ii = 0; ii < n_iter; ++ii)
    {
        const int ix_lo = ii * n_bins_comb + 1;
        const int ix_hi = (ii + 1) * n_bins_comb;

        TH1D* h_tmp = h2_in->ProjectionY(
            Form("_hpy_%s_%d", h2_in->GetName(), ii),
            ix_lo, ix_hi);
        h_tmp->SetDirectory(nullptr);

        // ── Weighted sums ──────────────────────────────────────────────────────
        double sumw = 0.0, sumwy = 0.0, sumwy2 = 0.0, sumw2 = 0.0;
        for (int jj = 1; jj <= h_tmp->GetNbinsX(); ++jj)
        {
            const double bc = h_tmp->GetBinContent(jj);
            const double bx = h_tmp->GetBinCenter(jj);
            const double be = h_tmp->GetBinError(jj);
            sumw   += bc;
            sumwy  += bc * bx;
            sumwy2 += bc * bx * bx;
            sumw2  += be * be;
        }

        const double n_eff = (sumw > 0.0 && sumw2 > 0.0)
                             ? (sumw * sumw) / sumw2
                             : sumw;

        // ── RMS ───────────────────────────────────────────────────────────────
        double rms = 0.0, rms_err = 0.0;
        if (sumw > 0.0)
        {
            const double mean     = sumwy / sumw;
            const double variance = sumwy2 / sumw - mean * mean;
            rms = (variance > 0.0) ? std::sqrt(variance) : 0.0;
            rms_err = (n_eff > 1.0) ? rms / std::sqrt(2.0 * (n_eff - 1.0)) : 0.0;
        }

        // ── Mode ──────────────────────────────────────────────────────────────
        const int    maxBin  = h_tmp->GetMaximumBin();
        const double mode    = h_tmp->GetBinCenter(maxBin);
        const double binW    = h_tmp->GetBinWidth(maxBin);
        // Scale error by 1/sqrt(n_eff) so it shrinks with statistics. At n_eff=1 this equals the original binW/2 (pure geometric floor). (not used here)
        //const double modeErr = (n_eff > 0.0) ? binW / (2.0 * std::sqrt(n_eff)) : binW / 2.0;
        //Use the bin widths to estimate error and do not shrink it with statistics (design choice)
        const double modeErr = h_tmp->GetBinWidth(maxBin) / 2.0;

        // Fill mode and RMS unconditionally — FWHM may still be skipped below.
        h1_mode->SetBinContent(ii + 1, mode);
        h1_mode->SetBinError  (ii + 1, modeErr);
        h1_rms ->SetBinContent(ii + 1, rms);
        h1_rms ->SetBinError  (ii + 1, rms_err);

        // ── FWHM ──────────────────────────────────────────────────────────────
        const double half = 0.5 * h_tmp->GetMaximum();
        const int    bin1 = h_tmp->FindFirstBinAbove(half);
        const int    bin2 = h_tmp->FindLastBinAbove(half);

        if (bin1 > 0 && bin2 > 0 && bin2 > bin1)
        {
            const double w1   = h_tmp->GetBinWidth(bin1);
            const double w2   = h_tmp->GetBinWidth(bin2);
            const double fwhm = h_tmp->GetBinCenter(bin2) - h_tmp->GetBinCenter(bin1);
            // Scale error by 1/sqrt(n_eff) so it shrinks with statistics. At n_eff=1 this equals the original binW/2 (pure geometric floor). (not used here)
            //const double fwhmErr = 0.5 * std::sqrt(w1*w1 + w2*w2) / (n_eff > 0.0 ? std::sqrt(n_eff) : 1.0);
            
            //Use the bin widths to estimate error and do not shrink it with statistics (design choice)
            const double fwhmErr = 0.5 * std::sqrt(w1*w1 + w2*w2);
            h1_fwhm->SetBinContent(ii + 1, fwhm);
            h1_fwhm->SetBinError  (ii + 1, fwhmErr);
        }
        // else: FWHM bin left at 0 — unresolvable for this slice

        delete h_tmp;
    }
}

namespace
{
    std::string StripRootExtension(const std::string& path)
    {
        const std::size_t slash = path.find_last_of("/\\");
        const std::size_t dot = path.find_last_of('.');
        if (dot != std::string::npos && (slash == std::string::npos || dot > slash))
        {
            return path.substr(0, dot);
        }
        return path;
    }

    void EnsureParentDirectoryExists(const std::string& path)
    {
        const std::size_t slash = path.find_last_of("/\\");
        if (slash == std::string::npos || slash == 0)
        {
            return;
        }
        gSystem->mkdir(path.substr(0, slash).c_str(), true);
    }

    void DrawHistogramForPdf(TH1* hist)
    {
        if (!hist)
        {
            return;
        }

        if (hist->InheritsFrom(TH3::Class()))
        {
            hist->Draw();
        }
        else if (hist->InheritsFrom(TH2::Class()))
        {
            hist->Draw("COLZ");
        }
        else
        {
            hist->Draw("E1");
        }
    }

    void ExportHistogramsRecursive(
        TDirectory* directory,
        TCanvas* canvas,
        const std::string& pdfPath,
        const std::string& prefix,
        bool& openedPdf,
        std::size_t& pageCount)
    {
        if (!directory || !canvas)
        {
            return;
        }

        TIter nextKey(directory->GetListOfKeys());
        while (TKey* key = dynamic_cast<TKey*>(nextKey()))
        {
            TObject* obj = key->ReadObj();
            if (!obj)
            {
                continue;
            }

            if (obj->InheritsFrom(TDirectory::Class()))
            {
                TDirectory* subdir = dynamic_cast<TDirectory*>(obj);
                const std::string nextPrefix = prefix.empty()
                    ? std::string(subdir->GetName())
                    : prefix + "/" + subdir->GetName();
                ExportHistogramsRecursive(subdir, canvas, pdfPath, nextPrefix, openedPdf, pageCount);
                delete obj;
                continue;
            }

            TH1* hist = dynamic_cast<TH1*>(obj);
            if (!hist)
            {
                delete obj;
                continue;
            }

            const std::string pageTitle = prefix.empty()
                ? std::string(hist->GetName())
                : prefix + "/" + hist->GetName();

            canvas->Clear();
            canvas->SetLeftMargin(0.12);
            canvas->SetRightMargin(0.15);
            canvas->SetTopMargin(0.08);
            canvas->SetBottomMargin(0.12);
            canvas->cd();
            canvas->SetTitle(pageTitle.c_str());

            DrawHistogramForPdf(hist);
            canvas->Modified();
            canvas->Update();

            if (!openedPdf)
            {
                canvas->Print((pdfPath + "[").c_str());
                openedPdf = true;
            }
            canvas->Print(pdfPath.c_str());
            ++pageCount;

            delete hist;
        }
    }
}

inline void SaveAllHistogramsToPdf(
    const char* rootFilePath,
    const char* outputPdfPath = nullptr)
{
    if (!rootFilePath || !rootFilePath[0])
    {
        std::cerr << "[SaveAllHistogramsToPdf] ERROR: empty ROOT file path.\n";
        return;
    }

    const std::string rootPath = rootFilePath;
    const std::string pdfPath = (outputPdfPath && outputPdfPath[0])
        ? std::string(outputPdfPath)
        : StripRootExtension(rootPath) + "_allhistograms.pdf";

    EnsureParentDirectoryExists(pdfPath);

    TFile* inputFile = TFile::Open(rootPath.c_str(), "READ");
    if (!inputFile || inputFile->IsZombie())
    {
        std::cerr << "[SaveAllHistogramsToPdf] ERROR: could not open "
                  << rootPath << "\n";
        delete inputFile;
        return;
    }

    TCanvas* canvas = new TCanvas("c_all_histograms_pdf",
                                  "c_all_histograms_pdf",
                                  1200, 900);

    bool openedPdf = false;
    std::size_t pageCount = 0;
    ExportHistogramsRecursive(inputFile, canvas, pdfPath, "", openedPdf, pageCount);

    if (openedPdf)
    {
        canvas->Print((pdfPath + "]").c_str());
        std::cout << "[SaveAllHistogramsToPdf] Wrote " << pageCount
                  << " page(s) to " << pdfPath << "\n";
    }
    else
    {
        std::cerr << "[SaveAllHistogramsToPdf] WARNING: no histograms found in "
                  << rootPath << "\n";
    }

    delete canvas;
    inputFile->Close();
    delete inputFile;
}

// CompareTH2DYdistforxbins.cxx
//
// For each x-bin of a TH2D, projects the Y distribution from two files and
// overlays them on a single canvas, then saves as PDF and PNG.
//
// Optional displaymin / displaymax parameters fix the vertical (bin-content)
// axis to [displaymin, displaymax] uniformly across every pad.  If omitted
// (or if an invalid range is given), the range is auto-computed per-pad from
// the data with 15% padding.
//
// Optional ytitle sets the label on the vertical axis.  Defaults to
// "Bin content" if null or empty.

// Compare1DSliceDistsfromTH2D.cxx
//
// For a named TH2D present in two ROOT files, produces two sets of comparison
// canvases:
//
//   (A) Y-slice canvas  — one pad per X bin, each pad shows the Y-axis
//       projection (ProjectionY) of that X-bin slice overlaid for both files.
//       Saved as  <outputname>_yslices.pdf / .png
//
//   (B) X-slice canvas  — one pad per Y bin, each pad shows the X-axis
//       projection (ProjectionX) of that Y-bin slice overlaid for both files.
//       Saved as  <outputname>_xslices.pdf / .png
//
// Parameters
// ----------
//   file1, file2   : paths to the two ROOT files
//   histname       : name of the TH2D inside both files
//   outputname     : human-readable prefix for canvas titles and file names
//   ytitle         : label for the vertical (bin-content) axis
//                    (default: "Bin content")
//   displaymin,
//   displaymax     : optional fixed vertical axis range applied to every pad
//                    in both canvases; auto-computed per-pad when omitted

void Compare1DSliceDistsfromTH2D(
    const char* file1,
    const char* file2,
    const char* histname,
    const char* outputname,         // canvas title prefix and file name prefix
    const char* ytitle    = "Bin content",
    double      displaymin = std::numeric_limits<double>::quiet_NaN(),
    double      displaymax = std::numeric_limits<double>::quiet_NaN()
)
{
    // ------------------------------------------------------------------ //
    // 0.  Basic argument validation
    // ------------------------------------------------------------------ //
    if (!file1 || !file1[0] || !file2 || !file2[0] ||
        !histname || !histname[0] || !outputname || !outputname[0])
    {
        std::cerr << "[Compare1DSliceDistsfromTH2D] ERROR: null/empty input.\n";
        return;
    }

    // Fall back to a sensible default if ytitle is null or empty.
    const char* yaxisTitle = (ytitle && ytitle[0]) ? ytitle : "Bin content";

    static int s_callCount = 0;
    const int  callId      = ++s_callCount;

    auto sanitise = [](const char* raw) -> std::string
    {
        std::string s = raw ? raw : "";
        const std::string bad = "/\\.()[]{}*#? \t\r\n";
        for (char& c : s)
            if (bad.find(c) != std::string::npos)
                c = '_';
        return s;
    };

    const std::string outputTitle = outputname;
    const std::string outputFile  = sanitise(outputname);

    // ------------------------------------------------------------------ //
    // Helper: basename of a file path (for legend labels).
    // ------------------------------------------------------------------ //
    auto baseName = [](const char* path) -> std::string
    {
        std::string s = path ? path : "";
        const std::size_t pos = s.find_last_of("/\\");
        if (pos != std::string::npos) s = s.substr(pos + 1);
        return s;
    };

    auto sameAxis = [](const TAxis* a, const TAxis* b) -> bool
    {
        if (!a || !b) return false;
        if (a->GetNbins() != b->GetNbins()) return false;
        const int nbins = a->GetNbins();
        for (int i = 1; i <= nbins; ++i) {
            const double lo1 = a->GetBinLowEdge(i);
            const double lo2 = b->GetBinLowEdge(i);
            const double hi1 = a->GetBinUpEdge(i);
            const double hi2 = b->GetBinUpEdge(i);
            const double tol = 1e-12 * (1.0
                + std::max(std::fabs(lo1), std::fabs(lo2))
                + std::max(std::fabs(hi1), std::fabs(hi2)));
            if (std::fabs(lo1 - lo2) > tol) return false;
            if (std::fabs(hi1 - hi2) > tol) return false;
        }
        return true;
    };

    // ------------------------------------------------------------------ //
    // 1.  Open files
    // ------------------------------------------------------------------ //
    TFile* f1 = TFile::Open(file1, "READ");
    TFile* f2 = TFile::Open(file2, "READ");

    if (!f1 || f1->IsZombie() || !f2 || f2->IsZombie()) {
        std::cerr << "[Compare1DSliceDistsfromTH2D] ERROR: could not open input file(s).\n";
        
        if (f1 && !f1->IsZombie()) f1->Close(); else delete f1;
        if (f2 && !f2->IsZombie()) f2->Close(); else delete f2;
        return;
    }

    // ------------------------------------------------------------------ //
    // 2.  Retrieve and clone histograms
    // ------------------------------------------------------------------ //
    TH2D* h1_raw = dynamic_cast<TH2D*>(f1->Get(histname));
    TH2D* h2_raw = dynamic_cast<TH2D*>(f2->Get(histname));

    if (!h1_raw || !h2_raw) {
        std::cerr << "[Compare1DSliceDistsfromTH2D] ERROR: TH2D '" << histname
                  << "' not found (or wrong type) in one or both files.\n";
        f1->Close();
        f2->Close();
        return;
    }

    TH2D* h1 = static_cast<TH2D*>(
        h1_raw->Clone(Form("%s_file1_%d", outputFile.c_str(), callId)));
    TH2D* h2 = static_cast<TH2D*>(
        h2_raw->Clone(Form("%s_file2_%d", outputFile.c_str(), callId)));
    h1->SetDirectory(nullptr);
    h2->SetDirectory(nullptr);

    f1->Close();
    f2->Close();

    // ------------------------------------------------------------------ //
    // 3.  Validate axes
    // ------------------------------------------------------------------ //
    if (!sameAxis(h1->GetXaxis(), h2->GetXaxis()) ||
        !sameAxis(h1->GetYaxis(), h2->GetYaxis()))
    {
        std::cerr << "[Compare1DSliceDistsfromTH2D] ERROR: histograms must have "
                     "matching X/Y binning.\n";
        delete h1; delete h2;
        return;
    }

    const int nBinsX = h1->GetNbinsX();
    const int nBinsY = h1->GetNbinsY();
    if (nBinsX < 1 || nBinsY < 1) {
        std::cerr << "[Compare1DSliceDistsfromTH2D] ERROR: histograms have no bins.\n";
        delete h1; delete h2;
        return;
    }

    const std::string lab1 = baseName(file1);
    const std::string lab2 = baseName(file2);

    // R5: pre-compute fixed range flag once; applies to both canvases.
    const bool useDisplayRange = std::isfinite(displaymin) &&
                                 std::isfinite(displaymax) &&
                                 (displaymax > displaymin);

    // ------------------------------------------------------------------ //
    // 4.  Shared drawing helper
    //
    //     Builds one canvas of 1D overlay pads from an arbitrary set of
    //     slice projections, then saves and destroys it.
    //
    //     Parameters:
    //       nSlices     — number of pads (bins along the sliced axis)
    //       canvasTag   — short string embedded in the canvas ROOT name
    //       canvasTitle — human-readable canvas window title
    //       fileSuffix  — appended to outputFile for the saved filenames
    //       project1    — callable(ix) → TH1D* from h1 for slice ix
    //       project2    — callable(ix) → TH1D* from h2 for slice ix
    //       sliceTitle  — callable(ix) → std::string pad title for slice ix
    //       horizTitle  — label for the horizontal axis of each pad
    // ------------------------------------------------------------------ //
    auto drawSliceCanvas = [&](
        int         nSlices,
        const char* canvasTag,
        const char* canvasTitle,
        const char* fileSuffix,
        std::function<TH1D*(int)> project1,
        std::function<TH1D*(int)> project2,
        std::function<std::string(int)> sliceTitle,
        const char* horizTitle)
    {
        const int ncols   = (nSlices < 3) ? nSlices : 3;
        const int nrows   = (nSlices + ncols - 1) / ncols;
        const int cheight = 400 * nrows;

        TCanvas* canvas = new TCanvas(
            Form("c_%s_%s_%d", outputFile.c_str(), canvasTag, callId),
            canvasTitle,
            1400, cheight);
        canvas->Divide(ncols, nrows);

        // Pools — deleted AFTER canvas
        std::vector<TH1D*>    h_pool;
        std::vector<TLegend*> leg_pool;

        for (int i = 1; i <= nSlices; ++i)
        {
            canvas->cd(i);
            gPad->SetGrid();

            TH1D* h1_proj = project1(i);
            TH1D* h2_proj = project2(i);

            if (!h1_proj || !h2_proj) {
                std::cerr << "[Compare1DSliceDistsfromTH2D] WARNING: projection "
                             "returned null for slice " << i << " — skipping.\n";
                if (h1_proj) delete h1_proj;
                if (h2_proj) delete h2_proj;
                continue;
            }

            h1_proj->SetDirectory(nullptr);
            h2_proj->SetDirectory(nullptr);
            h1_proj->SetStats(0);
            h2_proj->SetStats(0);

            h1_proj->SetLineColor(kBlack);
            h1_proj->SetMarkerColor(kBlack);
            h1_proj->SetMarkerStyle(20);
            h1_proj->SetLineWidth(2);

            h2_proj->SetLineColor(kRed + 1);
            h2_proj->SetMarkerColor(kRed + 1);
            h2_proj->SetMarkerStyle(24);
            h2_proj->SetLineWidth(2);

            h1_proj->SetTitle(sliceTitle(i).c_str());

            // ---- Vertical range ------------------------------------------
            double yMin, yMax;
            if (useDisplayRange) {
                yMin = displaymin;
                yMax = displaymax;
            }
            else {
                yMin = h1_proj->GetMinimum(0);
                yMax = h1_proj->GetMaximum(0);
                if (h2_proj->GetMinimum(0) < yMin) yMin = h2_proj->GetMinimum(0);
                if (h2_proj->GetMaximum(0) > yMax) yMax = h2_proj->GetMaximum(0);

                if (!std::isfinite(yMin) || !std::isfinite(yMax) || yMin == yMax) {
                    yMin = 0.0;
                    yMax = 1.0;
                }
                else {
                    const double pad = 0.15 * std::fabs(yMax - yMin);
                    
                    yMin = (yMin >= 0.0 && yMin - pad < 0.0) ? 0.0 : yMin - pad;
                    yMax += pad;
                }
            }

            // ---- Frame histogram -----------------------------------------
            TH1D* hFrame = static_cast<TH1D*>(
                h1_proj->Clone(
                    Form("_frame_%s_%s_%d_%d", outputFile.c_str(), canvasTag, callId, i)));
            hFrame->SetDirectory(nullptr);
            hFrame->Reset();
            hFrame->SetStats(0);
            hFrame->GetXaxis()->SetTitle(horizTitle);
            hFrame->GetYaxis()->SetTitle(yaxisTitle);
            hFrame->SetMinimum(yMin);
            hFrame->SetMaximum(yMax);

            hFrame->Draw();
            h1_proj->Draw("E1 SAME");
            h2_proj->Draw("E1 SAME");

            TLegend* leg = new TLegend(0.55, 0.75, 0.88, 0.88);
            leg->SetBorderSize(0);
            leg->SetFillStyle(0);
            leg->AddEntry(h1_proj, lab1.c_str(), "lep");
            leg->AddEntry(h2_proj, lab2.c_str(), "lep");
            leg->Draw("same");

            h_pool.push_back(hFrame);
            h_pool.push_back(h1_proj);
            h_pool.push_back(h2_proj);
            leg_pool.push_back(leg);
        }

        canvas->Update();
        canvas->SaveAs(Form("%s_%s.pdf", outputFile.c_str(), fileSuffix));
        canvas->SaveAs(Form("%s_%s.png", outputFile.c_str(), fileSuffix));

        delete canvas;
        for (TH1D*    h   : h_pool)  delete h;
        for (TLegend* leg : leg_pool) delete leg;
    };

    // ------------------------------------------------------------------ //
    // 5A.  Y-slice canvas: for each X bin, project along Y
    // ------------------------------------------------------------------ //
    drawSliceCanvas(
        nBinsX,
        "yslice",
        Form("%s — Y slices", outputTitle.c_str()),
        "yslices",
        // project1 / project2
        [&](int ix) -> TH1D* {
            return static_cast<TH1D*>(h1->ProjectionY(
                Form("_cmpy_%s_%d_h1_%d", outputFile.c_str(), callId, ix), ix, ix));
        },
        [&](int ix) -> TH1D* {
            return static_cast<TH1D*>(h2->ProjectionY(
                Form("_cmpy_%s_%d_h2_%d", outputFile.c_str(), callId, ix), ix, ix));
        },
        // pad title: histname: (X-axis title) ∈ [lo, hi]
        [&](int ix) -> std::string {
            return Form("%s: (%s) #in [%.4g, %.4g]",
                        histname,
                        h1->GetXaxis()->GetTitle(),
                        h1->GetXaxis()->GetBinLowEdge(ix),
                        h1->GetXaxis()->GetBinUpEdge(ix));
        },
        // horizontal axis label = TH2D Y-axis title
        h1->GetYaxis()->GetTitle()
    );

    // ------------------------------------------------------------------ //
    // 5B.  X-slice canvas: for each Y bin, project along X
    // ------------------------------------------------------------------ //
    drawSliceCanvas(
        nBinsY,
        "xslice",
        Form("%s — X slices", outputTitle.c_str()),
        "xslices",
        // project1 / project2
        [&](int iy) -> TH1D* {
            return static_cast<TH1D*>(h1->ProjectionX(
                Form("_cmpx_%s_%d_h1_%d", outputFile.c_str(), callId, iy), iy, iy));
        },
        [&](int iy) -> TH1D* {
            return static_cast<TH1D*>(h2->ProjectionX(
                Form("_cmpx_%s_%d_h2_%d", outputFile.c_str(), callId, iy), iy, iy));
        },
        // pad title: histname: (Y-axis title) ∈ [lo, hi]
        [&](int iy) -> std::string {
            return Form("%s: (%s) #in [%.4g, %.4g]",
                        histname,
                        h1->GetYaxis()->GetTitle(),
                        h1->GetYaxis()->GetBinLowEdge(iy),
                        h1->GetYaxis()->GetBinUpEdge(iy));
        },
        // horizontal axis label = TH2D X-axis title
        h1->GetXaxis()->GetTitle()
    );

    // ------------------------------------------------------------------ //
    // 6.  Release cloned histograms
    // ------------------------------------------------------------------ //
    delete h1;
    delete h2;
}

void CreateandSaveCanvaswithPID(const char* outputdir, const char* outputname, TH1* h1, TH1* h2, TH1* h3, TH1* h4, TH1* h5, TH1* h6, TH1* h7, TH1* h8, const float title_xstart, const float title_ystart, const char* description, const char* cuts_info, float yfitrange){
        TCanvas* canvas = new TCanvas(outputname,outputname,800,600);
        TPad *pad = new TPad("pad_title", "pad_title", 0.0, 0.92, 1.0, 1.0);
        pad->Draw();
        
        TLatex* latex=new TLatex();
        latex->SetNDC();
        latex->SetTextSize(0.04);
        latex->SetTextAlign(22);
        latex->DrawLatex(0.5, title_ystart, Form("%s",description));
        latex->SetTextSize(0.03);
        if(cuts_info && yfitrange!=0){latex->DrawLatex(0.5, 0.85, Form("%s, Fit: |yfit|<%.4f ",cuts_info,yfitrange));}
        else if(cuts_info && yfitrange==0){latex->DrawLatex(0.5, 0.85, Form("%s",cuts_info));}
        else {}
        
        //canvas_meanwidth->Divide(2);
        canvas->cd();
        pad  = new TPad("pad11",  "pad11",  0.0, 0.4, 0.25, 0.82);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if(h1){h1->Draw("E");}
        
        
        canvas->cd();
        pad  = new TPad("pad21",  "pad21",  0.0, 0.0, 0.25, 0.4);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if(h2){h2->Draw("E");}
    
        canvas->cd();
        pad  = new TPad("pad12",  "pad12",  0.25, 0.4, 0.5, 0.82);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if(h3){h3->Draw("E");}
        
        
        canvas->cd();
        pad  = new TPad("pad22",  "pad22",  0.25, 0.0, 0.5, 0.4);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if(h4){h4->Draw("E");}
    
        canvas->cd();
        pad  = new TPad("pad13",  "pad13",  0.5, 0.4, 0.75, 0.82);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if(h5){h5->Draw("E");}
        
        
        canvas->cd();
        pad  = new TPad("pad23",  "pad23",  0.5, 0.0, 0.75, 0.4);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if(h6){h6->Draw("E");}
    
        canvas->cd();
        pad  = new TPad("pad14",  "pad14",  0.75, 0.4, 1, 0.82);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if(h7){h7->Draw("E");}
        
        
        canvas->cd();
        pad  = new TPad("pad24",  "pad24",  0.75, 0.0, 1, 0.4);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if(h8){h8->Draw("E");}
    
    
        canvas->Draw();
        canvas->SaveAs(Form("%s/%s.pdf",outputdir,outputname));
        canvas->SaveAs(Form("%s/%s.png",outputdir,outputname));
        //canvas->SaveAs(Form("%s/%s.root",outputdir,outputname));
    
        delete canvas;
        delete latex;
}

void CreateandSaveCanvaswithPID(const char* outputdir, const char* outputname, TObject* h1, TObject* h2, TObject* h3, TObject* h4, const float title_xstart, const float title_ystart, const char* description, const char* cuts_info, float yfitrange=0.0){
        TCanvas* canvas = new TCanvas(outputname,outputname,800,600);
        TPad *pad = new TPad("pad_title", "pad_title", 0.0, 0.92, 1.0, 1.0);
        pad->Draw();
        
        TLatex* latex=new TLatex();
        latex->SetNDC();
        latex->SetTextSize(0.04);
        latex->SetTextAlign(22);
        latex->DrawLatex(0.5, title_ystart, Form("%s",description));
        latex->SetTextSize(0.03);
        if(cuts_info && yfitrange!=0){latex->DrawLatex(0.5, 0.85, Form("%s, Fit: |yfit|<%.4f ",cuts_info,yfitrange));}
        else if(cuts_info && yfitrange==0){latex->DrawLatex(0.5, 0.85, Form("%s",cuts_info));}
        else {}
        
        
        //canvas_meanwidth->Divide(2);
        canvas->cd();
        pad  = new TPad("pad11",  "pad11",  0.0, 0.4, 0.5, 0.82);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if (h1) {
            if (h1->InheritsFrom(TH2::Class())) {
                h1->Draw("colz");
            }
            else if (h1->InheritsFrom(TEfficiency::Class())) {
                h1->Draw("AP");  // or "COLZ" if 2D
            }
        }
        
        
        canvas->cd();
        pad  = new TPad("pad21",  "pad21",  0.0, 0.0, 0.5, 0.4);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if (h2) {
            if (h2->InheritsFrom(TH2::Class())) {
                h2->Draw("colz");
            }
            else if (h2->InheritsFrom(TEfficiency::Class())) {
                h2->Draw("AP");  // or "COLZ" if 2D
            }
        }
    
        canvas->cd();
        pad  = new TPad("pad12",  "pad12",  0.5, 0.4, 1.0, 0.82);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if (h3) {
            if (h3->InheritsFrom(TH2::Class())) {
                h3->Draw("colz");
            }
            else if (h3->InheritsFrom(TEfficiency::Class())) {
                h3->Draw("AP");  // or "COLZ" if 2D
            }
        }
        
        
        canvas->cd();
        pad  = new TPad("pad22",  "pad22",  0.5, 0.0, 1.0, 0.4);
        pad->Draw();
        pad->cd();
        pad->SetLeftMargin(0.18);
        pad->SetRightMargin(0.18);
        pad->SetTopMargin(0.12);
        pad->SetBottomMargin(0.12);
        if (h4) {
            if (h4->InheritsFrom(TH2::Class())) {
                h4->Draw("colz");
            }
            else if (h4->InheritsFrom(TEfficiency::Class())) {
                h4->Draw("AP");  // or "COLZ" if 2D
            }
        }
    
        canvas->Draw();
        canvas->SaveAs(Form("%s/%s.pdf",outputdir,outputname));
        canvas->SaveAs(Form("%s/%s.png",outputdir,outputname));
        //canvas->SaveAs(Form("%s/%s.root",outputdir,outputname));
    
        delete canvas;
        delete latex;
}

void CreateandSaveCanvaswithPID(const char* outputdir, const char* outputname, const std::vector<TH1D*>& histset1,const std::vector<TH1D*>& histset2, const std::vector<TH1D*>& histset3, const std::vector<TH1D*>& histset4, const std::vector<TH1D*>& histset5, const std::vector<TH1D*>& histset6, const std::vector<TH1D*>& histset7, const std::vector<TH1D*>& histset8, const std::vector<std::string> particles, float Upperpanels_ymin, float Upperpanels_ymax, float Lowerpanels_ymin, float Lowerpanels_ymax, const float title_xstart, const float title_ystart, const char* description, const char* cuts_info, float yfitrange){
    
    TCanvas* canvas = new TCanvas(outputname,outputname,800,600);
    TPad *pad = new TPad("pad_title", "pad_title", 0.0, 0.92, 1.0, 1.0);
    pad->Draw();
    
    std::vector<int> colors = {
        kBlack,
        kRed,
        kBlue,
        kGreen+2,
        kMagenta,
        kCyan+1
    };
    
    
    TLatex* latex=new TLatex();
    latex->SetNDC();
    latex->SetTextSize(0.04);
    latex->SetTextAlign(22);
    latex->DrawLatex(0.5, title_ystart, Form("%s",description));
    latex->SetTextSize(0.03);
    if(cuts_info && yfitrange!=0){latex->DrawLatex(0.5, 0.85, Form("%s, Fit: |yfit|<%.4f ",cuts_info,yfitrange));}
    else if(cuts_info && yfitrange==0){latex->DrawLatex(0.5, 0.85, Form("%s",cuts_info));}
    else {}
    
    TLine* line=new TLine(histset1[0]->GetXaxis()->GetXmin(),0.0,histset1[0]->GetXaxis()->GetXmax(),0.0);
    
    //canvas_meanwidth->Divide(2);
    canvas->cd();
    pad  = new TPad("pad11",  "pad11",  0.0, 0.4, 0.25, 0.82);
    pad->Draw();
    pad->cd();
    pad->SetLeftMargin(0.18);
    pad->SetRightMargin(0.18);
    pad->SetTopMargin(0.12);
    pad->SetBottomMargin(0.12);
    
    TLegend* leg = new TLegend(0.5, 0.6, 0.81, 0.87);
    
    for(int i=0;i<histset1.size();i++){
        histset1[i]->GetYaxis()->SetRangeUser(Upperpanels_ymin,Upperpanels_ymax);
        histset1[i]->SetLineColor(colors[i]);
        histset1[i]->SetMarkerColor(colors[i]);
        histset1[i]->SetLineWidth(2);
        histset1[i]->Draw(i==0 ? "" : "SAME");
        
        leg->AddEntry(histset1[i], particles[i].c_str(), "l");
    }
    line->Draw("same");
    leg->Draw("same");
    
    canvas->cd();
    pad  = new TPad("pad21",  "pad21",  0.0, 0.0, 0.25, 0.4);
    pad->Draw();
    pad->cd();
    pad->SetLeftMargin(0.18);
    pad->SetRightMargin(0.18);
    pad->SetTopMargin(0.12);
    pad->SetBottomMargin(0.12);
    
    for(int i=0;i<histset2.size();i++){
        histset2[i]->GetYaxis()->SetRangeUser(Lowerpanels_ymin,Lowerpanels_ymax);
        histset2[i]->SetLineColor(colors[i]);
        histset2[i]->SetMarkerColor(colors[i]);
        histset2[i]->SetLineWidth(2);
        histset2[i]->Draw(i==0 ? "" : "SAME");
    }
    
    canvas->cd();
    pad  = new TPad("pad12",  "pad12",  0.25, 0.4, 0.5, 0.82);
    pad->Draw();
    pad->cd();
    pad->SetLeftMargin(0.18);
    pad->SetRightMargin(0.18);
    pad->SetTopMargin(0.12);
    pad->SetBottomMargin(0.12);
    
    for(int i=0;i<histset3.size();i++){
        histset3[i]->GetYaxis()->SetRangeUser(Upperpanels_ymin,Upperpanels_ymax);
        histset3[i]->SetLineColor(colors[i]);
        histset3[i]->SetMarkerColor(colors[i]);
        histset3[i]->SetLineWidth(2);
        histset3[i]->Draw(i==0 ? "" : "SAME");
    }
    line->Draw("same");
    
    canvas->cd();
    pad  = new TPad("pad22",  "pad22",  0.25, 0.0, 0.5, 0.4);
    pad->Draw();
    pad->cd();
    pad->SetLeftMargin(0.18);
    pad->SetRightMargin(0.18);
    pad->SetTopMargin(0.12);
    pad->SetBottomMargin(0.12);
    
    for(int i=0;i<histset4.size();i++){
        histset4[i]->GetYaxis()->SetRangeUser(Lowerpanels_ymin,Lowerpanels_ymax);
        histset4[i]->SetLineColor(colors[i]);
        histset4[i]->SetMarkerColor(colors[i]);
        histset4[i]->SetLineWidth(2);
        histset4[i]->Draw(i==0 ? "" : "SAME");
    }
    
    canvas->cd();
    pad  = new TPad("pad13",  "pad13",  0.5, 0.4, 0.75, 0.82);
    pad->Draw();
    pad->cd();
    pad->SetLeftMargin(0.18);
    pad->SetRightMargin(0.18);
    pad->SetTopMargin(0.12);
    pad->SetBottomMargin(0.12);
    
    for(int i=0;i<histset5.size();i++){
        histset5[i]->GetYaxis()->SetRangeUser(Upperpanels_ymin,Upperpanels_ymax);
        histset5[i]->SetLineColor(colors[i]);
        histset5[i]->SetMarkerColor(colors[i]);
        histset5[i]->SetLineWidth(2);
        histset5[i]->Draw(i==0 ? "" : "SAME");
    }
    line->Draw("same");
    
    canvas->cd();
    pad  = new TPad("pad23",  "pad23",  0.5, 0.0, 0.75, 0.4);
    pad->Draw();
    pad->cd();
    pad->SetLeftMargin(0.18);
    pad->SetRightMargin(0.18);
    pad->SetTopMargin(0.12);
    pad->SetBottomMargin(0.12);
    
    for(int i=0;i<histset6.size();i++){
        histset6[i]->GetYaxis()->SetRangeUser(Lowerpanels_ymin,Lowerpanels_ymax);
        histset6[i]->SetLineColor(colors[i]);
        histset6[i]->SetMarkerColor(colors[i]);
        histset6[i]->SetLineWidth(2);
        histset6[i]->Draw(i==0 ? "" : "SAME");
    }
    
    canvas->cd();
    pad  = new TPad("pad14",  "pad14",  0.75, 0.4, 1, 0.82);
    pad->Draw();
    pad->cd();
    pad->SetLeftMargin(0.18);
    pad->SetRightMargin(0.18);
    pad->SetTopMargin(0.12);
    pad->SetBottomMargin(0.12);
    
    for(int i=0;i<histset7.size();i++){
        histset7[i]->GetYaxis()->SetRangeUser(Upperpanels_ymin,Upperpanels_ymax);
        histset7[i]->SetLineColor(colors[i]);
        histset7[i]->SetMarkerColor(colors[i]);
        histset7[i]->SetLineWidth(2);
        histset7[i]->Draw(i==0 ? "" : "SAME");
    }
    line->Draw("same");
    
    canvas->cd();
    pad  = new TPad("pad24",  "pad24",  0.75, 0.0, 1, 0.4);
    pad->Draw();
    pad->cd();
    pad->SetLeftMargin(0.18);
    pad->SetRightMargin(0.18);
    pad->SetTopMargin(0.12);
    pad->SetBottomMargin(0.12);
    
    for(int i=0;i<histset8.size();i++){
        histset8[i]->GetYaxis()->SetRangeUser(Lowerpanels_ymin,Lowerpanels_ymax);
        histset8[i]->SetLineColor(colors[i]);
        histset8[i]->SetMarkerColor(colors[i]);
        histset8[i]->SetLineWidth(2);
        histset8[i]->Draw(i==0 ? "" : "SAME");
    }
    
    canvas->Draw();
    canvas->SaveAs(Form("%s/%s_particlesoverlaid.pdf",outputdir,outputname));
    canvas->SaveAs(Form("%s/%s_particlesoverlaid.png",outputdir,outputname));
    //canvas->SaveAs(Form("%s/%s_particlesoverlaid.root",outputdir,outputname));
    
    delete canvas;
    delete leg;
    delete line;
    delete latex;
}

TH2D* GetWidthFromTProfile2D(const TProfile2D* prof)
{
    if (!prof) return nullptr;

    // Clone to safely change error mode without mutating the original
    TProfile2D* tmp = (TProfile2D*)prof->Clone();
    tmp->SetDirectory(nullptr);  // prevent ROOT global directory ownership
    tmp->SetErrorOption("s");    // GetBinError now returns RMS (spread)

    // Pull axis metadata from the original to make intent clear
    const TAxis* xaxis = prof->GetXaxis();
    const TAxis* yaxis = prof->GetYaxis();

    int nx = xaxis->GetNbins();
    int ny = yaxis->GetNbins();

    // Build a unique name to avoid ROOT duplicate-name warnings if this
    // function is called more than once on the same profile.
    TString name  = TString::Format("RMSwidthof_%s", prof->GetName());
    TString title = TString::Format("RMSwidthof_%s", prof->GetTitle()); //TString(prof->GetTitle()) + " (width)";

    // Correctly handle both uniform and variable-width bin axes.
    // GetXbins()->GetArray() returns nullptr for uniform axes.
    const double* xbins = xaxis->GetXbins()->GetArray();
    const double* ybins = yaxis->GetXbins()->GetArray();

    TH2D* h_width = nullptr;
    if      ( xbins &&  ybins) h_width = new TH2D(name, title, nx, xbins, ny, ybins);
    else if ( xbins && !ybins) h_width = new TH2D(name, title, nx, xbins, ny, yaxis->GetXmin(), yaxis->GetXmax());
    else if (!xbins &&  ybins) h_width = new TH2D(name, title, nx, xaxis->GetXmin(), xaxis->GetXmax(), ny, ybins);
    else                       h_width = new TH2D(name, title, nx, xaxis->GetXmin(), xaxis->GetXmax(),
                                                                   ny, yaxis->GetXmin(), yaxis->GetXmax());
    h_width->GetXaxis()->SetTitle(xaxis->GetTitle());
    h_width->GetYaxis()->SetTitle(yaxis->GetTitle());
    h_width->SetDirectory(nullptr);

    for (int ix = 1; ix <= nx; ++ix)
    {
        for (int iy = 1; iy <= ny; ++iy)
        {
            int bin = tmp->GetBin(ix, iy);

            // Use > 0.5 rather than > 0 to guard against floating-point
            // artifacts that can arise from weighted fills.
            double entries = tmp->GetBinEntries(bin);
            if (entries < 0.5) continue;  // leaves content and error at 0

            double rms = tmp->GetBinError(bin);  // RMS due to SetErrorOption("s")

            h_width->SetBinContent(ix, iy, rms);

            // Propagate a meaningful uncertainty on the RMS itself.
            // Requires at least 2 entries for a well-defined sample spread;
            // with exactly 1 entry the RMS is identically 0 and its
            // uncertainty is undefined, so we leave the error at 0.
            if (entries > 1.5)
            {
                // Standard error of the sample standard deviation:
                //   sigma(RMS) ≈ RMS / sqrt(2 * (n - 1))
                double rms_error = rms / std::sqrt(2.0 * (entries - 1.0));
                h_width->SetBinError(ix, iy, rms_error);
            }
        }
    }

    delete tmp;
    
    return h_width;
}

struct FitSliceCanvasResults {
    TCanvas* cMean  = nullptr;
    TCanvas* cSigma = nullptr;
};

/*
 * For every z-bin of a TH3D:
 *
 *   - Extract the corresponding x-y slice
 *   - Run GetFitSlicesY on that slice
 *   - Draw the mean, mode ,width, FWHM histograms on separate canvases
 *
 * IMPORTANT:
 *   TH3 does NOT have ProjectionXY().
 *   The correct method is Project3D("yx") after setting the z range.
 */

FitSliceCanvasResults DrawFitSlicesYForTH3D(
    TH3D* h3,
    TF1* fittingfunc,
    const char* basename = "hist3d",
    const char* xlabel   = "x",
    const char* ylabel   = "y")
{
    FitSliceCanvasResults results;

    if (!h3 || !fittingfunc) {
        return results;
    }

    const int nz = h3->GetNbinsZ();

    //
    // Canvas layout
    //
    int nCols = (int) std::ceil(std::sqrt((double)nz));
    int nRows = (int) std::ceil((double)nz / nCols);

    //
    // Create canvases
    //
    results.cMean = new TCanvas(
        Form("%s_mean_canvas", basename),
        Form("%s Mean", basename),
        1600,
        1000
    );

    results.cSigma = new TCanvas(
        Form("%s_sigma_canvas", basename),
        Form("%s Width", basename),
        1600,
        1000
    );

    results.cMean->Divide(nCols, nRows);
    results.cSigma->Divide(nCols, nRows);
    std::vector<TH1D*> hist_pool;

    //
    // Loop over z bins
    //
    for (int iz = 1; iz <= nz; ++iz) {

        //
        // Restrict histogram to ONE z-bin
        //
        h3->GetZaxis()->SetRange(iz, iz);

        //
        // Create XY slice
        //
        TH2D* h2Slice = dynamic_cast<TH2D*>(
            h3->Project3D("yx")
        );

        if (!h2Slice) {
            continue;
        }

        //
        // Detach from ROOT directory ownership
        //
        h2Slice->SetDirectory(nullptr);

        //
        // Rename uniquely
        //
        h2Slice->SetName(
            Form("%s_xy_zbin%d", basename, iz)
        );

        //
        // Skip empty slices
        //
        if (h2Slice->GetEntries() == 0) {
            delete h2Slice;
            continue;
        }

        //
        // z-bin range
        //
        double zLow = h3->GetZaxis()->GetBinLowEdge(iz);
        double zUp  = h3->GetZaxis()->GetBinUpEdge(iz);

        TString ptTitle = Form(
            "%.2f < p_{T} < %.2f",
            zLow,
            zUp
        );

        //
        // Output histograms
        //
        TH1D* hMean  = nullptr;
        TH1D* hSigma = nullptr;

        //
        // Run fits
        //
        GetFitSlicesY(
            h2Slice,
            fittingfunc,
            hMean,
            hSigma,
            ptTitle.Data(),
            xlabel,
            ylabel
        );

        //
        // Draw mean histogram
        //
        results.cMean->cd(iz);

        if (hMean) {

            hMean->SetDirectory(nullptr);

            hMean->SetTitle(
                Form("Mean: %s", ptTitle.Data())
            );

            hMean->GetXaxis()->SetTitle(xlabel);
            hMean->GetYaxis()->SetTitle("Mean");

            hMean->SetLineWidth(2);

            gPad->SetGrid();

            hMean->Draw("E1");
            hist_pool.push_back(hMean);
        }

        //
        // Draw width histogram
        //
        results.cSigma->cd(iz);

        if (hSigma) {

            hSigma->SetDirectory(nullptr);

            hSigma->SetTitle(
                Form("Width: %s", ptTitle.Data())
            );

            hSigma->GetXaxis()->SetTitle(xlabel);
            hSigma->GetYaxis()->SetTitle("Width");

            hSigma->SetLineWidth(2);

            gPad->SetGrid();

            hSigma->Draw("E1");
            hist_pool.push_back(hSigma);
        }

        delete h2Slice;
    }

    //
    // IMPORTANT:
    // Reset z range afterwards
    //
    h3->GetZaxis()->SetRange(0, 0);

    results.cMean->Update();
    results.cSigma->Update();
    
    results.cMean->SaveAs(Form("%s_Mean_canvas.pdf", basename));
    results.cSigma->SaveAs(Form("%s_Sigma_canvas.pdf", basename));
    
    for (TH1D* h : hist_pool) delete h;
    
    return results;
}

void DrawFitSlicesComparisonForTH3D(
    const char* simulation_filename,
    const char* data_filename,
    const char* hist3d_name,
    const char* outputname,
    TF1*        fitfunc,
    int         base_along,
    int         fit_along,
    int         slice_along,
    const char* BaseAxisLabel,
    const char* FitAxisLabel,
    const char* SliceAxisLabel,
    double fitrange_lo_meanmode,
    double fitrange_hi_meanmode,
    double fitrange_lo_sigmaFWHMRMS,
    double fitrange_hi_sigmaFWHMRMS,
    double BaseAxisrange_lo  = -2.0,
    double BaseAxisrange_hi  =  2.0,
    int    rebin_BaseAxis    =  8,
    int    rebin_SliceAxis   =  1,
    const char* vertex_level_cuts = "vertex_ntracks > 3 && |v_{x}|>=1e^{-7}",
    const char* track_level_cuts  = "nmaps>=2, nintt>=1, pt>=0.8",
    const char* pair_level_cuts   = "|p_{1}-p_{2}|<=0.1")
{
    const double fwhm_to_sigma = 1.0 / (2.0 * std::sqrt(2.0 * std::log(2.0)));

    // ── Validate axis parameters ────────────────────────────────────────────────
    auto axisInRange = [](int ax) { return ax >= 1 && ax <= 3; };
    if (!axisInRange(fit_along) || !axisInRange(slice_along) || !axisInRange(base_along)) {
        std::cerr << "[DrawFitSlicesComparisonForTH3D] ERROR: fit_along, slice_along and "
                  << "base_along must each be 1, 2, or 3. Got fit_along=" << fit_along
                  << " slice_along=" << slice_along << " base_along=" << base_along << ".\n";
        return;
    }
    if (fit_along == slice_along || fit_along == base_along || slice_along == base_along) {
        std::cerr << "[DrawFitSlicesComparisonForTH3D] ERROR: fit_along, slice_along and "
                  << "base_along must all be distinct. Got fit_along=" << fit_along
                  << " slice_along=" << slice_along << " base_along=" << base_along << ".\n";
        return;
    }

    if (rebin_SliceAxis <= 0) {
        std::cerr << "[DrawFitSlicesComparisonForTH3D] ERROR: rebin_SliceAxis must be >= 1, got "
                  << rebin_SliceAxis << ".\n";
        return;
    }
    if (!fitfunc) {
        std::cerr << "[DrawFitSlicesComparisonForTH3D] ERROR: null TF1 pointer.\n";
        return;
    }

    // ── Helpers to map axis integer → axis name character and TAxis* ────────────
    //
    // AxisName: returns the ROOT Project3D character ("x","y","z") for a given
    //           TH3D axis integer (1=X, 2=Y, 3=Z).
    auto AxisName = [](int ax) -> const char* {
        if (ax == 1) return "x";
        if (ax == 2) return "y";
        return "z";
    };

    // GetAxis: returns the TAxis* for the requested axis integer from a TH3D.
    auto GetAxis = [](TH3D* h, int ax) -> TAxis* {
        if (ax == 1) return h->GetXaxis();
        if (ax == 2) return h->GetYaxis();
        return h->GetZaxis();
    };

    // Project3D string: leftmost char → output TH2D Y (FitAxis),
    //                   rightmost char → output TH2D X (BaseAxis).
    const std::string projStr = std::string(AxisName(fit_along))
                              + std::string(AxisName(base_along));

    // ── Open files ─────────────────────────────────────────────────────────────
    TFile* f1 = TFile::Open(simulation_filename);
    TFile* f2 = TFile::Open(data_filename);

    if (!f1 || f1->IsZombie() || !f2 || f2->IsZombie()) {
        std::cerr << "[DrawFitSlicesComparisonForTH3D] ERROR: could not open input file(s).\n";
        if (f1) delete f1;
        if (f2) delete f2;
        return;
    }

    TH3D* h3_1_raw = dynamic_cast<TH3D*>(f1->Get(hist3d_name));
    TH3D* h3_2_raw = dynamic_cast<TH3D*>(f2->Get(hist3d_name));

    if (!h3_1_raw || !h3_2_raw) {
        std::cerr << "[DrawFitSlicesComparisonForTH3D] ERROR: TH3D '"
                  << hist3d_name << "' not found (or wrong type) in one or both files.\n";
        delete f1;
        delete f2;
        return;
    }

    TH3D* h3_1 = static_cast<TH3D*>(h3_1_raw->Clone(Form("%s_sim",  hist3d_name)));
    TH3D* h3_2 = static_cast<TH3D*>(h3_2_raw->Clone(Form("%s_data", hist3d_name)));
    h3_1->SetDirectory(nullptr);
    h3_2->SetDirectory(nullptr);

    delete f1;
    delete f2;

    // ── Layout ──────────────────────────────────────────────────────────────────
    const int nz      = GetAxis(h3_1, slice_along)->GetNbins();
    const int nz_iter = (nz + rebin_SliceAxis - 1) / rebin_SliceAxis;
    const int ncols   = 3;
    const int nrows   = (nz_iter + ncols - 1) / ncols;
    const int cheight = std::min(400 * nrows, 4000);

    // ── Canvas title / annotation string ────────────────────────────────────────
    const TString fitRangeAnnotation = TString::Format(
        "FitRange: %.4g < %s < %.4g  |  EvtCuts: %s  |  TrackCuts: %s  |  PairCuts: %s",
        fitrange_lo_sigmaFWHMRMS, FitAxisLabel, fitrange_hi_sigmaFWHMRMS,
        vertex_level_cuts, track_level_cuts, pair_level_cuts);

    // ── Layout constants ────────────────────────────────────────────────────────
    const double titleFrac = 0.06;
    const double padAreaHi = 1.0 - titleFrac;

    // ── SetupCanvas ─────────────────────────────────────────────────────────────
    auto SetupCanvas = [&](TCanvas* c, const char* quantityLabel,
                           const TString& fitRangeStr)
    {
        c->cd();

        // Title strip — transparent TPad occupying the top titleFrac of canvas
        TPad* titlePad = new TPad(
            Form("titlePad_%s", c->GetName()), "",
            0.0, padAreaHi, 1.0, 1.0);
        titlePad->SetFillStyle(0);
        titlePad->SetBorderSize(0);
        titlePad->Draw();
        titlePad->cd();

        // Line 1: quantity label (large, centred)
        TLatex* texQty = new TLatex(0.5, 0.72,
            TString::Format("%s", quantityLabel).Data());
        texQty->SetNDC();
        texQty->SetTextAlign(22);
        texQty->SetTextSize(0.38);
        texQty->SetTextFont(62);   // bold
        texQty->Draw();

        // Line 2: FitRange | EvtCuts | TrackCuts | PairCuts (smaller, centred)
        TLatex* texCuts = new TLatex(0.5, 0.28, fitRangeStr.Data());
        texCuts->SetNDC();
        texCuts->SetTextAlign(22);
        texCuts->SetTextSize(0.28);
        texCuts->SetTextFont(42);
        texCuts->Draw();

        // Sub-pads in the lower (1-titleFrac) portion of the canvas
        const double margin = 0.01;
        const double cellW  = (1.0 - margin) / ncols;
        const double cellH  = (padAreaHi - margin) / nrows;

        for (int row = 0; row < nrows; ++row) {
            for (int col = 0; col < ncols; ++col) {
                const double x1 = col       * cellW + margin;
                const double x2 = (col + 1) * cellW;
                const double y2 = padAreaHi - row       * cellH - margin;
                const double y1 = padAreaHi - (row + 1) * cellH;

                c->cd();
                TPad* pad = new TPad(
                    Form("%s_pad%d", c->GetName(), row * ncols + col + 1), "",
                    x1, y1, x2, y2);
                pad->SetNumber(row * ncols + col + 1);
                pad->Draw();
            }
        }
    };

    // ── Create canvases ─────────────────────────────────────────────────────────
    TCanvas* cMean  = new TCanvas(Form("cMean_%s",  outputname), Form("Mean_%s",  outputname), 1400, cheight);
    TCanvas* cSigma = new TCanvas(Form("cSigma_%s", outputname), Form("Sigma_%s", outputname), 1400, cheight);
    TCanvas* cMode  = new TCanvas(Form("cMode_%s",  outputname), Form("Mode_%s",  outputname), 1400, cheight);
    TCanvas* cFWHM  = new TCanvas(Form("cFWHM_%s",  outputname), Form("FWHM_%s",  outputname), 1400, cheight);
    TCanvas* cRMS   = new TCanvas(Form("cRMS_%s",   outputname), Form("RMS_%s",   outputname), 1400, cheight);
    TCanvas* cSimCmp = new TCanvas(
        Form("cSigmaRMSFWHMComparison_Simulation_%s", outputname),
        Form("SigmaRMSFWHMComparison_Simulation_%s",  outputname), 1400, cheight);
    TCanvas* cDataCmp = new TCanvas(
        Form("cSigmaRMSFWHMComparison_Data_%s", outputname),
        Form("SigmaRMSFWHMComparison_Data_%s",  outputname), 1400, cheight);

    const TString titleMean  = TString::Format("Mean of %s", FitAxisLabel);
    const TString titleSigma = TString::Format("Fitted Gaussian Width of %s", FitAxisLabel);
    const TString titleMode  = TString::Format("Mode of %s", FitAxisLabel);
    const TString titleFWHM  = TString::Format("FWHM / (2#sqrt{2ln2}) of %s", FitAxisLabel);
    const TString titleRMS   = TString::Format("RMS of %s", FitAxisLabel);
    const TString titleSimCmp =
        TString::Format("Sim: Width vs RMS vs FWHM of %s", FitAxisLabel);
    const TString titleDataCmp =
        TString::Format("Data: Width vs RMS vs FWHM of %s", FitAxisLabel);

    SetupCanvas(cMean,    titleMean.Data(),    fitRangeAnnotation);
    SetupCanvas(cSigma,   titleSigma.Data(),   fitRangeAnnotation);
    SetupCanvas(cMode,    titleMode.Data(),    fitRangeAnnotation);
    SetupCanvas(cFWHM,    titleFWHM.Data(),    fitRangeAnnotation);
    SetupCanvas(cRMS,     titleRMS.Data(),     fitRangeAnnotation);
    SetupCanvas(cSimCmp,  titleSimCmp.Data(),  fitRangeAnnotation);
    SetupCanvas(cDataCmp, titleDataCmp.Data(), fitRangeAnnotation);

    std::vector<TH1D*> hist_pool;

    // ── Helper lambdas ──────────────────────────────────────────────────────────

    // RestrictYRange / RestoreYRange operate on the output TH2D's Y-axis, which
    // always corresponds to the FitAxis regardless of which TH3D axis that was,
    // because Project3D is constructed to put FitAxis on TH2D-Y.
    auto RestrictYRange = [](TH2D* h, double ylo, double yhi)
        -> std::pair<int,int>
    {
        TAxis* ay = h->GetYaxis();
        const int oldFirst = ay->GetFirst();
        const int oldLast  = ay->GetLast();
        ay->SetRange(ay->FindBin(ylo), ay->FindBin(yhi));
        return {oldFirst, oldLast};
    };

    auto RestoreYRange = [](TH2D* h, std::pair<int,int> saved)
    {
        h->GetYaxis()->SetRange(saved.first, saved.second);
    };

    double fitfunc_range_lo, fitfunc_range_hi;
    fitfunc->GetRange(fitfunc_range_lo, fitfunc_range_hi);
    fitfunc->SetRange(fitrange_lo_sigmaFWHMRMS, fitrange_hi_sigmaFWHMRMS);

    auto DrawPair = [&](TH1D* h1, TH1D* h2,
                        const char* sliceLabelStr,
                        const char* xTitle, const char* yTitle,
                        double ylo, double yhi,
                        const char* lab1, const char* lab2)
    {
        TH1D* hFrame = h1 ? h1 : h2;
        if (!hFrame) return;

        hFrame->SetTitle(sliceLabelStr);
        hFrame->GetXaxis()->SetTitle(xTitle);
        hFrame->GetYaxis()->SetTitle(yTitle);
        hFrame->GetXaxis()->SetRangeUser(BaseAxisrange_lo, BaseAxisrange_hi);
        hFrame->GetYaxis()->SetRangeUser(ylo, yhi);
        hFrame->Draw("EP");

        if (h1 && h2) {
            h2->GetXaxis()->SetRangeUser(BaseAxisrange_lo, BaseAxisrange_hi);
            h2->Draw("EP SAME");
        }

        TLegend* leg = new TLegend(0.60, 0.75, 0.88, 0.88);
        leg->SetBorderSize(0);
        if (h1) leg->AddEntry(h1, lab1, "lp");
        if (h2) leg->AddEntry(h2, lab2, "lp");
        leg->Draw();
    };

    auto DrawTriple = [&](TH1D* hS, TH1D* hR, TH1D* hF,
                          const char* sliceLabelStr)
    {
        TH1D* hFrame = hS ? hS : (hR ? hR : hF);
        if (!hFrame) return;

        hFrame->SetTitle(sliceLabelStr);
        hFrame->GetXaxis()->SetTitle(BaseAxisLabel);
        hFrame->GetYaxis()->SetTitle(Form("Width (%s)", FitAxisLabel));
        hFrame->GetXaxis()->SetRangeUser(BaseAxisrange_lo, BaseAxisrange_hi);
        hFrame->GetYaxis()->SetRangeUser(0, fitrange_hi_sigmaFWHMRMS * 0.8);
        hFrame->Draw("EP");

        auto Overlay = [&](TH1D* h) {
            if (h && h != hFrame) {
                h->GetXaxis()->SetRangeUser(BaseAxisrange_lo, BaseAxisrange_hi);
                h->Draw("EP SAME");
            }
        };
        Overlay(hS);
        Overlay(hR);
        Overlay(hF);

        TLegend* leg = new TLegend(0.60, 0.75, 0.88, 0.88);
        leg->SetBorderSize(0);
        if (hS) leg->AddEntry(hS, "Fitted Gaussian Width", "lp");
        if (hR) leg->AddEntry(hR, "RMS",                   "lp");
        if (hF) leg->AddEntry(hF, "FWHM/(2#sqrt{2ln2})",  "lp");
        leg->Draw();
    };

    auto MakeStyledClone = [&](TH1D* src, const char* suffix,
                                Color_t col, Style_t marker, int ii) -> TH1D*
    {
        if (!src) return nullptr;
        TH1D* clone = static_cast<TH1D*>(
            src->Clone(Form("%s_%s_%d", src->GetName(), suffix, ii)));
        clone->SetDirectory(nullptr);
        clone->SetLineColor(col);
        clone->SetMarkerColor(col);
        clone->SetMarkerStyle(marker);
        return clone;
    };

    // ── Main loop over SliceAxis bin groups ─────────────────────────────────────
    for (int ii = 0; ii < nz_iter; ++ii)
    {
        const int iz_lo = ii * rebin_SliceAxis + 1;
        const int iz_hi = std::min((ii + 1) * rebin_SliceAxis, nz);

        // Use GetAxis to access the SliceAxis of the TH3D, regardless of
        // which of X/Y/Z it physically is.
        const double zlow  = GetAxis(h3_1, slice_along)->GetBinLowEdge(iz_lo);
        const double zhigh = GetAxis(h3_1, slice_along)->GetBinUpEdge(iz_hi);

        GetAxis(h3_1, slice_along)->SetRange(iz_lo, iz_hi);
        GetAxis(h3_2, slice_along)->SetRange(iz_lo, iz_hi);

        // Temporary rename to avoid ROOT name-collision warnings from Project3D.
        const TString savedName1 = h3_1->GetName();
        const TString savedName2 = h3_2->GetName();
        h3_1->SetName(Form("_psrc_sim_%d",  ii));
        h3_2->SetName(Form("_psrc_data_%d", ii));

        // Project3D: projStr built so FitAxis → TH2D-Y, BaseAxis → TH2D-X.
        TH2D* h2_1 = static_cast<TH2D*>(h3_1->Project3D(projStr.c_str()));
        TH2D* h2_2 = static_cast<TH2D*>(h3_2->Project3D(projStr.c_str()));

        h3_1->SetName(savedName1);
        h3_2->SetName(savedName2);

        h2_1->SetName(Form("h2_sim_slice%d_bin%d",  slice_along, ii));
        h2_2->SetName(Form("h2_data_slice%d_bin%d", slice_along, ii));
        h2_1->SetDirectory(nullptr);
        h2_2->SetDirectory(nullptr);

        // RebinX operates on the TH2D's X-axis, which is always the BaseAxis
        // because of how projStr is constructed.
        h2_1->RebinX(rebin_BaseAxis);
        h2_2->RebinX(rebin_BaseAxis);

        // RestrictYRange operates on the TH2D's Y-axis, which is always the
        // FitAxis because of how projStr is constructed.
        const auto savedYRange1 = RestrictYRange(h2_1, fitrange_lo_sigmaFWHMRMS, fitrange_hi_sigmaFWHMRMS);
        const auto savedYRange2 = RestrictYRange(h2_2, fitrange_lo_sigmaFWHMRMS, fitrange_hi_sigmaFWHMRMS);

        TH1D* hMean1 = nullptr, *hSigma1 = nullptr;
        TH1D* hMean2 = nullptr, *hSigma2 = nullptr;

        GetFitSlicesY(h2_1, fitfunc, hMean1, hSigma1,
                      Form("%s_sim_slice%d_bin%d",  hist3d_name, slice_along, ii),
                      BaseAxisLabel, FitAxisLabel);
        GetFitSlicesY(h2_2, fitfunc, hMean2, hSigma2,
                      Form("%s_data_slice%d_bin%d", hist3d_name, slice_along, ii),
                      BaseAxisLabel, FitAxisLabel);

        TH1D* hMode1 = nullptr, *hFWHM1 = nullptr, *hRMS1 = nullptr;
        TH1D* hMode2 = nullptr, *hFWHM2 = nullptr, *hRMS2 = nullptr;

        GetModeFWHMRMS_Y(h2_1, hMode1, hFWHM1, hRMS1, /*n_bins_comb=*/1,
                         Form("%s_sim_slice%d_bin%d",  hist3d_name, slice_along, ii),
                         BaseAxisLabel, FitAxisLabel);
        GetModeFWHMRMS_Y(h2_2, hMode2, hFWHM2, hRMS2, /*n_bins_comb=*/1,
                         Form("%s_data_slice%d_bin%d", hist3d_name, slice_along, ii),
                         BaseAxisLabel, FitAxisLabel);

        RestoreYRange(h2_1, savedYRange1);
        RestoreYRange(h2_2, savedYRange2);

        delete h2_1;
        delete h2_2;

        if (hFWHM1) hFWHM1->Scale(fwhm_to_sigma);
        if (hFWHM2) hFWHM2->Scale(fwhm_to_sigma);

        // ── Style ─────────────────────────────────────────────────────────────
        if (hMean1)  { hMean1 ->SetLineColor(kRed+1);   hMean1 ->SetMarkerColor(kRed+1);   hMean1 ->SetMarkerStyle(20); }
        if (hMean2)  { hMean2 ->SetLineColor(kBlue+1);  hMean2 ->SetMarkerColor(kBlue+1);  hMean2 ->SetMarkerStyle(24); }
        if (hSigma1) { hSigma1->SetLineColor(kRed+1);   hSigma1->SetMarkerColor(kRed+1);   hSigma1->SetMarkerStyle(20); }
        if (hSigma2) { hSigma2->SetLineColor(kBlue+1);  hSigma2->SetMarkerColor(kBlue+1);  hSigma2->SetMarkerStyle(24); }
        if (hMode1)  { hMode1 ->SetLineColor(kRed+1);   hMode1 ->SetMarkerColor(kRed+1);   hMode1 ->SetMarkerStyle(20); }
        if (hMode2)  { hMode2 ->SetLineColor(kBlue+1);  hMode2 ->SetMarkerColor(kBlue+1);  hMode2 ->SetMarkerStyle(24); }
        if (hFWHM1)  { hFWHM1 ->SetLineColor(kRed+1);   hFWHM1 ->SetMarkerColor(kRed+1);   hFWHM1 ->SetMarkerStyle(20); }
        if (hFWHM2)  { hFWHM2 ->SetLineColor(kBlue+1);  hFWHM2 ->SetMarkerColor(kBlue+1);  hFWHM2 ->SetMarkerStyle(24); }
        if (hRMS1)   { hRMS1  ->SetLineColor(kRed+1);   hRMS1  ->SetMarkerColor(kRed+1);   hRMS1  ->SetMarkerStyle(21); }
        if (hRMS2)   { hRMS2  ->SetLineColor(kBlue+1);  hRMS2  ->SetMarkerColor(kBlue+1);  hRMS2  ->SetMarkerStyle(25); }

        const TString sliceLabel = TString::Format(
            "%.3f < %s < %.3f", zlow, SliceAxisLabel, zhigh);
        const char* sl = sliceLabel.Data();

        // ── Mean canvas ───────────────────────────────────────────────────────
        cMean->cd(ii + 1);
        gPad->SetGrid();
        DrawPair(hMean1, hMean2, sl,
                 BaseAxisLabel, TString::Format("Mean (%s)", FitAxisLabel).Data(),
                 fitrange_lo_meanmode, fitrange_hi_meanmode,
                 "Simulation", "Data");

        // ── Sigma canvas ──────────────────────────────────────────────────────
        cSigma->cd(ii + 1);
        gPad->SetGrid();
        DrawPair(hSigma1, hSigma2, sl,
                 BaseAxisLabel, TString::Format("Width (%s)", FitAxisLabel).Data(),
                 0, fitrange_hi_sigmaFWHMRMS * 0.8,
                 "Simulation", "Data");

        // ── Mode canvas ───────────────────────────────────────────────────────
        cMode->cd(ii + 1);
        gPad->SetGrid();
        DrawPair(hMode1, hMode2, sl,
                 BaseAxisLabel, TString::Format("Mode (%s)", FitAxisLabel).Data(),
                 fitrange_lo_meanmode, fitrange_hi_meanmode,
                 "Simulation", "Data");

        // ── FWHM canvas ───────────────────────────────────────────────────────
        cFWHM->cd(ii + 1);
        gPad->SetGrid();
        DrawPair(hFWHM1, hFWHM2, sl,
                 BaseAxisLabel,
                 TString::Format("FWHM / (2#sqrt{2ln2}) (%s)", FitAxisLabel).Data(),
                 0, fitrange_hi_sigmaFWHMRMS * 0.8,
                 "Simulation", "Data");

        // ── RMS canvas ────────────────────────────────────────────────────────
        cRMS->cd(ii + 1);
        gPad->SetGrid();
        DrawPair(hRMS1, hRMS2, sl,
                 BaseAxisLabel, TString::Format("RMS (%s)", FitAxisLabel).Data(),
                 0, fitrange_hi_sigmaFWHMRMS * 0.8,
                 "Simulation", "Data");

        // ── Width-measure comparison canvases ─────────────────────────────────
        TH1D* hSigma1_cmp = MakeStyledClone(hSigma1, "cmp", kRed+1,   20, ii);
        TH1D* hRMS1_cmp   = MakeStyledClone(hRMS1,   "cmp", kGreen+2, 21, ii);
        TH1D* hFWHM1_cmp  = MakeStyledClone(hFWHM1,  "cmp", kBlue+1,  24, ii);
        TH1D* hSigma2_cmp = MakeStyledClone(hSigma2, "cmp", kRed+1,   20, ii);
        TH1D* hRMS2_cmp   = MakeStyledClone(hRMS2,   "cmp", kGreen+2, 21, ii);
        TH1D* hFWHM2_cmp  = MakeStyledClone(hFWHM2,  "cmp", kBlue+1,  24, ii);

        cSimCmp->cd(ii + 1);
        gPad->SetGrid();
        DrawTriple(hSigma1_cmp, hRMS1_cmp, hFWHM1_cmp, sl);

        cDataCmp->cd(ii + 1);
        gPad->SetGrid();
        DrawTriple(hSigma2_cmp, hRMS2_cmp, hFWHM2_cmp, sl);

        for (TH1D* h : {hMean1,      hMean2,
                        hSigma1,     hSigma2,
                        hMode1,      hMode2,
                        hFWHM1,      hFWHM2,
                        hRMS1,       hRMS2,
                        hSigma1_cmp, hRMS1_cmp,  hFWHM1_cmp,
                        hSigma2_cmp, hRMS2_cmp,  hFWHM2_cmp})
        {
            if (h) hist_pool.push_back(h);
        }
    }

    // ── Restore the caller's TF1 range ─────────────────────────────────────────
    fitfunc->SetRange(fitfunc_range_lo, fitfunc_range_hi);

    // ── Reset slice axis range ──────────────────────────────────────────────────
    GetAxis(h3_1, slice_along)->SetRange(0, 0);
    GetAxis(h3_2, slice_along)->SetRange(0, 0);

    // ── Save all canvases ───────────────────────────────────────────────────────
    cMean ->SaveAs(Form("%s_mean.pdf",  outputname));
    cMean ->SaveAs(Form("%s_mean.png",  outputname));
    cSigma->SaveAs(Form("%s_width.pdf", outputname));
    cSigma->SaveAs(Form("%s_width.png", outputname));
    cMode ->SaveAs(Form("%s_mode.pdf",  outputname));
    cMode ->SaveAs(Form("%s_mode.png",  outputname));
    cFWHM ->SaveAs(Form("%s_fwhm.pdf",  outputname));
    cFWHM ->SaveAs(Form("%s_fwhm.png",  outputname));
    cRMS  ->SaveAs(Form("%s_rms.pdf",   outputname));
    cRMS  ->SaveAs(Form("%s_rms.png",   outputname));
    cSimCmp ->SaveAs(Form("%s_sigma_rms_fwhm_comparison_sim.pdf",  outputname));
    cSimCmp ->SaveAs(Form("%s_sigma_rms_fwhm_comparison_sim.png",  outputname));
    cDataCmp->SaveAs(Form("%s_sigma_rms_fwhm_comparison_data.pdf", outputname));
    cDataCmp->SaveAs(Form("%s_sigma_rms_fwhm_comparison_data.png", outputname));

    // ── Cleanup ──────────────────────────────────────────────────────────────────
    delete cMean;
    delete cSigma;
    delete cMode;
    delete cFWHM;
    delete cRMS;
    delete cSimCmp;
    delete cDataCmp;
    
    delete h3_1;
    delete h3_2;
    for (TH1D* h : hist_pool) delete h;
}


void FitTH3ZSlices(
    TH3D*       h3,
    TF1*        fitFunc,
    TH2D*&      h_mean,
    TH2D*&      h_width,
    int         par_mean    = 1,
    int         par_width   = 2,
    int         min_entries = 20,
    const char* fit_opt     = "QSRN",
    int         rebin_x     = 1,
    int         rebin_y     = 1,
    int         rebin_z     = 1
)
{
    // ── Sanity checks on pointers ────────────────────────────────────────────
    if (!h3) {
        std::cerr << "[FitTH3ZSlices] ERROR: null TH3D pointer.\n"; return;
    }
    if (!fitFunc) {
        std::cerr << "[FitTH3ZSlices] ERROR: null TF1 pointer.\n";  return;
    }
    if (par_mean  < 0 || par_mean  >= fitFunc->GetNpar() ||
        par_width < 0 || par_width >= fitFunc->GetNpar()) {
        std::cerr << "[FitTH3ZSlices] ERROR: par_mean/par_width out of range "
                  << "for TF1 with " << fitFunc->GetNpar() << " parameter(s).\n";
        return;
    }

    // ── Sanity checks on rebin factors ───────────────────────────────────────
    if (rebin_x < 1 || rebin_y < 1 || rebin_z < 1) {
        std::cerr << "[FitTH3ZSlices] ERROR: rebin factors must be >= 1.\n";
        return;
    }

    const int origX = h3->GetNbinsX();
    const int origY = h3->GetNbinsY();
    const int origZ = h3->GetNbinsZ();

    if (origX % rebin_x != 0) {
        std::cerr << "[FitTH3ZSlices] ERROR: rebin_x=" << rebin_x
                  << " does not divide evenly into nBinsX=" << origX << ".\n";
        return;
    }
    if (origY % rebin_y != 0) {
        std::cerr << "[FitTH3ZSlices] ERROR: rebin_y=" << rebin_y
                  << " does not divide evenly into nBinsY=" << origY << ".\n";
        return;
    }
    if (origZ % rebin_z != 0) {
        std::cerr << "[FitTH3ZSlices] ERROR: rebin_z=" << rebin_z
                  << " does not divide evenly into nBinsZ=" << origZ << ".\n";
        return;
    }

    // ── Cause 2 guard: check that enough z-bins fall inside the TF1 range ───
    //
    //  After rebin_z, the z-bin width grows and the fit range covers fewer
    //  points.  A Gaussian needs at least ~5 bins inside the fit range to
    //  converge reliably; below that Minuit regularly fails, leaving output
    //  bins empty.
    //
    //  Original z-bin width:          zBinW_orig = (zMax - zMin) / origZ
    //  Rebinned z-bin width:          zBinW_rb   = zBinW_orig * rebin_z
    //  Bins inside TF1 range [lo,hi]: nBinsInRange = (hi - lo) / zBinW_rb
    {
        const double zMin     = h3->GetZaxis()->GetXmin();
        const double zMax     = h3->GetZaxis()->GetXmax();
        const double zBinW_rb = (zMax - zMin) / origZ * rebin_z;

        double fitLo, fitHi;
        fitFunc->GetRange(fitLo, fitHi);
        const double nBinsInRange = (fitHi - fitLo) / zBinW_rb;

        if (nBinsInRange < 5.0) {
            std::cerr << "[FitTH3ZSlices] WARNING: \"" << h3->GetName() << "\""
                      << "  rebin_z=" << rebin_z
                      << " leaves only " << nBinsInRange
                      << " z-bin(s) inside the TF1 fit range ["
                      << fitLo << ", " << fitHi << "] cm.\n"
                      << "  Fits will likely fail for most bins (empty output).\n"
                      << "  Reduce rebin_z or widen the TF1 range.\n";
            // Not a fatal error — the user may proceed and inspect the output,
            // but they are warned.
        }
    }

    // ── Rebinning (on a private clone — h3 is never modified) ────────────────
    TH3D* h3_work    = h3;
    TH3D* h3_rebinned = nullptr;

    const bool need_rebin = (rebin_x > 1 || rebin_y > 1 || rebin_z > 1);
    if (need_rebin) {
        TString cloneName = Form("%s_rb", h3->GetName());
        h3_rebinned = static_cast<TH3D*>(
            h3->Rebin3D(rebin_x, rebin_y, rebin_z, cloneName.Data()));
        h3_rebinned->SetDirectory(nullptr);
        h3_work = h3_rebinned;

//        std::cout << "[FitTH3ZSlices] \"" << h3->GetName() << "\": rebinned "
//                  << origX << "x" << origY << "x" << origZ << "  →  "
//                  << h3_work->GetNbinsX() << "x"
//                  << h3_work->GetNbinsY() << "x"
//                  << h3_work->GetNbinsZ() << " bins.\n";
    }

    // ── Axis metadata — always from h3_work so dims reflect any rebinning ────
    const int nBinsX = h3_work->GetNbinsX();
    const int nBinsY = h3_work->GetNbinsY();

    TAxis* xAxis = h3_work->GetXaxis();
    TAxis* yAxis = h3_work->GetYaxis();
    TAxis* zAxis = h3_work->GetZaxis();

    const TArrayD* xEdgeArr = xAxis->GetXbins();
    const TArrayD* yEdgeArr = yAxis->GetXbins();
    const double*  xEdges   = (xEdgeArr->GetSize() > 0) ? xEdgeArr->GetArray() : nullptr;
    const double*  yEdges   = (yEdgeArr->GetSize() > 0) ? yEdgeArr->GetArray() : nullptr;

    // ── Helper: build a TH2D with the (post-rebin) x-y binning ──────────────
    auto MakeOutputTH2 = [&](const char* suffix, const char* zTitle) -> TH2D*
    {
        TString name = need_rebin
            ? Form("%s_%s", h3->GetName(), suffix)
            : Form("%s_%s",           h3->GetName(), suffix);
        TString title = Form("%s %s;%s;%s;%s",
                             h3->GetName(), suffix,
                             xAxis->GetTitle(), yAxis->GetTitle(), zTitle);
        TH2D* h = nullptr;
        if      (xEdges && yEdges)
            h = new TH2D(name, title, nBinsX, xEdges,  nBinsY, yEdges);
        else if (xEdges)
            h = new TH2D(name, title, nBinsX, xEdges,
                                      nBinsY, yAxis->GetXmin(), yAxis->GetXmax());
        else if (yEdges)
            h = new TH2D(name, title, nBinsX, xAxis->GetXmin(), xAxis->GetXmax(),
                                      nBinsY, yEdges);
        else
            h = new TH2D(name, title, nBinsX, xAxis->GetXmin(), xAxis->GetXmax(),
                                      nBinsY, yAxis->GetXmin(), yAxis->GetXmax());
        return h;
    };

    const char* zVarName = zAxis->GetTitle();
    h_mean  = MakeOutputTH2("FitMean",  Form("Fitted mean of %s",  zVarName));
    h_width = MakeOutputTH2("FitWidth", Form("Fitted width of %s", zVarName));

    // ── Main loop over (ix, iy) bins ─────────────────────────────────────────
    int n_ok      = 0;
    int n_fail    = 0;
    int n_skipped = 0;

    for (int ix = 1; ix <= nBinsX; ++ix)
    {
        for (int iy = 1; iy <= nBinsY; ++iy)
        {
            TString sliceName = Form("_zslice_%s_%d_%d", h3_work->GetName(), ix, iy);
            TH1D* hslice = static_cast<TH1D*>(
                h3_work->ProjectionZ(sliceName, ix, ix, iy, iy));
            hslice->SetDirectory(nullptr);

            hslice->Sumw2();                          // 1b: recompute errors
            const double nInSlice = hslice->Integral(); // 1a: count from contents

            if (nInSlice < static_cast<double>(min_entries))
            {
                ++n_skipped;
                delete hslice;
                continue;
            }

            TF1* fitClone = static_cast<TF1*>(
                fitFunc->Clone(Form("_fitclone_%s_%d_%d",
                                    h3_work->GetName(), ix, iy)));

            TFitResultPtr result = hslice->Fit(fitClone, fit_opt);

            const bool fit_valid = result.Get() && result->IsValid();
            if (fit_valid)
            {
                const double mean      =  fitClone->GetParameter(par_mean);
                const double mean_err  =  fitClone->GetParError (par_mean);
                const double width     = std::fabs(fitClone->GetParameter(par_width));
                const double width_err =           fitClone->GetParError (par_width);

                h_mean->SetBinContent(ix, iy, mean);
                h_mean->SetBinError  (ix, iy, mean_err);

                h_width->SetBinContent(ix, iy, width);
                h_width->SetBinError  (ix, iy, width_err);

                ++n_ok;
            }
            else
            {
                ++n_fail;
            }

            delete fitClone;
            delete hslice;
        }
    }

    delete h3_rebinned; // no-op when nullptr

//    std::cout << "[FitTH3ZSlices] \"" << h3->GetName() << "\""
//              << (need_rebin
//                  ? Form(" (rebinned %dx%dx%d)", rebin_x, rebin_y, rebin_z) : "")
//              << "  fit OK: "   << n_ok
//              << "  failed: "   << n_fail
//              << "  skipped (<" << min_entries << "): " << n_skipped
//              << "  (total: "   << nBinsX * nBinsY << " bins)\n";
}

void FitTH3ZSlices_GetFWHMRMS(
    TH3D*  h3,
    TH2D*& h_mode,
    TH2D*& h_fwhm,
    TH2D*& h_rms,
    int    min_entries = 20,
    int    rebin_x     = 1,
    int    rebin_y     = 1,
    int    rebin_z     = 1
)
{
    h_mode = nullptr;
    h_fwhm = nullptr;
    h_rms  = nullptr;
 
    // ── Sanity checks ────────────────────────────────────────────────────────
    if (!h3) {
        std::cerr << "[FitTH3ZSlices_GetFWHM] ERROR: null TH3D pointer.\n";
        return;
    }
    if (rebin_x < 1 || rebin_y < 1 || rebin_z < 1) {
        std::cerr << "[FitTH3ZSlices_GetFWHM] ERROR: rebin factors must be >= 1.\n";
        return;
    }
 
    const int origX = h3->GetNbinsX();
    const int origY = h3->GetNbinsY();
    const int origZ = h3->GetNbinsZ();
 
    if (origX % rebin_x != 0) {
        std::cerr << "[FitTH3ZSlices_GetFWHM] ERROR: rebin_x=" << rebin_x
                  << " does not divide evenly into nBinsX=" << origX << ".\n";
        return;
    }
    if (origY % rebin_y != 0) {
        std::cerr << "[FitTH3ZSlices_GetFWHM] ERROR: rebin_y=" << rebin_y
                  << " does not divide evenly into nBinsY=" << origY << ".\n";
        return;
    }
    if (origZ % rebin_z != 0) {
        std::cerr << "[FitTH3ZSlices_GetFWHM] ERROR: rebin_z=" << rebin_z
                  << " does not divide evenly into nBinsZ=" << origZ << ".\n";
        return;
    }
 
    // ── Rebin onto a private clone (h3 is never modified) ────────────────────
    TH3D* h3_work     = h3;
    TH3D* h3_rebinned = nullptr;
 
    const bool need_rebin = (rebin_x > 1 || rebin_y > 1 || rebin_z > 1);
    if (need_rebin) {
        TString cloneName = Form("%s_rb", h3->GetName());
        h3_rebinned = static_cast<TH3D*>(
            h3->Rebin3D(rebin_x, rebin_y, rebin_z, cloneName.Data()));
        h3_rebinned->SetDirectory(nullptr);
        h3_work = h3_rebinned;
    }
 
    // ── Axis metadata (from the post-rebin histogram) ────────────────────────
    const int nBinsX = h3_work->GetNbinsX();
    const int nBinsY = h3_work->GetNbinsY();
 
    TAxis* xAxis = h3_work->GetXaxis();
    TAxis* yAxis = h3_work->GetYaxis();
    TAxis* zAxis = h3_work->GetZaxis();
 
    const TArrayD* xEdgeArr = xAxis->GetXbins();
    const TArrayD* yEdgeArr = yAxis->GetXbins();
    const double*  xEdges   = (xEdgeArr->GetSize() > 0) ? xEdgeArr->GetArray() : nullptr;
    const double*  yEdges   = (yEdgeArr->GetSize() > 0) ? yEdgeArr->GetArray() : nullptr;
 
    // ── Helper: build a TH2D with the (post-rebin) x-y binning ──────────────
    auto MakeOutputTH2 = [&](const char* suffix, const char* zTitle) -> TH2D*
    {
        TString name  = Form("%s_%s", h3->GetName(), suffix);
        TString title = Form("%s %s;%s;%s;%s",
                             h3->GetName(), suffix,
                             xAxis->GetTitle(), yAxis->GetTitle(), zTitle);
        TH2D* h = nullptr;
        if      (xEdges && yEdges)
            h = new TH2D(name, title, nBinsX, xEdges,  nBinsY, yEdges);
        else if (xEdges)
            h = new TH2D(name, title, nBinsX, xEdges,
                                      nBinsY, yAxis->GetXmin(), yAxis->GetXmax());
        else if (yEdges)
            h = new TH2D(name, title, nBinsX, xAxis->GetXmin(), xAxis->GetXmax(),
                                      nBinsY, yEdges);
        else
            h = new TH2D(name, title, nBinsX, xAxis->GetXmin(), xAxis->GetXmax(),
                                      nBinsY, yAxis->GetXmin(), yAxis->GetXmax());
        return h;
    };
 
    const char* zVarName = zAxis->GetTitle();
    h_mode = MakeOutputTH2("Mode", Form("Mode of %s",  zVarName));
    h_fwhm = MakeOutputTH2("FWHM", Form("FWHM of %s",  zVarName));
    h_rms  = MakeOutputTH2("RMS",  Form("RMS of %s",   zVarName));
 
    // ── Main loop over (ix, iy) bins ─────────────────────────────────────────
    int n_ok      = 0;
    int n_fail    = 0;
    int n_skipped = 0;
 
    for (int ix = 1; ix <= nBinsX; ++ix)
    {
        for (int iy = 1; iy <= nBinsY; ++iy)
        {
            TString sliceName = Form("_zslice_%s_%d_%d", h3_work->GetName(), ix, iy);
            TH1D* hslice = static_cast<TH1D*>(
                h3_work->ProjectionZ(sliceName, ix, ix, iy, iy));
            hslice->SetDirectory(nullptr);
 
            // Recompute errors from bin contents
            hslice->Sumw2();
            const double nInSlice = hslice->Integral();
 
            if (nInSlice < static_cast<double>(min_entries))
            {
                ++n_skipped;
                delete hslice;
                continue;
            }
 
            // ── Weighted sums for RMS ────────────────────────────────────────
            double sumw = 0.0, sumwy = 0.0, sumwy2 = 0.0, sumw2 = 0.0;
            for (int jj = 1; jj <= hslice->GetNbinsX(); ++jj)
            {
                const double bc = hslice->GetBinContent(jj);
                const double bx = hslice->GetBinCenter(jj);
                const double be = hslice->GetBinError(jj);
                sumw   += bc;
                sumwy  += bc * bx;
                sumwy2 += bc * bx * bx;
                sumw2  += be * be;
            }

            // Effective number of entries (accounts for weighted fills)
            const double n_eff = (sumw > 0.0 && sumw2 > 0.0)
                                 ? (sumw * sumw) / sumw2
                                 : sumw;

            // ── RMS ──────────────────────────────────────────────────────────
            double rms = 0.0, rms_err = 0.0;
            if (sumw > 0.0)
            {
                const double mean     = sumwy / sumw;
                const double variance = sumwy2 / sumw - mean * mean;
                rms = (variance > 0.0) ? std::sqrt(variance) : 0.0;
                // sigma(RMS) ≈ RMS / sqrt(2*(n_eff-1))
                rms_err = (n_eff > 1.0) ? rms / std::sqrt(2.0 * (n_eff - 1.0)) : 0.0;
            }
            
            h_rms ->SetBinContent(ix, iy, rms);
            h_rms ->SetBinError  (ix, iy, rms_err);
            
            // ── Mode ─────────────────────────────────────────────────────────
            const int    maxBin = hslice->GetMaximumBin();
            const double mode   = hslice->GetBinCenter(maxBin);
            const double modeErr = hslice->GetBinWidth(maxBin) / 2.0;
            
            h_mode->SetBinContent(ix, iy, mode);
            h_mode->SetBinError  (ix, iy, modeErr);
            
            // ── FWHM ─────────────────────────────────────────────────────────
            const double half = 0.5 * hslice->GetMaximum();
            const int    bin1 = hslice->FindFirstBinAbove(half);
            const int    bin2 = hslice->FindLastBinAbove(half);
 
            // Guard: if no bin is found above half-maximum, treat as failure
            if (bin1 <= 0 || bin2 <= 0)
            {
                ++n_fail;
                delete hslice;
                continue;
            }
 
            const double fwhm    = hslice->GetBinCenter(bin2) - hslice->GetBinCenter(bin1);
            const double fwhmErr = 0.5 * std::sqrt(
                                       std::pow(hslice->GetBinWidth(bin1), 2.0) +
                                       std::pow(hslice->GetBinWidth(bin2), 2.0));
 
            h_fwhm->SetBinContent(ix, iy, fwhm);
            h_fwhm->SetBinError  (ix, iy, fwhmErr);
 
 
            ++n_ok;
            delete hslice;
        }
    }
 
    delete h3_rebinned; // no-op when nullptr
 
//    std::cout << "[FitTH3ZSlices_GetFWHM] \"" << h3->GetName() << "\""
//              << (need_rebin
//                  ? Form(" (rebinned %dx%dx%d)", rebin_x, rebin_y, rebin_z) : "")
//              << "  ok: "      << n_ok
//              << "  failed: "  << n_fail
//              << "  skipped (<" << min_entries << "): " << n_skipped
//              << "  (total: "  << nBinsX * nBinsY << " bins)\n";
}
