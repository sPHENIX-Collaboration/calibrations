# JEWEL cards for PHJewel

These are JEWEL 2.6.0 parameter cards and tables for `PHJewel`, the Fun4All module in
coresoftware `generators/PHJewel`. They are adapted from cards originally written by Jamie Nagle.

Each run uses a **main card**. For Au+Au, the main card points to a **medium card** through
`MEDIUMPARAMS`. You choose the medium type in the macro with `set_medium()`; it is not in the card.

## Cards

| main card | medium | process |
|---|---|---|
| `pp200_dijet.dat` | `vac` | p+p 200 GeV dijets (`PPJJ`), pT-hat > 15 GeV |
| `pp200_photonjet.dat` | `vac` | p+p 200 GeV prompt photon + jet (`PPYJ`), pT-hat > 10 GeV |
| `auau200_<cent>_dijet.dat` | `simple` | Au+Au 200 GeV dijets, recoils kept |
| `auau200_<cent>_photonjet.dat` | `simple` | Au+Au 200 GeV prompt photon + jet, recoils kept |

`<cent>` is `0010`, `1030`, `3050` or `5090` (0-10%, 10-30%, 30-50% and 50-90%). The matching
medium cards are `medium_auau200_<cent>.dat`.

Common settings:
- CT14nlo proton PDF (LHAPDF 13100, from the sPHENIX cvmfs LHAPDF)
- three active flavours
- pT-hat weighted generation (`WEIGHTED T`, `WEXPO 5.`)
- `ISOCHANNEL XX` for Au+Au: proton and neutron mix by Z/A
- `ETAMAX 2.5`

The medium is the JEWEL simple medium: a Glauber initial state with Bjorken expansion. It uses
T_i = 375 MeV and tau_i = 0.4 fm/c, as for the JEWEL x_J curves in sPH-CONF-JET-2025-05. The other
settings are TC 0.17 GeV, SIGMANN 4.2 fm^2 (42 mb), MDFACTOR 0.45 and MDSCALEFAC 0.9.

`PPYJ` switches on the PYTHIA 6 prompt-photon processes qg -> q gamma, q qbar -> g gamma and
gg -> g gamma. The photon does not interact with the medium; only the recoiling parton is quenched.
`PPYQ` (quark jets only) and `PPYG` (gluon jets only) are also available.

The p+p dijet card and the Au+Au dijet cards are the ones used to reproduce the JEWEL x_J curves of
sPH-CONF-JET-2025-05 (Fig. 6) with PHJewel.

## Recoils and 4MomSub

The Au+Au cards keep the medium recoils (`KEEPRECOILS T`). They also write the thermal momenta of
the scattering centres as status-3 entries (`WRITESCATCEN T`). Jets that contain recoils carry
thermal momentum that belongs to the underlying event. 4MomSub (arXiv:1707.04142) removes it: you
cluster the status-3 entries as ghosts and subtract their four-momenta from each jet. Geant never
transports the status-3 entries.

## Usage

```cpp
PHJewel *jw = new PHJewel();
jw->set_card(std::string(getenv("CALIBRATIONROOT")) + "/Generators/JEWEL/auau200_0010_dijet.dat");
jw->set_medium("simple");   // "vac" for the p+p cards

PHJewelPhotonTrigger *trig = new PHJewelPhotonTrigger();   // or PHJewelJetTrigger
trig->SetMinPt(10.);
trig->SetEtaMax(1.1);
jw->register_trigger(trig);
```

Normalise a histogram filled with the event weight (`weights()[0]`) by
`sigma / Sum_Of_Weight`, using the values in `RUN/PHGenIntegral`.

## Writing your own card

Copy a card and edit it. PHJewel reads the cards strictly. JEWEL itself silently falls back to
its LHC Pb+Pb defaults on a typo, so PHJewel refuses a card that has any of these problems:
- an unknown or duplicated key;
- a value it cannot parse;
- a missing required key;
- a line longer than 80 characters in the main card (100 in the medium card), because standalone
  JEWEL truncates longer lines.

Relative paths, such as `tables/...` and the medium card, are relative to the card's own
directory. The full card rules are in the coresoftware `generators/PHJewel/README.md`.

## Tables

The cards also need three JEWEL lookup tables:
- `splitint.dat`: splitting integrals, 79 MB
- `pdfs_ct14nlo.dat`: the medium-scattering PDFs, not the proton PDF
- `xsecs_ct14nlo.dat`: elastic cross sections

They are too large for this repository, so they are installed on cvmfs outside git, in
`$CALIBRATIONROOT/JEWEL_tables/`, next to the other large calibration files. The cards point to them
as `../../JEWEL_tables/<file>`, a path relative to the card, so they work wherever
`$CALIBRATIONROOT` is. To use a local checkout of this repository, copy or link that directory to
`JEWEL_tables/` at the top of the checkout. PHJewel refuses to start if a table is missing, because
otherwise JEWEL would spend hours regenerating it.

They are the tables from the JEWEL 2.6.0 installation that the original cards used.

JEWEL does not check what parameters a table was built with. PHJewel prints the fraction of lookups
that fall outside the tables at the end of a run. For the Au+Au 0-10% simple medium, about 11-12%
of PDF and cross-section lookups are outside. If you change Q0, LAMBDAQCD, NF or the medium Debye
mass, the tables may need to be regenerated: run a dedicated job with
`PHJewel::allow_table_generation()` and a card that points at new file names.
