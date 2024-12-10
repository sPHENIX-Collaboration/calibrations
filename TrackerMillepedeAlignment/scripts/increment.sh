cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
source alignment_setup.sh

NEW_ALIGN_PARAMS=${ALIGN_DATA_DIR}/newLocalAlignmentParamsFile.txt

ITR=$((ITR+1))
echo "${ITR}" | tee "${ALIGN_SOURCE_DIR}/txt/iteration.txt" 

source alignment_setup.sh
cp ${NEW_ALIGN_PARAMS} ${ALIGN_DATA_DIR}/localAlignmentParamsFile.txt

