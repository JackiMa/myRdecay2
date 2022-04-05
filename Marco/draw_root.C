#include <cstring>
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"
#include "TStyle.h"
#include "TLegend.h"


void draw_root(std::string crystalName){
    TFile* f1 = new TFile((crystalName+".root").c_str(),"Read");
    TCanvas* c1 = new TCanvas("Time_Engery", "Time_Engery",797,215,1098,500);
    auto Time_Energy =static_cast<TH2D *> (f1->Get("Time_Energy"));
    c1->SetLogz();
    gStyle->SetOptStat(0); // remove TPaveStats objects
    
    Time_Energy->Draw("COLZ");
    c1->SaveAs((crystalName+"_Full.pdf").c_str());

    TH1D* prjX511 = Time_Energy->ProjectionX("511_prj",52,52); // 第52道恰好是包含511keV的这一道
    prjX511->Draw("hist");
    prjX511->SetNameTitle(crystalName.c_str(),"511keV");
    prjX511->SetLineColor(kBlue);
    TH1D* prjXAll = Time_Energy->ProjectionX("All_prj",1,-1); // 全部的计数
    prjXAll->Draw("hist,same");
    prjXAll->SetNameTitle(crystalName.c_str(),"TotalEng");
    prjXAll->SetLineColor(kRed);
    c1->BuildLegend(0.8,0.7,0.9,0.9,"Timeprj");     // 投影到时间上画图
    c1->SaveAs((crystalName+"_TimePrj.pdf").c_str());

    Time_Energy->Rebin2D(10,10,"100")->Draw("COLZ"); // 10合一
    c1->SaveAs((crystalName+"_Rebin_Full.pdf").c_str());
    
    delete c1;
}

// root -l draw_root.C 
// void draw_root(){
//     std::string path = "../build/";
//     std::string crystalName = "GAGG";
//     TFile* f1 = new TFile((path+crystalName+".root").c_str(),"Read");
//     TCanvas* c1 = new TCanvas("Time_Engery", "Time_Engery",797,215,1098,500);
//     auto Time_Energy =static_cast<TH2D *> (f1->Get("Time_Energy"));
//     c1->SetLogz();
//     gStyle->SetOptStat(0); // remove TPaveStats objects
    
//     Time_Energy->Draw("COLZ");
//     c1->SaveAs((crystalName+"_Full.pdf").c_str());

//     TH1D* prjX511 = Time_Energy->ProjectionX("511_prj",52,52); // 第52道恰好是包含511keV的这一道
//     prjX511->Draw("hist");
//     prjX511->SetNameTitle(crystalName.c_str(),"511keV");
//     prjX511->SetLineColor(kBlue);
//     TH1D* prjXAll = Time_Energy->ProjectionX("All_prj",1,-1); // 全部的计数
//     prjXAll->Draw("hist,same");
//     prjXAll->SetNameTitle(crystalName.c_str(),"TotalEng");
//     prjXAll->SetLineColor(kRed);
    
//     c1->BuildLegend(0.8,0.7,0.9,0.9,"Timeprj");
//     c1->SaveAs((crystalName+"_TimePrj.pdf").c_str());

//     // Time_Energy->Rebin2D(3,3,"100")->Draw("COLZ");
//     // c1->SaveAs((crystalName+"_Rebin_Full.pdf").c_str());
    
// }