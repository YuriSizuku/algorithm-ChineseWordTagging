#include "DivideWord.h"
#ifndef _BFMM_H
#define _BFMM_H
class BFMM :public DivideWord
{
	
};
class BMM:public BFMM
{
	public:
		int divide_sentence(std::string &sentence,std::string seperator="/");
};
class FMM:public BFMM
{
	public:
		int divide_sentence(std::string &sentence,std::string seperator="/");
};
#endif