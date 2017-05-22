// $Id: $

/*!
 * \file LightCollection.C
 * \brief Construct the light collection data file for PHG4FullProjSpacalCellReco::LightCollectionModel
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <TMath.h>

#include <cassert>
#include <iostream>

using namespace std;

void LightCollection()
{
  //  gStyle->SetOptStat(0);

  //  TFile* fdata = new TXMLFile("../LightCollection/Prototype2Module.xml",
  //                              "recreate");

  TFile* fdata = new TXMLFile("../LightCollection/Prototype3Module.xml",
                              "recreate");

  fdata->ls();

  //  TH2 * h2 = LoadMikePhippsLightGuideEff();
  //  TH2 * h2 = LoadMikePhippsLightGuideEffUpdated();
  //  TH2* h2 = LoadSeanStollLightGuideEff();
  TH2* h2 = LoadSeanStoll1InchMachinedLightGuideEff();

  h2->SetDirectory(NULL);
  fdata->cd();
  h2->Write();

  TH1* h1 = LoadSCSF78Fiber();

  h1->SetDirectory(NULL);
  fdata->cd();
  h1->Write();

  fdata->ls();
}

TH2* LoadSeanStollLightGuideEff()
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

  TH2F* data_grid_light_guide_efficiency =
      new TH2F("data_grid_light_guide_efficiency",
               "Extracted light collection efficiency from Sean Stoll;x position fraction;y position fraction",  //
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

TH2* LoadSeanStoll1InchMachinedLightGuideEff()
{
  //  From: Sean Stoll [mailto:stoll@bnl.gov]
  //  Sent: Tuesday, May 2, 2017 1:00 PM
  //  To: Huang, Jin <jhuang@bnl.gov>
  //  Subject: light guide uniformity map data
  //
  //  Jin,
  //
  //    I have been trying to get you a perfectly symmetrical scan of a 1"
  //  machined trapezoidal light guide.
  //
  //  Thus far, they all have some small asymmetry.  See attached Excel file.
  //
  //
  //  Sean

  TH2F* data_grid_light_guide_efficiency =
      new TH2F("data_grid_light_guide_efficiency",
               "Extracted light collection efficiency from Sean Stoll;x position fraction;y position fraction",  //
               24, 0., 1., 24, 0., 1.);

  data_grid_light_guide_efficiency->SetBinContent(1, 1, 0.790768);
  data_grid_light_guide_efficiency->SetBinContent(1, 2, 0.821671);
  data_grid_light_guide_efficiency->SetBinContent(1, 3, 0.849902);
  data_grid_light_guide_efficiency->SetBinContent(1, 4, 0.886091);
  data_grid_light_guide_efficiency->SetBinContent(1, 5, 0.914561);
  data_grid_light_guide_efficiency->SetBinContent(1, 6, 0.932864);
  data_grid_light_guide_efficiency->SetBinContent(1, 7, 0.946284);
  data_grid_light_guide_efficiency->SetBinContent(1, 8, 0.961116);
  data_grid_light_guide_efficiency->SetBinContent(1, 9, 0.973408);
  data_grid_light_guide_efficiency->SetBinContent(1, 10, 0.980160);
  data_grid_light_guide_efficiency->SetBinContent(1, 11, 0.983122);
  data_grid_light_guide_efficiency->SetBinContent(1, 12, 0.986777);
  data_grid_light_guide_efficiency->SetBinContent(1, 13, 0.987070);
  data_grid_light_guide_efficiency->SetBinContent(1, 14, 0.985100);
  data_grid_light_guide_efficiency->SetBinContent(1, 15, 0.981667);
  data_grid_light_guide_efficiency->SetBinContent(1, 16, 0.973526);
  data_grid_light_guide_efficiency->SetBinContent(1, 17, 0.964364);
  data_grid_light_guide_efficiency->SetBinContent(1, 18, 0.952229);
  data_grid_light_guide_efficiency->SetBinContent(1, 19, 0.944970);
  data_grid_light_guide_efficiency->SetBinContent(1, 20, 0.930873);
  data_grid_light_guide_efficiency->SetBinContent(1, 21, 0.907044);
  data_grid_light_guide_efficiency->SetBinContent(1, 22, 0.881434);
  data_grid_light_guide_efficiency->SetBinContent(1, 23, 0.839935);
  data_grid_light_guide_efficiency->SetBinContent(1, 24, 0.816008);
  data_grid_light_guide_efficiency->SetBinContent(2, 1, 0.806983);
  data_grid_light_guide_efficiency->SetBinContent(2, 2, 0.834132);
  data_grid_light_guide_efficiency->SetBinContent(2, 3, 0.869299);
  data_grid_light_guide_efficiency->SetBinContent(2, 4, 0.897915);
  data_grid_light_guide_efficiency->SetBinContent(2, 5, 0.926631);
  data_grid_light_guide_efficiency->SetBinContent(2, 6, 0.951017);
  data_grid_light_guide_efficiency->SetBinContent(2, 7, 0.958715);
  data_grid_light_guide_efficiency->SetBinContent(2, 8, 0.967339);
  data_grid_light_guide_efficiency->SetBinContent(2, 9, 0.980725);
  data_grid_light_guide_efficiency->SetBinContent(2, 10, 0.989807);
  data_grid_light_guide_efficiency->SetBinContent(2, 11, 0.994055);
  data_grid_light_guide_efficiency->SetBinContent(2, 12, 0.995627);
  data_grid_light_guide_efficiency->SetBinContent(2, 13, 0.996140);
  data_grid_light_guide_efficiency->SetBinContent(2, 14, 0.994611);
  data_grid_light_guide_efficiency->SetBinContent(2, 15, 0.986562);
  data_grid_light_guide_efficiency->SetBinContent(2, 16, 0.977253);
  data_grid_light_guide_efficiency->SetBinContent(2, 17, 0.974414);
  data_grid_light_guide_efficiency->SetBinContent(2, 18, 0.970805);
  data_grid_light_guide_efficiency->SetBinContent(2, 19, 0.959860);
  data_grid_light_guide_efficiency->SetBinContent(2, 20, 0.950595);
  data_grid_light_guide_efficiency->SetBinContent(2, 21, 0.920173);
  data_grid_light_guide_efficiency->SetBinContent(2, 22, 0.888319);
  data_grid_light_guide_efficiency->SetBinContent(2, 23, 0.860983);
  data_grid_light_guide_efficiency->SetBinContent(2, 24, 0.829740);
  data_grid_light_guide_efficiency->SetBinContent(3, 1, 0.838195);
  data_grid_light_guide_efficiency->SetBinContent(3, 2, 0.858899);
  data_grid_light_guide_efficiency->SetBinContent(3, 3, 0.887907);
  data_grid_light_guide_efficiency->SetBinContent(3, 4, 0.917567);
  data_grid_light_guide_efficiency->SetBinContent(3, 5, 0.946765);
  data_grid_light_guide_efficiency->SetBinContent(3, 6, 0.967655);
  data_grid_light_guide_efficiency->SetBinContent(3, 7, 0.976344);
  data_grid_light_guide_efficiency->SetBinContent(3, 8, 0.980392);
  data_grid_light_guide_efficiency->SetBinContent(3, 9, 0.989033);
  data_grid_light_guide_efficiency->SetBinContent(3, 10, 0.997773);
  data_grid_light_guide_efficiency->SetBinContent(3, 11, 1.002259);
  data_grid_light_guide_efficiency->SetBinContent(3, 12, 1.005109);
  data_grid_light_guide_efficiency->SetBinContent(3, 13, 1.003968);
  data_grid_light_guide_efficiency->SetBinContent(3, 14, 1.002171);
  data_grid_light_guide_efficiency->SetBinContent(3, 15, 0.997793);
  data_grid_light_guide_efficiency->SetBinContent(3, 16, 0.989217);
  data_grid_light_guide_efficiency->SetBinContent(3, 17, 0.986725);
  data_grid_light_guide_efficiency->SetBinContent(3, 18, 0.983732);
  data_grid_light_guide_efficiency->SetBinContent(3, 19, 0.978275);
  data_grid_light_guide_efficiency->SetBinContent(3, 20, 0.963986);
  data_grid_light_guide_efficiency->SetBinContent(3, 21, 0.935209);
  data_grid_light_guide_efficiency->SetBinContent(3, 22, 0.914160);
  data_grid_light_guide_efficiency->SetBinContent(3, 23, 0.881257);
  data_grid_light_guide_efficiency->SetBinContent(3, 24, 0.849800);
  data_grid_light_guide_efficiency->SetBinContent(4, 1, 0.868599);
  data_grid_light_guide_efficiency->SetBinContent(4, 2, 0.886254);
  data_grid_light_guide_efficiency->SetBinContent(4, 3, 0.910378);
  data_grid_light_guide_efficiency->SetBinContent(4, 4, 0.939688);
  data_grid_light_guide_efficiency->SetBinContent(4, 5, 0.968196);
  data_grid_light_guide_efficiency->SetBinContent(4, 6, 0.985772);
  data_grid_light_guide_efficiency->SetBinContent(4, 7, 0.994952);
  data_grid_light_guide_efficiency->SetBinContent(4, 8, 0.997025);
  data_grid_light_guide_efficiency->SetBinContent(4, 9, 1.003911);
  data_grid_light_guide_efficiency->SetBinContent(4, 10, 1.010044);
  data_grid_light_guide_efficiency->SetBinContent(4, 11, 1.012549);
  data_grid_light_guide_efficiency->SetBinContent(4, 12, 1.014373);
  data_grid_light_guide_efficiency->SetBinContent(4, 13, 1.015031);
  data_grid_light_guide_efficiency->SetBinContent(4, 14, 1.017748);
  data_grid_light_guide_efficiency->SetBinContent(4, 15, 1.016088);
  data_grid_light_guide_efficiency->SetBinContent(4, 16, 1.008112);
  data_grid_light_guide_efficiency->SetBinContent(4, 17, 1.004351);
  data_grid_light_guide_efficiency->SetBinContent(4, 18, 1.001089);
  data_grid_light_guide_efficiency->SetBinContent(4, 19, 0.993722);
  data_grid_light_guide_efficiency->SetBinContent(4, 20, 0.977941);
  data_grid_light_guide_efficiency->SetBinContent(4, 21, 0.958333);
  data_grid_light_guide_efficiency->SetBinContent(4, 22, 0.939970);
  data_grid_light_guide_efficiency->SetBinContent(4, 23, 0.904151);
  data_grid_light_guide_efficiency->SetBinContent(4, 24, 0.881684);
  data_grid_light_guide_efficiency->SetBinContent(5, 1, 0.885466);
  data_grid_light_guide_efficiency->SetBinContent(5, 2, 0.904939);
  data_grid_light_guide_efficiency->SetBinContent(5, 3, 0.930993);
  data_grid_light_guide_efficiency->SetBinContent(5, 4, 0.956727);
  data_grid_light_guide_efficiency->SetBinContent(5, 5, 0.983056);
  data_grid_light_guide_efficiency->SetBinContent(5, 6, 0.998277);
  data_grid_light_guide_efficiency->SetBinContent(5, 7, 1.005909);
  data_grid_light_guide_efficiency->SetBinContent(5, 8, 1.008336);
  data_grid_light_guide_efficiency->SetBinContent(5, 9, 1.014598);
  data_grid_light_guide_efficiency->SetBinContent(5, 10, 1.019690);
  data_grid_light_guide_efficiency->SetBinContent(5, 11, 1.022941);
  data_grid_light_guide_efficiency->SetBinContent(5, 12, 1.024457);
  data_grid_light_guide_efficiency->SetBinContent(5, 13, 1.024253);
  data_grid_light_guide_efficiency->SetBinContent(5, 14, 1.027170);
  data_grid_light_guide_efficiency->SetBinContent(5, 15, 1.027352);
  data_grid_light_guide_efficiency->SetBinContent(5, 16, 1.019871);
  data_grid_light_guide_efficiency->SetBinContent(5, 17, 1.015747);
  data_grid_light_guide_efficiency->SetBinContent(5, 18, 1.014730);
  data_grid_light_guide_efficiency->SetBinContent(5, 19, 1.005288);
  data_grid_light_guide_efficiency->SetBinContent(5, 20, 0.993640);
  data_grid_light_guide_efficiency->SetBinContent(5, 21, 0.978207);
  data_grid_light_guide_efficiency->SetBinContent(5, 22, 0.956179);
  data_grid_light_guide_efficiency->SetBinContent(5, 23, 0.922712);
  data_grid_light_guide_efficiency->SetBinContent(5, 24, 0.909399);
  data_grid_light_guide_efficiency->SetBinContent(6, 1, 0.891773);
  data_grid_light_guide_efficiency->SetBinContent(6, 2, 0.920505);
  data_grid_light_guide_efficiency->SetBinContent(6, 3, 0.953056);
  data_grid_light_guide_efficiency->SetBinContent(6, 4, 0.976009);
  data_grid_light_guide_efficiency->SetBinContent(6, 5, 0.997705);
  data_grid_light_guide_efficiency->SetBinContent(6, 6, 1.007432);
  data_grid_light_guide_efficiency->SetBinContent(6, 7, 1.012030);
  data_grid_light_guide_efficiency->SetBinContent(6, 8, 1.016673);
  data_grid_light_guide_efficiency->SetBinContent(6, 9, 1.020613);
  data_grid_light_guide_efficiency->SetBinContent(6, 10, 1.026960);
  data_grid_light_guide_efficiency->SetBinContent(6, 11, 1.032115);
  data_grid_light_guide_efficiency->SetBinContent(6, 12, 1.036521);
  data_grid_light_guide_efficiency->SetBinContent(6, 13, 1.032614);
  data_grid_light_guide_efficiency->SetBinContent(6, 14, 1.028127);
  data_grid_light_guide_efficiency->SetBinContent(6, 15, 1.031762);
  data_grid_light_guide_efficiency->SetBinContent(6, 16, 1.026670);
  data_grid_light_guide_efficiency->SetBinContent(6, 17, 1.020124);
  data_grid_light_guide_efficiency->SetBinContent(6, 18, 1.020925);
  data_grid_light_guide_efficiency->SetBinContent(6, 19, 1.016306);
  data_grid_light_guide_efficiency->SetBinContent(6, 20, 1.010608);
  data_grid_light_guide_efficiency->SetBinContent(6, 21, 0.992892);
  data_grid_light_guide_efficiency->SetBinContent(6, 22, 0.970460);
  data_grid_light_guide_efficiency->SetBinContent(6, 23, 0.936174);
  data_grid_light_guide_efficiency->SetBinContent(6, 24, 0.919919);
  data_grid_light_guide_efficiency->SetBinContent(7, 1, 0.890416);
  data_grid_light_guide_efficiency->SetBinContent(7, 2, 0.935800);
  data_grid_light_guide_efficiency->SetBinContent(7, 3, 0.969278);
  data_grid_light_guide_efficiency->SetBinContent(7, 4, 0.994161);
  data_grid_light_guide_efficiency->SetBinContent(7, 5, 1.005238);
  data_grid_light_guide_efficiency->SetBinContent(7, 6, 1.015489);
  data_grid_light_guide_efficiency->SetBinContent(7, 7, 1.016175);
  data_grid_light_guide_efficiency->SetBinContent(7, 8, 1.019010);
  data_grid_light_guide_efficiency->SetBinContent(7, 9, 1.027517);
  data_grid_light_guide_efficiency->SetBinContent(7, 10, 1.031740);
  data_grid_light_guide_efficiency->SetBinContent(7, 11, 1.032768);
  data_grid_light_guide_efficiency->SetBinContent(7, 12, 1.035071);
  data_grid_light_guide_efficiency->SetBinContent(7, 13, 1.038416);
  data_grid_light_guide_efficiency->SetBinContent(7, 14, 1.036218);
  data_grid_light_guide_efficiency->SetBinContent(7, 15, 1.033919);
  data_grid_light_guide_efficiency->SetBinContent(7, 16, 1.034942);
  data_grid_light_guide_efficiency->SetBinContent(7, 17, 1.030875);
  data_grid_light_guide_efficiency->SetBinContent(7, 18, 1.031964);
  data_grid_light_guide_efficiency->SetBinContent(7, 19, 1.024361);
  data_grid_light_guide_efficiency->SetBinContent(7, 20, 1.018443);
  data_grid_light_guide_efficiency->SetBinContent(7, 21, 1.013435);
  data_grid_light_guide_efficiency->SetBinContent(7, 22, 0.982159);
  data_grid_light_guide_efficiency->SetBinContent(7, 23, 0.952128);
  data_grid_light_guide_efficiency->SetBinContent(7, 24, 0.936445);
  data_grid_light_guide_efficiency->SetBinContent(8, 1, 0.904081);
  data_grid_light_guide_efficiency->SetBinContent(8, 2, 0.946157);
  data_grid_light_guide_efficiency->SetBinContent(8, 3, 0.972494);
  data_grid_light_guide_efficiency->SetBinContent(8, 4, 0.997122);
  data_grid_light_guide_efficiency->SetBinContent(8, 5, 1.009838);
  data_grid_light_guide_efficiency->SetBinContent(8, 6, 1.013840);
  data_grid_light_guide_efficiency->SetBinContent(8, 7, 1.023668);
  data_grid_light_guide_efficiency->SetBinContent(8, 8, 1.024541);
  data_grid_light_guide_efficiency->SetBinContent(8, 9, 1.031063);
  data_grid_light_guide_efficiency->SetBinContent(8, 10, 1.031631);
  data_grid_light_guide_efficiency->SetBinContent(8, 11, 1.033656);
  data_grid_light_guide_efficiency->SetBinContent(8, 12, 1.037851);
  data_grid_light_guide_efficiency->SetBinContent(8, 13, 1.040074);
  data_grid_light_guide_efficiency->SetBinContent(8, 14, 1.040130);
  data_grid_light_guide_efficiency->SetBinContent(8, 15, 1.039815);
  data_grid_light_guide_efficiency->SetBinContent(8, 16, 1.041162);
  data_grid_light_guide_efficiency->SetBinContent(8, 17, 1.036804);
  data_grid_light_guide_efficiency->SetBinContent(8, 18, 1.034528);
  data_grid_light_guide_efficiency->SetBinContent(8, 19, 1.030921);
  data_grid_light_guide_efficiency->SetBinContent(8, 20, 1.023848);
  data_grid_light_guide_efficiency->SetBinContent(8, 21, 1.010963);
  data_grid_light_guide_efficiency->SetBinContent(8, 22, 0.991011);
  data_grid_light_guide_efficiency->SetBinContent(8, 23, 0.963552);
  data_grid_light_guide_efficiency->SetBinContent(8, 24, 0.955503);
  data_grid_light_guide_efficiency->SetBinContent(9, 1, 0.919402);
  data_grid_light_guide_efficiency->SetBinContent(9, 2, 0.950148);
  data_grid_light_guide_efficiency->SetBinContent(9, 3, 0.978893);
  data_grid_light_guide_efficiency->SetBinContent(9, 4, 0.997890);
  data_grid_light_guide_efficiency->SetBinContent(9, 5, 1.013263);
  data_grid_light_guide_efficiency->SetBinContent(9, 6, 1.018361);
  data_grid_light_guide_efficiency->SetBinContent(9, 7, 1.028210);
  data_grid_light_guide_efficiency->SetBinContent(9, 8, 1.030782);
  data_grid_light_guide_efficiency->SetBinContent(9, 9, 1.033979);
  data_grid_light_guide_efficiency->SetBinContent(9, 10, 1.032989);
  data_grid_light_guide_efficiency->SetBinContent(9, 11, 1.032323);
  data_grid_light_guide_efficiency->SetBinContent(9, 12, 1.037742);
  data_grid_light_guide_efficiency->SetBinContent(9, 13, 1.038539);
  data_grid_light_guide_efficiency->SetBinContent(9, 14, 1.037188);
  data_grid_light_guide_efficiency->SetBinContent(9, 15, 1.037596);
  data_grid_light_guide_efficiency->SetBinContent(9, 16, 1.038421);
  data_grid_light_guide_efficiency->SetBinContent(9, 17, 1.039222);
  data_grid_light_guide_efficiency->SetBinContent(9, 18, 1.034783);
  data_grid_light_guide_efficiency->SetBinContent(9, 19, 1.030834);
  data_grid_light_guide_efficiency->SetBinContent(9, 20, 1.024968);
  data_grid_light_guide_efficiency->SetBinContent(9, 21, 1.008002);
  data_grid_light_guide_efficiency->SetBinContent(9, 22, 0.990724);
  data_grid_light_guide_efficiency->SetBinContent(9, 23, 0.969309);
  data_grid_light_guide_efficiency->SetBinContent(9, 24, 0.965490);
  data_grid_light_guide_efficiency->SetBinContent(10, 1, 0.930601);
  data_grid_light_guide_efficiency->SetBinContent(10, 2, 0.956367);
  data_grid_light_guide_efficiency->SetBinContent(10, 3, 0.983437);
  data_grid_light_guide_efficiency->SetBinContent(10, 4, 1.001459);
  data_grid_light_guide_efficiency->SetBinContent(10, 5, 1.016421);
  data_grid_light_guide_efficiency->SetBinContent(10, 6, 1.027045);
  data_grid_light_guide_efficiency->SetBinContent(10, 7, 1.031408);
  data_grid_light_guide_efficiency->SetBinContent(10, 8, 1.031400);
  data_grid_light_guide_efficiency->SetBinContent(10, 9, 1.033326);
  data_grid_light_guide_efficiency->SetBinContent(10, 10, 1.031473);
  data_grid_light_guide_efficiency->SetBinContent(10, 11, 1.029721);
  data_grid_light_guide_efficiency->SetBinContent(10, 12, 1.036930);
  data_grid_light_guide_efficiency->SetBinContent(10, 13, 1.038887);
  data_grid_light_guide_efficiency->SetBinContent(10, 14, 1.036758);
  data_grid_light_guide_efficiency->SetBinContent(10, 15, 1.038165);
  data_grid_light_guide_efficiency->SetBinContent(10, 16, 1.040060);
  data_grid_light_guide_efficiency->SetBinContent(10, 17, 1.041918);
  data_grid_light_guide_efficiency->SetBinContent(10, 18, 1.036125);
  data_grid_light_guide_efficiency->SetBinContent(10, 19, 1.032828);
  data_grid_light_guide_efficiency->SetBinContent(10, 20, 1.023926);
  data_grid_light_guide_efficiency->SetBinContent(10, 21, 1.008832);
  data_grid_light_guide_efficiency->SetBinContent(10, 22, 0.992905);
  data_grid_light_guide_efficiency->SetBinContent(10, 23, 0.973232);
  data_grid_light_guide_efficiency->SetBinContent(10, 24, 0.970925);
  data_grid_light_guide_efficiency->SetBinContent(11, 1, 0.938301);
  data_grid_light_guide_efficiency->SetBinContent(11, 2, 0.963659);
  data_grid_light_guide_efficiency->SetBinContent(11, 3, 0.984584);
  data_grid_light_guide_efficiency->SetBinContent(11, 4, 1.007861);
  data_grid_light_guide_efficiency->SetBinContent(11, 5, 1.020731);
  data_grid_light_guide_efficiency->SetBinContent(11, 6, 1.033301);
  data_grid_light_guide_efficiency->SetBinContent(11, 7, 1.032443);
  data_grid_light_guide_efficiency->SetBinContent(11, 8, 1.032515);
  data_grid_light_guide_efficiency->SetBinContent(11, 9, 1.034012);
  data_grid_light_guide_efficiency->SetBinContent(11, 10, 1.031799);
  data_grid_light_guide_efficiency->SetBinContent(11, 11, 1.030685);
  data_grid_light_guide_efficiency->SetBinContent(11, 12, 1.037225);
  data_grid_light_guide_efficiency->SetBinContent(11, 13, 1.041153);
  data_grid_light_guide_efficiency->SetBinContent(11, 14, 1.041399);
  data_grid_light_guide_efficiency->SetBinContent(11, 15, 1.043990);
  data_grid_light_guide_efficiency->SetBinContent(11, 16, 1.045153);
  data_grid_light_guide_efficiency->SetBinContent(11, 17, 1.045581);
  data_grid_light_guide_efficiency->SetBinContent(11, 18, 1.040102);
  data_grid_light_guide_efficiency->SetBinContent(11, 19, 1.037664);
  data_grid_light_guide_efficiency->SetBinContent(11, 20, 1.027165);
  data_grid_light_guide_efficiency->SetBinContent(11, 21, 1.011935);
  data_grid_light_guide_efficiency->SetBinContent(11, 22, 1.001981);
  data_grid_light_guide_efficiency->SetBinContent(11, 23, 0.980331);
  data_grid_light_guide_efficiency->SetBinContent(11, 24, 0.974211);
  data_grid_light_guide_efficiency->SetBinContent(12, 1, 0.946257);
  data_grid_light_guide_efficiency->SetBinContent(12, 2, 0.966881);
  data_grid_light_guide_efficiency->SetBinContent(12, 3, 0.988287);
  data_grid_light_guide_efficiency->SetBinContent(12, 4, 1.016172);
  data_grid_light_guide_efficiency->SetBinContent(12, 5, 1.027503);
  data_grid_light_guide_efficiency->SetBinContent(12, 6, 1.034263);
  data_grid_light_guide_efficiency->SetBinContent(12, 7, 1.031917);
  data_grid_light_guide_efficiency->SetBinContent(12, 8, 1.038595);
  data_grid_light_guide_efficiency->SetBinContent(12, 9, 1.038182);
  data_grid_light_guide_efficiency->SetBinContent(12, 10, 1.039168);
  data_grid_light_guide_efficiency->SetBinContent(12, 11, 1.038220);
  data_grid_light_guide_efficiency->SetBinContent(12, 12, 1.038176);
  data_grid_light_guide_efficiency->SetBinContent(12, 13, 1.042225);
  data_grid_light_guide_efficiency->SetBinContent(12, 14, 1.044688);
  data_grid_light_guide_efficiency->SetBinContent(12, 15, 1.045415);
  data_grid_light_guide_efficiency->SetBinContent(12, 16, 1.045012);
  data_grid_light_guide_efficiency->SetBinContent(12, 17, 1.049639);
  data_grid_light_guide_efficiency->SetBinContent(12, 18, 1.048035);
  data_grid_light_guide_efficiency->SetBinContent(12, 19, 1.037478);
  data_grid_light_guide_efficiency->SetBinContent(12, 20, 1.038246);
  data_grid_light_guide_efficiency->SetBinContent(12, 21, 1.013786);
  data_grid_light_guide_efficiency->SetBinContent(12, 22, 1.010295);
  data_grid_light_guide_efficiency->SetBinContent(12, 23, 0.988901);
  data_grid_light_guide_efficiency->SetBinContent(12, 24, 0.972993);
  data_grid_light_guide_efficiency->SetBinContent(13, 1, 0.952807);
  data_grid_light_guide_efficiency->SetBinContent(13, 2, 0.973789);
  data_grid_light_guide_efficiency->SetBinContent(13, 3, 0.991873);
  data_grid_light_guide_efficiency->SetBinContent(13, 4, 1.021166);
  data_grid_light_guide_efficiency->SetBinContent(13, 5, 1.028389);
  data_grid_light_guide_efficiency->SetBinContent(13, 6, 1.035084);
  data_grid_light_guide_efficiency->SetBinContent(13, 7, 1.036862);
  data_grid_light_guide_efficiency->SetBinContent(13, 8, 1.037557);
  data_grid_light_guide_efficiency->SetBinContent(13, 9, 1.037699);
  data_grid_light_guide_efficiency->SetBinContent(13, 10, 1.042379);
  data_grid_light_guide_efficiency->SetBinContent(13, 11, 1.048732);
  data_grid_light_guide_efficiency->SetBinContent(13, 12, 1.044713);
  data_grid_light_guide_efficiency->SetBinContent(13, 13, 1.045791);
  data_grid_light_guide_efficiency->SetBinContent(13, 14, 1.047032);
  data_grid_light_guide_efficiency->SetBinContent(13, 15, 1.045210);
  data_grid_light_guide_efficiency->SetBinContent(13, 16, 1.050855);
  data_grid_light_guide_efficiency->SetBinContent(13, 17, 1.052760);
  data_grid_light_guide_efficiency->SetBinContent(13, 18, 1.051135);
  data_grid_light_guide_efficiency->SetBinContent(13, 19, 1.042319);
  data_grid_light_guide_efficiency->SetBinContent(13, 20, 1.036540);
  data_grid_light_guide_efficiency->SetBinContent(13, 21, 1.021087);
  data_grid_light_guide_efficiency->SetBinContent(13, 22, 1.006179);
  data_grid_light_guide_efficiency->SetBinContent(13, 23, 0.991909);
  data_grid_light_guide_efficiency->SetBinContent(13, 24, 0.979751);
  data_grid_light_guide_efficiency->SetBinContent(14, 1, 0.952102);
  data_grid_light_guide_efficiency->SetBinContent(14, 2, 0.976164);
  data_grid_light_guide_efficiency->SetBinContent(14, 3, 0.996727);
  data_grid_light_guide_efficiency->SetBinContent(14, 4, 1.017679);
  data_grid_light_guide_efficiency->SetBinContent(14, 5, 1.030935);
  data_grid_light_guide_efficiency->SetBinContent(14, 6, 1.038039);
  data_grid_light_guide_efficiency->SetBinContent(14, 7, 1.038480);
  data_grid_light_guide_efficiency->SetBinContent(14, 8, 1.043967);
  data_grid_light_guide_efficiency->SetBinContent(14, 9, 1.044225);
  data_grid_light_guide_efficiency->SetBinContent(14, 10, 1.046478);
  data_grid_light_guide_efficiency->SetBinContent(14, 11, 1.048124);
  data_grid_light_guide_efficiency->SetBinContent(14, 12, 1.046884);
  data_grid_light_guide_efficiency->SetBinContent(14, 13, 1.052052);
  data_grid_light_guide_efficiency->SetBinContent(14, 14, 1.051454);
  data_grid_light_guide_efficiency->SetBinContent(14, 15, 1.050309);
  data_grid_light_guide_efficiency->SetBinContent(14, 16, 1.052929);
  data_grid_light_guide_efficiency->SetBinContent(14, 17, 1.046549);
  data_grid_light_guide_efficiency->SetBinContent(14, 18, 1.045379);
  data_grid_light_guide_efficiency->SetBinContent(14, 19, 1.040710);
  data_grid_light_guide_efficiency->SetBinContent(14, 20, 1.036864);
  data_grid_light_guide_efficiency->SetBinContent(14, 21, 1.026592);
  data_grid_light_guide_efficiency->SetBinContent(14, 22, 1.010564);
  data_grid_light_guide_efficiency->SetBinContent(14, 23, 0.993606);
  data_grid_light_guide_efficiency->SetBinContent(14, 24, 0.983803);
  data_grid_light_guide_efficiency->SetBinContent(15, 1, 0.952870);
  data_grid_light_guide_efficiency->SetBinContent(15, 2, 0.975888);
  data_grid_light_guide_efficiency->SetBinContent(15, 3, 0.996476);
  data_grid_light_guide_efficiency->SetBinContent(15, 4, 1.014474);
  data_grid_light_guide_efficiency->SetBinContent(15, 5, 1.030433);
  data_grid_light_guide_efficiency->SetBinContent(15, 6, 1.038367);
  data_grid_light_guide_efficiency->SetBinContent(15, 7, 1.036131);
  data_grid_light_guide_efficiency->SetBinContent(15, 8, 1.039932);
  data_grid_light_guide_efficiency->SetBinContent(15, 9, 1.043115);
  data_grid_light_guide_efficiency->SetBinContent(15, 10, 1.046927);
  data_grid_light_guide_efficiency->SetBinContent(15, 11, 1.043115);
  data_grid_light_guide_efficiency->SetBinContent(15, 12, 1.045057);
  data_grid_light_guide_efficiency->SetBinContent(15, 13, 1.051420);
  data_grid_light_guide_efficiency->SetBinContent(15, 14, 1.048100);
  data_grid_light_guide_efficiency->SetBinContent(15, 15, 1.050004);
  data_grid_light_guide_efficiency->SetBinContent(15, 16, 1.052488);
  data_grid_light_guide_efficiency->SetBinContent(15, 17, 1.047619);
  data_grid_light_guide_efficiency->SetBinContent(15, 18, 1.044576);
  data_grid_light_guide_efficiency->SetBinContent(15, 19, 1.039385);
  data_grid_light_guide_efficiency->SetBinContent(15, 20, 1.037802);
  data_grid_light_guide_efficiency->SetBinContent(15, 21, 1.029136);
  data_grid_light_guide_efficiency->SetBinContent(15, 22, 1.012764);
  data_grid_light_guide_efficiency->SetBinContent(15, 23, 0.988362);
  data_grid_light_guide_efficiency->SetBinContent(15, 24, 0.984000);
  data_grid_light_guide_efficiency->SetBinContent(16, 1, 0.946659);
  data_grid_light_guide_efficiency->SetBinContent(16, 2, 0.969284);
  data_grid_light_guide_efficiency->SetBinContent(16, 3, 0.993160);
  data_grid_light_guide_efficiency->SetBinContent(16, 4, 1.010317);
  data_grid_light_guide_efficiency->SetBinContent(16, 5, 1.023886);
  data_grid_light_guide_efficiency->SetBinContent(16, 6, 1.032821);
  data_grid_light_guide_efficiency->SetBinContent(16, 7, 1.034056);
  data_grid_light_guide_efficiency->SetBinContent(16, 8, 1.036743);
  data_grid_light_guide_efficiency->SetBinContent(16, 9, 1.037893);
  data_grid_light_guide_efficiency->SetBinContent(16, 10, 1.041430);
  data_grid_light_guide_efficiency->SetBinContent(16, 11, 1.041659);
  data_grid_light_guide_efficiency->SetBinContent(16, 12, 1.046285);
  data_grid_light_guide_efficiency->SetBinContent(16, 13, 1.046927);
  data_grid_light_guide_efficiency->SetBinContent(16, 14, 1.043772);
  data_grid_light_guide_efficiency->SetBinContent(16, 15, 1.048441);
  data_grid_light_guide_efficiency->SetBinContent(16, 16, 1.050287);
  data_grid_light_guide_efficiency->SetBinContent(16, 17, 1.050392);
  data_grid_light_guide_efficiency->SetBinContent(16, 18, 1.045616);
  data_grid_light_guide_efficiency->SetBinContent(16, 19, 1.039028);
  data_grid_light_guide_efficiency->SetBinContent(16, 20, 1.032012);
  data_grid_light_guide_efficiency->SetBinContent(16, 21, 1.023614);
  data_grid_light_guide_efficiency->SetBinContent(16, 22, 1.007482);
  data_grid_light_guide_efficiency->SetBinContent(16, 23, 0.979566);
  data_grid_light_guide_efficiency->SetBinContent(16, 24, 0.977322);
  data_grid_light_guide_efficiency->SetBinContent(17, 1, 0.939351);
  data_grid_light_guide_efficiency->SetBinContent(17, 2, 0.961985);
  data_grid_light_guide_efficiency->SetBinContent(17, 3, 0.990823);
  data_grid_light_guide_efficiency->SetBinContent(17, 4, 1.008902);
  data_grid_light_guide_efficiency->SetBinContent(17, 5, 1.018884);
  data_grid_light_guide_efficiency->SetBinContent(17, 6, 1.027627);
  data_grid_light_guide_efficiency->SetBinContent(17, 7, 1.034699);
  data_grid_light_guide_efficiency->SetBinContent(17, 8, 1.040316);
  data_grid_light_guide_efficiency->SetBinContent(17, 9, 1.036117);
  data_grid_light_guide_efficiency->SetBinContent(17, 10, 1.036522);
  data_grid_light_guide_efficiency->SetBinContent(17, 11, 1.039405);
  data_grid_light_guide_efficiency->SetBinContent(17, 12, 1.042688);
  data_grid_light_guide_efficiency->SetBinContent(17, 13, 1.042703);
  data_grid_light_guide_efficiency->SetBinContent(17, 14, 1.042572);
  data_grid_light_guide_efficiency->SetBinContent(17, 15, 1.045318);
  data_grid_light_guide_efficiency->SetBinContent(17, 16, 1.044232);
  data_grid_light_guide_efficiency->SetBinContent(17, 17, 1.044280);
  data_grid_light_guide_efficiency->SetBinContent(17, 18, 1.038210);
  data_grid_light_guide_efficiency->SetBinContent(17, 19, 1.036564);
  data_grid_light_guide_efficiency->SetBinContent(17, 20, 1.026050);
  data_grid_light_guide_efficiency->SetBinContent(17, 21, 1.017928);
  data_grid_light_guide_efficiency->SetBinContent(17, 22, 1.000827);
  data_grid_light_guide_efficiency->SetBinContent(17, 23, 0.973603);
  data_grid_light_guide_efficiency->SetBinContent(17, 24, 0.967011);
  data_grid_light_guide_efficiency->SetBinContent(18, 1, 0.943867);
  data_grid_light_guide_efficiency->SetBinContent(18, 2, 0.963172);
  data_grid_light_guide_efficiency->SetBinContent(18, 3, 0.987290);
  data_grid_light_guide_efficiency->SetBinContent(18, 4, 1.010460);
  data_grid_light_guide_efficiency->SetBinContent(18, 5, 1.020858);
  data_grid_light_guide_efficiency->SetBinContent(18, 6, 1.027627);
  data_grid_light_guide_efficiency->SetBinContent(18, 7, 1.032738);
  data_grid_light_guide_efficiency->SetBinContent(18, 8, 1.037003);
  data_grid_light_guide_efficiency->SetBinContent(18, 9, 1.034328);
  data_grid_light_guide_efficiency->SetBinContent(18, 10, 1.040327);
  data_grid_light_guide_efficiency->SetBinContent(18, 11, 1.033752);
  data_grid_light_guide_efficiency->SetBinContent(18, 12, 1.031316);
  data_grid_light_guide_efficiency->SetBinContent(18, 13, 1.041182);
  data_grid_light_guide_efficiency->SetBinContent(18, 14, 1.040708);
  data_grid_light_guide_efficiency->SetBinContent(18, 15, 1.036186);
  data_grid_light_guide_efficiency->SetBinContent(18, 16, 1.040090);
  data_grid_light_guide_efficiency->SetBinContent(18, 17, 1.038902);
  data_grid_light_guide_efficiency->SetBinContent(18, 18, 1.029974);
  data_grid_light_guide_efficiency->SetBinContent(18, 19, 1.031527);
  data_grid_light_guide_efficiency->SetBinContent(18, 20, 1.028851);
  data_grid_light_guide_efficiency->SetBinContent(18, 21, 1.018525);
  data_grid_light_guide_efficiency->SetBinContent(18, 22, 0.994987);
  data_grid_light_guide_efficiency->SetBinContent(18, 23, 0.969841);
  data_grid_light_guide_efficiency->SetBinContent(18, 24, 0.956496);
  data_grid_light_guide_efficiency->SetBinContent(19, 1, 0.930475);
  data_grid_light_guide_efficiency->SetBinContent(19, 2, 0.963117);
  data_grid_light_guide_efficiency->SetBinContent(19, 3, 0.979764);
  data_grid_light_guide_efficiency->SetBinContent(19, 4, 0.996306);
  data_grid_light_guide_efficiency->SetBinContent(19, 5, 1.018079);
  data_grid_light_guide_efficiency->SetBinContent(19, 6, 1.024594);
  data_grid_light_guide_efficiency->SetBinContent(19, 7, 1.028910);
  data_grid_light_guide_efficiency->SetBinContent(19, 8, 1.032180);
  data_grid_light_guide_efficiency->SetBinContent(19, 9, 1.037196);
  data_grid_light_guide_efficiency->SetBinContent(19, 10, 1.043717);
  data_grid_light_guide_efficiency->SetBinContent(19, 11, 1.040269);
  data_grid_light_guide_efficiency->SetBinContent(19, 12, 1.039791);
  data_grid_light_guide_efficiency->SetBinContent(19, 13, 1.045731);
  data_grid_light_guide_efficiency->SetBinContent(19, 14, 1.044152);
  data_grid_light_guide_efficiency->SetBinContent(19, 15, 1.038650);
  data_grid_light_guide_efficiency->SetBinContent(19, 16, 1.041539);
  data_grid_light_guide_efficiency->SetBinContent(19, 17, 1.046052);
  data_grid_light_guide_efficiency->SetBinContent(19, 18, 1.034975);
  data_grid_light_guide_efficiency->SetBinContent(19, 19, 1.032857);
  data_grid_light_guide_efficiency->SetBinContent(19, 20, 1.018582);
  data_grid_light_guide_efficiency->SetBinContent(19, 21, 1.008535);
  data_grid_light_guide_efficiency->SetBinContent(19, 22, 0.987613);
  data_grid_light_guide_efficiency->SetBinContent(19, 23, 0.956026);
  data_grid_light_guide_efficiency->SetBinContent(19, 24, 0.952773);
  data_grid_light_guide_efficiency->SetBinContent(20, 1, 0.917676);
  data_grid_light_guide_efficiency->SetBinContent(20, 2, 0.939996);
  data_grid_light_guide_efficiency->SetBinContent(20, 3, 0.974379);
  data_grid_light_guide_efficiency->SetBinContent(20, 4, 0.994284);
  data_grid_light_guide_efficiency->SetBinContent(20, 5, 1.017036);
  data_grid_light_guide_efficiency->SetBinContent(20, 6, 1.027573);
  data_grid_light_guide_efficiency->SetBinContent(20, 7, 1.030179);
  data_grid_light_guide_efficiency->SetBinContent(20, 8, 1.031205);
  data_grid_light_guide_efficiency->SetBinContent(20, 9, 1.037093);
  data_grid_light_guide_efficiency->SetBinContent(20, 10, 1.043588);
  data_grid_light_guide_efficiency->SetBinContent(20, 11, 1.043249);
  data_grid_light_guide_efficiency->SetBinContent(20, 12, 1.045160);
  data_grid_light_guide_efficiency->SetBinContent(20, 13, 1.049504);
  data_grid_light_guide_efficiency->SetBinContent(20, 14, 1.046971);
  data_grid_light_guide_efficiency->SetBinContent(20, 15, 1.043299);
  data_grid_light_guide_efficiency->SetBinContent(20, 16, 1.042242);
  data_grid_light_guide_efficiency->SetBinContent(20, 17, 1.036515);
  data_grid_light_guide_efficiency->SetBinContent(20, 18, 1.034766);
  data_grid_light_guide_efficiency->SetBinContent(20, 19, 1.031901);
  data_grid_light_guide_efficiency->SetBinContent(20, 20, 1.016943);
  data_grid_light_guide_efficiency->SetBinContent(20, 21, 1.002085);
  data_grid_light_guide_efficiency->SetBinContent(20, 22, 0.987166);
  data_grid_light_guide_efficiency->SetBinContent(20, 23, 0.951092);
  data_grid_light_guide_efficiency->SetBinContent(20, 24, 0.938124);
  data_grid_light_guide_efficiency->SetBinContent(21, 1, 0.905446);
  data_grid_light_guide_efficiency->SetBinContent(21, 2, 0.929100);
  data_grid_light_guide_efficiency->SetBinContent(21, 3, 0.964877);
  data_grid_light_guide_efficiency->SetBinContent(21, 4, 0.992757);
  data_grid_light_guide_efficiency->SetBinContent(21, 5, 1.010659);
  data_grid_light_guide_efficiency->SetBinContent(21, 6, 1.024454);
  data_grid_light_guide_efficiency->SetBinContent(21, 7, 1.029330);
  data_grid_light_guide_efficiency->SetBinContent(21, 8, 1.027935);
  data_grid_light_guide_efficiency->SetBinContent(21, 9, 1.035597);
  data_grid_light_guide_efficiency->SetBinContent(21, 10, 1.042911);
  data_grid_light_guide_efficiency->SetBinContent(21, 11, 1.042973);
  data_grid_light_guide_efficiency->SetBinContent(21, 12, 1.044774);
  data_grid_light_guide_efficiency->SetBinContent(21, 13, 1.045381);
  data_grid_light_guide_efficiency->SetBinContent(21, 14, 1.044227);
  data_grid_light_guide_efficiency->SetBinContent(21, 15, 1.044172);
  data_grid_light_guide_efficiency->SetBinContent(21, 16, 1.038959);
  data_grid_light_guide_efficiency->SetBinContent(21, 17, 1.034286);
  data_grid_light_guide_efficiency->SetBinContent(21, 18, 1.034523);
  data_grid_light_guide_efficiency->SetBinContent(21, 19, 1.026724);
  data_grid_light_guide_efficiency->SetBinContent(21, 20, 1.012732);
  data_grid_light_guide_efficiency->SetBinContent(21, 21, 0.992219);
  data_grid_light_guide_efficiency->SetBinContent(21, 22, 0.977594);
  data_grid_light_guide_efficiency->SetBinContent(21, 23, 0.947606);
  data_grid_light_guide_efficiency->SetBinContent(21, 24, 0.928223);
  data_grid_light_guide_efficiency->SetBinContent(22, 1, 0.900330);
  data_grid_light_guide_efficiency->SetBinContent(22, 2, 0.930515);
  data_grid_light_guide_efficiency->SetBinContent(22, 3, 0.960167);
  data_grid_light_guide_efficiency->SetBinContent(22, 4, 0.986289);
  data_grid_light_guide_efficiency->SetBinContent(22, 5, 1.002128);
  data_grid_light_guide_efficiency->SetBinContent(22, 6, 1.018686);
  data_grid_light_guide_efficiency->SetBinContent(22, 7, 1.026479);
  data_grid_light_guide_efficiency->SetBinContent(22, 8, 1.029438);
  data_grid_light_guide_efficiency->SetBinContent(22, 9, 1.037925);
  data_grid_light_guide_efficiency->SetBinContent(22, 10, 1.043421);
  data_grid_light_guide_efficiency->SetBinContent(22, 11, 1.047723);
  data_grid_light_guide_efficiency->SetBinContent(22, 12, 1.048462);
  data_grid_light_guide_efficiency->SetBinContent(22, 13, 1.044826);
  data_grid_light_guide_efficiency->SetBinContent(22, 14, 1.044950);
  data_grid_light_guide_efficiency->SetBinContent(22, 15, 1.042204);
  data_grid_light_guide_efficiency->SetBinContent(22, 16, 1.037800);
  data_grid_light_guide_efficiency->SetBinContent(22, 17, 1.039914);
  data_grid_light_guide_efficiency->SetBinContent(22, 18, 1.035219);
  data_grid_light_guide_efficiency->SetBinContent(22, 19, 1.024413);
  data_grid_light_guide_efficiency->SetBinContent(22, 20, 1.010189);
  data_grid_light_guide_efficiency->SetBinContent(22, 21, 0.984429);
  data_grid_light_guide_efficiency->SetBinContent(22, 22, 0.960477);
  data_grid_light_guide_efficiency->SetBinContent(22, 23, 0.938934);
  data_grid_light_guide_efficiency->SetBinContent(22, 24, 0.915472);
  data_grid_light_guide_efficiency->SetBinContent(23, 1, 0.874293);
  data_grid_light_guide_efficiency->SetBinContent(23, 2, 0.899540);
  data_grid_light_guide_efficiency->SetBinContent(23, 3, 0.933195);
  data_grid_light_guide_efficiency->SetBinContent(23, 4, 0.956481);
  data_grid_light_guide_efficiency->SetBinContent(23, 5, 0.981263);
  data_grid_light_guide_efficiency->SetBinContent(23, 6, 0.997323);
  data_grid_light_guide_efficiency->SetBinContent(23, 7, 1.004277);
  data_grid_light_guide_efficiency->SetBinContent(23, 8, 1.014060);
  data_grid_light_guide_efficiency->SetBinContent(23, 9, 1.020007);
  data_grid_light_guide_efficiency->SetBinContent(23, 10, 1.024423);
  data_grid_light_guide_efficiency->SetBinContent(23, 11, 1.034811);
  data_grid_light_guide_efficiency->SetBinContent(23, 12, 1.036046);
  data_grid_light_guide_efficiency->SetBinContent(23, 13, 1.033158);
  data_grid_light_guide_efficiency->SetBinContent(23, 14, 1.033567);
  data_grid_light_guide_efficiency->SetBinContent(23, 15, 1.026439);
  data_grid_light_guide_efficiency->SetBinContent(23, 16, 1.023835);
  data_grid_light_guide_efficiency->SetBinContent(23, 17, 1.024999);
  data_grid_light_guide_efficiency->SetBinContent(23, 18, 1.016962);
  data_grid_light_guide_efficiency->SetBinContent(23, 19, 1.011986);
  data_grid_light_guide_efficiency->SetBinContent(23, 20, 0.999467);
  data_grid_light_guide_efficiency->SetBinContent(23, 21, 0.975618);
  data_grid_light_guide_efficiency->SetBinContent(23, 22, 0.938920);
  data_grid_light_guide_efficiency->SetBinContent(23, 23, 0.921228);
  data_grid_light_guide_efficiency->SetBinContent(23, 24, 0.892937);
  data_grid_light_guide_efficiency->SetBinContent(24, 1, 0.858156);
  data_grid_light_guide_efficiency->SetBinContent(24, 2, 0.886388);
  data_grid_light_guide_efficiency->SetBinContent(24, 3, 0.920861);
  data_grid_light_guide_efficiency->SetBinContent(24, 4, 0.962905);
  data_grid_light_guide_efficiency->SetBinContent(24, 5, 0.998505);
  data_grid_light_guide_efficiency->SetBinContent(24, 6, 1.009460);
  data_grid_light_guide_efficiency->SetBinContent(24, 7, 1.018225);
  data_grid_light_guide_efficiency->SetBinContent(24, 8, 1.022106);
  data_grid_light_guide_efficiency->SetBinContent(24, 9, 1.029519);
  data_grid_light_guide_efficiency->SetBinContent(24, 10, 1.040662);
  data_grid_light_guide_efficiency->SetBinContent(24, 11, 1.045424);
  data_grid_light_guide_efficiency->SetBinContent(24, 12, 1.047923);
  data_grid_light_guide_efficiency->SetBinContent(24, 13, 1.045369);
  data_grid_light_guide_efficiency->SetBinContent(24, 14, 1.041162);
  data_grid_light_guide_efficiency->SetBinContent(24, 15, 1.037693);
  data_grid_light_guide_efficiency->SetBinContent(24, 16, 1.029743);
  data_grid_light_guide_efficiency->SetBinContent(24, 17, 1.024350);
  data_grid_light_guide_efficiency->SetBinContent(24, 18, 1.016210);
  data_grid_light_guide_efficiency->SetBinContent(24, 19, 1.005852);
  data_grid_light_guide_efficiency->SetBinContent(24, 20, 0.990725);
  data_grid_light_guide_efficiency->SetBinContent(24, 21, 0.968687);
  data_grid_light_guide_efficiency->SetBinContent(24, 22, 0.928501);
  data_grid_light_guide_efficiency->SetBinContent(24, 23, 0.908896);
  data_grid_light_guide_efficiency->SetBinContent(24, 24, 0.893887);

  return data_grid_light_guide_efficiency;
}

TH2* LoadMikePhippsLightGuideEff()
{
  TFile* fsrc = new TFile("contaminationFourNeighbor.root");
  assert(fsrc->IsOpen());

  TH2F* contamHisto = fsrc->GetObjectChecked("contamHisto", "TH2F");
  const int n = contamHisto->GetXaxis()->GetNbins() - 1;  // remove one empty bins

  TH2F* data_grid_light_guide_efficiency =
      new TH2F("data_grid_light_guide_efficiency",
               "Extracted light collection efficiency from Mike Phipps;x positio fraction;y position fraction",  //
               n, 0., 1., n, 0., 1.);
  TH1F* stat_data_grid_light_guide_efficiency = new TH1F(
      "stat_data_grid_light_guide_efficiency",
      "Efficiency statistics;Reletive Efficiency;Count of bins", 100, 0, 0.33);
  TH1F* stat_data_grid_light_guide_efficiency_normalized = new TH1F(
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

  TCanvas* c2 = new TCanvas("LoadMikePhippsLightGuideEff",
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

TH2* LoadMikePhippsLightGuideEffUpdated()
{
  TFile* fsrc = new TFile("unifHistoDet1.root");
  assert(fsrc->IsOpen());

  TH2F* contamHisto = fsrc->Get("unifHisto;4096");
  assert(contamHisto);
  const int n = contamHisto->GetXaxis()->GetNbins();

  TH2F* data_grid_light_guide_efficiency =
      new TH2F("data_grid_light_guide_efficiency",
               "Extracted light collection efficiency from Mike Phipps;x positio fraction;y position fraction",  //
               n, 0., 1., n, 0., 1.);
  TH1F* stat_data_grid_light_guide_efficiency = new TH1F(
      "stat_data_grid_light_guide_efficiency",
      "Efficiency statistics;Reletive Efficiency;Count of bins", 100, 0, 1);
  TH1F* stat_data_grid_light_guide_efficiency_normalized = new TH1F(
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

  TCanvas* c2 = new TCanvas("LoadMikePhippsLightGuideEffUpdated",
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

TH1* LoadSCSF78Fiber()
{
  const double fiber_length = 13.5;       // cm
  const double attenuation_length = 105;  // cm, from Sean Stoll
  const double back_reflection = 0.3;     // Rough estimation from Sean Stoll
  const int N = 100;

  TH1* data_grid_fiber_trans =
      new TH1F("data_grid_fiber_trans",
               "SCSF-78 Fiber Transmission VS hit position in fiber;position in fiber (cm);Effective transmission",
               N, -fiber_length / 2, fiber_length / 2);

  for (int i = 1; i <= N; i++)
  {
    const double z = data_grid_fiber_trans->GetXaxis()->GetBinCenter(i);

    const double eff = (TMath::Exp(
                            -(z + fiber_length / 2) / attenuation_length) +
                        back_reflection * TMath::Exp(
                                              -(fiber_length + fiber_length / 2 - z) / attenuation_length)) /
                       (1 + back_reflection * TMath::Exp(-fiber_length * 2 / attenuation_length));
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
