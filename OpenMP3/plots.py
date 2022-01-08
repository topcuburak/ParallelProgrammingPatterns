import matplotlib.pyplot as plt
plt.plot([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16], 
		 [1.961, 1.676, 1.615, 1.574, 1.534, 1.513, 1.483, 1.507, 1.484, 1.486, 1.572, 
		 1.631, 1.590, 1.554, 1.478, 1.526], linewidth = 4, color = 'r')
plt.ylabel('Elapsed time during the execution (in seconds)')
plt.xlabel('Thread amount used for the execuiton')
plt.savefig('results.png')