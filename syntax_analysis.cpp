#include<iostream>
#include<string>
#include<set>
#include<map>
#include<iomanip>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;

multimap <string,string> grammar;//�ķ�
vector <string> nonter;//���ս��
vector <string> ter;//�ս��

map <string,set<string> > first;//first��
map <string,set<string> > follow;//follow��
pair <string,string> table[30][20];//Ԥ�������
vector <string> stack;//Ԥ�����ջ

class BuildSet
{
public:
    void buildfirstset()
    {
        for(int i=0;i<nonter.size();i++)
        {
            first.insert(pair<string,set<string> >(nonter[i],{}));
        }
        for(auto it=first.begin();it!=first.end();it++)//����ÿ�����ս��
        {
            ergodic(it->first);
        }
        cout << "FIRST:" << endl;
        for(auto it=first.begin();it!=first.end();it++)//���first��
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
        for(auto i=grammar.begin();i!=grammar.end();i++)//ÿ������ʽ
        {
            pos=0;
            if(i->first==a)//����ʽ��Ϊ������ս��
            {
                while(pos<i->second.size())
                {
                    flag=false;
                    char x=i->second.at(pos);
                    string y=i->second.substr(pos,1);
                    if(y=="@")
                    {
                        er->second.insert(y);//����first��
                        break;
                    }
                    else if(x<'A'||x>'Z')//�ս��
                    {
                        er->second.insert(y);//����first��
                        break;
                    }
                    else//���ս��
                    {
                        for(auto rt=first.begin();rt!=first.end();rt++)
                        {
                            if(rt->first==y)
                            {
                                if(rt->second.size()==0)//���first����δ����
                                {
                                    ergodic(y);//�ݹ鹹����ս����first��
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
                if(pos==i->second.size()&&flag==true)//����ʽֱ�����һλ��Ϊ���ս����first������
                {
                    er->second.insert("@");
                }
            }
        }
        return;
    }
    void buildfollowset()
    {
        multimap <string,string> tran;//��¼follow�����붯��
        for(int i=0;i<nonter.size();i++)
        {
            follow.insert(pair<string,set<string> >(nonter[i],{}));
            if(i==0)
            {
                auto j=follow.begin();
                j->second.insert("$");//��ʼ�������"$"
            }
        }
        int pos=0;//����ʽ�Ҳ�����ָ��
        for(auto it=grammar.begin();it!=grammar.end();it++)
        {
            pos=0;
            while(pos<it->second.size())
            {
                char x=it->second.at(pos);
                string y=it->second.substr(pos,1);
                if(x<'A'||x>'Z')//Ϊ�ս��
                {
                    pos++;
                    continue;
                }
                else//���ս��
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
                            if(x1<'A'||x1>'Z')//��һ���ַ����ս��
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
                            else//���ս��
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
                                        mn->second.insert(*rt);//first������follow��
                                    }
                                    else
                                    {
                                        fl=true;
                                    }
                                }
                                if(fl==false)//first���в����ڿ�
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
                        tran.insert(pair<string,string>{it->first,y});//��¼����ʽ�󲿵�follow�������Ҳ������ս��
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
        while(tran.size()>0)//���մ洢�Ķ�������Ӧfollow������
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
                    bn->second.insert(hg->second.begin(),hg->second.end());//����follow��
                    tran.erase(q);
                }
            }
        }
        cout << "FOLLOW:" << endl;
        for(auto it=follow.begin();it!=follow.end();it++)//���follow��
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
        ifstream fin(str,ios::in);//���ļ�
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
                    nonter.push_back(temp1);//������ս��
                }
                flag=false;
                for(int i=0;i<temp2.length();i++)
                {
                    if((temp2.at(i)<'A'||temp2.at(i)>'Z')&&temp2.at(i)!='@')//������ս��
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
                            ter.push_back(temp3);//�����ս��
                        }
                    }
                }
            }
            
        }
        ter.push_back("$");
        fin.close();
    }
    void bt()//����Ԥ�������
    {
        for(auto it=grammar.begin();it!=grammar.end();it++)//�ķ���ÿһ������ʽ
        {
            int pos=0;
            bool flag=true;
            while(pos<it->second.size())//��������ʽ�Ҳ�
            {
                flag=true;
                char x=it->second.at(pos);
                string y=it->second.substr(pos,1);
                if(x>='A'&&x<='Z')//���ս��
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
                        if(ip->first==y)//�ҵ��÷��ս����first��
                        {
                            for(auto io=ip->second.begin();io!=ip->second.end();io++)//����first��
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
                else if(x!='@')//�ս��
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
                    break;//�ж�Ϊ�ս����ͽ�����һ���ķ�
                }
                else if(x=='@')//Ϊ��
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
                        if(ip->first==it->first)//�ҵ��÷��ս����follow��
                        {
                            for(auto io=ip->second.begin();io!=ip->second.end();io++)//����follow��
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
                if(flag==true)//���first��������
                {
                    break;
                }
            }
        }
        for(auto i=follow.begin();i!=follow.end();i++)//����ͬ����Ϣ
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
    void outputtable()//���Ԥ�������
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
    void predict(string& str)//���зǵݹ�Ԥ�����
    {
        int ip=0;//��ǰָ��
        string x;//ջ���ķ�����
        string a;//ip��ָ����������
        stack.push_back("$");//$ѹ��ջ��
        auto it=grammar.begin();
        stack.push_back(it->first);//��ʼ����ѹ��ջ��
        x=it->first;
        str+="$";//�ַ����غ�$���뻺����
        a=str.substr(0,1);
        do
        {
            string qw;
            auto h=find(ter.begin(),ter.end(),x);
            if(x=="@")//���xΪ��
            {
                stack.pop_back();
            }
            else if(x=="$"||h!=ter.end())//���x��$���ս����
            {
                if(x==a)
                {
                    stack.pop_back();
                    ip++;
                    a=str.substr(ip,1);
                }
                else
                {
                    error(1);//ջ�������뵱ǰ������Ų�ƥ��
                    stack.pop_back();
                }
            }
            else//x�Ƿ��ս����
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
                    stack.pop_back();//��ջ������x
                    string rt(table[tablex][tabley].second.rbegin(),table[tablex][tabley].second.rend());
                    for(int p=0;p<rt.size();p++)
                    {
                        stack.push_back(rt.substr(p,1));//�������ջ��
                    }
                    qw=table[tablex][tabley].first+"->"+table[tablex][tabley].second;
                }
                else if(table[tablex][tabley].first=="synch")//���и���Ϊͬ����Ϣ
                {
                    error(2);
                    stack.pop_back();
                }
                else//���и���Ϊ�հ�
                {
                    error(3);
                    ip++;
                    a=str.substr(ip,1);//��ǰ�ƶ�ָ��
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
            cout << "ջ�������뵱ǰ������Ų�ƥ��" << endl;
        }
        else if(fl==2)
        {
            cout << "Ԥ�������������Ӧ����ʽ������ͬ����Ϣ����ջ���������ս��" << endl;
        }
        else if(fl==3)
        {
            cout << "Ԥ�������������Ӧ����ʽ��������ͬ����Ϣ����ǰ�ƶ�ָ��" << endl;
        }
        else
        {
            cout << "���봮���Ȳ������ķ�Ҫ��" << endl;
        }
    }
};

int main()
{
    string str;
    BuildSet buildset;
    BuildTable buildtable;
    Predict predict;
    // cout << "����������﷨�������ķ�" << endl;
    // cin >> str;
    // str+=".txt";
    str="t2.txt";
    buildtable.input(str);
    buildset.buildfirstset();
    buildset.buildfollowset();
    buildtable.bt();
    buildtable.outputtable();
    // cout << "����������﷨�����ķ��Ŵ�" << endl;
    // cin >> str;
    str="101";
    predict.predict(str);
    return 0;
}