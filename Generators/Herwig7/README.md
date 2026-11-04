# Herwig7 input cards
(The cards were adapted from local files originally developed by Jamie and company.)

Herwig generator configurations for PHHerwig7 in Fun4All. Each sample is one self-contained `.in` card, the same way the PHPythia8 `.cfg` cards are kept. PHHerwig7 reads the card in-process at Init.

## Directory structure
- `in/`: sample cards, and the files they `read`:
  - `tune-lht-mpi.in`: MPI part of the Les Houches Tune, read by `mb200_sphenix_lht_s1.in`
  - `tune-nashville.in`, `tune-newhaven.in`: STAR-based MPI/underlying-event tunes (arXiv:2411.16897), fitted with NNPDF3.0 LO; not read by any card
  - `stable-strange.in`: weakly-decaying strange hadrons stable (read its header before using it)

## Sample cards
All cards are p+p at sqrt(s) = 200 GeV.

| Card | Process | Tune | PDF | Decays | Follows |
|---|---|---|---|---|---|
| `mb200_sphenix_s1.in` | minimum bias: non-diffractive + diffractive | Herwig default | CT14lo | particles with c*tau > 1e-7 mm left to Geant4 | `phpythia8_detroit_minBias.cfg` |
| `mb200_sphenix_lht_s1.in` | minimum bias: non-diffractive + diffractive | Les Houches Tune, MPI part (`tune-lht-mpi.in`) | CT14lo | particles with c*tau > 1e-7 mm left to Geant4 | `mb200_sphenix_s1.in` |
| `mb200_ue_s1.in` | non-diffractive minimum bias, for underlying-event observables | Herwig default | CT14lo | all by Herwig except the weakly-decaying strange hadrons (`stable-strange.in`) | |
| `mb200_ue_decayed_s1.in` | non-diffractive minimum bias, for underlying-event observables | Herwig default | CT14lo | all by Herwig | `mb200_ue_s1.in` |
| `heavyquark200_sphenix_s1.in` | c cbar + b bbar | Herwig default | CT14lo | particles with c*tau > 1e-7 mm left to Geant4 | HF TG D0 card `HeavyFlavor_TG/phpythia8_d02kpi_MDC2.cfg` |
| `heavyquark200_sphenix_decayed_s1.in` | c cbar + b bbar | Herwig default | CT14lo | all by Herwig | `heavyquark200_sphenix_s1.in` |

## Usage
Pass the card to `PHHerwig7::set_config_file` in the Fun4All steering macro, for example `$CALIBRATIONROOT/Generators/Herwig7/in/mb200_sphenix_s1.in`. The tune and other files a card `read`s are found next to the card. Configure the seed, event count, trigger selection, and DST output in the macro.

The cards should not contain `saverun` or `run` command: PHHerwig7 creates the run in memory, and stops at Init if the card contains either command.


Regenerate `.run` files when changing the configuration.

## Photon+jet cards (p+p 200 GeV, Herwig 7.3.0 default tune)

`in/Herwig_Photon{5,10,20}_{direct,frag}.in` are self-contained cards, with their `.run` files in `run/`. They do **not** go through
`prepare_HerwigRuncard.sh`; to rebuild a `.run` file:

```bash
cd run
Herwig read ../in/Herwig_Photon10_direct.in   # -> Herwig_Photon10_direct.run
```

They are J. Nagle's sPHENIX G4-request cards (Herwig 7.3.0 default tune, CT14lo in all four PDF
slots, MHatMin 2 GeV, ACDC Ntry 1e5, generator decays only for c*tau0 < 1e-7 mm as in the PYTHIA
MDC2 photon cards). Each original card is split into two samples with one matrix element each:

| Sample            | Matrix element | Generation cut c (JetKtCut = PhotonKtCut) | Photon trigger | Truth photon pT used |
|-------------------|----------------|-------------------------------------------|----------------|----------------------|
| Photon5_direct    | MEGammaJet     | 2.5 GeV                                   | pT > 5 GeV     | < 14 GeV             |
| Photon5_frag      | MEQCD2to2      | 2.5 GeV                                   | pT > 5 GeV     | < 14 GeV             |
| Photon10_direct   | MEGammaJet     | 4 GeV                                     | pT > 10 GeV    | 14-22 GeV            |
| Photon10_frag     | MEQCD2to2      | 6 GeV                                     | pT > 10 GeV    | 14-22 GeV            |
| Photon20_direct   | MEGammaJet     | 7 GeV                                     | pT > 20 GeV    | > 22 GeV             |
| Photon20_frag     | MEQCD2to2      | 12 GeV                                    | pT > 20 GeV    | > 22 GeV             |

- **direct**: prompt photons from the hard process. **frag**: QCD 2->2 with photons from the QED
  shower (fragmentation photons). Together they are the Herwig analogue of PYTHIA
  `PromptPhoton:all` + `HardQCD:all`.
- The split exists for speed: in the combined card MEQCD2to2 dominates the cross section, and
  almost all events fail the photon trigger. Each frag cut was raised to the highest value at
  which a generation-cut scan showed <~1% loss of photons in that sample's truth-pT window.
- Trigger in the Fun4All macro: `PHHerwig7ParticleTrigger`, photon (PID 22), |eta| < 1.5,
  pT above the threshold in the table, prompt only (`SetPromptOnly`: final-state photon with no
  hadron parent).
- Combining: direct and frag are added by cross section, and the three pT samples are stitched on
  truth photon pT at 14 and 22 GeV. Per-event weight of a sample = 1 / (sum of
  `Integrated_Lumi` over its `PHGenIntegral` nodes), i.e. sigma * N_written / N_generated per
  written event. The per-sample weights of the official production are listed on the Jet TG wiki.
- CT14lo ships with the sPHENIX Herwig installation; no `LHAPDF_DATA_PATH` change is needed.
- The matching `run/Herwig_Photon*_{direct,frag}.run` files are committed (built with `Herwig read` in the
  `-n new` environment; identical to the ones used for production). Regenerate them whenever a card changes.
