#include "Viterbi.h"
#include <iostream>
int* Viterbi(int num_obs,int num_states,int len_obs,
			 int *obs,int *states,
			 double *start_p,double **trans,double **emit)//要求trans，emits必须是连续的二维数组
{
	/*创建变量*/
	double **v=new double*[len_obs];			//每个状态的概率
	int **path;									//到最终状态的最大概率路径的指针
	int **tpath;								//dp用的临时路径的指针
	int ***path_data=new int**[2];				//交换路径缓冲区
	double **trans_p=new double*[num_states];	//传递矩阵
	double **emit_p=new double*[num_states];	//发射矩阵
	int y,y0;									//隐藏态
	int i,j,t;
	

	path_data[0]=new int*[num_states];path_data[1]=new int*[num_states];
	path=path_data[0];tpath=path_data[1];

	for(i=0;i<len_obs;i++)
		v[i]=new double[num_states];
	for(i=0;i<num_states;i++)
	{
		path[i]=new int[len_obs];
		tpath[i]=new int[len_obs];
		trans_p[i]=(double*)trans+i*num_states;
		emit_p[i]=(double*)emit+i*num_obs;
	}
	/*END*/
	
	//初始化概率
	for(i=0;i<num_states;i++)
	{
		y=states[i];
		v[0][y]=start_p[y] * emit_p[y][obs[0]];
		path[y][0]=y;//到初始点状态路径即为本身
	}
	//dp
	for(t=1;t<len_obs;t++)
	{
		for(i=0;i<num_states;i++)//t列状态
		{
			y=states[i];
			double prob=-1.0;//最大概率
			for(j=0;j<num_states;j++)//t-1列状态
			{
				y0=states[j];
				//double tprob=v[t-1][y0] * trans_p[y0][y] * emit_p[y][obs[t]];//这样连乘，可能会溢出
				double tprob=v[t-1][y0] * trans_p[y0][y];
				tprob*=emit_p[y][obs[t]];
				if(tprob > prob)
				{
					prob=tprob;
					v[t][y]=prob;
					//memcpy(tpath[y],path[y0],t*sizeof(double));//这句会使堆栈损坏
					for(int k=0;k<t;k++)//复制路径,到这列的最优路径
						tpath[y][k]=path[y0][k];
					tpath[y][t]=y;//到第t列y状态的路径为，上一列最大概率上一列路径  和 y点
				}
			}
		}
		int **temp;
		temp=tpath;tpath=path;path=temp;//交换路径缓存区
	}

	//取得概率最大的列
	double prob=-1.0;
	int *result;
	for(i=0;i<num_states;i++)
	{
		y=states[i];
		if(v[len_obs-1][y]>prob)
		{
			prob=v[len_obs-1][y];
			result=path[y];
		}
	}

	/*释放变量*/
	for(i=0;i<len_obs;i++)
		delete[] v[i];
	for(i=0;i<num_states;i++)
	{
		delete[] tpath[i];
		if(result!=path[i]) delete[] path[i];
	}
	delete[] v;
	delete[] path_data[0];
	delete[] path_data[1];
	delete[] path_data;
	delete[] trans_p;
	delete[] emit_p;
	/*END*/
	return result;//返回最终概率最大状态的路径行
}