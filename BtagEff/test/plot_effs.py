#!/usr/bin/env python


"""Quickly draw TEfficiencies from all ROOT files in given dirs.

Makes b/c/udsg plots in same location as ROOT files.

Usage:

    ./plot_effs.py <dirname1> <dirname2> ...

"""

import ROOT
import sys
import os


ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(1)
ROOT.TH1.SetDefaultSumw2()
ROOT.gStyle.SetOptStat(0)


def make_eff_plots(filename):
    print "Doing", filename
    f = ROOT.TFile(filename)
    eff_b = f.Get("BtagAnalyzer/BTaggingEff_b")
    eff_c = f.Get("BtagAnalyzer/BTaggingEff_c")
    eff_udsg = f.Get("BtagAnalyzer/BTaggingEff_udsg")

    stem = os.path.splitext(os.path.abspath(filename))[0]

    c = ROOT.TCanvas("c", "", 800, 600)
    eff_b.SetTitle("b efficiency")
    eff_b.Draw("COLZ TEXT89")
    c.SaveAs(stem+"_eff_b.pdf")

    eff_c.SetTitle("c efficiency")
    eff_c.Draw("COLZ TEXT89")
    c.SaveAs(stem+"_eff_c.pdf")

    eff_udsg.SetTitle("udsg efficiency")
    eff_udsg.Draw("COLZ TEXT89")
    c.SaveAs(stem+"_eff_udsg.pdf")
    f.Close()


if __name__ == "__main__":
    for dirname in sys.argv[1:]:
        root_files = [x for x in os.listdir(dirname) if x.endswith(".root")]
        for fname in root_files:
            make_eff_plots(os.path.join(dirname, fname))
