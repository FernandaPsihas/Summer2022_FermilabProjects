import numpy as np
import h5py
import ROOT
import uproot
import matplotlib
from matplotlib import pyplot as plt

# Using uproot to open root files & create HDF5s 
f = uproot.open('low_energy_electrons_undoped.root')
print(f.keys())

# debugging - print tree info
# f["EventTree"].keys()
# f["EventTree"]["true_x"].keys()
# f["EventTree"]["true_x"].array()
## >>> truex=f["EventTree"]["true_x"].array()
## >>> truex[0]
## -24.83823299407959

true_x=f["EventTree"]["true_x"].array()
true_E=f["EventTree"]["true_E"].array()
wiremaps=f["EventTree"]["wiremap"].array()

# debugging - print wire map contents
# wiremaps[0]
# wiremaps[0][0][2]
# wiremaps[0][0][50]

# make numpy arrays for the h5 file
True_x = np.array(true_x)
True_E = np.array(true_E)
Wiremaps = np.array(wiremaps)

# store in h5 file - probably better to get the name from the input file
hf = h5py.File('example_data.h5', 'w')

hf.create_dataset('True_x', data=True_x)
hf.create_dataset('True_E', data=True_E)
hf.create_dataset('Wiremaps', data=Wiremaps)

hf.close()


#-----------------
