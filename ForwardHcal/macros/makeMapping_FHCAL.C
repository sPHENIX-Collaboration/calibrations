makeMapping_FHCAL()
{
  float hhcal_rmin1 = 5.0; // cm
  float hhcal_rmax1 = 262.0; // cm
  float hhcal_rmin2 = 5.0; // cm
  float hhcal_rmax2 = 336.9.0; // cm

  float tower_dx = 10.0; // cm
  float tower_dy = 10.0; // cm
  float tower_dz = 100.0; // cm

  // all towers at fixed z position which is center of mother volume
  float zpos = 400; // cm

  // assume center tower is centered at (0,0)
  // find index of 'theoretical' center tower on chessboard
  unsigned n_towers_j = 2 * ( (unsigned)( (hhcal_rmax1+0.5*tower_dx) / tower_dx) ) + 1;
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
  ofstream fout("towerMap_FHCAL_latest.txt");
  fout << "#idx_j,idx_k,idx_l,x[cm],y[cm],z[cm],dx[cm],dy[cm],dz[cm],alpha,beta,gamma,type" << endl;

  unsigned idx_l = 0;

  for (unsigned idx_j = 0; idx_j < n_towers_j; idx_j++)
    {
      for (unsigned idx_k = 0; idx_k < n_towers_k; idx_k++)
        {

	  // check if all four corners are within envelope volume
	  float xpos = xpos_j0_k0 + idx_j * tower_dx;;
	  float ypos = ypos_j0_k0 + idx_k * tower_dy;;

	  float r_corner_1 = sqrt( pow( xpos + tower_dx/2. , 2 ) + pow( ypos + tower_dy/2. , 2 ) );
	  float r_corner_2 = sqrt( pow( xpos - tower_dx/2. , 2 ) + pow( ypos + tower_dy/2. , 2 ) );
	  float r_corner_3 = sqrt( pow( xpos + tower_dx/2. , 2 ) + pow( ypos - tower_dy/2. , 2 ) );
	  float r_corner_4 = sqrt( pow( xpos - tower_dx/2. , 2 ) + pow( ypos - tower_dy/2. , 2 ) );

	  if ( r_corner_1 > hhcal_rmax1 ||
	       r_corner_2 > hhcal_rmax1 ||
	       r_corner_3 > hhcal_rmax1 ||
	       r_corner_4 > hhcal_rmax1 )
	    continue;

	  if ( r_corner_1 < hhcal_rmin1 ||
	       r_corner_2 < hhcal_rmin1 ||
	       r_corner_3 < hhcal_rmin1 ||
	       r_corner_4 < hhcal_rmin1 )
	    continue;

	  if ( idx_j == j_center && idx_k == k_center )
	    continue;

	  fout << idx_j << " " << idx_k << " " << idx_l << " " << xpos << " " << ypos << " " << zpos << " " << tower_dx << " " << tower_dy << " " << tower_dz << " 0 0 0 0" << endl;

	}

    }

  fout.close();

}
