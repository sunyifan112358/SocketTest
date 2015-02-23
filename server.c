#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *handle_thread(void *client_socket)
{
	char buf[BUFSIZ];
	unsigned int len;
	int client_sockfd = *(int *)client_socket;

	while ((len = recv(client_sockfd, buf, BUFSIZ, 0)) > 0)
	{
		buf[len] = '\0';
		printf("%s\n", buf);

		if (send(client_sockfd, buf, len, 0) < 0)
		{
			perror("write");
			exit(1);
		}
	}

	close(client_sockfd);

	return NULL;
}

int main()
{	
	int server_sockfd;
	int client_sockfd[100];
	pthread_t thread[100];
	int count = 0;

	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(8000);

	unsigned int len;
	char buf[BUFSIZ];

	unsigned int sin_size;

	if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}

	if (bind(server_sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind");
		return 1;
	}

	listen(server_sockfd, 5);
	printf("After listen\n");

	sin_size = sizeof(struct sockaddr_in);
	while (count < 100)
	{
		if ((client_sockfd[count] = accept(server_sockfd, (struct sockaddr *)&remote_addr, &sin_size)) < 0)
		{
			perror("accpet");
			return 1;
		}

		if (pthread_create(&thread[count], NULL, handle_thread, &client_sockfd[count]))
		{
			perror("Thread");
			return 1;
		}
		count ++;
		printf("Accept client %s\n", inet_ntoa(remote_addr.sin_addr));
	}

	close(server_sockfd);
	return 0;
	
}
