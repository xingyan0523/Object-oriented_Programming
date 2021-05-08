#include<iostream>
#include<vector>
#include<queue>
#include<cmath>

using namespace std;

double dist(double x1, double x2, double y1, double y2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

class packet{
    public:
        int src, des;
        int last, next;

        packet(int _src = 0, int _des = 0, int _last = 0, int _next = 0){
            src = _src;
            des = _des;
            last = _last;
            next = _next;
        }
        
        /*void printf_info(){
            cout<<endl<<"-----------------"<<endl;;
            cout<<"src: "<<src<<"  des: "<<des<<endl;
            cout<<"last: "<<last<<"  next: "<<next<<endl;
            cout<<"-----------------"<<endl;;
        }*/
};

class node{
    public:
        int nodeID;
        double x, y;
        queue<packet> q;
        vector<node*> neighbors;

        bool checkQueue(vector<node> &net){
            if(!q.empty()){
                if(q.front().next == nodeID){
                    q.front().last = nodeID;
                    q.front().next = getNextHop(net, q.front().des);
                    if(q.front().next >= 0){
                        cout<<q.front().next<<" ";
                        send(net);
                    }
                }
                q.pop();
                    
                return true;
            }
            return false;
        }

        int getNextHop(vector<node> &net, int des){
            double min = dist(x, net[des].x, y, net[des].y);
            int min_pos = -1;
            for(unsigned int i = 0; i < neighbors.size(); i++){
                if(dist(neighbors[i]->x, net[des].x, neighbors[i]->y, net[des].y) < min){
                    min = dist(neighbors[i]->x, net[des].x, neighbors[i]->y, net[des].y);
                    min_pos = i;
                }
            }
            return min_pos >= 0 ? neighbors[min_pos]->nodeID : -1;
        }

        void send(vector<node> &net){
            for(unsigned int i = 0; i < neighbors.size(); i++)
                net[neighbors[i]->nodeID].q.push(q.front());
        }

        /*void print_info(){
            cout<<"ID: "<<nodeID<<endl;
            cout<<x<<" "<<y<<endl;
            cout<<"neighbors: ";
            for(int i = 0; i < neighbors.size(); i++){
                cout<<neighbors[i].nodeID<<" ";
            }
            cout<<endl;
        }*/
};

int main(){
    int numsofnode, pairs;
    cin>>numsofnode;
    vector<node> net(numsofnode);

    for(int i = 0; i < numsofnode; i++){
        cin>>net[i].nodeID;
        cin>>net[i].x>>net[i].y;
        
        for(int j = 0; j < i; j++){
            if(dist(net[i].x, net[j].x, net[i].y, net[j].y) <= 1){
                net[i].neighbors.push_back(&net[j]);
                net[j].neighbors.push_back(&net[i]);
            }
        }
    }

    cin>>pairs;
    cout<<pairs<<endl;

    for(int i = 0; i < pairs; i++){
        int src, des;

        cin>>src>>des;
        cout<<src<<" ";

        packet p(src, des, src, src);
        net[src].q.push(p);

        bool isempty = false;
        while(!isempty){
            isempty = true;
            for(int j = 0; j < numsofnode; j++)
                if(net[j].checkQueue(net))
                    isempty = false;
        }
        cout<<endl;
    }
    return 0;
}