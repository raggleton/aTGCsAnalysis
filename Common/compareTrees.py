#!/usr/bin/env python

"""
Produce comparison plots for all variable in all branches 
in two ROOT files with same tree name.
Histograms with differing # entries or means will have 
"DIFF_" prepended to their filename.
"""


import os
import ROOT
import sys
import argparse
from collections import OrderedDict


ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(1)
ROOT.TH1.SetDefaultSumw2()
# ROOT.gErrorIgnoreLevel = ROOT.kError
ROOT.gErrorIgnoreLevel = ROOT.kBreak  # to turn off all Error in <TCanvas::Range> etc


def do_comparison_plot(T1, T2, varname, output_name, print_warning=True):
    c = ROOT.TCanvas("c"+varname, "", 800, 600)

    h1name = "h1_%s" % varname
    T1.Draw(varname + ">>" + h1name)
    h1 = ROOT.TH1F(ROOT.gROOT.FindObject(h1name))
    h1_colour = ROOT.kBlack
    h1.SetLineColor(h1_colour)
    h1.SetLineWidth(2)
    h1.Draw("HIST")
    c.Update()
    stats1 = h1.GetListOfFunctions().FindObject("stats").Clone("stats1")

    # Need to use same binning as first hist
    h2name = "h2_%s" % varname
    # print "Hist settings: ", h1.GetNbinsX(), h1.GetBinLowEdge(1), h1.GetBinLowEdge(1+h1.GetNbinsX())
    h2 = ROOT.TH1F(h2name, "", h1.GetNbinsX(), h1.GetBinLowEdge(1), h1.GetBinLowEdge(1+h1.GetNbinsX()))
    # T2.Draw(varname + ">>%s(, %d, %g, %g)" % (h2name, h1.GetNbinsX(), h1.GetBinLowEdge(1), h1.GetBinLowEdge(1+h1.GetNbinsX())))
    # h2 = ROOT.TH1F(ROOT.gROOT.FindObject(h2name))
    T2.Draw(varname + ">>%s" % (h2name))
    h2_colour = ROOT.kRed
    h2.SetLineColor(h2_colour)
    h2.SetLineStyle(2)
    h2.SetLineWidth(2)
    h2.Draw("HIST")
    c.Update()
    # To get the 2nd stats box we MUST NOT draw with SAME
    # Instead draw by itself, then plot them together afterwards
    stats2 = h2.GetListOfFunctions().FindObject("stats").Clone("stats1")
    stats2.SetY1NDC(0.52);
    stats2.SetY2NDC(0.7);
    stats2.SetTextColor(h2_colour);

    if h1.GetEntries() == 0 and h2.GetEntries() == 0:
        return

    # Do a simple check to see if hists differ
    is_diff = False
    if print_warning and (h1.GetEntries() > 0 or h2.GetEntries() > 0):
        if h1.GetEntries() != h2.GetEntries():
            is_diff = True
            print varname, " has differing entries", h1.GetEntries(), "vs", h2.GetEntries()
        if h1.GetMean() != h2.GetMean():
            is_diff = True
            print varname, " has differing means", h1.GetMean(), "vs", h2.GetMean()

    if is_diff:
        basename = os.path.basename(output_name)
        output_name = output_name.replace(basename, "DIFF_"+basename)

    hst = ROOT.THStack()
    hst.Add(h1)
    hst.Add(h2)
    hst.Draw("HIST NOSTACK")
    stats1.Draw()
    stats2.Draw()
    c.Modified()
    c.SaveAs(output_name)

    return is_diff


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("filename1", help="Reference ROOT filename")
    parser.add_argument("filename2", help="Comparison ROOT filename")
    default_dir = "ComparisonPlots"
    parser.add_argument("--outputDir", help="Output directory for plots, defaults to %s" % default_dir, default=default_dir)
    default_tree = "treeDumper/BasicTree"
    parser.add_argument("--treeName", help="Name of TTree, defaults to %s" % default_tree, default=default_tree)
    args = parser.parse_args()
    print args

    f1 = ROOT.TFile(args.filename1)
    f2 = ROOT.TFile(args.filename2)

    T1 = f1.Get(args.treeName)
    T2 = f2.Get(args.treeName)

    br_list1 = T1.GetListOfBranches()
    # br_list2 = T2.GetListOfBranches()
    num_br = len(br_list1)

    results = OrderedDict()

    print "Plots produced in", args.outputDir
    if not os.path.isdir(args.outputDir):
        os.makedirs(args.outputDir)

    for i in range(br_list1.GetEntries()):
    # for i in range(130, 200):
        branch1 = br_list1.At(i)
        branch_name = branch1.GetName()
        print "BRANCH", i, ":", branch_name
        branch2 = T2.GetBranch(branch_name)
        if branch2 is None:
            print "WARNING Tree2 doesn't have branch ", branch_name

        output_name = os.path.join(args.outputDir, branch_name+"_compare.pdf")
        is_diff = do_comparison_plot(T1, T2, branch_name, output_name)
        results[branch_name] = is_diff

    f1.Close()
    f2.Close()

    # Print results
    if any(results.values()):
        max_len = max([len(x) for x in results.keys()])
        print "*" * max_len
        print "Differing vars (%d/%d):" % (len([r for r in results.values() if r]), num_br)
        print "*" * max_len
        for name, result in results.iteritems():
            if result:
                print name
        print "*" * max_len
        sys.exit(1)
    else:
        print "All distributions same"
        sys.exit(0)