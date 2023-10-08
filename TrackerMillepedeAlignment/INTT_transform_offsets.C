#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TFile.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TChain.h>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <Eigen/SVD>
#include <Math/Transform3D.h>
#include <fstream>
#include <map>

#include <trackbase/TrkrDefs.h>
#include <trackbase/InttDefs.h>


std::map<int, Eigen::Affine3d> getIdealTransforms()
{
  std::map<int, Eigen::Affine3d> ideal_map;

  //std::ifstream fideal("transform_out_file_current.txt",std::ifstream::in);
  std::ifstream fideal("transform_out_file_corrected.txt",std::ifstream::in);

  int hitsetkey;
  double t11,t12,t13,t14;
  double t21,t22,t23,t24;
  double t31,t32,t33,t34;
  double t41,t42,t43,t44;

  while(fideal.good())
    {
      fideal >> hitsetkey;
      fideal >> t11 >> t12 >> t13 >> t14;
      fideal >> t21 >> t22 >> t23 >> t24;
      fideal >> t31 >> t32 >> t33 >> t34;
      fideal >> t41 >> t42 >> t43 >> t44;

      Eigen::Matrix3d rot;
      rot << t11, t12, t13, 
	t21, t22, t23,
	t31, t32, t33;

      Eigen::Vector3d translation;
      translation << t14, t24, t34;

      Eigen::Matrix4d input;
      input.setIdentity();  // sets last row to 0,0,0,1
      input.block<3,3>(0,0) = rot;
      input.block<3,1>(0,3) = translation;
      Eigen::Affine3d transform(input);

      std::cout << "Ideal transform for hitsetkey " << hitsetkey << std::endl;
      std::cout <<transform.matrix() << std::endl;

      // Put it in a map
      ideal_map.insert(std::pair<int,Eigen::Affine3d>(hitsetkey, transform));

    }

  return ideal_map;

}

Eigen::Vector3d getTransformAngles(Eigen::Affine3d transform)
{
  // https://en.wikipedia.org/wiki/Euler_angles
  // Tait-Bryan angles from rotation matrix from Z1-Y2-X3 rotation order 

  // This is for x-y-z order
  double a = atan2(-transform.linear()(1, 2), transform.linear()(2, 2)); 
  double b = asin(transform.linear()(0, 2));                                                 
  double g = atan2(-transform.linear()(0, 1), transform.linear()(0, 0));    

  Eigen::Vector3d angles = {a,b,g};

  return angles;
}

Eigen::Vector3d getRotationAngles(Eigen::Matrix3d rot)
{
  // https://en.wikipedia.org/wiki/Euler_angles
  // Tait-Bryan angles from rotation matrix from Z1-Y2-X3 rotation order 

  // This is for x-y-z order
  double a = atan2(-rot(1, 2), rot(2, 2)); 
  double b = asin(rot(0, 2));                                                 
  double g = atan2(-rot(0, 1), rot(0, 0));    

  Eigen::Vector3d angles = {a,b,g};

  return angles;
}

Eigen::Affine3d flipTransformAxes(Eigen::Affine3d tin)
{
  // Flip columns 2 and 3 of the rotation matrix and change the sign of one of them

  Eigen::Matrix3d inrotation = tin.linear();
  Eigen::Matrix3d outrotation;
  outrotation(0,0) = inrotation(0,0);
  outrotation(1,0) = inrotation(1,0);
  outrotation(2,0) = inrotation(2,0);

  outrotation(0,1) = inrotation(0,2);
  outrotation(1,1) = inrotation(1,2);
  outrotation(2,1) = inrotation(2,2);

  outrotation(0,2) = -inrotation(0,1);
  outrotation(1,2) = -inrotation(1,1);
  outrotation(2,2) = -inrotation(2,1);

  Eigen::Affine3d tout;
  tout.linear() = outrotation;
  tout.translation() = tin.translation();

  return tout;
}

Eigen::Affine3d unflipTransformAxes(Eigen::Affine3d tin)
{
  // Flip columns 2 and 3 of the rotation matrix and change the sign of one of them

  Eigen::Matrix3d inrotation = tin.linear();
  Eigen::Matrix3d outrotation;
  outrotation(0,0) = inrotation(0,0);
  outrotation(1,0) = inrotation(1,0);
  outrotation(2,0) = inrotation(2,0);

  outrotation(0,1) = -inrotation(0,2);
  outrotation(1,1) = -inrotation(1,2);
  outrotation(2,1) = -inrotation(2,2);

  outrotation(0,2) = inrotation(0,1);
  outrotation(1,2) = inrotation(1,1);
  outrotation(2,2) = inrotation(2,1);

  Eigen::Affine3d tout;
  tout.linear() = outrotation;
  tout.translation() = tin.translation();

  return tout;
}

void extractPerturbationParameters(Eigen::Affine3d tsurv, Eigen::Affine3d tideal, Eigen::Vector3d& angles_diff, Eigen::Vector3d& transl_diff)
{
  Eigen::Vector3d angles_ideal = getTransformAngles(tideal);
  Eigen::Vector3d angles_surv = getTransformAngles(tsurv);
  angles_diff[0] = angles_surv[0] - angles_ideal[0];
  angles_diff[1] = angles_surv[1] - angles_ideal[1];
  angles_diff[2] =angles_surv[2] - angles_ideal[2];

  Eigen::Vector3d transl_ideal = {tideal.translation().x(),  tideal.translation().y(), tideal.translation().z()};
  Eigen::Vector3d transl_surv = {tsurv.translation().x(),  tsurv.translation().y(), tsurv.translation().z()};
   transl_diff[0] = transl_surv[0]-transl_ideal[0];
  transl_diff[1] = transl_surv[1]-transl_ideal[1];
  transl_diff[2] = transl_surv[2]-transl_ideal[2];

  return;
}

Eigen::Affine3d createAlignmentTransform(Eigen::Vector3d angle_parameters, Eigen::Vector3d transl_parameters, Eigen::Affine3d ideal_transform)
{
    //define null matrices
  Eigen::Vector3d nullTranslation(0,0,0);
  Eigen::AngleAxisd a(0, Eigen::Vector3d::UnitX());
  Eigen::AngleAxisd b(0, Eigen::Vector3d::UnitY());
  Eigen::AngleAxisd g(0, Eigen::Vector3d::UnitZ());
  Eigen::Quaternion<double> qnull       = g*b*a;
  Eigen::Matrix3d nullRotation = qnull.matrix();

  // Create alignment rotation matrix
  Eigen::AngleAxisd alpha(angle_parameters[0], Eigen::Vector3d::UnitX());
  Eigen::AngleAxisd beta(angle_parameters[1], Eigen::Vector3d::UnitY());
  Eigen::AngleAxisd gamma(angle_parameters[2], Eigen::Vector3d::UnitZ());
  Eigen::Quaternion<double> q       = gamma*beta*alpha; 
  Eigen::Matrix3d millepedeRotation = q.matrix();

  Eigen::Affine3d mpRotationAffine;   
  mpRotationAffine.linear() = millepedeRotation;
  mpRotationAffine.translation() = nullTranslation;   

  // create alignment translation matrix
  Eigen::Affine3d mpTranslationAffine;   
  mpTranslationAffine.linear() = nullRotation;
  mpTranslationAffine.translation() = transl_parameters;   

  // separate the ideal transform components into two affine transforms
  Eigen::Affine3d idealRotationAffine;
  Eigen::Matrix3d idealRotation = ideal_transform.linear();
  idealRotationAffine.linear() = idealRotation;
  idealRotationAffine.translation() = nullTranslation;

  Eigen::Affine3d idealTranslationAffine;
  idealTranslationAffine.linear() = nullRotation;
  idealTranslationAffine.translation() = ideal_transform.translation();

  // put it all together
  Eigen::Affine3d  combined_transform = mpTranslationAffine *  idealTranslationAffine *  mpRotationAffine * idealRotationAffine;
  
  return combined_transform;
}

void INTT_transform_offsets()
{
  // open output text file for survey alignment parameters
  std::ofstream fout("intt_data.txt",std::ofstream::out);

  auto ideal_map = getIdealTransforms();

  std::cout << "Ideal_map size(): " << ideal_map.size() << std::endl;

  TH1D *hx = new TH1D("hx","hx",2000,-40,40);
  TH1D *hy = new TH1D("hy","hy",2000,-40,40);
  TH1D *hz = new TH1D("hz","hz",2000,-40,40);
  TH2D *hxy = new TH2D("hxy","hxy", 2000, -120, 120, 2000, -120, 120);
  TH2D *hxy_ideal = new TH2D("hxy_ideal","hxy_ideal", 2000, -120, 120, 2000, -120, 120);
  TH2D * hphi_alpha_survey = new TH2D("hphi_alpha_survey", "hphi_alpha_survey", 200, -1, 16, 200, -185.0, 185.0);
  TH2D * hphi_alpha_ideal = new TH2D("hphi_alpha_ideal", "hphi_alpha_ideal", 200, -1, 16, 200, -185.0, 185.0);
  TH2D *hrphi_survey = new TH2D("hrphi_survey", "hrphi_survey", 200, -1, 16, 200, 0, 120);
  TH2D *hrphi_ideal = new TH2D("hrphi_ideal", "hrphi_ideal", 200, -1, 16, 200, 0, 120);

  TChain *tree = new TChain("intt_transforms");
  tree->Add("intt_transforms.root");

  double x,y,z,a,b,c;
  ROOT::Math::Transform3D *transform = new ROOT::Math::Transform3D();
  int hitsetkey;

  tree->SetBranchAddress("x",&x);
  tree->SetBranchAddress("y",&y);
  tree->SetBranchAddress("z",&z);
  tree->SetBranchAddress("a",&a);
  tree->SetBranchAddress("b",&b);
  tree->SetBranchAddress("c",&c);
  tree->SetBranchAddress("hitsetkey",&hitsetkey);
  tree->SetBranchAddress("transform",&transform);

  int nentries = tree->GetEntries();
  std::cout << " tree has " << nentries << " entries " << std::endl;
  for(int ientry=0;ientry<nentries;++ientry)
    {
      tree->GetEntry(ientry);

      std::cout << std::endl << "entry number " << ientry << "  hitsetkey:  " << hitsetkey << std::endl;

      Eigen::Affine3d tin;  // This has rotation order x/y/z in the local-global transform
      transform->GetTransformMatrix(tin);
      Eigen::Matrix4d t_matrix = tin.matrix();

      Eigen::Affine3d t = flipTransformAxes(tin);   // convert rotation order to x/z/y
      std::cout <<   "Flipped survey transform: "<< std::endl << t.matrix() << std::endl;

      // get the ideal transform
      Eigen::Affine3d ideal_transform;
      for(auto it = ideal_map.begin(); it != ideal_map.end(); ++it)
	{
	  if(it->first == hitsetkey)
	    {
	      ideal_transform = it->second;
	      break;
	    }
	}

      unsigned int layer = TrkrDefs::getLayer((unsigned int) hitsetkey);
      unsigned int ladz = InttDefs::getLadderZId((unsigned int) hitsetkey);
      unsigned int ladphi = InttDefs::getLadderPhiId((unsigned int) hitsetkey);

      // Get some values from the translations
      double survey_radius = sqrt(t_matrix(0,3)*t_matrix(0,3) + t_matrix(1,3)*t_matrix(1,3));
      Eigen::Matrix4d ideal_matrix = ideal_transform.matrix();
      double ideal_radius = sqrt(ideal_matrix(0,3)*ideal_matrix(0,3) + ideal_matrix(1,3)*ideal_matrix(1,3));
      double survey_alpha = atan2(t_matrix(1,3),t_matrix(0,3)) * 180.0/M_PI;
      double ideal_alpha = atan2(ideal_matrix(1,3),ideal_matrix(0,3)) * 180.0/M_PI;

      std::cout << " Readback: hitsetkey = " << hitsetkey << " layer " << layer 
		<< " ladder z ID " << ladz << " ladder phi ID " << ladphi << std::endl
		<< " survey radius " << survey_radius << " ideal radius " << ideal_radius 
		<< " survey alpha " << survey_alpha << " ideal alpha " << ideal_alpha
		<< std::endl;
      std::cout << "Ideal transform:" << std::endl << ideal_transform.matrix() << std::endl;

      Eigen::Vector3d ideal_angles = getTransformAngles(ideal_transform);
      Eigen::Vector3d survey_angles = getTransformAngles(t);

      std::cout << " Flipped survey angles: " << survey_angles[0]*180/M_PI << "  " << survey_angles[1]*180/M_PI << "  " << survey_angles[2]*180/M_PI << std::endl;
      std::cout << " ideal transform angles: " << ideal_angles[0]*180/M_PI << "  " << ideal_angles[1]*180/M_PI << "  " << ideal_angles[2]*180/M_PI << std::endl;

      if(layer == 3)  // for example
	{
	  hx->Fill(t_matrix(0,3) - ideal_matrix(0,3));
	  hy->Fill(t_matrix(1,3) - ideal_matrix(1,3));
	  hz->Fill(t_matrix(2,3) - ideal_matrix(2,3));
	  hxy->Fill(t_matrix(0,3), t_matrix(1,3));
	  hxy_ideal->Fill(ideal_matrix(0,3), ideal_matrix(1,3));
	  hphi_alpha_survey->Fill(ladphi, survey_alpha);
	  hphi_alpha_ideal->Fill(ladphi, ideal_alpha);
	}

       // We do one of the following:
       //======================
       //
       //    Either:
       //=======
       //    Flip the rotation order of the survey data to xzy by exchanging columns
       //    Extract the rotation matrix for the flipped survey and the ideal transforms
       //    Determine the perturbation rotation matrix
       //    Extract the perturbation angles
       //    Subtract the ideal translation parameters from the survey translation parameters to get the perturbation
       //    Combine the perturbations with the ideal transform to check that we recover the survey transform 

       std::cout << std::endl << " Flip the rotation order in the survey transform: " << std::endl;

       Eigen::Matrix3d surv_rot_matrix = t.linear();   // flipped rotation order
       Eigen::Matrix3d ideal_rot_matrix = ideal_transform.linear();
       Eigen::Matrix3d perturb_rot_matrix = surv_rot_matrix * ideal_rot_matrix.inverse();
       Eigen::Vector3d perturb_angles =  getRotationAngles(perturb_rot_matrix);

       Eigen::Vector3d surv_transl_vector = t.translation();
       Eigen::Vector3d ideal_transl_vector = ideal_transform.translation();
       Eigen::Vector3d perturb_transl_vector = surv_transl_vector - ideal_transl_vector;

     std::cout << "Perturbation parameters: alpha/beta/gamma/dx/dy/dz: " 
		 << "  " << perturb_angles[0] << "  " << perturb_angles[1] << "  " << perturb_angles[2] << "  "
		 << perturb_transl_vector[0] << "  " << perturb_transl_vector[1] << "  " << perturb_transl_vector[2] << std::endl;

       Eigen::Affine3d alignment_transform_check = createAlignmentTransform(perturb_angles, perturb_transl_vector, ideal_transform);
       std::cout << "alignment_transform_check: " << std::endl;
       std::cout << alignment_transform_check.matrix() << std::endl;
       std::cout <<   "Survey transform flipped: "<< std::endl << t.matrix() << std::endl;

       fout << hitsetkey << "  " << perturb_angles[0] << "  " << perturb_angles[1] << "  " << perturb_angles[2] << "  "
	    << perturb_transl_vector[0] << "  " << perturb_transl_vector[1] << "  " << perturb_transl_vector[2] << std::endl;

       //   Or:
       //====
       // Unflip the rotation order of the ideal transform and leave the survey transform alone

       std::cout << std::endl << " Unflip the rotation order in the ideal transform: " << std::endl;
       Eigen::Matrix3d surv_rot_matrix_2 = tin.linear();   // original survey rotation order

       Eigen::Affine3d ideal_transform_2 = unflipTransformAxes(ideal_transform);
       Eigen::Matrix3d ideal_rot_2 = ideal_transform_2.linear();  // unflipped ideal rotation order
       Eigen::Matrix3d perturb_rot_2 = surv_rot_matrix_2 * ideal_rot_2.inverse();
       Eigen::Vector3d perturb_angles_2 =  getRotationAngles(perturb_rot_2);

       Eigen::Vector3d surv_transl_vector_2 = tin.translation();
       Eigen::Vector3d ideal_transl_vector_2 = ideal_transform_2.translation();
       Eigen::Vector3d perturb_transl_2 = surv_transl_vector_2 - ideal_transl_vector_2;

       std::cout << "Perturbation parameters: alpha/beta/gamma/dx/dy/dz: " 
		 << "  " << perturb_angles_2[0] << "  " << perturb_angles_2[1] << "  " << perturb_angles_2[2] << "  "
		 << perturb_transl_2[0] << "  " << perturb_transl_2[1] << "  " << perturb_transl_2[2] << std::endl;

       Eigen::Affine3d alignment_transform_check_2 = createAlignmentTransform(perturb_angles_2, perturb_transl_2, ideal_transform);
       std::cout << "alignment_transform_check 2: " << std::endl;
       std::cout << alignment_transform_check_2.matrix() << std::endl;
       std::cout <<   "Survey transform original: "<< std::endl << tin.matrix() << std::endl;

    }

  TCanvas *cc = new TCanvas("cc","cc",5,5,800,800);
  cc->Divide(2,2);

  cc->cd(1);
  hx->DrawCopy();

  cc->cd(2);
  hy->DrawCopy();

  cc->cd(3);
  hz->DrawCopy();

  cc->cd(4);
  hxy->SetMarkerStyle(20);
  hxy->SetMarkerSize(0.5);
  hxy->DrawCopy();
  hxy_ideal->SetMarkerColor(kRed);
  hxy_ideal->SetMarkerStyle(20);
  hxy_ideal->SetMarkerSize(0.5);
  hxy_ideal->DrawCopy("same");

  TCanvas *calpha = new TCanvas("calpha", "calpha",150, 150,800,800);
  hphi_alpha_survey->SetMarkerStyle(20);
  hphi_alpha_survey->SetMarkerSize(0.5);
  hphi_alpha_survey->SetMarkerColor(kBlue);
  hphi_alpha_survey->DrawCopy();

  hphi_alpha_ideal->SetMarkerStyle(20);
  hphi_alpha_ideal->SetMarkerSize(0.5);
  hphi_alpha_ideal->SetMarkerColor(kRed);
  hphi_alpha_ideal->DrawCopy("same");

  std::cout << " Done" << std::endl;

  return;
}
