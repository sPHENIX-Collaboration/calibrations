// $Id: $                                                                                             

/*!
 * \file LightCollection.C
 * \brief Construct the light collection data file for PHG4FullProjSpacalCellReco::LightCollectionModel
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <TMath.h>

#include <iostream>
#include <cassert>

using namespace std;

void
LightCollection()
{
//  gStyle->SetOptStat(0);

  TFile * fdata = new TXMLFile("../LightCollection/Prototype2Module.xml",
      "recreate");

//  TH2 * h2 = LoadMikePhippsLightGuideEff();
//  TH2 * h2 = LoadMikePhippsLightGuideEffUpdated();
  TH2 * h2 = LoadSeanStollLightGuideEff();

  h2->SetDirectory(NULL);
  fdata->cd();
  h2->Write();

  TH1 * h1 = LoadSCSF78Fiber();

  h1->SetDirectory(NULL);
  fdata->cd();
  h1->Write();

  fdata->Print();
}

TH2 *
LoadSeanStollLightGuideEff()
{

//> From: Sean Stoll [mailto:stoll@bnl.gov]
//> Sent: Friday, August 12, 2016 6:36 PM
//> To: Huang, Jin <jhuang@bnl.gov>
//> Subject: light collection mapping data
//> 
//> Jin,
//> 
//>   Here is the response mapping data that I have now for 1 absorber 
//> block tower w/ light guide, and 2 other different light guides / DAQ channels.
//> 
//> 
//> We can talk about what additional or more appropriate measurements 
//> would be helpful, but I hope this is enough to get you started.
//> 
//> 
//> Sean
//> 
//> 
//> --
//> Sean Stoll
//> Physics Associate
//> Brookhaven Nat. Lab
//> Upton, NY 11973
//> 631.344.5331

  TH2F * data_grid_light_guide_efficiency =
      new TH2F("data_grid_light_guide_efficiency",
          "Extracted light collection efficiency from Sean Stoll;x position fraction;y position fraction", //
          22, 0., 1., 24, 0., 1.);


data_grid_light_guide_efficiency->SetBinContent(1, 1, 0.812252);
data_grid_light_guide_efficiency->SetBinContent(1, 2, 0.913868);
data_grid_light_guide_efficiency->SetBinContent(1, 3, 0.957109);
data_grid_light_guide_efficiency->SetBinContent(1, 4, 0.966568);
data_grid_light_guide_efficiency->SetBinContent(1, 5, 0.978459);
data_grid_light_guide_efficiency->SetBinContent(1, 6, 0.992783);
data_grid_light_guide_efficiency->SetBinContent(1, 7, 1.003323);
data_grid_light_guide_efficiency->SetBinContent(1, 8, 1.009133);
data_grid_light_guide_efficiency->SetBinContent(1, 9, 1.010214);
data_grid_light_guide_efficiency->SetBinContent(1, 10, 0.992648);
data_grid_light_guide_efficiency->SetBinContent(1, 11, 0.971568);
data_grid_light_guide_efficiency->SetBinContent(1, 12, 0.946974);
data_grid_light_guide_efficiency->SetBinContent(1, 13, 0.926976);
data_grid_light_guide_efficiency->SetBinContent(1, 14, 0.904544);
data_grid_light_guide_efficiency->SetBinContent(1, 15, 0.879681);
data_grid_light_guide_efficiency->SetBinContent(1, 16, 0.846710);
data_grid_light_guide_efficiency->SetBinContent(1, 17, 0.815495);
data_grid_light_guide_efficiency->SetBinContent(1, 18, 0.786037);
data_grid_light_guide_efficiency->SetBinContent(1, 19, 0.762525);
data_grid_light_guide_efficiency->SetBinContent(1, 20, 0.743337);
data_grid_light_guide_efficiency->SetBinContent(1, 21, 0.728473);
data_grid_light_guide_efficiency->SetBinContent(1, 22, 0.721717);
data_grid_light_guide_efficiency->SetBinContent(1, 23, 0.705096);
data_grid_light_guide_efficiency->SetBinContent(1, 24, 0.678611);
data_grid_light_guide_efficiency->SetBinContent(2, 1, 0.945893);
data_grid_light_guide_efficiency->SetBinContent(2, 2, 1.050347);
data_grid_light_guide_efficiency->SetBinContent(2, 3, 1.092507);
data_grid_light_guide_efficiency->SetBinContent(2, 4, 1.109803);
data_grid_light_guide_efficiency->SetBinContent(2, 5, 1.128181);
data_grid_light_guide_efficiency->SetBinContent(2, 6, 1.147639);
data_grid_light_guide_efficiency->SetBinContent(2, 7, 1.162773);
data_grid_light_guide_efficiency->SetBinContent(2, 8, 1.168854);
data_grid_light_guide_efficiency->SetBinContent(2, 9, 1.165881);
data_grid_light_guide_efficiency->SetBinContent(2, 10, 1.152368);
data_grid_light_guide_efficiency->SetBinContent(2, 11, 1.133856);
data_grid_light_guide_efficiency->SetBinContent(2, 12, 1.110344);
data_grid_light_guide_efficiency->SetBinContent(2, 13, 1.083859);
data_grid_light_guide_efficiency->SetBinContent(2, 14, 1.054401);
data_grid_light_guide_efficiency->SetBinContent(2, 15, 1.021970);
data_grid_light_guide_efficiency->SetBinContent(2, 16, 0.981162);
data_grid_light_guide_efficiency->SetBinContent(2, 17, 0.943056);
data_grid_light_guide_efficiency->SetBinContent(2, 18, 0.907652);
data_grid_light_guide_efficiency->SetBinContent(2, 19, 0.876032);
data_grid_light_guide_efficiency->SetBinContent(2, 20, 0.850088);
data_grid_light_guide_efficiency->SetBinContent(2, 21, 0.829819);
data_grid_light_guide_efficiency->SetBinContent(2, 22, 0.821981);
data_grid_light_guide_efficiency->SetBinContent(2, 23, 0.818063);
data_grid_light_guide_efficiency->SetBinContent(2, 24, 0.818063);
data_grid_light_guide_efficiency->SetBinContent(3, 1, 0.927516);
data_grid_light_guide_efficiency->SetBinContent(3, 2, 1.020349);
data_grid_light_guide_efficiency->SetBinContent(3, 3, 1.088453);
data_grid_light_guide_efficiency->SetBinContent(3, 4, 1.117641);
data_grid_light_guide_efficiency->SetBinContent(3, 5, 1.141964);
data_grid_light_guide_efficiency->SetBinContent(3, 6, 1.161422);
data_grid_light_guide_efficiency->SetBinContent(3, 7, 1.172773);
data_grid_light_guide_efficiency->SetBinContent(3, 8, 1.175610);
data_grid_light_guide_efficiency->SetBinContent(3, 9, 1.169935);
data_grid_light_guide_efficiency->SetBinContent(3, 10, 1.160206);
data_grid_light_guide_efficiency->SetBinContent(3, 11, 1.143585);
data_grid_light_guide_efficiency->SetBinContent(3, 12, 1.120073);
data_grid_light_guide_efficiency->SetBinContent(3, 13, 1.090885);
data_grid_light_guide_efficiency->SetBinContent(3, 14, 1.060076);
data_grid_light_guide_efficiency->SetBinContent(3, 15, 1.027646);
data_grid_light_guide_efficiency->SetBinContent(3, 16, 0.983054);
data_grid_light_guide_efficiency->SetBinContent(3, 17, 0.944137);
data_grid_light_guide_efficiency->SetBinContent(3, 18, 0.910895);
data_grid_light_guide_efficiency->SetBinContent(3, 19, 0.882519);
data_grid_light_guide_efficiency->SetBinContent(3, 20, 0.857385);
data_grid_light_guide_efficiency->SetBinContent(3, 21, 0.835494);
data_grid_light_guide_efficiency->SetBinContent(3, 22, 0.820090);
data_grid_light_guide_efficiency->SetBinContent(3, 23, 0.831035);
data_grid_light_guide_efficiency->SetBinContent(3, 24, 0.868330);
data_grid_light_guide_efficiency->SetBinContent(4, 1, 0.957514);
data_grid_light_guide_efficiency->SetBinContent(4, 2, 1.043320);
data_grid_light_guide_efficiency->SetBinContent(4, 3, 1.074670);
data_grid_light_guide_efficiency->SetBinContent(4, 4, 1.106560);
data_grid_light_guide_efficiency->SetBinContent(4, 5, 1.132234);
data_grid_light_guide_efficiency->SetBinContent(4, 6, 1.151693);
data_grid_light_guide_efficiency->SetBinContent(4, 7, 1.166827);
data_grid_light_guide_efficiency->SetBinContent(4, 8, 1.176151);
data_grid_light_guide_efficiency->SetBinContent(4, 9, 1.179664);
data_grid_light_guide_efficiency->SetBinContent(4, 10, 1.166151);
data_grid_light_guide_efficiency->SetBinContent(4, 11, 1.147098);
data_grid_light_guide_efficiency->SetBinContent(4, 12, 1.122505);
data_grid_light_guide_efficiency->SetBinContent(4, 13, 1.094399);
data_grid_light_guide_efficiency->SetBinContent(4, 14, 1.061428);
data_grid_light_guide_efficiency->SetBinContent(4, 15, 1.023592);
data_grid_light_guide_efficiency->SetBinContent(4, 16, 0.977919);
data_grid_light_guide_efficiency->SetBinContent(4, 17, 0.936570);
data_grid_light_guide_efficiency->SetBinContent(4, 18, 0.899545);
data_grid_light_guide_efficiency->SetBinContent(4, 19, 0.878735);
data_grid_light_guide_efficiency->SetBinContent(4, 20, 0.860628);
data_grid_light_guide_efficiency->SetBinContent(4, 21, 0.845223);
data_grid_light_guide_efficiency->SetBinContent(4, 22, 0.849277);
data_grid_light_guide_efficiency->SetBinContent(4, 23, 0.862655);
data_grid_light_guide_efficiency->SetBinContent(4, 24, 0.885356);
data_grid_light_guide_efficiency->SetBinContent(5, 1, 0.984135);
data_grid_light_guide_efficiency->SetBinContent(5, 2, 1.067914);
data_grid_light_guide_efficiency->SetBinContent(5, 3, 1.080345);
data_grid_light_guide_efficiency->SetBinContent(5, 4, 1.105749);
data_grid_light_guide_efficiency->SetBinContent(5, 5, 1.126289);
data_grid_light_guide_efficiency->SetBinContent(5, 6, 1.141964);
data_grid_light_guide_efficiency->SetBinContent(5, 7, 1.160341);
data_grid_light_guide_efficiency->SetBinContent(5, 8, 1.174800);
data_grid_light_guide_efficiency->SetBinContent(5, 9, 1.185340);
data_grid_light_guide_efficiency->SetBinContent(5, 10, 1.171827);
data_grid_light_guide_efficiency->SetBinContent(5, 11, 1.152909);
data_grid_light_guide_efficiency->SetBinContent(5, 12, 1.128586);
data_grid_light_guide_efficiency->SetBinContent(5, 13, 1.100479);
data_grid_light_guide_efficiency->SetBinContent(5, 14, 1.066157);
data_grid_light_guide_efficiency->SetBinContent(5, 15, 1.025619);
data_grid_light_guide_efficiency->SetBinContent(5, 16, 0.978594);
data_grid_light_guide_efficiency->SetBinContent(5, 17, 0.936570);
data_grid_light_guide_efficiency->SetBinContent(5, 18, 0.899545);
data_grid_light_guide_efficiency->SetBinContent(5, 19, 0.884410);
data_grid_light_guide_efficiency->SetBinContent(5, 20, 0.874411);
data_grid_light_guide_efficiency->SetBinContent(5, 21, 0.869546);
data_grid_light_guide_efficiency->SetBinContent(5, 22, 0.883870);
data_grid_light_guide_efficiency->SetBinContent(5, 23, 0.896437);
data_grid_light_guide_efficiency->SetBinContent(5, 24, 0.907247);
data_grid_light_guide_efficiency->SetBinContent(6, 1, 1.007376);
data_grid_light_guide_efficiency->SetBinContent(6, 2, 1.094128);
data_grid_light_guide_efficiency->SetBinContent(6, 3, 1.105479);
data_grid_light_guide_efficiency->SetBinContent(6, 4, 1.115208);
data_grid_light_guide_efficiency->SetBinContent(6, 5, 1.124127);
data_grid_light_guide_efficiency->SetBinContent(6, 6, 1.132234);
data_grid_light_guide_efficiency->SetBinContent(6, 7, 1.153314);
data_grid_light_guide_efficiency->SetBinContent(6, 8, 1.171557);
data_grid_light_guide_efficiency->SetBinContent(6, 9, 1.186961);
data_grid_light_guide_efficiency->SetBinContent(6, 10, 1.177232);
data_grid_light_guide_efficiency->SetBinContent(6, 11, 1.161017);
data_grid_light_guide_efficiency->SetBinContent(6, 12, 1.138315);
data_grid_light_guide_efficiency->SetBinContent(6, 13, 1.109128);
data_grid_light_guide_efficiency->SetBinContent(6, 14, 1.074265);
data_grid_light_guide_efficiency->SetBinContent(6, 15, 1.033726);
data_grid_light_guide_efficiency->SetBinContent(6, 16, 0.985080);
data_grid_light_guide_efficiency->SetBinContent(6, 17, 0.944137);
data_grid_light_guide_efficiency->SetBinContent(6, 18, 0.910895);
data_grid_light_guide_efficiency->SetBinContent(6, 19, 0.899545);
data_grid_light_guide_efficiency->SetBinContent(6, 20, 0.898734);
data_grid_light_guide_efficiency->SetBinContent(6, 21, 0.908463);
data_grid_light_guide_efficiency->SetBinContent(6, 22, 0.923868);
data_grid_light_guide_efficiency->SetBinContent(6, 23, 0.932381);
data_grid_light_guide_efficiency->SetBinContent(6, 24, 0.934002);
data_grid_light_guide_efficiency->SetBinContent(7, 1, 1.050077);
data_grid_light_guide_efficiency->SetBinContent(7, 2, 1.143315);
data_grid_light_guide_efficiency->SetBinContent(7, 3, 1.155747);
data_grid_light_guide_efficiency->SetBinContent(7, 4, 1.160611);
data_grid_light_guide_efficiency->SetBinContent(7, 5, 1.163584);
data_grid_light_guide_efficiency->SetBinContent(7, 6, 1.164665);
data_grid_light_guide_efficiency->SetBinContent(7, 7, 1.181961);
data_grid_light_guide_efficiency->SetBinContent(7, 8, 1.195880);
data_grid_light_guide_efficiency->SetBinContent(7, 9, 1.206419);
data_grid_light_guide_efficiency->SetBinContent(7, 10, 1.199393);
data_grid_light_guide_efficiency->SetBinContent(7, 11, 1.181826);
data_grid_light_guide_efficiency->SetBinContent(7, 12, 1.153720);
data_grid_light_guide_efficiency->SetBinContent(7, 13, 1.127235);
data_grid_light_guide_efficiency->SetBinContent(7, 14, 1.092372);
data_grid_light_guide_efficiency->SetBinContent(7, 15, 1.049131);
data_grid_light_guide_efficiency->SetBinContent(7, 16, 1.004269);
data_grid_light_guide_efficiency->SetBinContent(7, 17, 0.963325);
data_grid_light_guide_efficiency->SetBinContent(7, 18, 0.926300);
data_grid_light_guide_efficiency->SetBinContent(7, 19, 0.916571);
data_grid_light_guide_efficiency->SetBinContent(7, 20, 0.915219);
data_grid_light_guide_efficiency->SetBinContent(7, 21, 0.922246);
data_grid_light_guide_efficiency->SetBinContent(7, 22, 0.938732);
data_grid_light_guide_efficiency->SetBinContent(7, 23, 0.949947);
data_grid_light_guide_efficiency->SetBinContent(7, 24, 0.955893);
data_grid_light_guide_efficiency->SetBinContent(8, 1, 1.028997);
data_grid_light_guide_efficiency->SetBinContent(8, 2, 1.113722);
data_grid_light_guide_efficiency->SetBinContent(8, 3, 1.126964);
data_grid_light_guide_efficiency->SetBinContent(8, 4, 1.140747);
data_grid_light_guide_efficiency->SetBinContent(8, 5, 1.147504);
data_grid_light_guide_efficiency->SetBinContent(8, 6, 1.147234);
data_grid_light_guide_efficiency->SetBinContent(8, 7, 1.158044);
data_grid_light_guide_efficiency->SetBinContent(8, 8, 1.167503);
data_grid_light_guide_efficiency->SetBinContent(8, 9, 1.175610);
data_grid_light_guide_efficiency->SetBinContent(8, 10, 1.167773);
data_grid_light_guide_efficiency->SetBinContent(8, 11, 1.151423);
data_grid_light_guide_efficiency->SetBinContent(8, 12, 1.126559);
data_grid_light_guide_efficiency->SetBinContent(8, 13, 1.096290);
data_grid_light_guide_efficiency->SetBinContent(8, 14, 1.060752);
data_grid_light_guide_efficiency->SetBinContent(8, 15, 1.019943);
data_grid_light_guide_efficiency->SetBinContent(8, 16, 0.977243);
data_grid_light_guide_efficiency->SetBinContent(8, 17, 0.938326);
data_grid_light_guide_efficiency->SetBinContent(8, 18, 0.903193);
data_grid_light_guide_efficiency->SetBinContent(8, 19, 0.896707);
data_grid_light_guide_efficiency->SetBinContent(8, 20, 0.896031);
data_grid_light_guide_efficiency->SetBinContent(8, 21, 0.901166);
data_grid_light_guide_efficiency->SetBinContent(8, 22, 0.915490);
data_grid_light_guide_efficiency->SetBinContent(8, 23, 0.925759);
data_grid_light_guide_efficiency->SetBinContent(8, 24, 0.931975);
data_grid_light_guide_efficiency->SetBinContent(9, 1, 0.944137);
data_grid_light_guide_efficiency->SetBinContent(9, 2, 1.005350);
data_grid_light_guide_efficiency->SetBinContent(9, 3, 1.019133);
data_grid_light_guide_efficiency->SetBinContent(9, 4, 1.055617);
data_grid_light_guide_efficiency->SetBinContent(9, 5, 1.075886);
data_grid_light_guide_efficiency->SetBinContent(9, 6, 1.079940);
data_grid_light_guide_efficiency->SetBinContent(9, 7, 1.081562);
data_grid_light_guide_efficiency->SetBinContent(9, 8, 1.086426);
data_grid_light_guide_efficiency->SetBinContent(9, 9, 1.094534);
data_grid_light_guide_efficiency->SetBinContent(9, 10, 1.082372);
data_grid_light_guide_efficiency->SetBinContent(9, 11, 1.069805);
data_grid_light_guide_efficiency->SetBinContent(9, 12, 1.056833);
data_grid_light_guide_efficiency->SetBinContent(9, 13, 1.016295);
data_grid_light_guide_efficiency->SetBinContent(9, 14, 0.979405);
data_grid_light_guide_efficiency->SetBinContent(9, 15, 0.946164);
data_grid_light_guide_efficiency->SetBinContent(9, 16, 0.904004);
data_grid_light_guide_efficiency->SetBinContent(9, 17, 0.869141);
data_grid_light_guide_efficiency->SetBinContent(9, 18, 0.841575);
data_grid_light_guide_efficiency->SetBinContent(9, 19, 0.839953);
data_grid_light_guide_efficiency->SetBinContent(9, 20, 0.841169);
data_grid_light_guide_efficiency->SetBinContent(9, 21, 0.845223);
data_grid_light_guide_efficiency->SetBinContent(9, 22, 0.854142);
data_grid_light_guide_efficiency->SetBinContent(9, 23, 0.859817);
data_grid_light_guide_efficiency->SetBinContent(9, 24, 0.862249);
data_grid_light_guide_efficiency->SetBinContent(10, 1, 0.975216);
data_grid_light_guide_efficiency->SetBinContent(10, 2, 1.039942);
data_grid_light_guide_efficiency->SetBinContent(10, 3, 1.061292);
data_grid_light_guide_efficiency->SetBinContent(10, 4, 1.078859);
data_grid_light_guide_efficiency->SetBinContent(10, 5, 1.086967);
data_grid_light_guide_efficiency->SetBinContent(10, 6, 1.085615);
data_grid_light_guide_efficiency->SetBinContent(10, 7, 1.093723);
data_grid_light_guide_efficiency->SetBinContent(10, 8, 1.098588);
data_grid_light_guide_efficiency->SetBinContent(10, 9, 1.100209);
data_grid_light_guide_efficiency->SetBinContent(10, 10, 1.088588);
data_grid_light_guide_efficiency->SetBinContent(10, 11, 1.072238);
data_grid_light_guide_efficiency->SetBinContent(10, 12, 1.051158);
data_grid_light_guide_efficiency->SetBinContent(10, 13, 0.998728);
data_grid_light_guide_efficiency->SetBinContent(10, 14, 0.961838);
data_grid_light_guide_efficiency->SetBinContent(10, 15, 0.940488);
data_grid_light_guide_efficiency->SetBinContent(10, 16, 0.904274);
data_grid_light_guide_efficiency->SetBinContent(10, 17, 0.872654);
data_grid_light_guide_efficiency->SetBinContent(10, 18, 0.845629);
data_grid_light_guide_efficiency->SetBinContent(10, 19, 0.841305);
data_grid_light_guide_efficiency->SetBinContent(10, 20, 0.839007);
data_grid_light_guide_efficiency->SetBinContent(10, 21, 0.838737);
data_grid_light_guide_efficiency->SetBinContent(10, 22, 0.841710);
data_grid_light_guide_efficiency->SetBinContent(10, 23, 0.848737);
data_grid_light_guide_efficiency->SetBinContent(10, 24, 0.859817);
data_grid_light_guide_efficiency->SetBinContent(11, 1, 1.007241);
data_grid_light_guide_efficiency->SetBinContent(11, 2, 1.083453);
data_grid_light_guide_efficiency->SetBinContent(11, 3, 1.102641);
data_grid_light_guide_efficiency->SetBinContent(11, 4, 1.110749);
data_grid_light_guide_efficiency->SetBinContent(11, 5, 1.113722);
data_grid_light_guide_efficiency->SetBinContent(11, 6, 1.111560);
data_grid_light_guide_efficiency->SetBinContent(11, 7, 1.119938);
data_grid_light_guide_efficiency->SetBinContent(11, 8, 1.123857);
data_grid_light_guide_efficiency->SetBinContent(11, 9, 1.123316);
data_grid_light_guide_efficiency->SetBinContent(11, 10, 1.109263);
data_grid_light_guide_efficiency->SetBinContent(11, 11, 1.088994);
data_grid_light_guide_efficiency->SetBinContent(11, 12, 1.062509);
data_grid_light_guide_efficiency->SetBinContent(11, 13, 1.014133);
data_grid_light_guide_efficiency->SetBinContent(11, 14, 0.979000);
data_grid_light_guide_efficiency->SetBinContent(11, 15, 0.957109);
data_grid_light_guide_efficiency->SetBinContent(11, 16, 0.920084);
data_grid_light_guide_efficiency->SetBinContent(11, 17, 0.888464);
data_grid_light_guide_efficiency->SetBinContent(11, 18, 0.862249);
data_grid_light_guide_efficiency->SetBinContent(11, 19, 0.856574);
data_grid_light_guide_efficiency->SetBinContent(11, 20, 0.851845);
data_grid_light_guide_efficiency->SetBinContent(11, 21, 0.848061);
data_grid_light_guide_efficiency->SetBinContent(11, 22, 0.848061);
data_grid_light_guide_efficiency->SetBinContent(11, 23, 0.855763);
data_grid_light_guide_efficiency->SetBinContent(11, 24, 0.871168);
data_grid_light_guide_efficiency->SetBinContent(12, 1, 1.040212);
data_grid_light_guide_efficiency->SetBinContent(12, 2, 1.135883);
data_grid_light_guide_efficiency->SetBinContent(12, 3, 1.143180);
data_grid_light_guide_efficiency->SetBinContent(12, 4, 1.151287);
data_grid_light_guide_efficiency->SetBinContent(12, 5, 1.156152);
data_grid_light_guide_efficiency->SetBinContent(12, 6, 1.157774);
data_grid_light_guide_efficiency->SetBinContent(12, 7, 1.160206);
data_grid_light_guide_efficiency->SetBinContent(12, 8, 1.162233);
data_grid_light_guide_efficiency->SetBinContent(12, 9, 1.163854);
data_grid_light_guide_efficiency->SetBinContent(12, 10, 1.144396);
data_grid_light_guide_efficiency->SetBinContent(12, 11, 1.120073);
data_grid_light_guide_efficiency->SetBinContent(12, 12, 1.090885);
data_grid_light_guide_efficiency->SetBinContent(12, 13, 1.062509);
data_grid_light_guide_efficiency->SetBinContent(12, 14, 1.030889);
data_grid_light_guide_efficiency->SetBinContent(12, 15, 0.996026);
data_grid_light_guide_efficiency->SetBinContent(12, 16, 0.951434);
data_grid_light_guide_efficiency->SetBinContent(12, 17, 0.916571);
data_grid_light_guide_efficiency->SetBinContent(12, 18, 0.891437);
data_grid_light_guide_efficiency->SetBinContent(12, 19, 0.885762);
data_grid_light_guide_efficiency->SetBinContent(12, 20, 0.879681);
data_grid_light_guide_efficiency->SetBinContent(12, 21, 0.873195);
data_grid_light_guide_efficiency->SetBinContent(12, 22, 0.873195);
data_grid_light_guide_efficiency->SetBinContent(12, 23, 0.880897);
data_grid_light_guide_efficiency->SetBinContent(12, 24, 0.896302);
data_grid_light_guide_efficiency->SetBinContent(13, 1, 1.054806);
data_grid_light_guide_efficiency->SetBinContent(13, 2, 1.137234);
data_grid_light_guide_efficiency->SetBinContent(13, 3, 1.155341);
data_grid_light_guide_efficiency->SetBinContent(13, 4, 1.165611);
data_grid_light_guide_efficiency->SetBinContent(13, 5, 1.169665);
data_grid_light_guide_efficiency->SetBinContent(13, 6, 1.167503);
data_grid_light_guide_efficiency->SetBinContent(13, 7, 1.173719);
data_grid_light_guide_efficiency->SetBinContent(13, 8, 1.175746);
data_grid_light_guide_efficiency->SetBinContent(13, 9, 1.173583);
data_grid_light_guide_efficiency->SetBinContent(13, 10, 1.155206);
data_grid_light_guide_efficiency->SetBinContent(13, 11, 1.134126);
data_grid_light_guide_efficiency->SetBinContent(13, 12, 1.110344);
data_grid_light_guide_efficiency->SetBinContent(13, 13, 1.106290);
data_grid_light_guide_efficiency->SetBinContent(13, 14, 1.064941);
data_grid_light_guide_efficiency->SetBinContent(13, 15, 0.986297);
data_grid_light_guide_efficiency->SetBinContent(13, 16, 0.939002);
data_grid_light_guide_efficiency->SetBinContent(13, 17, 0.905490);
data_grid_light_guide_efficiency->SetBinContent(13, 18, 0.885762);
data_grid_light_guide_efficiency->SetBinContent(13, 19, 0.874681);
data_grid_light_guide_efficiency->SetBinContent(13, 20, 0.867249);
data_grid_light_guide_efficiency->SetBinContent(13, 21, 0.863466);
data_grid_light_guide_efficiency->SetBinContent(13, 22, 0.868871);
data_grid_light_guide_efficiency->SetBinContent(13, 23, 0.879816);
data_grid_light_guide_efficiency->SetBinContent(13, 24, 0.896302);
data_grid_light_guide_efficiency->SetBinContent(14, 1, 1.060211);
data_grid_light_guide_efficiency->SetBinContent(14, 2, 1.132640);
data_grid_light_guide_efficiency->SetBinContent(14, 3, 1.155341);
data_grid_light_guide_efficiency->SetBinContent(14, 4, 1.165611);
data_grid_light_guide_efficiency->SetBinContent(14, 5, 1.171016);
data_grid_light_guide_efficiency->SetBinContent(14, 6, 1.171557);
data_grid_light_guide_efficiency->SetBinContent(14, 7, 1.180205);
data_grid_light_guide_efficiency->SetBinContent(14, 8, 1.182772);
data_grid_light_guide_efficiency->SetBinContent(14, 9, 1.179259);
data_grid_light_guide_efficiency->SetBinContent(14, 10, 1.160881);
data_grid_light_guide_efficiency->SetBinContent(14, 11, 1.140477);
data_grid_light_guide_efficiency->SetBinContent(14, 12, 1.118046);
data_grid_light_guide_efficiency->SetBinContent(14, 13, 1.108047);
data_grid_light_guide_efficiency->SetBinContent(14, 14, 1.062509);
data_grid_light_guide_efficiency->SetBinContent(14, 15, 0.981432);
data_grid_light_guide_efficiency->SetBinContent(14, 16, 0.934137);
data_grid_light_guide_efficiency->SetBinContent(14, 17, 0.899680);
data_grid_light_guide_efficiency->SetBinContent(14, 18, 0.878059);
data_grid_light_guide_efficiency->SetBinContent(14, 19, 0.865628);
data_grid_light_guide_efficiency->SetBinContent(14, 20, 0.859141);
data_grid_light_guide_efficiency->SetBinContent(14, 21, 0.858601);
data_grid_light_guide_efficiency->SetBinContent(14, 22, 0.867249);
data_grid_light_guide_efficiency->SetBinContent(14, 23, 0.880221);
data_grid_light_guide_efficiency->SetBinContent(14, 24, 0.897518);
data_grid_light_guide_efficiency->SetBinContent(15, 1, 1.056428);
data_grid_light_guide_efficiency->SetBinContent(15, 2, 1.122100);
data_grid_light_guide_efficiency->SetBinContent(15, 3, 1.143180);
data_grid_light_guide_efficiency->SetBinContent(15, 4, 1.151287);
data_grid_light_guide_efficiency->SetBinContent(15, 5, 1.160206);
data_grid_light_guide_efficiency->SetBinContent(15, 6, 1.169935);
data_grid_light_guide_efficiency->SetBinContent(15, 7, 1.179664);
data_grid_light_guide_efficiency->SetBinContent(15, 8, 1.183313);
data_grid_light_guide_efficiency->SetBinContent(15, 9, 1.180880);
data_grid_light_guide_efficiency->SetBinContent(15, 10, 1.161422);
data_grid_light_guide_efficiency->SetBinContent(15, 11, 1.139126);
data_grid_light_guide_efficiency->SetBinContent(15, 12, 1.113992);
data_grid_light_guide_efficiency->SetBinContent(15, 13, 1.067779);
data_grid_light_guide_efficiency->SetBinContent(15, 14, 1.023592);
data_grid_light_guide_efficiency->SetBinContent(15, 15, 0.981432);
data_grid_light_guide_efficiency->SetBinContent(15, 16, 0.936840);
data_grid_light_guide_efficiency->SetBinContent(15, 17, 0.899139);
data_grid_light_guide_efficiency->SetBinContent(15, 18, 0.868330);
data_grid_light_guide_efficiency->SetBinContent(15, 19, 0.858601);
data_grid_light_guide_efficiency->SetBinContent(15, 20, 0.855358);
data_grid_light_guide_efficiency->SetBinContent(15, 21, 0.858601);
data_grid_light_guide_efficiency->SetBinContent(15, 22, 0.868330);
data_grid_light_guide_efficiency->SetBinContent(15, 23, 0.882113);
data_grid_light_guide_efficiency->SetBinContent(15, 24, 0.899950);
data_grid_light_guide_efficiency->SetBinContent(16, 1, 1.046158);
data_grid_light_guide_efficiency->SetBinContent(16, 2, 1.107506);
data_grid_light_guide_efficiency->SetBinContent(16, 3, 1.130208);
data_grid_light_guide_efficiency->SetBinContent(16, 4, 1.135072);
data_grid_light_guide_efficiency->SetBinContent(16, 5, 1.143180);
data_grid_light_guide_efficiency->SetBinContent(16, 6, 1.154530);
data_grid_light_guide_efficiency->SetBinContent(16, 7, 1.166422);
data_grid_light_guide_efficiency->SetBinContent(16, 8, 1.171962);
data_grid_light_guide_efficiency->SetBinContent(16, 9, 1.171151);
data_grid_light_guide_efficiency->SetBinContent(16, 10, 1.157098);
data_grid_light_guide_efficiency->SetBinContent(16, 11, 1.132370);
data_grid_light_guide_efficiency->SetBinContent(16, 12, 1.096966);
data_grid_light_guide_efficiency->SetBinContent(16, 13, 1.055617);
data_grid_light_guide_efficiency->SetBinContent(16, 14, 1.013863);
data_grid_light_guide_efficiency->SetBinContent(16, 15, 0.971703);
data_grid_light_guide_efficiency->SetBinContent(16, 16, 0.914138);
data_grid_light_guide_efficiency->SetBinContent(16, 17, 0.872384);
data_grid_light_guide_efficiency->SetBinContent(16, 18, 0.846439);
data_grid_light_guide_efficiency->SetBinContent(16, 19, 0.839413);
data_grid_light_guide_efficiency->SetBinContent(16, 20, 0.836440);
data_grid_light_guide_efficiency->SetBinContent(16, 21, 0.837521);
data_grid_light_guide_efficiency->SetBinContent(16, 22, 0.849953);
data_grid_light_guide_efficiency->SetBinContent(16, 23, 0.864276);
data_grid_light_guide_efficiency->SetBinContent(16, 24, 0.880492);
data_grid_light_guide_efficiency->SetBinContent(17, 1, 1.038186);
data_grid_light_guide_efficiency->SetBinContent(17, 2, 1.083183);
data_grid_light_guide_efficiency->SetBinContent(17, 3, 1.103452);
data_grid_light_guide_efficiency->SetBinContent(17, 4, 1.113992);
data_grid_light_guide_efficiency->SetBinContent(17, 5, 1.126829);
data_grid_light_guide_efficiency->SetBinContent(17, 6, 1.141964);
data_grid_light_guide_efficiency->SetBinContent(17, 7, 1.155747);
data_grid_light_guide_efficiency->SetBinContent(17, 8, 1.163854);
data_grid_light_guide_efficiency->SetBinContent(17, 9, 1.166287);
data_grid_light_guide_efficiency->SetBinContent(17, 10, 1.151693);
data_grid_light_guide_efficiency->SetBinContent(17, 11, 1.123857);
data_grid_light_guide_efficiency->SetBinContent(17, 12, 1.082778);
data_grid_light_guide_efficiency->SetBinContent(17, 13, 1.045753);
data_grid_light_guide_efficiency->SetBinContent(17, 14, 1.005079);
data_grid_light_guide_efficiency->SetBinContent(17, 15, 0.960757);
data_grid_light_guide_efficiency->SetBinContent(17, 16, 0.900761);
data_grid_light_guide_efficiency->SetBinContent(17, 17, 0.856439);
data_grid_light_guide_efficiency->SetBinContent(17, 18, 0.827792);
data_grid_light_guide_efficiency->SetBinContent(17, 19, 0.819684);
data_grid_light_guide_efficiency->SetBinContent(17, 20, 0.816171);
data_grid_light_guide_efficiency->SetBinContent(17, 21, 0.817252);
data_grid_light_guide_efficiency->SetBinContent(17, 22, 0.833737);
data_grid_light_guide_efficiency->SetBinContent(17, 23, 0.846710);
data_grid_light_guide_efficiency->SetBinContent(17, 24, 0.856169);
data_grid_light_guide_efficiency->SetBinContent(18, 1, 1.032510);
data_grid_light_guide_efficiency->SetBinContent(18, 2, 1.049131);
data_grid_light_guide_efficiency->SetBinContent(18, 3, 1.062914);
data_grid_light_guide_efficiency->SetBinContent(18, 4, 1.088048);
data_grid_light_guide_efficiency->SetBinContent(18, 5, 1.111155);
data_grid_light_guide_efficiency->SetBinContent(18, 6, 1.132234);
data_grid_light_guide_efficiency->SetBinContent(18, 7, 1.147639);
data_grid_light_guide_efficiency->SetBinContent(18, 8, 1.158990);
data_grid_light_guide_efficiency->SetBinContent(18, 9, 1.166287);
data_grid_light_guide_efficiency->SetBinContent(18, 10, 1.145207);
data_grid_light_guide_efficiency->SetBinContent(18, 11, 1.113587);
data_grid_light_guide_efficiency->SetBinContent(18, 12, 1.071427);
data_grid_light_guide_efficiency->SetBinContent(18, 13, 1.038186);
data_grid_light_guide_efficiency->SetBinContent(18, 14, 0.997242);
data_grid_light_guide_efficiency->SetBinContent(18, 15, 0.948596);
data_grid_light_guide_efficiency->SetBinContent(18, 16, 0.896707);
data_grid_light_guide_efficiency->SetBinContent(18, 17, 0.851304);
data_grid_light_guide_efficiency->SetBinContent(18, 18, 0.812387);
data_grid_light_guide_efficiency->SetBinContent(18, 19, 0.799415);
data_grid_light_guide_efficiency->SetBinContent(18, 20, 0.794550);
data_grid_light_guide_efficiency->SetBinContent(18, 21, 0.797794);
data_grid_light_guide_efficiency->SetBinContent(18, 22, 0.819684);
data_grid_light_guide_efficiency->SetBinContent(18, 23, 0.829413);
data_grid_light_guide_efficiency->SetBinContent(18, 24, 0.826981);
data_grid_light_guide_efficiency->SetBinContent(19, 1, 0.993864);
data_grid_light_guide_efficiency->SetBinContent(19, 2, 1.033456);
data_grid_light_guide_efficiency->SetBinContent(19, 3, 1.066157);
data_grid_light_guide_efficiency->SetBinContent(19, 4, 1.101560);
data_grid_light_guide_efficiency->SetBinContent(19, 5, 1.126829);
data_grid_light_guide_efficiency->SetBinContent(19, 6, 1.141964);
data_grid_light_guide_efficiency->SetBinContent(19, 7, 1.151963);
data_grid_light_guide_efficiency->SetBinContent(19, 8, 1.160071);
data_grid_light_guide_efficiency->SetBinContent(19, 9, 1.166287);
data_grid_light_guide_efficiency->SetBinContent(19, 10, 1.131153);
data_grid_light_guide_efficiency->SetBinContent(19, 11, 1.100074);
data_grid_light_guide_efficiency->SetBinContent(19, 12, 1.073049);
data_grid_light_guide_efficiency->SetBinContent(19, 13, 1.033861);
data_grid_light_guide_efficiency->SetBinContent(19, 14, 0.989945);
data_grid_light_guide_efficiency->SetBinContent(19, 15, 0.941299);
data_grid_light_guide_efficiency->SetBinContent(19, 16, 0.893194);
data_grid_light_guide_efficiency->SetBinContent(19, 17, 0.850223);
data_grid_light_guide_efficiency->SetBinContent(19, 18, 0.812387);
data_grid_light_guide_efficiency->SetBinContent(19, 19, 0.794010);
data_grid_light_guide_efficiency->SetBinContent(19, 20, 0.780767);
data_grid_light_guide_efficiency->SetBinContent(19, 21, 0.772660);
data_grid_light_guide_efficiency->SetBinContent(19, 22, 0.786443);
data_grid_light_guide_efficiency->SetBinContent(19, 23, 0.805901);
data_grid_light_guide_efficiency->SetBinContent(19, 24, 0.831035);
data_grid_light_guide_efficiency->SetBinContent(20, 1, 0.973865);
data_grid_light_guide_efficiency->SetBinContent(20, 2, 1.028456);
data_grid_light_guide_efficiency->SetBinContent(20, 3, 1.070616);
data_grid_light_guide_efficiency->SetBinContent(20, 4, 1.105479);
data_grid_light_guide_efficiency->SetBinContent(20, 5, 1.128316);
data_grid_light_guide_efficiency->SetBinContent(20, 6, 1.139126);
data_grid_light_guide_efficiency->SetBinContent(20, 7, 1.150206);
data_grid_light_guide_efficiency->SetBinContent(20, 8, 1.156693);
data_grid_light_guide_efficiency->SetBinContent(20, 9, 1.158584);
data_grid_light_guide_efficiency->SetBinContent(20, 10, 1.123992);
data_grid_light_guide_efficiency->SetBinContent(20, 11, 1.095074);
data_grid_light_guide_efficiency->SetBinContent(20, 12, 1.071832);
data_grid_light_guide_efficiency->SetBinContent(20, 13, 1.032916);
data_grid_light_guide_efficiency->SetBinContent(20, 14, 0.989134);
data_grid_light_guide_efficiency->SetBinContent(20, 15, 0.940488);
data_grid_light_guide_efficiency->SetBinContent(20, 16, 0.895626);
data_grid_light_guide_efficiency->SetBinContent(20, 17, 0.854547);
data_grid_light_guide_efficiency->SetBinContent(20, 18, 0.817252);
data_grid_light_guide_efficiency->SetBinContent(20, 19, 0.794821);
data_grid_light_guide_efficiency->SetBinContent(20, 20, 0.776443);
data_grid_light_guide_efficiency->SetBinContent(20, 21, 0.762120);
data_grid_light_guide_efficiency->SetBinContent(20, 22, 0.764822);
data_grid_light_guide_efficiency->SetBinContent(20, 23, 0.782929);
data_grid_light_guide_efficiency->SetBinContent(20, 24, 0.816441);
data_grid_light_guide_efficiency->SetBinContent(21, 1, 0.972514);
data_grid_light_guide_efficiency->SetBinContent(21, 2, 1.034132);
data_grid_light_guide_efficiency->SetBinContent(21, 3, 1.076292);
data_grid_light_guide_efficiency->SetBinContent(21, 4, 1.099804);
data_grid_light_guide_efficiency->SetBinContent(21, 5, 1.115614);
data_grid_light_guide_efficiency->SetBinContent(21, 6, 1.123721);
data_grid_light_guide_efficiency->SetBinContent(21, 7, 1.142369);
data_grid_light_guide_efficiency->SetBinContent(21, 8, 1.148855);
data_grid_light_guide_efficiency->SetBinContent(21, 9, 1.143180);
data_grid_light_guide_efficiency->SetBinContent(21, 10, 1.123721);
data_grid_light_guide_efficiency->SetBinContent(21, 11, 1.098588);
data_grid_light_guide_efficiency->SetBinContent(21, 12, 1.067779);
data_grid_light_guide_efficiency->SetBinContent(21, 13, 1.035348);
data_grid_light_guide_efficiency->SetBinContent(21, 14, 0.994810);
data_grid_light_guide_efficiency->SetBinContent(21, 15, 0.946164);
data_grid_light_guide_efficiency->SetBinContent(21, 16, 0.904004);
data_grid_light_guide_efficiency->SetBinContent(21, 17, 0.864276);
data_grid_light_guide_efficiency->SetBinContent(21, 18, 0.826981);
data_grid_light_guide_efficiency->SetBinContent(21, 19, 0.801847);
data_grid_light_guide_efficiency->SetBinContent(21, 20, 0.781578);
data_grid_light_guide_efficiency->SetBinContent(21, 21, 0.766174);
data_grid_light_guide_efficiency->SetBinContent(21, 22, 0.754823);
data_grid_light_guide_efficiency->SetBinContent(21, 23, 0.760498);
data_grid_light_guide_efficiency->SetBinContent(21, 24, 0.783200);
data_grid_light_guide_efficiency->SetBinContent(22, 1, 0.818468);
data_grid_light_guide_efficiency->SetBinContent(22, 2, 0.859006);
data_grid_light_guide_efficiency->SetBinContent(22, 3, 0.894680);
data_grid_light_guide_efficiency->SetBinContent(22, 4, 0.932246);
data_grid_light_guide_efficiency->SetBinContent(22, 5, 0.958866);
data_grid_light_guide_efficiency->SetBinContent(22, 6, 0.974540);
data_grid_light_guide_efficiency->SetBinContent(22, 7, 0.988323);
data_grid_light_guide_efficiency->SetBinContent(22, 8, 0.995080);
data_grid_light_guide_efficiency->SetBinContent(22, 9, 0.994810);
data_grid_light_guide_efficiency->SetBinContent(22, 10, 0.976432);
data_grid_light_guide_efficiency->SetBinContent(22, 11, 0.951299);
data_grid_light_guide_efficiency->SetBinContent(22, 12, 0.919408);
data_grid_light_guide_efficiency->SetBinContent(22, 13, 0.889140);
data_grid_light_guide_efficiency->SetBinContent(22, 14, 0.858060);
data_grid_light_guide_efficiency->SetBinContent(22, 15, 0.826170);
data_grid_light_guide_efficiency->SetBinContent(22, 16, 0.786173);
data_grid_light_guide_efficiency->SetBinContent(22, 17, 0.751039);
data_grid_light_guide_efficiency->SetBinContent(22, 18, 0.720771);
data_grid_light_guide_efficiency->SetBinContent(22, 19, 0.716176);
data_grid_light_guide_efficiency->SetBinContent(22, 20, 0.700502);
data_grid_light_guide_efficiency->SetBinContent(22, 21, 0.673746);
data_grid_light_guide_efficiency->SetBinContent(22, 22, 0.672125);
data_grid_light_guide_efficiency->SetBinContent(22, 23, 0.684286);
data_grid_light_guide_efficiency->SetBinContent(22, 24, 0.710231);

  return data_grid_light_guide_efficiency;
}

TH2 *
LoadMikePhippsLightGuideEff()
{
  TFile * fsrc = new TFile("contaminationFourNeighbor.root");
  assert(fsrc->IsOpen());

  TH2F * contamHisto = fsrc->GetObjectChecked("contamHisto", "TH2F");
  const int n = contamHisto->GetXaxis()->GetNbins() - 1; // remove one empty bins

  TH2F * data_grid_light_guide_efficiency =
      new TH2F("data_grid_light_guide_efficiency",
          "Extracted light collection efficiency from Mike Phipps;x positio fraction;y position fraction", //
          n, 0., 1., n, 0., 1.);
  TH1F * stat_data_grid_light_guide_efficiency = new TH1F(
      "stat_data_grid_light_guide_efficiency",
      "Efficiency statistics;Reletive Efficiency;Count of bins", 100, 0, 0.33);
  TH1F * stat_data_grid_light_guide_efficiency_normalized = new TH1F(
      "stat_data_grid_light_guide_efficiency_normalized",
      "Efficiency statistics;Reletive Efficiency;Count of bins", 100, 0, 2);

  for (int x = 1; x <= n; x++)
    {
      for (int y = 1; y <= n; y++)
        {
          data_grid_light_guide_efficiency->SetBinContent(x, y,
              contamHisto->GetBinContent(x + 1, y + 1));
        }
    }

  data_grid_light_guide_efficiency->Smooth(1, "k5b");
  data_grid_light_guide_efficiency->Rebin2D(2, 2);
  data_grid_light_guide_efficiency->Smooth(1, "k5a");

  for (int x = 1; x <= data_grid_light_guide_efficiency->GetNbinsX(); x++)
    {
      for (int y = 1; y <= data_grid_light_guide_efficiency->GetNbinsY(); y++)
        {
          double eff = data_grid_light_guide_efficiency->GetBinContent(x, y);

          //Fix the large variation relative to newer version concluded by Mike:
          //          Currrent sPHENIX LG
          //          Uniformity RMS:
          //          0.011 (% efficiency)
          //          Total Efficiency: 13.5%
          //          Surface area covered by sipm: 18.4 %
          eff = (eff + 0.0119 * 3) * 3;

          stat_data_grid_light_guide_efficiency->Fill(eff);
          data_grid_light_guide_efficiency->SetBinContent(x, y, eff);
        }
    }

  data_grid_light_guide_efficiency->Scale(
      1. / stat_data_grid_light_guide_efficiency->GetMean());

  for (int x = 1; x <= data_grid_light_guide_efficiency->GetNbinsX(); x++)
    {
      for (int y = 1; y <= data_grid_light_guide_efficiency->GetNbinsY(); y++)
        {
          double eff = data_grid_light_guide_efficiency->GetBinContent(x, y);
          stat_data_grid_light_guide_efficiency_normalized->Fill(eff);
        }
    }

  TCanvas * c2 = new TCanvas("LoadMikePhippsLightGuideEff",
      "LoadMikePhippsLightGuideEff", 1800, 600);
  c2->Divide(3, 1);

  c2->cd(1);
  contamHisto->Draw("colz");

  c2->cd(2);
  data_grid_light_guide_efficiency->Draw("colz");

  c2->cd(3);
  stat_data_grid_light_guide_efficiency_normalized->Draw();

  return data_grid_light_guide_efficiency;
}

TH2 *
LoadMikePhippsLightGuideEffUpdated()
{
  TFile * fsrc = new TFile("unifHistoDet1.root");
  assert(fsrc->IsOpen());

  TH2F * contamHisto = fsrc->Get("unifHisto;4096");
  assert(contamHisto);
  const int n = contamHisto->GetXaxis()->GetNbins();

  TH2F * data_grid_light_guide_efficiency =
      new TH2F("data_grid_light_guide_efficiency",
          "Extracted light collection efficiency from Mike Phipps;x positio fraction;y position fraction", //
          n, 0., 1., n, 0., 1.);
  TH1F * stat_data_grid_light_guide_efficiency = new TH1F(
      "stat_data_grid_light_guide_efficiency",
      "Efficiency statistics;Reletive Efficiency;Count of bins", 100, 0, 1);
  TH1F * stat_data_grid_light_guide_efficiency_normalized = new TH1F(
      "stat_data_grid_light_guide_efficiency_normalized",
      "Efficiency statistics;Reletive Efficiency;Count of bins", 100, 0, 2);

  for (int x = 1; x <= n; x++)
    {
      for (int y = 1; y <= n; y++)
        {
          data_grid_light_guide_efficiency->SetBinContent(x, y,
              contamHisto->GetBinContent(x, y));
        }
    }

  data_grid_light_guide_efficiency->Smooth(1, "k5b");
  data_grid_light_guide_efficiency->Rebin2D(2, 2);
  data_grid_light_guide_efficiency->Smooth(1, "k5a");

  for (int x = 1; x <= data_grid_light_guide_efficiency->GetNbinsX(); x++)
    {
      for (int y = 1; y <= data_grid_light_guide_efficiency->GetNbinsY(); y++)
        {
          double eff = data_grid_light_guide_efficiency->GetBinContent(x, y);

          stat_data_grid_light_guide_efficiency->Fill(eff);
          data_grid_light_guide_efficiency->SetBinContent(x, y, eff);
        }
    }

  data_grid_light_guide_efficiency->Scale(
      1. / stat_data_grid_light_guide_efficiency->GetMean());

  for (int x = 1; x <= data_grid_light_guide_efficiency->GetNbinsX(); x++)
    {
      for (int y = 1; y <= data_grid_light_guide_efficiency->GetNbinsY(); y++)
        {
          double eff = data_grid_light_guide_efficiency->GetBinContent(x, y);
          stat_data_grid_light_guide_efficiency_normalized->Fill(eff);
        }
    }

  TCanvas * c2 = new TCanvas("LoadMikePhippsLightGuideEffUpdated",
      "LoadMikePhippsLightGuideEffUpdated", 1800, 600);
  c2->Divide(3, 1);

  c2->cd(1);
  contamHisto->Draw("colz");

  c2->cd(2);
  data_grid_light_guide_efficiency->Draw("colz");

  c2->cd(3);
//  stat_data_grid_light_guide_efficiency->Draw();
  stat_data_grid_light_guide_efficiency_normalized->Draw();

  return data_grid_light_guide_efficiency;
}

TH1 *
LoadSCSF78Fiber()
{
  const double fiber_length = 13.5; // cm
  const double attenuation_length = 105; // cm, from Sean Stoll
  const double back_reflection = 0.3; // Rough estimation from Sean Stoll
  const int N = 100;

  TH1 * data_grid_fiber_trans =
      new TH1F("data_grid_fiber_trans",
          "SCSF-78 Fiber Transmission VS hit position in fiber;position in fiber (cm);Effective transmission",
          N, -fiber_length / 2, fiber_length / 2);

  for (int i = 1; i <= N; i++)
    {
      const double z = data_grid_fiber_trans->GetXaxis()->GetBinCenter(i);

      const double eff = (TMath::Exp(
          -(z + fiber_length / 2) / attenuation_length)
          + back_reflection
              * TMath::Exp(
                  -(fiber_length + fiber_length / 2 - z) / attenuation_length))
          / (1
              + back_reflection
                  * TMath::Exp(-fiber_length * 2 / attenuation_length));
      data_grid_fiber_trans->SetBinContent(i, eff);

//      cout << "LoadSCSF78Fiber - bin " << i << " @ z = " << z << " : eff = "
//          << eff << endl;
    }

  const double mean = data_grid_fiber_trans->GetSumOfWeights() / N;
  data_grid_fiber_trans->Scale(1. / mean);

  new TCanvas("LoadSCSF78Fiber", "LoadSCSF78Fiber");
  data_grid_fiber_trans->Draw();

  return data_grid_fiber_trans;
}
