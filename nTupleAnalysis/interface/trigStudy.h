// -*- C++ -*-
#if !defined(trigStudy_H)
#define trigStudy_H

#include <ctime>
#include <sys/resource.h>

#include <TChain.h>
#include <TTree.h>
#include <TF1.h>
#include <TSpline.h>
#include <TRandom3.h>
#include "DataFormats/FWLite/interface/InputSource.h" //for edm::LuminosityBlockRange
#include "nTupleAnalysis/baseClasses/interface/brilCSV.h"
#include "nTupleAnalysis/baseClasses/interface/initBranch.h"
#include "AnaExample/nTupleAnalysis/interface/eventData.h"
#include "nTupleAnalysis/baseClasses/interface/cutflowHists.h"
#include "AnaExample/nTupleAnalysis/interface/eventHists.h"
#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulatorTool.h"

namespace nTupleAnalysis {

  class trigStudy {
  public:

    TChain* events;
    TChain* runs;
    TChain* lumiBlocks;
    Long64_t genEventCount;
    double_t genEventSumw;
    double_t genEventSumw2;
    Long64_t mcEventCount = 0;
    double_t mcEventSumw  = 0;
    double_t mcEventSumw2 = 0;
    
    bool debug = false;
    std::string year;
    bool isMC  = false;
    bool blind = true;
    int histogramming = 1e6;
    int treeEvents;
    eventData* event;

    eventHists* allEvents   = NULL;
    TH1F*   hOffJetPt         = NULL;
    TH1F*   hOffJetPt_l         = NULL;
    TH1F*   hOffJetPt_matched = NULL;
    TH1F*   hOffJetPt_l_matched = NULL;
    TH1F*   hOffJetDeltaR     = NULL;


    long int nEvents = 0;
    double lumi      = 1;
    std::vector<edm::LuminosityBlockRange> lumiMask;
    UInt_t prevLumiBlock = 0;
    UInt_t firstRun      = 1e9;
    UInt_t lastRun       = 0;
    UInt_t prevRun       = 0;
    UInt_t nruns = 0;
    UInt_t nls   = 0;
    float  intLumi = 0;
    double kFactor = 1;
    double xs = 1;

    bool writePicoAOD = false;
    TFile* picoAODFile;
    TTree* picoAODEvents;
    TTree* picoAODRuns;
    TTree* picoAODLumiBlocks;

    nTupleAnalysis::cutflowHists* cutflow;
    nTupleAnalysis::cutflowHists* passTrig;

    //Monitoring Variables
    long int percent;
    std::clock_t start;
    double duration;
    double eventRate;
    double timeRemaining;
    int minutes;
    int seconds;
    int who = RUSAGE_SELF;
    struct rusage usage;
    long int usageMB;

    trigStudy(TChain*, TChain*, TChain*, fwlite::TFileService&, bool, bool, std::string, int, bool);
    void createPicoAOD(std::string);
    void addDerivedQuantitiesToPicoAOD();
    void storePicoAOD();
    void monitor(long int);
    int eventLoop(int);
    int processEvent();
    bool passLumiMask();
    std::map<edm::LuminosityBlockID, float> lumiData;
    void getLumiData(std::string);
    void countLumi();
    ~trigStudy();


    //
    //  The Trigger Emulation
    // 
    TriggerEmulator::TrigEmulatorTool* trigEmulator_PF40;
    TriggerEmulator::TrigEmulatorTool* trigEmulator_PF80;
    TriggerEmulator::TrigEmulatorTool* trigEmulator_PFHT180;

  };// trigStudy

}
#endif // trigStudy_H

