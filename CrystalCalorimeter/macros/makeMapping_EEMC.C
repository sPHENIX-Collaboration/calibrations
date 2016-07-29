makeMapping_EEMC()
{

  /* Global detector position / transformation */
  float eemc_x0 =  0.0; // cm,
  float eemc_y0 =  0.0; // cm,
  float eemc_z0 = -108.0; // cm,

  float eemc_rot_x0 =  0.0;
  float eemc_rot_y0 =  TMath::Pi();
  float eemc_rot_z0 =  0.0;

  /* Detector envelope size (conde shape) */
  float eemc_rmin1 = 2.2; // cm
  float eemc_rmax1 = 65.6; // cm
  float eemc_rmin2 = 2.6; // cm
  float eemc_rmax2 = 77.5; // cm
  float eemc_dz    = 18.0; // cm

  /* Tower parameters */
  float crystal_width = 2.0; // cm
  float crystal_length = 18.0; // cm
  float carbon_thickness = 0.009; // cm
  float airgap = 0.012; // cm

  float tower_dx = crystal_width + 2 * ( carbon_thickness + airgap ); // cm
  float tower_dy = tower_dx;
  float tower_dz = crystal_length; // cm

  // all towers at fixed z position in CENTER of mother volume
  float zpos = 0;

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

  /* Global detector transformation */
  fout << "#Global detector geometry and transforamtion; lengths given in cm" << endl;
  fout << "Gtype " << 1 << endl;
  fout << "Gr1_inner " << eemc_rmin1 << endl;
  fout << "Gr1_outer " << eemc_rmax1 << endl;
  fout << "Gr2_inner " << eemc_rmin2 << endl;
  fout << "Gr2_outer " << eemc_rmax2 << endl;
  fout << "Gdz " << eemc_dz << endl;
  fout << "Gx0 " << eemc_x0 << endl;
  fout << "Gy0 " << eemc_y0 << endl;
  fout << "Gz0 " << eemc_z0 << endl;
  fout << "Grot_x " << eemc_rot_x0 << endl;
  fout << "Grot_y " << eemc_rot_y0 << endl;
  fout << "Grot_z " << eemc_rot_z0 << endl;
  fout << "Gcrystal_dx " << crystal_width << endl;
  fout << "Gcrystal_dy " << crystal_width << endl;
  fout << "Gcrystal_dz " << crystal_length << endl;

  /* Tower mapping */
  fout << "#Tower type,idx_j,idx_k,idx_l,x[cm],y[cm],z[cm],dx[cm],dy[cm],dz[cm],rot_x,rot_y,rot_z" << endl;

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

	  fout << "Tower " << 0 << " " << idx_j << " " << idx_k << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << crystal_width << " " << crystal_width << " " << tower_dz << " 0 0 0" << endl;

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
