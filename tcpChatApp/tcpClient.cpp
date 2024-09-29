#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<cstring>
#include<netdb.h>
#include<thread>
#include<csignal>
#include<mutex>
#include<iostream>
using namespace std;
static const size_t bufferSize=1024;

struct Data
{
	int id;
	char message[bufferSize];
};

class Client
{
	
	private:
		  int sockfd;
	      struct addrinfo hints;
	      struct addrinfo *host;
	      char* port;
	      char* address;
	      bool state=true;
	      mutex lock;
	      
	      void receiverTask(int serverfd)
		  {
			 int r;
			 Data data;
			 while(true){
				int r=recv(serverfd,&data,sizeof(data),0);
		        if(r<=0)
		        {
		        	if(r==0)
		        	{
		        	   	fprintf(stdout,"Server Connection Closed....\n");
					}
		   	        else
					{
					     fprintf(stdout,"receiving  failed....\n");
					}
		   	        break;
		        }
		        else{
			       printf("User %d: %s\n",data.id,data.message);
			       memset(data.message,'\0',1024);
			     }
			}
			
			lock_guard<mutex> locker(lock);
			state=false;	
		}
	      
	public:
			
		
		Client(char* address,char* port):address(address),
										 port(port)
		{
			sockfd=-1;
			host=NULL;
		}
		
		bool tcpClientConfiguration()
		{
			
			printf("Configuring server......");
	        memset(&hints,0,sizeof(hints));
	        hints.ai_family=AF_INET;
	        hints.ai_socktype=SOCK_STREAM;
	      
	        char buffer[bufferSize];
			if(getaddrinfo(address,port,&hints,&host)!=0)
			{
				fprintf(stdout,"failed server initialization. for address :%s port:%s\n",address,port);
				return false;
			}
	
			//creating socket
			printf("Socket creation.....\n");
			sockfd=socket(
	   			host->ai_family,
	   			host->ai_socktype,
	   			host->ai_protocol
	   		);
	   
			if(sockfd==-1)
			{
				fprintf(stdin,"failed socket connection ....\n");
				return false;
			}
			

	
			printf("binding socket to the address and port....\n");
	
			
		   int r;
		   if(connect(sockfd,host->ai_addr,host->ai_addrlen)!=0)
		   {
				fprintf(stdout,"Socket connect failed....\n");
				return false;
		   }
				 
		   
		   thread receiver(&Client::receiverTask,this,sockfd);
		   receiver.detach();
		   Data data;
		   while(state){
		   	    
		   		fgets(data.message,sizeof(data.message),stdin);
				{   
					lock_guard<mutex> locker(lock);
					if(!state)
		   			{
		   				
		   		   		printf("connection break;\n");
				   		break;	
			    	}
			   	
		   	
		   			r=send(sockfd,&data,sizeof(data),0);
		   			if(r<=0)
		   			{
		   	    		 printf("-->Socket sending failed....\n");
			        	 break;
		   			}
		   			printf("You : %s",data.message);
		   	   }
		   	}
	       memset(data.message,'\0',1024);
		   return true;
		}
		
		void clean()
		{
			printf("freeing resources\n");
			if(sockfd!=-1) 
			{  
			     printf("sockettt");
				close(sockfd);
			}
			if(host) freeaddrinfo(host);
		}
		
		~Client()
		{
			printf("Destructor\n");
			clean();
		}
};
static Client *ptr=NULL;
void handleSignal(int signal)
{
	      printf("server stopped...");
	      if(ptr) ptr->clean();
	      _exit(0);
}

int main()
{
	signal(SIGTSTP,handleSignal);
	Client tcp("127.0.0.1","8085");
	ptr=&tcp;
	if(tcp.tcpClientConfiguration()==false)
	{
		printf("server configuration failed ....");
	}
	return 0;
}
