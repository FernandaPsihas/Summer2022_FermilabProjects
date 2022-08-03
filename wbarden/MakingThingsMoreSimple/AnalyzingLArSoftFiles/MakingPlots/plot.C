void plot(){

  gStyle->SetOptStat(0);

  TF1* f = new TF1("f","[0] + ([1] - [2])/(1 + pow(x/[3],[4]))",0,100);  
  f->SetParameter(0,-0.213431);
  f->SetParameter(1,6.74848);
  f->SetParameter(2,0.373372);
  f->SetParameter(3,4.87814);
  f->SetParameter(4,0.642656);
  f->SetLineStyle(2);
  f->SetLineColor(kRed);

  TCanvas *c1 = new TCanvas("c1","c1",200,10,1300,1300);
  TPad *pad1 = new TPad("pad1","",0,0,1,1);
  TPad *pad2 = new TPad("pad2","",0,0,1,1);
  pad2->SetFillColor(0);
  pad2->SetFillStyle(0);
  pad2->SetFrameFillStyle(0);

  pad1->Draw();
  pad1->cd();  

 
  TH1F* h = new TH1F("h", ";Light Collection Efficiency [%]; Energy Resolution at 1 MeV [%]; ", 1, 0, 100);
  TH1F* h2 = new TH1F("h2", ";Light Collection Efficiency [#gamma / MeV];", 1, ((0/100)*30000), ((100/100)*30000));
  h->GetXaxis()->SetLabelSize(0.025);
  h->SetMaximum(5.05);
  h->SetMinimum(0.51);
  h->Draw();
  f->Draw("same");
 
  pad2->Draw();
  pad2->cd();
  h2->GetYaxis()->SetLabelSize(0.);
  h2->GetXaxis()->SetLabelSize(0.025);
  h2->GetYaxis()->SetTickSize(0.);
  h2->Draw("X+");

 

}
