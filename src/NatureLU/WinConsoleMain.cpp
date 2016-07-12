#include <iostream>
#include<time.h>
#include <string>
#include<fstream>
#include "DivideWord.h"
#include "bfmm.h"
#include "hmm.h"
#include "HMMword.h"
#define MAX_PATHLEN 200
using namespace std;
void demo_divideword()
{
	char path_dict[MAX_PATHLEN]="dict.txt";
	char path_in[MAX_PATHLEN]="in.txt";
	char path_answer[MAX_PATHLEN]="answer.txt";
	char path_hmmdata[MAX_PATHLEN]="hmm.data";
	char path_fmmout[MAX_PATHLEN]="out_fmm.txt";
	char path_bmmout[MAX_PATHLEN]="out_bmm.txt";
	char path_hmmout[MAX_PATHLEN]="out_hmm.txt";

	int flag_dict;
	int flag_hmm;
	int flag_eval;
	int flag_process;
	int start,end;
	double time;
	double correct_ratio,recall_radio;

	BMM bmm;
	FMM fmm;
	HMM hmm;
	string sepr="  ";
	
	//载入字典
	printf("loading dict(%s)...\n",path_dict);
	start=clock();
	flag_dict=DivideWord::loadDict(path_dict);
	end=clock();
	time=double(end-start)/1000;
	if(flag_dict!=-1) 
		printf("loading successfully \n %d words in %lf seconds\n",DivideWord::getWordCount(),time);
	else printf("loading %s fail!\n",path_dict);
	
	cout<<endl;
	//载入hmm矩阵数据
	printf("loading hmm matrix(%s)...",path_hmmdata);
	start=clock();
	flag_hmm=hmm.load_hmm(path_hmmdata);
	end=clock();
	time=double(end-start)/1000;
	if(flag_hmm!=-1) 
		printf("loading successfully in %lf seconds\n",time);
	else printf("loading %s fail!\n",path_hmmdata);
	
	cout<<endl;
	//分词
	if(flag_dict!=-1)
	{
		printf("bmm processing...(%s)\n",path_in);
		start=clock();
		flag_process=bmm.divide_file(path_in,path_bmmout,sepr);
		end=clock();
		time=double(end-start)/1000;
		cout<<"bmm finished ("<<path_bmmout<<") in"<<time<<"seconds"<<endl;
		
		cout<<endl;
		printf("fmm processing...(%s)\n",path_in);
		start=clock();
		flag_process=fmm.divide_file(path_in,path_fmmout,sepr);
		end=clock();
		time=double(end-start)/1000;
		cout<<"fmm finished ("<<path_fmmout<<") in"<<time<<"seconds"<<endl;
	}
	if(flag_hmm!=-1)
	{
		cout<<endl;
		printf("hmm processing...(%s)\n",path_in);
		start=clock();
		flag_process=hmm.divide_file(path_in,path_hmmout,sepr);
		end=clock();
		time=double(end-start)/1000;
		cout<<"hmm finished ("<<path_hmmout<<") in"<<time<<"seconds"<<endl;
	}
	
	cout<<endl;
	//评价
	flag_eval^=flag_eval;
	start=clock();
	flag_eval=bmm.eval_file(path_bmmout,path_answer,correct_ratio,recall_radio,sepr);
	end=clock();
	time=double(end-start)/1000;
	if(flag_eval==-1)
	{
		cout<<endl<<"evaluate error!"<<endl;
	}
	else
	{
		cout<<"bmm : correct ratio="<<correct_ratio<<" recall radio="<<recall_radio<<endl;
	}
	flag_eval^=flag_eval;
	start=clock();
	flag_eval=fmm.eval_file(path_fmmout,path_answer,correct_ratio,recall_radio,sepr);
	end=clock();
	time=double(end-start)/1000;
	if(flag_eval==-1)
	{
		cout<<endl<<"evaluate error!"<<endl;
	}
	else
	{
		cout<<"fmm : correct ratio="<<correct_ratio<<" recall radio="<<recall_radio<<endl;
	}
	flag_eval^=flag_eval;
	start=clock();
	flag_eval=hmm.eval_file(path_hmmout,path_answer,correct_ratio,recall_radio,sepr);
	end=clock();
	time=double(end-start)/1000;
	if(flag_eval==-1)
	{
		cout<<endl<<"evaluate error!"<<endl;
	}
	else
	{
		cout<<"hmm : correct ratio="<<correct_ratio<<" recall radio="<<recall_radio<<endl;
	}
	system("pause");
}
void demo_tagword()
{
	char path_dict[MAX_PATHLEN]="dict.txt";
	char path_hwdict[MAX_PATHLEN]="hwdict.txt";
	char path_hwdata[MAX_PATHLEN]="hmmword.data";
	char path_in[MAX_PATHLEN]="in.txt";
	char path_hwout[MAX_PATHLEN]="out_tagword.txt";

	long start,end;
	double time;
	int res;

	cout<<"loading standard dictionary ("<<path_dict<<")..."<<endl;
	res=DivideWord::loadDict(path_dict);//载入字典
	if(res<0)	cout<<"error:loading fail!"<<endl;
	else cout<<"loading successfully!"<<endl;
	
	BMM bmm;
	//HMM hmm;
	//hmm.load_hmm("hmm.data");
	//此项标注训练训练
	//HMMword hmmtag(&hmm);
	//hmmtag.train_hmm("G:\\dict\\corpus.txt");
	//hmmtag.save_args("G:\\dict\\hmmword.data","G:\\dict\\dict.txt");
	
	HMMword hmmtag(&bmm);//加载已经训练好的矩阵和字典,并用bmm分词方法
	cout<<"loading tagging dictionary ("<<path_hwdict<<") and hmm data ("<<path_hwdata<<")"<<endl;
	res=hmmtag.load_args(path_hwdata,path_hwdict);
	if(res<0)	cout<<"error:loading fail!"<<endl;
	else cout<<"loading successfully!"<<endl;
	
	cout<<"HMMtagging word on file ("<<path_in<<") start..."<<endl;
	start=clock();
	hmmtag.tagword_file(path_in,path_hwout);//处理分词文件
	end=clock();
	time=double(end-start)/1000;
	cout<<"HMMtagging finished ("<<path_hwout<<") in "<<time<<" seconds"<<endl;
	system("pause");
}
void main()
{
	//demo_divideword();//分词示例
	demo_tagword();//词性标注实例
}