from delaunay import computeVolume
from random import randint
from transform import *
import symbols as sym

class Point3D():
	"""generic point"""
	def __init__(self, x, y, z):
		self.x = x
		self.y = y
		self.z = z
		self.L = (self.x, self.y, self.z)

	def __str__(self):
		return str(self.L)



class PointCloud():
	"""n points in space"""		
	def __init__(self, n1, n2=None):
		if n2 is None:
			self.point_array = [Point3D(randint(0, 9), randint(0, 9), randint(0, 9)) for i in range(int(n1))]
		else :
			pts = [ Point3D(t[0], t[1], t[2]) for t in n1]
			self.point_array = pts

		self.orig_array = (self.point_array).copy()

	def print_details(self):
		print("Points in Cloud : ")
		for pt in self.point_array: 
			print(pt)

	def get_volume(self):
		v = computeVolume(self.point_array)
		return v
	
	def recompute(self, transformations, idx):
		self.point_array = (self.orig_array).copy()
		for i in range(idx+1):
			self.transform((sym.ROTATE_EULER_MODE, transformations[i][1]))

	def transform(self,transform_op, normalize=False):
		pts = self.point_array
		self.point_array = [Point3D(t[0], t[1], t[2]) for t in transformAllPoints(pts, transform_op, normalize)]