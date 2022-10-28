#include <trackbase/MvtxDefs.h>
#include <trackbase/InttDefs.h>
#include <trackbase/TpcDefs.h>
#include <MicromegasDefs.h>
#include <trackbase/ActsGeometry.h>

#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void populate_mvtx_stave(int layer, int stave, ofstream& fout, float params[])
{ 
  // mvtxdat for each layer: rMin, rMid, rMax, NChip/Stave, phi0, nStaves
  double mvtxdat[3][6] = {{24.61, 25.23, 27.93, 9., 0.285, 12.}, {31.98, 33.36, 36.25, 9., 0.199, 16.},{39.93, 41.48, 44.26, 9., 0.166, 20.}};
  int staveNum = mvtxdat[layer][5];             // Number of staves per layer
  
  for(int chip = 0; chip <= 8; chip++)      // loops over chips 
    {
      TrkrDefs::hitsetkey hitSetKey = MvtxDefs::genHitSetKey(layer,stave,chip,0);	  
      fout << hitSetKey << " " <<params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
    }  
 }

 void populate_entire_mvtx_layer(int layer, ofstream& fout, float params[])
{ 
  // Create MVTX hitsetkeys
  // mvtxdat for each layer: rMin, rMid, rMax, NChip/Stave, phi0, nStaves
  double mvtxdat[3][6] = {{24.61, 25.23, 27.93, 9., 0.285, 12.}, {31.98, 33.36, 36.25, 9., 0.199, 16.},{39.93, 41.48, 44.26, 9., 0.166, 20.}};
  int staveNum = mvtxdat[layer][5];             // Number of staves per layer
  
  for(int stave = 0; stave < staveNum; stave++) // loop over staves
    {    
      populate_mvtx_stave(layer, stave, fout, params);


      //      for(int chip = 0; chip <= 8; chip++)      // loops over chips 
      //{
      //TrkrDefs::hitsetkey hitSetKey = MvtxDefs::genHitSetKey(layer,stave,chip,0);	  
      //fout << hitSetKey << " " <<params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
    //}
    }
 }

void populate_intt_stave(int layer, int stave, ofstream& fout, float params[])
{ 
  for(int chip = 0; chip <= 3; chip++)       // loops over chips 
    {
      TrkrDefs::hitsetkey hitSetKey = InttDefs::genHitSetKey(layer,chip,stave,0);
      fout << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
    }
   }

void populate_entire_intt_layer(int layer, ofstream& fout, float params[])
{ 
 // Create INTT hitsetkeys
  int nladder[4] = {12, 12, 16, 16};

  int inttlayer = layer - 3;
  int staveNum = nladder[inttlayer];             // Number of staves per layer
    
  for(int stave = 0; stave < staveNum; stave++)  // loop over staves
    {    
      populate_intt_stave(layer, stave, fout, params);

      //      for(int chip = 0; chip <= 3; chip++)       // loops over chips 
		//{
      //TrkrDefs::hitsetkey hitSetKey = InttDefs::genHitSetKey(layer,chip,stave,0);
	  //fout << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
	//}
    }
 }

void populate_tpc_sector(int layer, int sector, ofstream& fout, float params[])
{ 
  for(unsigned int side = 0; side < 2; side++)        // loops over sides
    {
      TrkrDefs::hitsetkey hitSetKey = TpcDefs::genHitSetKey(layer,sector,side);
      fout << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
    }
}

 void populate_entire_tpc_layer(int layer, ofstream& fout, float params[])
{
  // Create TPC hitsetkeys
  for(unsigned int sector = 0; sector < 12; sector++)     // loop over sectors
    {    
      populate_tpc_sector(layer, sector, fout, params);

      //      for(unsigned int side = 0; side < 2; side++)        // loops over sides
      //{
      //TrkrDefs::hitsetkey hitSetKey = TpcDefs::genHitSetKey(layer,sector,side);
      //fout << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
      //	}
    }
 }

void populate_mms_tile(int layer, int tile, ofstream& fout, float params[])
{ 
  unsigned short segmentation = 0;
  if(layer == 55){segmentation=1;}
  else if(layer == 56){segmentation=0;}
  MicromegasDefs::SegmentationType mmDefsSeg =  (MicromegasDefs::SegmentationType) segmentation;
  
  TrkrDefs::hitsetkey hitSetKey = MicromegasDefs::genHitSetKey(layer, mmDefsSeg, tile);
  fout << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
  
}

 void populate_entire_mms_layer(int layer, ofstream& fout, float params[])
{
  // Create Micromegas hitsetkeys
  //  unsigned short segmentation = 0;  
  //if(layer == 55){segmentation=1;}
  //else if(layer == 56){segmentation=0;}
  
  //  MicromegasDefs::SegmentationType mmDefsSeg =  (MicromegasDefs::SegmentationType) segmentation;
  
  for(unsigned int tile = 0; tile < 8; tile++)        // loops over tiles with each tile having corresponding segmentation values
    {
      populate_mms_tile(layer, tile, fout, params);
      //      TrkrDefs::hitsetkey hitSetKey = MicromegasDefs::genHitSetKey(layer, mmDefsSeg, tile);
      //fout << hitSetKey <<" " << params[0] << " " << params[1]  << " " << params[2]  << " " << params[3]  << " " << params[4]  << " " << params[5]  << std::endl;
    }
}
      
TrkrDefs::hitsetkey getHitSetKey(int layer, int stave, int sensor)
{
 TrkrDefs::hitsetkey hitSetKey = 0;

  if(layer < 3)
    hitSetKey = MvtxDefs::genHitSetKey(layer,stave,sensor,0);

  if(layer >2 && layer < 7)
    hitSetKey = InttDefs::genHitSetKey(layer,sensor,stave,0);

  if(layer > 6 && layer < 55)
    {
      // need to deduce side somehow
      // sector will have surfaces from both sides?
      int sector = stave;
      int side = 0;
      hitSetKey = TpcDefs::genHitSetKey(layer, sector, side);
    }

  if(layer > 54)
    {
      unsigned short segmentation = 0;  
      if(layer == 55){segmentation=1;}
      else if(layer == 56){segmentation=0;}
      MicromegasDefs::SegmentationType mmDefsSeg =  (MicromegasDefs::SegmentationType) segmentation;
      int tile = stave;  
      hitSetKey = MicromegasDefs::genHitSetKey(layer, mmDefsSeg, tile);
    }

    return hitSetKey;
}

void populate_entire_layer(int layer, ofstream& fout, float params[])
{
  // fill this layer with the same alignment parameters

  if(layer < 3)
    populate_entire_mvtx_layer(layer, fout, params);

  if(layer > 2 && layer < 7)
    populate_entire_mvtx_layer(layer, fout, params);

  if(layer > 6 && layer < 55)
    populate_entire_tpc_layer(layer, fout, params);

  if(layer > 54)
    populate_entire_mms_layer(layer, fout, params);
  
  return;
 }

void getParameters(std::vector<std::pair<int, float>> par_vec, float parameters[])
{ 
  for(unsigned int ip = 0; ip < par_vec.size(); ++ip)
    {
      int ipar = par_vec[ip].first;
      if((unsigned int) ipar != ip+1)
	{
	  std::cout << " Error: ipar " << ipar << " ip " << ip << std::endl;
	}
      float align = par_vec[ip].second;
      parameters[ip] = align;
    }	
  }

void populate_stave(int layer, int stave, ofstream& fout, float params[])
{
  // stave can be stave, ladder, sector or tile

  if(layer < 3)
    populate_mvtx_stave(layer, stave, fout, params);

  if(layer > 2 && layer < 7)
    populate_intt_stave(layer, stave, fout, params);

  if(layer > 6 && layer < 55)
    populate_tpc_sector(layer, stave, fout, params);

  if(layer > 54)
    populate_mms_tile(layer, stave, fout, params);
  
  return;

}

void process_millepede_results()
{
  // macro to read in millepede.res files (pede output files) and process them
  // into a new alignment parameters file with one line for every surface
  // The millepede.res file contains entries only for parameters given non zero alignment derivatives
  // For sensors where the alignment parameter derivatives were zero, set the parameters to zero

  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

  int nladders_layer[7] = {12, 16, 20, 12, 12, 16, 16};

  ifstream fin("millepede.res");
  //ifstream fin("millepede_allsensors_silicon.res");

  if(!fin.is_open()) std::cout << "Unable to open file" << std::endl;

  int label = 0;
  float align = 0.0;

  std::string line;
  getline(fin,line);
  std::cout << line << std::endl;  // no data in first line

  // Depending on the surface grouping settings, we may have:
  //    a line for every sensor in part or all of the tracker
  //    a line for every stave or sector in part or all of the tracker
  //    a line for an entire subdetector
  // or a mixture of these. We have to figure out what we have in the file.

 int layer_keep = 0;
  int stave_keep = 0;
  int sensor_keep = 0;
  std::vector<std::pair<int, float>> par_vec;
  std::vector<std::vector<std::pair<int, float>>> sensor_vec;
  std::vector<std::vector<std::vector<std::pair<int, float>>>> stave_vec;
  std::map<int, std::vector<std::vector<std::vector<std::pair<int, float>>>>> layer_stave_vec_map;

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

      std::cout << " Input: layer " << layer << " stave " << stave << " sensor " << sensor << " ipar " << ipar << " label " << label << " align " << align << std::endl;

      if(layer > layer_keep || fin.eof())
	{
	  // layer layer_keep is done.
	  std::cout << "   fitted staves in layer " << layer_keep << " = " << stave_keep+1 << " sensors = " << sensor_keep+1 << std::endl; 

	  //flush everything
	  sensor_vec.push_back(par_vec);
	  stave_vec.push_back(sensor_vec);
	  layer_stave_vec_map.insert( std::make_pair(layer_keep, stave_vec) );

	  // reset for the new layer
	  layer_keep = layer;
	  stave_keep = 0;
	  sensor_keep = 0;
	  stave_vec.clear();
	  sensor_vec.clear();
	  par_vec.clear();
	}      

       if(sensor > sensor_keep) { 
	 // new sensor, close out par_vec
	 sensor_vec.push_back(par_vec);
	 par_vec.clear();
	 sensor_keep = sensor; 
       }

       if(stave > stave_keep) { 
	 // new stave, close out sensor_vec and par_vec
	 sensor_vec.push_back(par_vec);
	 stave_vec.push_back(sensor_vec);
	 sensor_vec.clear();
	 par_vec.clear();
	 stave_keep = stave;  
       }	
 
          par_vec.push_back(std::make_pair(ipar, align));
       
    }  // end loop over file lines

  // output the parameters
  ofstream fout("new_alignment_corrections.txt");	

  for(int layer=0; layer < 57; ++layer)
    {
      auto it = layer_stave_vec_map.find(layer);
      if (it == layer_stave_vec_map.end())
	{
	  float params[6] = {0,0,0,0,0,0};
	  populate_entire_layer(layer, fout, params);
	  continue;
	}
      auto stave_vec = it->second;
      if(stave_vec.size() == 1) 
	{
	  std::cout << " All staves are grouped together in layer " << layer << std::endl;
	  // fill in all stave and sensor lines for this layer using the single parameter set for the only sensor entry
	  // using  populate_entire_layer(layer, fout, params);
	}

      for(unsigned int ivec=0;ivec<stave_vec.size(); ++ivec)
	{
	  int stave = ivec;
	  auto sensor_vec = stave_vec[ivec];

	  if(sensor_vec.size() == 1) 
	    {
	      std::cout << " All sensors are grouped together for layer " << layer << " stave " << stave << std::endl;
	      // fill in all sensor lines for this stave, using the single parameter set for this stave's only sensor entry
	      auto par_vec = sensor_vec[0];
	      float parameters[6];
	      getParameters(par_vec, parameters);

	      // need a populate_stave method
	      populate_stave(layer, stave, fout, parameters);
	      continue;
	    }

	  for(unsigned int is = 0; is < sensor_vec.size(); ++is)
	    {
	      int sensor = is;	      
	      auto par_vec = sensor_vec[is];
	      float parameters[6];
	      getParameters(par_vec, parameters);
	      // generate a hitsetkey for this entry
	      TrkrDefs::hitsetkey hitSetKey = getHitSetKey(layer, stave, sensor);
	      fout << hitSetKey
		   << parameters[0] << " " << parameters[1] << " "  << parameters[2] << " "
		   << parameters[3] << " " << parameters[4] << " "  << parameters[5]  << std::endl;
	    }
	}
    }
  fout.close(); 
  
}
