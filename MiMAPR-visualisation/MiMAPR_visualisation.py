from asyncio.windows_events import NULL
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d.axes3d as p3
import matplotlib.animation as animation
import numpy as np

from matplotlib import cbook
from matplotlib import cm
from matplotlib.colors import LightSource
import time



def CreateGif(data, step):
    fig = plt.figure()
    ax = fig.add_subplot()

    # Make the X, Y meshgrid.
    xs = np.linspace(0, 500, 500//step+1)
    ys = np.linspace(0, 400, 400//step+1)
    X, Y = np.meshgrid(xs, ys)

    # Set the z axis limits, so they aren't recalculated each frame.
    #ax.set_zlim(-1, 1)

    # Begin plotting.
    cs = None
    tstart = time.time()
    for i in range(1000):
        # If a line collection is already remove it before drawing.

        if cs:
            cs.remove()
        # Generate data.
        #print(data[i])
        cs = ax.contourf(xs, ys, data[i],cmap="bwr")
    # cs = ax.contourf(res, cmap ="Wistia")
        #cbar = plt.colorbar(cs)
        # Plot the new wireframe and pause briefly before continuing.
        #wframe = ax.plot(xs, ys, data[i], rstride=2, cstride=2)
        plt.pause(.001)
    plt.show()
    print('Average FPS: %f' % (100 / (time.time() - tstart)))

def main():
    f = open("../output.txt", "r")
    step = int(input())
    h_s = 400//step
    w_s = 500//step
    temp_grid = np.array([np.array([np.array([-100 for k in range(w_s+1)]) for j in range(h_s+1)]) for i in range(1000)])
    i = 0
    last_time = -1;
    for line in f:
        i += 1; 
        data = line.split()
        data_int = []
        if(i > 1):
            for d in range(len(data)):
                if(d == 3):
                    data_int.append(float(data[d]))
                else:
                    data_int.append(int(data[d]))

            temp_grid[data_int[0]][data_int[2]][data_int[1]] = data_int[3]
    
    CreateGif(temp_grid, step)


        



if __name__ == "__main__":
    main()