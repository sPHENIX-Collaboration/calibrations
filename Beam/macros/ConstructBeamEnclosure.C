// $Id: $

/*!
 * \file ConstructBeamEnclosure.C
 * \brief 
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#include <TEveGeoNode.h>
#include <TEveManager.h>
#include <TGLClip.h>
#include <TGLUtil.h>
#include <TGLViewer.h>
#include <TGeoManager.h>
#include <TGeoPcon.h>
#include <TROOT.h>

void ConstructBeamEnclosure()
{
  TEveManager::Create();

  // gStyle->SetCanvasPreferGL(true);
  TGeoManager *geom = new TGeoManager("BeamEnclosure", "BeamEnclosure");
  //--- define some materials
  TGeoMaterial *matVacuum = new TGeoMaterial("VACUUM", 0, 0, 0);
  //   //--- define some media
  TGeoMedium *Vacuum = new TGeoMedium("VACUUM", 1, matVacuum);
  //--- define the transformations

  //--- make the top container volume
  Double_t worldx = 10000.;
  Double_t worldy = 10000.;
  Double_t worldz = 10000.;
  TGeoVolume *top = geom->MakeBox("TOP", Vacuum, worldx, worldy, worldz);
  geom->SetTopVolume(top);

  static const double eclosure_expansion = .5;
  {
    //  Hadron forward
    //  z (cm)  R (cm)
    //  66.81   3.1       pipe lip
    //  67.051  3.227
    //  120   7.582+1.3
    //  124   7.582+1.3   Inner Flange
    //  448.425 12.896+9.4    hadron pipe
    //  450   3.0       electron pipe
    TGeoVolume *vol = gGeoManager->MakePcon("HadronForwardEnclosure", Vacuum, 0, 360, 6);
    TGeoPcon *pcon = (TGeoPcon *) (vol->GetShape());
    pcon->DefineSection(0, 66.81, 0, 3.1 + eclosure_expansion);
    pcon->DefineSection(1, 67.051, 0, 3.227 + eclosure_expansion);
    pcon->DefineSection(2, 120, 0, 7.582 + 1.3 + eclosure_expansion);
    pcon->DefineSection(3, 124, 0, 7.582 + 1.3 + eclosure_expansion);
    pcon->DefineSection(4, 448.425, 0, 12.896 + 9.4 + eclosure_expansion);
    pcon->DefineSection(5, 450, 0, 3 + eclosure_expansion);
    top->AddNode(vol, 1);
  }

  {
    //    Electron forward
    //    z (cm)    R (cm)
    //    -79.810   3.1   pipe lip
    //    -80     3.227
    //    -450    10.25+3.372
    //    -463    10.050+3.372
    TGeoVolume *vol = gGeoManager->MakePcon("ElectronForwardEnclosure", Vacuum, 0, 360, 4);
    TGeoPcon *pcon = (TGeoPcon *) (vol->GetShape());
    pcon->DefineSection(0, -463, 0, 10.050 + 3.372 + eclosure_expansion);
    pcon->DefineSection(1, -450, 0, 10.25 + 3.372 + eclosure_expansion);
    pcon->DefineSection(2, -80, 0, 3.227 + eclosure_expansion);
    pcon->DefineSection(3, -79.810, 0, 3.1 + eclosure_expansion);
    top->AddNode(vol, 2);
  }

  //--- close the geometry
  geom->CloseGeometry();
  //--- draw the ROOT box.

  assert(gGeoManager);

  gGeoManager->Export("ConstructBeamEnclosure.gdml");

  if (!gROOT->GetListOfGeometries()->FindObject(gGeoManager))
    gROOT->GetListOfGeometries()->Add(gGeoManager);
  if (!gROOT->GetListOfBrowsables()->FindObject(gGeoManager))
    gROOT->GetListOfBrowsables()->Add(gGeoManager);
  //  gGeoManager->UpdateElements();

  TGeoNode *current = gGeoManager->GetCurrentNode();
  //Alternate drawing
  //current->GetVolume()->Draw("ogl");
  //Print the list of daughters
  //current->PrintCandidates();
  //  for (int igeom = 0; igeom < current->GetNdaughters(); igeom++)
  //    {
  //      TGeoNode *geo_node = (TGeoNode*) current->GetNodes()->UncheckedAt(igeom);
  //      geo_node->GetVolume()->VisibleDaughters(kFALSE);
  //      geo_node->GetVolume()->SetTransparency(2);
  //    }
  TEveGeoTopNode *eve_node = new TEveGeoTopNode(gGeoManager, current);
  eve_node->SetVisLevel(6);
  gEve->AddGlobalElement(eve_node);
  gEve->FullRedraw3D(kTRUE);

  // EClipType not exported to CINT (see TGLUtil.h):
  // 0 - no clip, 1 - clip plane, 2 - clip box
  TGLViewer *v = gEve->GetDefaultGLViewer();
  //  if (do_clip)
  //    {
  //      v->GetClipSet()->SetClipType( TGLClip::kClipPlane  );
  //    }
  //  v->ColorSet().Background().SetColor(kMagenta + 4);
  v->SetGuideState(TGLUtil::kAxesEdge, kTRUE, kFALSE, 0);
  v->RefreshPadEditor(v);

  v->CurrentCamera().RotateRad(-1.6, 0.);
  v->DoDraw();
}
