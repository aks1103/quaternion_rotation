import numpy as np
from scipy.spatial import Delaunay


def determinant_3x3(m):
    return (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1]) + m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1]))

def subtract(a, b):
    return (a[0] - b[0], a[1] - b[1], a[2] - b[2])

def tetrahedron_calc_volume(t):
	a, b, c, d = t
	return (abs(determinant_3x3((subtract(a, b), subtract(b, c), subtract(c, d), ))) / 6.0)



def computeVolume(pts):
	pts = [ pt.L for pt in pts]
	pts = np.array(pts)
	volume = 0
	tetra = Delaunay(pts)
	# print(tetra.simplices)
	# print(pts[tetra.simplices])
	for t in pts[tetra.simplices]:
		volume += tetrahedron_calc_volume(t)

	# print("Total volume : " , v)
	return volume




def main():
	pts = [(0,0,0), (1,0,0), (0,1,0), (0,0,1), (1,0,1), (1,1,0), (0,1,1), (1,1,1)]
	print("Volume of test surface : " ,computeVolume(pts))


if __name__ == '__main__':
	main()