makeMapping_FEMC_fsPHENIX()
{

  /* Global detector position / transformation */
  double femc_x0 = 0.0; // cm,
  double femc_y0 = 0.0; // cm,
  double femc_z0 = 0.0; // cm,

  double femc_rot_x0 = 0.0;
  double femc_rot_y0 = 0.0;
  double femc_rot_z0 = 0.0;

  /* Detector envelope size (cone shape) */
  double femc_rmin1 = 0; // cm
  double femc_rmax1 = 0; // cm
  double femc_rmin2 = 0; // cm
  double femc_rmax2 = 0; // cm

  /* Detector calculation envelope (rectangular shape) */
  double femc_xmin = 0; // cm
  double femc_xmax = 0; // cm
  double femc_ymin = 0; // cm
  double femc_ymax = 0; // cm

  double femc_dz = 0; // cm

  /* Tower parameters */
  double tower_dx = 0.0; // cm
  double tower_dy = 0.0; // cm
  double tower_dz = 0.0; // cm

  unsigned int twr_count = 0; 
  unsigned int twr_count0 = 0; 
  unsigned int twr_count1 = 0; 

  // create map
  ofstream fout("towerMap_FEMC_fsPHENIX_latest.txt");

  // PbGl modules first - the envelope for the geometry file is also 
  // written out here.

  {
      /* Global detector position / transformation */
      femc_x0 =  0.0; // cm,
      femc_y0 =  0.0; // cm,
      femc_z0 = 305.0; // cm,

      femc_rot_x0 =  0.0;
      femc_rot_y0 =  0.0;
      femc_rot_z0 =  0.0;

      /* Detector envelope size (cone shape) */
      femc_rmin1 = 4.0; // cm
      femc_rmax1 = 183; // cm
      femc_rmin2 = 4.0; // cm
      femc_rmax2 = 183; // cm

      /* Detector size (square array) */
      femc_xmin = 22.0; // cm
      femc_xmax = 182.655; // cm
      femc_ymin = 22.0; // cm
      femc_ymax = 182.655; // cm
      femc_dz = 40.0; // cm (FULL SIZE)
 
      /* Tower parameters */
      // From PHENIX PbGl JGL 3/14/2016
      tower_dx = 4.0; // cm
      tower_dy = 4.0; // cm
      tower_dz = 40.0; // cm (FULL SIZE)

      float towers_per_supermodule_x = 6; 
      float towers_per_supermodule_y = 4; 

      // The (former) PbGl crystals are placed in an array
      // surrounding the MPC crystals.  This arrangement tries to respect the 6x4 arrangement
      // of the supermodules as much as possible, except for a layer of three modules in height 
      // along the centerline of the detector. 

      /* Global detector transformation */
      fout << "#Global detector geometry and transformation; lengths given in cm" << endl;
      fout << "Gtype " << 1 << endl;
      fout << "Gr1_inner " << femc_rmin1 << endl;
      fout << "Gr1_outer " << femc_rmax1 << endl;
      fout << "Gr2_inner " << femc_rmin2 << endl;
      fout << "Gr2_outer " << femc_rmax2 << endl;
      fout << "Gdz " << femc_dz << endl;
      fout << "Gx0 " << femc_x0 << endl;
      fout << "Gy0 " << femc_y0 << endl;
      fout << "Gz0 " << femc_z0 << endl;
      fout << "Grot_x " << femc_rot_x0 << endl;
      fout << "Grot_y " << femc_rot_y0 << endl;
      fout << "Grot_z " << femc_rot_z0 << endl;
      fout << "Gtower0_dx " << tower_dx << endl;
      fout << "Gtower0_dy " << tower_dy << endl;
      fout << "Gtower0_dz " << tower_dz << endl;

      /* Tower mapping */
      fout << "#Tower type,idx_j,idx_k,idx_l,x[cm],y[cm],z[cm],dx[cm],dy[cm],dz[cm],rot_x,rot_y,rot_z" << endl;
      fout << "#PbGl crystals" << endl;
      fout << "#First layers (single towers)" << endl;

      // lay down the first layers along the centerline (individual modules)

      unsigned n_towers_j = 2 * ( (unsigned)( (femc_xmax/tower_dx) ));
      unsigned n_towers_k = 2 * ( (unsigned)( (femc_ymax/tower_dy) )) + 1;
      double xpos_j0_k0 = (-1 * ( (double)( n_towers_j / 2 ) * tower_dx)) + 0.5*tower_dx;
      double ypos_j0_k0 = 0.0; 
      unsigned idx_l = 0;

      for (int idx_j = 0; idx_j < n_towers_j; idx_j++)
	{
	  for (int idx_k = -1; idx_k < 2; idx_k++)
	    {

	      /* Calculate center position for tower */
	      double xpos = xpos_j0_k0 + idx_j * tower_dx;
	      double ypos = ypos_j0_k0 + idx_k * tower_dy;
	      double zpos = 0;

	      // check if all four corners are within envelope volume
	      double x_corner_1 =  fabs(xpos + tower_dx/2);
	      double y_corner_1 =  fabs(ypos + tower_dy/2);
	      double x_corner_2 =  fabs(xpos - tower_dx/2);
	      double y_corner_2 =  fabs(ypos + tower_dy/2);
	      double x_corner_3 =  fabs(xpos + tower_dx/2);
	      double y_corner_3 =  fabs(ypos - tower_dy/2);
	      double x_corner_4 =  fabs(xpos - tower_dx/2);
	      double y_corner_4 =  fabs(ypos - tower_dy/2);

	      if( ((x_corner_1<femc_xmin) && (y_corner_1<femc_ymin)) ||
		  ((x_corner_2<femc_xmin) && (y_corner_2<femc_ymin)) || 
		  ((x_corner_3<femc_xmin) && (y_corner_3<femc_ymin)) || 
		  ((x_corner_4<femc_xmin) && (y_corner_4<femc_ymin)) )
		continue; 

	      if( ((x_corner_1>femc_xmax) && (y_corner_1>femc_ymax)) ||
		  ((x_corner_2>femc_xmax) && (y_corner_2>femc_ymax)) || 
		  ((x_corner_3>femc_xmax) && (y_corner_3>femc_ymax)) || 
		  ((x_corner_4>femc_xmax) && (y_corner_4>femc_ymax)) )
		continue; 

	      double r_corner_1 = sqrt(pow(x_corner_1,2)+pow(y_corner_1,2)); 
	      double r_corner_2 = sqrt(pow(x_corner_2,2)+pow(y_corner_2,2)); 
	      double r_corner_3 = sqrt(pow(x_corner_3,2)+pow(y_corner_3,2)); 
	      double r_corner_4 = sqrt(pow(x_corner_4,2)+pow(y_corner_4,2)); 

	      if( (r_corner_1>femc_rmax1) || 
		  (r_corner_2>femc_rmax1) || 
		  (r_corner_3>femc_rmax1) || 
		  (r_corner_4>femc_rmax1) )
		continue; 

	      int f_idx_j = idx_j; 
	      int f_idx_k = n_towers_k + idx_k; 

	      fout << "Tower " << 0 << " " << f_idx_j << " " << f_idx_k << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
	      twr_count++; 
	      twr_count0++; 
	  
	    }

	}

      // lay down the remaining groups as supermodules of 6x4 modules 

      // First, the top/bottom groups above and below the modules we just laid down.  This will
      // "even out" the horizontal layer with the MPC
      fout << "#First group of supermodules" << endl;
      
      double n_SM_j = ( (unsigned)( (femc_xmax/(towers_per_supermodule_x*tower_dx)) ));
      double xpos_SM_j0_k0 = 22.0 + 0.5*(towers_per_supermodule_x*tower_dx);
      double ypos_SM_j0_k0 = 1.5*tower_dy + 0.5*(towers_per_supermodule_y*tower_dy);
      unsigned idx_l = 0;

      unsigned idx_offset = 1000; 

      for (int idx_j = 0; idx_j < n_SM_j; idx_j++)
	{

	      /* Calculate center position for tower */
	      double xpos = xpos_SM_j0_k0 + idx_j * towers_per_supermodule_x*tower_dx;
	      double ypos = ypos_SM_j0_k0;
	      double zpos = 0;

	      // check if all four corners are within envelope volume
	      double x_corner_1 =  fabs(xpos + towers_per_supermodule_x*tower_dx/2);
	      double y_corner_1 =  fabs(ypos + towers_per_supermodule_y*tower_dy/2);
	      double x_corner_2 =  fabs(xpos - towers_per_supermodule_x*tower_dx/2);
	      double y_corner_2 =  fabs(ypos + towers_per_supermodule_y*tower_dy/2);
	      double x_corner_3 =  fabs(xpos + towers_per_supermodule_x*tower_dx/2);
	      double y_corner_3 =  fabs(ypos - towers_per_supermodule_y*tower_dy/2);
	      double x_corner_4 =  fabs(xpos - towers_per_supermodule_x*tower_dx/2);
	      double y_corner_4 =  fabs(ypos - towers_per_supermodule_y*tower_dy/2);

	      if( ((x_corner_1<femc_xmin) && (y_corner_1<femc_ymin)) ||
		  ((x_corner_2<femc_xmin) && (y_corner_2<femc_ymin)) || 
		  ((x_corner_3<femc_xmin) && (y_corner_3<femc_ymin)) || 
		  ((x_corner_4<femc_xmin) && (y_corner_4<femc_ymin)) )
		continue; 

	      if( ((x_corner_1>femc_xmax) && (y_corner_1>femc_ymax)) ||
		  ((x_corner_2>femc_xmax) && (y_corner_2>femc_ymax)) || 
		  ((x_corner_3>femc_xmax) && (y_corner_3>femc_ymax)) || 
		  ((x_corner_4>femc_xmax) && (y_corner_4>femc_ymax)) )
		continue; 

	      double r_corner_1 = sqrt(pow(x_corner_1,2)+pow(y_corner_1,2)); 
	      double r_corner_2 = sqrt(pow(x_corner_2,2)+pow(y_corner_2,2)); 
	      double r_corner_3 = sqrt(pow(x_corner_3,2)+pow(y_corner_3,2)); 
	      double r_corner_4 = sqrt(pow(x_corner_4,2)+pow(y_corner_4,2)); 

	      if( (r_corner_1>femc_rmax1) || 
		  (r_corner_2>femc_rmax1) || 
		  (r_corner_3>femc_rmax1) || 
		  (r_corner_4>femc_rmax1) )
		continue; 

	      // Place the modules within the superpmodule

	      for (int m_idx_j = 0; m_idx_j < towers_per_supermodule_x; m_idx_j++)
		{
		  for (int m_idx_k = 0; m_idx_k < towers_per_supermodule_y; m_idx_k++)
		    {

		      // tower indices should be >=0
		      
		      // +x above beamline

		      double xposm = xpos - 0.5*towers_per_supermodule_x*tower_dx + (m_idx_j+0.5)*tower_dx;
		      double yposm = ypos - 0.5*towers_per_supermodule_y*tower_dy + (m_idx_k+0.5)*tower_dy;
		      
		      int f_idx_j = n_towers_j + ( (unsigned)( (fabs(xposm)/tower_dx) )); 
		      int f_idx_k = n_towers_k + 1 + m_idx_k; 

		      fout << "Tower " << 0 << " " << f_idx_j + idx_offset << " " << f_idx_k << " " << idx_l << " " << xposm << " " << yposm << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
		      twr_count++; 
		      twr_count0++; 

		      // +x below beamline

		      xposm = xpos - 0.5*towers_per_supermodule_x*tower_dx + (m_idx_j+0.5)*tower_dx;
		      yposm = -(ypos - 0.5*towers_per_supermodule_y*tower_dy + (m_idx_k+0.5)*tower_dy);
		      
		      int f_idx_j = n_towers_j + ( (unsigned)( (fabs(xposm)/tower_dx) )); 
		      int f_idx_k = n_towers_k - 1 - m_idx_k; 
		      
		      fout << "Tower " << 0 << " " << f_idx_j + idx_offset << " " << f_idx_k << " " << idx_l << " " << xposm << " " << yposm << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
		      twr_count++; 
		      twr_count0++; 
		      
		      // -x above beamline

		      xposm = -(xpos - 0.5*towers_per_supermodule_x*tower_dx + (m_idx_j+0.5)*tower_dx);
		      yposm = ypos - 0.5*towers_per_supermodule_y*tower_dy + (m_idx_k+0.5)*tower_dy;
		      
		      int f_idx_j = n_towers_j - 1 -  ( (unsigned)( (fabs(xposm)/tower_dx) )); 
		      int f_idx_k = n_towers_k + 1 + m_idx_k; 

		      fout << "Tower " << 0 << " " << f_idx_j + idx_offset << " " << f_idx_k << " " << idx_l << " " << xposm << " " << yposm << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
		      twr_count++; 
		      twr_count0++; 

		      // -x below beamline

		      int f_idx_j = n_towers_j - 1 -  ( (unsigned)( (fabs(xposm)/tower_dx) )); 
		      int f_idx_k = n_towers_k - 1 - m_idx_k; 

		      xposm = -(xpos - 0.5*towers_per_supermodule_x*tower_dx + (m_idx_j+0.5)*tower_dx);
		      yposm = -(ypos - 0.5*towers_per_supermodule_y*tower_dy + (m_idx_k+0.5)*tower_dy);
		      
		      fout << "Tower " << 0 << " " << f_idx_j + idx_offset << " " << f_idx_k << " " << idx_l << " " << xposm << " " << yposm << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
		      twr_count++; 
		      twr_count0++; 

		    }

		}

	}


      // Next, the top/bottom groups above and below the MPC array.  This will finish out the 
      // PbGl supermodules 
      fout << "#Second group of supermodules" << endl;
     
      double n_SM_j = ( (unsigned)( (femc_xmax/(towers_per_supermodule_x*tower_dx)) ));
      double n_SM_k = ( (unsigned)( (femc_ymax/(towers_per_supermodule_y*tower_dy)) ));
      double xpos_SM_j0_k0 = 0.5*(towers_per_supermodule_x*tower_dx);
      double ypos_SM_j0_k0 = 22.0 + 0.5*(towers_per_supermodule_y*tower_dy);
      unsigned idx_l = 0;

      idx_offset = 2000; 

      for (int idx_j = 0; idx_j < n_SM_j; idx_j++)
	{
	  for (int idx_k = 0; idx_k < n_SM_k; idx_k++)
	    {
	      /* Calculate center position for tower */
	      double xpos = xpos_SM_j0_k0 + idx_j * towers_per_supermodule_x*tower_dx;
	      double ypos = ypos_SM_j0_k0 + idx_k * towers_per_supermodule_y*tower_dy;
	      double zpos = 0;

	      // check if all four corners are within envelope volume
	      double r_corner_1 =  sqrt(pow((xpos + towers_per_supermodule_x*tower_dx/2),2) + 
					pow((ypos + towers_per_supermodule_y*tower_dy/2),2));
	      double r_corner_2 =  sqrt(pow((xpos - towers_per_supermodule_x*tower_dx/2),2) + 
					pow((ypos + towers_per_supermodule_y*tower_dy/2),2));
	      double r_corner_3 =  sqrt(pow((xpos + towers_per_supermodule_x*tower_dx/2),2) + 
					pow((ypos - towers_per_supermodule_y*tower_dy/2),2));
	      double r_corner_4 =  sqrt(pow((xpos - towers_per_supermodule_x*tower_dx/2),2) + 
					pow((ypos - towers_per_supermodule_y*tower_dy/2),2));

	      if( (r_corner_1>femc_rmax1) || 
		  (r_corner_2>femc_rmax1) || 
		  (r_corner_3>femc_rmax1) || 
		  (r_corner_4>femc_rmax1) )
		continue; 
	      	      
	      // Place the modules within the superpmodule

	      for (int m_idx_j = 0; m_idx_j < towers_per_supermodule_x; m_idx_j++)
		{
		  for (int m_idx_k = 0; m_idx_k < towers_per_supermodule_y; m_idx_k++)
		    {

		      // tower indices should be >=0
		      
		      // +x above beamline

		      double xposm = xpos - 0.5*towers_per_supermodule_x*tower_dx + (m_idx_j+0.5)*tower_dx;
		      double yposm = ypos - 0.5*towers_per_supermodule_y*tower_dy + (m_idx_k+0.5)*tower_dy;
		      
		      int f_idx_j = n_towers_j + ( (unsigned)( (fabs(xposm)/tower_dx) )); 
		      int f_idx_k = n_towers_k + 6 + m_idx_k + idx_k*towers_per_supermodule_y; 

		      fout << "Tower " << 0 << " " << f_idx_j + idx_offset << " " << f_idx_k << " " << idx_l << " " << xposm << " " << yposm << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
		      twr_count++; 
		      twr_count0++; 

		      // +x below beamline

		      xposm = xpos - 0.5*towers_per_supermodule_x*tower_dx + (m_idx_j+0.5)*tower_dx;
		      yposm = -(ypos - 0.5*towers_per_supermodule_y*tower_dy + (m_idx_k+0.5)*tower_dy);
		      
		      int f_idx_j = n_towers_j + ( (unsigned)( (fabs(xposm)/tower_dx) )); 
		      int f_idx_k = n_towers_k - 6 - m_idx_k - idx_k*towers_per_supermodule_y; 
		      
		      fout << "Tower " << 0 << " " << f_idx_j + idx_offset << " " << f_idx_k << " " << idx_l << " " << xposm << " " << yposm << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
		      twr_count++; 
		      twr_count0++; 
		      
		      // -x above beamline

		      xposm = -(xpos - 0.5*towers_per_supermodule_x*tower_dx + (m_idx_j+0.5)*tower_dx);
		      yposm = ypos - 0.5*towers_per_supermodule_y*tower_dy + (m_idx_k+0.5)*tower_dy;
		      
		      int f_idx_j = n_towers_j - 1 -  ( (unsigned)( (fabs(xposm)/tower_dx) )); 
		      int f_idx_k = n_towers_k + 6 + m_idx_k + idx_k*towers_per_supermodule_y; 

		      fout << "Tower " << 0 << " " << f_idx_j + idx_offset << " " << f_idx_k << " " << idx_l << " " << xposm << " " << yposm << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
		      twr_count++; 
		      twr_count0++; 

		      // -x below beamline

		      int f_idx_j = n_towers_j - 1 -  ( (unsigned)( (fabs(xposm)/tower_dx) )); 
		      int f_idx_k = n_towers_k - 6 - m_idx_k - idx_k*towers_per_supermodule_y; 

		      xposm = -(xpos - 0.5*towers_per_supermodule_x*tower_dx + (m_idx_j+0.5)*tower_dx);
		      yposm = -(ypos - 0.5*towers_per_supermodule_y*tower_dy + (m_idx_k+0.5)*tower_dy);
		      
		      fout << "Tower " << 0 << " " << f_idx_j + idx_offset << " " << f_idx_k << " " << idx_l << " " << xposm << " " << yposm << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
		      twr_count++; 
		      twr_count0++; 

		    }


		}


	    }

	}


  }

  // Next the MPC modules 

    {

      /* Detector envelope size (rectangluar shape) */
      femc_xmin = 11.0; // cm
      femc_xmax = 22.0; // cm
      femc_ymin = 11.0; // cm
      femc_ymax = 22.0; // cm

      femc_dz = 40.0; // cm (FULL SIZE)

      /* Tower parameters */
      // From PHENIX MPC JGL 3/14/2016
      tower_dx = 2.2; // cm
      tower_dy = 2.2; // cm
      tower_dz = 18.0; // cm (FULL SIZE)

      // index offset - keep indices unique
      unsigned idx_offset = 3000; 

      // The (former) MPC crystals are placed in a square array
      // 20x20 crytals wide with 4x4 hole in the center

      /* Tower mapping */
      fout << "#MPC crystals" << endl;

      fout << "Gtower1_dx " << tower_dx << endl;
      fout << "Gtower1_dy " << tower_dy << endl;
      fout << "Gtower1_dz " << tower_dz << endl;

      unsigned n_towers_j = 2 * ( (unsigned)( (femc_xmax/tower_dx) ));
      unsigned n_towers_k = 2 * ( (unsigned)( (femc_ymax/tower_dy) ));

      double xpos_j0_k0 = (-1 * ( (double)( n_towers_j / 2 ) * tower_dx)) + 0.5*tower_dx;
      double ypos_j0_k0 = (-1 * ( (double)( n_towers_k / 2 ) * tower_dy)) + 0.5*tower_dy;

      unsigned idx_l = 0;

      for (int idx_j = 0; idx_j < n_towers_j; idx_j++)
	{
	  for (int idx_k = 0; idx_k < n_towers_k; idx_k++)
	    {

	      /* Calculate center position for tower */
	      double xpos = xpos_j0_k0 + idx_j * tower_dx;
	      double ypos = ypos_j0_k0 + idx_k * tower_dy;
	      double zpos = 0.0; // back face to match up with PbGl

	      // check if all four corners are within envelope volume
	      double x_corner_1 =  fabs(xpos + tower_dx/2);
	      double y_corner_1 =  fabs(ypos + tower_dy/2);
	      double x_corner_2 =  fabs(xpos - tower_dx/2);
	      double y_corner_2 =  fabs(ypos + tower_dy/2);
	      double x_corner_3 =  fabs(xpos + tower_dx/2);
	      double y_corner_3 =  fabs(ypos - tower_dy/2);
	      double x_corner_4 =  fabs(xpos - tower_dx/2);
	      double y_corner_4 =  fabs(ypos - tower_dy/2);

	      if( ((x_corner_1<femc_xmin) && (y_corner_1<femc_ymin)) ||
		  ((x_corner_2<femc_xmin) && (y_corner_2<femc_ymin)) || 
		  ((x_corner_3<femc_xmin) && (y_corner_3<femc_ymin)) || 
		  ((x_corner_4<femc_xmin) && (y_corner_4<femc_ymin)) )
		continue; 

	      if( ((x_corner_1>femc_xmax) && (y_corner_1>femc_ymax)) ||
		  ((x_corner_2>femc_xmax) && (y_corner_2>femc_ymax)) || 
		  ((x_corner_3>femc_xmax) && (y_corner_3>femc_ymax)) || 
		  ((x_corner_4>femc_xmax) && (y_corner_4>femc_ymax)) )
		continue; 


	      fout << "Tower " << 1 << " " << idx_j + idx_offset << " " << idx_k + idx_offset << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	  
	      twr_count++; 
	      twr_count1++; 
	  
	    }

	}

    }

  fout.close();

  cout << "Placed " << twr_count << " towers in mapping file." << endl; 
  cout << "(PbGl towers = " << twr_count0 << " MPC towers = " << twr_count1 << ")" << endl; 

}
