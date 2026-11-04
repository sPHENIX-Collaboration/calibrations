# Herwig7 run card preparation
(The script and pre-generated cards were adapted from local files originally developed by Jamie and company.)

Prepare a Herwig generator configuration for PHHerwig7 in Fun4All:

```text
.cfg -> prepare_HerwigRuncard.sh -> .in and .run
```

Event generation runs separately inside Fun4All, which consumes the `.run` file.

## Directory structure
- `cfg/`: Generator configuration files
- `in/`: Generator input cards (include tunes etc)
- `run/`: Generator run configuration (consumed by Fun4All)

## Usage
0.  Set up the sPHENIX software environment as you normally do
1.  Make sure the script is executable, and then run it 

    ```bash
    chmod +x prepare_HerwigRuncard.sh
    # Usage: ./prepare_HerwigRuncard.sh [cfg/AAAAA.cfg]
    ./prepare_HerwigRuncard.sh cfg/mb200_sphenix.cfg

    # Configuration settings can be overridden on the command line, for example:
    # ./prepare_HerwigRuncard.sh cfg/mb200_sphenix.cfg seed=7 energy=200
    ```
    For the default seed, it produces:

    ```text
    in/mb200_sphenix_s1.in
    run/mb200_sphenix_s1.run
    ```

2. Pass the `.run` path to PHHerwig7 in the Fun4All steering macro. Configure the event count, trigger selection, and DST output in the macro.

## PDF data

The script currently reads the PDF set (NNPDF30_lo_as_0118) from Jamie's directory:

```text
/sphenix/user/nagle/ProjectHERWIG/Source/lhapdf
```

(Note: this PDF set is not currently available in the sPHENIX CVMFS. The script needs to be modified once the PDF is integrated.)

For independently launched Fun4All jobs, set this path after software setup:

```bash
export LHAPDF_DATA_PATH=/sphenix/user/nagle/ProjectHERWIG/Source/lhapdf${LHAPDF_DATA_PATH:+:$LHAPDF_DATA_PATH}
```

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
