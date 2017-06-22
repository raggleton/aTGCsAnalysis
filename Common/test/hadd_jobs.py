#!/usr/bin/env python


"""
hadd output of crab jobs utilising the fact that the output files are here at DESY,
so we can skip the painful crab getoutput
"""


import os
import subprocess
from collections import namedtuple
from itertools import chain
import re

from retrieve_jobs import TaskDictionaryNameUnordered
from submit_jobs import MCBackgroundsSampleDictionaryUnordered, SignalMCSampleDictionaryUnordered, DataDictionaryMuonChannelUnordered, DataDictionaryElectronChannelUnordered


# Location of crab output files
NAF_DIR = "/pnfs/desy.de/cms/tier2/store/user/raggleto/"

# Where you want the hadded files to go
OUTPUT_DIR = "/nfs/dust/cms/user/aggleton/aTGCsAnalysis/Samples_80X_Working_21_6_17"

# whether to run hadd jobs on bird cluster instead
RUN_ON_BIRD = True

Sample = namedtuple("Sample", ["taskname", "dataset", "outname"])


def create_sample_listing():
    """Go through submission lists and retrival list, and match based on task name to form one larger listing"""
    entries = []
    for task_name, dataset in chain(MCBackgroundsSampleDictionaryUnordered, SignalMCSampleDictionaryUnordered, DataDictionaryMuonChannelUnordered):
        matching_output = [x for x in TaskDictionaryNameUnordered if x[0] == task_name]
        if len(matching_output) > 1:
            raise RuntimeError("More than 1 match for %s" % task_name)
        if len(matching_output) == 0:
            print "No match for task %s" % task_name
            continue
        output_name = matching_output[0][1]
        entries.append(Sample(task_name, dataset, output_name))

    return entries


def get_job_status(job_dir):
    print "Checking job status of", job_dir
    cmd = "crab status -d crab_projects/%s" % job_dir
    status_text = subprocess.check_output(cmd, shell=True)
    return {
        "finished": ("Jobs status:                    finished      100.0%" in status_text),
        "task_name": re.search(r"Task name:[ \t]+([0-9a-zA-Z_:-]+)", status_text).group(1)
    }


def do_one_task(entry, channel):
    """Do checking & hadding for one crab task"""
    if channel not in ["mu", "ele"]:
        raise RuntimeError("channel arg must be mu or ele")

    output_file = os.path.join(OUTPUT_DIR, entry.outname + "_%s.root" % channel)

    if os.path.isfile(output_file):
        print "! Output file already exists - skipping this task"
        return

    crab_dir = "crab_%s_%s_my_feature" % (entry.taskname, channel)

    status_dict = get_job_status(crab_dir)
    print status_dict
    # if not status_dict['finished']:
    #     print "crab jobs not finished - skipping"
    #     return

    sample_dir = entry.dataset.split("/")[1]
    date_str = status_dict['task_name'].split(":")[0]
    input_str = os.path.join(NAF_DIR, sample_dir, crab_dir, date_str, "0000", "tree_%s_*.root" % channel)

    # actually do the hadding
    if RUN_ON_BIRD:
        qsub_command = """qsub -N %s -v OUTPUTF="%s",INPUTF="%s" qsub_hadd.sh""" % (entry.taskname, output_file, input_str)
        # print qsub_command # Uncomment this line when testing to view the qsub command
        subprocess.check_call(qsub_command, shell=True)
    else:
        hadd_cmd = "hadd %s %s" % (output_file, input_str)
        print hadd_cmd
        subprocess.check_output(hadd_cmd, shell=True)  # need shell=True for wildcard expansion?


if __name__ == "__main__":
    samples = create_sample_listing()
    for x in samples:
        print x

    if not os.path.isdir(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    for ind, entry in enumerate(samples, 1):
        print ">>> [%d/%d]" % (ind, len(samples)), entry.taskname
        do_one_task(entry, "mu")
