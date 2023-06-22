#include <trackermillepedealignment/AlignmentDefs.h>

void makePedeConstraintsFile(int mvtxgrp = 2, int inttgrp = AlignmentDefs::inttGrp::chp, int tpcgrp = AlignmentDefs::tpcGrp::sctr)
{
  // MVTX
  // possible procedure for MVTX
  // start with group 3 and no constraints, to get average clamshell offsets
  // Go to group 2 and constrain layers to add up to average clamshell offsets
  // Go to group 1 and constrain staves to add up to average layer offsets
  // Go to group 0 and constrain sensors to add up to average stave offsets
  // Practically: Either (equivalently):
  //   Implement this by inputting the optimized clamshell, layer, stave parameters after each step and set constraints to zero
  //   Or input the optimized clamshell, layer, stave parameters in pede through the constraint values
  // The first option is easier to implement


  // return all possible global labels for this MVTX grouping 
  std::vector<int> mvtx_glbl_labels = AlignmentDefs::getAllMvtxGlobalLabels(mvtxgrp);

  for(unsigned int i=0;i<mvtx_glbl_labels.size(); ++i)
    {
      std::cout << " label index " << i << " label " << mvtx_glbl_labels[i] << std::endl; 
    }
	
  if(mvtxgrp == 3)
    {
      std::cout << " For mvtxgrp 3 we apply no constraints " << std::endl;
    }

  if(mvtxgrp == 2)
    {
      // Grouped by layer within each clamshell
      // Constrain all layer (dx,dy,dz) parameters to total to the clamshell offset.
      // sum(layer glbl_labels * 1) = 0.0 for (dx,dy,dz) 
      std::vector<int> shell_labels[2][3];
      for(unsigned int i=0;i<mvtx_glbl_labels.size(); ++i)
	{
	  std::cout << " label index " << i << " label " << mvtx_glbl_labels[i] << std::endl; 
	  int layer = mvtx_glbl_labels[i] / 1000000;
	  int clamshell =  (mvtx_glbl_labels[i] - layer *1000000) / 10000;

	  shell_labels[clamshell][layer].push_back(mvtx_glbl_labels[i]);
	}

      std::vector<int> mvtx_constraint[2][3];
      for (unsigned int ishl=0; ishl < 2; ++ishl)
	{
	  for(unsigned int ilyr=0; ilyr < 3; ++ilyr)	  
	    {
	      for(unsigned int ip = 0; ip < shell_labels[ishl][ilyr].size(); ++ip)
		{
		  int par =  shell_labels[ishl][ilyr][ip] -ilyr * 1000000 -ishl*10000;

		  std::cout << " ishl " << ishl << " ilyr " << ilyr << " par " << par << std::endl;

		  // add to constraint equation
		  // This makes 18 constraint equations
		  mvtx_constraint[ishl][ip].push_back(shell_labels[ishl][ilyr][ip]);
		}
	    }
	}
      // write the constraints file
      ofstream fconstraint("mvtx_constraints_layer_shell.txt");

      for (unsigned int ishl=0; ishl < 2; ++ishl)
	{
	  for(unsigned int ip=0;ip<3;++ip)
	    {
	      fconstraint << "Constraint 0.0" << std::endl;
	      for(unsigned int ilyr = 0; ilyr < 3; ++ilyr)
		{
		  fconstraint << mvtx_constraint[ishl][ip][ilyr] << "  " << 1.0 << std::endl;
		}
	    }
	}
      fconstraint.close();      
    }

  if(mvtxgrp == 1)
    {
      // grouped by stave within each layer

      std::vector<int> layer_labels[2][3][3];
      for(unsigned int i=0;i<mvtx_glbl_labels.size(); ++i)
	{
	  std::cout << " label index " << i << " label " << mvtx_glbl_labels[i] << std::endl; 
	  int layer = mvtx_glbl_labels[i] / 1000000;
	  int stave =  (mvtx_glbl_labels[i] - layer *1000000) / 10000;
	  int clamshell = AlignmentDefs::getMvtxClamshell(layer, stave);
	  int sensor = (mvtx_glbl_labels[i] - layer *1000000 -stave*10000) / 10;
	  int ipar =  mvtx_glbl_labels[i] - layer *1000000 -stave*10000 - sensor*10;

	  int ip = ipar-4;
	  layer_labels[clamshell][layer][ip].push_back(mvtx_glbl_labels[i]);   // stave is in this clamshell, layer for parameter ipar

	  std::cout << "ishl " << clamshell << " ilyr " << layer << " stave " << stave << " ip " << ipar <<  " stave vec size " << layer_labels[clamshell][layer][ip].size() << std::endl;
	}

      // write the constraints file
      ofstream fconstraint("mvtx_constraints_stave_layer.txt");

      for (unsigned int ishl=0; ishl < 2; ++ishl)
	{
	  for(unsigned int ilyr = 0; ilyr < 3; ++ilyr)
	    {
	      for(unsigned int ip=0;ip<3;++ip)
		{
		  std::cout << "ishl " << ishl << " ilyr " << ilyr << " ip " << ip <<  " stave vec size " << layer_labels[ishl][ilyr][ip].size() << std::endl;
		  fconstraint << "Constraint 0.0" << std::endl;
		  for(unsigned int istv = 0; istv <  layer_labels[ishl][ilyr][ip].size(); ++istv)
		    {
		      std::cout << "     istv " << istv << " label " <<  layer_labels[ishl][ilyr][ip][istv] << std::endl;
		      fconstraint << layer_labels[ishl][ilyr][ip][istv] << "  " << 1.0 << std::endl;
		    }
		}
	    }
	}
      fconstraint.close();
    }

  if(mvtxgrp == 0)
    {
      // grouped by sensor within each stave 

      std::vector<int> stave_labels[2][3][20][3];
      for(unsigned int i=0;i<mvtx_glbl_labels.size(); ++i)
	{
	  std::cout << " label index " << i << " label " << mvtx_glbl_labels[i] << std::endl; 
	  int layer = mvtx_glbl_labels[i] / 1000000;
	  int stave =  (mvtx_glbl_labels[i] - layer *1000000) / 10000;
	  int clamshell = AlignmentDefs::getMvtxClamshell(layer, stave);
	  int sensor = (mvtx_glbl_labels[i] - layer *1000000 -stave*10000) / 10;
	  int ipar =  mvtx_glbl_labels[i] - layer *1000000 -stave*10000 - sensor*10;

	  int ip = ipar-4;
	  stave_labels[clamshell][layer][stave][ip].push_back(mvtx_glbl_labels[i]); 

	  std::cout << "ishl " << clamshell << " ilyr " << layer << " stave " << stave << " sensor " << sensor << " ip " << ipar <<  " sensor vec size " << stave_labels[clamshell][layer][stave][ip].size() << std::endl;
	}

      // write the constraints file
      ofstream fconstraint("mvtx_constraints_sensor_stave.txt");

      for (int ishl=0; ishl < 2; ++ishl)
	{
	  for(unsigned int ilyr = 0; ilyr < 3; ++ilyr)
	    {
	      for(int istv = 0; istv < AlignmentDefs::nstaves_layer_mvtx[ilyr]; ++istv)
		{
		  int clamshell = AlignmentDefs::getMvtxClamshell(ilyr, istv);
		  std::cout << "     ishl " << ishl << " clamshell " << clamshell << std::endl;
		  if(clamshell != ishl) continue;

		  for(unsigned int ip=0;ip<3;++ip)
		    {
		      std::cout << "ishl " << ishl << " ilyr " << ilyr << " istv " << istv << " ip " << ip 
				<<  " sensor vec size " << stave_labels[ishl][ilyr][istv][ip].size() << std::endl;
		      fconstraint << "Constraint 0.0" << std::endl;
		      for(unsigned int isens = 0; isens <  stave_labels[ishl][ilyr][istv][ip].size(); ++isens)
			{
			  std::cout << "     isens " << isens << " label " <<  stave_labels[ishl][ilyr][istv][ip][isens] << std::endl;
			  fconstraint << stave_labels[ishl][ilyr][istv][ip][isens] << "  " << 1.0 << std::endl;
			}
		    }
		}
	    }	  
	}
      fconstraint.close();
    }


  // INTT

  // return all possible global labels for this INTT grouping 
  std::vector<int> intt_glbl_labels = AlignmentDefs::getAllInttGlobalLabels(inttgrp);

  if(inttgrp == AlignmentDefs::inttGrp::inttbrl)
    {
      std::cout << " For inttgrp " << AlignmentDefs::inttGrp::inttbrl << " we apply no constraints " << std::endl;
    }

  if(inttgrp ==  AlignmentDefs::inttGrp::inttlyr)
    {
      // constrain all layer parameters to total to the INTT offset.
      std::vector<int> layer_labels[3];
      for(unsigned int i=0;i<intt_glbl_labels.size(); ++i)
	{
	  std::cout << " label index " << i << " label " << intt_glbl_labels[i] << std::endl; 
	  int layer = intt_glbl_labels[i] / 1000000;
	  int ipar =  intt_glbl_labels[i] - layer *1000000;
	  layer_labels[ipar-4].push_back(intt_glbl_labels[i]);
	}
      
      // write the constraints file
      ofstream fconstraint("intt_constraints_layer_barrel.txt");

      for(unsigned int ip=0;ip<3;++ip)
	{
	  fconstraint << "Constraint 0.0" << std::endl;
	  for(unsigned int ilyr = 0; ilyr < 4; ++ilyr)
	    {
	      fconstraint << layer_labels[ip][ilyr] << "  " << 1.0 << std::endl;
	    }
	}
    
      fconstraint.close();      

    }

  if(inttgrp ==  AlignmentDefs::inttGrp::lad)
    {
      // constrain all stave parameters to total to the layer offset.
      std::vector<int> layer_labels[3][4];
      for(unsigned int i=0;i<intt_glbl_labels.size(); ++i)
	{
	  std::cout << " label index " << i << " label " << intt_glbl_labels[i] << std::endl; 
	  int layer = intt_glbl_labels[i] / 1000000;
	  int stave =  (intt_glbl_labels[i] - layer *1000000) / 10000;
	  int sensor = (intt_glbl_labels[i] - layer *1000000 -stave*10000) / 10;
	  int ipar =  intt_glbl_labels[i] - layer *1000000 -stave*10000 - sensor*10;

	  int ip = ipar-4;
	  int ilyr = layer -3;
	  layer_labels[ip][ilyr].push_back(intt_glbl_labels[i]);   // stave is in this layer for parameter ipar

	  std::cout << " ilyr " << ilyr << " stave " << stave << " ip " << ip <<  " stave vec size " << layer_labels[ip][ilyr].size() << std::endl;
	}

      // write the constraints file
      ofstream fconstraint("intt_constraints_stave_layer.txt");

      for(unsigned int ilyr = 0; ilyr < 4; ++ilyr)
	{
	  for(unsigned int ip=0;ip<3;++ip)
	    {
	      std::cout << " ilyr " << ilyr << " ip " << ip <<  " stave vec size " << layer_labels[ip][ilyr].size() << std::endl;
	      fconstraint << "Constraint 0.0" << std::endl;
	      for(unsigned int istv = 0; istv <  layer_labels[ip][ilyr].size(); ++istv)
		{
		  std::cout << "     istv " << istv << " label " <<  layer_labels[ip][ilyr][istv] << std::endl;
		  fconstraint << layer_labels[ip][ilyr][istv] << "  " << 1.0 << std::endl;
		}
	    }
	}
    
      fconstraint.close();
    }

  if(inttgrp ==  AlignmentDefs::inttGrp::chp)
    {
      // constrain all sensor parameters to total to the stave offset

      std::vector<int> stave_labels[4][20][3];
      for(unsigned int i=0;i<intt_glbl_labels.size(); ++i)
	{
	  std::cout << " label index " << i << " label " << intt_glbl_labels[i] << std::endl; 
	  int layer = intt_glbl_labels[i] / 1000000;
	  int stave =  (intt_glbl_labels[i] - layer *1000000) / 10000;
	  int sensor = (intt_glbl_labels[i] - layer *1000000 -stave*10000) / 10;
	  int ipar =  intt_glbl_labels[i] - layer *1000000 -stave*10000 - sensor*10;

	  int ip = ipar-4;
	  int ilyr = layer-3;
	  stave_labels[ilyr][stave][ip].push_back(intt_glbl_labels[i]); 

	  std::cout << " ilyr " << layer << " stave " << stave << " sensor " << sensor << " ip " << ipar <<  " sensor vec size " << stave_labels[ilyr][stave][ip].size() << std::endl;
	}

      // write the constraints file
      ofstream fconstraint("intt_constraints_sensor_stave.txt");

      for(unsigned int ilyr = 0; ilyr < 4; ++ilyr)
	{
	  for(int istv = 0; istv < AlignmentDefs::nstaves_layer_intt[ilyr]; ++istv)
	    {
	      for(unsigned int ip=0;ip<3;++ip)
		{
		  std::cout << " ilyr " << ilyr << " istv " << istv << " ip " << ip 
			    <<  " sensor vec size " << stave_labels[ilyr][istv][ip].size() << std::endl;
		  fconstraint << "Constraint 0.0" << std::endl;
		  for(unsigned int isens = 0; isens <  stave_labels[ilyr][istv][ip].size(); ++isens)
		    {
		      std::cout << "     isens " << isens << " label " <<  stave_labels[ilyr][istv][ip][isens] << std::endl;
		      fconstraint << stave_labels[ilyr][istv][ip][isens] << "  " << 1.0 << std::endl;
		    }
		}
	    }
	}	  
	
      fconstraint.close();

    }


  // TPC

  std::vector<int> tpc_glbl_labels = AlignmentDefs::getAllTpcGlobalLabels(tpcgrp);

  if(tpcgrp ==  AlignmentDefs::tpcGrp::sctr)
    {
      // constrain all sector parameters to total to the TPC offset.
      std::vector<int> sector_labels[3];
      for(unsigned int i=0;i<tpc_glbl_labels.size(); ++i)
	{
	  std::cout << " label index " << i << " label " << tpc_glbl_labels[i] << std::endl; 
	  int layer = tpc_glbl_labels[i] / 1000000;
	  int sector =  (tpc_glbl_labels[i] - layer *1000000) / 10000;
	  int sensor = (tpc_glbl_labels[i] - layer *1000000 -sector*10000) / 10;
	  int ipar =  tpc_glbl_labels[i] - layer *1000000 -sector*10000 - sensor*10;

	  int ip = ipar-4;
	  sector_labels[ip].push_back(tpc_glbl_labels[i]); 

	  std::cout << " ilyr " << layer << " sector " << sector << " ip " << ipar <<  " sector vec size " << sector_labels[ip].size() << std::endl;
	}

      // write the constraints file
      ofstream fconstraint("tpc_constraints_sector_tpc.txt");

      for(unsigned int ip=0;ip<3;++ip)
	{
	  std::cout << " ip " << ip 
		    <<  " sector vec size " << sector_labels[ip].size() << std::endl;
	  fconstraint << "Constraint 0.0" << std::endl;
	  for(unsigned int isec = 0; isec <  sector_labels[ip].size(); ++isec)
	    {
	      std::cout << "     isec " << isec << " label " <<  sector_labels[ip][isec] << std::endl;
	      fconstraint << sector_labels[ip][isec] << "  " << 1.0 << std::endl;
	    }
	}
	
      fconstraint.close();
      
    }
}



