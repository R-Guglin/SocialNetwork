#include "user.h"
#include "network.h"

#include <iostream>
#include <sstream>
#include <set>

using namespace std;

string readName()
{
    string fname; cin >> fname;
    string lname; cin >> lname;
    string name; name.append(fname); name.append(" "); name.append(lname);
    return name;
}

int main(int argc, char *argv[]) {

    char* fname = argv[1];
    int option = 1;
    Network* net = new Network();

    net->readUsers(fname);

    while(option >= 1 && option <= 11)
    {
        cout<<"Choose an option: ";
        cin>>option;

        if(option == 1)
        {
            int id = net->numUsers();
            string name = readName();
            int year; cin >> year;
            int zip; cin >> zip;
            set<int> friends = {};

            User* newUser = new User(id, name, year, zip, friends);
            net->addUser(newUser);
        }
        else if(option == 2)
        {
            string name1 = readName();
            string name2 = readName();

            int r = net->addConnection(name1, name2);
            if(r == -1)
                cout<<"Invalid user connection\n";
        }
        else if(option == 3)
        {
            string name1 = readName();
            string name2 = readName();

            int r = net->deleteConnection(name1, name2);
            if(r == -1)
                cout<<"Invalid user connection\n";
        }
        else if(option == 4)
        {
            net->printAllUsers();
        }
        else if(option == 5)
        {
            string name = readName();
            int id = net->getId(name);
            if(id == -1)
                cout<<"Invalid user\n";
            else
            {
                User* u = net->getUser(id);
                assert(u != nullptr);
                set<int> friends = u->getFriends();
                for(auto it = friends.begin(); it != friends.end(); ++it)
                {
                    int id = *it;
                    cout<<id<<" ";
                    User* f = net->getUser(id);
                    assert(f != nullptr);
                    cout<<f->getName()<<endl;
                }
            }
        }
        else if(option == 6)
        {
            char fname[128];
            cin >> fname;
            net->writeUsers(fname);
        }
        else if(option == 7)
        {
            string name1 = readName();
            string name2 = readName();

            int id1 = net->getId(name1);
            int id2 = net->getId(name2);

            if(id1 == -1 || id2 == -1)
                cout<<"Invalid user(s)\n";

            else    
            {
                vector<int> path = net->shortestPath(id1, id2);
                if(path[0] == -1)
                {
                    cout << "None\n";
                }
                else
                {   
                    int n = path.size();
                    cout<<"Distance: "<<(n-1)<<endl;
                    for(int i = 0; i < n; ++i)
                    {
                        string name = net->getUser(path[i])->getName();
                        if(i < (n-1))
                            cout<<name<<" -> ";
                        else
                            cout<<name<<endl;
                    }
                }
            }
        }
        else if(option == 8)
        {
            string name = readName();
            int from = net->getId(name);
            int distance; cin >> distance;
            int to = -1;
            vector<int> path = net->distanceUser(from, to, distance);
            int n = path.size();
            if(to >= 0)
            {
                cout<<net->getUser(to)->getName()<<": ";
                for(int i = 0; i < n; ++i)
                {
                    string name = net->getUser(path[i])->getName();
                    if(i < (n-1))
                        cout<<name<<" -> ";
                    else
                        cout<<name<<endl;
                }
            }
            else
                cout<<"No user found\n";
        }
        else if(option == 9)
        {
            string name = readName();
            int id = net->getId(name);
            int score = 0;
            vector<int> suggestions = net->suggestFriends(id, score);
            if(score == 0)
                cout<<"No suggestions found\n";
            else
            {
                cout<<"The suggested friend(s) is/are:\n";
                for(int i : suggestions)
                {
                    string name = net->getUser(i)->getName();
                    cout<<name<<" — Score: "<<score<<endl;
                }
            }
        }
        else if(option == 10)
        {
            vector<vector<int> > groups = net->groups();
            for(int i = 0; i < groups.size(); ++i)
            {
                cout<<"Set "<<(i+1)<<" => ";
                vector<int> group = groups[i];
                for(int i = 0; i < group.size(); ++i)
                {
                    if(i < (group.size() - 1))
                        cout<<net->getUser(group[i])->getName()<<", ";
                    else
                        cout<<net->getUser(group[i])->getName()<<"\n";
                }
            }
        }
        else if(option == 11)
        {
            string name = readName();
            int howMany; cin >> howMany;
            User* u = net->getUser(net->getId(name));
            cout<<u->getPostsString(howMany, false);
        }
    }
    delete net;
}