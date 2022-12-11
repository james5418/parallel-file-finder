import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(8, 5))

x_index = [2, 4, 6, 8]

find = [8]
fd = [6.08, 3.14, 2.45, 1.22]
ppfind = [5.08, 2.14, 1.45, 0.22]

df = pd.DataFrame(fd)
df2 = pd.DataFrame(ppfind)

for i,j in enumerate(fd):
    plt.annotate(j, xy=(i,j+0.15), color='tab:blue')
for i,j in enumerate(ppfind):
    plt.annotate(j, xy=(i,j+0.15), color='tab:orange')

plt.plot(df, label="fd", marker='o', markersize=5)
plt.plot(df2, label="pp-find", marker='o', markersize=5)

plt.xlabel('number of threads')
plt.ylabel('execution time')
plt.title("Random Directory Structure")
plt.xticks(ticks=range(len(x_index)), labels=x_index)
plt.legend()
plt.show()