import numpy as np 
import csv 
import matplotlib.pyplot as plt

NUMBER_OF_POINTS = 250000
NUMBER_OF_CLUSTERS = 5

points = np.zeros((NUMBER_OF_POINTS, 10), dtype=float)
counter = -1
with open('points.csv', 'r', newline = '') as csvDataFile:
    csvReader = csv.reader(csvDataFile)
    for row in csvReader:
    	if counter != -1:
	    	#x
	    	points[counter][0] = row[0]
    		#y
    		points[counter][1] = row[1]
    		#cluster
    		points[counter][2] = row[2]
    	counter += 1


clusters = np.zeros((NUMBER_OF_CLUSTERS, 3), dtype=float)
counter = -1
with open('clusters.csv', 'r', newline = '') as csvDataFile:
    csvReader = csv.reader(csvDataFile)
    for row in csvReader:
    	if counter != -1:
	    	clusters[counter][0] = row[0]    		#y
    		clusters[counter][1] = row[1]
    		clusters[counter][2] = row[2]
    	counter += 1

cluster_1 = 0
cluster_2 = 0
cluster_3 = 0
cluster_4 = 0
cluster_5 = 0
'''cluster_6 = 0
cluster_7 = 0
cluster_8 = 0
cluster_9 = 0
cluster_10 = 0
'''
for i in range(0, NUMBER_OF_POINTS):
	if points[i][2] == 0:
		cluster_1 += 1
	if points[i][2] == 1:
		cluster_2 += 1
	if points[i][2] == 2:
		cluster_3 += 1
	if points[i][2] == 3:
		cluster_4 += 1
	if points[i][2] == 4:
		cluster_5 += 1
'''	if points[i][2] == 5:
		cluster_6 += 1
	if points[i][2] == 6:
		cluster_7 += 1
	if points[i][2] == 7:
		cluster_8 += 1
	if points[i][2] == 8:
		cluster_9 += 1
	if points[i][2] == 9:
		cluster_10 += 1
'''

cluster_pts_1 = np.zeros((cluster_1, 2), dtype=int)
cluster_pts_2 = np.zeros((cluster_2, 2), dtype=int)
cluster_pts_3 = np.zeros((cluster_3, 2), dtype=int)
cluster_pts_4 = np.zeros((cluster_4, 2), dtype=int)
cluster_pts_5 = np.zeros((cluster_5, 2), dtype=int)
'''cluster_pts_6 = np.zeros((cluster_6, 2), dtype=int)
cluster_pts_7 = np.zeros((cluster_7, 2), dtype=int)
cluster_pts_8 = np.zeros((cluster_8, 2), dtype=int)
cluster_pts_9 = np.zeros((cluster_9, 2), dtype=int)
cluster_pts_10 = np.zeros((cluster_10, 2), dtype=int)

'''

counter_1 = 0
counter_2 = 0
counter_3 = 0
counter_4 = 0
counter_5 = 0
'''counter_6 = 0
counter_7 = 0
counter_8 = 0
counter_9 = 0
counter_10 = 0

'''
for i in range (0, NUMBER_OF_POINTS):
	if points[i][2] == 0 and counter_1 < 42835:
		cluster_pts_1[counter_1][0] = points[i][0]
		cluster_pts_1[counter_1][1] = points[i][1]
		counter_1 += 1
	if points[i][2] == 1 and counter_2 < 42835:
		cluster_pts_2[counter_2][0] = points[i][0]
		cluster_pts_2[counter_2][1] = points[i][1]
		counter_2 += 1
	if points[i][2] == 2 and counter_3 < 42835:
		cluster_pts_3[counter_3][0] = points[i][0]
		cluster_pts_3[counter_3][1] = points[i][1]
		counter_3 += 1
	if points[i][2] == 3 and counter_4 < 42835:
		cluster_pts_4[counter_4][0] = points[i][0]
		cluster_pts_4[counter_4][1] = points[i][1]
		counter_1 += 1
	if points[i][2] == 4 and counter_5 < 42835:
		cluster_pts_5[counter_5][0] = points[i][0]
		cluster_pts_5[counter_5][1] = points[i][1]
		counter_5 += 1
'''	if points[i][2] == 5 and counter_6 < 108142:
		cluster_pts_6[counter_6][0] = points[i][0]
		cluster_pts_6[counter_6][1] = points[i][1]
		counter_1 += 1
	if points[i][2] == 6 and counter_7 < 108142:
		cluster_pts_7[counter_7][0] = points[i][0]
		cluster_pts_7[counter_7][1] = points[i][1]
		counter_7 += 1
	if points[i][2] == 7 and counter_8 < 108142:
		cluster_pts_8[counter_8][0] = points[i][0]
		cluster_pts_8[counter_8][1] = points[i][1]
		counter_8 += 1
	if points[i][2] == 8 and counter_9 < 108142:
		cluster_pts_9[counter_9][0] = points[i][0]
		cluster_pts_9[counter_9][1] = points[i][1]
		counter_9 += 1
	if points[i][2] == 9 and counter_10 < 108142:
		cluster_pts_10[counter_10][0] = points[i][0]
		cluster_pts_10[counter_10][1] = points[i][1]
		counter_10 += 1
'''
plt.scatter(cluster_pts_1[:,0], cluster_pts_1[:,1] , color = 'b', s=1)
plt.scatter(cluster_pts_2[:,0], cluster_pts_2[:,1] , color = 'g', s=1)
plt.scatter(cluster_pts_3[:,0], cluster_pts_3[:,1] , color = 'r', s=1)
plt.scatter(cluster_pts_4[:,0], cluster_pts_4[:,1] , color = 'c', s=1)
plt.scatter(cluster_pts_5[:,0], cluster_pts_5[:,1] , color = 'm', s=1)
'''plt.scatter(cluster_pts_6[:,0], cluster_pts_6[:,1] , color = 'y', s=1)
plt.scatter(cluster_pts_7[:,0], cluster_pts_7[:,1] , color = 'k', s=1)
plt.scatter(cluster_pts_8[:,0], cluster_pts_8[:,1] , color = 'w', s=1)
plt.scatter(cluster_pts_9[:,0], cluster_pts_9[:,1] , color = 'b', s=1)
plt.scatter(cluster_pts_10[:,0], cluster_pts_10[:,1] , color = 'g', s=1)
'''
plt.scatter(clusters[0][0], clusters[0][1] , color = 'y', s=200)
plt.scatter(clusters[1][0], clusters[1][1] , color = 'y', s=200)
plt.scatter(clusters[2][0], clusters[2][1] , color = 'y', s=200)
plt.scatter(clusters[3][0], clusters[3][1] , color = 'y', s=200)
plt.scatter(clusters[4][0], clusters[4][1] , color = 'y', s=200)
'''plt.scatter(clusters[5][0], clusters[5][1] , color = 'b', s=200)
plt.scatter(clusters[6][0], clusters[6][1] , color = 'b', s=200)
plt.scatter(clusters[7][0], clusters[7][1] , color = 'b', s=200)
plt.scatter(clusters[8][0], clusters[8][1] , color = 'm', s=200)
plt.scatter(clusters[9][0], clusters[9][1] , color = 'b', s=200)
'''
plt.show()

