#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define RANKING_FILE "ranking.dat" //保存ファイル名
#define BREAKTIME 3 //試行を何回で止めるか
#define TURN 3	//ターン数
#define RANKING 11 //rankの配列で用いる。この時、結果を最後に入れるため11としている 表示したい「rank数+1」の値を入れてください
#define RANKING_SIZE RANKING-1 //ランキングをいじる時に使用する
#define ANSWER_WIDE 826 //答えの範囲
#define RANDOM rand()%ANSWER_WIDE+1//答え生成 1~826
#define FIRST 0 //１位
#define FALSE 0
#define TRUE 1
#define NAME_LENGTH 9 //名前の長さ

typedef struct ranking_data_t{
	double score;
	char name[NAME_LENGTH];
}ranking_data_t;

void load_ranking(ranking_data_t *rank){
	FILE *file;
	if((file = fopen(RANKING_FILE, "r+")) == NULL) {
		fprintf(stderr, "RANKING_FILE は存在していません\n");
		puts("初期動作開始");
	}else{
		for(int i=0; i < RANKING; i++){
			fscanf(file,"%lf%s",&rank[i].score,rank[i].name);
		}
		puts("RANKING_FILEが読み込まれました。");
	}
	fclose(file);
}

void save_ranking(ranking_data_t *rank){
	FILE *file;
	if((file = fopen(RANKING_FILE, "r+")) == NULL) {
		fprintf(stderr, "RANKING_FILE は存在していません\n");
	}else{
		for(int i=0; i < RANKING; i++){
			fprintf(file,"%lf %s \n",rank[i].score,rank[i].name);
		}
		puts("RANKING_FILEに保存されました。");
	}
	fclose(file);
}

//rankingを出力する
void print_ranking(ranking_data_t *rank){
	int display_rank = 1;
	int tmp=0;
	puts("RANK       SCORE   NICKNAME");
	for (int i = 0; i < RANKING_SIZE; i++, display_rank++){
		printf("%2d・・・%f", display_rank, rank[i].score);	printf("   %s",rank[i].name);
		putchar('\n');
		if(rank[i].score == rank[i+1].score){
			display_rank-=1;
			tmp++;
		}	
		if(rank[i-1].score == rank[i].score){
			display_rank+=tmp;
			tmp = 0; //初期化
		}
	}
}

//rankingを更新する
void update_ranking(ranking_data_t *rank,float result_rank){

	for (int j = result_rank-1; j < RANKING; ++j){
		for (int k = j + 1; k < RANKING; ++k){
			if (rank[j].score >= rank[k].score){
				float tmp_rank = 0;
				tmp_rank = rank[j].score;
				rank[j].score = rank[k].score;
				rank[k].score = tmp_rank;

				char tmp_name[NAME_LENGTH];
				strncpy(tmp_name,rank[j].name,NAME_LENGTH);
				strncpy(rank[j].name,rank[k].name,NAME_LENGTH);
				strncpy(rank[k].name,tmp_name,NAME_LENGTH);
			}
		}
	}
	
}

void check_name(ranking_data_t *rank,int ranking){
	while(1){
		int flag = FALSE;
		char tmp_name[NAME_LENGTH];
		puts("ニックネームを8文字以内で入力してください\n※有効文字はA-Z,a-z,0-9と.です");
		scanf("%s",tmp_name);
		if(strncpy(tmp_name,tmp_name,NAME_LENGTH)){
			tmp_name[sizeof(tmp_name)-1] = '\0';
			for(int i=0; i<sizeof(tmp_name)-1; i++){
				if((tmp_name[i] >= 1 && tmp_name[i] <= 45) || tmp_name[i] == 47 || (tmp_name[i] <= 58 && tmp_name[i] >= 64) || (tmp_name[i] >= 91 && tmp_name[i] <= 96) || tmp_name[i] >= 123){
					flag = TRUE;
				}
			}
			if(flag == FALSE){
				strncpy(rank[RANKING-1].name,tmp_name,NAME_LENGTH);
				break;
			}
		}
	}
}

//結果より順位を決め、出力する
int get_rank(ranking_data_t *rank,float result){
	int h=RANKING;
	rank[RANKING-1].score = result;
	while(1){
		if(h < 0){
			puts("ランク外です・・・");
			return -1;
		}else if(rank[h-1].score<result){
			printf("%d位です！\n",h+1);
			check_name(rank,h);
			return h+1;
		}
		h--;
	}
}


int main(){
	/***********************************************
			ゲームの処理
	***********************************************/
	ranking_data_t rank[] = {
		{1.2 + 0.2, "KING"},
		{1.2 + 0.2*2, "None1"},
		{1.2 + 0.2*3, "None2"},
		{1.2 + 0.2*4, "None3"},
		{1.2 + 0.2*5, "None4"},
		{1.2 + 0.2*6, "None5"},
		{1.2 + 0.2*7, "None6"},
		{1.2 + 0.2*8, "None7"},
		{1.2 + 0.2*9, "None8"},
		{1.2 + 0.2*10, "None9"},
		//余分に１つ
		{1.2 + 0.2*11, "TMP"},
	};

	load_ranking(rank);
	
	while (1){
		print_ranking(rank);

		/***********************************************
				ターン毎の処理
		***********************************************/
		int total_number_of_trials = 0;
		srand((unsigned)time(NULL));// 答え生成用乱数が同一系列とならないように

		for (int turn = 1; turn <= TURN; turn++){
			
			long int answer = RANDOM;//答え生成 1~826
			printf("%ld\n", answer); //答え出力(デバッグ用)
			
			printf("%dターン目\n", turn);
			int trials = 0;

			/***********************************************
					試行毎の処理
			***********************************************/
			while (1){
				//試行回数が決められた値になったら試行終了
				if (trials == BREAKTIME){
					break;
				}
				trials++;//試行回数をふやす

				//playerの予想
				int predict;
				printf("%d回予想>", trials);
				scanf("%d", &predict);

				if (predict == answer){//playerの予想が当たった場合
					printf("正解！おめでとう！%d回目の予想で正解！\n", trials);
					break;
				}else{//playerの予想が外れた場合
					if (predict > answer - 3 && predict < answer + 3){//予想が答えから+-3の範囲の場合
						puts("惜しい！");
					}else if (predict > answer - 10 && predict < answer + 10){//予想が答えから+-10の範囲の場合
						puts("もうちょい！");
					}else{////予想が答えから+-10の範囲ではない場合
						puts("全然ダメ！");
					}
				}
			}
			total_number_of_trials += trials;//n回目の試行回数を合計試行回数に追加
		}

		/***********************************************
				結果出力の処理
		***********************************************/

		float result = total_number_of_trials / (float)TURN;//正解した平均値を決定
		printf("%dターンで%d回の予想をしました！スコア(平均予想回数)は%fです\n", TURN, total_number_of_trials, result);
		if (result < 3){//正解までの平均試行回数が３回以下の場合
			puts("すごい！やったね！");
		}else if (result < 4 && result > 3){//正解までの平均試行回数が３回~4回の場合
			puts("頑張ったね！");
		}else{///正解までの平均試行回数が4回以下の場合
			puts("ざんねん！もう１回やってみよう！");
		}
		
		int result_rank = 0;//playerのrank
		result_rank = get_rank(rank,result);
		if(result_rank != -1) update_ranking(rank,result_rank); //playerがranking外ではない場合rankingをupdateする
		print_ranking(rank);

		/***********************************************
			ゲーム終了または再起動の処理
		***********************************************/

		//1を入力した場合ゲームをもう一度行い、0でゲームを終了する。
		puts("もう一度チャレンジ　Yes(1) or No(0)");
		int user;
		scanf("%d", &user);
		if(user == 0) break;

	}

	save_ranking(rank);
	return 0;
}
