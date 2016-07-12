#include<set>

#ifndef _DIVIDEWORD_H
#define _DIVIDEWORD_H
#define MAX_SIZE 10000000
#define MAX_LEN 100
/*
	此类为汉语分词的基类
	此类与所有的派生类编码格式asci（中文gb2312）
*/
struct DictNode
{
	char *word;
	int freq;
};
struct Dict
{
	struct DictNode dict_array[MAX_SIZE];
	int size;
};
struct DictIndex
{
	struct
	{
		int start;
		int end;
	}len_shift[MAX_LEN];
	int len_max;
};
class DivideWord
{
	protected:
		static inline int _comp_1(const void *a,const void *b)
		{
			DictNode *p1=(DictNode*)a;
			DictNode *p2=(DictNode*)b;
			int len1=strlen(p1->word);
			int len2=strlen(p2->word);
			if(len1!=len2)
				return len2-len1;
			//else if(p2->freq !=p1->freq)
			//	return p2->freq - p1->freq;
			else return strcmp(p2->word,p1->word);
		}
		static int addWord(const std::string &word);
		static struct Dict dict_chs;
		static struct DictIndex dict_index;
	
		static std::set<std::string> dict_set;
	public:
		static bool useDictSet;//定义是否用字典集合结构

		//字典操作函数
		static void iniDict();
		static int getWordCount();
		static int saveDict(const char *path);
		static int loadDict(const char *path,bool useDictSet=true);
		static void createDictIndex(bool isSorted=true);
		static int createDictFromCorpus(const char *path,bool useDictSet=true);//从语料库创建词典
		static void sortDict();
		static int destroyDict();

		//字典集合操作函数
		static void createDictSet(bool useDictSet=true);
		static void distoryDictSet();
		
		//文章与搜索相关函数
		int load_passage(const char *path,std::string &passage);
		int save_passage(const char *path,std::string &passage);
		int searchWord(const std::string &word);//搜索词语，返回词典中的索引值
		
		//经测试虚函数对性能影响不大
		//转换函数
		virtual int word2num(const std::string &word);//将asci字转换为数（机内码->区位码）
		virtual int word2num(char c1,char c2);
		//分词函数
		virtual int divide_sentence(std::string &sentence,std::string seperator)=0;//分没有标点句子
		virtual int divide_passage(const std::string &str_in,std::string &str_out,std::string seperator="/");//分一个字符串（可有标点）
		virtual int divide_file(const char *inpath,const char *outpath,std::string seperator="/");
		//评价函数
		virtual void eval_line(std::string &test,std::string &answer,
								int &num_test,int &num_answer,int &num_correct,std::string seperator);
		virtual void eval_line(std::string &test,std::string &answer,
								double &correct_ratio,double &recall_ratio,std::string seperator);
		virtual int eval_file(const char *path_test,const char *path_answerint,
								int &num_test,int &num_answer,int &num_correct,std::string seperator);
		virtual int eval_file(const char *path_test,const char *path_answer,
								double &correct_ratio,double &recall_ratio,std::string seperator);
		
};
#endif