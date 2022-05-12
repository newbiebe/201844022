 1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <unistd.h>
  5 #include <arpa/inet.h>
  6 #include <sys/socket.h>
  7
  8 #define BUF_SIZE 1024
  9 #define ALT_SIZE 4
 10 #define OPSZ 4
 11 void error_handling(char *message);
 12
 13 int main(int argc, char *argv[])
 14 {
 15         int sock;
 16         char opmsg[BUF_SIZE];
 17         int result,opnd_cnt,i;
 18         struct sockaddr_in serv_adr;
 19
 20         if(argc!=3){
 21                 printf("Usage:%s<IP><port>\n",argv[0]);
 22                 exit(1);
 23         }
 24
 25         sock=socket(PF_INET,SOCK_STREAM,0);
 26         if(sock==-1)
 27                 error_handling("socket() error");
 28
 29         memset(&serv_adr, 0,sizeof(serv_adr));
 30         serv_adr.sin_family=AF_INET;
 31         serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
 32         serv_adr.sin_port=htons(atoi(argv[2]));
 33
 34         if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
 35                 error_handling("connect() error");
 36         else
 37                 puts("Connected...........");
 38
 39         fputs("Operand count:",stdout);
 40         scanf("%d",&opnd_cnt);
 41         opmsg[0]=(char)opnd_cnt;
 42         for(i=0; i<opnd_cnt; i++)
 43         {
 44                 printf("Operand %d:",i+1);
 45                 scanf("%d",(int*)&opmsg[i*OPSZ+1]);
 46         }
 47         fgetc(stdin);
 48         fputs("Operator:",stdout);
 49         scanf("%c",&opmsg[opnd_cnt*OPSZ+1]);
 50         write(sock,opmsg,opnd_cnt*OPSZ+2);
 51         read(sock,&result,ALT_SIZE);
 52
 53         printf("Operation result:%d\n",result);
 54         close(sock);
 55         return 0;
 56 }
 57
 58 void error_handling(char *message)
 59 {
 60         fputs(message,stderr);
 61         fputc('\n',stderr);
 62         exit(1);
 63 }
