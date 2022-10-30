#include <iostream>
#include <string>
#include <fstream>
#include <trackbase/MvtxDefs.h>
#include <trackbase/InttDefs.h>
#include <trackbase/TpcDefs.h>
#include <MicromegasDefs.h>
#include <trackbase/ActsGeometry.h>

#include <cmath>

// Creates an alignment corrections file containing all zero alignment corrections

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

	    fw << hitSetKey << " " << alpha << " " << beta  << " " << gamma  << " " << dx  << " " << dy  << " " << dz  << std::endl;
	    alpha = 0; beta = 0; gamma = 0; dx = 0; dy = 0; dz = 0;
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

	    fw << hitSetKey << " " << alpha << " " << beta  << " " << gamma  << " " << dx  << " " << dy  << " " << dz  << std::endl;
	    alpha = 0; beta = 0; gamma = 0; dx = 0; dy = 0; dz = 0;
	  }
      }
    }

  // Create TPC hitsetkeys
  for (unsigned int layer = 7; layer<=54; layer++)          // loop over layers in TPC
  {    
    for(unsigned int side = 0; side < 2; side++)        // loops over sides
      {    
	for(unsigned int sector = 0; sector < 12; sector++)     // loop over sectors
	  {
	    TrkrDefs::hitsetkey hitSetKey = TpcDefs::genHitSetKey(layer,sector,side);
  	    fw << hitSetKey <<" " << alpha << " " << beta  << " " << gamma  << " " << dx  << " " << dy  << " " << dz  << std::endl;
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
	fw << hitSetKey <<" " << alpha << " " << beta  << " " << gamma  << " " << dx  << " " << dy  << " " << dz  << std::endl;
      }
  }

  
  fw.close();

}
