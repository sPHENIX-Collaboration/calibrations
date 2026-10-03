#!/bin/bash
# Prepare .in and .run cards for PHHerwig7, adapted from Jamie's hwrun.sh.
# Usage: ./prepare_HerwigRuncard.sh cfg/sample.cfg [key=value ...]
#
# Any key in the config may be overridden on the command line.
set -euo pipefail

# ---------------------------------------------------------------- defaults ---
energy=200
process=minbias
minkt=0
preweight=0
preweight_scale=30
tune=nashville
pdf=NNPDF30_lo_as_0118
stable_strange=1
diffraction=0
max_lifetime=-1
photon_minkt=0
photon_maxeta=0
mhatmin=-1
seed=1

die() { echo "prepare_HerwigRuncard: $*" >&2; exit 1; }

[ $# -ge 1 ] || die "usage: $0 cfg/sample.cfg [key=value ...]"
CFG=$1
shift
[ -f "$CFG" ] || die "no such config: $CFG"

# ------------------------------------------------------------ read config ---
# "key = value", # comments, blank lines ignored.  Unknown keys are an error so
# a typo cannot silently produce a differently-configured sample.
known=" energy process minkt preweight preweight_scale tune pdf stable_strange diffraction max_lifetime photon_minkt photon_maxeta mhatmin seed "
setkey() {
    case "$known" in
        *" $1 "*) printf -v "$1" '%s' "$2" ;;
        *) die "unknown key '$1'" ;;
    esac
}

while IFS= read -r line || [ -n "$line" ]; do
    line=${line%%#*}
    line=$(echo "$line" | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')
    [ -z "$line" ] && continue
    [[ "$line" == *=* ]] || die "expected key=value: $line"
    k=$(echo "${line%%=*}" | sed 's/[[:space:]]*$//')
    v=$(echo "${line#*=}" | sed 's/^[[:space:]]*//')
    setkey "$k" "$v"
done < "$CFG"

for kv in "$@"; do
    case "$kv" in
        *=*) setkey "${kv%%=*}" "${kv#*=}" ;;
        *) die "expected key=value: $kv" ;;
    esac
done

# ------------------------------------------------------------ environment ---
HERE=$(cd "$(dirname "$0")" && pwd)
#! Note: this PDF set is not currently available in the sPHENIX CVMFS, point it to the local lhapdf install instead for now
export LHAPDF_DATA_PATH=/sphenix/user/nagle/ProjectHERWIG/Source/lhapdf${LHAPDF_DATA_PATH:+:$LHAPDF_DATA_PATH}

if [ -n "${HERWIGDIR:-}" ]; then
    HW=$HERWIGDIR/bin/Herwig
else
    HW=$(command -v Herwig) || die "Herwig is unavailable in PATH"
fi
[ -x "$HW" ] || die "Herwig is not executable: $HW"

# Herwig's own share/ must stay on the read path (snippets/*.in live there); -I
# only PREPENDS, so give it explicitly.  This is what lets the cards live here
# instead of being copied into a read-only cvmfs install.
HWSHARE=$(cd "$(dirname "$HW")/../share/Herwig" && pwd)
command -v bc >/dev/null || die "bc is unavailable in PATH"

# --------------------------------------------------------------- the card ---
mkdir -p "$HERE/in" "$HERE/run"
tag=$(basename "$CFG" .cfg)_s${seed}
card=$HERE/in/${tag}.in

me=MEQCD2to2
if [ "$process" = "gammajet" ]; then
    me=MEGammaJet
fi

{
    echo "# Generated from $CFG by prepare_HerwigRuncard.sh"
    echo "read snippets/PPCollider.in"

    case "$process" in
        minbias)
            echo "read snippets/MB.in"
            # single- and double-diffractive subprocesses on top of the
            # non-diffractive MB.in, as PYTHIA SoftQCD:inelastic generates
            if [ "$diffraction" = "1" ]; then
                echo "read snippets/Diffraction.in"
            fi
            ;;
        hardqcd|gammajet)
            echo "cd /Herwig/MatrixElements/"
            echo "insert SubProcess:MatrixElements[0] $me"
            echo "cd /"
            ;;
        *) die "unknown process '$process'" ;;
    esac

    echo "set /Herwig/Generators/EventGenerator:EventHandler:LuminosityFunction:Energy ${energy}*GeV"

    case "$tune" in
        nashville|newhaven|lht_mpi)
            echo "read tune-${tune//_/-}.in"
            ;;
        default) ;;
        *) die "unknown tune '$tune'" ;;
    esac

    # PDFs set explicitly on every path so minbias and hardqcd are comparable.
    for p in HardLOPDF ShowerLOPDF MPIPDF RemnantPDF; do
        echo "set /Herwig/Partons/${p}:PDFName ${pdf}"
    done
    echo "set /Herwig/Particles/p+:PDF /Herwig/Partons/HardLOPDF"
    echo "set /Herwig/Particles/pbar-:PDF /Herwig/Partons/HardLOPDF"

    if [ "$stable_strange" = "1" ]; then
        echo "read stable-strange.in"
    fi

    # Leave every particle type whose mean c*tau exceeds max_lifetime [mm] undecayed,
    # for Geant4 to decay -- PYTHIA's ParticleDecays:limitTau0 + tau0Max.
    if [ "$(echo "$max_lifetime > 0" | bc -l)" = "1" ]; then
        echo "set /Herwig/Decays/DecayHandler:MaxLifeTime ${max_lifetime}*mm"
        echo "set /Herwig/Decays/DecayHandler:LifeTimeOption Average"
    fi

    # Generation-level parton cut.  Applies to BOTH outgoing partons
    if [ "$process" != "minbias" ] && [ "$(echo "$minkt > 0" | bc -l)" = "1" ]; then
        echo "set /Herwig/Cuts/JetKtCut:MinKT ${minkt}*GeV"
    fi

    # Global cut on the partonic invariant mass.  Herwig's DEFAULT is
    # Cuts:MHatMin = 20 GeV (defaults/Cuts.in:100).  For a CENTRAL 2->2 process
    # mHat ~= 2*pT, so 20 GeV silently removes everything below pT ~ 10 GeV -- which
    # at RHIC is most of the interesting range.  Set it explicitly for 200 GeV work.
    if [ "$(echo "$mhatmin >= 0" | bc -l)" = "1" ]; then
        echo "set /Herwig/Cuts/Cuts:MHatMin ${mhatmin}*GeV"
    fi

    # Generation-level cut on the hard photon. Herwig's DEFAULT PhotonKtCut:MinKT is
    # 20 GeV (defaults/Cuts.in) -- an LHC value that sits ABOVE essentially the whole
    # RHIC direct-photon range, so leaving it alone silently produces a sample that
    # starts at 20 GeV no matter what JetKtCut:MinKT says. Always set it explicitly.
    if [ "$(echo "$photon_minkt > 0" | bc -l)" = "1" ]; then
        echo "set /Herwig/Cuts/PhotonKtCut:MinKT ${photon_minkt}*GeV"
    fi
    if [ "$(echo "$photon_maxeta > 0" | bc -l)" = "1" ]; then
        echo "set /Herwig/Cuts/PhotonKtCut:MinEta -${photon_maxeta}"
        echo "set /Herwig/Cuts/PhotonKtCut:MaxEta ${photon_maxeta}"
    fi

    # Pre-weight: sampler over-produces hard events, compensating weight rides in
    # the HepMC record. Alternative to stitching MinKT windows.
    if [ "$(echo "$preweight > 0" | bc -l)" = "1" ]; then
        echo "create ThePEG::ReweightMinPT reweightMinPT ReweightMinPT.so"
        echo "set reweightMinPT:Power ${preweight}"
        echo "set reweightMinPT:Scale ${preweight_scale}*GeV"
        echo "insert /Herwig/MatrixElements/${me}:Preweights 0 reweightMinPT"
    fi

    # Fun4All controls event generation and PHHerwig7 converts to HepMC.
    echo "set /Herwig/Random:Seed ${seed}"
    # in-process: Fun4All owns the event count, so the .run must not cap it
    echo "set /Herwig/Generators/EventGenerator:NumberOfEvents -1"
    echo "set /Herwig/Generators/EventGenerator:DebugLevel 0"
    echo "set /Herwig/Generators/EventGenerator:PrintEvent 0"
    echo "set /Herwig/Generators/EventGenerator:MaxErrors 100000"
    echo "saverun ${tag} /Herwig/Generators/EventGenerator"
} > "$card"

cd "$HERE/run"
"$HW" read -I "$HERE/in" -i "$HWSHARE" "$card"
[ -s "${tag}.run" ] || die "missing run card: $HERE/run/${tag}.run"
echo "Prepared input: $card"
echo "Prepared run:   $HERE/run/${tag}.run"