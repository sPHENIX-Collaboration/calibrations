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

(Note: this PDF set is not currently available in the sPHENIX CVMFS. The script needs to be modified once the PDF integrated.)

For independently launched Fun4All jobs, set this path after software setup:

```bash
export LHAPDF_DATA_PATH=/sphenix/user/nagle/ProjectHERWIG/Source/lhapdf${LHAPDF_DATA_PATH:+:$LHAPDF_DATA_PATH}
```

Regenerate `.run` files when changing the configuration.