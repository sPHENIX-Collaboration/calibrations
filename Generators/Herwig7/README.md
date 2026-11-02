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


### Disclaimer and acknowledgement
The cards were adapted from files originally developed by Jamie and company. The Fun4All integration was implemented with the assistance of Claude Code (Claude Team plan for scientists).