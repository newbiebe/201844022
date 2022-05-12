 1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <unistd.h>
  5 #include <arpa/inet.h>
  6 #include <sys/socket.h>
  7
  8 #define BUF_SIZE 1024
  9 void error_handling(char *message);
 10
 11 int main(int argc, char *argv[])
 12 {
 13         int sock;
 14         char message[BUF_SIZE];
 15         int str_len,recv_len,recv_cnt;
 16         struct sockaddr_in serv_adr;
 17
 18         if(argc!=3){
 19                 printf("Usage:%s<IP><port>\n",argv[0]);
 20                 exit(1);
 21         }
 22
 23         sock=socket(PF_INET,SOCK_STREAM,0);
 24         if(sock==-1)
 25                 error_handling("socket() error");
 26
 27         memset(&serv_adr, 0,sizeof(serv_adr));
 28         serv_adr.sin_family=AF_INET;
 29         serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
 30         serv_adr.sin_port=htons(atoi(argv[2]));
 31
 32         if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
 33                 error_handling("connect() error");
 34         else
 35                 puts("Connected...........");
 36
 37         while(1)
 38         {
 39                 fputs("Input message(Q to quit):",stdout);
 40                 fgets(message,BUF_SIZE,stdin);
 41
 42                 if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
 43                         break;
 44
 45                 str_len=write(sock,message,strlen(message));
 46
 47                 recv_len=0;
 48                 while(recv_len<str_len)
 49                 {
 50                         recv_cnt=read(sock,&message[recv_len],BUF_SIZE-1);
 51                         if(recv_cnt==-1)
 52                                 error_handling("read() error!");
 53                         recv_len+=recv_cnt;
 54                 }
 55
 56                 message[recv_len]=0;
 57                 printf("Message from server: %s",message);
 58         }
 59         close(sock);
 60         return 0;
 61 }
 62
 63 void error_handling(char *message)
 64 {
 65         fputs(message,stderr);
 66         fputc('\n',stderr);
 67         exit(1);
 68 }
 69
