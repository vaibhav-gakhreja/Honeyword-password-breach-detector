#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>
#define maxn 2000
//assumption : there are atleast k-1 users registered before the start of program
int indicesPresent[maxn],randomIndices[maxn];
int idx,numIndices;
//idx : randomly generated index number assigned to the passords.
//numIndices : number of entries already present in table F2, i.e. the number of preexisting passwords.

//this function converts integer num into character array buffer.
void integerToCharPointer(int num,char *buffer)
{
	char tmp[20];
	int pos=0;
	while(num>0)
	{
		tmp[pos]=(char)(num%10+'0');
		num/=10;
		pos++;
	}
	tmp[pos]='\0';
	for(int i=0;i<pos;i++)
	{
		buffer[i]=tmp[pos-i-1];
	}
	buffer[pos]='\0';
}

//this function generates k distinct random numbers in the range 0 to k-1.
//in other words it generates a random permutation of numbers 0 to k-1.
void generateDistinctRandomIndices(int k)
{
	int curr=0,tmp,pass;
	while(curr!=k)
	{
		tmp=rand()%k;//generate a random number between 0 to k-1
		pass=1;
		for(int i=0;i<curr;i++)
		{
			if(tmp==randomIndices[i])//already a part of array
			{
				pass=0;//reject this number
				break;
			}
		}
		if(pass)
		{
			randomIndices[curr]=tmp;
			curr++;
		}
	}
}

//this function generates a random number(not used previously) to represent the new user.
void assignIndexToPassword()
{
	int done=0;
	while(done==0)
	{
		int flag=0;
		int tmp=rand()%maxn;//generate a random number in the range 0 to maxn-1
		for(int i=0;i<numIndices;i++)
		{
			if(tmp==indicesPresent[i])//if this number is already assigned to a previous password
			{
				flag=1;//then reject this number and continue the while loop
				break;
			}
		}
		if(flag==0)//if it is a new number , then store it into idx varible and exit the while loop
		{
			idx=tmp;
			done=1;
		}
	}
}

//this function sorts the F1 table in ascending order
//username is considered as a key for sorting(as described in the paper).
void sortF1InAscendingOrder()
{
	FILE *fptr1;
	fptr1=fopen("F1.txt","r");
	char buffer[1000],line[100][1000],username[100][1000];
	int ind[100];
	int pos=0;
	//read and store the contents of F1 table into line variable.
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
	//extract username from the data of F1 table
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
	//I have used bubble sort algorithm to sort the array of indices corresponding to the usernames
	//using array of indices in place of actual data array makes the algorithm faster
	//since swapping two strings take much more time than swapping two integers
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
	//now use the index array to get the sorted table F1
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
	fptr1=fopen("F1.txt","w");
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

//this function sorts the table F2 in ascending order of assigned indices
void sortF2InAscendingOrder()
{
	FILE *fptr1,*fptr2;
	fptr1=fopen("F2_unhashed.txt","r");
	fptr2=fopen("F2.txt","r");
	char buffer[1000],line[100][1000],numbers[100][1000],line1[100][1000];
	int ind[100];
	int pos=0;
	//read the contents of F2_unhashed.txt and store into line variable
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
	int pos1=0;
	//read the contents of F2.txt and store into line1 variable
	while(fgets(buffer,sizeof(buffer),fptr2))
	{
		for(int i=0;i<strlen(buffer);i++)
		{
			line1[pos1][i]=buffer[i];
		}
		line1[pos1][strlen(buffer)]='\0';
		pos1++;
	}
	//extract id from the F2 table and store into array numbers
	for(int i=0;i<pos;i++)
	{
		for(int j=0;j<strlen(line[i]);j++)
		{
			if(line[i][j]==' ')
			{
				numbers[i][j]='\0';
				break;
			}
			numbers[i][j]=line[i][j];
		}
	}
	int check[1000];
	for(int i=0;i<pos;i++)
	{
		check[i]=atoi(numbers[i]);//convert string into integer data type
	}
	//I have used bubble sort algorithm to sort the array of indices corresponding to the assigned ids of each password
	for(int i=0;i<pos;i++)
	{
		for(int j=0;j<i;j++)
		{
			if(check[ind[i]]<check[ind[j]])
			{
				int tmp=ind[i];
				ind[i]=ind[j];
				ind[j]=tmp;
			}
		}
	}
	//form the final sorted data into tmp (for unhashed F2)
	char tmp[100][1000],tmp1[100][1000];
	for(int i=0;i<pos;i++)
	{
		for(int j=0;j<strlen(line[ind[i]]);j++)
		{
			tmp[i][j]=line[ind[i]][j];
		}
		tmp[i][strlen(line[ind[i]])]='\0';
	}
	//form the final sorted data into tmp1 (for hashed F2)
	for(int i=0;i<pos1;i++)
	{
		for(int j=0;j<strlen(line1[ind[i]]);j++)
		{
			tmp1[i][j]=line1[ind[i]][j];
		}
		tmp1[i][strlen(line1[ind[i]])]='\0';
	}
	fclose(fptr1);
	fclose(fptr2);
	//copy tmp into F2_unhashed.txt
	fptr1=fopen("F2_unhashed.txt","w");
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
	//copy tmp1 into F2.txt
	fptr2=fopen("F2.txt","w");
	char dataToWrite1[10000];
	ptr=0;
	for(int i=0;i<pos1;i++)
	{
		for(int j=0;j<strlen(tmp1[i]);j++)
		{
			dataToWrite1[ptr]=tmp1[i][j];
			ptr++;
		}
	}
	dataToWrite1[ptr]='\0';
	fprintf(fptr2, "%s", dataToWrite1);
	fclose(fptr2);
}

int main()
{
	//read and store already used indices from table F2
	FILE *fptr3;
	//hashed or unhashed, any table could be used
	fptr3=fopen("F2_unhashed.txt","r");
	numIndices=0;
	char buff[1000];
	while(fgets(buff,sizeof(buff),fptr3))
	{
		char tmp[20];
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]==' ')
			{
				tmp[i]='\0';
				break;
			}
			tmp[i]=buff[i];
		}
		int indx=atoi(tmp);
		indicesPresent[numIndices]=indx;
		numIndices++;
	}
	//continue to take user input until it exits willingly
	while(1)
	{
		int taskType;
		printf("to create a new user : enter 0\nto try to login as a existing user : enter 1\nto exit : enter -1\n");
		scanf("%d",&taskType);
		if(taskType==0)
		{
			//a new user must be added to database
			srand(time(0));//we need to seed the random function so that it generated new set of numbers on each iteration.
			char psswd[200],username[50];
			int k,x=-1;//number of sweetwords
			printf("\nenter username\n");
			scanf("%s",username);
			printf("\nenter password\n");
			scanf("%s",psswd);
			printf("\nenter the number of sweetwords(k)\n");
			scanf("%d",&k);
			generateDistinctRandomIndices(k);
			char sweetwords[k][20];
			//store k random indices from the already existing set of passwords as honeyword indices
			for(int i=0;i<k;i++)
			{
				char tmp[20];
				integerToCharPointer(indicesPresent[randomIndices[i]],tmp);
				for(int j=0;j<strlen(tmp);j++)
				{
					sweetwords[i][j]=tmp[j];
				}
				sweetwords[i][strlen(tmp)]='\0';
			}
			assignIndexToPassword();
			//idx equals the index of correct password in F2
			//randIdx equals the index of idx in sweetwords
			int randIdx=rand()%k;
			char tmp1[20];
			integerToCharPointer(idx,tmp1);
			for(int j=0;j<strlen(tmp1);j++)
			{
				sweetwords[randIdx][j]=tmp1[j];
			}
			sweetwords[randIdx][strlen(tmp1)]='\0';
			printf("\nsweetword indices for this user:->\n");
			for(int i=0;i<k;i++)
			{
				printf("%s\n",sweetwords[i]);
			}
			printf("\n");
			//store the newly added username and password into tables
			char dataToAppend[1000];
			for(int i=0;i<strlen(username);i++)
			{
				dataToAppend[i]=username[i];
			}
			dataToAppend[strlen(username)]=' ';
			int tmp=strlen(username)+1;
			for(int i=0;i<k;i++)
			{
				for(int j=tmp;j<tmp+strlen(sweetwords[i]);j++)
				{
					dataToAppend[j]=sweetwords[i][j-tmp];
				}
				dataToAppend[tmp+strlen(sweetwords[i])]=' ';
				tmp+=strlen(sweetwords[i])+1;
			}
			dataToAppend[tmp-1]='\n';//last whitespace is overwritten
			dataToAppend[tmp]='\0';//end the string
			FILE *fptr;
			fptr=fopen("F1.txt","a");
			fputs(dataToAppend,fptr);
			fclose(fptr);
			fptr=fopen("F2_unhashed.txt","a");
			char dataToAppend1[1000];
			for(int i=0;i<strlen(tmp1);i++)//tmp1 has the index of correct password
			{
				dataToAppend1[i]=tmp1[i];
			}
			dataToAppend1[strlen(tmp1)]=' ';
			int tmp2=strlen(tmp1)+1;
			for(int i=tmp2;i<tmp2+strlen(psswd);i++)
			{
				dataToAppend1[i]=psswd[i-tmp2];
			}
			dataToAppend1[tmp2+strlen(psswd)]='\n';
			dataToAppend1[tmp2+strlen(psswd)+1]='\0';
			fputs(dataToAppend1,fptr);
			fclose(fptr);
			//store the hashed data
			fptr=fopen("F2.txt","a");
			char dataToAppend2[1000];
			for(int i=0;i<strlen(tmp1);i++)//tmp1 has the index of correct password
			{
				dataToAppend2[i]=tmp1[i];
			}
			dataToAppend2[strlen(tmp1)]=' ';
			tmp2=strlen(tmp1)+1;
			//now psswd should store the hashed password
			FILE *fptr2;
			fptr2=fopen("temp.txt","w");
			fprintf(fptr2, "%s", psswd);
			fclose(fptr2);
			//now run shell command to convert the password into hashed password
			char *command="md5deep temp.txt > hashFile.txt";
			int res=system(command);
			if(res){
				printf("the command to convert password into md5 hashing did not terminate correctly\n");
			}
			//read from hashFile.txt and store <honeyindex,hashed_password> in file F2.txt
			fptr2=fopen("hashFile.txt","r");
			fscanf(fptr2,"%s",psswd);
			fclose(fptr2);
			for(int i=tmp2;i<tmp2+strlen(psswd);i++)
			{
				dataToAppend2[i]=psswd[i-tmp2];
			}
			dataToAppend2[tmp2+strlen(psswd)]='\n';
			dataToAppend2[tmp2+strlen(psswd)+1]='\0';
			fputs(dataToAppend2,fptr);
			fclose(fptr);
			int status=fork();
			if(status==0)
			{
				//child process
				char buffer[20],requestType[20]="add";
				integerToCharPointer(randIdx+1,buffer);
				char *args[]={"./honeywordServer",username,buffer,requestType,NULL};
	       	 	execv(args[0],args);
	       	 	//if control comes back to this process, then this means that the honeywordServer program did not terminate properly
	        	printf("honeywordServer executeable did not terminate properly\n");
			}else
			{
				//parent process
				//status = process id of the created child
				wait(NULL);//wait for the child process to complete
				sortF1InAscendingOrder();
				sortF2InAscendingOrder();
			}
		}else if(taskType==1)
		{
			//check the login credentials
			char psswd[20],username[50];
			printf("\nenter username\n");
			scanf("%s",username);
			//assuming that the attacker has access to the sweetwords database and he has already cracked the hashing to get the original honeywords+sugarword
			FILE *fptr;
			fptr=fopen("F1.txt","r");
			char line[1000],data[50][50];
			//data array stores all the sweetwords of the username
			int found=0,dataPtr=0,curr=0;
			while (fgets(line, sizeof(line), fptr)) {
		        //get the first strlen(username) characters of line and ensure that line[strlen(username)]==' '
				if(line[strlen(username)]!=' ')//if length of stored username and entered username is not same, then it is a mismatch
				{
					continue;
				}
				char check[1000];
				for(int i=0;i<strlen(username);i++)
				{
					check[i]=line[i];
				}
				check[strlen(username)]='\0';
				if(strncmp(username,check,sizeof(username))==0)
				{
					//match found
					found=1;
					//print all the sweetwords
					for(int i=strlen(username)+1;i<strlen(line);i++)
					{
						if(line[i]==' '||line[i]=='\n')
						{
							data[dataPtr][curr]='\0';
							dataPtr++;
							curr=0;
						}else
						{
							data[dataPtr][curr]=line[i];
							curr++;
						}
					}
					fclose(fptr);
					break;
				}
		    }
		    if(found==0)//username not found on database
		    {
		    	printf("this username is not registered in the database, try another name\n");
		    	continue;
		    }
			//find the index of the entered password in the variable line(one based index)
			//report if it is not found
			//data array stores the honeyindices of the username 
			//we need to replace those honey indices with honeywords(using table F2) to proceed further.
			char newData[50][50];
			int newDataPtr=0;
			curr=0;
			for(int ii=0;ii<dataPtr;ii++)
			{
				fptr=fopen("F2_unhashed.txt","r");
				while (fgets(line, sizeof(line), fptr)) {
			        //get the first strlen(data[ii]) characters of line and ensure that line[strlen(data[ii])]==' '
					if(line[strlen(data[ii])]!=' ')//if length of stored index and data[ii](from table F1) is not same, then it is a mismatch
					{
						continue;
					}
					char check[1000];
					for(int i=0;i<strlen(data[ii]);i++)
					{
						check[i]=line[i];
					}
					check[strlen(data[ii])]='\0';
					if(strncmp(data[ii],check,sizeof(data[ii]))==0)
					{
						//match found
						found=1;
						for(int i=strlen(data[ii])+1;i<strlen(line);i++)
						{
							if(line[i]==' '||line[i]=='\n')
							{
								newData[newDataPtr][curr]='\0';
								newDataPtr++;
								curr=0;
							}else
							{
								newData[newDataPtr][curr]=line[i];
								curr++;
							}
						}
						fclose(fptr);
						break;
					}
			    }
			}
			printf("\nthe sweetwords for this username are :->\n");
			for(int i=0;i<newDataPtr;i++)
			{
				printf("%s\n", newData[i]);
			}
			printf("\nenter the guessed password\n");
			scanf("%s",psswd);
			//now search for the index of entered passwd in newData array
			found=0;
			int passwordIndex;
			for(int i=0;i<newDataPtr;i++)
			{
				if(strncmp(newData[i],psswd,strlen(psswd))==0)
				{
					found=1;
					passwordIndex=i+1;
					break;
				}
			}
			//if not match is found then print the message
			if(found==0)
			{
				printf("the password = %s was not among the sweetwords of the user = %s\n",psswd,username);
				continue;
			}
			int status=fork();
			if(status==0)
			{
				//child process
				char buffer[20],requestType[20]="auth";
				integerToCharPointer(passwordIndex,buffer);
				char *args[]={"./honeywordServer",username,buffer,requestType,NULL};
	       	 	execv(args[0],args);
	       	 	//if control comes back to this process, then this means that the honeywordServer program did not terminate properly
	        	printf("honeywordServer executeable did not terminate properly\n");
			}else
			{
				//parent process
				//status = process id of the created child
				wait(NULL);//wait for the child process to complete
				FILE *fptr1;
				char buffer1[1000];
				fptr1=fopen("result.txt","r");
				fgets(buffer1,sizeof(buffer1),fptr1);
				printf("\n%s\n",buffer1);
				fclose(fptr1);
			}
		}else
		{
			return 0;
		}
	}
}