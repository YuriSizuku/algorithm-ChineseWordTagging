#include<map>
#include "TagWord.h"
#include "Viterbi.h"
#ifndef _HMMWORD_H
#define _HMMWORD_H
#pragma comment(linker, "/STACK:5000000000 ")//用该语句设置/STACK:后面数字为容量 (默认是1m)
const int NUM_WORD=55000;//最多词语数
class HMMword:public TagWord
{
	protected:
		double start_p[NUM_TAG];
		double trans_p[NUM_TAG][NUM_TAG];
		double emit_p[NUM_TAG][NUM_WORD];
		std::map<std::string,int> word_map;//词语映射

		void count2prob(int start[],int trans[][NUM_TAG],int emit[][NUM_WORD]);//数量矩阵转换为概率矩阵
	public:
		HMMword(DivideWord *spliter):TagWord(spliter){};
		HMMword(DivideWord *spliter,const char *path_hmm,const char *path_wordmap);
		int train_hmm(const char *path);//训练标注的马尔可夫模型（从语料库）
		int tagword_sentence(std::string &sentence,std::string seperator="/");//标注
		int divide_sentence(std::string &sentence,std::string seperator);//分词
		
		int save_args(const char *path_hmm,const char *path_wordmap);//存储参数
		int load_args(const char *path_hmm,const char *path_wordmap);//读取参数
		int save_hmm(const char *path);//读取hmm模型参数
		int load_hmm(const char *path);//存储hmm模型参数
		int save_wordmap(const char *path);//存储映射表
		int load_wordmap(const char *path);//读取映射表
};
#endif