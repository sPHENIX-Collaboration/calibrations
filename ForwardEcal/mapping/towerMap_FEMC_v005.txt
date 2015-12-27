makeMapping_FEMC()
{
  /* Global detector position / transformation */
  float femc_x0 =  0.0; // cm,
  float femc_y0 =  0.0; // cm,
  float femc_z0 = 310.0; // cm,

  float femc_rot_x0 =  0.0;
  float femc_rot_y0 =  0.0;
  float femc_rot_z0 =  0.0;

  /* Detector envelope size (cone shape) */
  float femc_rmin1 = 5.0; // cm
  float femc_rmax1 = 219.18; // cm
  float femc_rmin2 = 5.0; // cm
  float femc_rmax2 = 246.51; // cm
  float femc_dz = 36.5; // cm (FULL SIZE)

  /* Tower parameters */
  // From PHENIX EMCal JGL 12/27/2015
  float tower_dx = 5.535; // cm
  float tower_dy = 5.535; // cm
  float tower_dz = 36.3; // cm (FULL SIZE)

  //unsigned n_towers_j = 200;
  unsigned n_towers_j = 2 * ( (unsigned)( (femc_rmax1+0.5*tower_dx) / tower_dx) ) + 1;
  unsigned n_towers_k = n_towers_j;

  unsigned j_center = n_towers_j / 2 + 1;
  unsigned k_center = j_center;

  float xpos_j0_k0 = -1 * ( (float)( n_towers_j - 1 ) / 2 ) * tower_dx - tower_dx;;
  float ypos_j0_k0 = xpos_j0_k0;

  cout << "n_towers_j = " << n_towers_j << endl;
  cout << "n_towers_k = " << n_towers_k << endl;
  cout << "xpos_j0_k0 = " << xpos_j0_k0 << endl;
  cout << "ypos_j0_k0 = " << ypos_j0_k0 << endl;
  cout << "center tower j = " << j_center << " / k = " << k_center << " is at " << xpos_j0_k0 + j_center * tower_dx << " , " <<  ypos_j0_k0 + k_center * tower_dy << endl;

  // create map
  ofstream fout("towerMap_FEMC_latest.txt");

  /* Global detector transformation */
  fout << "#Global detector geometry and transforamtion; lengths given in cm" << endl;
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
  fout << "Gtower_dx " << tower_dx << endl;
  fout << "Gtower_dy " << tower_dy << endl;
  fout << "Gtower_dz " << tower_dz << endl;

  /* Tower mapping */
  fout << "#Tower type,idx_j,idx_k,idx_l,x[cm],y[cm],z[cm],dx[cm],dy[cm],dz[cm],rot_x,rot_y,rot_z" << endl;

  float r_min = femc_rmin2;
  float r_max = femc_rmax1;
  float tower_r = sqrt(  tower_dx * tower_dx + tower_dy * tower_dy );

  /* define center of crystal with index j=0, k=0 */
  float xpos_j0_k0 = 0.0 - ( 0.5 * n_towers_j - 0.5 ) * tower_dx;
  float ypos_j0_k0 = 0.0 - ( 0.5 * n_towers_k - 0.5 ) * tower_dy;

  unsigned idx_l = 0;

  for (int idx_j = 0; idx_j < n_towers_j; idx_j++)
    {
      for (int idx_k = 0; idx_k < n_towers_k; idx_k++)
	{

	  /* Calculate center position for tower */
	  double xpos = xpos_j0_k0 + idx_j * tower_dx;
	  double ypos = ypos_j0_k0 + idx_k * tower_dx;
	  double zpos = 0;

	  /* check if tower extends beyond calorimeter envelope volume */
	  double tower_rpos = sqrt( xpos * xpos + ypos * ypos );

	  double tower_r_clear_max = tower_rpos + tower_r;
	  double tower_r_clear_min = tower_rpos - tower_r;

	  if ( tower_r_clear_min < r_min || tower_r_clear_max > r_max )
	    continue;

	  //fout << idx_j << " " << idx_k << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0 0" << endl;

	  fout << "Tower " << 0 << " " << idx_j << " " << idx_k << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0" << endl;
	}

    }

  fout.close();

}
