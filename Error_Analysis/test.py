from utils import Point3D , PointCloud
import symbols as sym
import matplotlib.pyplot as plt
from transform import *

threshold = sym.THRESHOLD

threshold_value = 0.05

def main():
	# shape : point cloud


	# pts = [(0,0,0), (1,0,0), (0,1,0), (0,0,1), (1,0,1), (1,1,0), (0,1,1), (1,1,1)]
	n = input("Enter number of points : ")
	compare_modes = [
		# ["ROTATE_EULER_MODE" , sym.ROTATE_EULER_MODE] ,
	 #  	["ROTATE_QUATERNION_MODE" , sym.ROTATE_QUATERNION_MODE], 
	 	["ROTATE_EULER_APPROX_MODE" , sym.ROTATE_EULER_APPROX_MODE], 
	 	["ROTATE_QUATERNION_APPROX_MODE" , sym.ROTATE_QUATERNION_APPROX_MODE ]
	 	]

	num_op_options = 1000

	
	# shape = PointCloud(pts, n)
	for mode in compare_modes:
		# define the series of transformations

		shape = PointCloud(int(n))
		# print(mode[1])

		transform_ops = [(mode[1], (sym.ROTATE_X, 1) ) for i in range(num_op_options)]


		pt = Point3D(1, 1, 1)
		# print(transform(pt, sym.ROTATE_EULER_MODE , (sym.ROTATE_X, 45)))
		
		volumes = []

		# perform transformation

		orig_volume = shape.get_volume()

		normalize = False
		idx = -1
		for transform_op in transform_ops:
			
			idx += 1

			shape.transform(transform_op, normalize)
			# print("\n\nDetails : ")
			# shape.print_details()
			v = shape.get_volume()

			normalize = False
			error_percent = abs(v - orig_volume)/orig_volume

			print(error_percent , " % " )
			if( sym.THRESHOLD and error_percent > threshold_value):
				print("dwdawdw\n\n")
				shape.recompute(transform_ops, idx)

				# normalize = True
			
			volumes.append(v)


		print(volumes)

		volume_error = []
		volume_error_percent = []
		for i in range(len(volumes)):
			volume_error.append(abs(volumes[i] - orig_volume))
			volume_error_percent.append(abs(volumes[i] - orig_volume)/orig_volume)

		# plot the volumes wrt to degree of rotation

		threshold_label = ""
		if threshold is True:
			threshold_label = "_threshold"

		plt.plot(volume_error)
		plt.xlabel( "rotation angle(degrees)")
		plt.ylabel("volume error")
		plt.title( "(error v/s rotation_angle)")
		# plt.show()
		plt.savefig("figs/" + mode[0] + "(error vs rotation_angle)"+ threshold_label + ".png")
		plt.clf()


		plt.plot(volume_error_percent)
		plt.xlabel( "rotation angle(degrees)")
		plt.ylabel("volume error %")
		plt.title( "(error % v/s rotation_angle)")
		# plt.show()
		plt.savefig("figs/" + mode[0] + "(error_percent vs rotation_angle)"+ threshold_label + ".png")
		plt.clf()


		plt.plot(volumes)
		plt.xlabel("rotation angle(degrees)")
		plt.ylabel("volume")
		plt.title("(volumes v/s rotation_angle)")
		# plt.show()
		plt.savefig("figs/" + mode[0] + "(volumes vs rotation_angle)" + threshold_label + ".png")
		plt.clf()


if __name__ == '__main__':
	main()