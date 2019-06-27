
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
    if(event->HLT_PFHT_Results[trigName])
      passTrig->Fill(trigName,event->weight);
  }

  //
  //  PFJet Triggers
  //
  for(std::string trigName : event->HLT_PFJet_Names){
    if(event->HLT_PFJet_Results[trigName])
      passTrig->Fill(trigName,event->weight);
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

