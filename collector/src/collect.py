#!/usr/bin/env python

import rospy
import rosbag
from my_beat.msg import EcgData
import csv
import sys


def get_writer(csv_file):
    return csv.writer(csv_file, delimiter=',', quoting=csv.QUOTE_MINIMAL)
    
def write_csv(csv_filename, bag_file):
    with \
        open(csv_filename + "_emg.csv", mode='w') as emg_csv, \
        open(csv_filename + "_ecg.csv", mode='w') as ecg_csv:

        emg_writer = get_writer(emg_csv)
        ecg_writer = get_writer(ecg_csv)

        emg_writer.writerow(['EMG1', "EMG2"])
        ecg_writer.writerow(['ECG', "Temp", "Stamp"])

        topics = ['/emg_data', '/ecg_data']
        ros_bag = rosbag.Bag(bag_file)
    
        for topic, msg, _, in ros_bag.read_messages(topics=topics):
            if(topic == '/ecg_data'):
                ecg_writer.writerow([msg.ecg, msg.temp, msg.header.stamp])
            elif(topic == '/emg_data'):
                emg_writer.writerow([msg.ch1_emg, msg.ch2_emg])


def main():

    csv_filename = str(sys.argv[1])
    bag_file = str(sys.argv[2])

    write_csv(csv_filename, bag_file)

if __name__ == '__main__':
    main()