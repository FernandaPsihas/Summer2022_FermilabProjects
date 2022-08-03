import h5py
import numpy as np
import matplotlib
from matplotlib import pyplot as plt

datafilename = 'example_data'
hf = h5py.File('%s.h5' % datafilename , 'r')

print(hf.keys())
# <KeysViewHDF5 ['True_E', 'True_x', 'Wiremaps']>
print(hf['True_E'])

# Get darasets
E = hf.get('True_E')
x = hf.get('True_x')
W = np.array(hf.get('Wiremaps'))

E=np.array(E)
x=np.array(x)

# random plot to see
#probably good to make these for each distribution (x, E, Q, etc.)

fig, ax = plt.subplots(figsize=(5,4))
plt.hist(x,linewidth=2, color="#14213D")
plt.xlabel('x position [<units>]')
#ax.set_xlim(0.0, 3.5) # set axis limits
plt.show()
fig.savefig('plot_x_datafilename.pdf')


# Make example images

#for i in range(len(Wiremaps)):
for i in range(10): # Do just 10 for now
    this_2d_array = W[i]
    fig, ax = plt.subplots(figsize=(12,8))
    plt.imshow(this_2d_array)
    plt.text(0.96, 0.2, 'Energy = %1.2f <units>' % (E[i]), horizontalalignment='right',verticalalignment='center', transform=ax.transAxes,fontsize=13,color="#ffffff")
    plt.text(0.96, 0.1, 'x pos. = %1.2f <units>' % (x[i]), horizontalalignment='right',verticalalignment='center', transform=ax.transAxes,fontsize=13,color="#ffffff")
    plt.show()
    fig.savefig('plot_x_%s_%1.0f.pdf' % (datafilename, i))
