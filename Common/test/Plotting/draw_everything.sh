#!/bin/bash
#
# Do all the plots!
#
ROOTDIR="/nfs/dust/cms/user/aggleton/aTGCsAnalysis/CMSSW_8_0_25/src/aTGCsAnalysis/Common/test/Samples_80X_Working_21_6_17/"


for cr in ttbar WJets TTBarEnrichedInclusive TTBarEnrichedBTagVeto
do
    echo "Doing ${cr}"
    # Run locally
    # ./draw --CR "${cr}" --output "${cr}_CR" --channel mu --withMC --withData --input "${ROOTDIR}"

    # Run on BIRD
    qsub -N ${cr} -v CHANNEL="mu",CR=${cr},OUTPUT="${cr}_CR",INPUT="${ROOTDIR}" qsub_draw.sh
done