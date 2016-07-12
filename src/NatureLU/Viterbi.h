/*
  维特比算法，返回最佳隐藏态序列
  要求trans，emits必须是连续的二维数组
*/
#ifndef _VITERBI_H
#define _VITERBI_H
//函数形式，返回值是长度为num_obs队上的数组
int* Viterbi(int num_obs,int num_states,int len_obs,
			 int *obs,int *states,
			 double *start_p,double **trans,double **emit);

//类形式，
//多次用相同模型调用不再重复分配内存，提高效率，
//但是实测hmm算法效率还可以，暂时不写这个类
class Viterbi
{

};
#endif