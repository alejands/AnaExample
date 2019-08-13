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

    


    struct trigCounter {
      
      unsigned int nTotal = 0;
      int mode = 0;  // pt Cut / mode = 1 Use shape of turn on
      TRandom3* rand = nullptr;

      std::vector<std::string> PFJet_trigNames = {"40",   "60","80","140","200","260","320","400","450","500","550"};
      std::vector<float> PFJet_trigThresholds  = {40. ,      60.,     80.,   140.,   200.,   260.,   320.,   400.,   450.,   500.,  550.};
      // From Fits to tTbar MC (2017)
      std::vector<float> PFJet_p0              = {0.188,  0.166,   0.150,  0.137,  0.121,  0.113,  0.100,  0.100,   0.086,  0.085,  0.087};
      std::vector<float> PFJet_p1              = {42.45,   64.6,    85.8,   150.,    212.2, 272.8,  330.6,  411.5,  462.3,  505.5,  559.1};

      std::map<std::string, unsigned int> PFJet_nPass;
      std::map<std::string, bool>         PFJet_pass;
      std::map<std::string, float>        PFJet_thresholds;
      std::map<std::string, TF1*>         PFJet_Sigmoid;


      std::vector<std::string> PFHT_trigNames = {"180",   "250",    "370",    "430",   "510",   "590",    "680",   "780",    "890",   "1050"};
      std::vector<float> PFHT_trigThresholds  = {180. ,    250.     ,370.,     430.,    510.,    590.,     680.,    780.,     890.,    1050.};
      std::vector<float> PFHT_p0              = {0.0319,  0.0343,  0.0334,  0.0347,  0.0334,   0.0313,   0.0294,  0.0283,  0.0268,     0.0273};
      std::vector<float> PFHT_p1              = {213.0,    294.1 ,  422.1 ,   483.4 ,   566.9 ,  650.0 ,    743.7 , 847.4 ,  956.5 ,    1113.};


      std::map<std::string, unsigned int> PFHT_nPass;
      std::map<std::string, float>        PFHT_thresholds;
      std::map<std::string, TF1*>         PFHT_Sigmoid;
      

      trigCounter(){

	mode = 1;
	rand = new TRandom3();


	//
	// PFJet
	//
	for(unsigned int i = 0; i<PFJet_trigNames.size(); ++i){
	  std::string tName = PFJet_trigNames.at(i);
	  float       thres = PFJet_trigThresholds.at(i);
	  PFJet_nPass.insert(std::make_pair(tName,0));
	  PFJet_pass .insert(std::make_pair(tName,false));
	  PFJet_thresholds .insert(std::make_pair(tName,thres));
	  PFJet_Sigmoid.insert(std::make_pair(tName, new TF1(("func"+tName).c_str(),"(1.0/(1+ TMath::Exp(-[0]*(x-[1]))))", 0, 14000)));
	  PFJet_Sigmoid[tName]->SetParameters(PFJet_p0.at(i), PFJet_p1.at(i));
	}

	//
	// PFHT
	//
	for(unsigned int i = 0; i<PFHT_trigNames.size(); ++i){
	  std::string tName = PFHT_trigNames.at(i);
	  float       thres = PFHT_trigThresholds.at(i);
	  PFHT_nPass.insert(std::make_pair(tName,0));
	  PFHT_thresholds .insert(std::make_pair(tName,thres));
	  PFHT_Sigmoid.insert(std::make_pair(tName, new TF1(("funcHT"+tName).c_str(),"(1.0/(1+ TMath::Exp(-[0]*(x-[1]))))", 0, 14000)));
	  PFHT_Sigmoid[tName]->SetParameters(PFHT_p0.at(i), PFHT_p1.at(i));
	}

	
      };

      void Fill(eventData* event){
	++nTotal;

	//
	// PFJet
	//
	for(auto& passThisJet : PFJet_pass){
	  passThisJet.second = false;
	}

	for(auto &jet: event->allJets){

	  for(const std::string& trigN : PFJet_trigNames){
	    if(mode == 0){
	      if(jet->pt > PFJet_thresholds[trigN]) 
		PFJet_pass[trigN] = true;
	    } else{
	      float probPassTrig = PFJet_Sigmoid[trigN]->Eval(jet->pt);
	      if(probPassTrig > rand->Rndm())
		PFJet_pass[trigN] = true;
	    }
	  }
	}

	for(const std::string& trigN : PFJet_trigNames){
	  if(PFJet_pass[trigN]) ++PFJet_nPass[trigN];
	}

	//
	// PFHT
	//
	for(const std::string& trigN : PFHT_trigNames){
	  if(mode == 0){
	    if(event->ht >  PFHT_thresholds[trigN])
	      ++PFHT_nPass[trigN];
	  }else{
	    float probPassTrig = PFHT_Sigmoid[trigN]->Eval(event->ht);
	    if(probPassTrig > rand->Rndm())
	      ++PFHT_nPass[trigN];
	  }
	}	


      };

      
      void dumpPFJet(){
	std::cout << "N Total\t\t" << nTotal << std::endl;
	for(const std::string& trigN : PFJet_trigNames){
	  std::cout << " J " << trigN << " Pass Frac\t\t" << float(PFJet_nPass[trigN])/nTotal << "    (" << PFJet_nPass[trigN] << ")"<< std::endl;
	}

	  
      };

      void dumpEffPFJet(){
	std::cout << "np.array((" ;
	for(const std::string& trigN : PFJet_trigNames){
	  std::cout << float(PFJet_nPass[trigN])/nTotal << " , ";
	}
	std::cout << "),dtype=np.float32), " << nTotal << " )" << std::endl;
	
      };


      void dumpPFHT(){
	std::cout << "N Total\t\t" << nTotal << std::endl;
	for(const std::string& trigN : PFHT_trigNames){
	  std::cout << " J " << trigN << " Pass Frac\t\t" << float(PFHT_nPass[trigN])/nTotal << "    (" << PFHT_nPass[trigN] << ")"<< std::endl;
	}

	  
      };

      void dumpEffPFHT(){
	std::cout << "np.array((" ;
	for(const std::string& trigN : PFHT_trigNames){
	  std::cout << float(PFHT_nPass[trigN])/nTotal << " , ";
	}
	std::cout << "),dtype=np.float32), " << nTotal << " )" << std::endl;
	
      };


    };
    std::map<std::string, trigCounter> trigCounts;

    //
    //  The Trigger Emulation
    // 
    TriggerEmulator::TrigEmulatorTool* trigEmulator_PF40;
    TriggerEmulator::TrigEmulatorTool* trigEmulator_PF80;

  };// trigStudy

}
#endif // trigStudy_H

