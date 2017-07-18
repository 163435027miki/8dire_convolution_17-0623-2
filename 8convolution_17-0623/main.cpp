#define ANSI
#include "nrutil.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //文字ストリーム
#include <direct.h>//フォルダを作成する

using namespace std;

int image_x, image_y;		//画像サイズ
char date[128] = "";
//出力ファイルディレクトリ
char date_directory[128];
char image_nameP[128];
char image_nameP2[128];
int sd;


int timeset(char date[]);

int paramerter_count_max=3,sd_max=50;

//void convolution(int argc, char** argv,char image_nameP2[],int &image_x,int &image_y,int paramerter[],int paramerter_count,int sd,char date[],char date_directory[]);
int convolution(int argc, char** argv,char image_nameP2[],int &image_x,int &image_y,int paramerter[],int paramerter_count,int sd,char date[],char date_directory[]);
int cossim(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]);
int arctan(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]);
int cossim_result_row(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count_max,int sd_max);
int Bazen_kernel(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]);
int Bazen(char image_nameP2[],int &image_x,int &image_y,int paramerter[],int paramerter_count,int sd,char date[],char date_directory[]);




int main(int argc, char** argv){

	timeset(date);

	int paramerter[4]={0,3,10,100};		//paramerter[0]=1でsobelフィルタ
	int paramerter_count=0;
	for(int paramerter_count=1;paramerter_count<=3;++paramerter_count){
		for(sd=0;sd<=50;sd=sd+10){
		
			if(paramerter[0]==1){
				sprintf(image_nameP,"..\\property_usa\\simulation17-0712\\property_%d×%dsobel_conv_",paramerter[paramerter_count],paramerter[paramerter_count]);
			}else{
				sprintf(image_nameP,"..\\property_usa\\simulation17-0627-2\\property_%dk_conv_",paramerter[paramerter_count]);
				//sprintf(image_nameP,"..\\property_usa\\simulation17-0705_noise_sobel\\property_3×3sobel_conv_");
			}
				sprintf(image_nameP2,"%ssd%d.txt",image_nameP,sd);
			//sprintf(image_nameP2,"%sproperty_%d×%dsobel_conv_IT%d.txt",image_nameP,paramerter[paramerter_count],paramerter[paramerter_count],sd);

			convolution(argc, argv,image_nameP2,image_x,image_y,paramerter,paramerter_count,sd,date,date_directory);

			
			cossim(date_directory,image_x,image_y,paramerter,paramerter_count,sd,date);
			arctan(date_directory,image_x,image_y,paramerter,paramerter_count,sd,date);
			Bazen_kernel(date_directory,image_x,image_y,paramerter,paramerter_count,sd,date);
			Bazen(image_nameP2,image_x,image_y,paramerter,paramerter_count,sd,date,date_directory);

		}
	}

	cossim_result_row(date_directory, image_x ,image_y,paramerter,paramerter_count_max,sd_max);

	printf("全ての処理が終了しました\n");
	
	return 0;


}


