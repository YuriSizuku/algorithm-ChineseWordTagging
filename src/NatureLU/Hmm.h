#include "DivideWord.h"
#include"Viterbi.h"
#ifndef _HMM_H
#define _HMM_H

//不再头文件中定义const变量，防止重定义(不行，这样就不能数组大小了)
const int NUM_STATE=4;      //隐藏态数量，即B,M,E,S
const int NUM_OBS=10000;    //观察态数量，即汉字数量

class HMM :public DivideWord
{
	/*出现了很奇怪的问题，循，这个字有时候asci值不对，使用unicode字符集可以过去
	*/
	protected:
		double start_p[NUM_STATE];					//初始隐含态概率(BMES)
		double trans_p[NUM_STATE][NUM_STATE];		//状态转移矩阵
		double emit_p[NUM_STATE][NUM_OBS];			//发射矩阵概率
		
		int word2num(const std::string &word);//将asci字转换为数（机内码->区位码）
		int word2num(char c1,char c2);
		int char2state(char c);//通过B,M,E,S 获得状态索引
		void count2prob(int start[],int trans[][NUM_STATE],int emit[][NUM_OBS]);//数量矩阵转换为概率矩阵
	public:
		enum WordState{B,M,E,S};

		HMM();
		HMM(const char *hmm_path);

		int divide_sentence(std::string &sentence,std::string seperator="/");
		int createTrainFromCorpus(const char *inpath,const char *outpath);//从语料库创建hmm训练（train_hmm的训练）(暂时不写次函数)
		int train_hmm(const char *path);
		//没有标点，没有其他半角字符，每个字之间紧邻有 /B,/M,/E,/S 标签的文本，训练hmm模型
		//“人/B们/E  常/S  说/S  生/B活/E  是/S  部/S  教/B科/M书/E”类似于这样的模型去训练
		int save_hmm(const char *path);//读取hmm模型参数
		int load_hmm(const char *path);//存储hmm模型参数
};
#endif