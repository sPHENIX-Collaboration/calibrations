## Alignment Workflow ##
### A directory containing scripts and macros for doing alignment ###

Workflow:
* Edit `scripts/alignment_setup.sh` for your environment
	* Define `MYINSTALL` to the location of your custom-built sPHENIX libraries (if you need custom libraries--you do not necessarily need this)
	* Define `ALIGN_DATA_DIR` to the top-level directory where you want data output (.root files, pede binaries, large .txt files)
	* The setup script will NOT create top-level directories if they do not already exist
	* (But it will create subdirectories for you)
* Run `scripts/alignment_setup.sh` for the first time
	* Check that an `iteration_0` subdirectory appears under `ALIGN_DATA_DIR`
	* Check that an `txt/iteration.txt` file appers (which only contains "0")
* Run `scripts/submit_fun4all_jobs.sh`
	* Copies `localAlignmentParamsFile.txt` to `ALIGN_DATA_DIR/iteration_0` (only if this is the 0th iteration)
	* Creates a file list with one line per segment for each run in the `RUN_NUMS` variable
	* You can edit this variable (and will likely need to edit the `FILE_FORMAT` variable depending on the range)
	* This creates a job file to submit a job for each segment--the job is `scripts/Fun4All_SiliconOnlyAlignment.sh`
	* The output is under `ALIGN_DATA_DIR/iteration_0/dat`, the `out` files are in `out/`
* Run `scripts/run_pede.sh`
	* This runs millepede over the expansion `ALIGN_DATA_DIR/iteration_0/dat/*.bin`
	* It creates the steering file with ls and sets the scale errors to the end of this file
	* It then runs several macros in sequences to get the next localAlignmentParamsFile.txt and create plots
* Run `scripts/increment.sh`
	* This increments the contents `txt/iteration.txt` and copies the `newLocalAlignmentParamsFile.txt` to `localAlignmentParamsFile.txt` for the next iteration

