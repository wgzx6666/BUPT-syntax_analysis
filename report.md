<strong><font size=6><center>编译原理与技术实验报告</center></font></strong>
<strong><font size=5><center>设计语法分析程序</center></font></strong>

[toc]

###一、实验环境
1. 实验平台：Windows10
2. 编译环境：Visual Studio Code
3. 编程语言：C++
4. 版本号：1.0.0

###二、实验内容及要求
&emsp;&emsp;编写语法分析程序，实现对算术表达式的语法分析。程序可以分析LL(1)文法，自行计算其FIRST集FOLLOW集，为给定文法自动构造预测分析表，并通过预测分析程序根据预测分析表进行逐步分析。根据同步信息的情况进行一定的错误处理。

###三、程序设计
####主要数据结构
```cpp
multimap <string,string> grammar;     //文法
vector <string> nonter;               //非终结符
vector <string> ter;                  //终结符
map <string,set<string> > first;      //first集
map <string,set<string> > follow;     //follow集
pair <string,string> table[30][20];   //预测分析表
vector <string> stack;                //预测分析栈
```
&emsp;&emsp;由于输入文法时采取一个非终结符多个产生式时分别读入的策略，所以采用```multimap```容器来存储文法。终结符非终结符用```vector```来存储。对于FIRST集，FOLLOW集，由于集合与非终结符之间有对应关系，所以采取了```map```结构。预测分析表采用```pair```数组的形式。考虑到方便输出，预测分析时用到的栈采用```vector```容器，而没有使用c++的```stack```结构。
####程序设计与算法
```cpp
class BuildSet
{
public:
    void buildfirstset();             //构建first集
    void ergodic(string a);           //递归构建first集
    void buildfollowset()             //构建follow集
}
```
&emsp;&emsp;构建FIRST集时首先将所有```nonter```中所有非终结符插入```map<string,set> first```中。然后调用```ergodic```函数，依次计算每一个非终结符的FIRST集。在```ergodic```函数中，传入的参数为非终结符。对于该符，首先遍历该符在文法中的所有产生式，使用```while```循环和一个```pos```指针遍历每个产生式的右部。如果字符是"@"或是终结符，则将其加入FIRST集，跳出循环，分析下一个产生式；如果是非终结符，则判断其FIRST集是否构造完成，没构造完则递归构造。构造完成后将其FIRST集所有非"@"元素加入本符的FIRST集。若含有"@"，则```pos```指针加一，继续遍历右部；不含空则跳出循环，继续处理下一个产生式。
&emsp;&emsp;构建FOLLOW集时，首先在初始符号的FOLLOW集中加入"$"，然后用```while```循环和一个```pos```指针遍历每个产生式的右部。
```cpp
class BuildTable
{
public:
    void input(string& str)           //读取文件输入文法
    void bt()                         //构建预测分析表
    void outputtable()                //输出预测分析表
}
```
```cpp
class Predict
{
public:
    void predict(string& str)         //进行非递归预测分析
    void error(int fl)                //输出错误
}
```

####错误处理
&emsp;&emsp;

###四、程序输入输出及执行结果
####输入形式
&emsp;&emsp;
####输出形式
&emsp;&emsp;
####执行结果