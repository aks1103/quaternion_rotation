from mpl_toolkits import mplot3d
import numpy as np 
import matplotlib.pyplot as plt
import pickle


from sklearn.preprocessing import PolynomialFeatures
from sklearn import linear_model




thresh_vals = [0.05,0.08,0.09, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6,0.7,0.8,0.9]

thresh_vals_fake = [0.05,0.08,0.09, 0.1, 0.2, 0.3, 0.4, 0.5, 0.4, 0.3, 0.2, 0.1]


X, Y, Z = [], [], []

for i in range(len(thresh_vals)):
	fp = open("./figs/thresh"+ str(thresh_vals_fake[i])  +"/ROTATE_EULER_APPROX_MODEperformances_each_angle.pkl", "rb")
	z= pickle.load(fp)
	Z.extend(z)


X = [float(i)/10 for i in range(len(z))]
Y = thresh_vals

X, Y = np.meshgrid(X, Y)

print(X[0])
print(Y[0])


X_ = []
for i in range(len(X)):
	X_.extend([[o[0],o[1]] for o in zip(X[i],Y[i])])


for i in range(len(X_)):
	print(X_[i], Z[i])

ax = plt.axes(projection='3d')
ax.scatter3D(X, Y, Z);
# plt.show()





# X_ = [[0.44, 0.68], [0.99, 0.23]]
# vector = [109.85, 155.72]
predict= [0.49, 0.18]

poly = PolynomialFeatures(degree=3)
X_ = poly.fit_transform(X_)
# predict_ = poly.fit_transform(predict)


clf = linear_model.LinearRegression()
clf.fit(X_, Z)


final = clf.predict(X_)
print(final)


# X = np.array(X)
# Y = np.array(Y)
# # Z = np.array(Z)

final = np.array(final).reshape((len(thresh_vals), -1))

ax = plt.axes(projection='3d')

plt.xlabel('X = angle')
plt.ylabel('Y = threshold')

ax.plot_surface(X, Y, final, rstride=1, cstride=1, cmap='viridis', edgecolor='none')
# ax.contour3D(X, Y, final)
ax.set_title('Performance Vs (angle, threshold)')




# r = np.linspace(0, 6, 20)
# theta = np.linspace(-0.9 * np.pi, 0.8 * np.pi, 40)
# r, theta = np.meshgrid(r, theta)

# X = r * np.sin(theta)
# Y = r * np.cos(theta)
# Z = (X +  Y)

# ax = plt.axes(projection='3d')
# ax.plot_surface(X, Y, Z, rstride=1, cstride=1,
#                 cmap='viridis', edgecolor='none');

plt.show()



	# z = np.polyfit(x, y_dash, 5)
	# p = np.poly1d(z)

	# px = p(x)
	# max_pt_y = np.max(px)
	# max_pt_x, = np.where(px == max_pt_y)

	# print(max_pt_y, max_pt_x)

	# plt.plot(x, y_dash, '--', x, p(x), '-')
	# plt.plot([float(max_pt_x)/10], [max_pt_y], "x")
	# plt.xlabel("rotation_angle(degrees)")
	# plt.ylabel("performance(1/secs)")
	# plt.title("performance vs rotation_angle")
	# plt.savefig("performance_05.png")
