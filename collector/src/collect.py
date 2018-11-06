#!/usr/bin/env python

import rospy
from tms_msg_db.msg import Tmsdb
from tms_msg_db.msg import TmsdbStamped



def callback(data):
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.tmsdb[0].name)

def collect():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("tms_db_data", TmsdbStamped, callback)

    rospy.spin()

def main():
    collect()

if __name__ == '__main__':
    main()