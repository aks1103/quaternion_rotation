class Quaternion():
	def __init__(self, q0, q1, q2, q3):
		self.s = q0
		self.x = q1
		self.y = q2
		self.z = q3
		self.L = [ q0, q1, q2, q3 ]

	def norm(self):
		return math.sqrt(self.s*self.s + self.x*self.x + self.y*self.y + self.z*self.z)
	
