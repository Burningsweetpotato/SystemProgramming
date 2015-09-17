#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/wait.h>
#include<sys/file.h>
#include<sys/stat.h>

void execmd(char input[])
{
		
                char *token;
                char *result[256];
               
		if(strstr(input,"cd")){
			printf("into cd command\n");
              		char *token = strtok(input," ");
              		token = strtok(NULL," ");
        	        if(chdir(token) < 0){
	                      perror("wrong directory");
	                }       

		}
		else{
		        token = strtok(input," ");
		        result[0] = (char*)malloc(strlen(token)+1);
			strcpy(result[0],token);
		        int i=1;
		        while(token=strtok(NULL," ")){
		               result[i]=(char *)malloc(strlen(token)+1);
		               strcpy(result[i],token);
		               i++;
		        }
		        result[i] = NULL;
			execvp(result[0],result);				
		}

 
}


void pipefunction(char input[],int amp){
	//printf("input : %s\n",input);
	char *tok; 
	char *commandtok ;

	char *firstcommand[256];
	char firsttemp[256];

	char secondcommand[256];
	char *secondcmd[256];

	memset(firsttemp,0,256);
	memset(secondcommand,0,256);

	tok = strtok(input,"|");
	commandtok = strtok(NULL, "");

	//printf("tok : %s\n",tok);
	//printf("commandtok : %s\n",commandtok);

	strcpy(firsttemp,tok);
	//printf("firsttemp : %s\n",firsttemp);
	firstcommand[0] = (char*)malloc(strlen(firsttemp)+1);
        firstcommand[0] = firsttemp;
	tok = strtok(firsttemp," ");
	
	int i = 1;
	while(tok=strtok(NULL," ")){
		     	      
                      firstcommand[i]=(char*)malloc(strlen(tok)+1);
                      strcpy(firstcommand[i],tok);
		      
                      i++;
                }
	
	 firstcommand[i] = NULL;
	 
	
	 strcpy(secondcommand,commandtok);
	
	
        int p[2];
        pipe(p);
	int stat = 0;
        if(fork() == 0)
        {
                close(1);
                dup(p[1]);
                close(p[0]);
                close(p[1]); 
		execvp(firstcommand[0],firstcommand);
                perror("error1");
        }
       
	if(strstr(secondcommand,"|")){		
		if(fork() == 0){			
			pipefunction(secondcommand,amp);
		}
		
	}
	else{
		
		commandtok = strtok(secondcommand," ");
	
                secondcmd[0] = (char*)malloc(strlen(commandtok)+1);
                strcpy(secondcmd[0],commandtok);
                
		int i = 1;
                while(commandtok=strtok(NULL," ")){
		    
                     secondcmd[i]=(char *)malloc(strlen(commandtok)+1);
                     strcpy(secondcmd[i],commandtok);
                     i++;
                }
		secondcmd[i] = NULL; 

		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]); 
               
		execvp(secondcmd[0],secondcmd);
		perror("error2");
	}



}

void multiplecmd(char input[],int amp)
{
	 		
		char *token;
                char *token1;

		char p[256];
		int stat = 0;
		memset(p,0,256);
		if(strstr(input,";")){
		
		       token = strtok(input,";");
		       printf("%s\n",token);
		       		
			if(fork() == 0){		       
		       		execmd(token);
			}
		       wait(&stat);
		       if(token1= strtok(NULL,"")){
			        strcpy(p,token1);
				multiplecmd(token1,amp);
			}			
		       
		}
		else{
			
			execmd(input);
		}

		
		
}
void input_redirect(char input[]){
                int fd;
                char *tok;
                char path[256];
                char command[256];

                memset(path,0,256);
                memset(command,0,256);


                tok=strtok(input,"<");
                strcpy(command,tok);
		

                tok = strtok(NULL," ");
		if(tok[0] == ' '){
	                strcpy(path,tok+1);			
		}
		else{
			
		      strcpy(path,tok);		
		}

                fd = open(path,O_RDONLY);
                close(0);
                dup(fd);
                close(fd);
                execmd(command);

}


void owrite_redirect(char input[]){
		int fd;
                char *tok;
                char path[256];
                char command[256];

                memset(path,0,256);
                memset(command,0,256);


                tok=strtok(input,">");
                strcpy(command,tok);

                tok = strtok(NULL,"!");
		if(tok[0] == ' '){
	                strcpy(path,tok+1);			
		}
		else{
			
		      strcpy(path,tok);		
		}
              
	
                fd = open(path,O_RDWR|O_CREAT|O_TRUNC);		
                close(1);
                dup(fd);
                close(fd);
                execmd(command);
		


              

}
void append_redirect(char input[]){
		//printf("!1\n");
                int fd;
                char *tok;
                char path[256];
                char command[256];

                memset(path,0,256);
                memset(command,0,256);


                tok=strtok(input,">");
		//printf("!2%s\n",tok);
                strcpy(command,tok);
		
                tok = strtok(NULL,">");
		//printf("!3%s\n",tok);
		if(tok[0] == ' '){
	                strcpy(path,tok+1);			
		}
		else{
			
		      strcpy(path,tok);		
		}
		//printf("!4%s\n",path);
                fd = open(path,O_RDWR|O_CREAT|O_APPEND);
                close(1);
                dup(fd);
                close(fd);
                execmd(command);

          


}

void out_redirect(char input[]){
                int fd;
                char *tok;
		char path[256];
		char command[256];

		memset(path,0,256);
		memset(command,0,256);


                tok=strtok(input,">");
	        strcpy(command,tok);
		
                tok = strtok(NULL," ");
		if(tok[0] == ' '){
	                strcpy(path,tok+1);			
		}
		else{
			
		      strcpy(path,tok);		
		}
                
   

                fd = open(path,O_WRONLY|O_CREAT);
                close(1);
                dup(fd);
                close(fd);
                execmd(command);
		

}

void parser(char input[],int amp)
{
     if(fork() == 0){
		if(strcmp(input,"history")== 0){

			FILE *f;
			f=fopen("history.txt","r");
			char tmp[256];
			int hiscount= 1;
			while(fgets(tmp,256,f)!=NULL){
				printf("%d: %s\n",hiscount,tmp);
				hiscount ++;
		}
			fclose(f);		
		}

		if(strstr(input,";")){
			multiplecmd(input,amp);
		
		}

		else if(strchr(input,'>') ||strstr(input,"<")){
			if(strstr(input,">>")){
				append_redirect(input);
			}
			else if(strstr(input,">!")){
				owrite_redirect(input);
			}
			else if(strstr(input,"<")){
				input_redirect(input);
			}
			else{
				out_redirect(input);
			}		
		}
		else if(strstr(input,"|")){
		 	
			pipefunction(input,amp);
		}
	
		else{
			execmd(input);
		}
	
	}

	if(amp == 0){
		wait(&amp);
	}
}

//main 함수
int main()
{
	char buf[256]; //for getcwd
	char input[256]; //입력한 값을 저장해두는 곳
	char tempchar;
	char *token;
	int inputindex = 0; //
	int history = 1;

	int stat = 0;

 //초기화

	while(1){
		int amp = 0;
		
		memset(input,0,256);//input array 초기화
		inputindex = 0;//

		getcwd(buf,256);
		printf("%s >> ",buf); //minishell 처음부분 입력을 받는곳
		
		
		while((tempchar = getchar()) !=10){
			//printf("tempchar : %d\n",tempchar);
			input[inputindex] = tempchar; //input한 char를 input에 배정
			inputindex ++;
		}
		if(strcmp(input,"exit") == 0){
                        printf("bye\n"); //exit if command is 'exit'
                        exit(1);
                }
		
	

		else{
			FILE *f;

			f=fopen("history.txt","a");
			if(f==NULL){
				perror("can't open file");
			}
			fprintf(f, "%s\n",input);
			fclose(f);
		        
			 if(strstr(input,"&")){
                        	amp = 1;	                       
				char *tok = strtok(input,"&");		

	                }	
					
	 			parser(input,amp);
			}
			if(amp == 0){
				wait(&amp);
			}					
			


		}
	
}

