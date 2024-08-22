import pandas as pd
import numpy as np

SIZE = 21

array_size = [1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576]

file_in = pd.read_table("output.txt",delimiter=" ",header=None)

mat = np.zeros((47,21,21),dtype='object')

counter = 0

for i in range(47):
	mat[i,0,:] = array_size

	j = 1
	k = 0
	while(file_in.iat[counter,0]!="&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"):
		mat[i,j,k] = file_in.iat[counter,0]
		k += 1
		counter += 1

		if(k==len(array_size)):
			k = 0
			j += 1

	counter += 1

for i in range(47):
	file_out = open("sync/sync_"+str(i+2)+".csv",'w')

	for j in range(len(array_size)):
		linea = ""
		for k in range(len(array_size)-1):
			linea = linea + str(mat[i,k,j]) + ","
		linea = linea + str(mat[i,-1,j]) + "\n"
		file_out.write(linea)

	file_out.close()



