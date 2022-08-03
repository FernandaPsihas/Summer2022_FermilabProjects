The first step is to source the setup file in this directory:

> source setup.sh

You should now have access to the Fermilab software stack, including ROOT (https://root.cern.ch/doc/v622/).

From here we can start analyzing some files. 

The first step is to analyze the complex files that come out of the experimental simulation. These files contain _way_ more information than we'll need for this summer so we'll try to condense them down to the relevant information. We do this by starting here:

* AnalyzingLArSoftFiles *

This directory reads in our complex simulation (LArSoft) files and aims to produce a much sparser output file that aims to "flatten" these files. 

To run this all that is needed is:

> root AnalyzerStandard.C

This will create a file called "output.root" and it should contain a "TTree" which is a multidimensional representation of the data created inside "AnalyzerStandard.C" 

One thing that can be adjusted is this line:

```
 filenames.erase(filenames.begin()+25,filenames.end());
```

This limits the number of files processed to "25" this number can be dropped entirely. 

Once this has been run we have a easier to analyze file that stores each event as a single entry. We'll want to now analyze this into a very quick to play file. 

* AnalyzingNtuples *

This consumes the TTree that we created in the previous module and aims to make a file that is very very quick to run over so that we can make. This is a little clunky and the first thing we need to do is edit the header file "EventTree.h"

in "EventTree.h" we'll want to make sure we're point towards the correct file, this is currently set here:

```
TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../AnalyzingLArSoftFiles/output.root");
if (!f || !f->IsOpen()) {
  f = new TFile("../AnalyzingLArSoftFiles/output.root");
```

We need to make sure that these match the name of the file we created in the previous step. 

To run this we do the following:

> root EventTree.C
> EventTree t; t.Loop()

This will then loop through the output file we created and make a very condensed file that we can use to make plots! 

* MakingPlotsFromAnalysisFile * 

At this point we should be ready to make plots! 

In this directory there is a fairly simple file `plot.C` inside there is documentation. 

to run you just need to do the following:
>root plot.C

and you'll have a plot! 
