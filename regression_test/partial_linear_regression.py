import numpy as np
import matplotlib.pyplot as plt
from collections import namedtuple as nt

MAX_DEPTH = 4

def loadData(fname):
    with open(fname) as f:
        data = f.readlines()
    return [float(n) for n in data]

Line = nt("Line","a b startX endX")
Point = nt("Point","x y")
# y = ax + b

def grad(pointFrom,pointTo):
    # gradient a
    return (pointTo.y - pointFrom.y)/(pointTo.x - pointTo.y)
def bFrom(a, x, y):
    return y - a*x

def yFrom(line, x):
    return line.a*x + line.b

def distanceSquare(line, point):
    return pow(abs( line.a*point.x - point.y + line.b),2) / (line.a*line.a + 1)

def linearRegression(points):
    xSum = sum([p.x for p in points])
    xSumSq = sum([pow(p.x,2) for p in points])
    ySum = sum([p.y for p in points])
    xySum = sum([p.x*p.y for p in points])
    n = len(points)
  
    a = (n*xySum - xSum * ySum)/(n*xSumSq - xSum*xSum)
    b = ySum/n - a*xSum/n
    return Line(a,b,points[0].x,points[-1].x)

def split(line, points, depth):
    #print(points)
    needleMin = min(points, key=lambda p: distanceSquare(line,p))
    needleMax = max(points, key=lambda p: distanceSquare(line,p))
    needleMinIdx = points.index(needleMin)
    needleMaxIdx = points.index(needleMax)
    middleIdx = len(points)/2

    # max로 잡았을 때와 min으로 잡았을 때의 특성이 다른데, 여기서 둘의 특성이 잘 합쳐진다
    if abs(middleIdx-needleMaxIdx) > abs(middleIdx-needleMinIdx):
        needleIdx = needleMinIdx
    else:
        needleIdx = needleMaxIdx

    lPoints = points[:needleIdx]
    rPoints = points[needleIdx:]

    if len(lPoints)>1 and len(rPoints)>1:

        leftLine = linearRegression(lPoints)
        rightLine = linearRegression(rPoints)
    
        if depth == MAX_DEPTH-1:
            return [leftLine, rightLine]
        else:
            splited1 = split(leftLine, lPoints, depth+1)
            splited2 = split(rightLine, rPoints, depth+1)
            splited1.extend(splited2)
            return splited1
    else:
        return [line]


def drawLines(lines,dataPoints):
    points = [[l.startX, yFrom(l,l.startX)] for l in lines]
    lastPoint = [lines[-1].endX, yFrom(lines[-1],lines[-1].endX)]
    points.append(lastPoint)
    points = np.array(points)
    dataPoints = np.array(dataPoints)
    x = points[:,0]
    y = points[:,1]

    plt.scatter(dataPoints[:,0],dataPoints[:,1],s=10)
    plt.plot(x,y,'r')
    plt.show()

if __name__ == "__main__":
    data = loadData("./linear_regression.csv")
    data = [ Point(2*i, data[i]) for i in range(len(data))]
    line0 = linearRegression(data)
    #drawLines([line0],data)
    lines = split(line0, data, 0)
    print(len(lines))
    drawLines(lines,data)