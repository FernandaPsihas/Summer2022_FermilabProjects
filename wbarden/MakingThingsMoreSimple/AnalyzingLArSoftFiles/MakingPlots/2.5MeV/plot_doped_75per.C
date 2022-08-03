void plot_doped_75per(){ //Name of program

  gStyle->SetOptStat(0); // shuts off annyoing stats box
  TFile* def_f = new TFile("../../monoenergetic_2.5MeV_electrons_doped_75percent.root"); //loads the file we want
  TTree* def_t = (TTree*)def_f->Get("EventTree"); //loads the TTree we just created
  
  //We'll create a plot now, either a 1D (TH1D) or a 2D (TH2D).
  // the numbers are the end are:
  // 1. the number of bins (25)
  // 2. the lower bound of the x-axis (0 MeV)
  // 3. the upper bound of the x-axis (10 MeV)
  // this is then repeated for the y-axis
  
  TH1D* def = new TH1D("def",";E_{dep} [MeV];;",500,0,1500); 
  
  // we then fill that histogram ("def") with the variables we are interested in 
  // using "<y-axis variable>:<x-axis variable>" 
  // you can also add any cuts you'd like to apply in the currently empty quotes.
  def_t->Project("def","reco_tot_wire_Q/true_Edep","true_Edep > 0 && reco_tot_wire_Q > 0");

  def->Sumw2();
  /*  TF1* f = new TF1("f","crystalball",450,570);
  f->SetParameter(0,3e4);
  f->SetParameter(1,5.25202e2);
  f->SetParameter(2,9e1);
  f->SetParameter(3,0);
  f->SetParameter(4,0);*/
  TF1* f = new TF1("f","gaus",860,920);
  def->Fit("f","RLM");
    
  // finally we draw it! 
  // "colz" means color brightness on the z-axis to designate density of events
  def->Draw("");

  double sigma = f->GetParameter(2);
  double mu = f->GetParameter(1);
  double err_sigma = f->GetParError(2);  
  double err_mu = f->GetParError(1);

  std::cout << "Energy Resolution : " << 100*sigma/mu << " ± " << 100*sqrt(pow(sigma/mu,2)*(pow(err_sigma/sigma,2)+pow(err_mu/mu,2))) << " %"<< std::endl;

}
