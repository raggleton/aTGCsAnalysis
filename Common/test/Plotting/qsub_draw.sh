#!/bin/bash

#$ -V
#$ -cwd
#$ -l os=sld6
#$ -l site=hh
#$ -P unihh2
#$ -m eas
#$ -M robin.aggleton@desy.de
#$ -l h_vmem=3G
#$ -l h_fsize=2G
#$ -l h_rt=2:00:00


./draw --CR "${CR}" --output "${OUTPUT}" --channel "${CHANNEL}" --withMC --withData --input "${INPUT}"

