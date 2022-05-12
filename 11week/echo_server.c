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
 13         int serv_sock, clnt_sock;
 14         char message[BUF_SIZE];
 15         int str_len,i;
 16
 17         struct sockaddr_in serv_adr;
 18         struct sockaddr_in clnt_adr;
 19         socklen_t clnt_adr_sz;
 20
 21         if(argc!=2){
 22                 printf("Usage:%s<port>\n",argv[0]);
 23                 exit(1);
 24         }
 25
 26         serv_sock=socket(PF_INET,SOCK_STREAM,0);
 27         if(serv_sock==-1)
 28                 error_handling("socket() error");
 29
 30         memset(&serv_adr, 0,sizeof(serv_adr));
 31         serv_adr.sin_family=AF_INET;
 32         serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
 33         serv_adr.sin_port=htons(atoi(argv[1]));
 34
 35         if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
 36                 error_handling("bind() error");
 37
 38         if(listen(serv_sock,5)==-1)
 39                  error_handling("listen() error");
 40
 41         clnt_adr_sz=sizeof(clnt_adr);
 42
 43         for(i=0;i<5;i++)
 44         {
 45                 clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt    _adr_sz);
 46                 if(clnt_sock==-1)
 47                         error_handling("accept() error");
 48                 else
 49                         printf("Connected client:%d\n",i+1);
 50                 while((str_len=read(clnt_sock,message,BUF_SIZE))!=0)
 51                         write(clnt_sock,message,str_len);
 52                 close(clnt_sock);
 53         }
 54         close(serv_sock);
 55         return 0;
 56 }
 57 void error_handling(char *message)
 58 {
 59         fputs(message,stderr);
 60         fputc('\n',stderr);
 61         exit(1);
 62 }
 63
