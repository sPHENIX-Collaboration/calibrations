// $$Id: $$
#include <TVector2.h>
#include <iostream>

using namespace std;

void Construct_DeadMap()
{
  for (int ladder_type = 0; ladder_type < 2; ++ladder_type)
    for (int rnd_seed = 0; rnd_seed < 4; ++rnd_seed)
    {
      Construct_DeadMap_Work(rnd_seed, ladder_type);
    }
}

void Construct_DeadMap_Work(const int rnd_seed = 3,
                            const int ladder_type = 1  // 0 z-sensitive, 1: phi-sensitive
)
{
  // Output format : Form("INTT_%02d_%d_%03d_%03d", ladder_phi, ladder_z, strip_z, strip_phi)
  //  defined in PHG4SiliconTrackerSubsystem::SetDefaultParameters(). The relevant values are:
  //
  //  ===========
  //   // We define default parameters for laddertype 0 or 1
  //    int nstrips_phi_sensor[2] = {1, 256};
  //    int nstrips_z_sensor_0[2] = {128*5, 8};  // inner sensor
  //    int nstrips_z_sensor_1[2] = {128*5, 5};  // outer sensor
  //
  //    for (int i = 0; i < 2; i++)
  //    {
  //      set_default_int_param(i, "nstrips_z_sensor_0", nstrips_z_sensor_0[i]);
  //      set_default_int_param(i, "nstrips_z_sensor_1", nstrips_z_sensor_1[i]);
  //      set_default_int_param(i, "nstrips_phi_sensor", nstrips_phi_sensor[i]);
  //    =============

//  const double dead_sensor_ratio = 0.50;
//  const double dead_fphx_ratio = 0.04;
//  const double dead_chan_ratio = 0.001;  //% 0.1% dead channel
  const double dead_sensor_ratio = 0.06;
  const double dead_fphx_ratio = 0.02;
  const double dead_chan_ratio = 0.001;  //% 0.1% dead channel
//  const double dead_sensor_ratio = 0.02;
//  const double dead_fphx_ratio = 0.02;
//  const double dead_chan_ratio = 0.001;  //% 0.1% dead channel
                                         //                                           const double dead_sensor_ratio = 0.02;
                                         //                                           const double dead_fphx_ratio = 0.00;
                                         //                                           const double dead_chan_ratio = 0.000;  //% 0.1% dead channel
  const string description = Form("INTT dead map with %f%% dead sensor %f%% dead FPHX chip %f%% isolated dead channel",
                                  dead_sensor_ratio, dead_fphx_ratio, dead_chan_ratio);

  const int nladder_phi = 64;  // max number of ladders, it is OK to over generate ladders
  const int nladder_z = 4;
  const int nstrip_z = ladder_type ? 8 : 128 * 5;
  const int nstrip_phi = ladder_type ? 256 : 1;

  const int nstrip_fphx = 128;

  cout << "Construct_DeadTowerMap() - constructing " << description << endl;

  gSystem->Load("libphparameter.so");

  TRandom3 rnd(rnd_seed);

  PHParameters *param = new PHParameters("SILICON_TRACKER");

  param->set_string_param("description", description);
  param->set_int_param("random_seed", rnd_seed);

  int cnt = 0;

  //----------------------------------------------
  // sensors
  //----------------------------------------------

  cnt = 0;
  int nDead = rnd.Poisson(nladder_phi * nladder_z * dead_sensor_ratio);
  while (cnt < nDead)
  {
    const int ladder_phi = rnd.Integer(nladder_phi);
    const int ladder_z = rnd.Integer(nladder_z);
    const int strip_z = -1;
    const int strip_phi = -1;

    const string ID = Form("INTT_%02d_%d_%03d_%03d", ladder_phi, ladder_z, strip_z, strip_phi);

    if (!param->exist_int_param(ID))
    {
      param->set_int_param(ID, 1);
      ++cnt;
    }
  }

  cout << "Number of dead sensor = " << cnt << endl;

  param->set_int_param("dead_sensor_count", cnt);

  //----------------------------------------------
  // fphx chips
  //----------------------------------------------

  cnt = 0;
  int nDead = rnd.Poisson(nladder_phi * nladder_z * nstrip_z * nstrip_phi * dead_fphx_ratio / nstrip_fphx);
  if (ladder_type)  // phi-sensitive strips
  {
    while (cnt < nDead)
    {
      const int ladder_phi = rnd.Integer(nladder_phi);
      const int ladder_z = rnd.Integer(nladder_z);
      const int strip_z = rnd.Integer(nstrip_z);

      const int chip = rnd.Integer(nstrip_phi / nstrip_fphx);
      const int strip_phi = chip * nstrip_fphx;

      const string ID = Form("INTT_%02d_%d_%03d_%03d", ladder_phi, ladder_z, strip_z, strip_phi);

      if (!param->exist_int_param(ID))
      {
        for (int i = 0; i < nstrip_fphx; ++i)
        {
          const string stripID = Form("INTT_%02d_%d_%03d_%03d", ladder_phi, ladder_z, strip_z, strip_phi + i);

          cout << "stripID = " << stripID << " strip_phi = " << strip_phi << " i = " << i << endl;
          param->set_int_param(stripID, 1);
        }

        ++cnt;
      }
    }
  }
  else  // z-sensitive strips
  {
    while (cnt < nDead)
    {
      const int ladder_phi = rnd.Integer(nladder_phi);
      const int ladder_z = rnd.Integer(nladder_z);

      const int chip = rnd.Integer(nstrip_z / nstrip_fphx);
      const int strip_z = chip * nstrip_fphx;

      const int strip_phi = rnd.Integer(nstrip_phi);

      const string ID = Form("INTT_%02d_%d_%03d_%03d", ladder_phi, ladder_z, strip_z, strip_phi);

      if (!param->exist_int_param(ID))
      {
        for (int i = 0; i < nstrip_fphx; ++i)
        {
          const string stripID = Form("INTT_%02d_%d_%03d_%03d", ladder_phi, ladder_z, strip_z + i, strip_phi);

          param->set_int_param(stripID, 1);
        }

        ++cnt;
      }
    }
  }

  cout << "Number of dead fphx chip = " << cnt << endl;

  param->set_int_param("dead_fphx_count", cnt);
  //----------------------------------------------
  // channels
  //----------------------------------------------

  cnt = 0;
  int nDead = rnd.Poisson(nladder_phi * nladder_z * nstrip_z * nstrip_phi * dead_chan_ratio);
  while (cnt < nDead)
  {
    const int ladder_phi = rnd.Integer(nladder_phi);
    const int ladder_z = rnd.Integer(nladder_z);
    const int strip_z = rnd.Integer(nstrip_z);
    const int strip_phi = rnd.Integer(nstrip_phi);

    const string ID = Form("INTT_%02d_%d_%03d_%03d", ladder_phi, ladder_z, strip_z, strip_phi);

    if (!param->exist_int_param(ID))
    {
      param->set_int_param(ID, 1);

      ++cnt;
    }
  }
  cout << "Number of dead channel = " << cnt << endl;
  param->set_int_param("dead_chan_count", cnt);

  //  storage
  param->Print();

  string dir_name(Form("../DeadMap_%.0fPercent/LadderType%d_RndSeed%d",
                       100 * (dead_sensor_ratio + dead_fphx_ratio + dead_chan_ratio),
                       ladder_type, rnd_seed));

  cout << "output to " << dir_name << endl;

  gSystem->mkdir(dir_name.c_str(), true);

  param->WriteToFile("xml", dir_name);
}
