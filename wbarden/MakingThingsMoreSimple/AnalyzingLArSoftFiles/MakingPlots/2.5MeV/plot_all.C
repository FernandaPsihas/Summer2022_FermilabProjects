void plot_all(){

  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);

  TH1F* h = new TH1F("h", ";Dopant Quantum Efficiency [%]; Energy Resolution at 2.5 MeV [%]; ", 1, -5, 110);
  h->GetXaxis()->SetLabelSize(0.04);
  h->GetYaxis()->SetLabelSize(0.04);
  h->SetMaximum(5.9);
  h->SetMinimum(0.1);
  h->Draw();

  Double_t x[5]  = {100, 75, 50, 25, 0};
  Double_t y[5]  = {1.35739,1.75182,2.38309,3.27365,4.96879};
  Double_t ex[5] = {0, 0, 0, 0, 0};
  Double_t ey[5] = {0.00196212,0.00228083,0.00251136,0.00320658,0.00671471};

  auto gr = new TGraphErrors(5,x,y,ex,ey);
  gr->SetName("gr");
  gr->SetMarkerColor(kBlue+1);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(2);


  TF1* f = new TF1("f","pol0(0)+expo(1)", 0, 100);
  f->SetLineStyle(2);
  f->SetLineColor(kRed-3);
  gr->Fit("f","R");
  gr->Draw("p same");

  //  gr->Draw("A same");

  TLatex latex;
  latex.SetTextSize(0.04);
  latex.SetTextFont(42);
  latex.DrawLatex(75,5.4,Form("#splitline{Fitted Function,}{y = %4.1f + e^{%4.1f + %4.2f x }}",
			     f->GetParameter(0),
			     f->GetParameter(1),
			     f->GetParameter(2)) );

}
