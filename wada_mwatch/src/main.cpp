/*
* This code is heavily influese by original whs_1
*
*/ 


#include <ros/ros.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 64002

using namespace std;

class m_watch{
    private:
        int sock;

    public:
        int test[10]; 
        float ch1;
        float ch2;
    
    void spin(){
        ROS_INFO("spin");
        while(ros::ok()){
            int recv_msg[2]; // todo change
            int n = recv(sock, recv_msg, sizeof(recv_msg), 0);

            if(n<0){
				continue;
			}

            ROS_INFO("recived a message %d ", n);
        }
        close(sock);
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
