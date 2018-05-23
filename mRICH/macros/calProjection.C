#include "TMath.h"

void calProjection(double theta = 0)
{
  double rotateTheta = theta*TMath::Pi()/180.0;
  // double rotateTheta = theta;
  // double z_max = 2760.0; // *mm
  double z_max = 2808.5; // *mm

  double eta_min = 1.1;
  // double eta_min = 1.242;
  double eta_max = 1.85;
  double theta_min = 2.0*atan(exp(-1.0*eta_max));
  double theta_max = 2.0*atan(exp(-1.0*eta_min));

  double y_min = z_max*tan(theta_min);

  double numerator = z_max*TMath::Tan(rotateTheta)*(TMath::Tan(theta_max)-TMath::Tan(theta_min));
  double denominator = 1.0 + TMath::Tan(rotateTheta)*TMath::Tan(theta_max);

  double z_length = numerator/denominator; // *mm

  cout << "z_length = " << z_length << endl;

  // final shift is z_max+y_min-halflength_z*cos(theta)
}
