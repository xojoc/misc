# Written by http://xojoc.pw. Public domain.
import math
from PIL import Image
import numpy as np
import matplotlib

zoom = 100
minx = -3*zoom
maxx = 8*zoom
miny = minx
maxy = maxx
width = maxx-minx
height = maxy-miny

#canvas = np.ones((width,height), np.float96)

n = 1

xs = np.arange(-0.1, 1, 0.04)
#ys = np.arange(-3, 8, 1)
ys = [1]

#x = -0.1
#y = 0.2

#x = -0.1
#y = 0.1

for x0 in xs:
    for y0 in ys:
        x = x0
        y = y0

        print(x, y, n)

        canvas = np.ones((width,height))

        for i in range(100000):
            x1 = x*zoom-minx
            y1 = y*zoom-miny
            if x1 >= 0 and y1 >= 0 and x1 < width and y1 < height:
                canvas[x1][y1] = 0.0
                t = x
                x = 1 - y + abs(x)
                y = t

        Image.fromarray(np.uint8(canvas*255)).save("gingerbreadman" + str(n)+".png")
        n += 1

#colormap = matplotlib.colors.LinearSegmentedColormap.from_list("", [[0.0,0,0], [0,1,0], [1,0,0]])

#Image.fromarray(np.uint8(canvas*255)).save("gingerbreadman.png")

