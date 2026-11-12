#include <TChain.h>
#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <Math/Vector3D.h>

#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

void Create_trackpairstree(const bool is_simulation_input=false)
{
  char outputrootfilename[200]="";
  TChain* resid_base = new TChain("residualtree");
  char* description="pgthanp8_dcaxyandxlessthanp1";
    
  if(is_simulation_input){
        snprintf(outputrootfilename, sizeof(outputrootfilename),"./Output/21May26/track_pairs_out_%s_Simulation.root",description);
        
        resid_base->Add("./output_Pythia_Simulation/SoftQCD_nonDiffractive/residuals_G4sPHENIX_minbiasPythia_200kevts_set*.root");
      
//        resid_base->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_105kevts_withtruedca.root");
//        resid_base->Add("output_Pythia_withPID/residuals_G4sPHENIX_Pythia_175kevts_withtruedca*.root");
//        resid_base->Add("residuals_G4sPHENIX_1.root");
  }
  else{
      snprintf(outputrootfilename, sizeof(outputrootfilename),"Output/19May26/track_pairs_out_%s_data.root",description);
      char name[200];
      for (int i = 0; i < 999; ++i)
      {
          //snprintf(name,sizeof(name), "/sphenix/tg/tg01/hf/gregoryottino/silicon_tpc_matching/cluster_seeds_all_79516-%i.root_resid.root",i);
          snprintf(name,sizeof(name), "/sphenix/tg/tg01/hf/gregoryottino/silicon_tpc_matching/output_baseAlignment/cluster_seeds_all_79516-%i.root_resid.root",i);
          std::cout << "Adding " << name << std::endl;
          resid_base->Add(name);
      }
  }
    
  if (resid_base->GetEntries() == 0)
  {
    std::cerr << "No entries found in residualtree." << std::endl;
    return;
  }

  resid_base->LoadTree(0);
  TTree* first_tree = resid_base->GetTree();

//  const bool is_simulation_input =
//    first_tree &&
//    first_tree->GetBranch("true_vx") && first_tree->GetBranch("true_vy") && first_tree->GetBranch("true_vz");

  TTreeReader reader(resid_base);
  TTreeReaderValue<Float_t> pcax(reader, "pcax");
  TTreeReaderValue<Float_t> pcay(reader, "pcay");
  TTreeReaderValue<Float_t> pcaz(reader, "pcaz");
  TTreeReaderValue<Float_t> dcaxy(reader, "dcaxy");
  TTreeReaderValue<Float_t> dcaz(reader, "dcaz");
  TTreeReaderValue<Float_t> px(reader, "px");
  TTreeReaderValue<Float_t> py(reader, "py");
  TTreeReaderValue<Float_t> pz(reader, "pz");
  TTreeReaderValue<Float_t> pt(reader, "pt");
  TTreeReaderValue<Float_t> eta(reader, "eta");
  TTreeReaderValue<Float_t> phi(reader, "phi");
  TTreeReaderValue<Int_t> charge(reader, "charge");
  TTreeReaderValue<Float_t> quality(reader, "quality");
  TTreeReaderValue<Int_t> nhits(reader, "nhits");
  TTreeReaderValue<Int_t> nmaps(reader, "nmaps");
  TTreeReaderValue<Int_t> nintt(reader, "nintt");
  TTreeReaderValue<Int_t> ntpc(reader, "ntpc");
  TTreeReaderValue<Int_t> nmms(reader, "nmms");
  TTreeReaderValue<Int_t> vertex_ntracks(reader, "vertex_ntracks");
  TTreeReaderValue<Int_t> run(reader, "run");
  TTreeReaderValue<Int_t> segment(reader, "segment");
  TTreeReaderValue<Int_t> event(reader, "event");
  TTreeReaderValue<Int_t> crossing(reader, "crossing");
  TTreeReaderValue<Float_t> vx(reader, "vx");
  TTreeReaderValue<Float_t> vy(reader, "vy");
  TTreeReaderValue<Float_t> vz(reader, "vz");
  TTreeReaderValue<UInt_t> vertexid(reader, "vertexid");

  TTreeReaderValue<Float_t>* true_vx = nullptr;
  TTreeReaderValue<Float_t>* true_vy = nullptr;
  TTreeReaderValue<Float_t>* true_vz = nullptr;
  TTreeReaderValue<Float_t>* true_px = nullptr;
  TTreeReaderValue<Float_t>* true_py = nullptr;
  TTreeReaderValue<Float_t>* true_pz = nullptr;
  TTreeReaderValue<Float_t>* true_dcaxy_in = nullptr;
  TTreeReaderValue<Float_t>* true_dcaz_in  = nullptr;
  if (is_simulation_input)
  {
    true_vx      = new TTreeReaderValue<Float_t>(reader, "true_vx");
    true_vy      = new TTreeReaderValue<Float_t>(reader, "true_vy");
    true_vz      = new TTreeReaderValue<Float_t>(reader, "true_vz");
    true_dcaxy_in = new TTreeReaderValue<Float_t>(reader, "true_dcaxy");
    true_dcaz_in  = new TTreeReaderValue<Float_t>(reader, "true_dcaz");
  }
  if (is_simulation_input)
  {
    true_px = new TTreeReaderValue<Float_t>(reader, "true_px");
    true_py = new TTreeReaderValue<Float_t>(reader, "true_py");
    true_pz = new TTreeReaderValue<Float_t>(reader, "true_pz");
  }

  TFile* outputFile = TFile::Open(outputrootfilename, "RECREATE");
  if (!outputFile || outputFile->IsZombie())
  {
    std::cerr << "Could not create " << outputrootfilename << std::endl;
    delete true_vx;
    delete true_vy;
    delete true_vz;
    delete true_px;
    delete true_py;
    delete true_pz;
    delete true_dcaxy_in;
    delete true_dcaz_in;
    return;
  }

  TTree* track_pairs = new TTree("track_pairs", "Selected same-event same-crossing track pairs");

  static const float kNaN = std::numeric_limits<float>::quiet_NaN();
  Int_t out_run = 0;
  Int_t out_segment = 0;
  Int_t out_event = 0;
  Int_t out_crossing = 0;
  Float_t eta_first = 0;
  Float_t eta_second = 0;
  Float_t pt_first = 0;
  Float_t pt_second = 0;
  Float_t p_first = 0;
  Float_t p_second = 0;
  Float_t phi_first = 0;
  Float_t phi_second = 0;
  Int_t charge_first = -99;
  Int_t charge_second = -99;
  Float_t dcaxy_first = 0;
  Float_t dcaxy_second = 0;
  Float_t dcaz_first = 0;
  Float_t dcaz_second = 0;
  Float_t quality_first = 0;
  Float_t quality_second = 0;
  Int_t nhits_first = 0;
  Int_t nhits_second = 0;
  Int_t nmaps_first = 0;
  Int_t nmaps_second = 0;
  Int_t nintt_first = 0;
  Int_t nintt_second = 0;
  Int_t ntpc_first = 0;
  Int_t ntpc_second = 0;
  Int_t nmms_first = 0;
  Int_t nmms_second = 0;
  Float_t true_px_first = std::numeric_limits<float>::quiet_NaN();
  Float_t true_px_second = std::numeric_limits<float>::quiet_NaN();
  Float_t true_py_first = std::numeric_limits<float>::quiet_NaN();
  Float_t true_py_second = std::numeric_limits<float>::quiet_NaN();
  Float_t true_pz_first = std::numeric_limits<float>::quiet_NaN();
  Float_t true_pz_second = std::numeric_limits<float>::quiet_NaN();
  Float_t true_dcaxy_first  = std::numeric_limits<float>::quiet_NaN();
  Float_t true_dcaz_first   = std::numeric_limits<float>::quiet_NaN();
  Float_t true_dcaxy_second = std::numeric_limits<float>::quiet_NaN();
  Float_t true_dcaz_second  = std::numeric_limits<float>::quiet_NaN();
  Float_t two_track_dcaxy = 0;
  Float_t two_track_dcaz = 0;
  UInt_t vertexid_first = 0;
  UInt_t vertexid_second = 0;
  Int_t vertex_ntracks_first = 0;
  Int_t vertex_ntracks_second = 0;
  Int_t vertex_wocuts_ntracks_first = 0;
  Int_t vertex_wocuts_ntracks_second = 0;
  Int_t vertex_selected_ntracks_first = 0;
  Int_t vertex_selected_ntracks_second = 0;
  Bool_t same_vertex = false;
  Bool_t is_simulation = false;
  ROOT::Math::XYZVectorF pca_first;
  ROOT::Math::XYZVectorF pca_second;
  ROOT::Math::XYZVectorF two_track_midpoint;
  ROOT::Math::XYZVectorF vertex_first;
  ROOT::Math::XYZVectorF vertex_second;
  ROOT::Math::XYZVectorF truth_vertex_first(kNaN, kNaN, kNaN);
  ROOT::Math::XYZVectorF truth_vertex_second(kNaN, kNaN, kNaN);

  track_pairs->Branch("run", &out_run, "run/I");
  track_pairs->Branch("segment", &out_segment, "segment/I");
  track_pairs->Branch("event", &out_event, "event/I");
  track_pairs->Branch("crossing", &out_crossing, "crossing/I");
  track_pairs->Branch("eta_first", &eta_first, "eta_first/F");
  track_pairs->Branch("eta_second", &eta_second, "eta_second/F");
  track_pairs->Branch("pt_first", &pt_first, "pt_first/F");
  track_pairs->Branch("pt_second", &pt_second, "pt_second/F");
  track_pairs->Branch("p_first", &p_first, "p_first/F");
  track_pairs->Branch("p_second", &p_second, "p_second/F");
  track_pairs->Branch("phi_first", &phi_first, "phi_first/F");
  track_pairs->Branch("phi_second", &phi_second, "phi_second/F");
  track_pairs->Branch("charge_first", &charge_first, "charge_first/I");
  track_pairs->Branch("charge_second", &charge_second, "charge_second/I");
  track_pairs->Branch("dcaxy_first", &dcaxy_first, "dcaxy_first/F");
  track_pairs->Branch("dcaxy_second", &dcaxy_second, "dcaxy_second/F");
  track_pairs->Branch("dcaz_first", &dcaz_first, "dcaz_first/F");
  track_pairs->Branch("dcaz_second", &dcaz_second, "dcaz_second/F");
  track_pairs->Branch("quality_first", &quality_first, "quality_first/F");
  track_pairs->Branch("quality_second", &quality_second, "quality_second/F");
  track_pairs->Branch("nhits_first", &nhits_first, "nhits_first/I");
  track_pairs->Branch("nhits_second", &nhits_second, "nhits_second/I");
  track_pairs->Branch("nmaps_first", &nmaps_first, "nmaps_first/I");
  track_pairs->Branch("nmaps_second", &nmaps_second, "nmaps_second/I");
  track_pairs->Branch("nintt_first", &nintt_first, "nintt_first/I");
  track_pairs->Branch("nintt_second", &nintt_second, "nintt_second/I");
  track_pairs->Branch("ntpc_first", &ntpc_first, "ntpc_first/I");
  track_pairs->Branch("ntpc_second", &ntpc_second, "ntpc_second/I");
  track_pairs->Branch("nmms_first", &nmms_first, "nmms_first/I");
  track_pairs->Branch("nmms_second", &nmms_second, "nmms_second/I");
  track_pairs->Branch("true_px_first", &true_px_first, "true_px_first/F");
  track_pairs->Branch("true_px_second", &true_px_second, "true_px_second/F");
  track_pairs->Branch("true_py_first", &true_py_first, "true_py_first/F");
  track_pairs->Branch("true_py_second", &true_py_second, "true_py_second/F");
  track_pairs->Branch("true_pz_first", &true_pz_first, "true_pz_first/F");
  track_pairs->Branch("true_pz_second", &true_pz_second, "true_pz_second/F");
  track_pairs->Branch("true_dcaxy_first",  &true_dcaxy_first,  "true_dcaxy_first/F");
  track_pairs->Branch("true_dcaz_first",   &true_dcaz_first,   "true_dcaz_first/F");
  track_pairs->Branch("true_dcaxy_second", &true_dcaxy_second, "true_dcaxy_second/F");
  track_pairs->Branch("true_dcaz_second",  &true_dcaz_second,  "true_dcaz_second/F");
  track_pairs->Branch("pca_first", &pca_first);
  track_pairs->Branch("pca_second", &pca_second);
  track_pairs->Branch("two_track_dcaxy", &two_track_dcaxy, "two_track_dcaxy/F");
  track_pairs->Branch("two_track_dcaz", &two_track_dcaz, "two_track_dcaz/F");
  track_pairs->Branch("two_track_midpoint", &two_track_midpoint);
  track_pairs->Branch("vertexid_first", &vertexid_first, "vertexid_first/i");
  track_pairs->Branch("vertexid_second", &vertexid_second, "vertexid_second/i");
  track_pairs->Branch("same_vertex", &same_vertex, "same_vertex/O");
  track_pairs->Branch("vertex_first", &vertex_first);
  track_pairs->Branch("vertex_second", &vertex_second);
  track_pairs->Branch("vertex_ntracks_first", &vertex_ntracks_first, "vertex_ntracks_first/I");
  track_pairs->Branch("vertex_ntracks_second", &vertex_ntracks_second, "vertex_ntracks_second/I");
  track_pairs->Branch("vertex_wocuts_ntracks_first", &vertex_wocuts_ntracks_first, "vertex_wocuts_ntracks_first/I");
  track_pairs->Branch("vertex_wocuts_ntracks_second", &vertex_wocuts_ntracks_second, "vertex_wocuts_ntracks_second/I");
  track_pairs->Branch("vertex_selected_ntracks_first", &vertex_selected_ntracks_first, "vertex_selected_ntracks_first/I");
  track_pairs->Branch("vertex_selected_ntracks_second", &vertex_selected_ntracks_second, "vertex_selected_ntracks_second/I");
  track_pairs->Branch("is_simulation", &is_simulation, "is_simulation/O");
  track_pairs->Branch("truth_vertex_first", &truth_vertex_first);
  track_pairs->Branch("truth_vertex_second", &truth_vertex_second);

  struct VertexInfo
  {
    ROOT::Math::XYZVectorF reco_vertex;
      ROOT::Math::XYZVectorF truth_vertex{std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::quiet_NaN()};
    int associated_ntracks = 0;
    int wocuts_ntracks = 0;
    int selected_ntracks = 0;
  };

  struct TrackInfo
  {
    ROOT::Math::XYZVectorF pca;
    ROOT::Math::XYZVectorF momentum;
    ROOT::Math::XYZVectorF reco_vertex;
      ROOT::Math::XYZVectorF truth_vertex{std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::quiet_NaN()};
    float eta = 0;
    float phi = 0;
    float pt = 0;
    float p = 0;
    int charge = -99;
    float dcaxy = 0;
    float dcaz = 0;
    float true_dcaxy = std::numeric_limits<float>::quiet_NaN();
    float true_dcaz  = std::numeric_limits<float>::quiet_NaN();
    float quality = 0;
    int nhits = 0;
    int nmaps = 0;
    int nintt = 0;
    int ntpc = 0;
    int nmms = 0;
    float true_px = std::numeric_limits<float>::quiet_NaN();
    float true_py = std::numeric_limits<float>::quiet_NaN();
    float true_pz = std::numeric_limits<float>::quiet_NaN();
    unsigned int vertexid = 0;
    bool is_simulation = false;
  };

  bool have_event = false;
  int run_cur = -1;
  int segment_cur = -1;
  int event_cur = -1;

  std::map<int, std::vector<TrackInfo>> tracks_by_crossing;
  std::map<unsigned int, VertexInfo> vertex_map;

  // ── Cut functions ────────────────────────────────────────────────────────
  // Each returns true if the input PASSES (should be kept), false to reject.
  // Edit the thresholds here; the call sites below do not need to change.

  // Event-level: checked per track row using raw vertex quantities before
  // the track is counted or added to any pool.
  auto EventLevelCuts = [](float ev_vz, int ev_vertex_ntracks) -> bool
  {
//    if (std::abs(ev_vz)      > 10.0f) return false;  // |vz| < 10 cm
//    if (ev_vertex_ntracks    < 2)     return false;  // min tracks at vertex
    return true;
  };

  // Track-level: checked after TrackInfo is built.
  // Tracks that fail are NOT added to the crossing pool (not paired).
  // wocuts_ntracks is incremented before this check; selected_ntracks after.
  auto TrackLevelCuts = [](const TrackInfo& t) -> bool
  {
    if (t.p              <= 0.8)  return false;  // min p (GeV/c)
//    if (t.pt              < 0.5)  return false;  // min pT (GeV/c)
//    if (std::abs(t.eta)   > 1.1)  return false;  // SVTX eta acceptance
//    if (t.nmaps           < 2)     return false;  // min MVTX clusters
//    if (t.nintt           < 1)     return false;  // min INTT clusters
//    if (t.ntpc            < 20)    return false;  // min TPC clusters
//    if (t.quality         > 10.0) return false;  // max track quality
//    if (std::abs(t.dcaxy) > 0.1)  return false;  // max |DCAxy| (cm)
//    if (std::abs(t.dcaz)  > 0.2)  return false;  // max |DCAz|  (cm)
    return true;
  };

  // Pair-level: checked after the two-track DCA is computed.
  // Pairs that fail are NOT written to the output tree.
  auto PairLevelCuts = [](const TrackInfo& first, const TrackInfo& second,
                           float pair_dcaxy, float pair_dcaz) -> bool
  {
    if (std::abs(pair_dcaxy) >= 0.1f) return false;  // max two-track DCAxy (cm)
    if (std::abs(pair_dcaz)  >= 0.1f) return false;  // max two-track DCAz  (cm)
//    (void)first; (void)second;  // available for additional cuts, e.g. delta-phi
    return true;
  };

  auto FillPairsForEvent = [&]() -> void
  {
    //cout<<"Filling for event "<<event_cur<<endl;
    if (!have_event)
    {
      return;
    }

    for (const auto& crossing_entry : tracks_by_crossing)
    {
      const int crossing_key = crossing_entry.first;
      const auto& tracks = crossing_entry.second;
      if (tracks.size() < 2)
      {
        continue;
      }

      for (size_t ii = 0; ii < tracks.size() - 1; ++ii)
      {
        const TrackInfo& first = tracks.at(ii);
        for (size_t jj = ii + 1; jj < tracks.size(); ++jj)
        {
          const TrackInfo& second = tracks.at(jj);

          ROOT::Math::XYZVectorF crossVec = first.momentum.Cross(second.momentum);
          const float crossMag = crossVec.R();
          if (crossMag < 1e-6)
          {
            continue;
          }

          ROOT::Math::XYZVectorF dirCrossDirUnit = crossVec * (1.0f / crossMag);
          ROOT::Math::XYZVectorF trackdiffxy(second.pca.X() - first.pca.X(), second.pca.Y() - first.pca.Y(), 0.0);
          ROOT::Math::XYZVectorF trackdiffz(0.0, 0.0, second.pca.Z() - first.pca.Z());

          const float pair_dcaxy = dirCrossDirUnit.Dot(trackdiffxy);
          const float pair_dcaz  = dirCrossDirUnit.Dot(trackdiffz);
          if (!PairLevelCuts(first, second, pair_dcaxy, pair_dcaz)) { continue; }

          out_run = run_cur;
          out_segment = segment_cur;
          out_event = event_cur;
          out_crossing = crossing_key;
          eta_first = first.eta;
          eta_second = second.eta;
          pt_first = first.pt;
          pt_second = second.pt;
          p_first = first.p;
          p_second = second.p;
          phi_first = first.phi;
          phi_second = second.phi;
          charge_first = first.charge;
          charge_second = second.charge;
          dcaxy_first = first.dcaxy;
          dcaxy_second = second.dcaxy;
          dcaz_first = first.dcaz;
          dcaz_second = second.dcaz;
          true_dcaxy_first  = first.true_dcaxy;
          true_dcaz_first   = first.true_dcaz;
          true_dcaxy_second = second.true_dcaxy;
          true_dcaz_second  = second.true_dcaz;
          quality_first = first.quality;
          quality_second = second.quality;
          nhits_first = first.nhits;
          nhits_second = second.nhits;
          nmaps_first = first.nmaps;
          nmaps_second = second.nmaps;
          nintt_first = first.nintt;
          nintt_second = second.nintt;
          ntpc_first = first.ntpc;
          ntpc_second = second.ntpc;
          nmms_first = first.nmms;
          nmms_second = second.nmms;
          true_px_first = first.true_px;
          true_px_second = second.true_px;
          true_py_first = first.true_py;
          true_py_second = second.true_py;
          true_pz_first = first.true_pz;
          true_pz_second = second.true_pz;
          pca_first = first.pca;
          pca_second = second.pca;
          two_track_dcaxy = pair_dcaxy;
          two_track_dcaz  = pair_dcaz;
          two_track_midpoint = (pca_first + pca_second) * 0.5f;
          vertexid_first = first.vertexid;
          vertexid_second = second.vertexid;
          same_vertex = (first.vertexid == second.vertexid);

          const auto vertex_first_it = vertex_map.find(first.vertexid);
          const auto vertex_second_it = vertex_map.find(second.vertexid);
          if (vertex_first_it != vertex_map.end())
          {
            const VertexInfo& vtx_info = vertex_first_it->second;
            vertex_first = vtx_info.reco_vertex;
            truth_vertex_first = vtx_info.truth_vertex;
            vertex_ntracks_first = vtx_info.associated_ntracks;
            vertex_wocuts_ntracks_first = vtx_info.wocuts_ntracks;
            vertex_selected_ntracks_first = vtx_info.selected_ntracks;
          }
          else
          {
            vertex_first = first.reco_vertex;
            truth_vertex_first = first.truth_vertex;
            vertex_ntracks_first = 0;
            vertex_wocuts_ntracks_first = 0;
            vertex_selected_ntracks_first = 0;
          }

          if (vertex_second_it != vertex_map.end())
          {
            const VertexInfo& vtx_info = vertex_second_it->second;
            vertex_second = vtx_info.reco_vertex;
            truth_vertex_second = vtx_info.truth_vertex;
            vertex_ntracks_second = vtx_info.associated_ntracks;
            vertex_wocuts_ntracks_second = vtx_info.wocuts_ntracks;
            vertex_selected_ntracks_second = vtx_info.selected_ntracks;
          }
          else
          {
            vertex_second = second.reco_vertex;
            truth_vertex_second = second.truth_vertex;
            vertex_ntracks_second = 0;
            vertex_wocuts_ntracks_second = 0;
            vertex_selected_ntracks_second = 0;
          }

          is_simulation = first.is_simulation && second.is_simulation;
          track_pairs->Fill();
        }
      }
    }

    tracks_by_crossing.clear();
    vertex_map.clear();
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
    else if (run_cur != *run || segment_cur != *segment || event_cur != *event)
    {
      FillPairsForEvent();
      run_cur = *run;
      segment_cur = *segment;
      event_cur = *event;
      have_event = true;
    }

    // Event-level cut — uses raw reader values before any vertex/track counting
    if (!EventLevelCuts(*vz, *vertex_ntracks)) { continue; }

    ROOT::Math::XYZVectorF truth_vertex(kNaN, kNaN, kNaN);
    if (is_simulation_input)
    {
      truth_vertex = ROOT::Math::XYZVectorF(**true_vx, **true_vy, **true_vz);
    }

    auto vertex_entry = vertex_map.emplace(
      *vertexid,
      VertexInfo{ROOT::Math::XYZVectorF(*vx, *vy, *vz), truth_vertex, *vertex_ntracks, 0, 0}
    );
    auto vertex_it = vertex_entry.first;
    vertex_it->second.wocuts_ntracks++;  // count all tracks passing EventLevelCuts

    ROOT::Math::XYZVectorF momentum(*px, *py, *pz);
    TrackInfo track;
    track.pca = ROOT::Math::XYZVectorF(*pcax, *pcay, *pcaz);
    track.momentum = momentum;
    track.reco_vertex = vertex_it->second.reco_vertex;
    track.truth_vertex = truth_vertex;
    track.eta = *eta;
    track.phi = *phi;
    track.pt = *pt;
    track.p = std::sqrt(momentum.Dot(momentum));
    track.charge = *charge;
    track.dcaxy = *dcaxy;
    track.dcaz = *dcaz;
    if (is_simulation_input)
    {
      track.true_dcaxy = **true_dcaxy_in;
      track.true_dcaz  = **true_dcaz_in;
    }
    track.quality = *quality;
    track.nhits = *nhits;
    track.nmaps = *nmaps;
    track.nintt = *nintt;
    track.ntpc = *ntpc;
    track.nmms = *nmms;
    if (is_simulation_input)
    {
      track.true_px = **true_px;
      track.true_py = **true_py;
      track.true_pz = **true_pz;
    }
    track.vertexid = *vertexid;
    track.is_simulation = is_simulation_input;

    // Track-level cut — only selected tracks enter the pairing pool
    if (!TrackLevelCuts(track)) { continue; }
    vertex_it->second.selected_ntracks++;  // count tracks passing TrackLevelCuts

    tracks_by_crossing[*crossing].push_back(track);
  }

  FillPairsForEvent();

  outputFile->cd();
  //const Long64_t npairs = track_pairs->GetEntries();
  track_pairs->Write("",TObject::kOverwrite);
  outputFile->Close();

  delete true_vx;
  delete true_vy;
  delete true_vz;
  delete true_px;
  delete true_py;
  delete true_pz;
  delete true_dcaxy_in;
  delete true_dcaz_in;

    std::cout << outputrootfilename << " has been created"<<endl;
    // with "<< npairs << " track-pair entries" << std::endl;
}
