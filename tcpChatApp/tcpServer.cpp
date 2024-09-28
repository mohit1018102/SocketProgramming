#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<cstring>
#include<netdb.h>
#include<thread>
#include<csignal>
#include<unordered_set>
#include<mutex>
using namespace std;
static const size_t bufferSize=1024;

struct Data
{
	int id;
	char message[bufferSize];
};

class Server
{
	
	private:
		  int sockfd;
	      struct addrinfo hints;
	      struct addrinfo *server;
	      struct sockaddr clientAddress;
	      socklen_t clientLen;
	      char* port;
	      char* address;
	      
	      unordered_set<int> clients;
	      mutex lock;
	      
	      
		void receiverTask(int clientfd)
		{
			Data data;
			int r;
			while(true){
				int r=recv(clientfd,&data,sizeof(data),0);
		        if(r<=0)
		        {
		        	if(r==0)
		        	{
		        	   	fprintf(stdout,"Client %d connection closed....\n",clientfd);
					}
		   	        else
					{
					     fprintf(stdout,"receiving  failed....\n");
					}
		   	       
					fprintf(stdin,"closing Client: %d\n",clientfd);
					
					lock_guard<mutex> locker(lock);
					if(clients.find(clientfd)!=clients.end())
			        {
		       				printf("client closed\n");		
							clients.erase(clientfd);
							close(clientfd);
		    		}
		    		
		   	        break;
		        }
		        else{
			       printf("User %d: %s\n",clientfd,data.message);
			       data.id=clientfd;
			       lock_guard<mutex> locker(lock);
			       for(auto fd:clients)
					{		
						if(fd!=clientfd)
						{
								r=send(fd,&data,sizeof(data),0);
		   						if(r<=0)
		   						{
		   	    		 			printf("-->Socket sending failed....\n");
		   						}
						}
					}
					memset(data.message,'\0',1024); 
			     }
			}
			
		}
		
	public:
		
		Server(char* address,char* port):address(address),
										 port(port)
		{
			sockfd=-1;
			server=NULL;
		}
		
		bool tcpServerConfiguration()
		{
			int clientfd=-1;
			printf("Configuring server......");
	        memset(&hints,0,sizeof(hints));
	        hints.ai_family=AF_INET;
	        hints.ai_socktype=SOCK_STREAM;

			if(getaddrinfo(address,port,&hints,&server)!=0)
			{
				fprintf(stderr,"failed server initialization. for address :%s port:%s\n",address,port);
				return false;
			}
	
			//creating socket
			printf("Socket creation.....\n");
			sockfd=socket(
	   			server->ai_family,
	   			server->ai_socktype,
	   			server->ai_protocol
	   		);
	   
			if(sockfd==-1)
			{
				fprintf(stderr,"failed socket connection ....\n");
				return false;
			}
			
			printf("binding socket to the address and port....\n");
			if(bind(sockfd,server->ai_addr,server->ai_addrlen)!=0)
			{
				fprintf(stderr,"Socket binding failed....\n");
				return false;
			}
			
			printf("start listening .........\n");
			if(listen(sockfd,5)!=0)
			{
				fprintf(stderr,"Socket listen failed....\n");
				return false;
			}
			
			clientLen=sizeof(clientAddress);
			
			
			while((clientfd=accept(sockfd,&clientAddress,&clientLen))!=-1){
				
				printf("connection accepted...\n");	
				clients.insert(clientfd);
				thread receiver(&Server::receiverTask,this,clientfd);
				receiver.detach();
			}
			
			
		}
		
		
		void clean()
		{
			printf("freeing resources\n");
			if(server) freeaddrinfo(server);
			if(sockfd!=-1) close(sockfd);
			lock_guard<mutex> locker(lock);
			for(auto fd:clients)
			{
				close(fd);
			}
			clients.clear();
			
		}
		
		~Server()
		{
			printf("destructor");
			clean();
		}
};


static Server *ptr=NULL;
void handleSignal(int signal)
{
	      printf("server stopped...");
	      if(ptr) ptr->clean();
	      _exit(0);
}
int main()
{
	signal(SIGTSTP,handleSignal);
	printf("\n--------------Server----------\n");
	Server tcp("127.0.0.1","8085");
	ptr=&tcp;
	if(tcp.tcpServerConfiguration()==false)
	{
		printf("server configuration failed ....");
	}
	return 0;
}
