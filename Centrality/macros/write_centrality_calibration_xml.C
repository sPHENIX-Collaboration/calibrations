#include <PHParameters.h>

void write_centrality_calibration_xml() {

  gSystem->Load("libg4detectors.so");

  PHParameters* param = new PHParameters("centrality");

  param->set_string_param("description","Centrality calibration from MDC-1, MBD and sEPD, based on Sum G4Hits");

  std::string line;
  std::ifstream myfile ("centrality_input_simple.txt");
  if (myfile.is_open())
    {

      // sEPD
      int cent = 0;
      for (int n = 0; n < 11; n++) {
	getline (myfile,line);
	float centile_value = atof(line.c_str());
	std::cout << " sEPD, centile " << cent * 10 << ", value = " << centile_value << std::endl;

	std::ostringstream s;
	s << "epd_centile_" << cent;
	param->set_double_param( s.str().c_str() , centile_value );

	cent += 10;
      }

      // MBD
      cent = 0;
      for (int n = 0; n < 11; n++) {
	getline (myfile,line);
	float centile_value = atof(line.c_str());
	std::cout << " MBD, centile " << cent * 10 << ", value = " << centile_value << std::endl;

	std::ostringstream s;
	s << "mbd_centile_" << cent;
	param->set_double_param( s.str().c_str() , centile_value );

	cent += 10;
      }

      // b (impact parameter)
      cent = 0;
      for (int n = 0; n < 11; n++) {
	getline (myfile,line);
	float centile_value = atof(line.c_str());
	std::cout << " b, centile " << cent * 10 << ", value = " << centile_value << std::endl;

	std::ostringstream s;
	s << "bimp_centile_" << cent;
	param->set_double_param( s.str().c_str() , centile_value );

	cent += 10;
      }

      myfile.close();
    }

  else {
    std::cout << "Unable to open calibration file" << std::endl;
  } 

  param->WriteToFile("xml", "..");

}
