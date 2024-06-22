#include <trackbase/MvtxDefs.h>
#include <trackbase/InttDefs.h>
#include <trackbase/TpcDefs.h>
#include <MicromegasDefs.h>
#include <trackbase/ActsGeometry.h>
#include <trackermillepedealignment/AlignmentDefs.h>

#include <iostream>
#include <string>
#include <fstream>
#include <random>

#include <cmath>

// This macro makes a file named "data.txt" in the local directory
// Copy that to where you want it

/*
  Usage:
      The values of surface offsets are set at different levels by setting the static arrays below.
      These are used in:  
          getMvtxInputs()   - set stave global mean, random dev wrt mean, sensor dev wrt to stave
          getInttInputs()      - set stave global mean, random dev wrt mean, sensor dev wrt to stave
          getTpcInputs()     - set TPC global mean, sector dev wrt to TPC, surface dev wrt sector
          getMmsInputs()  - set tile global mean, tile random dev wrt mean
 */

// Set the offsets all in one place
//  - THE TRANSLATIONS ARE IN mm
//============================= 

// example residual misalignment settings.
// These are for testing tracking performance versus alignment precision
// Baseline values: Start at 40% of ideal resolution
// MVTX:  2, 4 8 16 microns
// INTT:   10, 20, 40 80 microns
// TPC:     60, 120, 240 480 microns
static const double inflation = 1.0;  //  will use 1, 2, 4, 8 as multiplier of the deviations

/*
// These are sigmas of the random distribution of deviations from the mean in mm
static const double mvtx_stave_dev = 0.002;   // mm
static const double mvtx_sensor_dev = 0.002;
static const double intt_stave_dev = 0.010;
static const double intt_sensor_dev = 0.010;
static const double tpc_sector_dev = 0.060;
static const double tpc_surf_dev = 0.010;
static const double mms_tile_dev = 0.060;
*/

// These are sigmas of the random distribution of deviations from the mean in mm
static const double mvtx_stave_dev = 0.0;
static const double mvtx_sensor_dev = 0.0;
static const double intt_stave_dev = 0.0;
static const double intt_sensor_dev = 0.0;
static const double tpc_sector_dev = 0.0;
static const double tpc_surf_dev = 0.0;
static const double mms_tile_dev = 0.0;

// These are mean offsets, set them all to zero for alignment residual tests
static const std::array<double, 3> mvtx_clamshell_mean[2] ={
   0.0, 0.0, 0.0,
   0.0, 0.0, 0.0}; 
static const std::array<double,3> mvtx_stave_mean[3] = {
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0 };
// rotations around global x,y,z axes 
/*
static const std::array<double,3> mvtx_layer_rotation_mean[3] = {
  0.0, 0.0, -0.0079,
  0.0, 0.0, 0.0149,
  0.0, 0.0, -0.0060 };      
*/
static const std::array<double,3> mvtx_layer_rotation_mean[3] = {
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0 };      

static const std::array<double,3> intt_stave_mean[4] = {
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0 };
static const std::array<double,3> intt_layer_rotation_mean[4] = {
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0  };      

// the index here is tpc region (0-2)
static const std::array<double,3> tpc_sector_mean[3] = {
  0, 0, 0,
  0, 0, 0, 
  0, 0, 0 };
static const std::array<double,3> tpc_sector_rotation_mean[3] = {
  0, 0, 0,
  0, 0, 0, 
  0, 0, 0 };

static const std::array<double,3> mms_tile_mean = {0, 0, 0};
static const std::array<double,3> mms_tile_rotation_mean = {0, 0, 0};

// Creates an alignment corrections file containing all zero alignment corrections if true
bool make_zero_corrections_all = false;
 // otherwise it adds misalignments as specified in the get*Inputs() methods

// Set all alignment corrections to zero for specified subsystems only
bool make_zero_corrections_mvtx = false;
bool make_zero_corrections_intt = false;
bool make_zero_corrections_tpc = false;
bool make_zero_corrections_mms = false;

// these are used for bookkeeping when using heirarchical offsets
static std::array<unsigned int, 9> stave_now = {999,999,999,999,999,999,999,999,999};
static std::array<double, 9> stave_perturbation_now = {0,0,0,0,0,0,0,0,0};
static std::array<unsigned int, 9> physical_sector_now = {999,999,999,999,999,999,999,999,999};
static std::array<double,9> physical_sector_perturbation_now = {0,0,0,0,0,0,0,0,0};

std::default_random_engine generator;

int get_tpc_region(int layer)
{
  int region = 0;
  if(layer > 22 && layer < 39)
    region = 1;
  if(layer > 38 && layer < 55)
    region = 2;

  return region;  
}

bool is_in_mvtx(int layer)
{
  bool ret = false;
  if(layer < 3) ret = true;
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

bool is_in_mms(int layer)
{
  bool ret = false;
  if(layer > 54) ret = true;
  return ret;
}

void getMvtxInputs( int layer, int stave, std::array<double, 9>& staveMean, std::array<double, 9>& staveDev, std::array<double, 9>& sensorDev)
{
  // translations are in mm !!!
  int clamshell = AlignmentDefs::getMvtxClamshell(layer, stave); 

  double stmean[9] = {0, 0, 0, 
		      mvtx_clamshell_mean[clamshell][0] + mvtx_stave_mean[layer][0], 
		      mvtx_clamshell_mean[clamshell][1] + mvtx_stave_mean[layer][1], 
		      mvtx_clamshell_mean[clamshell][2] + mvtx_stave_mean[layer][2],
		      mvtx_layer_rotation_mean[layer][0],
		      mvtx_layer_rotation_mean[layer][1],
		      mvtx_layer_rotation_mean[layer][2] };  // mean offset
  double stdev[9] = {0, 0, 0, mvtx_stave_dev, mvtx_stave_dev, mvtx_stave_dev,0,0,0};  // sigma around mean offset
  double sendev[9] = {0, 0, 0, mvtx_sensor_dev, mvtx_sensor_dev, mvtx_sensor_dev,0,0,0}; // sigma, centered on zero

  std::cout << " MVTX layer " << layer << " stave " << stave 
	    << " stave mean [3-8]: " << stmean[3] << "  " << stmean[4] << "  " << stmean[5] 
	    << stmean[6] << "  " << stmean[7] << "  " << stmean[8] 
	    << std::endl;
  
  for(int i=0; i<9; ++i)
    {  staveMean.at(i) = stmean[i]; }
  for(int i=0; i<9; ++i)
    {  staveDev.at(i) = stdev[i] * inflation; }
  for(int i=0; i<9; ++i)
    {  sensorDev.at(i) = sendev[i] * inflation; }
}

void getInttInputs( int layer, std::array<double, 9>& staveMean, std::array<double, 9>& staveDev, std::array<double, 9>& sensorDev)
{
  // translations are in mm !!!
  double stmean[9] = {0, 0, 0, intt_stave_mean[layer-3][0], intt_stave_mean[layer-3][1], intt_stave_mean[layer-3][2], 
		      intt_layer_rotation_mean[layer-3][0],  intt_layer_rotation_mean[layer-3][1],  intt_layer_rotation_mean[layer-3][2] };  // mean stave offset
  double stdev[9] = {0, 0, 0, intt_stave_dev, intt_stave_dev, intt_stave_dev,0,0,0};  // sigma around mean stave offset
  double sendev[9] = {0, 0, 0, intt_sensor_dev, intt_sensor_dev, intt_sensor_dev,0,0,0}; // sigma of sensor within stave
  
  for(int i=0; i<9; ++i)
    {  staveMean.at(i) = stmean[i];  }
  for(int i=0; i<9; ++i)
    {  staveDev.at(i) = stdev[i] * inflation; }
  for(int i=0; i<9; ++i)
    {   sensorDev.at(i) = sendev[i] * inflation; }
}

void getTpcInputs( int layer, std::array<double, 9>& staveMean, std::array<double, 9>& staveDev, std::array<double, 9>& sensorDev)
{
  int region = get_tpc_region(layer);

  // translations are in mm !!!
  double secmean[9] = {0, 0, 0, tpc_sector_mean[region][0], tpc_sector_mean[region][1], tpc_sector_mean[region][2],0,0,0}; //mean sector offset
  double secdev[9] = {0, 0, 0, tpc_sector_dev, tpc_sector_dev, tpc_sector_dev,0,0,0};  // sigma around mean sector offset
  double surfdev[9] = {0, 0, 0, tpc_surf_dev, tpc_surf_dev, tpc_surf_dev,0,0,0}; // surfaces added sigma, centered on zero
  
  for(int i=0; i<9; ++i)
    {  staveMean.at(i) = secmean[i];  }
  for(int i=0; i<9; ++i)
    {  staveDev.at(i) = secdev[i] * inflation; }
  for(int i=0; i<9; ++i)
    {   sensorDev.at(i) = surfdev[i] * inflation; }
}

void getMmsInputs( std::array<double, 9>& staveMean, std::array<double, 9>& staveDev, std::array<double, 9>& sensorDev)
{
  // translations are in mm !!!
  double tilemean[9] = {0, 0, 0, mms_tile_mean[0], mms_tile_mean[1], mms_tile_mean[2],0,0,0};  // mean offset
  double tiledev[9] = {0, 0, 0, mms_tile_dev, mms_tile_dev, mms_tile_dev,0,0,0};  // sigma around mean offset
  
  for(int i=0; i<9; ++i)
    {  staveMean.at(i) = tilemean[i];  }
  for(int i=0; i<9; ++i)
    {  staveDev.at(i) = tiledev[i] * inflation;  }
  for(int i=0; i<9; ++i)
    { sensorDev.at(i) = 0.0; }
}

void getInputs(unsigned int layer, int stave, std::array<double, 9>& staveMean, std::array<double, 9>& staveDev, std::array<double, 9>& sensorDev)
{

  if(layer < 3)
    {
      getMvtxInputs( layer, stave, staveMean, staveDev, sensorDev);
    }
  else if(layer >2 && layer < 7)
    {
      getInttInputs( layer, staveMean, staveDev, sensorDev);
    }
  else if(layer > 6 && layer < 55)
    {
      getTpcInputs( layer, staveMean, staveDev, sensorDev);
    }
  else
    {
      getMmsInputs( staveMean, staveDev, sensorDev);
    }

}

std::array<double, 9> getOffset(unsigned int layer, unsigned int stave)
{
  std::array<double, 9> offset={0,0,0,0,0,0,0,0,0};

  // use ideal geometry for parts of the detector
  if(make_zero_corrections_all) return offset;
  if(is_in_mvtx(layer) && make_zero_corrections_mvtx) return offset;
  if(is_in_intt(layer) && make_zero_corrections_intt) return offset;
  if(is_in_tpc(layer) && make_zero_corrections_tpc) return offset;
  if(is_in_mms(layer) && make_zero_corrections_mms) return offset;

  std::array<double, 9> staveMean = {0,0,0,0,0,0,0,0,0};
  std::array<double, 9> staveDev =     {0,0,0,0,0,0,0,0,0};
  std::array<double, 9> sensorDev =  {0,0,0,0,0,0,0,0,0};
  getInputs(layer, stave, staveMean, staveDev, sensorDev);
  //std::cout << " layer " << layer << " stave " << stave << " staveMean[3] " << staveMean[3] << "  staveDev " << staveDev[3] << std::endl;
  for(int i=0;i<9;++i)
    {
      //if(staveMean.at(i) != 0 && stave != stave_now.at(i))
      if(stave != stave_now.at(i))
	{
	  // new stave or sector, need new perturbation value
	  std::normal_distribution<double> distribution(staveMean.at(i), staveDev.at(i));
	  double perturbation = distribution(generator);
	  offset.at(i) = perturbation;
	  stave_now.at(i) = stave;
	  stave_perturbation_now.at(i) = perturbation;
	  //if(i==3) std::cout << "    new stave, perturbation " << perturbation << "  offset is " << offset.at(i) << std::endl;
	}
      else
	{
	  // same stave or sector, use the same perturbation
	  offset.at(i) = stave_perturbation_now.at(i);
	  //if(i==3) std::cout << "    keep offset at " << offset.at(i) << std::endl;
	}

      if(sensorDev[i] != 0)
	{
	  std::normal_distribution<double> distribution(0.0, sensorDev.at(i));
	  double perturbation = distribution(generator);
	  offset.at(i) += perturbation;
	  //if(i==3) std::cout << "    add sensor offset of " << perturbation << " offset now " << offset.at(i) << std::endl;
	}
    }

  return offset; 
}

std::array<double, 9> getOffsetTpc(unsigned int layer, unsigned int physical_sector)
{
  std::array<double, 9> offset={0,0,0,0,0,0,0,0,0};
  if(make_zero_corrections_tpc) return offset;

  std::array<double, 9> staveMean = {0,0,0,0,0,0,0,0,0};
  std::array<double, 9> staveDev = {0,0,0,0,0,0,0,0,0};
  std::array<double, 9> sensorDev = {0,0,0,0,0,0,0,0,0};
  getInputs(layer, physical_sector, staveMean, staveDev, sensorDev);

  for(int i=0;i<9;++i)
    {
      if(physical_sector != physical_sector_now.at(i))
	{
	  // new sector, need new perturbation value
	  std::normal_distribution<double> distribution(staveMean.at(i), staveDev.at(i));
	  double perturbation = distribution(generator);
	  offset.at(i) = perturbation;
	  physical_sector_now.at(i) = physical_sector;
	  physical_sector_perturbation_now.at(i) = perturbation;
	}
      else
	{
	  // same stave or sector, use the same perturbation
	  offset.at(i) = physical_sector_perturbation_now.at(i);
	}

      if(sensorDev[i] != 0)
	{
	  std::normal_distribution<double> distribution(0.0, sensorDev.at(i));
	  double perturbation = distribution(generator);
	  offset.at(i) += perturbation;
	}
    }
  return offset; 
}

void makeAlignmentParams(
			 bool _make_zero_corrections_all = false,
			 bool _make_zero_corrections_mvtx = false,
			 bool _make_zero_corrections_intt = false,
			 bool _make_zero_corrections_tpc = false,
			 bool _make_zero_corrections_mms = false
)
{
  make_zero_corrections_all = _make_zero_corrections_all;
  make_zero_corrections_mvtx = _make_zero_corrections_mvtx;
  make_zero_corrections_intt = _make_zero_corrections_intt;
  make_zero_corrections_tpc = _make_zero_corrections_tpc;
  make_zero_corrections_mms = _make_zero_corrections_mms;

  // mvtxdat for each layer: rMin, rMid, rMax, NChip/Stave, phi0, nStaves
  double mvtxdat[3][6] = {{24.61, 25.23, 27.93, 9., 0.285, 12.}, {31.98, 33.36, 36.25, 9., 0.199, 16.},{39.93, 41.48, 44.26, 9., 0.166, 20.}};

  std::map<TrkrDefs::hitsetkey, std::array<double, 9>> outmap;

  ofstream fw("data.txt", std::ofstream::out); // open file for writing alignment params and hitsetkey

  // alignment params for development purposes 
  float dx    = 0.0;
  float dy    = 0.0;
  float dz    = 0.0;
  float alpha = 0.0;
  float beta  = 0.0;
  float gamma = 0.0; 
  
  // Create MVTX hitsetkeys
  for (int layer = 0; layer<=2; layer++)          // loop over layers in MVTX
  {
    int staveNum = mvtxdat[layer][5];             // Number of staves per layer
    
    for(int stave = 0; stave < staveNum; stave++) // loop over staves
      {   
	for(int chip = 0; chip <= 8; chip++)      // loops over chips 
	  {
	    TrkrDefs::hitsetkey hitSetKey = MvtxDefs::genHitSetKey(layer,stave,chip,0);
	    //std::cout << "layer " << layer << " stave " << stave << " chip " << chip << std::endl;
	    std::array<double, 9> offset = getOffset(layer, stave);
	    outmap.insert(make_pair(hitSetKey, offset));
	  }
      }
    }

  // Create INTT hitsetkeys
  int nladder[4] = {12, 12, 16, 16};

  for (int layer = 3; layer<=6; layer++)           // loop over layers in INTT
  {
    int inttlayer = layer - 3;
    int staveNum = nladder[inttlayer];             // Number of staves per layer
    
    for(int stave = 0; stave < staveNum; stave++)  // loop over staves
      {    
	for(int chip = 0; chip <= 3; chip++)       // loops over chips 
	  {
	    TrkrDefs::hitsetkey hitSetKey = InttDefs::genHitSetKey(layer,chip,stave,0);
	    std::array<double, 9> offset = getOffset(layer, stave);
	    outmap.insert(std::make_pair(hitSetKey, offset));
	  }
      }
    }

  // Create TPC hitsetkeys
  // There are 72 "sectors" in the TPC
  // 12 sectors x 2 sides x 3 regions
  // these 72 "sectors" are independent objects, sharing common parameters
 
  for(unsigned int side = 0; side < 2; side++)        // loops over sides
    {    
      for(unsigned int sector = 0; sector < 12; sector++)     // loop over sectors
	{
	  for(unsigned int region =0; region < 3; ++region)
	    {
	      unsigned int physical_sector = side * 12  + sector + region * 24;

	      for (unsigned int layer = 0; layer<16; layer++)          // loop over layers in this TPC region
		{    
		  unsigned int sphenix_layer = 7 + 16 * region + layer;
		  TrkrDefs::hitsetkey hitSetKey = TpcDefs::genHitSetKey(sphenix_layer,sector,side);

		  std::array<double, 9> offset = getOffsetTpc(sphenix_layer, physical_sector);
		  outmap.insert(std::make_pair(hitSetKey, offset));

		  std::cout << " hitsetkey " << hitSetKey << " tpc layer " << sphenix_layer << " side " << side << " sector " << sector << " region " << region << " physical_sector " << physical_sector 
			    << " offset[3] " << offset[3] 
			    << " offset[4] " << offset[4]
			    << " offset[5] " << offset[5]
			    << " offset[6] " << offset[6]
			    << " offset[7] " << offset[7]
			    << " offset[8] " << offset[8]
			    << std::endl;

		}
	    }
	}
    }

  // Create Micromegas hitsetkeys
  for (unsigned int layer = 55; layer<=56; layer++)      // loop over layers in MM
  {    
    unsigned short segmentation;

    if(layer == 55){segmentation=1;}
    else if(layer == 56){segmentation=0;}

    MicromegasDefs::SegmentationType mmDefsSeg =  (MicromegasDefs::SegmentationType) segmentation;

    for(unsigned int tile = 0; tile < 8; tile++)        // loops over tiles with each tile having corresponding segmentation values
      {
	TrkrDefs::hitsetkey hitSetKey = MicromegasDefs::genHitSetKey(layer, mmDefsSeg, tile);
	std::array<double, 9> offset = getOffset(layer, tile);
	outmap.insert(std::make_pair(hitSetKey, offset));
      }
  }

  // copy the map to the file - ensures it is sorted by hitsetkey

  for(auto it = outmap.begin(); it != outmap.end(); ++it)
    {
      fw << it->first 
	 << " " << it->second.at(0)
	 << " " << it->second.at(1)
	 << " " << it->second.at(2)
	 << " " << it->second.at(3)
	 << " " << it->second.at(4)
	 << " " << it->second.at(5)
	 << " " << it->second.at(6)
	 << " " << it->second.at(7)
	 << " " << it->second.at(8)
	 << std::endl;
    }
  
  fw.close();

}
