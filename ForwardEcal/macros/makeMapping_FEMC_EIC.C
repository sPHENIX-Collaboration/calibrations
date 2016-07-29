makeMapping_FEMC()
{
  float femc_rmin1 = 11; // cm
  float femc_rmax1 = 225; // cm
  float femc_rmin2 = 12; // cm
  float femc_rmax2 = 246; // cm

  float tower_dx = 3.0; // cm
  float tower_dy = 3.0; // cm
  float tower_dz = 17.0; // cm

  // all towers at fixed z position which is center of mother volume
  float zpos = 315; // cm;

  unsigned n_towers_j = 200;
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
  fout << "#idx_j,idx_k,idx_l,x[cm],y[cm],z[cm],dx[cm],dy[cm],dz[cm],alpha,beta,gamma,type" << endl;

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

	  /* check if tower extends beyond calorimeter envelope volume */
	  double tower_rpos = sqrt( xpos * xpos + ypos * ypos );

	  double tower_r_clear_max = tower_rpos + tower_r;
	  double tower_r_clear_min = tower_rpos - tower_r;

	  if ( tower_r_clear_min < r_min || tower_r_clear_max > r_max )
	    continue;

	  fout << idx_j << " " << idx_k << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0 0" << endl;

	}

    }

  fout.close();

}
