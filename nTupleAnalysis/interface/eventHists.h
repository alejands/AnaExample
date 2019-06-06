// -*- C++ -*-
#if !defined(eventHists_H)
#define eventHists_H

#include <iostream>
#include <TH1F.h>
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "AnaExample/nTupleAnalysis/interface/eventData.h"
#include "nTupleAnalysis/baseClasses/interface/jetHists.h"
#include "nTupleAnalysis/baseClasses/interface/muonHists.h"

using namespace nTupleAnalysis;

namespace nTupleAnalysis {

  class eventHists {
  public:
    bool doViews;
    TFileDirectory dir;
    
    // Object Level
    TH1F*     nAllJets;
    TH1F*     nSelJets;
    jetHists*  allJets;
    jetHists*  selJets;

    TH1F* nAllMuons;
    TH1F* nIsoMuons;
    muonHists* allMuons;
    muonHists* isoMuons;

    eventHists(std::string, fwlite::TFileService&, bool isMC = false);
    void Fill(eventData*);
    ~eventHists(); 

  };

}
#endif // eventHists_H
