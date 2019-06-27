#include "AnaExample/nTupleAnalysis/interface/eventData.h"

using namespace nTupleAnalysis;


eventData::eventData(TChain* t, bool mc, std::string y, bool d){
  std::cout << "eventData::eventData()" << std::endl;
  tree  = t;
  isMC  = mc;
  year  = y;
  debug = d;

  //std::cout << "eventData::eventData() tree->Lookup(true)" << std::endl;
  //tree->Lookup(true);
  std::cout << "eventData::eventData() tree->LoadTree(0)" << std::endl;
  tree->LoadTree(0);
  initBranch(tree, "run",             run);
  initBranch(tree, "luminosityBlock", lumiBlock);
  initBranch(tree, "event",           event);
  initBranch(tree, "PV_npvs",         nPVs);
  initBranch(tree, "PV_npvsGood",     nPVsGood);
  if(isMC){
    initBranch(tree, "genWeight", genWeight);
  }
  
  //
  // HT Triggers
  //
  for(std::string trigName : HLT_PFHT_Names){
    HLT_PFHT_Results.insert(std::make_pair(trigName, Bool_t()));
    initBranch(tree, trigName, HLT_PFHT_Results[trigName]);
  }

  //
  // PFJet Triggers
  //
  for(std::string trigName : HLT_PFJet_Names){
    HLT_PFJet_Results.insert(std::make_pair(trigName, Bool_t()));
    initBranch(tree, trigName, HLT_PFJet_Results[trigName]);
  }

  //
  // DiPFJet Triggers
  //
  for(std::string trigName : HLT_DiPFJet_Names){
    HLT_DiPFJet_Results.insert(std::make_pair(trigName, Bool_t()));
    initBranch(tree, trigName, HLT_DiPFJet_Results[trigName]);
  }

  //
  // QuadPFJet Triggers
  //
  for(std::string trigName : HLT_QuadPFJet_Names){
    HLT_QuadPFJet_Results.insert(std::make_pair(trigName, Bool_t()));
    initBranch(tree, trigName, HLT_QuadPFJet_Results[trigName]);
  }

  //
  // Unprescaled Triggers
  //
  for(std::string trigName : HLT_Unprescaled_Names){
    HLT_Unprescaled_Results.insert(std::make_pair(trigName, Bool_t()));
    initBranch(tree, trigName, HLT_Unprescaled_Results[trigName]);
  }


//  //
//  // L1 Triggers
//  //
//  for(std::string trigName : L1_Names){
//    L1_Results.insert(std::make_pair(trigName, Bool_t()));
//    initBranch(tree, trigName, L1_Results[trigName]);
//  }

  
  std::cout << "eventData::eventData() Initialize jets and muons" << std::endl;
  treeJets  = new jetData ("Jet",      tree);
  treeMuons = new muonData("Muon",     tree);
  //treeTrig  = new trigData("TrigObj",  tree);
} 


void eventData::update(int e){
  //if(e>2546040) debug = true;
  if(debug){
    std::cout<<"Get Entry "<<e<<std::endl;
    std::cout<<tree->GetCurrentFile()->GetName()<<std::endl;
    tree->Show(e);
  }
  Long64_t loadStatus = tree->LoadTree(e);
  if(loadStatus<0){
    std::cout << "Error "<<loadStatus<<" getting event "<<e<<std::endl; 
    return;
  }
  tree->GetEntry(e);
  if(debug) std::cout<<"Got Entry "<<e<<std::endl;

  if(debug) std::cout<<"Reset eventData"<<std::endl;


  passHLT = false;

  //
  //  HT Triggers
  //
  for(std::string trigName : HLT_PFHT_Names){
    if(HLT_PFHT_Results[trigName]){
      if(debug) std::cout << "\tPass trig " << trigName << " : " << HLT_PFHT_Results[trigName] << std::endl;;
      passHLT = true;
    }
  }

  //
  //  PFJet Triggers
  //
  for(std::string trigName : HLT_PFJet_Names){
    if(HLT_PFJet_Results[trigName]){
      if(debug) std::cout << "\tPass trig " << trigName << " : " << HLT_PFJet_Results[trigName] << std::endl;;
      passHLT = true;
    }
  }

  //
  //  Di-PFJet Triggers
  //
  for(std::string trigName : HLT_DiPFJet_Names){
    if(HLT_DiPFJet_Results[trigName]){
      if(debug) std::cout << "\tPass trig " << trigName << " : " << HLT_DiPFJet_Results[trigName] << std::endl;;
      passHLT = true;
    }
  }


  //
  //  Quad-PFJet Triggers
  //
  for(std::string trigName : HLT_QuadPFJet_Names){
    if(HLT_QuadPFJet_Results[trigName]){
      if(debug) std::cout << "\tPass trig " << trigName << " : " << HLT_QuadPFJet_Results[trigName] << std::endl;;
      passHLT = true;
    }
  }


//  //
//  //  Unprescaled Triggers
//  //
//  for(std::string trigName : HLT_Unprescaled_Names){
//    if(HLT_Unprescaled_Results[trigName]){
//      if(debug) std::cout << "\tPass trig " << trigName << " : " << HLT_Unprescaled_Results[trigName] << std::endl;;
//      passHLT = true;
//    }
//  }


//  //
//  //  L1 Triggers
//  //
//  for(std::string trigName : L1_Names){
//    if(L1_Results[trigName]){
//      std::cout << "\tPass trig " << trigName << " : " << L1_Results[trigName] << std::endl;;
//    }
//  }



  //Objects
  if(debug) std::cout << "Get Jets\n";
  allJets = treeJets->getJets(20);
  selJets = treeJets->getJets(allJets, jetPtMin, 1e6, jetEtaMax, doJetCleaning);
  
  if(debug) std::cout << "Get Muons\n";
  allMuons = treeMuons->getMuons();
  isoMuons = treeMuons->getMuons(40, 2.4, 2, true);

//  allTrigJets    = treeTrig->getTrigs(0,1e6,1);
//  allTrigFatJets = treeTrig->getTrigs(0,1e6,6);
//  allTrigElecs   = treeTrig->getTrigs(0,1e6,11);
//  allTrigPhotons = treeTrig->getTrigs(0,1e6,22);
//  allTrigTaus    = treeTrig->getTrigs(0,1e6,15);
//  allTrigHT      = treeTrig->getTrigs(0,1e6,3);
  //std::cout << "L1 Jets size:: " << allTriggerJets.size() << std::endl;

  ht = 0;
  ht30 = 0;
  for(auto &jet: allJets){
    if(fabs(jet->eta) < 2.5){
      ht += jet->pt;
      if(jet->pt > 30){
	ht30 += jet->pt;
      }
    }
  }


//  L1ht = 0;
//  L1ht30 = 0;
//  HLTht = 0;
//  HLTht30 = 0;
//  for(auto &trigjet: allTrigJets){
//    if(fabs(trigjet->eta) < 2.5){
//      L1ht += trigjet->l1pt;
//      HLTht += trigjet->pt;
//      if(trigjet->l1pt > 30){
//	L1ht30 += trigjet->l1pt;
//      }
//      if(trigjet->pt > 30){
//	HLTht30 += trigjet->pt;
//      }
//    }
//  }



  if(debug) std::cout<<"eventData updated\n";
  return;
}




void eventData::dump(){

  std::cout << "   Run: " << run    << std::endl;
  std::cout << " Event: " << event  << std::endl;  
  std::cout << "Weight: " << weight << std::endl;
  std::cout << " allJets: " << allJets .size() << " |  selJets: " << selJets .size() << std::endl;
  std::cout << "allMuons: " << allMuons.size() << " | isoMuons: " << isoMuons.size() << std::endl;

  return;
}

eventData::~eventData(){} 

