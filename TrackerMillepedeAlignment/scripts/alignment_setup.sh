#! /bin/bash

export USER="$(id -u -n)"
export LOGNAME="${USER}"
export HOME="/sphenix/u/${USER}"

export ALIGN_SOURCE_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && cd .. && pwd)"

mkdir -p ${ALIGN_SOURCE_DIR}/txt
if ! [ -f "${ALIGN_SOURCE_DIR}/txt/iteration.txt" ]; then
	echo "0" > "${ALIGN_SOURCE_DIR}/txt/iteration.txt" 
fi
export ITR=$(cat "${ALIGN_SOURCE_DIR}/txt/iteration.txt")

# These may need to be changed for your use case
export MYINSTALL="/sphenix/user/${USER}/MYINSTALL"
export ALIGN_DATA_DIR="/sphenix/tg/tg01/hf/${USER}/alignment"

if [ -z "${ALIGN_DATA_DIR}" ] || ! [ -d "${ALIGN_DATA_DIR}" ] || ! [ -w "${ALIGN_DATA_DIR}" ]; then
cat << EOF

    Define the variable
        ALIGN_DATA_DIR
    in
        ${ALIGN_SOURCE_DIR}/scripts/$(basename $0)
    to an existing directory (where you have write permissions)
                                                                                  
    This script will not create one for you.

    Note that you may also wish to change the value of MYINSTALL, which is
        ${MYINSTALL}

EOF
    exit 1
fi

export ALIGN_DATA_DIR="${ALIGN_DATA_DIR}/iteration_${ITR}"
mkdir -p ${ALIGN_DATA_DIR}

