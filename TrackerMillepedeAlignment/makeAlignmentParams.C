#include <trackbase/MvtxDefs.h>
#include <trackbase/InttDefs.h>
#include <trackbase/TpcDefs.h>
#include <MicromegasDefs.h>
#include <trackbase/ActsGeometry.h>

#include <iostream>
#include <string>
#include <fstream>
#include <random>

#include <cmath>

// Creates an alignment corrections file containing all zero alignment corrections

static std::array<unsigned int, 6> stave_now = {999,999,999,999,999,999};
static std::array<double, 6> stave_perturbation_now = {0,0,0,0,0,0};

static std::array<unsigned int, 6> physical_sector_now = {999,999,999,999,999,999};
static std::array<double,6> physical_sector_perturbation_now = {0,0,0,0,0,0};

std::default_random_engine generator;

void getMvtxInputs( std::array<double, 6>& staveMean, std::array<double, 6>& staveDev, std::array<double, 6>& sensorDev)
{
  double stmean[6] = {0, 0, 0, 0.010, 0.010, 0.025};  // mean offset
  double stdev[6] = {0, 0, 0, 0.005, 0.005, 0.005};  // sigma around mean offset
  double sendev[6] = {0, 0, 0, 0.002, 0.002, 0.002}; // sigma, centered on zero

  for(int i=0; i<6; ++i)
    {  staveMean.at(i) = stmean[i]; }
  for(int i=0; i<6; ++i)
    {  staveDev.at(i) = stdev[i]; }
  for(int i=0; i<6; ++i)
    {  sensorDev.at(i) = sendev[i]; }
}

void getInttInputs( std::array<double, 6>& staveMean, std::array<double, 6>& staveDev, std::array<double, 6>& sensorDev)
{
  double stmean[6] = {0, 0, 0, 0.010, 0.010, 0.025};  // mean stave offset
  double stdev[6] = {0, 0, 0, 0.005, 0.005, 0.005};  // sigma around mean stave offset
  double sendev[6] = {0, 0, 0, 0.002, 0.002, 0.002}; // sigma of sensor within stave, centered on zero

  for(int i=0; i<6; ++i)
    {  staveMean.at(i) = stmean[i];  }
  for(int i=0; i<6; ++i)
    {  staveDev.at(i) = stdev[i]; }
  for(int i=0; i<6; ++i)
    {   sensorDev.at(i) = sendev[i]; }
}

void getTpcInputs( std::array<double, 6>& staveMean, std::array<double, 6>& staveDev, std::array<double, 6>& sensorDev)
{
  double secmean[6] = {0, 0, 0, 0.020, 0.020, 0.0300};  // mean sector offset (i.e. TPC offset)
  double secdev[6] = {0, 0, 0, 0.010, 0.010, 0.01};  // sigma around mean sector offset
  double surfdev[6] = {0, 0, 0, 0, 0, 0}; // surfaces sigma, centered on zero

  for(int i=0; i<6; ++i)
    {  staveMean.at(i) = secmean[i];  }
  for(int i=0; i<6; ++i)
    {  staveDev.at(i) = secdev[i]; }
  for(int i=0; i<6; ++i)
    {   sensorDev.at(i) = surfdev[i]; }
}

void getMmsInputs( std::array<double, 6>& staveMean, std::array<double, 6>& staveDev, std::array<double, 6>& sensorDev)
{
  double tilemean[6] = {0, 0, 0, 0.010, 0.010, 0.025};  // mean offset
  double tiledev[6] = {0, 0, 0, 0.005, 0.005, 0.005};  // sigma around mean offset

  for(int i=0; i<6; ++i)
    {  staveMean.at(i) = tilemean[i];  }
  for(int i=0; i<6; ++i)
    {  staveDev.at(i) = tiledev[i];  }
  for(int i=0; i<6; ++i)
    { sensorDev.at(i) = 0.0; }
}

void getInputs(unsigned int layer, std::array<double, 6>& staveMean, std::array<double, 6>& staveDev, std::array<double, 6>& sensorDev)
{

  if(layer < 3)
    {
      getMvtxInputs( staveMean, staveDev, sensorDev);
    }
  else if(layer >2 && layer < 7)
    {
      getInttInputs( staveMean, staveDev, sensorDev);
    }
  else if(layer > 6 && layer < 55)
    {
      getTpcInputs( staveMean, staveDev, sensorDev);
    }
  else
    {
      getMmsInputs( staveMean, staveDev, sensorDev);
    }

}

std::array<double, 6> getOffset(unsigned int layer, unsigned int stave)
{
  std::array<double, 6> staveMean = {0,0,0,0,0,0};
  std::array<double, 6> staveDev = {0,0,0,0,0,0};
  std::array<double, 6> sensorDev = {0,0,0,0,0,0};
  getInputs(layer, staveMean, staveDev, sensorDev);

  std::array<double, 6> offset={0,0,0,0,0,0};

  for(int i=0;i<6;++i)
    {
      if(staveDev.at(i) != 0 && stave != stave_now.at(i))
	{
	  // new stave or sector, need new perturbation value
	  std::normal_distribution<double> distribution(staveMean.at(i), staveDev.at(i));
	  double perturbation = distribution(generator);
	  offset.at(i) = perturbation;
	  stave_now.at(i) = stave;
	  stave_perturbation_now.at(i) = perturbation;
	}
      else
	{
	  // same stave or sector, use the same perturbation
	  offset.at(i) = stave_perturbation_now.at(i);
	}

      if(sensorDev[i] != 0)
	{
	  std::normal_distribution<double> distribution(offset.at(i), sensorDev.at(i));
	  double perturbation = distribution(generator);
	  offset.at(i) += perturbation;
	}
    }
  return offset; 
}

std::array<double, 6> getOffsetTpc(unsigned int layer, unsigned int physical_sector)
{
  std::array<double, 6> staveMean = {0,0,0,0,0,0};
  std::array<double, 6> staveDev = {0,0,0,0,0,0};
  std::array<double, 6> sensorDev = {0,0,0,0,0,0};
  getInputs(layer, staveMean, staveDev, sensorDev);

  std::array<double, 6> offset={0,0,0,0,0,0};

  for(int i=0;i<6;++i)
    {
      if(staveDev.at(i) != 0 && physical_sector != physical_sector_now.at(i))
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
	  std::normal_distribution<double> distribution(offset.at(i), sensorDev.at(i));
	  double perturbation = distribution(generator);
	  offset.at(i) += 0.0;
	}
    }
  return offset; 
}

void makeAlignmentParams()
{
  // mvtxdat for each layer: rMin, rMid, rMax, NChip/Stave, phi0, nStaves
  double mvtxdat[3][6] = {{24.61, 25.23, 27.93, 9., 0.285, 12.}, {31.98, 33.36, 36.25, 9., 0.199, 16.},{39.93, 41.48, 44.26, 9., 0.166, 20.}};

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
	    std::array<double, 6> offset = getOffset(layer, stave);
	    alpha = offset.at(0); beta = offset.at(1); gamma = offset.at(2); dx = offset.at(3); dy = offset.at(4); dz = offset.at(5);
	    fw << hitSetKey << " " << alpha << " " << beta  << " " << gamma  << " " << dx  << " " << dy  << " " << dz  << std::endl;	
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
	    std::array<double, 6> offset = getOffset(layer, stave);
	    alpha = offset.at(0); beta = offset.at(1); gamma = offset.at(2); dx = offset.at(3); dy = offset.at(4); dz = offset.at(5);
	    fw << hitSetKey << " " << alpha << " " << beta  << " " << gamma  << " " << dx  << " " << dy  << " " << dz  << std::endl;	
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

		  std::array<double, 6> offset = getOffsetTpc(sphenix_layer, physical_sector);
		  alpha = offset.at(0); beta = offset.at(1); gamma = offset.at(2); dx = offset.at(3); dy = offset.at(4); dz = offset.at(5);
		  //fw << side << "  " << sector << "  " << region << "  " << physical_sector << "  " << layer << "  " << sphenix_layer << "  " << alpha << " " << beta  << " " << gamma  << " " << dx  << " " << dy  << " " << dz  << std::endl;
		  fw << hitSetKey <<" " << alpha << " " << beta  << " " << gamma  << " " << dx  << " " << dy  << " " << dz  << std::endl;
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
	std::array<double, 6> offset = getOffset(layer, tile);
	alpha = offset.at(0); beta = offset.at(1); gamma = offset.at(2); dx = offset.at(3); dy = offset.at(4); dz = offset.at(5);
	fw << hitSetKey <<" " << alpha << " " << beta  << " " << gamma  << " " << dx  << " " << dy  << " " << dz  << std::endl;
      }
  }

  
  fw.close();

}
