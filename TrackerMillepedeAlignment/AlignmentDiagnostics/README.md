# Tracking Alignment Macros

This directory contains ROOT macros used for post-processing `residualtree` and `truthtree` outputs from sPHENIX tracking studies. The macros cover single-track residuals, two-track DCA studies, vertex diagnostics, truth-level summaries, reduced tree production, and histogram overlay utilities.

## Prerequisites

Run these macros from an environment where ROOT is available. Several macros expect the input ROOT files to contain either:

- `residualtree`: reconstructed-track residual and vertex information
- `truthtree`: truth-track kinematics and reconstruction-match information

Typical invocation pattern:

```bash
root -b -q 'MacroName.C(arguments)'
```

## Macros Summary

### `MakeHistograms_singleTrackQuantities.C`

Builds single-track histograms from `residualtree`, including DCA distributions, momentum and angular distributions, hit-content summaries, and Gaussian fit-slice outputs versus `p`, `pT`, `eta`, and `phi`. It supports both data-like and simulation-like inputs through the `is_simulation_input` argument which can be passed as `true` or `false`. The input file paths and output file names can be set in the two blocks after `TChain *residualtree = new TChain("residualtree");` corresponding to `is_simulation_input = true` and `is_simulation_input = false`, respectively.


To run:

```bash
root -b -q 'MakeHistograms_singleTrackQuantities.C(false)'
```
Use true instead of false for simulation input.

### `MakeHistograms_twoTrack_and_vertex_distributions.C`

Processes `residualtree` to form same-event track pairs, compute two-track DCA observables, compare reconstructed and truth vertex quantities when available, and write two-track and vertex-level histograms. The `is_simulation_input` argument switches on truth-dependent branches and simulation-specific cuts and can be passed as `true` or `false`. The input file paths and output file names can be set in the two blocks after `TChain *resid_base = new TChain("residualtree");` corresponding to `is_simulation_input = true` and `is_simulation_input = false`, respectively.


To run:

```bash
root -b -q 'MakeHistograms_twoTrack_and_vertex_distributions.C(false)'
```
Use true instead of false for simulation input.

### `Create_trackpairstree.C`

Builds a reduced `track_pairs` tree from `residualtree`. The output keeps event, crossing, vertex, momentum, charge, DCA, and optional truth information for selected same-event same-crossing track pairs. This macro is useful when downstream studies should run on a compact pair-level ntuple instead of the full residual tree and can be run over both simulated and experimental root files using the is_simulation_input argument as true or false respectively.

To run:
```bash
root -b -q 'Create_trackpairstree.C(false)'
```
Use true instead of false for simulation input.

### `MakeHistograms_withPID_Simulation.C`

Simulation-only macro that reads `residualtree` with truth information and particle flavor labels, then produces PID-resolved histograms for DCA, true DCA, momentum resolution, and related fit summaries. It is intended for truth-matched performance studies of identified hadrons.

To run:
```bash
root -b -q 'MakeHistograms_withPID_Simulation.C'
```

### `PlotTruthTrackQuantities.C`

Reads `truthtree` and fills truth-track kinematic histograms split by cut category and reconstruction-match status. This is useful for checking truth acceptance, cut migration, and matched versus unmatched truth-track populations. 

Arguments:
- `infile` = input rootfile 
- `treename` = tree read in from infile.

To run:
```bash
root -b -q 'PlotTruthTrackQuantities.C("input.root","truthtree")'
```

### `TrimROOTTree.C`

Creates a smaller ROOT tree by cloning only the branches needed by the analysis macros. This is useful when the original ntuple is large and only a restricted set of reconstructed or truth branches is needed for fast iteration. 

Arguments:

- `inputpattern`: file pattern passed to `TChain::Add` from which the output tree is produced
- `outputfile`: output ROOT file name
- `treename`: input tree name, default `tree`
- `is_simulation_input`: enables truth branches when `true`, should be set to true or false for running over simulation and experimental input root files respectively.

To run:
```bash
root -b -q 'TrimROOTTree.C("input*.root","trimmed.root","residualtree",false)'
```

### `Overlayhistograms_samehists_differentfiles.C`

Loads matching 1D histograms from several ROOT files and overlays them on common canvases and outputs a combined pdf file containing all the canvases. This is intended for direct comparison between data and simulation outputs, or between alternate processing configurations.

To run:
```bash
root -b -q 'Overlayhistograms_samehists_differentfiles.C(".")'
```


### `Overlayhistograms_differenthists_samefile.C`

Loads related histograms from a single ROOT file and overlays them together on different canvases and outputs a combined pdf file containing all the canvases.

To run:
```bash
root -b -q 'Overlayhistograms_differenthists_samefile.C'
```

### `OverlayDrawFitSlicesYComparisonforTH3D.C`

Uses utilities from `UsefulFunctions.h` to compare fit-slice projections derived from 3D histograms between simulation and data outputs. It is intended for compact visual comparisons of two-track DCA and related observables as functions of kinematic variables.

To run:
```bash
root -b -q 'OverlayDrawFitSlicesYComparisonforTH3D.C'
```

## Helper Header

### `UsefulFunctions.h`

Shared helper header providing histogram cleanup, Gaussian fit-slice extraction, mode/FWHM/RMS extraction, TH3 slice fitting, overlay helpers, and canvas-export utilities. This file is included by several macros and is not meant to be run directly.

## Suggested Workflow

1. Generate (simulated) or collect (data) input ROOT files containing `residualtree` or `truthtree`.
2. Optionally run `TrimROOTTree.C` to build a lighter analysis ntuple.
3. Run one or more production macros such as `MakeHistograms_singleTrackQuantities.C`, `MakeHistograms_twoTrack_and_vertex_distributions.C`, or `Create_trackpairstree.C` to generate the diagnostic plots.
4. Use the overlay macros to compare outputs across different configurations.
