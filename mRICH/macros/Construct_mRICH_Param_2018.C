
void Construct_mRICH_Param_2018()
{
  cout << "Construct_mRICH_Wall() - Entry" << endl;
  gSystem->Load("libg4detectors.so");

  PHG4Parameters * param = new PHG4Parameters("mRICH_0");
  param->set_string_param("description", "sPHENIX 2018 modular RICH in the default orientation");

  //------------------------ mRICH wall hadron side ------------------------
  {
    double gap = 30.0; // *mm
    double halfWidth = 74.025;
    double halfLength = 111.622 + gap;

    double rinner = 3000.0; // *mm
    double eta_min = 1.1;
    double eta_max = 1.9;

    double theta_min = 2.0*atan(exp(-1.0*eta_max));
    double theta_max = 2.0*atan(exp(-1.0*eta_min));

    param->set_double_param("mRICH_wall_hside_halfWidth", halfWidth);
    param->set_double_param("mRICH_wall_hside_halfLength", halfLength);
    param->set_double_param("mRICH_wall_hside_rinner", rinner);
    param->set_double_param("mRICH_wall_hside_eta_min", eta_min);
    param->set_double_param("mRICH_wall_hside_eta_max", eta_max);
    param->set_double_param("mRICH_wall_hside_theta_min", theta_min);
    param->set_double_param("mRICH_wall_hside_theta_max", theta_max);

    double deltaTheta = 2.0*atan(halfWidth/rinner);//+pi/180;
    int NumModuleMax = floor(2.0*theta_max/deltaTheta);

    double angle_max;
    if (NumModuleMax%2==0) angle_max=(NumModuleMax/2)*deltaTheta-deltaTheta/2;
    else angle_max=((NumModuleMax-1)/2)*deltaTheta;

    int counter_mRICH_wall_hside = 0;
    for (double theta_x = -angle_max; theta_x < angle_max; theta_x = theta_x+deltaTheta)
    {
      double x = (rinner+halfLength)*sin(theta_x);

      for (double theta_y = -angle_max; theta_y < angle_max; theta_y = theta_y+deltaTheta)
      {
	double y = (rinner+halfLength)*sin(theta_y);
	double z = sqrt(pow(rinner+halfLength,2)-pow(x,2)-pow(y,2));

	double theta = acos(z/sqrt(pow(x,2)+pow(y,2)+pow(z,2))); //in radian
	if (theta <= (theta_min+deltaTheta) || theta >= theta_max-deltaTheta) continue;

	double phi=atan2(y,x); //in radian

	stringstream prefix;
	prefix << "mRICH_wall_hside_" << counter_mRICH_wall_hside;
	param->set_double_param(prefix.str() + "_position_x", x);
	param->set_double_param(prefix.str() + "_position_y", y);
	param->set_double_param(prefix.str() + "_position_z", z);

	param->set_double_param(prefix.str() + "_rotation_theta", theta);
	param->set_double_param(prefix.str() + "_rotation_phi", phi);

	param->set_int_param(prefix.str() + "_moduleID", counter_mRICH_wall_hside);

	counter_mRICH_wall_hside++;
      }
    }
    param->set_int_param("NumOfModule_wall_hside", counter_mRICH_wall_hside);
  }
  //------------------------ mRICH wall hadron side ------------------------
  

  //------------------------ mRICH sector hadron side ------------------------
  {
    double pi = 3.1415926;

    double halfWidth = 74.025;
    // double halfLength = 111.622;
    double halfLength = 98.942;
    // double z_max = 3100.0; // *mm
    // double z_length = 500.0; // *mm
    double z_max = 2808.5; // *mm
    // double z_length = 297.174; // *mm
    double z_length = 395.516; // *mm

    double eta_min = 1.1;
    // double eta_max = 1.9;
    // double eta_min = 1.242;
    double eta_max = 1.85;
    double theta_min = 2.0*atan(exp(-1.0*eta_max));
    double theta_max = 2.0*atan(exp(-1.0*eta_min));

    double y_min = z_max*tan(theta_min);
    double y_length = (z_max-z_length)*tan(theta_max) - y_min;
    // double x_length = (y_length+y_min)*tan((45/2)*pi/180);
    double x_length = (y_length+y_min)*tan((43/2)*pi/180);
    double theta = atan(z_length/y_length);                 //in radian
    double y_max = sqrt(y_length*y_length+z_length*z_length)+y_min;
    double slope = y_max/x_length;

    param->set_double_param("mRICH_sector_hside_rotation_theta", theta);
    double shift = z_max + y_min*sin(theta) - halfLength*cos(theta);
    // param->set_double_param("mRICH_sector_hside_shift", shift/1000.0);
    param->set_double_param("mRICH_sector_hside_shift", 3.014);

    int counter_mRICH_sector_hside = 0;
    for (double y = y_min + halfWidth; y <= y_max-halfWidth; y = y + 2*halfWidth) 
    {
      double x_max =(double)(y-halfWidth)/slope; //half length of a row
      int NumModuleMax = floor(x_max/halfWidth);
      x_max = (double)(NumModuleMax-1)*halfWidth;      //max x-coordinate of the center of a module in a row

      for (double x = -x_max; x <= x_max; x = x + 2*halfWidth) 
      { 
	stringstream prefix;
	prefix << "mRICH_sector_hside_" << counter_mRICH_sector_hside;
	param->set_double_param(prefix.str() + "_position_x", x);
	param->set_double_param(prefix.str() + "_position_y", y);
	param->set_double_param(prefix.str() + "_position_z", halfLength);

	param->set_int_param(prefix.str() + "_moduleID", counter_mRICH_sector_hside);

	counter_mRICH_sector_hside++;
      }
    }
    param->set_int_param("NumOfModule_sector_hside", counter_mRICH_sector_hside);
  }

  //------------------------ mRICH sector hadron side ------------------------

  // param->Print();

  // gSystem->mkdir("./test_geom/");

  // param->WriteToFile("xml", "./test_geom/");
  param->WriteToFile("xml", "../Geometry/");
}
