import pandas as pd 
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D    

if __name__ == '__main__':
    fig = plt.figure()
    
    fn = "~/ORB_SLAM2/CameraTrajectory.txt"
    data = pd.read_csv(fn, sep = ' ',header=None)
    ax = Axes3D(fig)
    ax.scatter(data.iloc[:,1],data.iloc[:,2],data.iloc[:,3], marker="o", color="r")
    
    fn1 = "~/ORB_SLAM2/KeyFrameTrajectory.txt"
    data1 = pd.read_csv(fn1, sep = ' ',header=None)
    #ax1 = Axes3D(fig)
    ax.scatter(data1.iloc[:,1],data1.iloc[:,2],data1.iloc[:,3], marker="^", color="b")
    
    plt.show()