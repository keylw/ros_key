#!/usr/bin/env python
import csv
import sys
import rospy
from wada_mwatch.msg import EmgData

class emg_collector():
    def __init__(self):
        self.emg1_wave = []
        self.emg2_wave = []

    def callback(self, data):
        self.emg1_wave += [data.ch1_emg]
        self.emg2_wave += [data.ch2_emg]

    def collect(self):
        rospy.init_node('emg_listener', anonymous=True)
        rospy.Subscriber("emg_data", EmgData, self.callback)
        rospy.spin()

    def write(self, csv_filename):
        with open(csv_filename, mode='w') as csvfile:
            writer = csv.writer(csvfile,
                                delimiter=',',
                                quotechar='|',
                                quoting=csv.QUOTE_MINIMAL)        
            writer.writerow(['EMG1', "EMG2"])
            for e_1, e_2 in zip(self.emg1_wave, self.emg2_wave):
                writer.writerow([e_1, e_2])

def main():
    csv_filename = str(sys.argv[1])
    collector = emg_collector()
    collector.collect()
    collector.write(csv_filename)

if __name__ == '__main__':
    main()