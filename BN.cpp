#include<stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>
#include<list>
#include<cmath>
int NumProb,NumQ;
class vnode;
class Qvar;
class Q;

class vnode {
public:
std::string Name;
 double Myprobabilities[2];
 std::vector<Qvar*> Parents;
std::vector<std::vector <double> > CPT;
friend class Qvar;
};

class Qvar{
public:
vnode* myvar;
int myval;
std::vector<Qvar*> Parents;
friend class vnode;
};

class Q{
public:
std::vector<Qvar*> NumVisible;
std::vector<Qvar*> NumHidden;
std::vector<Qvar*> DenVisible;
std::vector<Qvar*> DenHidden;
};

std::vector<Q*> Queries;
std::vector<vnode*> variables;
std::vector<vnode*> parented;
vnode* findv(std::string name, std::vector<vnode*>where)
{
    vnode* p=0;
    if (!where.empty())
    {
       for(unsigned int i=0;i<where.size();i++)
       {if(name.compare(where[i]->Name)==0)
       p=where[i];
       }
    }

    return p;
}

Qvar* findq(std::string name, std::vector<Qvar*>where)
{
    Qvar* p=0;
    if (!where.empty())
    {
       for(unsigned int i=0;i<where.size();i++)
       {if(name.compare(where[i]->myvar->Name)==0)
       p=where[i];
       }
    }

    return p;
}

void initialize1(std::string line)
{
    std::string buffer;
    std::vector <double> initializer;
    vnode* p;
    for (unsigned int i=0;i<line.size();i++)
    {
        if (line[i]!=char(32)&&line[i]!=',')
            buffer.push_back(line[i]);
        else if (line[i]==',')
        {
            p=new vnode;
            p->Name.clear();
            p->Name+=(buffer);
            p->CPT.push_back(initializer);
            p->CPT.push_back(initializer);
            buffer.clear();
            variables.push_back(p);
            p=0;
        }
    }
         p=new vnode;
            p->Name.clear();
            p->Name+=(buffer);
            p->CPT.push_back(initializer);
            p->CPT.push_back(initializer);
            buffer.clear();
            variables.push_back(p);
            p=0;

return;
}

void initialize2(std::string line)
{
    bool child=0;
    int id1=0,id2=0;
    int val=1;
    vnode* p;
    vnode* q;
    vnode* r;

    std::string buff;
    p=0;
    q=0;
for (unsigned int i=0;i<line.size();i++)
{
    if(line[i]=='+'||line[i]=='-')
    {
        if(!child)
        {
            if(line[i]=='+')
                id1=1;
            else
                id1=0;
        }
        else
        {
             if(line[i]=='+')
                id2+=val;
        }
    }

 if(line[i]!='+'&&line[i]!='-'&&line[i]!='|'&&line[i]!='='&&line[i]!=',')
    {
        buff.push_back(line[i]);
    }
    else if(line[i]=='=')
    {
        if(!child)
        {
            p=findv(buff,variables);
            buff.clear();
        }
        else
        {
        r=findv(p->Name,parented);
        if(r==0)
        {Qvar* s=new Qvar;
        q=findv(buff,variables);
        s->myvar=q;
        s->myval=val;
        p->Parents.push_back(s);}
        buff.clear();
        val*=2;
        }
    }
    else if(line[i]=='|')
    {
        p=findv(buff,variables);
        child=1;
        buff.clear();
    }
    else if(line[i]==',')
    {
        r=findv(p->Name,parented);
        if(r==0)
        {Qvar* s=new Qvar;
        q=findv(buff,variables);
        s->myvar=q;
        s->myval=val;
        p->Parents.push_back(s);}
        buff.clear();
        val*=2;
    }
}
if (!child)
{
    p->Myprobabilities[id1]=atof(buff.c_str());
    p->Myprobabilities[(id1+1)%2]=1-(atof(buff.c_str()));
}
else
{
        p->CPT[id1][id2]=atof(buff.c_str());
        p->CPT[(id1+1)%2][id2]=1-(atof(buff.c_str()));

}
parented.push_back(p);
return;
}


void initializeQ(std::string line)
{
    bool Den=0;
    std::string buff;
    int val=0;
    Q* Query= new Q;
    vnode* q;
    Qvar* r;
    q=0;
    r=0;

  for(unsigned int i=0;i<line.size();i++)
  {
      if(line[i]=='+')
        val=1;
      if(line[i]=='-')
        val=0;
    if(line[i]!='+'&&line[i]!='-'&&line[i]!='|'&&line[i]!=',')
        {
        buff.push_back(line[i]);
        }
    else if(line[i]=='|')
    {
        Qvar* s=new Qvar;
        q=findv(buff,variables);
        s->myvar=q;
        s->myval=val;
        Query->NumVisible.push_back(s);
        buff.clear();
        Den=1;
    }
    else if(line[i]==',')
    {
        Qvar* s=new Qvar;
        q=findv(buff,variables);
        s->myvar=q;
        s->myval=val;
        Query->NumVisible.push_back(s);
        if(Den)
        {
        Query->DenVisible.push_back(s);
        }
        buff.clear();
    }
  }

        Qvar* s=new Qvar;
        q=findv(buff,variables);
        s->myvar=q;
        s->myval=val;
        Query->NumVisible.push_back(s);
        if(Den)
        {
        Query->DenVisible.push_back(s);
        }
        buff.clear();

        for(unsigned int i=0;i<Query->NumVisible.size();i++)
        {
            for(unsigned int j=0;j<Query->NumVisible[i]->myvar->Parents.size();j++)
            {

            r=findq(Query->NumVisible[i]->myvar->Parents[j]->myvar->Name,Query->NumVisible);

            if(r==0)
            {
                Qvar*h=new Qvar;
                h->myvar=Query->NumVisible[i]->myvar->Parents[j]->myvar;
                h->myval=0;
                Query->NumHidden.push_back(h);
                Query->NumVisible.push_back(h);
            }

            }

        }


           for(unsigned int i=0;i<Query->DenVisible.size();i++)
        {
            for(unsigned int j=0;j<Query->DenVisible[i]->myvar->Parents.size();j++)
            {

            r=findq(Query->DenVisible[i]->myvar->Parents[j]->myvar->Name,Query->DenVisible);
            if(r==0)
            {
                Qvar*h=new Qvar;
                h->myvar=Query->DenVisible[i]->myvar->Parents[j]->myvar;
                h->myval=0;
                Query->DenHidden.push_back(h);
                Query->DenVisible.push_back(h);
            }

            }

        }
Queries.push_back(Query);
  return;
}

int Parentval(Qvar* Child)
{
    int val=0;
    for(unsigned int i=0;i<Child->Parents.size();i++)
        {val+=(Child->Parents[i]->myval)*(Child->myvar->Parents[i]->myval);
    }
    return val;
}

void increase(std::vector<Qvar*> NV,unsigned int index)
{
    if(index<NV.size())
    {
    NV[index]->myval+=1;
    if(NV[index]->myval>=2)
    {
        NV[index]->myval=0;
        if(index+1<NV.size())
        increase(NV,(index+1));
    }
    }
    return;
}

void assignparent(std::vector<Qvar*> NV,std::vector<Qvar*> NV2)
{
    Qvar* s;
    for(unsigned int j=0;j<NV.size();j++)
    {
        NV[j]->Parents.clear();
        for(unsigned int k=0;k<NV[j]->myvar->Parents.size();k++)
        {

            s=findq(NV[j]->myvar->Parents[k]->myvar->Name,NV2);
            if(s!=0)
                NV[j]->Parents.push_back(s);
        }
    }
    //std::cout<<"ups";
}

double calcprob(std::vector<Qvar*> NV,Q* QR,std::vector<Qvar*> NH)
{
    double combNum=1;
    double Prob=0;
    int Pval;

        for(int j=0;j<pow(2,NH.size());j++)
    {

        for(unsigned int k=0;k<NV.size();k++)
        {
            if(NV[k]->Parents.empty())
                {combNum*=NV[k]->myvar->Myprobabilities[NV[k]->myval];
                //std::cout<<NV[k]->myvar->Myprobabilities[NV[k]->myval]<<std::endl;
                //std::cout<<NV[k]->myvar->Name<<std::endl;
                }
            else{
                    Pval=Parentval(NV[k]);
                combNum*=NV[k]->myvar->CPT[NV[k]->myval][Pval];
               // std::cout<<NV[k]->myvar->CPT[NV[k]->myval][Pval]<<std::endl;
                //std::cout<<NV[k]->myvar->Name<<std::endl;
            }
        }

    Prob+=combNum;
       //std::cout<<Prob<<std::endl;
        combNum=1;

        increase(NH,0);

    }
    //std::cout<<Prob<<std::endl;
    return Prob;
}

int main()
{
    double ProbNum=0, ProbDen=0;
    std::string read;
    std::getline(std::cin, read);
        initialize1(read);
    for(unsigned int a=0;a<pow(2,((variables.size())-1));a++)
    {
    for(unsigned int i=0;i<variables.size();i++)
        {
        variables[i]->CPT[1].push_back(0);
        variables[i]->CPT[0].push_back(0);
        }
    }
    std::cin>>NumProb;
    std::getline(std::cin,read);
for(int a=0;a<NumProb;a++)
{
    std::getline(std::cin,read);
    initialize2(read);
}

 std::cin>>NumQ;
 std::getline(std::cin,read);

for(int b=0;b<NumQ;b++)
{
    std::getline(std::cin,read);
    initializeQ(read);

}

for(unsigned int i=0;i<Queries.size();i++)
{
    assignparent(Queries[i]->NumVisible,Queries[i]->NumVisible);

    //std::cout<<"Ups1";
    /*if(!Queries[i]->DenVisible.empty())
    {
    if(Queries[i]->DenVisible.size()>2)
    std::cout<<Queries[i]->DenVisible[2]->Parents.size();

    }*/
    ProbNum=calcprob(Queries[i]->NumVisible,Queries[i],Queries[i]->NumHidden);
    //std::cout<<"DEN";
    assignparent(Queries[i]->DenVisible,Queries[i]->DenVisible);
    ProbDen=calcprob(Queries[i]->DenVisible,Queries[i],Queries[i]->DenHidden);

    std::cout<<(ProbNum/ProbDen)<<std::endl;
}
//std::cout<<variables[0]->CPT[0][0]<<Queries[4]->NumVisible[2]->myval;
}

/*
for (int i=0;i<variables.size();i++)
std::cout<<variables[i].Name<<std::endl;
2^((variables.size())-1)
*/
