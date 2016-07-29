makeMapping_EEMC()
{

  //  _place_in_x(0.0*mm),
  //  _place_in_y(0.0*mm),
  float eemz_z0 = -108.0; // cm,

  float eemc_rmin1 = 2.2; // cm
  float eemc_rmax1 = 65.6; // cm
  float eemc_rmin2 = 2.6; // cm
  float eemc_rmax2 = 77.5; // cm

  float crystal_width = 2.0; // cm
  float crystal_length = 18.0; // cm
  float carbon_thickness = 0.009; // cm
  float airgap = 0.012; // cm

  float tower_dx = crystal_width + 2 * ( carbon_thickness + airgap ); // cm
  float tower_dy = tower_dx;
  float tower_dz = crystal_length; // cm

  // all towers at fixed z position in CENTER of mother volume
  float zpos = eemz_z0;

  unsigned n_towers_j = 100;
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
  ofstream fout("towerMap_EEMC_latest.txt");
  fout << "#idx_j,idx_k,idx_l,x[cm],y[cm],z[cm],dx[cm],dy[cm],dz[cm],alpha,beta,gamma,type" << endl;

  float r_min = eemc_rmin2;
  float r_max = eemc_rmax1;
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

	  fout << idx_j << " " << idx_k << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << crystal_width << " " << crystal_width << " " << tower_dz << " 0 0 0 0" << endl;

	  // ideal data format:
	  // tower_id , x , y , z , alpha , beta , gamma , towertype
	}

    }

  fout.close();

//  /* Calculate center position for tower */
//  G4double xpos_tower_jk = xpos_j0_k0 + idx_j * _tower_dx;
//  G4double ypos_tower_jk = ypos_j0_k0 + idx_k * _tower_dx;
//  G4ThreeVector g4vec(xpos_tower_jk, ypos_tower_jk, 0);
//
//  /* check if tower extends beyond calorimeter envelope volume */
//  G4double tower_rpos = sqrt( xpos_tower_jk * xpos_tower_jk + ypos_tower_jk * ypos_tower_jk );
//
//  G4double tower_r_clear_max = tower_rpos + tower_r;
//  G4double tower_r_clear_min = tower_rpos - tower_r;
//
//  if ( tower_r_clear_min < r_min || tower_r_clear_max > r_max )
//    continue;
//
//  // write output
//  TFile *fout = new TFile("TGeo_eemc.root","RECREATE");
//  gm->Write();
//  fout->Close();

}
