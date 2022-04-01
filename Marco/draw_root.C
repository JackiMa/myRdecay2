#include <cstring>
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH2.h"

// void draw_root(std::string fileName){
//     TFile* f1 = new TFile(fileName.c_str(),"Read");
//     TCanvas* c1 = new TCanvas("Time_Engery","Time_Engery");
//     Time_Energy->Draw("COLZ");
//     c1->SaveAs("test.pdf");
// }

void draw_root(std::string crystalName){
    TFile* f1 = new TFile((crystalName+".root").c_str(),"Read");
    TCanvas* c1 = new TCanvas("Time_Engery", "Time_Engery",797,215,1098,500);
    auto Time_Energy =static_cast<TH2D *> (f1->Get("Time_Energy"));
    c1->SetLogz();
    gStyle->SetOptStat(0); // remove TPaveStats objects
    
    Time_Energy->Draw("COLZ");
    c1->SaveAs((crystalName+"_Full.pdf").c_str());

    Time_Energy->Rebin2D(3,3,"100")->Draw("COLZ");
    c1->SaveAs((crystalName+"_Rebin_Full.pdf").c_str());
    
}