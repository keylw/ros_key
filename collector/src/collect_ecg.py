#!/usr/bin/env python

import rospy
from my_beat.msg import EcgData
import csv
import sys

class ecg_collector:
    def __init__(self):
        self.ecg_wave = []
        self.acc_x = []
        self.acc_y = []
        self.acc_z = []
        self.temp = []
        self.stamp = []

    def collect(self):
        rospy.init_node('listener', anonymous=True)
        rospy.Subscriber("ecg_data", EcgData, self.callback)
        rospy.spin()

    def callback(self, data):
        self.ecg_wave += [data.ecg]
        self.acc_x += [data.acc_x]
        self.acc_y += [data.acc_y]
        self.acc_z += [data.acc_z]
        self.temp += [data.temp]
        self.stamp += [data.header.stamp]

    def write(self,csv_filename):
        with open(csv_filename, mode='w') as csvfile:
            writer = csv.writer(csvfile,
                    delimiter=',',
                    quoting=csv.QUOTE_MINIMAL)        
            writer.writerow(['ECG', "Temp", "Stamp"])
            for ecg, t , stamp in zip(self.ecg_wave, self.temp, self.stamp):
                writer.writerow([ecg, t, stamp])

def main():
    csv_filename = str(sys.argv[1])
    collector = ecg_collector()
    collector.collect()
    collector.write(csv_filename)

if __name__ == '__main__':
    main()