#include <trackbase/TrkrDefs.h>
#include <trackbase/MvtxDefs.h>
#include <trackbase/InttDefs.h>
#include <trackbase/TpcDefs.h>
#include <MicromegasDefs.h>
#include <trackbase/ActsGeometry.h>
#include<trackermillepedealignment/AlignmentDefs.h>

#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int get_tpc_region(int layer)
{
  int region = 0;
  if(layer > 22 && layer < 39)
    region = 1;
  if(layer > 38 && layer < 55)
    region = 2;

  return region;  
}

TrkrDefs::hitsetkey getHitSetKey(int layer, int stave, int sensor)
{
  // all hitsetkeys should be obtained from here

 TrkrDefs::hitsetkey hitSetKey = 0;
 int zero = 0;

  if(layer < 3)
    hitSetKey = MvtxDefs::genHitSetKey(layer,stave,sensor,zero);
    
  if(layer >2 && layer < 7)
    hitSetKey = InttDefs::genHitSetKey(layer,sensor,stave,zero);

  if(layer > 6 && layer < 55)
    {
      // for the tpc, "stave" = 0-11 means sector 0-11 for side 0, and "stave" = 12-23 means sector 0-11 for side 1
      int side = stave/12;  
      int sector = stave - side*12;
      hitSetKey = TpcDefs::genHitSetKey(layer, sector, side);
    }

  if(layer > 54)
    {
      int tile = stave;  
      unsigned short segmentation = 0;  
      if(tile < 8){segmentation=1; layer = 55;}
      else {segmentation=0; layer = 56; tile -=8;}
      MicromegasDefs::SegmentationType mmDefsSeg =  (MicromegasDefs::SegmentationType) segmentation;
      hitSetKey = MicromegasDefs::genHitSetKey(layer, mmDefsSeg, tile);
    }

    return hitSetKey;
}

void populate_mvtx_stave(int layer, int stave, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{ 
  // mvtxdat for each layer: rMin, rMid, rMax, NChip/Stave, phi0, nStaves
  double mvtxdat[3][6] = {{24.61, 25.23, 27.93, 9., 0.285, 12.}, {31.98, 33.36, 36.25, 9., 0.199, 16.},{39.93, 41.48, 44.26, 9., 0.166, 20.}};
  int staveNum = mvtxdat[layer][5];             // Number of staves per layer
  
  for(int chip = 0; chip <= 8; chip++)      // loops over chips 
    {
      TrkrDefs::hitsetkey hitSetKey = getHitSetKey(layer,stave,chip);
      outmap.insert(std::make_pair(hitSetKey, params));
      //      fout << hitSetKey << " " <<params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
    }  
 }

void populate_entire_mvtx_layer(int layer, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{ 
  // Create MVTX hitsetkeys
  // mvtxdat for each layer: rMin, rMid, rMax, NChip/Stave, phi0, nStaves
  double mvtxdat[3][6] = {{24.61, 25.23, 27.93, 9., 0.285, 12.}, {31.98, 33.36, 36.25, 9., 0.199, 16.},{39.93, 41.48, 44.26, 9., 0.166, 20.}};
  int staveNum = mvtxdat[layer][5];             // Number of staves per layer
  
  for(int stave = 0; stave < staveNum; stave++) // loop over staves
    {    
      populate_mvtx_stave(layer, stave, outmap, params);
    }
 }
/*
bool is_stave_in_clamshell(int layer, int stave, int clamshell)
{
  double mvtxdat[3][6] = {{24.61, 25.23, 27.93, 9., 0.285, 12.}, {31.98, 33.36, 36.25, 9., 0.199, 16.},{39.93, 41.48, 44.26, 9., 0.166, 20.}};
  int staveNum = mvtxdat[layer][5];             // Number of staves per layer
  int breakat = staveNum / 2;
  int which_clamshell = stave / breakat;
  if(which_clamshell == clamshell)
    {
      return true;
    }
  return false;
}
*/

void populate_mvtx_clamshell_layer(int clamshell, int layer, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{ 
  // Create MVTX hitsetkeys
  // mvtxdat for each layer: rMin, rMid, rMax, NChip/Stave, phi0, nStaves
  double mvtxdat[3][6] = {{24.61, 25.23, 27.93, 9., 0.285, 12.}, {31.98, 33.36, 36.25, 9., 0.199, 16.},{39.93, 41.48, 44.26, 9., 0.166, 20.}};
  int staveNum = mvtxdat[layer][5];             // Number of staves per layer
  
  for(int stave = 0; stave < staveNum; stave++) // loop over staves
    {    
      int this_clamshell = AlignmentDefs::getMvtxClamshell(layer, stave);
      if(this_clamshell == clamshell)
	{
	  populate_mvtx_stave(layer, stave, outmap, params);
	  std::cout << "  Populated mvtx stave " << stave << " in layer " << layer << " of clamshell " << clamshell << std::endl;
	}
      /*
      if(is_stave_in_clamshell(layer, stave, clamshell))
	{
	  populate_mvtx_stave(layer, stave, outmap, params);
	  std::cout << "  Populated mvtx stave " << stave << " in layer " << layer << " of clamshell " << clamshell << std::endl;
	}
      */
    }
}

void populate_entire_mvtx_clamshell(int clamshell, int layer, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{ 
  // Create MVTX hitsetkeys
  // mvtxdat for each layer: rMin, rMid, rMax, NChip/Stave, phi0, nStaves
  double mvtxdat[3][6] = {{24.61, 25.23, 27.93, 9., 0.285, 12.}, {31.98, 33.36, 36.25, 9., 0.199, 16.},{39.93, 41.48, 44.26, 9., 0.166, 20.}};
  int staveNum = mvtxdat[layer][5];             // Number of staves per layer
  
  for(int stave = 0; stave < staveNum; stave++) // loop over staves
    {    
      int this_clamshell = AlignmentDefs::getMvtxClamshell(layer, stave);
      if(this_clamshell == clamshell)
	{
	  populate_mvtx_stave(layer, stave, outmap, params);
	  std::cout << "  Populated mvtx stave " << stave << " in layer " << layer << " of clamshell " << clamshell << std::endl;
	}
      /*
      if(is_stave_in_clamshell(layer, stave, clamshell))
	{
	  populate_mvtx_stave(layer, stave, outmap, params);
	  std::cout << "  Populated mvtx stave " << stave << " in layer " << layer << " of clamshell " << clamshell << std::endl;
	}
      */
    }
}

void populate_intt_stave(int layer, int stave, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{ 
  for(int chip = 0; chip <= 3; chip++)       // loops over chips 
    {
      TrkrDefs::hitsetkey hitSetKey = getHitSetKey(layer,stave,chip);
      outmap.insert(std::make_pair(hitSetKey, params));
      //      fout << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
    }
   }

void populate_entire_intt_layer(int layer, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{ 
 // Create INTT hitsetkeys
  int nladder[4] = {12, 12, 16, 16};

  int inttlayer = layer - 3;
  int staveNum = nladder[inttlayer];             // Number of staves per layer
    
  for(int stave = 0; stave < staveNum; stave++)  // loop over staves
    {    
      populate_intt_stave(layer, stave, outmap, params);
    }
 }

void populate_tpc_sector(int layer, int sector, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{ 
  TrkrDefs::hitsetkey hitSetKey = getHitSetKey(layer, sector, 0);  // side is determined from sector
  std::cout << "    layer " << layer << " sector " << sector << " hitsetkey " << hitSetKey << std::endl;
  std::cout <<  params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;

  outmap.insert(std::make_pair(hitSetKey, params));
  //  fout << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
}

void populate_entire_tpc_layer(int layer, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{
  // Create TPC hitsetkeys
  for(unsigned int sector = 0; sector < 24; sector++)     // loop over sectors, sector 0-11 is side 0, 12-23 side 1
    {    
      populate_tpc_sector(layer, sector, outmap, params);
    }
 }

void populate_mms_tile(int layer, int tile, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{
  TrkrDefs::hitsetkey hitSetKey = getHitSetKey(layer, tile, 0);
  //std::cout << " layer " << layer << " tile " << tile << " hitsetkey " << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
  outmap.insert(std::make_pair(hitSetKey, params));
  //  fout << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
  
}

void populate_entire_mms_layer(int layer, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{
  for(unsigned int ntile = 0; ntile < 16; ntile++)        // loops over tiles with each tile having corresponding segmentation values
    {
      populate_mms_tile(layer, ntile, outmap, params);
    }
}
      
void populate_entire_layer(int layer, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{
  // fill this layer with the same alignment parameters

  if(layer < 3)
    populate_entire_mvtx_layer(layer, outmap, params);

  if(layer > 2 && layer < 7)
    populate_entire_intt_layer(layer, outmap, params);

  if(layer > 6 && layer < 55)
    populate_entire_tpc_layer(layer, outmap, params);

  if(layer > 54)
    populate_entire_mms_layer(layer, outmap, params);
  
  return;
 }

bool getParameters(std::vector<std::pair<int, float>> par_vec, std::array<double, 6>& parameters)
{ 
  bool ret = true;

  for(int ip = 0; ip < 6; ++ip)
    parameters.at(ip) = 0.0;

  for(unsigned int iparv = 0; iparv < par_vec.size(); ++iparv)
    {
      for(int ip = 0; ip < 6; ++ip)
	{
	  if(ip+1 == par_vec[iparv].first)
	    {
	      float align = par_vec[iparv].second;
	      parameters.at(ip) = align;	      
	    }
	}
    }
  
  return ret;
}

void populate_stave(int layer, int stave, std::map<TrkrDefs::hitsetkey, std::array<double, 6>>& outmap, std::array<double, 6>& params)
{
  // stave can be stave, ladder, sector or tile

  if(layer < 3)
    populate_mvtx_stave(layer, stave, outmap, params);

  if(layer > 2 && layer < 7)
    populate_intt_stave(layer, stave, outmap, params);

  if(layer > 6 && layer < 55)
    populate_tpc_sector(layer, stave, outmap, params);

  if(layer > 54)
    populate_mms_tile(layer, stave, outmap, params);
  
  return;

}

bool is_in_mvtx(int layer)
{
  bool ret = false;
  if(layer >= 0 && layer < 3) ret = true;
  return ret;
}

bool is_in_intt(int layer)
{
  bool ret = false;
  if(layer > 2 && layer < 7) ret = true;
  return ret;
}

bool is_in_tpc(int layer)
{
  bool ret = false;
  if(layer > 6 && layer < 55) ret = true;
  return ret;
}

bool is_layer_in_region(int layer, int isec)
{
  bool ret = false;
  int region = isec / 24; // 72 sectors, 24 each region (sides 0 and 1)
  if(get_tpc_region(layer) == region) ret = true;

  return ret;
}

void process_millepede_results(std::string pedefilename = "millepede.res",
			       std::string newalignmentfilename = "new_alignment_corrections.txt",
			       bool helical_fitter = true)
{
  // macro to read in millepede.res files (pede output files) and process them
  // into a new alignment parameters file with one line for every surface
  // The millepede.res file contains entries only for parameters given non zero alignment derivatives
  // For sensors where the alignment parameter derivatives were zero, set the parameters to zero

  // NOTE: the macro assumes there is an entry for every surface group - input files should be from many events!
 
  // macro figures out grouping from the input file. Tested for cases:
  //     all sensors, hitsets and tiles free (using MakeMilleFiles with zero original misalignments)
  //     all sensors grouped in their staves,  all hitsets grouped in their regions/sectors, mms tiles free (MakeMilleFiles, HelicalFitter, zero misalignments)
  //     silicon detectors only (using HelicalFitter with small net misalignments)
  //     all tpc hitsets grouped in the tpc as a whole, all silicon sensors grouped in their staves, all mms tiles free (MakeMilleFiles, zero misalignments)

  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

  int verbosity = 1;

  int nladders_layer[7] = {12, 16, 20, 12, 12, 16, 16};

  ifstream fin(pedefilename);
  if(!fin.is_open()) {
    std::cout << "Unable to open file name " << pedefilename << std::endl;
	return;
  }

  int label = 0;
  float align = 0.0;
  
  std::string line;
  getline(fin,line);
  std::cout << "Discarding line: " << line << std::endl;  // no data in first line

  // Depending on the surface grouping settings, we may have:
  //    a line for every sensor, tpc hitset or mms tile in part or all of the tracker
  //    a line for every stave or sector in part or all of the tracker
  //    a line for an entire subdetector
  // or a mixture of these. We have to figure out what we have in the file.

 int layer_keep = 0;
  int stave_keep = 0;
  int sensor_keep = 0;
  std::vector<std::pair<int, float>> par_vec;
  std::vector<std::vector<std::pair<int, float>>> sensor_vec;
  //std::vector<std::vector<std::vector<std::pair<int, float>>>> stave_vec;
  std::map<int, std::vector<std::vector<std::pair<int, float>>>> stave_map;
  //  std::map<int, std::pair<int, std::vector<std::vector<std::vector<std::pair<int, float>>>>>> layer_stave_vec_map;
  std::map<int, std::map<int, std::vector<std::vector<std::pair<int, float>>>>> layer_stave_map;

  while(fin.good())
    {
      getline(fin,line);
      stringstream line_in(line);
      line_in >> label;
      line_in >> align;
      // we don't care about the rest

      int layer = label / 1000000;
      int stave = (label - layer *1000000) / 10000;
      int sensor = (label - layer *1000000 -stave*10000) / 10;
      int ipar = label - layer *1000000 -stave*10000 - sensor*10;

      TrkrDefs::hitsetkey hsk = getHitSetKey(layer, stave, sensor);

      if(verbosity > 0) 
	std::cout << " Input: layer " << layer << " stave " << stave << " sensor " << sensor << " hitsetkey " << hsk << " ipar " << ipar << " label " << label << " align " << align << std::endl;

      if(layer > layer_keep || fin.eof())
	{
	  // layer number layer_keep is done, close out this layer.
	  if(verbosity > 0) 
	    std::cout << "   layer change: new layer " << layer << " layer_keep " << layer_keep << " new stave " 
		      << stave << " stave keep " << stave_keep << " sensor " << sensor 
		      << " sensor keep " << sensor_keep << std::endl; 

	  //flush everything
	  sensor_vec.push_back(par_vec);
	  stave_map.insert(std::make_pair(stave_keep, sensor_vec));
	  layer_stave_map.insert( std::make_pair(layer_keep, stave_map) );

	  // reset for the new layer
	  layer_keep = layer;
	  stave_keep = stave;
	  sensor_keep = 0;
	  stave_map.clear();
	  sensor_vec.clear();
	  par_vec.clear();
	}      

       if(stave > stave_keep) 
	 { 
	   // new stave, close out sensor_vec and par_vec
	   sensor_vec.push_back(par_vec);
	   stave_map.insert(std::pair(stave_keep, sensor_vec));
	    std::cout << "   stave change:  layer " << layer << " layer_keep " << layer_keep << " new stave " 
		      << stave << " stave keep " << stave_keep << " sensor " << sensor 
		      << " sensor keep " << sensor_keep << std::endl; 

	   sensor_vec.clear();
	   par_vec.clear();
	   stave_keep = stave;  
	   sensor_keep = sensor;
	 }	
       else if (sensor > sensor_keep) 
	 { 
	   // new sensor, close out par_vec
	   std::cout << " new sensor " << sensor << " sensor_keep " << sensor_keep << std::endl;
	   sensor_vec.push_back(par_vec);
	   par_vec.clear();
	   sensor_keep = sensor; 
	 }

       par_vec.push_back(std::make_pair(ipar, align));
       
    }  // end loop over file lines

  for(auto lyrit = layer_stave_map.begin(); lyrit != layer_stave_map.end(); ++lyrit)
    {
      auto stave_map = lyrit->second;
      std::cout << " layer " << lyrit->first << " stave map entries " << lyrit->second.size() << std::endl;
      for(auto it = stave_map.begin(); it != stave_map.end(); ++it)
	{
	  auto sensor_vec = it->second;
	  std::cout << "   stave " <<  it->first << " sensor vec size " << sensor_vec.size()		    << std::endl;	  
	  if(sensor_vec.size() > 0)
	    {  
	      auto par_vec = sensor_vec[0];
	      if(par_vec.size() > 4)
		{
		  auto par3 = par_vec[3].first;
		  auto val3 = par_vec[3].second;
		  std::cout << " par3 " << par3 << " val3 " << val3 << std::endl;
		}
	    }
	}
    }
  

  // process the information and output the new parameters

  std::map<TrkrDefs::hitsetkey, std::array<double, 6>> outmap;

  // The MVTX needs special treatment if the staves are grouped by clamshell
  bool mvtx_clamshell = false;
  auto it0 = layer_stave_map.find(0);
  if (it0 != layer_stave_map.end())
    { 
      // the MVTX is present
      auto it1 = layer_stave_map.find(1);
      if(it1 == layer_stave_map.end())
	{
	  // layer 0 but no layer 1, the MVTX is grouped by clamshell everything will be under layer 0
	  std::cout << "MVTX is grouped by clamshell" << std::endl;
	  mvtx_clamshell = true;
	}
    }

  // Another MVTX possibility is that the staves are grouped by layer separately within each clamshell
  // In that case, the layers will be present, but there will be two staves/layer - the two clamshells



  // TheINTT needs special treatment if the four layers are grouped
  // this block is just to find out if that is true
  bool intt_grouped = false;
  auto it3 = layer_stave_map.find(3);
  if (it3 != layer_stave_map.end())
    { 
      // the TPC is present
      auto it4 = layer_stave_map.find(8);
      if(it4 == layer_stave_map.end())
	{
	  // layer 3 but no layer 4, the INTT is grouped as a whole, everything will be under layer 3
	  std::cout << "INTT is grouped as a whole" << std::endl;
	  intt_grouped = true;
	}
    }

  // The TPC needs special treatment if the layers in each sector are grouped
  // this block is just to find out if that is true
  bool tpc_grouped = false;
  auto it7 = layer_stave_map.find(7);
  if (it7 != layer_stave_map.end())
    { 
      // the TPC is present
      auto it8 = layer_stave_map.find(8);
      if(it8 == layer_stave_map.end())
	{
	  // layer 7 but no layer 8, the TPC is grouped by sectors or as a whole, everything will be under layer 7
	  std::cout << "TPC is grouped by sectors, or as a whole" << std::endl;
	  tpc_grouped = true;
	}
    }

  for(int layer=0; layer < 56; ++layer)   // layer 56 is included in layer 55 in the geometry id as an additional set of tiles
    {
      // this block handles the case where the mvtx readout is grouped by clamshell
      if(mvtx_clamshell &&  is_in_mvtx(layer))
	{
	  // Only layer 0 in the MP result file, with parameters for the two MVTX clamshells
	  if(verbosity > 0) std::cout << " layer " << layer << " is in clamshell grouped mvtx " << std::endl;
	  auto it0 = layer_stave_map.find(0);
	  auto stave_map = it0->second;
	  std::cout << "    layer " <<  layer << " stave_map size " << stave_map.size() << std::endl;	  
	  if(stave_map.size() == 2)
	    {
	      for(int clamshell = 0; clamshell < 2; ++clamshell)
		{
		  auto sensor_vec = stave_map.find(clamshell)->second;
		  auto par_vec = sensor_vec[0];
		  std::array<double, 6> params;
		  bool ret = getParameters(par_vec, params);
		  if(!ret) {std::cout << "   getParameters failed for MVTX clamshell grouped" << std::endl; }
		  if(verbosity > 0) { std::cout << " populate layer " << layer << " for MVTX clamshell " << clamshell 
						<< " with params[3-5] " << params[3] << "  " << params[4] << "  " << params[5] << std::endl; }
		  populate_entire_mvtx_clamshell(clamshell, layer, outmap, params);
		}
	      // done with this layer, skip to the next
	      continue;
	    }
	  else
	    {
	      std::cout << "Oops: something wrong with populating MVTX clamshells" << std::endl;
	    }
	}

      // this block handles the case where the intt readout is grouped as a whole
      if(intt_grouped &&  is_in_intt(layer))
	{
	  // Only layer 3 in the MP result file, with parameters for the whole INTT
	  if(verbosity > 0) std::cout << " layer " << layer << " is in grouped intt " << std::endl;
	  auto it3 = layer_stave_map.find(3);
	  auto stave_map = it3->second;
	  std::cout << "    layer " <<  layer << " stave_map size " << stave_map.size() << std::endl;	  
	  // handle case where the entire intt is grouped, there is only one parameter set
	  if(stave_map.size() == 1)
	    {
	      auto sensor_vec = stave_map.find(0)->second;
	      auto par_vec = sensor_vec[0];
	      std::array<double, 6> params;
	      bool ret = getParameters(par_vec, params);
	      if(!ret) {std::cout << "   getParameters failed for entire INTT grouped" << std::endl; }
	      if(verbosity > 0) std::cout << " populate layer " << layer << " for INTT with params[3-5] " 
					  << params[3] << "  " << params[4] << "  " << params[5] << std::endl;
	      populate_entire_intt_layer(layer, outmap, params);
	      
	      // done with this layer, skip to the next
	      continue;
	    }
	  else
	    {
	      std::cout << "Oops: something wrong with populating entire INTT" << std::endl;
	    }

	}

      // this block handles the case where the tpc readout is grouped by sector
      if(tpc_grouped && is_in_tpc(layer))
	{
	  // Only layer 7 in the MP result file, with parameters for each sector
	  if(verbosity > 0) std::cout << " layer " << layer << " is in sector grouped tpc " << std::endl;
	  auto it7 = layer_stave_map.find(7);
	  //auto stave = it7->first;
	  auto stave_map = it7->second;
	  std::cout << "    layer " <<  layer << " stave_map size " << stave_map.size() << std::endl;	  
	  // handle case where the entire tpc is grouped, there is only one parameter set
	  if(stave_map.size() == 1)
	    {
	      auto sensor_vec = stave_map.find(0)->second;
	      auto par_vec = sensor_vec[0];
	      std::array<double, 6> params;
	      bool ret = getParameters(par_vec, params);
	      if(!ret) {std::cout << "   getParameters failed for entire TPC grouped" << std::endl; }
	      if(verbosity > 0) std::cout << " populate layer " << layer << " for all sectors with params[0] " << params[0] << std::endl;
	      populate_entire_tpc_layer(layer, outmap, params);
	      
	      // done with this layer, skip to the next
	      continue;
	    }
	  
	  // if we are here, we have multiple tpc "staves", grouped by sector
	  // here isec = region * 24(0-47) + sector (0-23). This layer is in only one region.
	  // we need to handle cases where one or more regions is fixed
	  // in that case, there will be no entry for these sectors in stave_map

	  // loop over all possible TPC sectors
	  for(int isec = 0; isec < 72; ++isec)
	    {
	      if(is_layer_in_region(layer, isec))
		{
		  auto secit = stave_map.find(isec);
		  if(secit == stave_map.end())
		    {
		      std::cout << "  Did not find sector " << isec << " for layer " << layer << " in stave_map " << std::endl;
		      // sector is missing, it must be fixed
		      // set all params to zero
		      std::array<double, 6> params = {0,0,0,0,0,0};
		      
		      std::cout << "  populate entire sector with zero for layer " << layer << " sector " << isec << std::endl;
		      // convert isec to the physical sector (0-23, including side)
		      int region = get_tpc_region(layer);
		      int sector = isec - region * 24;
		      populate_tpc_sector(layer, sector, outmap, params);
		    }
		  else
		    {
		      // is this layer in the region of this sector?
		      //      if(is_layer_in_region(layer, isec))
		      //{
			  std::cout << "  Found sector " << secit->first << " for layer " << layer << " in stave_map " << std::endl;
			  auto sector_vec = secit->second;
			  auto par_vec = sector_vec[0]; // only one parameter set per sector
			  std::array<double, 6> params;
			  bool ret = getParameters(par_vec, params);
			  if(!ret)
			    {
			      std::cout << " getParameters returned error for layer " << layer << " isec " << isec 
					<< " sensor " << 0 << " parameters set to zero " << std::endl;
			    }
			  
			  // convert isec to the physical sector (0-23, including side)
			  int region = get_tpc_region(layer);
			  int sector = isec - region * 24;
			  if(verbosity > 0) std::cout << " populate layer " << layer << " for isec " << isec << " sector " 
						      << sector << " with params[3-5] " << params[3] << "  " << params[4] << "  " << params[5] << std::endl;
			  populate_tpc_sector(layer, sector, outmap, params);
			  //}
		    }
		}
	      /*
	      // is this layer in the region of this sector?
	      if(is_layer_in_region(layer, isec))
		{
		  auto sector_vec = stave_vec[isec];
		  auto par_vec = sector_vec[0]; // only one parameter set per sector
		  std::array<double, 6> params;
		  bool ret = getParameters(par_vec, params);
		  if(!ret)
		    {
		      std::cout << " getParameters returned error for layer " << layer << " isec " << isec 
				<< " sensor " << 0 << " parameters set to zero " << std::endl;
		    }

		  // convert isec to the physical sector (0-23, including side)
		  int region = get_tpc_region(layer);
		  int sector = isec - region * 24;
		  if(verbosity > 0) std::cout << " populate layer " << layer << " for isec " << isec << " sector " << sector << " with params[3-5] " << params[3] << "  " << params[4] << "  " << params[5] << std::endl;
		  populate_tpc_sector(layer, sector, outmap, params);
		}
	      */
	    }

	  // done with this layer, skip to the next
	  continue;
	}
      
      // back to our normal program
      // find this layer in the vector of staves and get its vector of sensors
      auto it = layer_stave_map.find(layer);
      if (it == layer_stave_map.end())
	{
	  // layer is missing, all alignment corrections are zero for missing layers
	  std::array<double, 6> params = {0,0,0,0,0,0};
	  if(verbosity > 0) std::cout << " missing layer " << layer << " set all params to  " << params[0] << std::endl;
	  populate_entire_layer(layer, outmap, params);
	  // done with this layer
	  continue;
	}

      auto stave_map = it->second;

      if( (stave_map.size() == 1) &&( !is_in_tpc(layer)) ) 
	{
	  // layer exists, but all silicon staves or mms tiles are grouped together, all sensors in all staves share the same alignment parameters
	  if(verbosity > 0) std::cout << " All staves are grouped together in layer " << layer << std::endl;
	  // fill in all stave and sensor lines for this layer using the single parameter set for the only sensor entry
	  auto sensor_vec = stave_map.find(0)->second;
	  auto par_vec = sensor_vec[0];
	  std::array<double, 6> params;
	  bool ret = getParameters(par_vec, params);
	  if(!ret)
	    {
	      std::cout << " getParameters returned error for layer " << layer << " stave " << 0 
			<< " sensor " << 0 << " parameters set to zero "<< std::endl;
	    }
	  populate_entire_layer(layer, outmap, params);
	  // done with this layer
	  continue;
	}

      if( (stave_map.size() == 2) &&(is_in_mvtx(layer)) ) 
	{
	  // layer exists, but all mvtx staves are grouped together within a clamshell
	      for(int clamshell = 0; clamshell < 2; ++clamshell)
		{
		  if(verbosity > 0) std::cout << " All staves are grouped together in clamshell " << clamshell << " in layer " << layer << std::endl;
		  auto sensor_vec = stave_map.find(clamshell)->second;
		  auto par_vec = sensor_vec[0];
		  std::array<double, 6> params;
		  bool ret = getParameters(par_vec, params);
		  if(!ret) {std::cout << "   getParameters failed for MVTX clamshell grouped" << std::endl; }
		  if(verbosity > 0) { std::cout << " populate layer " << layer << " for MVTX clamshell " << clamshell 
						<< " with params[3-5] " << params[3] << "  " << params[4] << "  " << params[5] << std::endl; }

		  // fill in all stave and sensor lines for this clamshell/layer using the single parameter set for the only sensor entry
		  populate_mvtx_clamshell_layer(clamshell, layer, outmap, params);
		}
	      // done with this layer
	      continue;
	}

      if(verbosity > 0) std::cout << " layer "  << layer << " stave_map size " << stave_map.size() << std::endl;
      for(auto it = stave_map.begin(); it != stave_map.end(); ++it)
	{
	  int stave = it->first;
	  auto sensor_vec = stave_map.find(stave)->second;

	  if( (sensor_vec.size() == 1) &&( !is_in_tpc(layer)) ) 
	    {
	      // final special case, all sensors in this stave are grouped together
	      if(verbosity > 0) std::cout << " All sensors are grouped together for layer " << layer << " and stave " << stave << std::endl;
	      // fill in all sensor lines for this stave, using the single parameter set for this stave's only sensor entry
	      auto par_vec = sensor_vec[0];
	      std::array<double, 6> parameters;
	      bool ret = getParameters(par_vec, parameters);
	      if(!ret)
		{
		  std::cout << " getParameters returned error for layer " << layer << " stave " << stave 
			    << " sensor " << 0 << " parameters set to zero " << std::endl;
		}
	      populate_stave(layer, stave, outmap, parameters);
	      continue;
	    }

	  // if we got to here, the sensors or hitsets or tiles are ungrouped
	  //   -- we have individual results for each silicon sensor, tpc hitset, and mms tile
	  for(unsigned int is = 0; is < sensor_vec.size(); ++is)
	    {
	      int sensor = is;	      
	      auto par_vec = sensor_vec[is];
	      std::array<double, 6> parameters;
	      bool ret = getParameters(par_vec, parameters);
	      if(!ret)
		{
		  std::cout << " getParameters returned error for layer " << layer << " stave " << stave 
			    << " sensor " << sensor << " parameters set to zero "<< std::endl;
		}
	      // generate a hitsetkey for this entry
	      TrkrDefs::hitsetkey hitSetKey = getHitSetKey(layer, stave, sensor);
	      if(verbosity > 0)
		{
		  std::cout  << " To file: layer " << layer << " stave " << stave << " sensor " << sensor << " " 
			     << hitSetKey << " "
			     << parameters[0] << " " << parameters[1] << " "  << parameters[2] << " "
			     << parameters[3] << " " << parameters[4] << " "  << parameters[5]  << std::endl;
		}
	      outmap.insert(std::make_pair(hitSetKey, parameters));
	    }
	}
    }

  // All  fits, residuals and derivatives in HelicalFitter are done in cm
  // So the alignment parameters are in cm
  // The translation alignment parameters in the file should be in mm, so we change them here
  double factor = 1.0;
  if(helical_fitter) factor = 10.0;
  ofstream fout(newalignmentfilename);	
  for(auto it = outmap.begin(); it != outmap.end(); ++it)
    {
      fout << it->first 
	   << " " << it->second.at(0)
	   << " " << it->second.at(1)
	   << " " << it->second.at(2)
	   << " " << it->second.at(3) * factor
	   << " " << it->second.at(4) * factor
	   << " " << it->second.at(5) * factor
	   << std::endl;
    }
  
  fout.close(); 
  
  // The following is used during iterative running of tracking/millepede 
  // We would iterate on real data or simulations
  //
  // Procedure for simulated misalignments:
  //===============================
  // Uses the following files:
  // 
  // originalMisalignmentParams.txt -- the original input misalignment offsets (never changes)
  // currentAlignmentParams.txt -- sum of all corrections obtained from all millepede runs so far (starts at zero)
  // new_alignment_corrections.txt  -- corrections obtained in the last millepede run (new each iteration)
  // Update after each iteration: 
  // currentAlignmentParams.txt = new_alignment_corrections.txt + currentAlignmentParamsFile.txt
  // differenceAlignmentParams.txt = originalMisalignmentParamsFile.txt - currentAlignmentParamsFile.txt
  //
  // To begin:
  //
  // copy a file with zero alignment parameters into currentAlignmentParameters.txt
  // create originalMisalignmentsParamsFile.txt containing desired misalignments
  // create differenceAlignmentParams.txt = originalMisalignmentsParams.txt - currentAlignmentParams.txt

  // At each iteration:
  //
  // 1) Input differenceAlignmentParams.txt to run tracking with the residual misalignments after all previous iterations
  // 2) Run pede on Mille output file to produce millepede.res file
  // 3) Run process_millepede_output.C on millepede.res file to produce new_allignment_corrections.txt file
  // 4) Add new_alignment_corrections.txt to existing currentAlignmentParams.txt 
  // 5) Replace differenceAlignmentParams.txt = (originalMisalignmentsParams.txt - currentAlignmentParams.txt)
  // Ready to rerun tracking with newest differenceAlignmentParams.txt
  // 

  // steps 1, 2, 3 are done, this is step  4
//===========================
  bool update_existing_corrections_file = false;
  if(update_existing_corrections_file)
    {
      std::cout << " Add new parameters to existing ones" << std::endl;

      ifstream fexisting("currentAlignmentParams.txt");
      if(!fexisting.is_open()) std::cout << "Unable to open existing params file" << std::endl;
      
      ifstream fnew("new_alignment_corrections.txt");
      if(!fnew.is_open()) std::cout << "Unable to open new params file" << std::endl;
      
      ofstream fupdated("updatedAlignmentParams.txt");
      
      TrkrDefs::hitsetkey key_exist, key_new;
      float pars_exist[6], pars_new[6], pars_update[6];

      std::string line1, line2;
      
      while( getline(fexisting, line1) )
	{
	  stringstream line_exist(line1);
	  if(verbosity > 1) std::cout << "exist in: " << line_exist.str() << std::endl;
	  line_exist >> key_exist;
	  unsigned short layer_exist = TrkrDefs::getLayer(key_exist);
	  if(verbosity > 1) 
	    {
	      std::cout << "     layer_exist " << layer_exist << std::endl; 	  
	      if(layer_exist > 6 && layer_exist < 55)
		{
		  unsigned short sector = TpcDefs::getSectorId(key_exist);
		  unsigned short side = TpcDefs::getSide(key_exist);
		  std::cout << "       sector " << sector << " side " << side << std::endl;
		}	    
	      if(layer_exist > 54)
		{
		  unsigned short tile = MicromegasDefs::getTileId(key_exist);	      
		  MicromegasDefs::SegmentationType seg = MicromegasDefs::getSegmentationType(key_exist);
		  int type = 0;
		  if(seg == MicromegasDefs::SegmentationType::SEGMENTATION_Z) type = 1;
		  std::cout << "       tile " << tile << " segmentation " << type << std::endl;
		}
	    }

	  getline(fnew, line2);
	  stringstream line_new(line2);
	  if(verbosity > 1) std::cout << "new in: " << line_new.str() << std::endl;
	  line_new >> key_new;
	  unsigned short layer_new = TrkrDefs::getLayer(key_new);
	  if(verbosity > 1) 
	    {
	      std::cout << "     layer _new " << layer_new << std::endl; 	  
	      if(layer_new > 6 && layer_new < 55)
		{
		  unsigned short sector = TpcDefs::getSectorId(key_new);
		  unsigned short side = TpcDefs::getSide(key_new);
		  std::cout << "       sector " << sector << " side " << side << std::endl;
		}
	      if(layer_new > 54)
		{
		  unsigned short tile = MicromegasDefs::getTileId(key_new);	      
		  MicromegasDefs::SegmentationType seg = MicromegasDefs::getSegmentationType(key_new);
		  int type = 0;
		  if(seg == MicromegasDefs::SegmentationType::SEGMENTATION_Z) type = 1;
		  std::cout << "       tile " << tile << " segmentation " << type << std::endl;
		}
	    }

	  if(key_new != key_exist)
	    std::cout << "ERROR: Mismatched keys: in layer " << layer_new << " existing key " << key_exist << " new key " << key_new << std::endl;
	  
	  line_exist >> pars_exist[0] >> pars_exist[1] >> pars_exist[2] >> pars_exist[3] >> pars_exist[4] >> pars_exist[5];
	  line_new >> pars_new[0] >> pars_new[1] >> pars_new[2] >> pars_new[3] >> pars_new[4] >> pars_new[5];
	  fupdated << key_exist << " "
		   << pars_exist[0] + pars_new[0]<< " " 
		   << pars_exist[1] + pars_new[1] << " " 
		   << pars_exist[2] + pars_new[2] << " " 
		   << pars_exist[3] + pars_new[3] << " " 
		   << pars_exist[4] + pars_new[4] << " "
		   << pars_exist[5] + pars_new[5]
		   << std::endl;
	}
      fupdated.close();

      // copy updatedAlignmentParams.txt to currentAlignmentParams.txt
      gSystem->Exec("mv updatedAlignmentParams.txt currentAlignmentParams.txt");
      
      std::cout << "Done updating currentAlignmentParams.txt file" << std::endl;        
    }



  // steps 1, 2, 3, 4 are done, this is step 5 - used when using simulated misalignments
  //==============================================================
  // if requested, subtract the updated alignment corrections file from the original misalignments file
  bool subtract_from_misalignment_file =false;
  if(subtract_from_misalignment_file)
    {  
      std::cout << " Add new parameters to existing ones" << std::endl;
      
      //ifstream foriginal("originalMisalignmentParams.txt");
      ifstream foriginal("/sphenix/user/frawley/new_macros_april27/macros/detectors/sPHENIX/localAlignmentParamsFile.txt");
      if(!foriginal.is_open()) std::cout << "Unable to open original misalignment params file" << std::endl;
      
      ifstream fupdated("/sphenix/user/frawley/new_macros_april27/macros/detectors/sPHENIX/currentAlignmentParams.txt");
      if(!fupdated.is_open()) std::cout << "Unable to open updated params file" << std::endl;
      
      ofstream fdifference("differenceAlignmentParams.txt");
      
      TrkrDefs::hitsetkey key_original, key_updated;
      float pars_original[6], pars_updated[6];
      
      std::string line1, line2;
      
      while( getline(foriginal, line1) )
	{
	  stringstream line_original(line1);
	  if(verbosity > 1) std::cout << "original in: " << line_original.str() << std::endl;
	  line_original >> key_original;
	  getline(fupdated, line2);
	  stringstream line_updated(line2);
	  if(verbosity > 1) std::cout << "updated  in: " << line_updated.str() << std::endl;
	  line_updated >> key_updated;
	  if(key_updated != key_original)
	    std::cout << "ERROR: Mismatched keys: " << key_original << "   " << key_updated << std::endl;
	  
	  line_original >> pars_original[0] >> pars_original[1] >> pars_original[2] >> pars_original[3] >> pars_original[4] >> pars_original[5];
	  line_updated >> pars_updated[0] >> pars_updated[1] >> pars_updated[2] >> pars_updated[3] >> pars_updated[4] >> pars_updated[5];
	  fdifference << key_original << " "
		      << pars_original[0] - pars_updated[0]<< " " 
		      << pars_original[1] - pars_updated[1] << " " 
		      << pars_original[2] - pars_updated[2] << " " 
		      << pars_original[3] - pars_updated[3] << " " 
		      << pars_original[4] - pars_updated[4] << " "
		      << pars_original[5] - pars_updated[5]
		      << std::endl;
	}
      fdifference.close();
      
      std::cout << "Done making difference file " << std::endl;  
     }
  
}
