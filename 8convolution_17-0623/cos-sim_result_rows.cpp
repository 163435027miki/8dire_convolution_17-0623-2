#include<fstream>
#include<iostream>
#include<string>
#include<sstream> //文字ストリーム
#include<math.h>
#include<time.h>//時間を用いる
#include <direct.h>//フォルダを作成す
#include<stdio.h>
#include <windows.h>
#include <vector>

//メモリ確保を行うためのヘッダ
#define ANSI				
#include "nrutil.h"	

#define PI 3.14159265

using namespace std;
//char date_directory[128];
//FILE *fp_Angle_C,*fp_threshold2_C,*fp_Angle_A,*fp_threshold2_A;	//入力ファイル
FILE *fp_Angle_Summary,*fp_threshold2_Summary,*fp_Angle_Summary_A,*fp_threshold2_Summary_A;	//出力ファイル
//if((fp_arctan=fopen(math_name1,"w"))==NULL){cout<<"入力エラー arctan.csvが開けません";exit(1);//
int timeset(char date_directory[]);

int cossim_result_row(char date_directory[], int image_x, int image_y){
	printf("cossim_result_row\n");
	printf("image_x=%d,image_y=%d\n",image_x,image_y);
	//sprintf(date_directory,"..\\result_usa\\2017-0627-112941\\");
	int rows;	//行	
	int cols;	//列
	int read_rows_stard=120;	//1からスタート
	int read_rows_goal=125;
	int sd;
	int i,j,k,l;
	int count_rows=0,count_cols=0;
	string str_Angle_C,str_threshold2_C,str_Angle_A,str_threshold2_A;

	int filename_head[3]={3,10,100};
	char filename_headchar[3]="k";
	char simulation_name[128]="simulation17-0622";
	char inputfile_directry1[128];
	char inputfile_directry2[128];
	char inputfile_directry3[128];
	char inputfile_directry4[128];
	char outputfile_directry1[128];
	char outputfile_directry2[128];
	char outputfile_directry3[128];
	char outputfile_directry4[128];

	int lengh_inputfile_directry1;
	int lengh_inputfile_directry2;
	int lengh_inputfile_directry3;
	int lengh_inputfile_directry4;

	rows=image_x;
	cols=image_y;
	//timeset(date_directory);

	
	double **Angle_C = matrix(0, rows+1, 0, cols+1);
	double **threshold2_C = matrix(0, rows+1, 0, cols+1);
	double **Angle_A = matrix(0, rows+1, 0, cols+1);
	double **threshold2_A = matrix(0, rows+1, 0, cols+1);

	for (i = 0; i < rows+1; i++) {
				for (j = 0; j < cols+1; j++) {
					Angle_C[j][i]=0;
					threshold2_C[j][i]=0;
					Angle_A[j][i]=0;
					threshold2_A[j][i]=0;
				}
	}
	
	/*
	//配列の動的確保
	std::vector<std::vector<double>>Angle;
	std::vector<std::vector<double>>threshold2;
	std::vector<std::vector<double>>Angle_A;
	std::vector<std::vector<double>>threshold2_A;
	Angle.resize(rows+1);
	threshold2.resize(rows+1);
	Angle_A.resize(rows+1);
	threshold2_A.resize(rows+1);
	for(i=0;i<=rows;++i){
		Angle[i].resize(cols+1);
		threshold2[i].resize(cols+1);
		Angle_A[i].resize(cols+1);
		threshold2_A[i].resize(cols+1);
	}
	*/
	sprintf(outputfile_directry1,"%sAngle_Summary_C.csv",date_directory);
	sprintf(outputfile_directry2,"%sthresholds_Summary_C.csv",date_directory);
	sprintf(outputfile_directry3,"%sAngle_Summary_A.csv",date_directory);
	sprintf(outputfile_directry4,"%sthresholds_Summary_A.csv",date_directory);

	if((fp_Angle_Summary=fopen(outputfile_directry1,"w"))==NULL){cout<<"入力エラー Angle_Summary_C.csvが開けません";exit(1);}
	if((fp_threshold2_Summary=fopen(outputfile_directry2,"w"))==NULL){cout<<"入力エラー threshold2_Summary_C.csvが開けません";exit(1);}
	if((fp_Angle_Summary_A=fopen(outputfile_directry3,"w"))==NULL){cout<<"入力エラー Angle_Summary_C.csvが開けません";exit(1);}
	if((fp_threshold2_Summary_A=fopen(outputfile_directry4,"w"))==NULL){cout<<"入力エラー threshold2_Summary_C.csvが開けません";exit(1);}
	

	for(j=0;j<3;++j){
		for(i=0;i<6;++i){

			count_cols=0;
			sd=i*10;
			sprintf(inputfile_directry1,"%s%d%s_cossim_sd%d\\Angle.csv",date_directory,filename_head[j],filename_headchar,sd);
			sprintf(inputfile_directry2,"%s%d%s_cossim_sd%d\\threshold2.csv",date_directory,filename_head[j],filename_headchar,sd);
			sprintf(inputfile_directry3,"%s%d%s_atan_sd%d\\arctan.csv",date_directory,filename_head[j],filename_headchar,sd);
			sprintf(inputfile_directry4,"%s%d%s_atan_sd%d\\threshold2.csv",date_directory,filename_head[j],filename_headchar,sd);

			//sprintf(inputfile_directry1,"..\\input_data\\%s\\atan\\%d%s_atan_sd%d\\arctan.csv",simulation_name,filename_head[j],filename_headchar,sd);
			//sprintf(inputfile_directry2,"..\\input_data\\%s\\atan\\%d%s_atan_sd%d\\threshold2.csv",simulation_name,filename_head[j],filename_headchar,sd);
			//sprintf(inputfile_directry1,"..\\input_data\\%s\\atan_Bazel\\%d%s_atanB_sd%d\\arctan.csv",simulation_name,filename_head[j],filename_headchar,sd);
			//sprintf(inputfile_directry2,"..\\input_data\\%s\\atan_Bazel\\%d%s_atanB_sd%d\\threshold2.csv",simulation_name,filename_head[j],filename_headchar,sd);

			lengh_inputfile_directry1=strlen(inputfile_directry1);
			lengh_inputfile_directry2=strlen(inputfile_directry2);
			lengh_inputfile_directry3=strlen(inputfile_directry3);
			lengh_inputfile_directry4=strlen(inputfile_directry4);
			

			//入力ファイルを開く
			ifstream Angle_stream_C(inputfile_directry1);
			ifstream threshold2_stream_C(inputfile_directry2);
			ifstream Angle_stream_A(inputfile_directry3);
			ifstream threshold2_stream_A(inputfile_directry4);
			if(!Angle_stream_C){cout<<"入力エラー Angle.csvがありません";return 1;}
			if(!threshold2_stream_C){cout<<"入力エラー threshold2.csvがありません";return 1;}
			if(!Angle_stream_A){cout<<"入力エラー arctan.csvがありません";return 1;}
			if(!threshold2_stream_A){cout<<"入力エラー threshold2.csvがありません";return 1;}

			//Angleとthresholdを全て読み込む
			while(getline(Angle_stream_C,str_Angle_C)){

				count_rows=0;	//初期化

///////////////////定義//////////////////////////////////////////////////////////////
				string token_Angle_C;
				istringstream stream_Angle_C(str_Angle_C);
				
				getline(threshold2_stream_C,str_threshold2_C);	
				string token_threshold2_C;
				istringstream stream_threshold2_C(str_threshold2_C);

				getline(Angle_stream_A,str_Angle_A);	
				string token_Angle_A;
				istringstream stream_Angle_A(str_Angle_A);

				getline(threshold2_stream_A,str_threshold2_A);	
				string token_threshold2_A;
				istringstream stream_threshold2_A(str_threshold2_A);

			
				while(getline(stream_Angle_C,token_Angle_C,',')){
					
					double tmp_Angle_C=stof(token_Angle_C);//文字を数字に変換
					Angle_C[count_rows][count_cols]=tmp_Angle_C;
				
					getline(stream_Angle_A,token_Angle_A,',');
					double tmp_Angle_A=stof(token_Angle_A);//文字を数字に変換	個々が悪い
					Angle_A[count_rows][count_cols]=tmp_Angle_A;
					

					

					getline(stream_threshold2_A,token_threshold2_A,',');	
					double tmp_threshold2_A=stof(token_threshold2_A);//文字を数字に変換
					threshold2_A[count_rows][count_cols]=tmp_threshold2_A;
					
					
				

					getline(stream_threshold2_C,token_threshold2_C,',');	
					double tmp_threshold2_C=stof(token_threshold2_C);//文字を数字に変換
					threshold2_C[count_rows][count_cols]=tmp_threshold2_C;

			//if(count_rows==255)printf("ここ3");
					
					++count_rows;
					//printf("ここ%d,",count_rows);
					//
////////////////////break条件を記す//////////////////////////////////////////////////////////////////////////////////////////////////////////
					if(count_rows==rows){
						
						count_cols++;
						break;
					}
				}
			
			}
			
			//printf("ここ");
			
			
			for(k=0;k<cols;++k){
				
				if(k>=read_rows_stard-1 && k<read_rows_goal){
					
					fprintf(fp_Angle_Summary,"%d%s_cossim_sd%d_%d,",filename_head[j],filename_headchar,sd,k+1);
					fprintf(fp_threshold2_Summary,"%d%s_cossim_sd%d_%d,",filename_head[j],filename_headchar,sd,k+1);
					fprintf(fp_Angle_Summary_A,"%d%s_atan_sd%d_%d,",filename_head[j],filename_headchar,sd,k+1);
					fprintf(fp_threshold2_Summary_A,"%d%s_atan_sd%d_%d,",filename_head[j],filename_headchar,sd,k+1);
					//fprintf(fp_Angle_Summary,"%d%s_atanB_sd%d_%d,",filename_head[j],filename_headchar,sd,k+1);
					//fprintf(fp_threshold2_Summary,"%d%s_atanB_sd%d_%d,",filename_head[j],filename_headchar,sd,k+1);
					
					for(l=0;l<rows;++l){
						fprintf(fp_Angle_Summary,"%f,",Angle_C[l][k]);
						fprintf(fp_threshold2_Summary,"%f,",threshold2_C[l][k]);
						fprintf(fp_Angle_Summary_A,"%f,",Angle_A[l][k]);
						fprintf(fp_threshold2_Summary_A,"%f,",threshold2_A[l][k]);
						//printf("ここ");
						if(l==rows-1){
							fprintf(fp_Angle_Summary,"\n");
							fprintf(fp_threshold2_Summary,"\n");
							fprintf(fp_Angle_Summary_A,"\n");
							fprintf(fp_threshold2_Summary_A,"\n");
						}
					}
				}


			}

			//fclose(fp_Angle);
			//fclose(fp_threshold2);
			//memset関数で初期化
			memset( inputfile_directry1 , '\0' , lengh_inputfile_directry1 );
			memset( inputfile_directry2 , '\0' , lengh_inputfile_directry2 );
			memset( inputfile_directry3 , '\0' , lengh_inputfile_directry3 );
			memset( inputfile_directry4 , '\0' , lengh_inputfile_directry4 );
		}
	}
	
	fclose(fp_Angle_Summary);
	fclose(fp_threshold2_Summary);
	fclose(fp_Angle_Summary_A);
	fclose(fp_threshold2_Summary_A);
	
	free_matrix(Angle_C, 0, rows+1, 0, cols+1);
	free_matrix(threshold2_C, 0, rows+1, 0,cols+1);
	free_matrix(Angle_A, 0, rows+1, 0, cols+1);
	free_matrix(threshold2_A, 0, rows+1, 0, cols+1);
	
	return 0;
}