import matplotlib.pyplot as plt
import numpy as np

A = [0.27, 22.44, 14.49, 38.77, 42.67, 51.95, 59.87, 67.71, 75.11, 
    83.75, 92.28, 100.35, 82.14, 112.13, 110.492, 119.94]
B = [0.26, 20.64, 10.47, 34.26, 38.76, 50.85, 47.86, 63.07, 63.33, 
    74.33, 87.44, 90.10, 96.91, 95.044, 114.95, 127.54]

C = np.arange(1,17,1)

plt.plot(C, A, 'g', C, B, 'r', linewidth=4)
plt.ylabel('execution time in seconds')
plt.xlabel('number of threads being executed in parallel')
plt.title('Execution time vs Number of threads')
plt.show()