#include "UsefulFunctions.h"

void OverlayDrawFitSlicesYComparisonforTH3D(){
    float dcaxy_onesiderange_formeanwidthcalc=0.01, dcaz_onesiderange_formeanwidthcalc=0.01,
    deltavertex_onesiderange_formeanwidthcalc=0.01;
    
    const char* default_vertex_cuts = "vertex_ntracks > 3, |vx|>=1e^{-7}";
    const char* default_track_cuts = "nmaps>= 2 , nintt>=1 , ntpc>=26 , pt>= 0.2";
    const char* track_cuts_etaphitwotrackdca3dhists = "nmaps>= 2 , nintt>=1 , ntpc>=26 , pt>= 0.8";
    const char* default_pair_cuts = "|p_{1}-p_{2}|<=0.1";
    
    TF1 fgaus_dcaxy("fgaus_dcaxy","gaus",-dcaxy_onesiderange_formeanwidthcalc, dcaxy_onesiderange_formeanwidthcalc);
    TF1 fgaus_dcaz("fgaus_dcaz","gaus",-dcaz_onesiderange_formeanwidthcalc, dcaz_onesiderange_formeanwidthcalc);
    
    DrawFitSlicesComparisonForTH3D(/*simulation_filename*/"twoTrackDca_map_out_Simulation.root",/*data_filename*/"twoTrackDca_map_out_data.root","h_etadiff_two_track_dcaxy_pt","h_etadiff_two_track_dcaxy_pt",&fgaus_dcaxy,1,2,3,"#eta_{1}-#eta_{2}","dcaxy","(p_{T,1}+p_{T,2})/2",-0.008,0.008,-dcaxy_onesiderange_formeanwidthcalc,dcaxy_onesiderange_formeanwidthcalc,-2.0,2.0,8,2, default_vertex_cuts, default_track_cuts, default_pair_cuts);
    
    DrawFitSlicesComparisonForTH3D(/*simulation_filename*/"twoTrackDca_map_out_Simulation.root",/*data_filename*/"twoTrackDca_map_out_data.root","h_phidiff_two_track_dcaxy_pt","h_phidiff_two_track_dcaxy_pt",&fgaus_dcaxy,1,2,3,"#phi_{1}-#phi_{2}","dcaxy","(p_{T,1}+p_{T,2})/2",-0.008,0.008,-dcaxy_onesiderange_formeanwidthcalc,dcaxy_onesiderange_formeanwidthcalc,-6.0,6.0,8,2, default_vertex_cuts, default_track_cuts, default_pair_cuts);
    
    DrawFitSlicesComparisonForTH3D(/*simulation_filename*/"twoTrackDca_map_out_Simulation.root",/*data_filename*/"twoTrackDca_map_out_data.root","h_etadiff_two_track_dcaz_pt","h_etadiff_two_track_dcaz_pt",&fgaus_dcaz,1,2,3,"#eta_{1}-#eta_{2}","dcaz","(p_{T,1}+p_{T,2})/2",-0.008,0.008,-dcaz_onesiderange_formeanwidthcalc,dcaz_onesiderange_formeanwidthcalc,-2.0,2.0,8,2, default_vertex_cuts, default_track_cuts, default_pair_cuts);
    
    DrawFitSlicesComparisonForTH3D(/*simulation_filename*/"twoTrackDca_map_out_Simulation.root",/*data_filename*/"twoTrackDca_map_out_data.root","h_phidiff_two_track_dcaz_pt","h_phidiff_two_track_dcaz_pt",&fgaus_dcaz,1,2,3,"#phi_{1}-#phi_{2}","dcaz","(p_{T,1}+p_{T,2})/2",-0.01,0.01,-dcaz_onesiderange_formeanwidthcalc,dcaz_onesiderange_formeanwidthcalc,-6.0,6.0,8,2, default_vertex_cuts, default_track_cuts, default_pair_cuts);
    
    DrawFitSlicesComparisonForTH3D(/*simulation_filename*/"twoTrackDca_map_out_Simulation.root",/*data_filename*/"twoTrackDca_map_out_data.root","h_etadiff_two_track_dcaxy_pt","h_etadiff_two_track_dcaxy_pt",&fgaus_dcaxy,1,2,3,"#eta_{1}-#eta_{2}","dcaxy","(p_{T,1}+p_{T,2})/2",-0.008,0.008,-dcaxy_onesiderange_formeanwidthcalc,dcaxy_onesiderange_formeanwidthcalc,-2.0,2.0,8,2, default_vertex_cuts, default_track_cuts, default_pair_cuts);
    
    DrawFitSlicesComparisonForTH3D(/*simulation_filename*/"twoTrackDca_map_out_Simulation.root",/*data_filename*/"twoTrackDca_map_out_data.root","h_etadiff_phidiff_two_track_dcaxy","h_etadiff_phidiff_two_track_dcaxy",&fgaus_dcaxy,1,3,2,"#eta_{1}-#eta_{2}","dcaxy","#phi_{1}-#phi_{2}",-0.008,0.008,-dcaxy_onesiderange_formeanwidthcalc,dcaxy_onesiderange_formeanwidthcalc,-2.0,2.0,8,30, default_vertex_cuts, track_cuts_etaphitwotrackdca3dhists, default_pair_cuts);
    
    DrawFitSlicesComparisonForTH3D(/*simulation_filename*/"twoTrackDca_map_out_Simulation.root",/*data_filename*/"twoTrackDca_map_out_data.root","h_etadiff_phidiff_two_track_dcaxy","h_phidiff_etadiff_two_track_dcaxy",&fgaus_dcaxy,2,3,1,"#phi_{1}-#phi_{2}","dcaxy","#eta_{1}-#eta_{2}",-0.008,0.008,-dcaxy_onesiderange_formeanwidthcalc,dcaxy_onesiderange_formeanwidthcalc,-2.0,2.0,8,30, default_vertex_cuts, track_cuts_etaphitwotrackdca3dhists, default_pair_cuts);
    
    DrawFitSlicesComparisonForTH3D(/*simulation_filename*/"twoTrackDca_map_out_Simulation.root",/*data_filename*/"twoTrackDca_map_out_data.root","h_etadiff_phidiff_two_track_dcaz","h_etadiff_phidiff_two_track_dcaz",&fgaus_dcaz,1,3,2,"#eta_{1}-#eta_{2}","dcaz","#phi_{1}-#phi_{2}",-0.008,0.008,-dcaz_onesiderange_formeanwidthcalc,dcaz_onesiderange_formeanwidthcalc,-2.0,2.0,8,30, default_vertex_cuts, track_cuts_etaphitwotrackdca3dhists, default_pair_cuts);
    
    DrawFitSlicesComparisonForTH3D(/*simulation_filename*/"twoTrackDca_map_out_Simulation.root",/*data_filename*/"twoTrackDca_map_out_data.root","h_etadiff_phidiff_two_track_dcaz","h_phidiff_etadiff_two_track_dcaz",&fgaus_dcaz,2,3,1,"#phi_{1}-#phi_{2}","dcaz","#eta_{1}-#eta_{2}",-0.008,0.008,-dcaz_onesiderange_formeanwidthcalc,dcaz_onesiderange_formeanwidthcalc,-2.0,2.0,8,30, default_vertex_cuts, track_cuts_etaphitwotrackdca3dhists, default_pair_cuts);
    
}
