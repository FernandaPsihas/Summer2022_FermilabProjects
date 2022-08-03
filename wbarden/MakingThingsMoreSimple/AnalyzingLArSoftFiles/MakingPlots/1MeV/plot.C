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

  const int Ebins = 1;
  const int filesize = files.size();
  double eres[filesize][Ebins];  
  double eres_err[filesize][Ebins];
  double ebin_center[filesize][Ebins];
  double ebin_width[filesize][Ebins];

  ebin_center[0][0] = 0;
  ebin_center[1][0] = 25;
  ebin_center[2][0] = 100;

  TCanvas* c = new TCanvas("c");
  c->cd();
  TH1F* h = new TH1F("h", ";Dopant Quantum Efficiency [%]; Energy Resolution at 1 MeV [%]; ", 1, -5, 105);
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
      double elow = 0.95;
      double ehigh = 1.05;
      
      //ebin_center[i][e] = elow+((ehigh-elow)/2);
      //      ebin_width[i][e] = (ehigh-elow)/2;

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

  TF1* f2 = new TF1("f2","[0] + ([1] - [2])/(1 + pow(x/[3],[4]))",0,100);
  f2->SetParameter(0,-0.213431);
  f2->SetParameter(1,6.74848);
  f2->SetParameter(2,0.373372);
  f2->SetParameter(3,4.87814);
  f2->SetParameter(4,0.642656);
  f2->Draw("same");

  TLegend* leg = new TLegend(0.3, 0.65, 0.85, 0.88);
  leg->AddEntry(Form("gr_%s",files[0].c_str()), "Pure Liquid Argon", "pl");
  leg->AddEntry(Form("gr_%s",files[1].c_str()), "with P.S. Dopant,  25% Q.E.", "pl");
  leg->AddEntry(Form("gr_%s",files[2].c_str()), "with P.S. Dopant, 100% Q.E.", "pl");
  leg->Draw("same");



}
