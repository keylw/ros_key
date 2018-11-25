/*
* 
* This code is heavily influese by tms/whs_1
*
*/ 
#include <ros/ros.h>
#include <wada_mwatch/EmgData.h>

#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <cstdlib>
#include <pthread.h>

#include <boost/date_time/posix_time/posix_time.hpp>


#define PORT 65055

using namespace std;

class m_watch{
    private:
        int sock;
	    ros::NodeHandle nh;
    	ros::Publisher db_pub;
    public:
        float ch1;
        float ch2;
        bool stream_info = false;
    
    void spin(){
        ROS_INFO("spin");
        double rcvmsg[2];
        while(ros::ok()){

            if( 1 > recv(sock, rcvmsg, sizeof(rcvmsg), 0)){
				continue;
			}

            if(stream_info){
                ROS_INFO("recived a message %f and %f ", rcvmsg[0], rcvmsg[1]);
            }

            publish_emg(rcvmsg);
            ros::spinOnce();

        }
        close(sock);
    }

    void publish_emg(double emg[]){
        wada_mwatch::EmgData emg_msg;
        ros::Time now = ros::Time::now() + ros::Duration(9*60*60); // GMT +9
        std::string frame_id("/world");

		emg_msg.header.frame_id = frame_id;
        emg_msg.header.stamp = now;
        emg_msg.ch1_emg = emg[0];
        emg_msg.ch2_emg = emg[1];

        db_pub.publish(emg_msg);
    }

    void setup_socket(){
		sock = socket(AF_INET,SOCK_DGRAM,0);
		struct sockaddr_in s_address;
		s_address.sin_family=AF_INET;
		s_address.sin_addr.s_addr=INADDR_ANY;
		s_address.sin_port=htons(PORT);
		const int on = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
		bind(sock,(struct sockaddr *)&s_address,sizeof(s_address));
		int val=1;
		ioctl(sock,FIONBIO,&val);
		ROS_INFO("m_watch ready at port %d!! ...", PORT);
    }

    m_watch(){
        db_pub=nh.advertise<wada_mwatch::EmgData> ("emg_data", 1000);
        setup_socket();
    }
};


int main(int argc, char **argv)
{
	ros::init(argc,argv,"m_watch");
	m_watch emg;

	emg.spin();
	return 0;
}
