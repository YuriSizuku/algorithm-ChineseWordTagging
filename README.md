# ChineseWordTagging
A Chinese word tagging project based on fmm, bmm, hmm (Hidden Markov Model). 
<p> 1.本项目（ver0.8）实现了对asci编码（GB2312）的中文文章进行词性标注（文章中可以出现半角字符）。本项目是在之前的分词程序基础上开发而成（ver0.7）
<p> 2.通过正向最大匹配（fmm），逆向最大匹配（bmm），隐马尔可夫模型（hmm）分词。分词完了用隐马尔可夫模型进行词性标注
<p> 3.词性标注的隐马尔可夫可由语料库训练生成
<p> 4.对分词结果的正确率、召回率即时间进行评价。
<p> 5.本项目采取面向对象的编程思路，由c++编写完成，同时兼顾效率，方便后期添加功能
