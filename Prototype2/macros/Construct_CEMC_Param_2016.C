// $$Id: $$


void
Construct_CEMC_Param_2016()
{
  cout << "Construct_CEMC_Param() - Entry" << endl;

  gSystem->Load("libg4detectors.so");

  PHG4Parameters * param = new PHG4Parameters("CEMC");

  param->set_string_param("description",
      "sPHENIX 2016 SPACAL prototype in the default orientation");

  const double inch_to_cm = 2.54;

  // visual switches
  param->set_int_param("azimuthal_seg_visible", 1);
  param->set_int_param("virualize_fiber", 1);

  //general placement
  param->set_double_param("xpos", 30);
  param->set_double_param("ypos", 0);
  param->set_double_param("zpos", 0);
  param->set_double_param("z_rotation_degree", 0);

  //enclosure parameters
  // 13 inch box with 40mil G10 cover plates
  param->set_double_param("enclosure_x", 13 * inch_to_cm);
  param->set_double_param("enclosure_y", 13 * inch_to_cm);
  param->set_double_param("enclosure_z", 13 * inch_to_cm);
  param->set_double_param("enclosure_thickness", 0.04 * inch_to_cm);
  param->set_string_param("enclosure_material", "G10");
  param->set_double_param("enclosure_x_shift", -0.25 * 13 * inch_to_cm);

  param->set_double_param("electronics_thickness", 0.1 * inch_to_cm);
  param->set_string_param("electronics_material", "G10");

  // construct 4x8 modules
  const int ny = 4;
  const int nx = 8;
//  const int nx = 1;


  // Sean Stoll:
  // the avg thickness is about ~  24.02mm at the thin end and 26.92mm at the thick end
  /*
         height(mm)  width(mm) length(mm)  density (g/cc)
    UIUC  25.6  53.4  139.2 9.43
    THP   25.4  53.6  138.0 9.65
    ALL   25.5  53.5  138.6 9.53

    side gap    0.1 mm  gap between adjacent modules in row
    stack gap   0.33  mm  gap between stacked rows of modules
    pcb thickness:    1.75mm
   * */

  // 2016 THP new screen:
  const double hole_diameter = 0.0290 * inch_to_cm;
  const double hole_edge_to_edge = 0.0110 * inch_to_cm;
  const int NFiberX = 30;
  const int NFiberY = 52 * 2;
  const double hole_edge_to_screen_edge = 0;
  const double fiber_diameter = 0.047;

  // average module size
  const double nawrrow_width_x = 24.02 / 10;
  const double wide_width_x = 26.92 / 10;
  const double module_length = 138.6/10;
  assert(module_length > 0);
  assert(wide_width_x > nawrrow_width_x);

  // derived numbers
  const double distance_center_to_center = hole_edge_to_edge + hole_diameter;
//  const double screen_size_x = (NFiberX - 1) * sqrt(3) / 2
//      * distance_center_to_center + hole_diameter
//      + 2 * hole_edge_to_screen_edge;
  const double ModuleSkinThickness =
      (wide_width_x //
      - (NFiberX - 1) * sqrt(3) / 2* distance_center_to_center
      - fiber_diameter)/2;
  const double screen_size_y = (NFiberY - 1) * 0.5 * distance_center_to_center
      + fiber_diameter + 2 * ModuleSkinThickness;

  cout << "Construct_CEMC_Param_2016() - "
  << "Adjust screen_size_y by "
      << screen_size_y / (53.5/10)<< endl;

  //tapering, dxwidth/dlength
  const double tapering_ratio = (wide_width_x - nawrrow_width_x)
      / module_length;
  assert(tapering_ratio < 1);
  assert(tapering_ratio > 0);

  const double fiber_clading_thickness = 0.003 / 2;
  const double fiber_core_diameter = fiber_diameter
      - fiber_clading_thickness * 2;

  const double assembly_spacing_x = 0.33/10/2.;
  const double assembly_spacing_y = 0.1/10/2.;

  const double radius = (nawrrow_width_x + 2*assembly_spacing_x) / tapering_ratio;
  const double thickness = module_length * 1.03; // keep a small torlerence space
  const double zmin = -(0.5 * ny * screen_size_y
      + 2 * assembly_spacing_y * (ny + 1));
  const double zmax = -zmin;
  const int azimuthal_n_sec = floor(2 * TMath::Pi() / atan(tapering_ratio));
  const int max_phi_bin_in_sec = 1;

  const double nawrrow_width_x_construction = radius * 2
      * tan(TMath::Pi() / azimuthal_n_sec) - 2 * assembly_spacing_x;
  const double wide_width_x_construction = (radius + module_length) * 2
      * tan(TMath::Pi() / azimuthal_n_sec) - 2 * assembly_spacing_x;

  param->set_double_param("fiber_clading_thickness", fiber_clading_thickness);
  param->set_double_param("fiber_core_diameter", fiber_core_diameter);
  param->set_double_param("assembly_spacing", 0); // used for segment wall assembly, not used here in prototyes
  param->set_double_param("radius", radius);
  param->set_double_param("thickness", thickness);
  param->set_double_param("zmin", zmin);
  param->set_double_param("zmax", zmax);
  param->set_int_param("azimuthal_n_sec", azimuthal_n_sec);
  param->set_int_param("max_phi_bin_in_sec", max_phi_bin_in_sec);

  cout << "Construct_CEMC_Param_2016() - "
  << "Adjust wide end width by ratio of "
      << wide_width_x_construction / wide_width_x
      << " and narrow end by ratio of "
      << nawrrow_width_x_construction / nawrrow_width_x << endl;

//  sector_map
  param->set_int_param("sector_map_size", nx);
  for (int sec = 0; sec < nx; ++sec)
    {
      const double rot = 2 * TMath::Pi() / (double) (azimuthal_n_sec)
          * ((double) (sec) - 3.5) - TMath::Pi() / 2.;

      stringstream prefix;
      prefix << "sector_map";
      prefix << "[" << sec << "]" << ".";

//      sector_map[sec] = rot;

      param->set_int_param(prefix.str() + "id", sec);
      param->set_double_param(prefix.str() + "rotation", rot);
    }

  param->set_double_param("azimuthal_tilt", 0);
  param->set_double_param("polar_taper_ratio", 1);
  param->set_double_param("sidewall_thickness", 0);
  param->set_double_param("sidewall_outer_torr", 0);

//  sector_tower_map
//
  param->set_int_param("sector_tower_map_size", ny);
  for (int y = 0; y < ny; y++)
    {

      stringstream prefix;
      prefix << "sector_tower_map";
      prefix << "[" << y << "]" << ".";

      //use conform tower ID convension as defined in PHG4CylinderGeom_Spacalv3::get_tower_z_phi_ID()
      param->set_int_param(prefix.str() + "id", y * 10 + 1);

      param->set_double_param(prefix.str() + "pDz", module_length / 2);
      param->set_double_param(prefix.str() + "pTheta", 0.);
      param->set_double_param(prefix.str() + "pPhi", 0.);
      param->set_double_param(prefix.str() + "pAlp1", 0.);
      param->set_double_param(prefix.str() + "pAlp2", 0.);
      param->set_double_param(prefix.str() + "pDy1", 0.5 * screen_size_y);
      param->set_double_param(prefix.str() + "pDx1",
          0.5 * nawrrow_width_x_construction);
      param->set_double_param(prefix.str() + "pDx2",
          0.5 * nawrrow_width_x_construction);
      param->set_double_param(prefix.str() + "pDy2", 0.5 * screen_size_y);
      param->set_double_param(prefix.str() + "pDx3",
          0.5 * wide_width_x_construction);
      param->set_double_param(prefix.str() + "pDx4",
          0.5 * wide_width_x_construction);

      param->set_double_param(prefix.str() + "centralX", 0.);
      param->set_double_param(prefix.str() + "centralY",
          module_length * 0.5 + radius + 0.0001 );
      param->set_double_param(prefix.str() + "centralZ",
          (screen_size_y + 2*assembly_spacing_y )* (y - 1.5));

      param->set_double_param(prefix.str() + "pRotationAngleX",
          -TMath::Pi() / 2.);
      param->set_double_param(prefix.str() + "ModuleSkinThickness",
          ModuleSkinThickness);
      param->set_int_param(prefix.str() + "NFiberX", NFiberX); // azimuthal / narrow direction
      param->set_int_param(prefix.str() + "NFiberY", NFiberY); // polar / wide direction

      param->set_int_param(prefix.str() + "NSubtowerX", 1);
      param->set_int_param(prefix.str() + "NSubtowerY", 2);
//      param->set_int_param(prefix.str() + "NSubtowerX", 2);
//      param->set_int_param(prefix.str() + "NSubtowerY", 3);

      param->set_double_param(prefix.str() + "LightguideHeight",
          1 * inch_to_cm);
      param->set_double_param(prefix.str() + "LightguideTaperRatio",
          0.545 / 1.039);
      param->set_string_param(prefix.str() + "LightguideMaterial", "PMMA");
    }

  // storage
  param->print();

  gSystem->mkdir("./test_geom/");
  param->WriteToFile("root", "./test_geom/");

  // this to solve an annoying problem for ROOT/XML regarding precision needed for Geant4
  TBufferXML::SetFloatFormat("%.18e"); // match precision for double
  param->WriteToFile("xml", "./test_geom/");

}

