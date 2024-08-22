import pandas as pd
import numpy as np

SIZE = 21

array_size = [1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576]

file_names = ["base","pipeline","binomial","ring"]


for i in range(len(file_names)):
	file = pd.read_table(file_names[i]+".txt",delimiter=" ",header=None)
	
	mat = np.zeros((21,21),dtype=float)
	mat[:,0] = array_size

	counter = 0

	for j in range(1,21):
		for k in range(21):
			mat[k,j] = file.iat[counter,0]
			counter += 1

	file_out = open(file_names[i]+".csv",'w')

	for j in range(21):
		linea = ""
		for k in range(21-1):
			linea = linea + str(mat[j,k]) + ","
		linea = linea + str(mat[j,-1]) + "\n"
		file_out.write(linea)

	file_out.close()