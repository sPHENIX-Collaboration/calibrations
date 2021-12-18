// $$Id: $$
R__LOAD_LIBRARY(libphparameter.so)
#include <phparameter/PHParameters.h>
#include <G4_Intt.C>
#include <TVector2.h>
#include <iostream>

using namespace std;

void Construct_DeadMap_Work(const int layer);

void Construct_DeadMap()
{
  for (int layer = 0; layer < G4INTT::n_intt_layer; ++layer)  // range depend on MVTX ON/OFF???
  {
    Construct_DeadMap_Work(layer);
  }
}

void Construct_DeadMap_Work(const int layer)
{
  // Output format : Form("INTT_%02d_%d_%03d_%03d", ladder_phi, ladder_z, strip_z, strip_phi)
  const double dead_sensor_ratio = 0.00; // no dead senor for now
  const double dead_fphx_ratio = 0.00;   // no dead fphx chip for now
  const double dead_chan_ratio = 0.01;  // assume 1%

  const string description = Form(
      "INTT dead map with %f%% dead sensor %f%% dead FPHX chip %f%% isolated dead channel",
      dead_sensor_ratio, dead_fphx_ratio, dead_chan_ratio);

  const int *nladder_phi = G4INTT::nladder; // number of ladder in each layer
  const int nladder_z = 4;  // no of sensors in Z-axis
  const int nstrip_z[nladder_z] = {8, 5, 8, 5};  // type-B(idx:1) == type-A(idx:0) == (origin) == type-A(idx:2) == type-B(idx:3) ---> Z-axis
  const int nstrip_phi = 256;   // 128 strip X 2 cells

  const int nstrip_fphx = 128; // no of strips per fphx

  int total_channel = nladder_phi[layer] * nladder_z/2 * nstrip_z[0] * nstrip_phi +  // type-A
                      nladder_phi[layer] * nladder_z/2 * nstrip_z[1] * nstrip_phi;    // type-B

  cout << "Construct_DeadTowerMap() - constructing " << description << endl;

  TRandom3 rnd(layer);

  PHParameters *param = new PHParameters("INTT");

  param->set_string_param("description", description);
  param->set_int_param("random_seed", layer);

  int cnt = 0;

  //----------------------------------------------
  // sensors
  //----------------------------------------------

  cnt = 0;
  int nDead = rnd.Poisson(nladder_phi[layer] * nladder_z * dead_sensor_ratio);
  while (cnt < nDead)
  {
    const int ladder_phi = rnd.Integer(nladder_phi[layer]);
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
  nDead = rnd.Poisson(total_channel * dead_fphx_ratio / nstrip_fphx);

  while (cnt < nDead)
  {
    const int ladder_phi = rnd.Integer(nladder_phi[layer]);
    const int ladder_z = rnd.Integer(nladder_z);
    const int strip_z = rnd.Integer(nstrip_z[ladder_z]);

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

  cout << "Number of dead fphx chip = " << cnt << endl;

  param->set_int_param("dead_fphx_count", cnt);
  //----------------------------------------------
  // channels
  //----------------------------------------------

  cnt = 0;
  //nDead = rnd.Poisson(nladder_phi[layer] * nladder_z * nstrip_z * nstrip_phi * dead_chan_ratio);
  nDead = rnd.Poisson(total_channel * dead_chan_ratio);
  while (cnt < nDead)
  {
    const int ladder_phi = rnd.Integer(nladder_phi[layer]);
    const int ladder_z = rnd.Integer(nladder_z);
    const int strip_z = rnd.Integer(nstrip_z[ladder_z]);
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

  string dir_name(Form("../DeadMap/intt_layer%d", layer));

  cout << "output to " << dir_name << endl;

  gSystem->mkdir(dir_name.c_str(), true);

  param->WriteToFile("xml", dir_name);
}
