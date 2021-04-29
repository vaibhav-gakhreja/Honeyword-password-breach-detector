#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//this program takes three arguments, username, index of the sugarword in the set of sweetwords for this user 
//and the mode of operation of program i.e. to authenticate user or to add a new user.

//this function sorts the entries of honeychecker server's data table.
//the key used for sorting is the username of the entry
void sortHoneywordData()
{
	FILE *fptr1;
	fptr1=fopen("honeyserverData.txt","r");
	char buffer[1000],line[100][1000],username[100][1000];
	int ind[100];
	int pos=0;
	//read data and store it into variable line
	while(fgets(buffer,sizeof(buffer),fptr1))
	{
		for(int i=0;i<strlen(buffer);i++)
		{
			line[pos][i]=buffer[i];
		}
		line[pos][strlen(buffer)]='\0';
		ind[pos]=pos;
		pos++;
	}
	//extract username from the data
	for(int i=0;i<pos;i++)
	{
		for(int j=0;j<strlen(line[i]);j++)
		{
			if(line[i][j]==' ')
			{
				username[i][j]='\0';
				break;
			}
			username[i][j]=line[i][j];
		}
	}
	//sort the index array using bubble sort
	for(int i=0;i<pos;i++)
	{
		for(int j=0;j<i;j++)
		{
			if(strcmp(username[ind[i]],username[ind[j]])<0)
			{
				int tmp=ind[i];
				ind[i]=ind[j];
				ind[j]=tmp;
			}
		}
	}
	//store the sorted data into tmp
	char tmp[100][1000];
	for(int i=0;i<pos;i++)
	{
		for(int j=0;j<strlen(line[ind[i]]);j++)
		{
			tmp[i][j]=line[ind[i]][j];
		}
		tmp[i][strlen(line[ind[i]])]='\0';
	}
	fclose(fptr1);
	//copy tmp into F1.txt
	fptr1=fopen("honeyserverData.txt","w");
	char dataToWrite[10000];
	int ptr=0;
	for(int i=0;i<pos;i++)
	{
		for(int j=0;j<strlen(tmp[i]);j++)
		{
			dataToWrite[ptr]=tmp[i][j];
			ptr++;
		}
	}
	dataToWrite[ptr]='\0';
	fprintf(fptr1, "%s", dataToWrite);
	fclose(fptr1);
}

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
		//sort the table after adding new data
		sortHoneywordData();
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