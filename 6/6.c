// #include<ctype.h>
// #include<string.h>
// #include<stdio.h>
// #include<omp.h>
// #include<stdlib.h>
// #define COUNT 10

// char *words[COUNT] = {"The","around","graphics","from","by","be","any","which","various","mount"};
// long int counts[COUNT];

// int isequal(char *word, char *key){
//     int len_a = strlen(word);
//     int len_b = strlen(key);
//     int i;
//     char b[40];
// 	strcpy(b,key);
//     if(len_a!=len_b){
//         return 0;
//     }
//     for(i=0;i<len_a;i++){
//         // if(word[i]>90){
//         //     word[i]-=32;
//         // }
//         // if(key[i]>90){
//         //     key[i]-=32;
//         // }
//         word[i] = tolower(word[i]);
//         b[i] = tolower(b[i]);
//     }
//     return (strcmp(word, b)==0);
// }

// void read_word(char *temp, FILE* fp){
    
//     int i=0;
//     char ch;
    
//     while((ch=fgetc(fp))!=EOF && isalpha(ch)==0){
//         ;
//     }
//     while(ch!=EOF && isalpha(ch)!=0){
//         temp[i++] = ch;
//         ch = fgetc(fp);
//     }
//     temp[i] = '\0';
//     // printf("%s \n",temp);
// }
// long int determine_count(char *fn, char *key){
//     int cnt =0;
//     char temp[40];
//     FILE *fp = fopen(fn,"r");
//     while(feof(fp)==0){
//         read_word(temp,fp);
        
//         if(isequal(temp,key)){
            
//             cnt++;
//         }
//     }
//     fclose(fp);
//     return cnt;
// }
// int main(int argc, char** argv){
//     int th=1,i;
//     char *fn = "words.txt";
//     for(th=1;th<=8;th*=2){
//         double t = omp_get_wtime();
//         omp_set_num_threads(th);
//         #pragma omp parallel for private(i)
//         for(i=0;i<COUNT;i++){
//             counts[i] = determine_count(fn, words[i]);

//         }
//         printf("Time taken for %d threads: %lf\n", th, omp_get_wtime()-t);
        
//     }
//     printf("Counts are as follows:\n");
//     for(i =0;i<COUNT;i++){
//         printf("%s: %ld\n", words[i], counts[i]);
//     }



//     return 0;
// }
#include<stdio.h>
#include<omp.h>
#include<string.h>
#define COUNT 10
#define FILE_NAME "words.txt"
char search_words[20][COUNT] = {"The","around","graphics","from","by","be","any","which","various","mount"};
long int counts[COUNT];
int line_c = 0;
int is_alpha(char c)
{
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}
int is_equal(char* a,const char* key, int ignore_case){
	char b[20];
	strcpy(b,key);
	int len_a = strlen(a),len_b = strlen(b);
	if(len_a != len_b)
	{
		return 0;
	}
	if(ignore_case != 0)
	{
		int i;
		for(i=0;i<len_a;i++)
		{
			if(a[i] > 90)
			{
				a[i] -= 32;
			}
		}
		for(i=0;i<len_b;i++)
		{
			if(b[i] > 90)
			{
				b[i] -= 32;
			}
		}
	}
	return (strcmp(a,b) == 0);
}

void read_word(char *temp, FILE *fp)
{
	int i=0;
	char ch;
	while((ch = fgetc(fp)) != EOF&&is_alpha(ch) == 0)
	{
	;
	}
	while(ch != EOF	&& is_alpha(ch) != 0)
	{
		temp[i++] = ch;
		ch = fgetc(fp);
	}
	temp[i] = '\0';
}
long determine_count(const char *file_name, const char *key, int ignore_case)
{
	int key_index=0,key_len = strlen(key);
	long word_count=0;
	char ch;
	FILE *fp = fopen(file_name,"r");
	char temp[40];
	int i=0;
	while(feof(fp) == 0)
	{
		read_word(temp,fp);
		if(is_equal(temp,key,ignore_case) != 0)
		{
			word_count++;
		}
		//printf("%s ",temp);
	}
	//printf("\nWord %s: %ld",key,word_count);
	return word_count;
}
void main()
{
	int th=1;
	for(th=1; th<16; th=th*2){
	int i;
	for(i=0;i<COUNT;i++)
	{
		counts[i] = 0;
	}
	double t = omp_get_wtime();
	omp_set_num_threads(th);
	#pragma omp parallel for private(i) 
	for(i=0;i<COUNT;i++)
	{
		counts[i] = determine_count(FILE_NAME,search_words[i],1);
	}
	t = omp_get_wtime() - t;
	
	for(i=0;i<COUNT;i++)
	{
		printf("\n%s: %ld",search_words[i],counts[i]);
	}
		printf("\nTime Taken for %d threads: %lf\n",th,t);
 }
}


