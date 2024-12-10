#! /bin/bash

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
source alignment_setup.sh
source /opt/sphenix/core/bin/sphenix_setup.sh -n new
if [ -n "$MYINSTALL" ] && [ -d "$MYINSTALL" ]; then
	source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL
fi

cd ${ALIGN_DATA_DIR}

# pede
STEER="steer.txt"
rm -rf ${STEER}
ls -1 dat/*.bin >> ${STEER} 2> /dev/null
echo "scaleerrors 5 5" >> ${STEER} 2> /dev/null
pede ${STEER} | tee millepede.out
echo "pede done"

root -q -b "${ALIGN_SOURCE_DIR}/process_millepede_results.C+" | tee process_millepede_results.out
root -q -b "${ALIGN_SOURCE_DIR}/plot_fits.C" | tee plot_fits.out
root -q -b "${ALIGN_SOURCE_DIR}/makeAlignmentParsSumFile.C+" | tee makeAlignmentParsSumFile.out
root -q -b "${ALIGN_SOURCE_DIR}/plot_alignment_residuals.C+" | tee plot_alignment_residuals.out
root -q -b "${ALIGN_SOURCE_DIR}/detailed_alignment_parameter_plots.C" | tee detailed_alignment_parameter_plots.out

echo "$0 done"

