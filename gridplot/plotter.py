import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import Image
import random
import os
from PIL import Image, ImageSequence
import images2gif as gifs
def readFromFile(fileName):
	f = open(fileName, 'r')
	grid = f.readline().split(';')[:-1]
	newGrid = []
	for y in range(len(grid)):
		currentColumn = grid[y].split(',')[:-1]
		newColumn = []
		
		for x in range(len(currentColumn)):
#			print currentColumn
			red = 255 * (float(currentColumn[x]))
			green = 255 * (1 - float(currentColumn[x]))
#			blue = 256 * (1 - float(currentColumn[x]))
			blue = 0
#			blue = 256 * (1 - float(currentColumn[x]))
#			red = 255
#			green = 255
#			blue = 255

			rgb = [red, green, blue]
			rgb = np.uint8(rgb)

			newColumn.append(rgb)
		newGrid.append(newColumn)

	f.close()
	return np.array(newGrid)

def writeToPNG(fileName, imageArray):
	im = Image.fromarray(imageArray, 'RGB')
	im.save(fileName + '.png')

def batchWrite():
#	frames = []
	for f in os.listdir(os.getcwd()):
		if f[:2] == '__':
			fileArray = readFromFile(f)
			writeToPNG(f[2:], fileArray)
#			frames+= [fileArray]

#	print frames
#	writeToGIF(frames)
def writeToGIF(fileArray):
	gifs.writeGif("test.gif", fileArray, duration = 10)	

if __name__ == "__main__":
#	fileName = 'test1'
#	fileArray = readFromFile(fileName)
#	print fileArray
	batchWrite()

#	writeToPNG(fileName, fileArray)
