#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int client_sockfd;
	unsigned int len;
	struct sockaddr_in remote_addr;
	char buf[BUFSIZ];
	memset(&remote_addr, 0, sizeof(remote_addr));
	remote_addr.sin_family = AF_INET;

	char ip_address[] = "127.0.0.1";
	remote_addr.sin_addr.s_addr = inet_addr(ip_address);
	remote_addr.sin_port = htons(8000);

	if((client_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}

	if(connect(client_sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("connect");
		return 1;
	}
	printf("connected to server\n");

	while(1)
	{
		scanf("%s", buf);
		len = send(client_sockfd, buf, strlen(buf), 0);
		len = recv(client_sockfd, buf, BUFSIZ, 0);
		buf[len] = '\0';
		printf("Received:%s\n", buf);
	}
	close(client_sockfd);
	return 0;
}
