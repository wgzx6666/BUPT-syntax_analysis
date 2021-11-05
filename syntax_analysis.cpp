#include<iostream>
#include<string>
#include<set>
#include<map>
#include<iomanip>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;

multimap <string,string> grammar;//文法
vector <string> nonter;//非终结符
vector <string> ter;//终结符

map <string,set<string> > first;//first集
map <string,set<string> > follow;//follow集
pair <string,string> table[30][20];//预测分析表
vector <string> stack;//预测分析栈

class BuildSet
{
public:
    void buildfirstset()
    {
        for(int i=0;i<nonter.size();i++)
        {
            first.insert(pair<string,set<string> >(nonter[i],{}));
        }
        for(auto it=first.begin();it!=first.end();it++)//遍历每个非终结符
        {
            ergodic(it->first);
        }
        cout << "FIRST:" << endl;
        for(auto it=first.begin();it!=first.end();it++)//输出first集
        {
            cout << it->first << ":";
            for(auto ti=it->second.begin();ti!=it->second.end();ti++)
            {
                cout << *ti << " ";
            }
            cout << endl;
        }
    }
    void ergodic(string a)
    {
        auto er=first.begin();
        bool flag=false;
        for(er=first.begin();;er++)
        {
            if(er->first==a)
            {
                break;
            }
        }
        int pos=0;
        for(auto i=grammar.begin();i!=grammar.end();i++)//每个产生式
        {
            pos=0;
            if(i->first==a)//产生式左部为所求非终结符
            {
                while(pos<i->second.size())
                {
                    flag=false;
                    char x=i->second.at(pos);
                    string y=i->second.substr(pos,1);
                    if(y=="@")
                    {
                        er->second.insert(y);//插入first集
                        break;
                    }
                    else if(x<'A'||x>'Z')//终结符
                    {
                        er->second.insert(y);//插入first集
                        break;
                    }
                    else//非终结符
                    {
                        for(auto rt=first.begin();rt!=first.end();rt++)
                        {
                            if(rt->first==y)
                            {
                                if(rt->second.size()==0)//如果first集还未构造
                                {
                                    ergodic(y);//递归构造非终结符的first集
                                }
                                for(auto qwe=rt->second.begin();qwe!=rt->second.end();qwe++)
                                {
                                    if(*qwe!="@")
                                    {
                                        er->second.insert(*qwe);
                                    }
                                    else if(*qwe=="@")
                                    {
                                        flag=true;
                                    }
                                }
                                break;
                            }
                        }
                        if(flag==false)
                        {
                            break;
                        }
                    }
                    pos++;
                }
                if(pos==i->second.size()&&flag==true)//产生式直到最后一位仍为非终结符且first集含空
                {
                    er->second.insert("@");
                }
            }
        }
        return;
    }
    void buildfollowset()
    {
        multimap <string,string> tran;//记录follow集加入动作
        for(int i=0;i<nonter.size();i++)
        {
            follow.insert(pair<string,set<string> >(nonter[i],{}));
            if(i==0)
            {
                auto j=follow.begin();
                j->second.insert("$");//初始符号添加"$"
            }
        }
        int pos=0;//生成式右部遍历指针
        for(auto it=grammar.begin();it!=grammar.end();it++)
        {
            pos=0;
            while(pos<it->second.size())
            {
                char x=it->second.at(pos);
                string y=it->second.substr(pos,1);
                if(x<'A'||x>'Z')//为终结符
                {
                    pos++;
                    continue;
                }
                else//非终结符
                {
                    int tem=pos+1;
                    if(tem<it->second.size())
                    {
                        string y1;
                        while(tem<it->second.size())
                        {
                            bool fl=false;
                            char x1=it->second.at(tem);
                            y1=it->second.substr(tem,1);
                            if(x1<'A'||x1>'Z')//下一个字符是终结符
                            {
                                for(auto yu=follow.begin();yu!=follow.end();yu++)
                                {
                                    if(y==yu->first)
                                    {
                                        yu->second.insert(y1);
                                        break;
                                    }
                                }
                                break;
                            }
                            else//非终结符
                            {
                                auto mn=follow.begin();
                                auto yp=first.begin();
                                for(mn=follow.begin();mn!=follow.end();mn++)
                                {
                                    if(y==mn->first)
                                    {
                                        break;
                                    }
                                }
                                for(yp=first.begin();yp!=first.end();yp++)
                                {
                                    if(yp->first==y1)
                                    {
                                        break;
                                    }
                                }
                                for(auto rt=yp->second.begin();rt!=yp->second.end();rt++)
                                {
                                    if(*rt!="@")
                                    {
                                        mn->second.insert(*rt);//first集加入follow集
                                    }
                                    else
                                    {
                                        fl=true;
                                    }
                                }
                                if(fl==false)//first集中不存在空
                                {
                                    break;
                                }
                            }
                            tem++;
                        }
                        if(tem==it->second.size())
                        {
                            tran.insert(pair<string,string>{it->first,y});
                        }
                    }
                    else
                    {
                        tran.insert(pair<string,string>{it->first,y});//记录生成式左部的follow集加入右部最后非终结符
                    }
                }
                pos++;
            }
        }
        for(auto q=tran.begin();q!=tran.end();q++)
        {
            if(q->first==q->second)
            {
                tran.erase(q);
            }
        }
        while(tran.size()>0)//按照存储的动作将相应follow集加入
        {
            set<string> temp;
            for(auto q=tran.begin();q!=tran.end();q++)
            {
                temp.insert(q->second);
            }
            for(auto q=tran.begin();q!=tran.end();q++)
            {
                if(temp.find(q->first)==temp.end())
                {
                    auto hg=follow.find(q->first);
                    auto bn=follow.find(q->second);
                    bn->second.insert(hg->second.begin(),hg->second.end());//加入follow集
                    tran.erase(q);
                }
            }
        }
        cout << "FOLLOW:" << endl;
        for(auto it=follow.begin();it!=follow.end();it++)//输出follow集
        {
            cout << it->first << ":";
            for(auto ti=it->second.begin();ti!=it->second.end();ti++)
            {
                cout << *ti << " ";
            }
            cout << endl;
        }
    }
};
class BuildTable
{
public:
    void input(string& str)
    {
        string temp,temp1,temp2;
        ifstream fin(str,ios::in);//读文件
        while(std::getline(fin,temp))
        {
            if(temp.at(1)=='-')
            {
                temp1=temp.substr(0,1);
                temp2=temp.substr(3);
                grammar.insert({temp1,temp2});
                bool flag=false;
                for(auto i=nonter.begin();i!=nonter.end();i++)
                 {
                     if(*i==temp1)
                    {
                        flag=true;
                        break;
                    }
                }
                if(flag==false)
                {
                    nonter.push_back(temp1);//存入非终结符
                }
                flag=false;
                for(int i=0;i<temp2.length();i++)
                {
                    if((temp2.at(i)<'A'||temp2.at(i)>'Z')&&temp2.at(i)!='@')//如果是终结符
                    {
                        string temp3(1,temp2.at(i));
                        for(auto j=ter.begin();j!=ter.end();j++)
                        {
                            if(*j==temp3)
                            {
                                flag=true;
                                break;
                            }
                        }
                        if(flag==false)
                        {
                            ter.push_back(temp3);//存入终结符
                        }
                    }
                }
            }
            
        }
        ter.push_back("$");
        fin.close();
    }
    void bt()//构建预测分析表
    {
        for(auto it=grammar.begin();it!=grammar.end();it++)//文法的每一个产生式
        {
            int pos=0;
            bool flag=true;
            while(pos<it->second.size())//遍历产生式右部
            {
                flag=true;
                char x=it->second.at(pos);
                string y=it->second.substr(pos,1);
                if(x>='A'&&x<='Z')//非终结符
                {
                    int tablex,tabley;
                    for(int k=0;k<nonter.size();k++)
                    {
                        if(nonter[k]==it->first)
                        {
                            tablex=k;
                            break;
                        }
                    }
                    for(auto ip=first.begin();ip!=first.end();ip++)
                    {
                        if(ip->first==y)//找到该非终结符的first集
                        {
                            for(auto io=ip->second.begin();io!=ip->second.end();io++)//遍历first集
                            {
                                if(*io=="@")
                                {
                                    flag=false;
                                }
                                for(int k=0;k<ter.size();k++)
                                {
                                    if(ter[k]==*io)
                                    {
                                        tabley=k;
                                        table[tablex][tabley]={it->first,it->second};
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                else if(x!='@')//终结符
                {
                    int tablex,tabley;
                    for(int k=0;k<nonter.size();k++)
                    {
                        if(nonter[k]==it->first)
                        {
                            tablex=k;
                            break;
                        }
                    }
                    for(int k=0;k<ter.size();k++)
                    {
                        if(ter[k]==y)
                        {
                            tabley=k;
                            break;
                        }
                    }
                    table[tablex][tabley]={it->first,it->second};
                    break;//判断为终结符后就进入下一个文法
                }
                else if(x=='@')//为空
                {
                    int tablex,tabley;
                    for(int k=0;k<nonter.size();k++)
                    {
                        if(nonter[k]==it->first)
                        {
                            tablex=k;
                            break;
                        }
                    }
                    for(auto ip=follow.begin();ip!=follow.end();ip++)
                    {
                        if(ip->first==it->first)//找到该非终结符的follow集
                        {
                            for(auto io=ip->second.begin();io!=ip->second.end();io++)//遍历follow集
                            {
                                for(int k=0;k<ter.size();k++)
                                {
                                    if(ter[k]==*io)
                                    {
                                        tabley=k;
                                        if(table[tablex][tabley].first=="")
                                        {
                                            table[tablex][tabley]={it->first,it->second};                                            
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                pos++;
                if(flag==true)//如果first集不含空
                {
                    break;
                }
            }
        }
        for(auto i=follow.begin();i!=follow.end();i++)//填入同步信息
        {
            int tablex,tabley;
            for(int k=0;k<nonter.size();k++)
            {
                if(nonter[k]==i->first)
                {
                    tablex=k;
                    break;
                }
            }
            for(auto j=i->second.begin();j!=i->second.end();j++)
            {
                for(int k=0;k<ter.size();k++)
                {
                    if(ter[k]==*j)
                    {
                        tabley=k;
                        break;
                    }
                }
                if(table[tablex][tabley].first=="")
                {
                    table[tablex][tabley].first="synch";
                    table[tablex][tabley].second="synch";
                }
            }
        }
    }
    void outputtable()//输出预测分析表
    {
        for(int j=0;j<ter.size();j++)
        {
            cout << setw(10) << ter[j];
        }
        cout << endl;
        for(int i=0;i<nonter.size();i++)
        {
            cout << nonter[i] << " ";
            for(int j=0;j<ter.size();j++)
            {
                if(table[i][j].first!=""&&table[i][j].first!="synch")
                {
                    string ou=table[i][j].first+"->"+table[i][j].second;
                    cout << setw(10) << ou;
                }
                else if(table[i][j].first=="synch")
                {
                    cout << setw(10) << "synch";
                }
                else
                {
                    cout << setw(10) << "";
                }
            }
            cout << endl;
        }
    }
};
class Predict
{
public:
    void predict(string& str)//进行非递归预测分析
    {
        int ip=0;//向前指针
        string x;//栈顶文法符号
        string a;//ip所指向的输入符号
        stack.push_back("$");//$压入栈底
        auto it=grammar.begin();
        stack.push_back(it->first);//起始符号压入栈顶
        x=it->first;
        str+="$";//字符串ω和$放入缓冲区
        a=str.substr(0,1);
        do
        {
            string qw;
            auto h=find(ter.begin(),ter.end(),x);
            if(x=="@")//如果x为空
            {
                stack.pop_back();
            }
            else if(x=="$"||h!=ter.end())//如果x是$或终结符号
            {
                if(x==a)
                {
                    stack.pop_back();
                    ip++;
                    a=str.substr(ip,1);
                }
                else
                {
                    error(1);//栈顶符号与当前输入符号不匹配
                    stack.pop_back();
                }
            }
            else//x是非终结符号
            {
                int tablex,tabley;
                for(int i=0;i<ter.size();i++)
                {
                    if(a==ter[i])
                    {
                        tabley=i;
                        break;
                    }
                }
                for(int i=0;i<nonter.size();i++)
                {
                    if(x==nonter[i])
                    {
                        tablex=i;
                        break;
                    }
                }
                if(table[tablex][tabley].first!=""&&table[tablex][tabley].first!="synch")
                {
                    stack.pop_back();//从栈顶弹出x
                    string rt(table[tablex][tabley].second.rbegin(),table[tablex][tabley].second.rend());
                    for(int p=0;p<rt.size();p++)
                    {
                        stack.push_back(rt.substr(p,1));//倒序放入栈中
                    }
                    qw=table[tablex][tabley].first+"->"+table[tablex][tabley].second;
                }
                else if(table[tablex][tabley].first=="synch")//表中该项为同步信息
                {
                    error(2);
                    stack.pop_back();
                }
                else//表中该项为空白
                {
                    error(3);
                    ip++;
                    a=str.substr(ip,1);//向前移动指针
                }
            }
            cout << setw(20) << qw;
            x=stack[stack.size()-1];
            string aq;
            for(int ty=0;ty<stack.size();ty++)
            {
                aq+=stack[ty];
            }
            cout << setw(20) << aq;
            cout << setw(20) << str.substr(ip) << endl;
        }
        while(x!="$");
        if(ip!=(str.size()-1))
        {
            error(4);
        }
    }
    void error(int fl)
    {
        if(fl==1)
        {
            cout << "栈顶符号与当前输入符号不匹配" << endl;
        }
        else if(fl==2)
        {
            cout << "预测分析表中无相应生成式，含有同步信息，从栈顶弹出非终结符" << endl;
        }
        else if(fl==3)
        {
            cout << "预测分析表中无相应生成式，不含有同步信息，向前移动指针" << endl;
        }
        else
        {
            cout << "输入串长度不符合文法要求" << endl;
        }
    }
};

int main()
{
    string str;
    BuildSet buildset;
    BuildTable buildtable;
    Predict predict;
    // cout << "请输入进行语法分析的文法" << endl;
    // cin >> str;
    // str+=".txt";
    str="t2.txt";
    buildtable.input(str);
    buildset.buildfirstset();
    buildset.buildfollowset();
    buildtable.bt();
    buildtable.outputtable();
    // cout << "请输入进行语法分析的符号串" << endl;
    // cin >> str;
    str="101";
    predict.predict(str);
    return 0;
}