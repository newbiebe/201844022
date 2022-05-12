#include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <unistd.h>
  5 #include <arpa/inet.h>
  6 #include <sys/socket.h>
  7
  8 #define BUF_SIZE 1024
  9 #define OPSZ 4
 10
 11
 12 void error_handling(char *message);
 13 int calculate(int opnum, int opnds[], char oprator);
 14
 15 int main(int argc, char *argv[])
 16 {
 17         int serv_sock, clnt_sock;
 18         char opinfo[BUF_SIZE];
 19         int result,opnd_cnt,i;
 20         int recv_cnt,recv_len;
 21         struct sockaddr_in serv_adr, clnt_adr;
 22         socklen_t clnt_adr_sz;
 23
 24         if(argc!=2){
 25                 printf("Usage:%s<port>\n",argv[0]);
 26                 exit(1);
 27         }
 28
 29         serv_sock=socket(PF_INET,SOCK_STREAM,0);
 30         if(serv_sock==-1)
 31                 error_handling("socket() error");
 32
 33         memset(&serv_adr, 0,sizeof(serv_adr));
 34         serv_adr.sin_family=AF_INET;
 35         serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
 36         serv_adr.sin_port=htons(atoi(argv[1]));
 37
 38         if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
 39                 error_handling("bind() error");
 40
 41         if(listen(serv_sock,5)==-1)
 42                  error_handling("listen() error");
 43
 44         clnt_adr_sz=sizeof(clnt_adr);
 45
 46         for(i=0;i<5;i++)
 47         {
 48                 opnd_cnt=0;
 49                 clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt    _adr_sz);
 50                 read(clnt_sock,&opnd_cnt,1);
 51                 recv_len=0;
 52
 53                 while((opnd_cnt*OPSZ+1)>recv_len)
 54                 {
 55                         recv_cnt=read(clnt_sock, &opinfo[recv_len],BUF_SIZE-    1);
 56                         recv_len+=recv_cnt;
 57                 }
 58                 result=calculate(opnd_cnt,(int*)opinfo,opinfo[recv_len-1]);
 59                 write(clnt_sock,(char*)&result,sizeof(result));
 60                 close(clnt_sock);
 61         }
 62         close(serv_sock);
 63         return 0;
 64 }
 int calculate(int opnum, int opnds[], char op)
 66 {
 67         int result=opnds[0],i;
 68
 69         switch(op)
 70         {
 71                 case'+':
 72                         for(i=1; i<opnum; i++) result+=opnds[i];
 73                         break;
 74                 case'-':
 75                         for(i=1; i<opnum; i++) result-=opnds[i];
 76                         break;
 77                 case'*':
 78                          for(i=1; i<opnum; i++) result*=opnds[i];
 79                          break;
 80         }
 81         return result;
 82 }
 83 void error_handling(char *message)
 84 {
 85         fputs(message,stderr);
 86         fputc('\n',stderr);
 87         exit(1);
 88 }
