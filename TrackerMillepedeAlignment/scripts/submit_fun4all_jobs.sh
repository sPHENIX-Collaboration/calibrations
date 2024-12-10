#! /bin/bash

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
source alignment_setup.sh
cd ${ALIGN_SOURCE_DIR}

if [ ${ITR} -eq 0 ]; then
	cp txt/localAlignmentParamsFile.txt ${ALIGN_DATA_DIR}/.
fi

# RUN_NUMS="41989 41992"
# FILE_FORMAT="/sphenix/lustre01/sphnxpro/commissioning/slurp/tpcbeam/run_%08d_%08d/*%08d*"

# RUN_NUMS="53285"
# FILE_FORMAT="/sphenix/lustre01/sphnxpro/physics/slurp/streaming/physics/new_2024p002/run_%08d_%08d/*%08d*"

# CHANGE ME
RUN_NUMS="49708 49709 49710 49711" # 49706 49707 49708 49709 49710 49711 49712 49713
FILE_FORMAT="/sphenix/lustre01/sphnxpro/physics/slurp/tracking/new_2024p005/run_%08d_%08d/DST_TRKR_HIT_*%08d*"

ARGS=""
for RUN_NUM in ${RUN_NUMS}; do
	ARGS="${ARGS}_${RUN_NUM}"
done

mkdir -p list
LIST="list/segments${ARGS}.list"

if ! [ -f ${LIST} ] || [ "$1" == "-f" ]; then
	rm -rf "$LIST"
	for RUN_NUM in $RUN_NUMS; do
		FILE_PATTERN=$(printf ${FILE_FORMAT} $(( (RUN_NUM / 100) * 100 )) $(( (RUN_NUM / 100 + 1) * 100 )) ${RUN_NUM})
		echo "${FILE_PATTERN}"
		ls -1 ${FILE_PATTERN} >> ${LIST} 2> /dev/null
	done
fi

NUM_JOB=$(wc -l ${LIST} | awk '{print $1}')

mkdir -p job
mkdir -p out
JOB="job/fun4all${ARGS}.job"
cat << EOF > ${JOB}
universe           = vanilla
executable         = scripts/Fun4All_SiliconOnlyAlignment.sh
arguments          = $LIST \$(process)

notification       = Never

output             = out/out${ARGS}_\$(process).out
error              = out/out${ARGS}_\$(process).out
log                = /tmp/${USER}_alignment${ARGS}_\$(process).log

request_memory     = 8192MB
PeriodicHold       = (NumJobStarts >= 1 && JobStatus == 1)
concurrency_limits = CONCURRENCY_LIMIT_DEFAULT:100

queue ${NUM_JOB}
EOF

echo "submitting job file $JOB"
condor_submit $JOB

