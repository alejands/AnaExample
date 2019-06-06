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

    
    //triggers
    bool passHLT             = false;
    ////2016
    //bool HLT_4j45_3b087      = false;
    //bool HLT_2j90_2j30_3b087 = false;
    ////2018
    //bool HLT_HT330_4j_75_60_45_40_3b = false;
    //bool HLT_4j_103_88_75_15_2b_VBF1 = false;
    //bool HLT_4j_103_88_75_15_1b_VBF2 = false;
    //bool HLT_2j116_dEta1p6_2b        = false;
    //bool HLT_J330_m30_2b             = false;
    //bool HLT_j500                    = false;
    //bool HLT_2j300ave                = false;

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

    // Constructors and member functions
    eventData(TChain*, bool, std::string, bool); 
    void update(int);

    void dump();
    ~eventData(); 

  };

}
#endif // exampleEventData_H
