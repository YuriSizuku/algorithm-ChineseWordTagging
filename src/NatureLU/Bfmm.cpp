#include <string>
#include "bfmm.h"
using namespace std;
int BMM::divide_sentence(string &sentence,string seperator)//句子中不能有标点、空格
{
	if(dict_index.len_max==0)
		createDictIndex(false);
	int count=0;
	int pi=0;//当前位置
	int m=dict_index.len_max;//词典中最长的字符长度
	int n=sentence.length()-pi;//剩余文字的长度
	string tword;//当前词语
	
	//逐字节匹配，与字典中比对，不会出问题
	while(n >= 1 )
	{
		m=dict_index.len_max;
		if(n<m) m=n;
		tword=sentence.substr(pi,m);
		if(searchWord(tword)!=-1)//找到匹配
		{
			sentence.insert(pi+m,seperator);
			pi+=seperator.length();
			count++;
			pi+=m;
			n=sentence.length()-pi;
		}
		else
		{
			if(n==2 && (unsigned char)sentence[pi]>=128) //全角字符
			{
				sentence.insert(pi+2,seperator);
				pi+=seperator.length();
				count++;
				pi+=2;
				n=sentence.length()-pi;
			}
			else if(n==1 && (unsigned char)sentence[pi]<128) //半角字符
			{
				sentence.insert(pi+1,seperator);
				pi+=seperator.length();
				count++;
				pi++;
				n=sentence.length()-pi;
			}
			else
				n--;
		}
	}
	return count;
}
int FMM::divide_sentence(string &sentence,string seperator)
{
	if(dict_index.len_max==0)
		createDictIndex(false);
	int count=0;

	int pi=sentence.length();//指向子串尾
	int m=dict_index.len_max;//词典中最长的字符长度
	int n=pi;//剩余文字的长度
	string tword;//当前词语
	while(n >= 1)
	{
		m=dict_index.len_max;
		if(n<m) m=n;
		tword=sentence.substr(pi-m,m);
		if(searchWord(tword)!=-1)//找到匹配
		{
			sentence.insert(pi,seperator);
			count++;
			pi-=m;
			n=pi;
		}
		else
		{
			if(n==2 && (unsigned char)sentence[pi-1]>=128) //全角字符
			{
				sentence.insert(pi,seperator);
				count++;
				pi-=2;
				n=pi;
			}
			else if(n==1 && (unsigned char)sentence[pi-1]<128) //半角字符
			{
				sentence.insert(pi,seperator);
				count++;
				pi--;
				n=pi;
			}
			else
				n--;
		}
	}
	return count;	 
}