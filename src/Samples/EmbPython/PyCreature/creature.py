#!/usr/bin/env python-
# -*- coding: utf-8 -*

##from Test import * 

import sys
import time
import math
import threading



class Creature(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		self.i = 0

	def run(self):
		print("Start creature thread")
		#time.sleep(5)

		right_hand.SetViewArea(Posed(1,0,0,0,0,0,0), 2, 2)

		last_pos = Vec3d(0,0,0)
		dtInt = 0
		dt = scene.GetTimeStep()
		while True:
			dtInt += dt
			curr_pos = pointer.GetPose().getPos()
			if (curr_pos - last_pos).norm() > 0.0001:
				v = (curr_pos - last_pos).norm() * (1 / dtInt)
				last_pos = curr_pos
				dtInt = 0
				if v > 0.1:
					right_hand.SetTargetPos(pointer.GetPose().getPos())
					right_hand.SetTimeLimit(0.2)
					right_hand.Start()


if __name__ == "__main__":
	
	human1 = Creature()

	