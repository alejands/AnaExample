// -*- C++ -*-

#if !defined(exampleEventData_H)
#define exampleEventData_H

#include <iostream>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include "nTupleAnalysis/baseClasses/interface/initBranch.h"
#include "nTupleAnalysis/baseClasses/interface/truthData.h"
#include "nTupleAnalysis/baseClasses/interface/jetData.h"
#include "nTupleAnalysis/baseClasses/interface/muonData.h"
#include "nTupleAnalysis/baseClasses/interface/trigData.h"

// for jet pseudoTag calculations
#include <numeric> 
#include <boost/math/special_functions/binomial.hpp> 

namespace nTupleAnalysis {

  class eventData {

  public:
    // Member variables
    TChain* tree;
    bool isMC;
    std::string year;
    bool debug;
    UInt_t    run       =  0;
    UInt_t    lumiBlock =  0;
    ULong64_t event     =  0;
    Int_t     nPVs = 0;
    Int_t     nPVsGood = 0;
    Float_t   genWeight =  1;
    Float_t   weight    =  1;

    
    //
    //triggers
    //
    bool passHLT             = false;
    
    //
    // HT Triggers
    //
    std::vector<std::string> HLT_PFHT_Names = {//"HLT_PFHT125" , "HLT_PFHT200", "HLT_PFHT300", "HLT_PFHT400","HLT_PFHT475", "HLT_PFHT600", "HLT_PFHT650", "HLT_PFHT800",
					       "HLT_PFHT180", "HLT_PFHT250", 
					       //"HLT_PFHT350", //"HLT_PFHT350MinPFJet15", 
					       "HLT_PFHT370",
					        "HLT_PFHT430",  "HLT_PFHT510",
					       "HLT_PFHT590",  "HLT_PFHT680",
					       "HLT_PFHT780", "HLT_PFHT890"};
    std::map<std::string, Bool_t> HLT_PFHT_Results;

    //
    // PF Jet Triggers
    //
    std::vector<std::string> HLT_PFJet_Names = {"HLT_PFJet40", "HLT_PFJet60", "HLT_PFJet80", "HLT_PFJet140",
						"HLT_PFJet200", "HLT_PFJet260", "HLT_PFJet320", "HLT_PFJet400",
						"HLT_PFJet450"};
    std::map<std::string, Bool_t> HLT_PFJet_Results;						


    //
    // Di-PF Jet Triggers
    //
    std::vector<std::string> HLT_DiPFJet_Names = {"HLT_DiPFJetAve40",      "HLT_DoublePFJets40_CaloBTagDeepCSV_p71", "HLT_DiPFJetAve60",       "HLT_DiPFJetAve60_HFJEC",
						  "HLT_DiPFJetAve80", "HLT_DiPFJetAve80_HFJEC","HLT_DiPFJetAve100_HFJEC","HLT_DoublePFJets100_CaloBTagDeepCSV_p71",
						  "HLT_DiPFJetAve140",     "HLT_DiPFJetAve160_HFJEC",
						  "HLT_DiPFJetAve200",     "HLT_DiPFJetAve220_HFJEC","HLT_DiPFJetAve260",     "HLT_DiPFJetAve320",
						  "HLT_DiPFJetAve400"};
    std::map<std::string, Bool_t> HLT_DiPFJet_Results;						



    //
    // Quad-PF Jet Triggers
    //
    std::vector<std::string> HLT_QuadPFJet_Names = {"HLT_QuadPFJet103_88_75_15", "HLT_QuadPFJet105_88_76_15",
						    "HLT_QuadPFJet111_90_80_15", "HLT_QuadPFJet98_83_71_15", "HLT_PFHT330PT30_QuadPFJet_75_60_45_40",
						    "HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5"};
    
    std::map<std::string, Bool_t> HLT_QuadPFJet_Results;						


    std::vector<std::string> HLT_Unprescaled_Names = {"HLT_PFHT1050",
						      "HLT_PFJet500", "HLT_PFJet550",
						      "HLT_DiPFJetAve300_HFJEC"};
    std::map<std::string, Bool_t> HLT_Unprescaled_Results;						    


//    std::vector<std::string> L1_Names = {"L1_DoubleJet100er2p3_dEta_Max1p6",
//					 "L1_DoubleJet100er2p5",
//					 "L1_DoubleJet112er2p3_dEta_Max1p6",
//					 "L1_DoubleJet120er2p5",
//					 "L1_DoubleJet150er2p5",
//					 "L1_DoubleJet30er2p5_Mass_Min150_dEta_Max1p5",
//					 "L1_DoubleJet30er2p5_Mass_Min200_dEta_Max1p5",
//					 "L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5",
//					 "L1_DoubleJet30er2p5_Mass_Min300_dEta_Max1p5",
//					 "L1_DoubleJet30er2p5_Mass_Min330_dEta_Max1p5",
//					 "L1_DoubleJet30er2p5_Mass_Min360_dEta_Max1p5",
//					 "L1_DoubleJet40er2p5",
//					 "L1_HTT120er",
//					 "L1_HTT160er",
//					 "L1_HTT200er",
//					 "L1_HTT255er",
//					 "L1_HTT280er",
//					 "L1_HTT280er_QuadJet_70_55_40_35_er2p4",
//					 "L1_HTT320er",
//					 "L1_HTT320er_QuadJet_70_55_40_40_er2p4",
//					 "L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3",
//					 "L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3",
//					 "L1_HTT360er",
//					 "L1_HTT400er",
//					 "L1_HTT450er",
//					 "L1_QuadJet60er2p5",
//					 "L1_SingleJet10erHE",
//					 "L1_SingleJet120",
//					 "L1_SingleJet120_FWD3p0",
//					 "L1_SingleJet120er2p5",
//					 "L1_SingleJet12erHE",
//					 "L1_SingleJet140er2p5",
//					 "L1_SingleJet140er2p5_ETMHF80",
//					 "L1_SingleJet140er2p5_ETMHF90",
//					 "L1_SingleJet160er2p5",
//					 "L1_SingleJet180",
//					 "L1_SingleJet180er2p5",
//					 "L1_SingleJet200",
//					 "L1_SingleJet20er2p5_NotBptxOR",
//					 "L1_SingleJet20er2p5_NotBptxOR_3BX",
//					 "L1_SingleJet35",
//					 "L1_SingleJet35_FWD3p0",
//					 "L1_SingleJet35er2p5",
//					 "L1_SingleJet43er2p5_NotBptxOR_3BX",
//					 "L1_SingleJet46er2p5_NotBptxOR_3BX",
//					 "L1_SingleJet60",
//					 "L1_SingleJet60_FWD3p0",
//					 "L1_SingleJet60er2p5",
//					 "L1_SingleJet8erHE",
//					 "L1_SingleJet90",
//					 "L1_SingleJet90_FWD3p0",
//					 "L1_SingleJet90er2p5"};
//    std::map<std::string, Bool_t> L1_Results;						    

    const float jetPtMin = 40;
    const float jetEtaMax= 2.4;
    const bool doJetCleaning=false;
     
    nTupleAnalysis::jetData* treeJets;
    std::vector< std::shared_ptr<nTupleAnalysis::jet> > allJets;//all jets in nTuple
    std::vector< std::shared_ptr<nTupleAnalysis::jet> > selJets;//jets passing pt/eta requirements
 
    uint nSelJets;

    nTupleAnalysis::muonData* treeMuons;
    std::vector< std::shared_ptr<nTupleAnalysis::muon> > allMuons;
    std::vector< std::shared_ptr<nTupleAnalysis::muon> > isoMuons;

    //nTupleAnalysis::trigData* treeTrig;
    //std::vector< std::shared_ptr<nTupleAnalysis::trig> > allTrigJets;//all jets in nTuple
    //std::vector< std::shared_ptr<nTupleAnalysis::trig> > allTrigFatJets;//all jets in nTuple
    //std::vector< std::shared_ptr<nTupleAnalysis::trig> > allTrigElecs;//all jets in nTuple
    //std::vector< std::shared_ptr<nTupleAnalysis::trig> > allTrigPhotons;//all jets in nTuple
    //std::vector< std::shared_ptr<nTupleAnalysis::trig> > allTrigTaus;//all jets in nTuple
    //std::vector< std::shared_ptr<nTupleAnalysis::trig> > allTrigHT;//all jets in nTuple
    float ht, ht30, L1ht, L1ht30, HLTht, HLTht30;

    // Constructors and member functions
    eventData(TChain*, bool, std::string, bool); 
    void update(int);

    void dump();
    ~eventData(); 

  };

}
#endif // exampleEventData_H
