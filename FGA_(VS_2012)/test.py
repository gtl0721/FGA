import matplotlib.pyplot as plt
import numpy as np
import math
path = "C://Users//a8000//OneDrive//桌面//碩士//6.機器智慧//HW#2//FGA(VS_2012)//FGA_(VS_2012)//final_pop.ga"
test_data = []
px = []
py = []
pz = []
start = -1
end = 1
interval = 0.01
size = (end - start) / interval
size = int(size)

rosenbrock = lambda x,y: (x-1)**2 + 100*(y-x**2)**2
rastrigrin = lambda x,y: x**2 - (10 * math.cos(2*x*math.pi)) + y**2 - (10 * math.cos(2*y*math.pi)) + 20

with open(path,'r') as f :
    lines = f.readlines()
    for line in lines:
        value = [float(s) for s in line.split()]
        if(value != []):
            test_data.append(value)

for data in test_data:
    px.append(data[0])
    py.append(data[1])
    pz.append(data[2])

px = np.array(px)
py = np.array(py)
pz = np.array(pz)

x = np.linspace(start, end, size)
y = np.linspace(start, end, size)
X, Y = np.meshgrid(x, y)

Z = rosenbrock(X,Y)
#Z = rastrigrin(X,Y)

fig = plt.figure()
plt.rcParams["font.size"] = 10

plt.contourf(X,Y,Z)
plt.plot(px, py, '*', color='red')

fig, ay = plt.subplots()
ay = plt.axes(projection="3d")
#ay.plot_wireframe(X,Y,Z,color='green')
ay.plot_surface(X,Y,Z,rstride=1,cstride=1,cmap='winter',edgecolor='none')
ay.set_title('surface')
ay.set_xlabel('x')
ay.set_ylabel('y')
ay.set_zlabel('z')

plt.show()