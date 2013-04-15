import scipy.io
import random
import plotter

def addNoise(fileName, image): #adds noise to image, writes to file
	f = open(fileName, 'w')
#	outputString = str(len(image[0])) + '\n' + str(len(image))#horizontal and vertical pixels
	outputString = ""
	for row in image:
#		outputString += ','.join(map(lambda x: str(x + random.uniform(-0.2, 0.2)), row)) + ',' #added comma to conform to previous format
		for pixel in row:
			if pixel == 1:
				pixel += random.uniform(-0.5, 0)
			else:
				pixel += random.uniform(0, 0.5)
		
			outputString += str(pixel)[:4] + ','
		outputString += ';'

	f.write(outputString)
	f.close()


def imageToFile(fileName, image):
	f = open(fileName, 'w')
#	outputString = str(len(image[0])) + '\n' + str(len(image))#horizontal and vertical pixels
	outputString = ""
	for row in image:
		outputString += ','.join(map(lambda x: str(x), row)) + ',' #added comma to conform to previous format
		outputString += ';'

	f.write(outputString)
	f.close()

if __name__ == "__main__":
	mat = scipy.io.loadmat('X.mat')
	imageToFile('matlab_ground_truth', mat['X'])
	plotter.writeToPNG('matlab_ground_truth', plotter.readFromFile('matlab_ground_truth'))
	addNoise('matlab_noisy', mat['X'])
	plotter.writeToPNG('matlab_noisy', plotter.readFromFile('matlab_noisy'))
