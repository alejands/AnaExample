
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <TROOT.h>
#include <boost/bind.hpp>

#include "AnaExample/nTupleAnalysis/interface/trigStudy.h"

using namespace nTupleAnalysis;

trigStudy::trigStudy(TChain* _events, TChain* _runs, TChain* _lumiBlocks, fwlite::TFileService& fs, bool _isMC, bool _blind, std::string _year, int _histogramming, bool _debug){
  if(_debug) std::cout<<"In trigStudy constructor"<<std::endl;
  debug      = _debug;
  isMC       = _isMC;
  blind      = _blind;
  year       = _year;
  events     = _events;
  events->SetBranchStatus("*", 0);
  runs       = _runs;
  histogramming = _histogramming;

  //Calculate MC weight denominator
  if(isMC){
    runs->SetBranchStatus("*", 0);
    runs->LoadTree(0);
    initBranch(runs, "genEventCount", genEventCount);
    initBranch(runs, "genEventSumw",  genEventSumw);
    initBranch(runs, "genEventSumw2", genEventSumw2);
    for(int r = 0; r < runs->GetEntries(); r++){
      runs->GetEntry(r);
      mcEventCount += genEventCount;
      mcEventSumw  += genEventSumw;
      mcEventSumw2 += genEventSumw2;
    }
    std::cout << "mcEventCount " << mcEventCount << " | mcEventSumw " << mcEventSumw << std::endl;
  }

  lumiBlocks = _lumiBlocks;
  event      = new eventData(events, isMC, year, debug);
  treeEvents = events->GetEntries();

  // hists
  if(histogramming >= 0) allEvents     = new eventHists("allEvents",     fs);

  std::string name = "trigStudy";
  TFileDirectory dir = fs.mkdir(name);
  hOffJetPt         = dir.make<TH1F>("offJetPt",         (name+"/offJetPt; Offline JetPt; Entries").c_str(),  100,0,200);
  hOffJetPt_l       = dir.make<TH1F>("offJetPt_l",         (name+"/offJetPt_l; Offline JetPt; Entries").c_str(),  100,0,1000);
  hOffJetPt_matched  = dir.make<TH1F>("offJetPt_matched", (name+"/offJetPt_l_matched; Offline JetPt; Entries").c_str(),  100,0,200);
  hOffJetPt_l_matched = dir.make<TH1F>("offJetPt_l_matched", (name+"/offJetPt_l_matched; Offline JetPt; Entries").c_str(),  100,0,1000);
  hOffJetDeltaR     = dir.make<TH1F>("offJetDeltaR",     (name+"/offJetDetltaR; Offline DeltaR; Entries").c_str(),  50,0,5);

  cutflow    = new nTupleAnalysis::cutflowHists("cutflow", fs);
  cutflow->AddCut("all");
  cutflow->AddCut("lumiMask");
  cutflow->AddCut("HLTOR");


  passTrig    = new nTupleAnalysis::cutflowHists("passTrig", fs);
  passTrig->AddCut("HLTOR");  

  //
  //  HT Triggers
  //
  for(std::string trigName : event->HLT_PFHT_Names){
    passTrig->AddCut(trigName);
  }

  //
  //  PFJet Triggers
  //
  for(std::string trigName : event->HLT_PFJet_Names){
    passTrig->AddCut(trigName);
  }

  //
  //  Di-PFJet Triggers
  //
  for(std::string trigName : event->HLT_DiPFJet_Names){
    passTrig->AddCut(trigName);
  }


  //
  //  Quad-PFJet Triggers
  //
  for(std::string trigName : event->HLT_QuadPFJet_Names){
    passTrig->AddCut(trigName);
  }

  


  //
  //  For testing
  // 
  //  All seeded from: HLT_PFJet40
  trigEmulator_PF40 = new TriggerEmulator::TrigEmulatorTool("trigEmulator_PF40");
  trigEmulator_PF40->AddTrig("EMU_PFJet60",   {"60" }, {1} );
  trigEmulator_PF40->AddTrig("EMU_PFJet80",   {"80" }, {1} );
  trigEmulator_PF40->AddTrig("EMU_PFJet140",  {"140"}, {1} );
  trigEmulator_PF40->AddTrig("EMU_PFJet200",  {"200"}, {1} );
  trigEmulator_PF40->AddTrig("EMU_PFJet260",  {"260"}, {1} );
  trigEmulator_PF40->AddTrig("EMU_PFJet320",  {"320"}, {1} );
  trigEmulator_PF40->AddTrig("EMU_PFJet400",  {"400"}, {1} );
  trigEmulator_PF40->AddTrig("EMU_PFJet450",  {"450"}, {1} );

  //
  // Emulation  of quad triggers
  //   All seeded from "HLT_PFJet80"
  trigEmulator_PF80 = new TriggerEmulator::TrigEmulatorTool("trigEmulator_PF80");
  trigEmulator_PF80->AddTrig("EMU_QuadPFJet103_88_75_15", {"105","90","75","15"}, {1,2,3,4} );
  trigEmulator_PF80->AddTrig("EMU_QuadPFJet105_88_76_15", {"105","90","75","15"}, {1,2,3,4} );
  trigEmulator_PF80->AddTrig("EMU_QuadPFJet110_90_80_15", {"110","90","80","15"}, {1,2,3,4} );


  trigEmulator_PFHT180 = new TriggerEmulator::TrigEmulatorTool("trigEmulator_PFHT180");
  trigEmulator_PFHT180->AddTrig("EMU_PFHT180",  "180",  {}, {} );
  trigEmulator_PFHT180->AddTrig("EMU_PFHT250",  "250",  {}, {} );
  trigEmulator_PFHT180->AddTrig("EMU_PFHT370",  "370",  {}, {} );
  trigEmulator_PFHT180->AddTrig("EMU_PFHT430",  "430",  {}, {} );
  trigEmulator_PFHT180->AddTrig("EMU_PFHT510",  "510",  {}, {} );
  trigEmulator_PFHT180->AddTrig("EMU_PFHT590",  "590",  {}, {} );
  trigEmulator_PFHT180->AddTrig("EMU_PFHT680",  "680",  {}, {} );
  trigEmulator_PFHT180->AddTrig("EMU_PFHT780",  "780",  {}, {} );
  trigEmulator_PFHT180->AddTrig("EMU_PFHT890",  "890",  {}, {} );
  trigEmulator_PFHT180->AddTrig("EMU_PFHT1050", "1050", {}, {} );

  //trigEmulator_PF80->AddTrig("EMU_QuadPFJet105_88_76_15", {"105","90","75","15"}, {1,2,3,4} );
  //trigEmulator_PF80->AddTrig("EMU_QuadPFJet110_90_80_15", {"110","90","80","15"}, {1,2,3,4} );


  //emulatedTrigMenu.insert(std::make_pair("EMU_QuadPFJet103_88_75_15",new trigEmulator("EMU_QuadPFJet103_88_75_15", "HLT_PFJet80", {PFJet["105"],PFJet["90"],PFJet["75"],PFJet["15"]}, {1,2,3,4} )));
  //emulatedTrigMenu.insert(std::make_pair("EMU_QuadPFJet103_88_75_15",new trigEmulator("EMU_QuadPFJet103_88_75_15", "HLT_PFJet80", {PFJet["105"],PFJet["90"],PFJet["75"],PFJet["15"]}, {1,2,3,4} )));
  //emulatedTrigMenu.insert(std::make_pair("EMU_QuadPFJet105_88_76_15",new trigEmulator("EMU_QuadPFJet105_88_76_15", "HLT_PFJet80", {PFJet["105"],PFJet["90"],PFJet["75"],PFJet["15"]}, {1,2,3,4} )));
  //emulatedTrigMenu.insert(std::make_pair("EMU_QuadPFJet110_90_80_15",new trigEmulator("EMU_QuadPFJet110_90_80_15", "HLT_PFJet80", {PFJet["110"],PFJet["90"],PFJet["80"],PFJet["15"]}, {1,2,3,4} )));



//    HLT_QuadPFJet111_90_80_15 rate 0.509 PS= 512.1 HLT unprescaled rate 260.652 L1_PS 1.0 unprescaled rate 260.659
//    HLT_PFHT350MinPFJet15 rate 0.3 PS= 3054.4 HLT unprescaled rate 916.324 L1_PS 1.0 unprescaled rate 916.337
//    HLT_PFHT330PT30_QuadPFJet_75_60_45_40 rate 1.651 PS= 128.0 HLT unprescaled rate 211.373 L1_PS 1.0 unprescaled rate 211.376
//    HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5 rate 3.766 PS= 1.0 HLT unprescaled rate 3.766 L1_PS 1.0 unprescaled rate 3.766
//    HLT_PFHT400_FivePFJet_100_100_60_30_30 rate 1.067 PS= 40.0 HLT unprescaled rate 42.679 L1_PS 1.0 unprescaled rate 42.679
//    HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5 rate 4.714 PS= 1.0 HLT unprescaled rate 4.714 L1_PS 1.0 unprescaled rate 4.714
//    HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5 rate 6.561 PS= 1.0 HLT unprescaled rate 6.561 L1_PS 1.0 unprescaled rate 6.561
    
									


} 

void trigStudy::createPicoAOD(std::string fileName){
  writePicoAOD = true;
  picoAODFile = TFile::Open(fileName.c_str() , "RECREATE");
  picoAODEvents     = events    ->CloneTree(0);
  picoAODRuns       = runs      ->CloneTree();
  picoAODLumiBlocks = lumiBlocks->CloneTree();
}

void trigStudy::addDerivedQuantitiesToPicoAOD(){
  // could add fancy vars here.
  return;
}

void trigStudy::storePicoAOD(){
  picoAODFile->Write();
  picoAODFile->Close();
  return;
}

void trigStudy::monitor(long int e){
  //Monitor progress
  percent        = (e+1)*100/nEvents;
  duration       = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  eventRate      = (e+1)/duration;
  timeRemaining  = (nEvents-e)/eventRate;
  minutes = static_cast<int>(timeRemaining/60);
  seconds = static_cast<int>(timeRemaining - minutes*60);
  getrusage(who, &usage);
  usageMB = usage.ru_maxrss/1024;
  //print status and flush stdout so that status bar only uses one line
  if(isMC){
    fprintf(stdout, "\rProcessed: %8li of %li ( %2li%% | %.0f events/s | done in %02i:%02i | memory usage: %li MB)       ", 
	                          e+1, nEvents, percent,   eventRate,    minutes, seconds,                usageMB);
  }else{
    fprintf(stdout, "\rProcessed: %8li of %li ( %2li%% | %.0f events/s | done in %02i:%02i | memory usage: %li MB | LumiBlocks %i | Est. Lumi %.2f/fb )       ", 
 	                          e+1, nEvents, percent,   eventRate,    minutes, seconds,                usageMB,            nls,         intLumi/1000 );    
  }
  fflush(stdout);
}

int trigStudy::eventLoop(int maxEvents){

  //Set Number of events to process. Take manual maxEvents if maxEvents is > 0 and less than the total number of events in the input files. 
  nEvents = (maxEvents > 0 && maxEvents < treeEvents) ? maxEvents : treeEvents;
  
  std::cout << "\nProcess " << nEvents << " of " << treeEvents << " events.\n";

  start = std::clock();//2546000 //2546043
  for(long int e = 0; e < nEvents; e++){

    event->update(e);    
    processEvent();
    if(debug) event->dump();

    //periodically update status
    if( (e+1)%10000 == 0 || e+1==nEvents || debug) 
      monitor(e);

  }


  trigEmulator_PF40->dumpResults();
  trigEmulator_PF80->dumpResults();  
  trigEmulator_PFHT180->dumpResults();



  std::cout << std::endl;
  if(!isMC) std::cout << "Runs " << firstRun << "-" << lastRun << std::endl;

  minutes = static_cast<int>(duration/60);
  seconds = static_cast<int>(duration - minutes*60);
                                        
  if(isMC){
    fprintf(stdout,"---------------------------\nProcessed in %02i:%02i", minutes, seconds);
  }else{
    fprintf(stdout,"---------------------------\nProcessed %.2f/fb in %02i:%02i", intLumi/1000, minutes, seconds);
  }
  return 0;
}

int trigStudy::processEvent(){
  if(debug) std::cout << "processEvent start" << std::endl;
  if(isMC){
    event->weight = event->genWeight * (lumi * xs * kFactor / mcEventSumw);
    if(debug) std::cout << "event->genWeight * (lumi * xs * kFactor / mcEventSumw) = " << event->genWeight << " * (" << lumi << " * " << xs << " * " << kFactor << " / " << mcEventSumw << ") = " << event->weight << std::endl;
  }
  cutflow->Fill("all", event->weight);

  //
  //if we are processing data, first apply lumiMask and trigger
  //
  if(!isMC){
    if(!passLumiMask()){
      if(debug) std::cout << "Fail lumiMask" << std::endl;
      return 0;
    }
    cutflow->Fill("lumiMask", event->weight);

    //keep track of total lumi
    countLumi();

    if(!event->passHLT){
      if(debug) std::cout << "Fail HLT: data" << std::endl;
      return 0;
    }
    cutflow->Fill("HLTOR", event->weight);
  }
  if(allEvents != NULL && event->passHLT) allEvents->Fill(event);

  passTrig->Fill("all",event->weight);
  if(event->passHLT) passTrig->Fill("HLTOR",event->weight);
    
  //
  //  HT Triggers
  //
  for(std::string trigName : event->HLT_PFHT_Names){
    if(event->HLT_PFHT_Results[trigName]){
      passTrig->Fill(trigName,event->weight);
    }
  }

  //
  //  PFJet Triggers
  //
  for(std::string trigName : event->HLT_PFJet_Names){
    if(event->HLT_PFJet_Results[trigName]){
      passTrig->Fill(trigName,event->weight);
    }
  }

  //
  //  Di-PFJet Triggers
  //
  for(std::string trigName : event->HLT_DiPFJet_Names){
    if(event->HLT_DiPFJet_Results[trigName])
      passTrig->Fill(trigName,event->weight);
  }


  //
  //  Quad-PFJet Triggers
  //
  for(std::string trigName : event->HLT_QuadPFJet_Names){
    if(event->HLT_QuadPFJet_Results[trigName])
      passTrig->Fill(trigName,event->weight);
  }


  //
  // Do the emulation
  //
  //if(event->HLT_PFJet_Results["HLT_PFJet40"])
  //  trigEmulator_PF40->Fill(event->allJets);

  //if(event->HLT_PFJet_Results["HLT_PFJet80"])
  //  trigEmulator_PF80->Fill(event->allJets);

  //if(event->HLT_PFHT_Results["HLT_PFHT180"])
  //  trigEmulator_PFHT180->Fill({}, event->ht);



  bool dumpEvents = false;

  if(dumpEvents){

    for(std::string trigName : event->HLT_PFJet_Names){
      if(event->HLT_PFJet_Results[trigName]){
	std::cout << "\tPass trig " << trigName << " : " << event->HLT_PFJet_Results[trigName] << std::endl;;
      }
    }


    for(const jetPtr &offjet: event->allJets) {
      std::cout << " \t\t offJet pt/eta/phi " << offjet->pt << "/" << offjet->eta << "/" << offjet->phi << std::endl;
    }

    //for(const trigPtr &trigjet: event->allTrigJets) {
    //  std::cout << " \t\t\t trigJet pt/eta/phi " << trigjet->pt << "/" << trigjet->eta << "/" << trigjet->phi << std::endl;
    //}
    //
    //for(const trigPtr &trigjet: event->allTrigFatJets) {
    //  std::cout << " \t\t\t trigFatJet pt/eta/phi " << trigjet->pt << "/" << trigjet->eta << "/" << trigjet->phi << std::endl;
    //}
    //
    //for(const trigPtr &trig: event->allTrigElecs) {
    //  std::cout << " \t\t\t trigElec pt/eta/phi " << trig->pt << "/" << trig->eta << "/" << trig->phi << std::endl;
    //}
    //
    //for(const trigPtr &trig: event->allTrigPhotons) {
    //  std::cout << " \t\t\t trigPhoton pt/eta/phi " << trig->pt << "/" << trig->eta << "/" << trig->phi << std::endl;
    //}
    //
    //for(const trigPtr &trig: event->allTrigTaus) {
    //  std::cout << " \t\t\t trigTau pt/eta/phi " << trig->pt << "/" << trig->eta << "/" << trig->phi << std::endl;
    //}
    //
    //for(const trigPtr &trig: event->allTrigHT) {
    //  std::cout << " \t\t\t trigHT pt/eta/phi " << trig->pt << "/" << trig->eta << "/" << trig->phi << std::endl;
    //}
  }

  if(writePicoAOD) picoAODEvents->Fill();  

  return 0;
}

bool trigStudy::passLumiMask(){
  // if the lumiMask is empty, then no JSON file was provided so all
  // events should pass
  if(lumiMask.empty()) return true;


  //make lumiID run:lumiBlock
  edm::LuminosityBlockID lumiID(event->run, event->lumiBlock);

  //define function that checks if a lumiID is contained in a lumiBlockRange
  bool (*funcPtr) (edm::LuminosityBlockRange const &, edm::LuminosityBlockID const &) = &edm::contains;

  //Loop over the lumiMask and use funcPtr to check for a match
  std::vector< edm::LuminosityBlockRange >::const_iterator iter = std::find_if (lumiMask.begin(), lumiMask.end(), boost::bind(funcPtr, _1, lumiID) );

  return lumiMask.end() != iter; 
}

void trigStudy::getLumiData(std::string fileName){
  std::cout << "Getting integrated luminosity estimate per lumiBlock from: " << fileName << std::endl;
  brilCSV brilFile(fileName);
  lumiData = brilFile.GetData();
}

void trigStudy::countLumi(){
  if(event->lumiBlock != prevLumiBlock || event->run != prevRun){
    if(event->run != prevRun){
      if(event->run < firstRun) firstRun = event->run;
      if(event->run >  lastRun)  lastRun = event->run;
    }
    prevLumiBlock = event->lumiBlock;
    prevRun       = event->run;
    edm::LuminosityBlockID lumiID(event->run, event->lumiBlock);
    intLumi += lumiData[lumiID];//convert units to /fb
    //std::cout << lumiID << " " << lumiData[lumiID] << " " << intLumi << " \n";
    nls   += 1;
    nruns += 1;
  }
  return;
}


trigStudy::~trigStudy(){} 

