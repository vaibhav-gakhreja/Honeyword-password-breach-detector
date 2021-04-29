#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//this program takes three arguments, username, index of the sugarword in the set of sweetwords for this user 
//and the mode of operation of program i.e. to authenticate user or to add a new user.

int main(int argc,char **argv)
{
	//print error if all arguments are not provided
	if(argc<4)
	{
		printf("Incorrect syntax, correct usage : ./honeyServer username index_of_sugarword type_of_request\n");
		exit(1);
	}
	char *username=argv[1];
	char *idx=argv[2];
	char *requestType=argv[3];
	//it handles two types of request : add a user or authenticate a user
	char *requests[2]={"add","auth"};
	if(strncmp(requestType,requests[0],strlen(requests[0]))==0)
	{
		//add request
		char dataToAppend[1000];
		for(int i=0;i<strlen(username);i++)
		{
			dataToAppend[i]=username[i];
		}
		dataToAppend[strlen(username)]=' ';
		for(int i=strlen(username)+1;i<strlen(username)+1+strlen(idx);i++)
		{
			dataToAppend[i]=idx[i-(strlen(username)+1)];
		}
		dataToAppend[strlen(username)+1+strlen(idx)]='\n';
		dataToAppend[strlen(username)+2+strlen(idx)]='\0';
		//append the username,index into honeyserverData table
		FILE *fptr;
		fptr=fopen("honeyserverData.txt","a");
		fputs(dataToAppend,fptr);
		fclose(fptr);
	}else if(strncmp(requestType,requests[1],strlen(requests[1]))==0)
	{
		//check request
		char dataToCheck[1000];
		for(int i=0;i<strlen(username);i++)
		{
			dataToCheck[i]=username[i];
		}
		dataToCheck[strlen(username)]=' ';
		for(int i=strlen(username)+1;i<strlen(username)+1+strlen(idx);i++)
		{
			dataToCheck[i]=idx[i-(strlen(username)+1)];
		}
		dataToCheck[strlen(username)+1+strlen(idx)]='\n';
		dataToCheck[strlen(username)+2+strlen(idx)]='\0';
		FILE *fptr,*fptr1;
		fptr=fopen("honeyserverData.txt","r");
		fptr1=fopen("result.txt","w");
		char line[1000];
		//check if there is an entry <username,index> in the table
		while (fgets(line, sizeof(line), fptr))
		{
			if(strncmp(line,dataToCheck,strlen(dataToCheck))==0)
			{
				//if entry is found, then it is an authentic user
				fprintf(fptr1,"this is an authentic user\n");
				return 0;
			}
		}
		fclose(fptr);
		//if no entry is found, that means the index was of a honeyword
		fprintf(fptr1,"password file compromise detected : the user is using a honeyword as a password, so take appropriate action(block this account or block the entire system)\n");
		fclose(fptr1);
		return 0;
	}else
	{
		//any request other than add or auth is not supported.
		printf("this request is not supported\n");
		exit(1);
	}
}
