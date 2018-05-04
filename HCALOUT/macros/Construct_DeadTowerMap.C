// $$Id: $$
#include <TVector2.h>
#include <iostream>

using namespace std;

void Construct_DeadTowerMap(
//    const double dead_tower_ratio = 0.1,
//    const string description = "10-percent dead tower as assigned in threshold key performance parameter"//
            const double dead_tower_ratio = 0.05,
        const string description = "5-percent dead tower as assigned in objective key performance parameter"//
        )
{
  const int nbin_eta = 12 * 2 ;
  const int nbin_phi = 64;

  cout << "Construct_DeadTowerMap() - nbin_eta = " << nbin_eta << ", nbin_phi = " << nbin_phi << ", dead_tower_ratio = " << dead_tower_ratio << endl;

  gSystem->Load("libphparameter.so");

  TRandom3 rnd(1234);

  PHParameters *param = new PHParameters("HCALOUT");

  param->set_string_param("description", description);

  int cnt = 0;

  map<int, int> dead_channels;

  while (cnt < nbin_eta * nbin_phi * dead_tower_ratio)
  {
    const int eta = rnd.Integer(nbin_eta);
    const int phi = rnd.Integer(nbin_phi);

    const int ID = eta * nbin_phi + phi;

    if (dead_channels[ID] == 0)
    {
      //      dead_channels.insert(ID);
      dead_channels[ID] = 1;

      param->set_int_param(Form("deadtower_eta_%d_phi_%d", eta, phi), 1);
      ++cnt;
    }
  }

  param->set_int_param("deadtower_size", cnt);

  //  storage
  param->Print();

  string dir_name(Form("../DeadMap_%.0fPercent/", dead_tower_ratio * 100));
  gSystem->mkdir(dir_name.c_str());

  param->WriteToFile("xml", dir_name);
}
