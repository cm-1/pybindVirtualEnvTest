import math
import numpy as np

class myPyClass:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def euclength(self):
        return math.sqrt(self.x*self.x + self.y*self.y)

    def translate(self, deltaX, deltaY):
        self.x += deltaX
        self.y += deltaY

    def arrayOfThruples(self):
        retVal = []
        for i in range(8):
            retVal.append((self.x, self.y, i))
        return retVal

    def print2darray(self, ar):
        if ar.ndim != 2:
            print("Array is not 2D.")
            return
        
        for i in range(ar.shape[0]):
            print(list(ar[i]))
        

