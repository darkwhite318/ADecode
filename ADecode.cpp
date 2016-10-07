#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <memory.h>
#include <cmath>

using namespace std;
# define SSIZE 37
double Bigram[SSIZE][SSIZE];
double Encode[SSIZE][SSIZE];

void file2Table(char*,double(&table)[SSIZE][SSIZE]);
int w2n(int);
char n2w(int);
void updPath(char,double(&score)[SSIZE],int(&path)[SSIZE]);

int main(int argc, char* argv[]){

    //set up 2 lookup table bigram and encode
    file2Table(argv[1],Bigram);
    file2Table(argv[2],Encode);

    //read in test.txt

    ifstream textFile;
    textFile.open(argv[3]);
    stringstream buffer;
    buffer<<textFile.rdbuf();    
    string text = buffer.str();
    int textL = text.length();

    string tempText="";
    for (int i=0;i<textL;i++){
       if(text[i]>=20)
          tempText = tempText+text[i];
    }
    text = tempText;
    textL = text.length();
   
    //calculate the optimal path
    double score[SSIZE];
    int** optPath;
    optPath = new int*[textL];
    for(int i=0;i<textL;i++){
       optPath[i] = new int[SSIZE];
    }

    for(int i=0;i<SSIZE;i++){
       score[i] = 0;
       optPath[textL-1][i] = 0;
    }

   for(int i = textL-2;i>=0;i--){
      int temp[SSIZE];
      updPath(text[i],score,temp);
      for(int j = 0;j<SSIZE;j++){
          optPath[i][j] = temp[j];
      }
   }

   double bestScore = score[0];
   int bestPath = 0;
   for(int i=0;i<SSIZE;i++){
      if(score[i]>bestScore){
	bestScore = score[i];
	bestPath = i;	
      }
   }
   ofstream outFile;
   outFile.open(argv[4]);

   outFile<<n2w(bestPath);
   int t=0;
   
   while(t<text.length()-1){
      bestPath = optPath[t][bestPath]; 
      outFile<<n2w(bestPath);
      t++;
   }

   outFile.close();

    
return 0;
}


//==covert .txt to look-up tabl==//
void file2Table(char* fileName, double (&table)[SSIZE][SSIZE]){
   
    memset(table,0,sizeof(table));
    char buffer[100];
    int id1,id2;
    fstream file;
    file.open(fileName,ios::in);
 
    if(!file)
         cerr<<"error!"<<endl;
    else{
        while(file.getline(buffer,sizeof(buffer),'\n')){

                char freq[30];
		strncpy(freq,buffer+4,30);
		id1 = w2n(buffer[0]-'0');
		id2 = w2n(buffer[2]-'0');

		table[id1][id2] = atof(freq);
		//cout<<id1<<" "<<id2<<" "<<atof(freq)<<endl;
        }
    }
           
}

//covert alphabet,number or SPACE to table index
int w2n(int idx){
	if(idx <0) // SPACE, ASCII = 20
	    return SSIZE-1;
	else if(idx>10) // a-z  ASCII = 97-122
	    return idx-int('a'-'0')+10;
	else // 0-9 ASCII = 48-57
	    return idx; 
			   
}

char n2w(int idx){
   if (idx == SSIZE-1)
	return ' ';
   else if (idx<10)
	return static_cast<char>(idx+int('0'));
   else
        return static_cast<char>(idx+int('a')-10);
}

//update optpath and score
void updPath(char ch, double (&score)[SSIZE] ,int (&path)[SSIZE]){
   int idx = w2n(ch-'0');
   double newScore[SSIZE];
   double nowScore;
   int optLastCh;
   //int path[SSIZE];
 
   for(int i=0 ; i<SSIZE ; i++){
      double tempScore = -1000000000;
      optLastCh = 0;
      for(int j=0 ; j<SSIZE ; j++){
         if(Encode[i][idx]==0 || Bigram[i][j]==0)
	     continue;
	 nowScore = log(Encode[i][idx])+log(Bigram[i][j])+score[j];
	 if(nowScore >tempScore){
	      tempScore = nowScore;
	      optLastCh = j;
	  }
       }
       newScore[i] = tempScore;
       path[i] = optLastCh;
   }
   for(int i =0;i<SSIZE;i++){
      score[i] = newScore[i];
   }
   //return path;
}
