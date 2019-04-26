import symbols as sym
import math 
from quat import Quaternion
import time
import config
threshold = sym.THRESHOLD
import rotation_euler


def transform(pt, mode, mode_info, normalize=False):
	ret = None
	if mode == sym.TRANSLATE_MODE:
		ret = translate3D(pt, mode_info[0], mode_info[1], mode_info[2])
	elif mode == sym.ROTATE_EULER_MODE: 
		ret = rotate3DEuler(pt, mode_info[0], mode_info[1])
	elif mode == sym.ROTATE_QUATERNION_MODE: 
		ret = rotate3DQuaternion(pt, mode_info[0], mode_info[1])
	elif mode == sym.ROTATE_EULER_APPROX_MODE: 
		ret = rotate3DEulerApprox(pt, mode_info[0], mode_info[1], normalize)
	elif mode == sym.ROTATE_QUATERNION_APPROX_MODE: 
		ret = rotate3DQuaternionApprox(pt, mode_info[0], mode_info[1], normalize)
	elif mode == sym.SCALE_MODE:
		ret = scale3D(pt, mode_info[0], mode_info[1], mode_info[2])
	else:
		print("Invalid from here MODE")
	return ret

def transformAllPoints(pts, transform_op, normalize=False):
	mode = transform_op[0]
	mode_info = transform_op[1]
	temp = []
	for pt in pts:
		temp.append(transform(pt, mode, mode_info, normalize))
	
	return temp

def translate3D(cartesianPt, delx , dely, delz):
	cartesianPt.x += delx;
	cartesianPt.y += dely;
	cartesianPt.z += delz;
	return cartesianPt

def rotate3DEuler(cartesianPt, rotType, angle):
	d = -1*(3.14159265359*angle)/180;
	t = None
	cos = math.cos(d)
	sin = math.sin(d)
	# print(cartesianPt)

	if(rotType == sym.ROTATE_X):

		t1  = (cartesianPt.y * cos - cartesianPt.z * sin);
		t2  = (cartesianPt.y * sin + cartesianPt.z * cos);
		# config.value += time.time() - a
		t100 = rotation_euler.time_taken1(int(cartesianPt.y),int(cartesianPt.z), 0, angle)
		config.value += t100

		t = (cartesianPt.x, t1, t2)

	elif(rotType == sym.ROTATE_Y):
		t1 = (cartesianPt.z * cos - cartesianPt.x * sin);
		t2 = (cartesianPt.z * sin + cartesianPt.x * cos);
		cartesianPt.z = t1
		cartesianPt.x = t2
		t = (t2, cartesianPt.y, t1)
	elif(rotType == sym.ROTATE_Z):
		t1= (cartesianPt.x * cos - cartesianPt.y * sin);
		t2 = (cartesianPt.x * sin + cartesianPt.y * cos);
		cartesianPt.x = t1
		cartesianPt.y = t2
		t = (t1, t2, cartesianPt.z)


	else:
		print("Invalid rotate mode")


	# print("t  ---> " , t)
	return t

def quaternionConjugate(q): 	
	return Quaternion(q.s, -1*q.x, -1*q.y, -1*q.z)

def quaternionMult(quat1,quat2):
	u = Quaternion(
		quat1.s*quat2.s - quat1.x*quat2.x - quat1.y*quat2.y - quat1.z*quat2.z, \
		quat2.s*quat1.x + quat1.s*quat2.x + quat1.y*quat2.z - quat1.z*quat2.y, \
		quat2.s*quat1.y + quat1.s*quat2.y + quat1.z*quat2.x - quat1.x*quat2.z, \
		quat2.s*quat1.z + quat1.s*quat2.z + quat1.x*quat2.y - quat1.y*quat2.x)
	return u;

def rotate3DQuaternion(cartesianPt, rotType, angle):
	axis = (1, 0, 0);	
	angle=-(3.14159265359*angle)/180;
	
	q = Quaternion(math.cos(angle/2.0), math.sin(angle/2.0)*axis[0], math.sin(angle/2.0)*axis[1], math.sin(angle/2.0)*axis[2])
	quaternionPt = Quaternion(0, cartesianPt.x, cartesianPt.y, cartesianPt.z)
	qstar = quaternionConjugate(q);
	quaternionPt = quaternionMult(q,quaternionPt);
	quaternionPt = quaternionMult(quaternionPt,qstar);
	t = (quaternionPt.x, quaternionPt.y, quaternionPt.z)

	return t

def rotate3DEulerApprox(cartesianPt, rotType, angle, normalize=False):
	d=-(3.14159265359*angle)/180;

	r = 1.0
	if threshold  and (normalize is True):
		r = math.sqrt(1 + d*d)

	t = None
	if(rotType == sym.ROTATE_X):
		# a = time.time()
		t1  = (cartesianPt.y  - cartesianPt.z * d);
		t2  = (cartesianPt.y * d + cartesianPt.z);
		# config.value += time.time() - a
		t100 = rotation_euler.time_taken2(int(cartesianPt.y),int(cartesianPt.z), 0, angle)
		# print("Approx : ", t100 )
		config.value += t100
		cartesianPt.y = t1
		cartesianPt.z = t2
		t = (cartesianPt.x, t1, t2)

	elif(rotType == sym.ROTATE_Y):
		t1 = (cartesianPt.z  - cartesianPt.x * d);
		t2 = (cartesianPt.z * d + cartesianPt.x );
		cartesianPt.z = t1
		cartesianPt.x = t2
		t = (t2, cartesianPt.y, t1)


	elif(rotType == sym.ROTATE_Z):
		t1= (cartesianPt.x  - cartesianPt.y * d);
		t2 = (cartesianPt.x * d + cartesianPt.y );
		cartesianPt.x = t1
		cartesianPt.y = t2
		t = (t1, t2, cartesianPt.z)

	else:
	 	print("Invalid rotate mode")

	return t

def mod_q(q):
	r = math.sqrt(q.s*q.s + q.x*q.x + q.y*q.y + q.z*q.z)
	return Quaternion(q.s/r, q.x/r , q.y/r, q.z/r)

def rotate3DQuaternionApprox(cartesianPt, rotType, angle, normalize=False):
	
	# TODO : rotType ka use karke quaternion define karna h

	# print(normalize, threshold)

	axis = (1, 0, 0);	
	d=-(3.14159265359*angle)/180;
	q = Quaternion(1, (d/2.0)*axis[0], (d/2.0)*axis[1], (d/2.0)*axis[2])
	if threshold and normalize:
		# print("here")
		# print(q.s, q.x, q.y, q.z)
		q = mod_q(q)
		# print(q.s, q.x, q.y, q.z)


	quaternionPt = Quaternion(0, cartesianPt.x, cartesianPt.y, cartesianPt.z)
	qstar = quaternionConjugate(q)
	quaternionPt = quaternionMult(q,quaternionPt)
	quaternionPt = quaternionMult(quaternionPt,qstar)
	t = (quaternionPt.x, quaternionPt.y, quaternionPt.z)
	return t

def scale3D(cartesianPt, sx, sy, sz):
	cartesianPt.x *= sx;
	cartesianPt.y *= sy;
	cartesianPt.z *= sz;
	return cartesianPt.L