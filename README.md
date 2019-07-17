# nTupleAnalysis

Process NANOAOD files to create skims (picoAODs) and event loop histograms. 

From a CMSSW/src release area

>#(only do this once the first time) cmsrel CMSSW_10_2_0

>cd CMSSW_10_2_0/src

>cmsenv

Checkout the nTupleAnalysis base class repo

>git clone git@github.com:johnalison/nTupleAnalysis.git

>scram b -j 5

>voms-proxy-init -voms cms

Run the following with --help to see what command line argument you can specify. Otherwise it runs with the default input files and default output directory. 

>exampleAna AnaExample/nTupleAnalysis/scripts/exampleAna_cfg.py  -i AnaExample/nTupleAnalysis/fileLists/ZToMuMu.txt -o $PWD -y 2018 -l 59.6e3 --histogramming 1 --histFile hists.root --isMC --nevents 1000


# Luminosity Data

When running on data a live count of the integrated luminosity that has been processed can be displayed. The luminosity per lumiblock in a given lumiMask json file is calculated with brilcalc. 

First download the lumiMask:

>wget https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions18/13TeV/PromptReco/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt

>mv Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt ZZ4b/lumiMasks/

Then setup the brilconda environment:

>export PATH=$HOME/.local/bin:/cvmfs/cms-bril.cern.ch/brilconda/bin:$PATH

And calculate the lumi contained in the json file for a given trigger. You must specify the trigger version, this can be done by including "_v*" at the end of the trigger name. 
The recommended normtag can be found at https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiLUM.

>brilcalc lumi -c web --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_BRIL.json -u /pb -i lumiMasks/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt --hltpath "HLT_PFHT330PT30_QuadPFJet_75_60_45_40_Tripl
ePFBTagDeepCSV_4p5_v*" --byls -o lumiMasks/brilcalc_2018_HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5.csv 

The output csv file can be quite large. For 2018 it is 30MB. This is because there is one line for every lumiblock and there are O(1M) lumiblocks in a dataset. The option --byls tells brilcalc to give the lumi split by lumiblock; the default is to only specify the lumi per run. 

