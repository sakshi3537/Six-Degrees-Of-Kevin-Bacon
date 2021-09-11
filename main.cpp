#include<bits/stdc++.h>
using namespace std;
unordered_map<string,int> movie,actor;

void fillupmovie(int num)
{
    string line;
    ifstream myfile("movie.txt");
    if(myfile.is_open())
    {
        while( getline(myfile,line))
        {
            movie[line]=num;
            num++;
        }
        myfile.close();
    }
}

void fillupactor(int num)
{
    string line;
    ifstream myfile("actor.txt");
    if(myfile.is_open())
    {
        while(getline(myfile,line))
        {
            int len=line.size();
            int start=0;
            for(int i=0;i<len;i++)
            {
                if(line[i]=='|')
                {
                    if(actor.find(line.substr(start,i-start))==actor.end())
                    {
                        actor[line.substr(start,i-start)]=num;
                        num++;
                    }
                    start=i+1;
                }
            }
            if(actor.find(line.substr(start))==actor.end())
            {
                actor[line.substr(start)]=num;
                num++;
            }
        }
        myfile.close();
    }
    else cout<<"Couldn't open file";
}

void fillupgraph(vector<vector<int> > &graph,int n)
{
    string line;
    ifstream myfile("conn.txt");
    if(myfile.is_open())
    {
        while(getline(myfile,line))
        {
            int len=line.size(),start=0;
            bool flag=true;
            vector<int> a_num;
            int m_num;
            for(int i=0;i<len;i++)
            {
                if(flag==false&&line[i]=='|')
                {
                    flag=true;
                    m_num=movie[line.substr(0,i-1)];
                    start=i+1;
                }
                else if(line[i]=='|')
                {
                    a_num.push_back(actor[line.substr(start,i-start)]);
                    start=i+1;
                }
            }
            a_num.push_back(actor[line.substr(start)]);
            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    graph[a_num[i]][a_num[j]]=m_num;
                    graph[a_num[j]][a_num[i]]=m_num;
                }
            }
            a_num.clear();
        }
        myfile.close();
    }
    else cout<<"Couldn't open file";
}

string getActorName(int num)
{
    for(auto itr=actor.begin();itr!=actor.end();itr++)
    {
        if(itr->second==num)
        {
            return itr->first;
        }
    }
    return "";
}

string getMovieName(int num)
{
    for(auto itr=movie.begin();itr!=movie.end();itr++)
    {
        if(itr->second==num)
        {
            return itr->first;
        }
    }
    return "";
}

void bfs(int src,int dest,vector<vector<int> > &graph,int n)
{
    queue<int> q;
    bool marked[n];
    int pre[n];
    for(int i=0;i<n;i++)
    {
        marked[i]=false;
        pre[i]=-1;
    }
    marked[src]=true;
    q.push(src);
    bool flag=false;
    while(!q.empty())
    {
        int val=q.front();
        q.pop();
        for(int i=0;i<n;i++)
        {
            if(graph[val][i]!=-1&&marked[i]!=true)
            {
                marked[i]=true;
                pre[i]=val;
                q.push(i);
                if(i==dest)
                {
                    flag=true;
                    break;
                }
            }
        }
        if(flag)
            break;
    }
    if(flag)
    {
        vector<int> connection;
        int pt=dest;
        connection.push_back(pt);
        while(pre[pt]!=-1)
        {
            connection.push_back(pre[pt]);
            pt=pre[pt];
        }
        cout<<"The bacon number of "<<getActorName(connection[0]) <<" to " << getActorName(connection[connection.size()-1])<<" is "<<connection.size()-1<<endl;
        for(int i=connection.size()-1;i>0;i--)
        {
            cout<<getActorName(connection[i])<<" is in the movie "<<getMovieName(graph[connection[i]][connection[i-1]])<<" with actor "<<getActorName(connection[i-1])<<endl;
        }
    }
    else
    {
        cout<<"Disconnected";
    }
}

int main()
{
    fillupactor(0);
    fillupmovie(0);
    int n=actor.size();
    vector<vector<int> > graph(n,vector<int>(n,-1));
    fillupgraph(graph,n);
    string src,dest;
    getline(cin,src);
    getline(cin,dest);
    bfs(actor[src],actor[dest],graph,n);
    return 0;
}

