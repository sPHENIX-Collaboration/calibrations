#! /bin/bash

SHOW(){
cat << EOF
Running in directory:
$(pwd)

Directory contents:
$(ls -1a)

EOF
}

if ! [ -f $1 ] || ! [[ $2 =~ ^[0-9]+$ ]]; then
cat << EOF

	usage:
		$0 [file list] [line number]
	Wrapper shell script to rum ../macros/Fun4All_SiliconOnlyAlignment.C
	Which produces millepede residuals with its HelicalFitter SubsysReco module

	Must be run as a Condor job

EOF
	exit 0
fi

LIST=$1
LINE=$2

if [ -z "${_CONDOR_SCRATCH_DIR}" ] || ! [ -d "${_CONDOR_SCRATCH_DIR}" ]; then
cat << EOF

	Job must run under Condor

EOF
	exit 1
fi

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
source alignment_setup.sh
source /opt/sphenix/core/bin/sphenix_setup.sh -n new
if [ -n "$MYINSTALL" ] && [ -d "$MYINSTALL" ]; then
	source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL
fi

cd ${_CONDOR_SCRATCH_DIR}
rsync -av ${ALIGN_SOURCE_DIR}/Fun4All_SiliconOnlyAlignment.C .
rsync -av ${ALIGN_SOURCE_DIR}/${LIST} .

# The submitter script has copied the correct thing to the DATA dir for
rsync -av ${ALIGN_DATA_DIR}/localAlignmentParamsFile.txt .
SHOW

root -q -b "Fun4All_SiliconOnlyAlignment.C(\"$(basename "${LIST}")\", ${LINE})"
RV=$?

SHOW
mkdir -p ${ALIGN_DATA_DIR}/dat
cp clusters_seeds* ${ALIGN_DATA_DIR}/dat/.

echo "$0" done
exit $RV

