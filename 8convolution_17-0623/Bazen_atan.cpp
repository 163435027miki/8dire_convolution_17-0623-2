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

#define PI 3.14159265

using namespace std;

//メモリ確保を行うためのヘッダ
#define ANSI				
#include "nrutil.h"	


int Bazen_atan(char date_directory[], int &image_x, int &image_y,int paramerter[],int paramerter_count,int sd,char date[]){
	
//////////////////定義///////////////////////////////////////////////////////////////////////////////////////////
	
	char *Input_Filename_Bazen_atan1_s = "\\V(0).csv";			//入力ファイル名の指定
	char *Input_Filename_Bazen_atan3_s = "\\V(90).csv";			//入力ファイル名の指定
	char inputdate_directory_Bazen_atan[128];					//inputdateのフォルダのディレクトリ

	char date_directory4[128];						//出力フォルダのディレクトリ

	//出力結果のファイル名の指定
	char *math_Bazen_atan1_s = "\\Bazen_arctan.csv";
	char *math_Bazen_atan5_s = "\\threshold_high.csv";
	char *math_Bazen_atan8_s = "\\threshold2.csv";
	char *math_Bazen_atan9_s = "\\threshold_low.csv";

	//入力ファイルのディレクトリ
	char Input_Filename_Bazen_atan1[255];						
	char Input_Filename_Bazen_atan3[255];

	//出力ファイルのディレクトリ
	char math_Bazen_atan1[128];							//内積						
	char math_Bazen_atan5[128];							//threshold_high
	char math_Bazen_atan8[128];							//2つの閾値を一つに
	char math_Bazen_atan9[128];							//threshold(use_Rvector_flagの応答電圧Vの大きさ）
	
	double threshold_high;
	double threshold_low;
	double threshold_low_abs;
	double threshold_high_abs;

	FILE *fp_arctan,*fp_threshold_low,*fp_threshold_high,*fp_threshold2;

	printf("****************************************\n");
	printf("start： Bazen_atan\n");
	printf("****************************************\n");

	int i=0,j=0;
/////////////////////////////////初期設定1 : input設定//////////////////////////////////////////////////////////////////

	//Nrutilを用いたメモリの確保
	double **V0 = matrix(0, image_x - 1, 0, image_y - 1);
	double **V90 = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_atan_high_flag = matrix(0, image_x - 1, 0, image_y - 1);
	double **threshold_atan_low_flag = matrix(0, image_x - 1, 0, image_y - 1);
	double **Angle = matrix(0, image_x - 1, 0, image_y - 1);
	double **AngleF = matrix(0, image_x - 1, 0, image_y - 1);
	double **Gxx = matrix(0, image_x - 1, 0, image_y - 1);
	double **Gyy = matrix(0, image_x - 1, 0, image_y - 1);
	double **Gxy = matrix(0, image_x - 1, 0, image_y - 1);
	double **sGxx = matrix(0, image_x - 1, 0, image_y - 1);
	double **sGyy = matrix(0, image_x - 1, 0, image_y - 1);
	double **sGxy = matrix(0, image_x - 1, 0, image_y - 1);
	
	//初期化
	for (i = 0; i < image_y; i++) {
		for (j = 0; j < image_x; j++) {
			V0[j][i] = 0;
			V90[j][i] = 0;
			threshold_atan_high_flag[j][i] = 0;
			threshold_atan_low_flag[j][i] = 0;
			Angle[j][i] = 0;
			AngleF[j][i] = 0;
			Gxx[j][i] = 0;
			Gyy[j][i] = 0;
			Gxy[j][i] = 0;
			sGxx[j][i] = 0;
			sGyy[j][i] = 0;
			sGxy[j][i] = 0;
		}
	}

////////////////////////////入出力ディレクトリの作成//////////////////////////////////////////////////////////////////////////////

	if(paramerter[0]==1){
		sprintf(inputdate_directory_Bazen_atan, "%s%d×%dsobel_conv_sd%d", date_directory, paramerter[paramerter_count],paramerter[paramerter_count], sd);
			}else{
		sprintf(inputdate_directory_Bazen_atan, "%s%dk_conv_sd%d", date_directory, paramerter[paramerter_count], sd);
	}
	
	//Inputファイルの設定
	sprintf(Input_Filename_Bazen_atan1, "%s%s", inputdate_directory_Bazen_atan, Input_Filename_Bazen_atan1_s);
	sprintf(Input_Filename_Bazen_atan3, "%s%s", inputdate_directory_Bazen_atan, Input_Filename_Bazen_atan3_s);
	printf("Input_Filename_atan1=%s\n", Input_Filename_Bazen_atan1);

	//Outputディレクトリの設定
	if(paramerter[0]==1){
		sprintf(date_directory4, "%s%d×%dsobel_Bazen_atan_sd%d", date_directory, paramerter[paramerter_count],paramerter[paramerter_count], sd);
	}else{
		sprintf(date_directory4, "%s%dk_Bazen_atan_sd%d", date_directory, paramerter[paramerter_count], sd);
	}

	//Outputフォルダのディレクトリ作成
	if (_mkdir(date_directory4) == 0) {
		printf("フォルダ %s を作成しました\n", date_directory4);
	}
	else {
		printf("フォルダ作成に失敗しました。\n");
	}

	//Outputファイルのディレクトリ設定
	sprintf(math_Bazen_atan1, "%s%s", date_directory4, math_Bazen_atan1_s);
	sprintf(math_Bazen_atan5, "%s%s", date_directory4, math_Bazen_atan5_s);
	sprintf(math_Bazen_atan8, "%s%s", date_directory4, math_Bazen_atan8_s);
	sprintf(math_Bazen_atan9, "%s%s", date_directory4, math_Bazen_atan9_s);

	//Inputファイルを開く
	ifstream V_0(Input_Filename_Bazen_atan1);
	ifstream V_90(Input_Filename_Bazen_atan3);

	if(!V_0){printf("\n入力エラー V(0).csvがありません\nInput_Filename_Bazen_atan1=%s",Input_Filename_Bazen_atan1);return 1;}
	if(!V_90){printf("\n入力エラー V(90).csvがありません\nInput_Filename_Bazen_atan3=%s",Input_Filename_Bazen_atan3);return 1;}

	
///////////Inputファイルの読み込み//////////////////////////////////////////////////////////////////////////////////
	int count_small=0,count_large=0;

	i = 0;
	string str_0, str_90;
	count_large = 0;			//初期化
	while (getline(V_0, str_0)) {					//このループ内ですべてを行う
		count_small = 0;		//初期化

		//いろいろ定義．
		string token_V_0;
		istringstream stream_V_0(str_0);

		getline(V_90, str_90);
		string token_V_90;
		istringstream stream_V_90(str_90);

		//配列に代入
		while (getline(stream_V_0, token_V_0, ',')) {	 //一行読み取る．V0のみ，繰り返しの範囲指定に用いる

			double tmp_V_0 = stof(token_V_0);			//文字を数字に変換
			V0[count_small][count_large] = tmp_V_0;		//配列に代入

			getline(stream_V_90, token_V_90, ',');
			double tmp_V_90 = stof(token_V_90);
			V90[count_small][count_large] = tmp_V_90;

			++count_small;
		}
		++count_large;
	}

	int k,l;


/////////////////大小判定/////////////////////////////////////////////////////////////
		for (count_small = 0; count_small < image_x; ++count_small) {
			for (count_large = 0; count_large < image_y; ++count_large) {

				for (count_small = 0; count_small < image_x; ++count_small) {
					for (count_large = 0; count_large < image_y; ++count_large) {
						if (V0[count_small][count_large] >= V90[count_small][count_large]) {
							threshold_atan_high_flag[count_small][count_large] = 1;			//Vの大小比較
							threshold_atan_low_flag[count_small][count_large] = 3;
						}else{
							threshold_atan_high_flag[count_small][count_large] = 3;
							threshold_atan_low_flag[count_small][count_large] = 1;
						}
					}
				}
			}
		}
		
////////////////////////////Bazenの計算//////////////////////////////////////////////////////////////////////////////////////////////	
		//傾きの2乗
		for(j=0;j<image_y;++j){
			for(i=1;i<image_x;++i){
				Gxx[i][j]=pow(V0[i][j],2);
				Gyy[i][j]=pow(V90[i][j],2);
				Gxy[i][j]=V0[i][j]*V90[i][j];

			}
		}

		//16×16の領域での和
		for(j=0;j<image_y;++j){
			for(i=0;i<image_x;++i){
				for(l=j-7;l<j+8+1;++l){
					for(k=i-7;k<i+8+1;++k){
						if(l>=0 && k>=0 && l<image_x && k<image_y){
							sGxx[i][j]=sGxx[i][j]+Gxx[k][l];
							sGyy[i][j]=sGyy[i][j]+Gyy[k][l];
							sGxy[i][j]=sGxy[i][j]+Gxy[k][l];
						}
					}
				}
			}
		}
		
		for(j=0;j<image_y;++j){
			for(i=0;i<image_x;++i){
				if(sGxx[i][j]-sGyy[i][j]>=0)AngleF[i][j]=(atan((2*sGxy[i][j])/(sGxx[i][j]-sGyy[i][j]))/2);
				if(sGxx[i][j]-sGyy[i][j]<0 && sGxy[i][j]>=0)AngleF[i][j]=((atan((2*sGxy[i][j])/(sGxx[i][j]-sGyy[i][j]))/2)+PI);
				if(sGxx[i][j]-sGyy[i][j]<0 && sGxy[i][j]<0)AngleF[i][j]=((atan((2*sGxy[i][j])/(sGxx[i][j]-sGyy[i][j]))/2)-PI);

				if(AngleF[i][j]<=0)Angle[i][j]=AngleF[i][j]+(PI/2);
				if(AngleF[i][j]>0)Angle[i][j]=AngleF[i][j]-(PI/2);

				Angle[i][j]=Angle[i][j]*180/PI;

			}
		}

/////////////////////////計算終わり/////////////////////////////////////////////////////////////////////////////////////////////

//////////////ファイルへの書き込み(一行ずつ書き込み）/////////////////////////////////////////////

/////////////////////////出力ファイルを開く///////////////////////////////////////////////////////////////////////////////////
	//FILE *fp_v0c;
	if((fp_arctan=fopen(math_Bazen_atan1,"w"))==NULL){cout<<"入力エラー Bazen_arctan.csvが開けません";exit(1);}
	if((fp_threshold_high=fopen(math_Bazen_atan5,"w"))==NULL){cout<<"入力エラー threshold_high.csvが開けません";exit(1);}
	if((fp_threshold2=fopen(math_Bazen_atan8,"w"))==NULL){cout<<"入力エラー threshold2.csvが開けません";exit(1);}
	if((fp_threshold_low=fopen(math_Bazen_atan9,"w"))==NULL){cout<<"入力エラー threshold.csvが開けません";exit(1);}
	//if((fp_newfanction=fopen(math_name10,"w"))==NULL){cout<<"入力エラー newfunction.csvが開けません";exit(1);}	//出力増やす用5/7
	
		for(j=0;j<image_y;++j){
			for(i=0;i<image_x;++i){
	
			fprintf(fp_arctan,"%lf,",Angle[i][j]);
			if(i==image_x - 1){fprintf(fp_arctan,"\n");}



			if(threshold_atan_high_flag[i][j]==1){threshold_high=V0[i][j];fprintf(fp_threshold_high,"%lf,",threshold_high);}
			if(threshold_atan_high_flag[i][j]==3){threshold_high=V90[i][j];fprintf(fp_threshold_high,"%lf,",threshold_high);}
			if(i==image_x - 1){fprintf(fp_threshold_high,"\n");}

			if(threshold_atan_low_flag[i][j]==1){threshold_low=V0[i][j];fprintf(fp_threshold_low,"%lf,",threshold_low);}
			if(threshold_atan_low_flag[i][j]==3){threshold_low=V90[i][j];fprintf(fp_threshold_low,"%lf,",threshold_low);}
			if(i==image_x - 1){fprintf(fp_threshold_low,"\n");}

			threshold_high_abs=threshold_high;
			threshold_low_abs=threshold_low;

			if(threshold_high<0){threshold_high_abs=threshold_high_abs*-1;}
			if(threshold_low<0){threshold_low_abs=threshold_low_abs*-1;}
			if(threshold_high_abs>threshold_low_abs){
				fprintf(fp_threshold2,"%lf,",threshold_high);
			}else{
				fprintf(fp_threshold2,"%lf,",threshold_low);
			}
			if(i==image_x - 1){fprintf(fp_threshold2,"\n");}
							//if(V0[j]<0)Angle[j]=Angle[j]-180;


			}
			
		}




///////////////////////////書き込み終わり/////////////////////////////////////////////////////////			
		
	//ファイルを閉じる
	fclose(fp_arctan);
	fclose(fp_threshold_high);
	fclose(fp_threshold2);
	fclose(fp_threshold_low);
	
////////////////////////logファイルの作成//////////////////////////////////////////////////////////////////////////
	FILE *fp_date;
	char filename_log[128];

	sprintf(filename_log, "%s\\log.txt",date_directory4);	//logファイル作成のディレクトリ指定
	if((fp_date=fopen(filename_log,"w"))==NULL){printf("logファイルが開けません");exit(1);}
	fprintf(fp_date,"Time       : %s\n",date);						//時間
	fprintf(fp_date,"使用データ : %s\n",inputdate_directory_Bazen_atan);		//使用した畳み込み済みデータ
	fprintf(fp_date,"保存先     : %s\n",date_directory);			//保存先
	fclose(fp_date);


	printf("使用データ : %s\n",inputdate_directory_Bazen_atan);				//使用した畳み込み済みデータ
	printf("logファイル %s を保存しました\n",filename_log);


	//メモリの開放
	free_matrix(V0, 0, image_x - 1, 0, image_y - 1);
	free_matrix(V90, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_atan_high_flag, 0, image_x - 1, 0, image_y - 1);
	free_matrix(threshold_atan_low_flag, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Angle, 0, image_x - 1, 0, image_y - 1);
	free_matrix(AngleF, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Gxx, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Gyy, 0, image_x - 1, 0, image_y - 1);
	free_matrix(Gxy, 0, image_x - 1, 0, image_y - 1);
	free_matrix(sGxx, 0, image_x - 1, 0, image_y - 1);
	free_matrix(sGyy, 0, image_x - 1, 0, image_y - 1);
	free_matrix(sGxy, 0, image_x - 1, 0, image_y - 1);

	printf("finish : Bazen_atan\n");

	
	return 0;
}
