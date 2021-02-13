#include<bits/stdc++.h>
using namespace std;
#define INF 7000000000000000000

//////////////////////////////////////////////
///          auxiliary functions           ///
//////////////////////////////////////////////

unsigned int IP_to_number(string IP)
{
    IP+='.';
    unsigned int address = 0, temp=0;
    int i=0;
    while(i<IP.size())
    {
        if(IP[i]=='.')
        {
            address*=256;
            address+=temp;
            temp=0;
        }
        else
        {
            temp*=10;
            temp+=IP[i]-'0';
        }
        i++;
    }
    return address;
}

string IP_to_string(unsigned int address)
{
    string IP;
    int counter=0, pom=0;
    for(int i=0; i<3; i++)
    {
        pom=address%256;
        address/=256;
        if(pom==0)
            IP='0'+IP;
        while(pom!=0)
        {
            IP=(char)(pom%10+'0')+IP;
            pom/=10;
        }
        IP='.'+IP;
    }
    pom=address%256;
    if(pom==0)
        IP='0'+IP;
    while(pom!=0)
    {
        IP=(char)(pom%10+'0')+IP;
        pom/=10;
    }
    return IP;
}

//////////////////////////////////////////////
///               tree part                ///
//////////////////////////////////////////////

struct tree{
    long long IP[3] = {0, 0, 0};
    tree* child[4] = {NULL, NULL, NULL, NULL};
    int subtree_size = 0;
};

tree* split(tree* root)
{
    tree* temp = new tree;
    tree* temp2 = new tree;

    if(root->child[0]!=NULL)
    {
        root->subtree_size = root->child[0]->subtree_size + root->child[1]->subtree_size + 1;
        temp2->subtree_size = root->child[2]->subtree_size + root->child[3]->subtree_size + 1;
        temp->subtree_size = root->subtree_size + temp2->subtree_size + 1;
    }
    else
    {
        root->subtree_size = 1;
        temp2->subtree_size = 1;
        temp->subtree_size = 3;
    }

    temp->IP[0] = root->IP[1];
    temp2->IP[0] = root->IP[2];

    temp->child[0]=root;
    temp->child[1]=temp2;
    temp2->child[0] = root->child[2];
    temp2->child[1] = root->child[3];
    root->child[2] = NULL;
    root->child[3] = NULL;

    root->IP[1] = 0;
    root->IP[2] = 0;

    return temp;
}

tree* add(unsigned int address, tree* (&root))
{
    int tree_size=0;
    if(root->IP[0]==address || root->IP[1]==address)
        return NULL;
    else if(root->IP[0]>address||root->IP[0]==0)
    {
        if(root->child[0]!=NULL)
        {
            tree* pom=add(address, root->child[0]);
            if(pom==NULL)
            {
                if(root->IP[1]==0)
                    tree_size=1;
                else
                    tree_size=2;
                for(int i=0; i<4; i++)
                    if(root->child[i]!=NULL)
                        tree_size+=root->child[i]->subtree_size;
                root->subtree_size=tree_size;

                return NULL;
            }
            for(int i=2; i>0; i--)
            {
                root->IP[i]=root->IP[i-1];
                root->child[i+1]=root->child[i];
            }
            root->IP[0]=pom->IP[0];
            root->child[1]=pom->child[1];
            root->child[0]=pom->child[0];
            delete pom;

            if(root->IP[1]==0)
                tree_size=1;
            else if(root->IP[2]==0)
                tree_size=2;
            else tree_size=3;
            for(int i=0; i<4; i++)
                if(root->child[i])
                    tree_size+=root->child[i]->subtree_size;
            root->subtree_size=tree_size;

            if(root->IP[2]!=0)
                return split(root);
            return NULL;
        }
        else
        {
            for(int i=2; i>0; i--)
            {
                root->IP[i]=root->IP[i-1];
            }
            root->IP[0]=address;

            (root->subtree_size)++;

            if(root->IP[2]!=0)
                return split(root);
            return NULL;
        }
    }
    else if(root->IP[1]>address||root->IP[1]==0)
    {
        if(root->child[1]!=NULL)
        {
            tree* pom=add(address, root->child[1]);
            if(pom==NULL)
            {
                if(root->IP[1]==0)
                    tree_size=1;
                else if(root->IP[2]==0)
                    tree_size=2;
                else
                    tree_size=3;
                for(int i=0; i<4; i++)
                    if(root->child[i])
                        tree_size += root->child[i]->subtree_size;
                root->subtree_size = tree_size;

                return NULL;
            }
            root->IP[2]=root->IP[1];
            root->child[3]=root->child[2];
            root->IP[1]=pom->IP[0];
            root->child[2]=pom->child[1];
            root->child[1]=pom->child[0];
            delete pom;

            if(root->IP[1]==0)
                tree_size=1;
            else if(root->IP[2]==0)
                tree_size=2;
            else tree_size=3;

            for(int i=0; i<4; i++)
                if(root->child[i])
                    tree_size+=root->child[i]->subtree_size;
            root->subtree_size=tree_size;

            if(root->IP[2]!=0)
                return split(root);
            return NULL;
        }
        else
        {
            root->IP[2]=root->IP[1];
            root->IP[1]=address;

            if(root->IP[1]==0)
                tree_size=1;
            else if(root->IP[2]==0)
                tree_size=2;
            else tree_size=3;

            for(int i=0; i<4; i++)
                if(root->child[i])
                    tree_size+=root->child[i]->subtree_size;
            root->subtree_size=tree_size;
            if(root->IP[2]!=0)
                return split(root);
            else return NULL;
        }
    }
    else if(root->IP[1]<address)
    {
        if(root->child[2]==NULL)
        {
            root->IP[2]=address;
            root->subtree_size=3;
            return split(root);
        }
        else
        {
            tree* pom=add(address, root->child[2]);
            if(pom==NULL)
            {
                if(root->IP[1]==0)
                    tree_size=1;
                else if(root->IP[2]==0)
                    tree_size=2;
                else
                    tree_size=3;

                for(int i=0; i<4; i++)
                    if(root->child[i])
                        tree_size+=root->child[i]->subtree_size;
                root->subtree_size=tree_size;
                return NULL;
            }
            root->IP[2]=pom->IP[0];
            root->child[3]=pom->child[1];
            root->child[2]=pom->child[0];
            delete pom;

            if(root->IP[1]==0)
                tree_size=1;
            else if(root->IP[2]==0)
                tree_size=2;
            else
                tree_size=3;

            for(int i=0; i<4; i++)
                if(root->child[i])
                    tree_size+=root->child[i]->subtree_size;
            root->subtree_size=tree_size;

            return split(root);
        }
    }
    return NULL;
}

void merge(tree* (&root), int child_nr)
{
    tree* temp=NULL;
    if(child_nr==0)
    {
        if(root->child[1]->IP[1]!=0)
        {
            root->child[0]->IP[0]=root->IP[0];
            root->IP[0]=root->child[1]->IP[0];
            root->child[1]->IP[0]=root->child[1]->IP[1];
            root->child[1]->IP[1]=0;

            root->child[0]->child[1]=root->child[1]->child[0];
            root->child[1]->child[0]=root->child[1]->child[1];
            root->child[1]->child[1]=root->child[1]->child[2];
            root->child[1]->child[2]=NULL;
        }
        else
        {
            root->child[0]->IP[0]=root->IP[0];
            root->IP[0]=root->IP[1];
            root->IP[1]=0;
            root->child[0]->IP[1]=root->child[1]->IP[0];

            root->child[0]->child[1]=root->child[1]->child[0];
            root->child[0]->child[2]=root->child[1]->child[1];
            delete root->child[1];
            root->child[1]=root->child[2];
            root->child[2]=NULL;
        }
    }
    else if(child_nr==1)
    {
        if(root->child[2]!=NULL && root->child[2]->IP[1]!=0)
        {
            root->child[1]->IP[0]=root->IP[1];
            root->IP[1]=root->child[2]->IP[0];
            root->child[2]->IP[0]=root->child[2]->IP[1];
            root->child[2]->IP[1]=0;

            root->child[1]->child[1]=root->child[2]->child[0];
            root->child[2]->child[0]=root->child[2]->child[1];
            root->child[2]->child[1]=root->child[2]->child[2];
            root->child[2]->child[2]=NULL;
        }
        else if(root->child[0]->IP[1]!=0)
        {
            root->child[1]->IP[0]=root->IP[0];
            root->IP[0]=root->child[0]->IP[1];
            root->child[0]->IP[1]=0;

            root->child[1]->child[1]=root->child[1]->child[0];
            root->child[1]->child[0]=root->child[0]->child[2];
            root->child[0]->child[2]=NULL;
        }
        else if(root->child[2]==NULL)
        {
            root->child[0]->IP[1]=root->IP[0];
            root->IP[0]=root->IP[1];
            root->IP[1]=0;

            root->child[0]->child[2]=root->child[1]->child[0];
            delete root->child[1];
            root->child[1]=root->child[2];
            root->child[2]=NULL;
        }
        else
        {
            root->child[1]->IP[0]=root->IP[1];
            root->child[1]->IP[1]=root->child[2]->IP[0];
            root->IP[1]=0;

            root->child[1]->child[1]=root->child[2]->child[0];
            root->child[1]->child[2]=root->child[2]->child[1];
            delete root->child[2];
            root->child[2]=NULL;
        }
    }
    else
    {
        if(root->child[1]->IP[1]!=0)
        {
            root->child[2]->IP[0]=root->IP[1];
            root->IP[1]=root->child[1]->IP[1];
            root->child[1]->IP[1]=0;

            root->child[2]->child[1]=root->child[2]->child[0];
            root->child[2]->child[0]=root->child[1]->child[2];
            root->child[1]->child[2]=NULL;
        }
        else
        {
            root->child[1]->IP[1]=root->IP[1];
            root->IP[1]=0;

            root->child[1]->child[2]=root->child[2]->child[0];
            delete root->child[2];
            root->child[2]=NULL;
        }
    }

    if(root->IP[0]==0)
        root->subtree_size=0;
    else if(root->IP[1]==0)
        root->subtree_size=1;
    else if(root->IP[2]==0)
        root->subtree_size=2;

    for(int i=0; i<=2; i++)
    {
        int tree_size=0;
        if(root->child[i]!=NULL)
        {
            if(root->child[i]->IP[0]==0)
                tree_size=0;
            else if(root->child[i]->IP[1]==0)
                tree_size=1;
            else
                tree_size=2;

            for(int j=0; j<=2; j++)
            {
                if(root->child[i]->child[j]!=NULL)
                    tree_size += root->child[i]->child[j]->subtree_size;
            }
            root->child[i]->subtree_size=tree_size;
        }
        root->subtree_size+=tree_size;
    }
}

bool rmv(long long address, tree* (&root))
{
    tree* temp=NULL;
    bool merge_required=false;
    int tree_size=0;

    if(root->IP[0]!=address && root->IP[1]!=address && root->child[0]==NULL)
        return false;

    if(root->IP[0]==address)
    {
        if(root->child[0]==NULL)
        {
            root->IP[0]=root->IP[1];
            root->IP[1]=0;
            root->subtree_size--;
            if(root->IP[0]==0)
                return true;
            return false;
        }
        else
        {
            temp=root->child[1];
            while(temp->child[0]!=NULL)
                temp=temp->child[0];
            root->IP[0]=temp->IP[0];
            merge_required=rmv(temp->IP[0], root->child[1]);
            if(merge_required)
                merge(root, 1);
        }
    }
    else if(root->IP[1]==address)
    {
        if(root->child[2]==NULL)
        {
            root->IP[1]=0;
            root->subtree_size--;
            return false;
        }
        else
        {
            temp=root->child[2];
            while(temp->child[0]!=NULL)
                temp=temp->child[0];
            root->IP[1]=temp->IP[0];
            merge_required=rmv(temp->IP[0], root->child[2]);
            if(merge_required)
                merge(root, 2);
        }
    }
    else if(root->IP[0]>address && root->child[0]!=NULL)
    {
        merge_required=rmv(address, root->child[0]);
        if(merge_required)
            merge(root, 0);
    }
    else if((root->IP[1]>address || root->IP[1]==0) && root->child[1]!=NULL)
    {
        merge_required=rmv(address, root->child[1]);
        if(merge_required)
            merge(root, 1);
    }
    else if(root->IP[1]<address && root->child[2]!=NULL)
    {
        merge_required=rmv(address, root->child[2]);
        if(merge_required)
            merge(root, 2);
    }

    if(root->IP[0]==0)
        tree_size=0;
    else if(root->IP[1]==0)
        tree_size=1;
    else tree_size=2;

    for(int i=0; i<=2; i++)
        if(root->child[i]!=NULL)
            tree_size += root->child[i]->subtree_size;
    root->subtree_size=tree_size;

    if(root->IP[0]==0)
        return true;
    return false;
}

bool find_IP(unsigned int address, tree* (&root))
{
    if(root->IP[0]==address || root->IP[1]==address)
        return true;

    else if(root->IP[0]>address && root->child[0]!=NULL)
        return find_IP(address, root->child[0]);

    else if((root->IP[1]>address || root->IP[1]==0) && root->child[1]!=NULL)
        return find_IP(address, root->child[1]);

    else if(root->IP[1]<address && root->child[2]!=NULL)
        return find_IP(address, root->child[2]);

    else return false;
}

int subnet_size(unsigned int net_address, tree* (&root))
{
    int range = root->subtree_size;
    tree* pom = root;
    while(pom->child[0] != NULL)
    {
        if(net_address < pom->IP[0])
        {
            pom = pom->child[0];
        }

        else if(net_address < pom->IP[1] || pom->IP[1]==0)
        {
            range -= pom->child[0]->subtree_size;
            range--;
            pom = pom->child[1];
        }
        else if(net_address > pom->IP[1] && pom->IP[1]!=0)
        {
            range -= pom->child[0]->subtree_size;
            range -= pom->child[1]->subtree_size;
            range -= 2;
            pom = pom->child[2];
        }
        else break;
    }
    if(pom->IP[0]<net_address)
        range--;
    if(pom->IP[1]<net_address && pom->IP[1]!=0)
        range--;
    pom=root;
    net_address+=255;
    while(pom->child[0] != NULL)
    {
        if(net_address > pom->IP[1] && pom->IP[1]!=0)
        {
            pom = pom->child[2];
        }

        else if(net_address > pom->IP[0])
        {
            if(pom->child[2]!=NULL)
                range -= pom->child[2]->subtree_size;
            if(pom->IP[1]!=0)
                range--;
            pom = pom->child[1];
        }
        else
        {
            if(pom->child[2]!=NULL)
                range -= pom->child[2]->subtree_size;
            if(pom->IP[1]!=0)
                range-=2;
            else
                range--;
            range-=pom->child[1]->subtree_size;
            pom = pom->child[0];
        }
    }
    if(pom->IP[0]>net_address)
        range--;
    if(pom->IP[1]>net_address)
        range--;
    return range;
}

void print_tree(tree* (&root), int depth=0)
{
    for(int i=1; i<=depth; i++)
        cout<<"\t";
    cout<<IP_to_string(root->IP[0])<<"\t"<<endl;
    if(root->IP[1]!=0)
    {
        for(int i=1; i<=depth; i++)
            cout<<"\t";
        cout<<IP_to_string(root->IP[1])<<endl;
    }
    if(root->child[0]!=NULL)
    {
        print_tree(root->child[0], depth+1);
    }
    else
    {
        for(int i=1; i<=depth+1; i++)
            cout<<"\t";
        cout<<"NULL"<<endl;
    }
    if(root->child[1]!=NULL)
    {
        print_tree(root->child[1], depth+1);
    }
    else
    {
        for(int i=1; i<=depth+1; i++)
            cout<<"\t";
        cout<<"NULL"<<endl;
    }
    if(root->child[2]!=NULL)
    {
        print_tree(root->child[2], depth+1);
    }
    else if(root->IP[1]!=0)
    {
        for(int i=1; i<=depth+1; i++)
            cout<<"\t";
        cout<<"NULL"<<endl;
    }
}

//////////////////////////////////////////////
///              graph part                ///
//////////////////////////////////////////////

vector<pair<unsigned int, long long>> G[16777220];

long long parse_speed(string speed)
{
    long long parsed=0;
    int i=0;
    while(i<speed.size()&&speed[i]<='9'&&speed[i]>='0')
    {
        parsed*=10;
        parsed+=speed[i]-'0';
        i++;
    }
    while(i<speed.size()&&speed[i]!='k'&&speed[i]!='M'&&speed[i]!='G')
        i++;
    if(i<speed.size())
    {
        switch(speed[i])
        {
        case 'k':
            parsed*=1000;
            break;
        case 'M':
            parsed*=1000000;
            break;
        case 'G':
            parsed*=1000000000;
            break;
        }
    }
    return parsed;
}

void add_connection(unsigned int IP1, unsigned int IP2, long long cost)
{
    G[IP1].push_back(make_pair(IP2, cost));
    G[IP2].push_back(make_pair(IP1, cost));
}

void rmv_connection(unsigned int IP1, unsigned int IP2)
{
    bool removed=false;
    for(int i=0; i<G[IP1].size(); i++)
    {
        if(G[IP1][i].first==IP2)
        {
            G[IP1].erase(G[IP1].begin()+i);
            removed=true;
            break;
        }
    }
    if(removed)
    {
        for(int i=0; i<G[IP2].size(); i++)
        {
            if(G[IP2][i].first==IP1)
            {
                G[IP2].erase(G[IP2].begin()+i);
                break;
            }
        }
    }
}

long long find_way(unsigned int IP1, unsigned int IP2)
{
    long long d[16777216];
    priority_queue<pair<long long, unsigned int>, vector<pair<long long, unsigned int>>, greater<pair<long long, unsigned int>>> Q;
    for(unsigned int i=0; i<=16777215; i++)
    {
        d[i]=INF;
    }
    d[IP1] = 0;
    Q.push(make_pair(0, IP1));
    while(Q.top().second!=IP2 && !Q.empty())
    {
        unsigned int v = Q.top().second;
        Q.pop();
        for(unsigned int i=0; i<G[v].size(); i++)
        {
            long long x = d[v]+G[v][i].second;
            if(d[G[v][i].first] > x)
            {
                d[G[v][i].first] = x;
                Q.push(make_pair(x, G[v][i].first));
            }
        }
    }
    return d[IP2];
}

long long find_way_modif(unsigned int IP1, unsigned int IP2, int k)
{
    long long d[16777216][4];
    bool visited[16777216];
    priority_queue<pair<long long, unsigned int>, vector<pair<long long, unsigned int>>, greater<pair<long long, unsigned int>>> Q;
    for(unsigned int i=0; i<=16777215; i++)
    {
        for(int j=0; j<4; j++)
        {
            d[i][j]=INF;
        }
        visited[i]=false;
    }
    for(int j=0; j<4; j++)
    {
        d[IP1][j] = 0;
    }
    Q.push(make_pair(0, IP1));

    //finding way without modifications
    while(!Q.empty())
    {
        unsigned int v = Q.top().second;
        Q.pop();
        if(!visited[v])
        {
            visited[v]=true;
            for(unsigned int i=0; i<G[v].size(); i++)
            {
                long long x = d[v][0]+G[v][i].second;
                if(d[G[v][i].first][0] > x)
                {
                    d[G[v][i].first][0] = x;
                    Q.push(make_pair(x, G[v][i].first));
                }
            }
        }
    }

    //way with 1 or more modifications
    if(k>=1)
    {
        for(unsigned int i=0; i<=16777215; i++)
        {
            visited[i]=false;
        }

        Q.push(make_pair(0, IP1));

        while(!Q.empty())
        {
            unsigned int v = Q.top().second;
            Q.pop();
            if(!visited[v])
            {
                visited[v]=true;
                for(unsigned int i=0; i<G[v].size(); i++)
                {
                    bool changed=false;
                    if(d[v][0]+1<d[G[v][i].first][1])
                    {
                        d[G[v][i].first][1] = d[v][0]+1;
                        changed = true;
                    }
                    if(d[v][1]+G[v][i].second<d[G[v][i].first][1])
                    {
                        d[G[v][i].first][1] = d[v][1]+G[v][i].second;
                        changed = true;
                    }
                    if(changed)
                        Q.push(make_pair(d[v][1], G[v][i].first));
                }
            }
        }
    }

    //way with 2 or more modifications
    if(k>=2)
    {
        for(unsigned int i=0; i<=16777215; i++)
        {
            visited[i]=false;
        }

        Q.push(make_pair(0, IP1));

        while(!Q.empty())
        {
            unsigned int v = Q.top().second;
            Q.pop();
            if(!visited[v])
            {
                visited[v]=true;
                for(unsigned int i=0; i<G[v].size(); i++)
                {
                    bool changed=false;
                    if(d[v][1]+1<d[G[v][i].first][2])
                    {
                        d[G[v][i].first][2] = d[v][1]+1;
                        changed = true;
                    }
                    if(d[v][2]+G[v][i].second<d[G[v][i].first][2])
                    {
                        d[G[v][i].first][2] = d[v][2]+G[v][i].second;
                        changed = true;
                    }
                    if(changed)
                        Q.push(make_pair(d[v][2], G[v][i].first));
                }
            }
        }
    }

    //way with 3 modifications
    if(k==3)
    {
        for(unsigned int i=0; i<=16777215; i++)
        {
            visited[i]=false;
        }

        Q.push(make_pair(0, IP1));

        while(!Q.empty())
        {
            unsigned int v = Q.top().second;
            Q.pop();
            if(!visited[v])
            {
                visited[v]=true;
                for(unsigned int i=0; i<G[v].size(); i++)
                {
                    bool changed=false;
                    if(d[v][2]+1<d[G[v][i].first][3])
                    {
                        d[G[v][i].first][3] = d[v][2]+1;
                        changed = true;
                    }
                    if(d[v][3]+G[v][i].second<d[G[v][i].first][3])
                    {
                        d[G[v][i].first][3] = d[v][3]+G[v][i].second;
                        changed = true;
                    }
                    if(changed)
                        Q.push(make_pair(d[v][3], G[v][i].first));
                }
            }
        }
    }
    if(k<=3)
        return d[IP2][k];
    return INF;
}

//////////////////////////////////////////////
///               main part                ///
//////////////////////////////////////////////

void execute_command(string command, tree* (&root)) //test
{
    if(command.size()==0)
        return;
    string pom = command.substr(0, command.find(" "));
    int x = pom[0] + pom[1];
    long long connection, numIP1, numIP2;
    if(pom.size()>2)
        x+=pom[2];
    string IP="", IP2="", speed="";
    tree* pointer=NULL;
    int k=0;
    switch(x)
    {
    case ('D'+'K'):
        command=command.substr(pom.size()+1, command.size()-pom.size());
        IP = command.substr(0, command.find(" "));
        pointer = add(IP_to_number(IP), root);
        if(pointer!=NULL)
            root = pointer;
        break;
    case ('U'+'K'):
        command=command.substr(pom.size()+1, command.size()-pom.size());
        IP = command.substr(0, command.find(" "));
        rmv(IP_to_number(IP), root);
        if(root->IP[0]==0&&root->child[0]!=NULL)
        {
            pointer=root->child[0];
            delete root;
            root=pointer;
        }
        break;
    case ('W'+'K'):
        command=command.substr(pom.size()+1, command.size()-pom.size());
        IP = command.substr(0, command.find(" "));
        if(find_IP(IP_to_number(IP), root))
            cout<<"TAK"<<endl;
        else cout<<"NIE"<<endl;
        break;
    case ('L'+'K'):
        command=command.substr(pom.size()+1, command.size()-pom.size());
        IP = command.substr(0, command.find(" "));
        IP+=".0";
        cout<<subnet_size(IP_to_number(IP), root)<<endl;
        break;
    case ('W'+'Y'):
        print_tree(root);
        break;
    case ('D'+'P'):
        command=command.substr(pom.size()+1, command.size()-pom.size());
        IP = command.substr(0, command.find(" "));
        command=command.substr(IP.size()+1, command.size()-IP.size());
        IP2 = command.substr(0, command.find(" "));
        command=command.substr(IP2.size()+1, command.size()-IP2.size());
        speed=command;
        add_connection(IP_to_number(IP), IP_to_number(IP2), 100000000000/parse_speed(speed));
        break;
    case ('U'+'P'):
        command=command.substr(pom.size()+1, command.size()-pom.size());
        IP = command.substr(0, command.find(" "));
        command=command.substr(IP.size()+1, command.size()-IP.size());
        IP2 = command.substr(0, command.find(" "));
        rmv_connection(IP_to_number(IP), IP_to_number(IP2));
        break;
    case ('N'+'P'):
        command=command.substr(pom.size()+1, command.size()-pom.size());
        IP = command.substr(0, command.find(" "));
        command=command.substr(IP.size()+1, command.size()-IP.size());
        IP2 = command.substr(0, command.find(" "));
        connection=find_way(IP_to_number(IP)/256, IP_to_number(IP2)/256);
        if(connection!=INF)
            cout<<connection<<endl;
        else cout<<"NIE"<<endl;
        break;
    case ('N'+'P'+'2'):
        command=command.substr(pom.size()+1, command.size()-pom.size());
        IP = command.substr(0, command.find(" "));
        command=command.substr(IP.size()+1, command.size()-IP.size());
        IP2 = command.substr(0, command.find(" "));
        command=command.substr(IP2.size()+1, command.size()-IP2.size());
        k = command[0]-'0';
        connection=find_way_modif(IP_to_number(IP)/256, IP_to_number(IP2)/256, k);
        if(connection!=INF&&connection!=-1)
            cout<<connection<<endl;
        else cout<<"NIE"<<endl;
        break;
    default:
        break;
    }
}

int main()
{
    ifstream file;
    file.open("projekt3_inX.txt");
    tree* root = new tree;
    string s;
    getline(file, s);
    if(s.find(" ")!= string::npos)
        execute_command(s, root);
    while(!file.eof())
    {
        getline(file, s);
        execute_command(s, root);
    }
    for(;;)
    {
        getline(cin, s);
        execute_command(s, root);
    }
}
