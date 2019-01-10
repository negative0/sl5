#include<stdio.h>
#include<string.h>
#include<stdlib.h>

static int lc=0;

void search(char s1[]);
void searchreg(char s2[]);

void main()
{
	FILE *f1,*f2,*f3,*f4;
	int sa,l,o,len,num,symbolcnt=1,litcnt=1;
	char m1[20],la[20],op[200],otp[20],op1[20],op2[20];
	
	f1=fopen("input.txt","r");
	f2=fopen("symtab.txt","w");
	f3=fopen("littab.txt","w");
	fscanf(f1,"%s %s %s %s",la,m1,op1,op2);
	if(strcmp(m1,"START")==0)
	{
		num = atoi(op1);
		lc = num;
 		printf("\t(AD,01)\t(C,%s)\n",op1);
 	}
 	//fscanf(f1,"%s %s %s %s",la,m1,op1,op2);
 	//while(strcmp(m1,"END")!=0)
	while(!feof(f1))
	{	
 		fscanf(f1,"%s %s %s %s",la,m1,op1,op2);
 		if(strcmp(la,"-")!=0)
		{
			fprintf(f2,"%d\t%s\t-\t%d\n",symbolcnt++,la,lc);
 		}
 		if(strncmp(op2,"=",1)==0)
 		{
 			fprintf(f3,"%d\t%s\t%d\n",litcnt++,op2,lc);
 		}
 		printf("%d",lc);
		search(m1);
 		searchreg(op1);
		if(strcmp(m1,"END")==0)
		exit(1);
		printf("\n");
	}
}

void searchreg(char str[100])
{
	if(strcmp(str,"AREG")==0)
		printf("\t(R,01)");
	else if(strcmp(str,"BREG")==0)
		printf("\t(R,02)");
	else if(strcmp(str,"CREG")==0)
		printf("\t(R,03)");
	else if(strcmp(str,"DREG")==0)
		printf("\t(R,04)");
}

void search(char str[100])
{
	FILE *fp;
	int num1;
	char s[100];
	
	fp = fopen("mnemonic.txt","r");
	if(fp == NULL)
    	{
        	printf("Error opening file\n");
        	exit(1);
    	}   
        while(fgets(s,100,fp)!=NULL)
    	{	
    		char s1[100]="",s2[100]="",s3[100]="",s4[100]="";
		sscanf(s,"%s %s %s %s",s1,s2,s3,s4);
		
    		if(strcmp(str,s1)==0)
    		{
    			printf("\t(%s,%s)",s3,s2);
    			num1 = atoi(s4);
    			lc = lc+num1;
    		}  		
    	}	
}


 /*}	
 	
 char s1[100]="",s2[100]="",s3[100]="",s4[100]="";
	sscanf(op,"%s %s %s %s",s1,s2,s3,s4);
	
 	printf(" %s %s %s %s",s1,s4,s3,s2);
*/
 	
	
	
	//char s5[10] = '-';
	/*if(strcmp(s1,"-")!=0)
	{
	f3=fopen("symtab.txt","w");
	fprintf(f3,"%d,%s,-,%s",i++,s1,la);
	printf("%s",s2);
	search(s2);
 	}*/
 	
 	

 
 
 
 
