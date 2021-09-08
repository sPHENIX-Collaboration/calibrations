/*
 * MVTX alignment generator
 * Cameron Dean
 * LANL 2021
 */

#include <g4mvtx/PHG4MvtxDefs.h>

#include <iostream>
#include <tuple>

#include <phparameter/PHParameters.h>
#include <phparameter/PHParametersContainer.h>

class StaveParameters
{
  public:
   StaveParameters();

    explicit StaveParameters(const int &layer,
                             const int &num,
                             const std::string &name,
                             const std::string &version,
                             const double &x_pos,
                             const double &y_pos,
                             const double &z_pos,
                             const double &tilt);

    virtual ~StaveParameters(){};

    const int get_layer();
    const int get_num();
    const std::string get_name();
    const std::string get_version();
    const double get_x_pos();
    const double get_y_pos();
    const double get_z_pos();
    const double get_tilt();

    void print();
  
  private:
    const int m_layer = 0;
    const int m_num = 0;
    const std::string m_name = "X000";
    const std::string m_version = "v1";
    const double m_x_pos = 0.0;
    const double m_y_pos = 0.0;
    const double m_z_pos = 0.0;
    const double m_tilt = 0.0;
};

StaveParameters::StaveParameters(const int &layer,
                                 const int &num,
                                 const std::string &name,
                                 const std::string &version,
                                 const double &x_pos,
                                 const double &y_pos,
                                 const double &z_pos,
                                 const double &tilt)
  : m_layer(layer)
  , m_num(num)
  , m_name(name)
  , m_version(version)
  , m_x_pos(x_pos)
  , m_y_pos(y_pos)
  , m_z_pos(z_pos)
  , m_tilt(tilt)
{}

const int StaveParameters::get_layer() { return m_layer; }
const int StaveParameters::get_num() { return m_num; }
const std::string StaveParameters::get_name() { return m_name; }
const std::string StaveParameters::get_version() { return m_version; }
const double StaveParameters::get_x_pos() { return m_x_pos; }
const double StaveParameters::get_y_pos() { return m_y_pos; }
const double StaveParameters::get_z_pos() { return m_z_pos; }
const double StaveParameters::get_tilt() { return m_tilt; }

void StaveParameters::print()
{
  std::cout << "***************************************" << std::endl;
  std::cout << "Layer:Number:Name:Version = " << m_layer << ":"
                                              << m_num << ":"
                                              << m_name  << ":"
                                              << m_version << std::endl;
  std::cout << "Offset = (" << m_x_pos << ", " 
                            << m_y_pos << ", "
                            << m_z_pos << ")" << std::endl;
  std::cout << "Tilt = " << m_tilt << std::endl;
}

void Construct_MVTXAlignment()
{
  gSystem->Load("libphparameter.so");

  bool verbose = true;

  PHParameters *param = new PHParameters("Alignmnent");
  std::string description = "MVTX alignment parameters";
  param->set_string_param("description", description);

  std::vector<StaveParameters*> mvtx_staves;
  mvtx_staves.push_back(new StaveParameters(0, 0, "X000", "v1", 0.0, 0.0, 1.0, 0.1));
  mvtx_staves.push_back(new StaveParameters(0, 1, "X000", "v1", 0.0, 0.0, 1.0, 0.1));
  mvtx_staves.push_back(new StaveParameters(1, 3, "X000", "v1", 0.3, 0.5, 1.0, 0.2));
  mvtx_staves.push_back(new StaveParameters(2, 9, "X000", "v1", 0.2, 0.0, 3.0, 0.3));
  mvtx_staves.push_back(new StaveParameters(2, 4, "X000", "v1", 0.0, 0.1, 5.0, 0.4));

  for (StaveParameters *stave: mvtx_staves)
  {
    if (verbose) stave->print();

    std::string stave_location = Form("layer_%i_stave_%i", stave->get_layer(), stave->get_num());
    param->set_string_param(stave_location + "_name", stave->get_name());
    param->set_double_param(stave_location + "_x_offset", stave->get_x_pos());
    param->set_double_param(stave_location + "_y_offset", stave->get_y_pos());
    param->set_double_param(stave_location + "_z_offset", stave->get_z_pos());
    param->set_double_param(stave_location + "_tilt", stave->get_tilt());
  }

  PHParametersContainer *paramcont = new PHParametersContainer("MVTX");  
  paramcont->AddPHParameters(PHG4MvtxDefs::ALIGNMENT, param);
  std::string dir_name = "alignment";
  gSystem->mkdir(dir_name.c_str());
  paramcont->WriteToFile("xml", dir_name);
}
