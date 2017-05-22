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
  param->set_int_param("azimuthal_seg_visible", 0);
  param->set_int_param("virualize_fiber", 0);
  param->set_int_param("config", PHG4CylinderGeom_Spacalv1::kFullProjective_2DTaper_Tilted_SameLengthFiberPerTower);

  //general placement
  param->set_double_param("xpos", 0);
  param->set_double_param("ypos", 0);
  param->set_double_param("zpos", 0);

  // construct 32 sectors
    const int n_sector = 32;
//  const int n_sector = 1;

  // 2017 design - The final choice was 58 rows x 46 cols = 2668 fibers.
  const int NFiberY = 58;
  const int NFiberX = 46 * 2;
  const double fiber_diameter = 0.047;

  //the distance between the tangent of a row of fibers and the edge of a block is 0.020" (0.5 mm).
  const double ModuleSkinThickness = 0.05;  // 20 mil
  const double fiber_clading_thickness = 0.003 / 2;
  const double fiber_core_diameter = fiber_diameter - fiber_clading_thickness * 2;

  //  the gap between blocks is 0.015" (0.381 mm), and the gap between the box and blocks is also 0.015" (0.381 mm).
  const double assembly_spacing = 0.381 / 10 / 2;

  //30 mil, the sheet metal box is 0.030" (0.762 mm) thick
  const double sidewall_thickness = 0.0762;

  //Dan: 2x 1/16 inch between two sector outer walls, The gap between sheet metal boxes is 0.125" (3.175 mm
  const double sidewall_outer_torr = 3.175 / 10 / 2;

  const double radius = 90;
  const double thickness = 26.130000;
  const double zmax = 149.470000;
  const double zmin = -zmax;
  const double azimuthal_tilt = -0.1;
  const int azimuthal_n_sec = 32;
  const int max_phi_bin_in_sec = 4;  // four blocks per sector in athemuth

  param->set_double_param("azimuthal_tilt", azimuthal_tilt);
  param->set_double_param("polar_taper_ratio", 1);
  param->set_double_param("sidewall_thickness", sidewall_thickness);
  param->set_double_param("sidewall_outer_torr", sidewall_outer_torr);

  param->set_double_param("fiber_clading_thickness", fiber_clading_thickness);
  param->set_double_param("fiber_core_diameter", fiber_core_diameter);
  param->set_double_param("assembly_spacing", assembly_spacing);  // used for segment wall assembly, not used here in prototyes
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

  const double half_block_phi = 2 * TMath::Pi() / (double) (azimuthal_n_sec * max_phi_bin_in_sec) / 2;
  //
  const int nblock = 24;
  //  const int nblock = 1;

  int cnt = 0;
  four_corner *b;
  four_corner *b_rotated;
  for (int side = -1; side <= +1; side += 2)
  {
    for (int block_id = 0; block_id < 0 + nblock; block_id++)
    {
      for (int block_sec = 0; block_sec < max_phi_bin_in_sec; block_sec++)
      {
        stringstream prefix;
        prefix << "sector_tower_map";
        prefix << "[" << cnt << "]"
               << ".";
        ++cnt;
        //

        //use conform tower ID convension as defined in PHG4CylinderGeom_Spacalv3::get_tower_z_phi_ID()
        const int block_map_id = (100 + side * (block_id + 1)) * 10 + block_sec;
        param->set_int_param(prefix.str() + "id", block_map_id);

        b = get_block(block_id, side);

        cout << "Construct_CEMC_Param_2017() - block_id " << prefix.str() << endl;
        TVector2 c12 = 0.5 * (b->p1 + b->p2);
        TVector2 c34 = 0.5 * (b->p3 + b->p4);
        TVector2 center = 0.5 * (c12 + c34);
        TVector2 center_line_vec = c12 - c34;
        double pRotationAngleX = -center_line_vec.Phi();

        b_rotated = new four_corner(*b);
        TVector2 c12_rotated = c12;
        TVector2 c34_rotated = c34;

        b_rotated->p1 -= center;
        b_rotated->p2 -= center;
        b_rotated->p3 -= center;
        b_rotated->p4 -= center;
        c12_rotated -= center;
        c34_rotated -= center;

        b_rotated->p1 = b_rotated->p1.Rotate(pRotationAngleX);
        b_rotated->p2 = b_rotated->p2.Rotate(pRotationAngleX);
        b_rotated->p3 = b_rotated->p3.Rotate(pRotationAngleX);
        b_rotated->p4 = b_rotated->p4.Rotate(pRotationAngleX);
        c12_rotated = c12_rotated.Rotate(pRotationAngleX);
        c34_rotated = c34_rotated.Rotate(pRotationAngleX);

        cout << "Construct_CEMC_Param_2017() - block_id " << block_id
             << " four corner after rotation to along z axis by "
             << pRotationAngleX << endl;
        //        cout << "\tp1 = " << b_rotated->p1.X() << ",\t" << b_rotated->p1.Y()
        //             << endl;
        //        cout << "\tp2 = " << b_rotated->p2.X() << ",\t" << b_rotated->p2.Y()
        //             << endl;
        //        cout << "\tp3 = " << b_rotated->p3.X() << ",\t" << b_rotated->p3.Y()
        //             << endl;
        //        cout << "\tp4 = " << b_rotated->p4.X() << ",\t" << b_rotated->p4.Y()
        //             << endl;
        //        cout << "\tc12 = " << c12_rotated.X() << ",\t" << c12_rotated.Y() << endl;
        //        cout << "\tc34 = " << c34_rotated.X() << ",\t" << c34_rotated.Y() << endl;

        param->set_double_param(prefix.str() + "pDz", c12_rotated.X());
        param->set_double_param(prefix.str() + "pTheta", 0.);
        param->set_double_param(prefix.str() + "pPhi", 0.);
        param->set_double_param(prefix.str() + "pAlp1", 0.);
        param->set_double_param(prefix.str() + "pAlp2", 0.);

        if (side > 0)
        {
          param->set_double_param(prefix.str() + "pDy1",
                                  0.5 * (b_rotated->p3.Y() - b_rotated->p4.Y()) - assembly_spacing);
          param->set_double_param(prefix.str() + "pDx1",
                                  b->p4.Y() * tan(half_block_phi) - assembly_spacing);
          param->set_double_param(prefix.str() + "pDx2",
                                  b->p3.Y() * tan(half_block_phi) - assembly_spacing);

          param->set_double_param(prefix.str() + "pDy2",
                                  0.5 * (b_rotated->p1.Y() - b_rotated->p2.Y()) - assembly_spacing);
          param->set_double_param(prefix.str() + "pDx3",
                                  b->p2.Y() * tan(half_block_phi) - assembly_spacing);
          param->set_double_param(prefix.str() + "pDx4",
                                  b->p1.Y() * tan(half_block_phi) - assembly_spacing);
        }
        else
        {
          param->set_double_param(prefix.str() + "pDy1",
                                  0.5 * (b_rotated->p4.Y() - b_rotated->p3.Y()) - assembly_spacing);
          param->set_double_param(prefix.str() + "pDx1",
                                  b->p3.Y() * tan(half_block_phi) - assembly_spacing);
          param->set_double_param(prefix.str() + "pDx2",
                                  b->p4.Y() * tan(half_block_phi) - assembly_spacing);

          param->set_double_param(prefix.str() + "pDy2",
                                  0.5 * (b_rotated->p2.Y() - b_rotated->p1.Y()) - assembly_spacing);
          param->set_double_param(prefix.str() + "pDx3",
                                  b->p1.Y() * tan(half_block_phi) - assembly_spacing);
          param->set_double_param(prefix.str() + "pDx4",
                                  b->p2.Y() * tan(half_block_phi) - assembly_spacing);
        }
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
                                1 * inch_to_cm);
        param->set_double_param(prefix.str() + "LightguideTaperRatio",
                                0.545 / 1.039);
        param->set_string_param(prefix.str() + "LightguideMaterial", "PMMA");
      }
    }
  }

  param->set_int_param("sector_tower_map_size", cnt);

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

four_corner *
get_block(const int block_id, const int z_sign)
{
  //  From: Dan Cacace [mailto:dannycacace@gmail.com]
  //  Sent: Wednesday, March 15, 2017 4:27 PM
  //  To: Huang, Jin <jhuang@bnl.gov>
  //  Cc: Woody, Craig <woody@bnl.gov>; Lynch, Don <dlynch@bnl.gov>
  //  Subject: EMCal Layout Coordinates
  //
  //  Hi Jin,
  //
  //  Attached are the coordinates of the sheet metal box and blocks.
  //
  //  The gap between sheet metal boxes is 0.125" (3.175 mm), the gap between blocks is 0.015" (0.381 mm), and the gap between the box and blocks is also 0.015" (0.381 mm). Additionally, the sheet metal box is 0.030" (0.762 mm) thick and the distance between the tangent of a row of fibers and the edge of a block_id is 0.020" (0.5 mm). The fiber step pattern faces the blocks within a modal and the sheet metal box.
  //
  //  Let me know if I'm forgetting anything.
  //
  //  -Dan

  //  const double radial_shift = 97 - 93.87578546;
  const double radial_shift = 0;
  const double data[] =
      {
          107.9138855, -0.27305,
          107.9138855, -5.362621276,
          93.87578546, -0.27305,
          93.87578546, -5.362621276,
          107.9138855, -5.362621276,
          107.9138855, -10.45219255,
          93.87578546, -5.362621276,
          93.87578546, -10.45219255,
          107.9138855, -10.45219255,
          107.9138855, -15.54176383,
          93.87578546, -10.45219255,
          93.87578546, -15.54176383,
          107.9138855, -15.54176383,
          107.7619676, -21.35577554,
          94.00872863, -15.54176383,
          93.87578546, -20.62959852,
          107.9138855, -21.36372007,
          107.4639033, -27.13439494,
          94.27145581, -20.65029008,
          93.87578546, -25.72445806,
          107.9138855, -27.1810862,
          107.1763014, -32.89540798,
          94.52732384, -25.7920633,
          93.87578546, -30.83975917,
          107.9138855, -33.00940459,
          106.89999, -38.65565532,
          94.77533018, -30.97878746,
          93.87578546, -35.98823404,
          107.9138855, -38.8633061,
          106.6356031, -44.43110212,
          95.01464578, -36.22147825,
          93.87578546, -41.18199566,
          107.9138855, -44.75659859,
          106.3835878, -50.2368955,
          95.24461422, -41.53054847,
          93.87578546, -46.43259331,
          107.9138855, -50.70234217,
          106.1442164, -56.08741278,
          95.46474583, -46.91588248,
          93.87578546, -51.75105991,
          107.9138855, -56.71291325,
          105.9176023, -61.99630291,
          95.67470777, -52.38690013,
          93.87578546, -57.14795194,
          107.9138855, -62.80005881,
          105.7037169, -67.97652281,
          95.87431111, -57.95261073,
          93.87578546, -62.63338321,
          107.9138855, -68.97494244,
          105.502409, -74.04036989,
          96.0634958, -63.6216576,
          93.87578546, -68.21705304,
          107.9138855, -75.24818329,
          105.3134233, -80.19951155,
          96.24231468, -69.40235412,
          93.87578546, -73.9082697,
          107.9138855, -81.62988875,
          105.1364193, -86.46501223,
          96.410917, -75.30271197,
          93.87578546, -79.71596971,
          107.9138855, -88.12968203,
          104.9709886, -92.8473584,
          96.56953235, -81.3304625,
          93.87578546, -85.6487334,
          107.9138855, -94.75672509,
          104.8166707, -99.35648168,
          96.71845534, -87.49307228,
          93.87578546, -91.71479749,
          107.9138855, -101.5197378,
          104.6729681, -106.0017801,
          96.85803143, -93.79775342,
          93.87578546, -97.92206485,
          107.9138855, -108.4270137,
          104.539358, -112.7921379,
          96.98864406, -100.2514698,
          93.87578546, -104.278112,
          107.9138855, -115.4864333,
          104.4153043, -119.7359435,
          97.11070332, -106.8609401,
          93.87578546, -110.7901947,
          107.9138855, -122.7054746,
          104.3002657, -126.8411061,
          97.22463606, -113.6326372,
          93.87578546, -117.4652519,
          107.9138855, -130.0912223,
          104.1937043, -134.1150709,
          97.33087753, -120.5727871,
          93.87578546, -124.3099083,
          107.9138855, -137.6503751,
          104.0950911, -141.5648331,
          97.42986432, -127.6873647,
          93.87578546, -131.3304759,
          107.9138855, -145.3892519,
          104.0039111, -149.19695,
          97.52202869, -134.9820895,
          93.87578546, -138.532955,

          0, 0, 0, 0, 0};

  assert(block_id >= 0);
  assert(block_id < 24);

  const int offset = block_id * 8;

  four_corner *b = new four_corner;

  b->p1.Set(-z_sign * data[offset + 1], data[offset + 0] + radial_shift);
  b->p2.Set(-z_sign * data[offset + 1 + 2], data[offset + 0 + 2] + radial_shift);
  b->p3.Set(-z_sign * data[offset + 1 + 4], data[offset + 0 + 4] + radial_shift);
  b->p4.Set(-z_sign * data[offset + 1 + 6], data[offset + 0 + 6] + radial_shift);

  return b;
}
