void plot(){

  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendTextSize(0.05);


  std::vector<int> color = {kBlack,
			    kViolet+5,
			    kTeal-5};

  std::vector<std::string> files = { "undoped"
				     ,"doped_25percent"
				     //,"doped_50percent"
				     //,"doped_75percent"
				     ,"doped"};
  std::vector<TGraphErrors*> gr;
  gr.resize(files.size());

  const int Ebins = 20;
  const int filesize = files.size();
  double eres[filesize][Ebins];  
  double eres_err[filesize][Ebins];
  double ebin_center[filesize][Ebins];
  double ebin_width[filesize][Ebins];

  TCanvas* c = new TCanvas("c");
  c->cd();
  TH1F* h = new TH1F("h", ";Energy Deposited [MeV]; Energy Resolution [%]; ", 1, 0, 10);
  h->GetXaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetLabelSize(0.05);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetTitleOffset(0.8);
  h->GetYaxis()->SetTitleOffset(0.7);
  h->SetMaximum(11);
  h->SetMinimum(0);
  h->Draw();
  
  TCanvas* c2 = new TCanvas("c2");
  for(int i = 0; i < files.size(); i++){
    
    TFile* f = new TFile(Form("../../low_energy_electrons_%s.root",files[i].c_str()));
    TTree* t = (TTree*)f->Get("EventTree");
    std::vector<TH1D*> hists;
    hists.resize(Ebins);

    for(int e = 0; e < Ebins; e++){
      c2->cd();
      double elow = 10.*e/Ebins;
      double ehigh = 10.*(e+1)/(Ebins);
      
      ebin_center[i][e] = elow+((ehigh-elow)/2);
      ebin_width[i][e] = (ehigh-elow)/2;

      hists[i] = new TH1D(Form("hists_%d_%s",e,files[i].c_str()),"",1000,0,2000);

      t->Project(Form("hists_%d_%s",e,files[i].c_str()),
		 "reco_tot_wire_Q/true_Edep",
		 Form("true_Edep > %f && true_Edep < %f && true_Edep > 0 && reco_tot_wire_Q > 0",
		      elow,
		      ehigh));
      
      double hist_max = hists[i]->GetMaximum();
      int hist_x_max = hists[i]->GetMaximumBin();
      int hist_binl = hists[i]->FindFirstBinAbove(hists[i]->GetMaximum()/2);
      int hist_binh = hists[i]->FindLastBinAbove(hists[i]->GetMaximum()/2);

      double hist_fwhm_high = hists[i]->GetBinCenter(hist_binh);
      double hist_fwhm_low = hists[i]->GetBinCenter(hist_binl);
           
      //      hists[i]->Draw();
      TF1* f = new TF1(Form("hists_%d_%s",e,files[i].c_str()), "gaus", hist_fwhm_low, hist_fwhm_high);     
      hists[i]->Fit(Form("hists_%d_%s",e,files[i].c_str()),"R");

      double sigma = f->GetParameter(2);
      double mu = f->GetParameter(1);
      double err_sigma = f->GetParError(2);
      double err_mu = f->GetParError(1);

      eres[i][e] = 100*sigma/mu;
      eres_err[i][e] = 100*sqrt(pow(sigma/mu,2)*(pow(err_sigma/sigma,2)+pow(err_mu/mu,2)));
      
    }
    c->cd();
    gr[i] = new TGraphErrors(Ebins,ebin_center[i],eres[i],ebin_width[i],eres_err[i]);
    gr[i]->SetName(Form("gr_%s",files[i].c_str()));
    gr[i]->SetLineWidth(2);
    gr[i]->SetMarkerStyle(21);
    gr[i]->SetLineColor(color[i]);
    gr[i]->SetMarkerColor(color[i]);
    gr[i]->Draw("p same");    
  }

  TLine* l1 = new TLine(0,1,10,1);
  l1->SetLineStyle(2);
  l1->SetLineWidth(2);
  l1->SetLineColor(kGreen+3);
  l1->Draw("same");

  TLatex latex1;
  latex1.SetTextSize(0.04);
  latex1.SetTextFont(62);
  latex1.SetTextColor(kGreen+3);
  latex1.DrawLatex(0.2,1.1, "1%");

  TLine* l2 = new TLine(0,2,10,2);
  l2->SetLineStyle(2);
  l2->SetLineWidth(2);
  l2->SetLineColor(kOrange+2);
  l2->Draw("same");

  TLatex latex2;
  latex2.SetTextSize(0.04);
  latex2.SetTextFont(62);
  latex2.SetTextColor(kOrange+2);
  latex2.DrawLatex(0.2,2.1, "2%");

  TLine* l5 = new TLine(0,5,10,5);
  l5->SetLineStyle(2);
  l5->SetLineWidth(2);
  l5->SetLineColor(kRed+1);
  l5->Draw("same");

  TLatex latex5;
  latex5.SetTextSize(0.04);
  latex5.SetTextFont(62);
  latex5.SetTextColor(kRed+1);
  latex5.DrawLatex(0.2,5.1, "5%");


  TLegend* leg = new TLegend(0.3, 0.65, 0.85, 0.88);
  leg->AddEntry(Form("gr_%s",files[0].c_str()), "Pure Liquid Argon", "pl");
  leg->AddEntry(Form("gr_%s",files[1].c_str()), "with P.S. Dopant,  25% Q.E.", "pl");
  leg->AddEntry(Form("gr_%s",files[2].c_str()), "with P.S. Dopant, 100% Q.E.", "pl");
  leg->Draw("same");

  TArrow* t1 = new TArrow(0.33,9.85, 0.33, 10.95,0.01,"|-|>");
  //  t1->SetAngle(40);
  t1->SetLineWidth(2);
  t1->SetLineColor(kBlack);
  t1->Draw();

  TArrow* t2 = new TArrow(0.17,9.85, 0.17, 10.95,0.01,"|-|>");
  //t2->SetAngle(40);
  t2->SetLineWidth(2);
  t2->SetLineColor(kViolet-3);  
  t2->SetFillColor(kViolet-3);
  t2->Draw();

}
