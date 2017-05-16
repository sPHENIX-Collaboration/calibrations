// $$Id: $$
#include <TVector2.h>
#include <iostream>

using namespace std;

// to load tower map from Richie/Dan
class four_corner
{
 public:
  TVector2 p1, p2, p3, p4;
};

void Construct_CEMC_Param_ProjTilted()
{
  cout << "Construct_CEMC_Param_ProjTilted() - Entry" << endl;

  gSystem->Load("libg4detectors.so");

  PHG4Parameters *param = new PHG4Parameters("CEMC_0");

  param->set_string_param("description",
                          "sPHENIX 2017 SPACAL design, tilted full projective design");

  const double inch_to_cm = 2.54;
  const double twopi = 2 * TMath::Pi();

  // visual switches
  param->set_int_param("azimuthal_seg_visible", 1);
  param->set_int_param("virualize_fiber", 0);
  param->set_int_param("config", PHG4CylinderGeom_Spacalv1::kFullProjective_2DTaper_Tilted_SameLengthFiberPerTower);

  //general placement
  param->set_double_param("xpos", 0);
  param->set_double_param("ypos", 0);
  param->set_double_param("zpos", 0);

  // construct 32 sectors
  const int n_sector = 32;

  // 2017 design - The final choice was 58 rows x 46 cols = 2668 fibers.
  const int NFiberY = 58;
  const int NFiberX = 46 * 2;
  const double fiber_diameter = 0.047;
  //
  const double ModuleSkinThickness = 0.05;  // 20 mil
  const double fiber_clading_thickness = 0.003 / 2;
  const double fiber_core_diameter = fiber_diameter - fiber_clading_thickness * 2;

  const double assembly_spacing_x = 0.33 / 10 / 2.;
  const double assembly_spacing_y = 0.1 / 10 / 2.;
  const double assembly_spacing = 0.002500;

  //30 mil
  const double sidewall_thickness = 0.075000;

  //Dan: 2x 1/16 inch between two sector outer walls.
  const double sidewall_outer_torr =0.15875;

  const double radius = 90;
  const double thickness = 26.130000;
  const double zmax = 149.470000;
  const double zmin = -zmax;
  const double azimuthal_tilt = -0.1;
  const int azimuthal_n_sec = 32;
  const int max_phi_bin_in_sec = 8;

  param->set_double_param("azimuthal_tilt", azimuthal_tilt);
  param->set_double_param("polar_taper_ratio", 1);
  param->set_double_param("sidewall_thickness", sidewall_thickness);
  param->set_double_param("sidewall_outer_torr", sidewall_outer_torr);

  param->set_double_param("fiber_clading_thickness", fiber_clading_thickness);
  param->set_double_param("fiber_core_diameter", fiber_core_diameter);
  param->set_double_param("assembly_spacing", 0);  // used for segment wall assembly, not used here in prototyes
  param->set_double_param("radius", radius);
  param->set_double_param("thickness", thickness);
  param->set_double_param("zmin", zmin);
  param->set_double_param("zmax", zmax);
  param->set_int_param("azimuthal_n_sec", azimuthal_n_sec);
  param->set_int_param("max_phi_bin_in_sec", max_phi_bin_in_sec);

  //  sector_map
  param->set_int_param("sector_map_size", n_sector);
  for (int sec = 0; sec < n_sector; ++sec)
  {
    //      const double rot = 2 * TMath::Pi() / (double) (azimuthal_n_sec)
    //          * ((double) (sec) - (nx - 1.) / 2) - TMath::Pi() / 2.;

    const double rot = twopi / (double) (n_sector) * ((double) (sec));

    stringstream prefix;
    prefix << "sector_map";
    prefix << "[" << sec << "]"
           << ".";

    //      sector_map[sec] = rot;

    param->set_int_param(prefix.str() + "id", sec);
    param->set_double_param(prefix.str() + "rotation", rot);
  }


  //  sector_tower_map
  //

  four_corner b1, b2, b3, b4;
  four_corner blocks[10];

  b1.p1.Set(1069.17 / 10, 1118.50 / 10);  // from Richie's drawing in mm
  b2.p1.Set(1142.89 / 10, 1118.50 / 10);  // from Richie's drawing in mm
  b3.p1.Set(1219.07 / 10, 1118.50 / 10);  // from Richie's drawing in mm
  b4.p1.Set(1297.88 / 10, 1118.50 / 10);  // from Richie's drawing in mm

  b1.p2.Set(1105.85 / 10, 1082.25 / 10);  // from Richie's drawing in mm
  b2.p2.Set(1178.32 / 10, 1081.11 / 10);  // from Richie's drawing in mm
  b3.p2.Set(1253.26 / 10, 1080.05 / 10);  // from Richie's drawing in mm
  b4.p2.Set(1330.86 / 10, 1079.06 / 10);  // from Richie's drawing in mm

  b1.p3.Set(963.71 / 10, 1008.17 / 10);   // from Richie's drawing in mm
  b2.p3.Set(1031.24 / 10, 1009.24 / 10);  // from Richie's drawing in mm
  b3.p3.Set(1101.06 / 10, 1010.23 / 10);  // from Richie's drawing in mm
  b4.p3.Set(1173.33 / 10, 1011.16 / 10);  // from Richie's drawing in mm

  b1.p4.Set(996.77 / 10, 975.50 / 10);   // from Richie's drawing in mm
  b2.p4.Set(1063.21 / 10, 975.50 / 10);  // from Richie's drawing in mm
  b3.p4.Set(1131.95 / 10, 975.50 / 10);  // from Richie's drawing in mm
  b4.p4.Set(1203.14 / 10, 975.50 / 10);  // from Richie's drawing in mm

  four_corner b, b_rotated;
  blocks[0] = b1;
  blocks[1] = b2;
  blocks[2] = b3;
  blocks[3] = b4;

  const double half_block_phi = 2 * TMath::Pi() / (double) (azimuthal_n_sec) / 2;
  //
  const int ny = 4;
  param->set_int_param("sector_tower_map_size", ny);
  for (int y = 0; y < ny; y++)
  {
    stringstream prefix;
    prefix << "sector_tower_map";
    prefix << "[" << y << "]"
           << ".";
    //

    //use conform tower ID convension as defined in PHG4CylinderGeom_Spacalv3::get_tower_z_phi_ID()
    param->set_int_param(prefix.str() + "id", y * 10 + 1);

    b = blocks[y];

    cout << "Construct_CEMC_Param_2017() - block " << prefix.str() << endl;
    TVector2 c12 = 0.5 * (b.p1 + b.p2);
    TVector2 c34 = 0.5 * (b.p3 + b.p4);
    TVector2 center = 0.5 * (c12 + c34);
    TVector2 center_line_vec = c12 - c34;
    double pRotationAngleX = -center_line_vec.Phi();

    b_rotated = b;
    TVector2 c12_rotated = c12;
    TVector2 c34_rotated = c34;

    b_rotated.p1 -= center;
    b_rotated.p2 -= center;
    b_rotated.p3 -= center;
    b_rotated.p4 -= center;
    c12_rotated -= center;
    c34_rotated -= center;

    b_rotated.p1 = b_rotated.p1.Rotate(pRotationAngleX);
    b_rotated.p2 = b_rotated.p2.Rotate(pRotationAngleX);
    b_rotated.p3 = b_rotated.p3.Rotate(pRotationAngleX);
    b_rotated.p4 = b_rotated.p4.Rotate(pRotationAngleX);
    c12_rotated = c12_rotated.Rotate(pRotationAngleX);
    c34_rotated = c34_rotated.Rotate(pRotationAngleX);

    cout << "Construct_CEMC_Param_2017() - block " << y
         << " four corner after rotation to along z axis by "
         << pRotationAngleX << endl;
    cout << "\tp1 = " << b_rotated.p1.X() << ",\t" << b_rotated.p1.Y()
         << endl;
    cout << "\tp2 = " << b_rotated.p2.X() << ",\t" << b_rotated.p2.Y()
         << endl;
    cout << "\tp3 = " << b_rotated.p3.X() << ",\t" << b_rotated.p3.Y()
         << endl;
    cout << "\tp4 = " << b_rotated.p4.X() << ",\t" << b_rotated.p4.Y()
         << endl;
    cout << "\tc12 = " << c12_rotated.X() << ",\t" << c12_rotated.Y() << endl;
    cout << "\tc34 = " << c34_rotated.X() << ",\t" << c34_rotated.Y() << endl;

    param->set_double_param(prefix.str() + "pDz", c12_rotated.X());
    param->set_double_param(prefix.str() + "pTheta", 0.);
    param->set_double_param(prefix.str() + "pPhi", 0.);
    param->set_double_param(prefix.str() + "pAlp1", 0.);
    param->set_double_param(prefix.str() + "pAlp2", 0.);

    param->set_double_param(prefix.str() + "pDy1",
                            0.5 * (b_rotated.p3.Y() - b_rotated.p4.Y()) - assembly_spacing_y);
    param->set_double_param(prefix.str() + "pDx1",
                            b.p4.Y() * tan(half_block_phi) - assembly_spacing_x);
    param->set_double_param(prefix.str() + "pDx2",
                            b.p3.Y() * tan(half_block_phi) - assembly_spacing_x);

    param->set_double_param(prefix.str() + "pDy2",
                            0.5 * (b_rotated.p1.Y() - b_rotated.p2.Y()) - assembly_spacing_y);
    param->set_double_param(prefix.str() + "pDx3",
                            b.p2.Y() * tan(half_block_phi) - assembly_spacing_x);
    param->set_double_param(prefix.str() + "pDx4",
                            b.p1.Y() * tan(half_block_phi) - assembly_spacing_x);
    //
    param->set_double_param(prefix.str() + "centralX", 0);
    param->set_double_param(prefix.str() + "centralY", center.Y());
    param->set_double_param(prefix.str() + "centralZ", center.X());
    //

    param->set_double_param(prefix.str() + "pRotationAngleX",
                            pRotationAngleX);
    param->set_double_param(prefix.str() + "ModuleSkinThickness",
                            ModuleSkinThickness);
    param->set_int_param(prefix.str() + "NFiberX", NFiberX);  // azimuthal / narrow direction
    param->set_int_param(prefix.str() + "NFiberY", NFiberY);  // polar / wide direction
                                                              //
    param->set_int_param(prefix.str() + "NSubtowerX", 2);
    param->set_int_param(prefix.str() + "NSubtowerY", 2);
    //
    param->set_double_param(prefix.str() + "LightguideHeight",
                            2 * inch_to_cm);
    param->set_double_param(prefix.str() + "LightguideTaperRatio",
                            0.545 / 1.039);
    param->set_string_param(prefix.str() + "LightguideMaterial", "PMMA");
  }

  //  storage
  param->Print();

  gSystem->mkdir("../Geometry_2017ProjTilted/");

  PdbParameterMapContainer *paramcontainer = new PdbParameterMapContainer();

  PdbParameterMap *myparm = new PdbParameterMap();
  param->CopyToPdbParameterMap(myparm);
  myparm->print();
  paramcontainer->AddPdbParameterMap(0, myparm);

  paramcontainer->WriteToFile("CEMC", "root", "../Geometry_2017ProjTilted/");

  paramcontainer->WriteToFile("CEMC", "xml", "../Geometry_2017ProjTilted/");
}
