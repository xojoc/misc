# Written by http://xojoc.pw. Public domain.
import math
from PIL import Image
import numpy as np
import matplotlib

π = math.pi
sin = math.sin

width = 300
height = 300

#canvas = Image.new("RGB", (width, height))
array = np.zeros((width,height), np.float96)

def circle_map(θ, Ω, K):
    return θ+Ω-(K/(2*π))*sin(2*π*θ)

def ω(Ω, K):
    θ = 0
    n = 1000
    for i in range(n):
        θ = circle_map(θ, Ω, K)
    return θ/n

def color(ω):
    if ω <= 1/2:
        return (0, int(ω*2*255), 0)
    else:
        return (int((ω-1/2)*2*255), int((1-(ω-1/2))*2*255),0)
    
def plot():
    for y in range(height):
        for x in range(width):
            Ω = x/width
            K = y/height*2*π
            #            canvas.putpixel((x, y), color(ω(Ω, K)))
            array[x][y] = ω(Ω, K)

            
colormap = matplotlib.colors.LinearSegmentedColormap.from_list("", [[0.0,0,0], [0,1,0], [1,0,0]])

plot()

Image.fromarray(np.uint8(colormap(array)*255)).save("/tmp/a.png")
