from utils import Point3D , PointCloud
import symbols as sym
import matplotlib.pyplot as plt
from transform import *
import time
import os
import config
import rotation_euler

import pickle

threshold = sym.THRESHOLD

threshold_values = [0.08]
# threshold_values = [ 0.001, 0.002, 0.005, 0.01, 0.025, 0.05, 0.1 , 0.2,  0.5, 0.9 ]
# threshold_values = [ i for i in range(10)]

def cumulative(lists): 
    cu_list = [] 
    length = len(lists) 
    cu_list = [sum(lists[0 : x+1]) for x in range(0, length)] 
    return cu_list 

def main():
	# shape : point cloud


	# pts = [(0,0,0), (1,0,0), (0,1,0), (0,0,1), (1,0,1), (1,1,0), (0,1,1), (1,1,1)]
	n = input("Enter number of points : ")
	compare_modes = [
		# ["ROTATE_EULER_MODE" , sym.ROTATE_EULER_MODE] ,
	 #  	["ROTATE_QUATERNION_MODE" , sym.ROTATE_QUATERNION_MODE], 
	 	["ROTATE_EULER_APPROX_MODE" , sym.ROTATE_EULER_APPROX_MODE], 
	 	# ["ROTATE_QUATERNION_APPROX_MODE" , sym.ROTATE_QUATERNION_APPROX_MODE ]
	 	]

	total_rotation = 360
	
	shape = PointCloud(int(n))
	# shape = PointCloud(pts, n)
	for mode in compare_modes:
		# define the series of transformations

		print(rotation_euler.time_taken1(1,1,0,10))
		print(rotation_euler.time_taken2(1,1,0,10))

		# exit(0)

		max_performances_each_thresh = []
		min_time_each_thresh = []

		for threshold_value in threshold_values:
			performances_each_angle = []
			time_each_angle = []

			for angle_aux in range(1,100):
				shape.restore();
				small_angle = float(angle_aux)/10



				num_op_options = int(total_rotation/small_angle)
				transform_ops = [(mode[1], (sym.ROTATE_X, small_angle)) for i in range(num_op_options)]

				# print(transform(pt, sym.ROTATE_EULER_MODE , (sym.ROTATE_X, 45)))
				
				volumes = []
				times_list = []
				# perform transformation

				orig_volume = shape.get_volume()

				normalize = False
				idx = -1
				for transform_op in transform_ops:
					
					idx += 1

					# begin_time = time.time()
					config.value=0
					shape.transform(transform_op, normalize)
					# end_time = time.time()
					# print(config.value,"@@@@@@@@@")
					times_list.append(config.value)
					# times_list.append(10**(-9))

					v = shape.get_volume()
					volumes.append(v)

					normalize = False
					error_percent = abs(v - orig_volume)/orig_volume

					if(idx % 500 == 0):
						print("At step " , idx , error_percent , " % " )
					if( sym.THRESHOLD and error_percent > threshold_value):
						# print("dwdawdw\n\n")
						
						config.value=0
						shape.recompute(transform_ops, idx)
						times_list.append(config.value)

						# 3e-05
						# 1e-06


						v = shape.get_volume()
						volumes.append(v)


						# normalize = True
					


				# print(volumes)

				volume_error = []
				volume_error_percent = []
				for i in range(len(volumes)):
					volume_error.append(abs(volumes[i] - orig_volume))
					volume_error_percent.append(abs(volumes[i] - orig_volume)/orig_volume)

				# plot the volumes wrt to degree of rotation

				threshold_label = ""
				if threshold is True:
					threshold_label = "_threshold"

				if( not os.path.exists("./figs/" + str(angle_aux))):
					os.mkdir("./figs/" + str(angle_aux))

				plt.plot(volume_error)
				plt.xlabel( "rotation angle(degrees)")
				plt.ylabel("volume error")
				plt.title( "(volume error v/s rotation_angle)")
				# plt.show()
				plt.savefig("figs/" + str(angle_aux) + "/"  + mode[0] + "(error vs rotation_angle)"+ threshold_label + ".png")
				plt.clf()


				plt.plot(volume_error_percent)
				plt.xlabel( "rotation angle(degrees)")
				plt.ylabel("volume error %")
				plt.title( "(volume error % v/s rotation_angle)")
				# plt.show()
				plt.savefig("figs/" + str(angle_aux) + "/" + mode[0] + "(error_percent vs rotation_angle)"+ threshold_label + ".png")
				plt.clf()


				plt.plot(cumulative(times_list), volume_error_percent)
				plt.xlabel("rotation angle(degrees)")
				plt.ylabel("volume error %")
				plt.title( "(volume error % v/s time_taken)")
				# plt.show()
				plt.savefig("figs/" + str(angle_aux) + "/" + mode[0] + "(error_percent vs time_taken)"+ threshold_label + ".png")
				plt.clf()


				# plt.plot(volume_error_percent, cumulative(times_list))
				# plt.xlabel("rotation angle(degrees)")
				# plt.ylabel("volume error %")
				# plt.title( "(error % v/s time_taken)")
				# # plt.show()
				# plt.savefig("figs/" + str(angle_aux) + "/" + mode[0] + "(error_percent vs time_taken)"+ threshold_label + ".png")
				# plt.clf()


				plt.plot(volumes)
				plt.xlabel("rotation angle(degrees)")
				plt.ylabel("volume")
				plt.title("(volume v/s rotation_angle)")
				# plt.show()
				plt.savefig("figs/" + str(angle_aux) + "/" + mode[0] + "(volumes vs rotation_angle)" + threshold_label + ".png")
				plt.clf()

				performances_each_angle.append(float(1)/sum(times_list))
				time_each_angle.append(sum(times_list)/(num_op_options*small_angle))

			


			if( not os.path.exists("./figs/thresh" + str(threshold_value))):
					os.mkdir("./figs/thresh" + str(threshold_value))


			plt.plot( [float(i)/10 for i in range(1,100)], performances_each_angle)
			plt.xlabel("small angle")
			plt.ylabel("performance")
			plt.title("(performances_each_angle v/s small_angle)")
			plt.savefig("figs/thresh" + str(threshold_value)+ "/" + mode[0] + "scatter(performances_each_angle vs small_angle)" + threshold_label + ".png")
			plt.clf()


			plt.scatter([float(i)/10 for i in range(1,100)], performances_each_angle, alpha=0.1)
			plt.xlabel("small angle")
			plt.ylabel("performance")
			plt.title("(performances_each_angle v/s small_angle)")
			plt.savefig("figs/thresh" + str(threshold_value)+ "/" + mode[0] + "(performances_each_angle vs small_angle)" + threshold_label + ".png")
			plt.clf()

			plt.plot([float(i)/10 for i in range(1,100)], time_each_angle)
			plt.xlabel("small angle")
			plt.ylabel("amortized cost(in nsec)")
			plt.title("(amortized cost v/s small_angle)")
			plt.savefig("figs/thresh" + str(threshold_value) + "/" + mode[0] + "(amortized cost vs small_angle)" + threshold_label + ".png")
			plt.clf()

			print(performances_each_angle)
			print(time_each_angle)




			fp = open("figs/thresh" + str(threshold_value)+ "/" + mode[0] + "performances_each_angle.pkl", "wb")
			pickle.dump(performances_each_angle, fp)
			fp.close()







if __name__ == '__main__':
	main()