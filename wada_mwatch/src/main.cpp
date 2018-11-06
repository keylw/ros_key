/*
* This code is heavily influese by original whs_1
*
*/ 


#include <ros/ros.h>
#include <wada_mwatch/EmgData.h>
#include <wada_mwatch/EmgDataStamped.h>

#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 9099

using namespace std;

class m_watch{
    private:
        int sock;
	    ros::NodeHandle nh;
    	ros::Publisher db_pub;
    public:
        int test[10]; 
        float ch1;
        float ch2;
    
    void spin(){
        ROS_INFO("spin");
        while(ros::ok()){
            double rcvmsg[2]; // todo change
            int n = recv(sock, rcvmsg, sizeof(rcvmsg), 0);

            if(n<0){
				continue;
			}

			double emg1 = rcvmsg[0];
			double emg2 = rcvmsg[1];

            ROS_INFO("recived a message %f ", emg1);

            publish_emg(rcvmsg);
            ros::spinOnce();

        }
        close(sock);
    }

    void publish_emg(double emg[]){
        wada_mwatch::EmgData data;
        wada_mwatch::EmgDataStamped emg_msg;

        ros::Time now = ros::Time::now() + ros::Duration(9*60*60); // GMT +9

        emg_msg.header.stamp = now;
        data.emg1 = emg[0];
        data.emg2 = emg[1];
        emg_msg.data = data;

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
        db_pub=nh.advertise<wada_mwatch::EmgDataStamped> ("emg_data", 1000);
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
