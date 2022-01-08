import matplotlib.pyplot as plt
plt.plot([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16], 
		 [111.201, 83.902, 62.393, 50.270, 41.209, 34.908, 29.762, 
		  26.712, 24.049, 21.730, 20.350, 19.686, 18.814, 17.932, 
		  16.693, 16.257], linewidth = 4, color = 'r')
plt.ylabel('Elapsed time during the execution (in seconds)')
plt.xlabel('Thread amount used for the execuiton')
plt.show()