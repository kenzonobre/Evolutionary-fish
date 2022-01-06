import matplotlib.pyplot as plt

file = open("data.txt", "r")

data = file.read().split()

##print(data)

generations = int(len(data) // 22)
print("Generations =", generations)

'''
for specie in range(0, 20):

	x = []
	y = []

	for i in range(generations):
		x.append(i)
		y.append(int(data[22 * i + specie]))

	##print(x);

	l = f"specie {specie}"

	plt.plot(x, y, c = "blue", linewidth = 1, label = l, linestyle = "dashed")
'''

x = []
y = []

for i in range(generations):
	x.append(i)
	y.append(int(data[22 * i + 20]))

##print(x);

plt.plot(x, y, c = "red", linewidth = 1, label = "Dead fish")##, marker = 'o', markersize = 3)

x = []
y = []

for i in range(generations):
	x.append(i)

	mean = 0;
	for j in range(20):
		mean += int(data[22 * i + j])
	y.append(mean / 20)

##print(x);

plt.plot(x, y, c = "black", linewidth = 1, label = "Mean Fitness")##, marker = 'o', markersize = 3)




'''
x = []
y = []

for i in range(generations):
	x.append(i)

	index = int(data[22 * i + 21][3:])
	y.append(int(data[22 * i + index]))

##print(x);

plt.plot(x, y, c = "blue", linewidth = 1, label = "Best individual", 
	marker = 'o', linestyle = "dashed", markersize = 3)
'''




plt.xlabel("Generations")
plt.ylabel("Fitness")
plt.xlim(0, generations)
plt.ylim(0, 1000)
plt.legend()
plt.show()