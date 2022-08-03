void plot(){ //Name of program

  gStyle->SetOptStat(0); // shuts off annyoing stats box
  TFile* def_f = new TFile("../AnalyzingNtuples/outfile.root"); //loads the file we want
  TTree* def_t = (TTree*)def_f->Get("FastTree"); //loads the TTree we just created
  
  //We'll create a plot now, either a 1D (TH1D) or a 2D (TH2D).
  // the numbers are the end are:
  // 1. the number of bins (25)
  // 2. the lower bound of the x-axis (0 MeV)
  // 3. the upper bound of the x-axis (10 MeV)
  // this is then repeated for the y-axis
  
  TH2D* def = new TH2D("def",";E_{dep} [MeV];FWHM;",25,0,10,100,0,5000); 
  
  // we then fill that histogram ("def") with the variables we are interested in 
  // using "<y-axis variable>:<x-axis variable>" 
  // you can also add any cuts you'd like to apply in the currently empty quotes.
  def_t->Project("def","correction*wireQ:Edep","");

    
  // finally we draw it! 
  // "colz" means color brightness on the z-axis to designate density of events
  def->Draw("colz");

}
