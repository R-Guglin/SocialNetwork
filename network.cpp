#include "network.h"

Network::Network()
{
    users_ = {};
}

Network::~Network()
{
    for(auto i = 0; i < users_.size(); ++i)
    {
        if(users_[i] != nullptr)
            delete users_[i];
    }
}

void Network::addUser(User* user)
{
    auto it = std::find(users_.begin(), users_.end(), user);
    if(it == users_.end())
        users_.push_back(user);
}

int Network::addConnection(const std::string & s1, const std::string & s2)
{
    int id1 = getId(s1);
    int id2 = getId(s2);

    if((id1 == -1) || (id2 == -1) || (id1 == id2))
        return -1;

    users_[id1]->addFriend(id2);
    users_[id2]->addFriend(id1);

    return 0;
}

int Network::deleteConnection(const std::string & s1, const std::string & s2)
{
    int id1 = getId(s1);
    int id2 = getId(s2);

    if((id1 == -1) || (id2 == -1) || (id1 == id2))
        return -1;

    users_[id1]->deleteFriend(id2);
    users_[id2]->deleteFriend(id1);

    return 0;
}

User* Network::getUser(int id) const
{
    if(id < 0 || id >= users_.size())
        return nullptr;
    return users_[id];
}

void Network::printAllUsers() const
{
    int n = users_.size();
    for(int i = 0; i < n; ++i)
        std::cout << users_[i]->getId() << " " << users_[i]->getName() << std::endl;
}

int Network::getId(const std::string & name) const
{
    int n = users_.size();
    for(int i = 0; i < n; ++i)
    {
        User* u = users_[i];
        assert(u != nullptr);
        if(u->getName() == name)
        {
            return u->getId();
        }
    }
    return -1;
}

int Network::numUsers() const
{
    return users_.size();
}

int Network::readUsers(const char* fname)
{
    std::ifstream myfile(fname);

    if(myfile.fail())
        return -1;

    std::string myline;

    int n;
    getline(myfile, myline);
    std::stringstream sstr(myline);
    sstr >> n;
    
    int i = 0;
    while(i < n)
    {
        int id = 0;
        std::string name = "";
        int year = 0;
        int zip = 0;
        std::set<int> friends = {};

        for(int j = 0; j < 5; ++j)
        {
            getline(myfile, myline);
            std::stringstream sstr(myline);
            if(j==0)
                sstr >> id;
            else if(j==1)
            {
                std::string fname; sstr >> fname;
                std::string lname; sstr >> lname;
                name.append(fname); name.append(" "); name.append(lname);
            }
            else if(j==2)
                sstr >> year;
            else if(j==3)
                sstr >> zip;
            else if(j==4)
            {
                int friendId;
                while(sstr >> friendId)
                    friends.insert(friendId);
            }
        }
        User* user = new User(id, name, year, zip, friends);
        addUser(user);
        ++i;
    }
    return 0;
}

int Network::writeUsers(const char* fname) const
{
    int n = numUsers();
    std::ofstream myfile(fname);

    if(myfile.fail())
        return -1;

    myfile << n << "\n";
    for(int i = 0; i < n; ++i)
    {
        User* user = users_[i];
        std::set<int> friends = user->getFriends();
        assert(i == user->getId());
        myfile << user->getId() << "\n\t";
        myfile << user->getName() << "\n\t";
        myfile << user->getYear() << "\n\t";
        myfile << user->getZip() << "\n\t";
        for(auto it = friends.begin(); it != friends.end(); ++it)
        {
            myfile << *it << " ";
        }
        myfile << "\n";
    }
    return 0;
}

std::vector<int> Network::shortestPath(int from, int to) const
{
    assert(from < numUsers() && to < numUsers());
    std::queue<int> q;
    std::vector<bool> visited(numUsers(), false);
    std::vector<int> prev(numUsers(), -1);
    bool foundTo = false;
    
    visited[from] = true;
    q.push(from);

    std::vector<int> output;

    while(!q.empty())
    {
        int curr = q.front();
        if(curr == to)
        {
            foundTo = true;
            break;
        }
        q.pop();
        for(int neighbor : getUser(curr)->getFriends())
        {
            if(!visited[neighbor])
            {   
                prev[neighbor] = curr;
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    if(foundTo)
    {
        int curr = to;
        while(curr != from)
        {
            output.push_back(curr);
            curr = prev[curr];
        }
        output.push_back(from);
    }
    else
        output.push_back(-1);

    //Put the path in order to make it easier for main to handle
    std::reverse(output.begin(), output.end());
    return output;
}

std::vector<int> Network::distanceUser(int from, int& to, int distance) const
{
    int n = numUsers();
    assert(from < n);
    std::vector<int> output;
    for(int i = 0; i < n; ++i)
    {
        to = i;
        output = shortestPath(from, to);
        if(output.size() == distance + 1)
        {
            return output;
        }
    }
    output.clear();
    to = -1;
    return output;
}

std::vector<int> Network::suggestFriends(int who, int& score) const
{
    int n = numUsers();
    assert(who < n);

    int maxScore = 0;
    std::vector<int> output;

    for(int i = 0; i < n; ++i)
    {
        std::vector<int> path = shortestPath(who, i);
        if(path.size() == 3)
        {
            int s = 0;
            const auto friends1 = getUser(who)->getFriends();
            const auto friends2 = getUser(i)->getFriends();

            for(auto it = friends1.begin(); it != friends1.end(); ++it)
            {
                int id = *it;
                if(friends2.find(id) != friends2.end())
                    ++s;
            }
            if(s > maxScore)
            {
                output.clear();
                output.push_back(i);
                maxScore = s;
            }
            else if(s == maxScore)
                output.push_back(i);
        }
    }
    score = maxScore;
    return output;
}

std::vector<std::vector<int> > Network::groups() const
{
    std::vector<std::vector<int> > groups;
    std::vector<bool> visited(numUsers(), false);
    
    for(User* u : users_)
    {
        int source = getId(u->getName());
        if(!visited[source])
        {
            std::vector<int> group;
            group.push_back(source);

            //DFS algorithm
            std::stack<int> stck;
            visited[source] = true;
            stck.push(source);

            while(stck.size() > 0)
            {
                User* cur = getUser(stck.top());
                assert(cur != nullptr);

                stck.pop();
                for(int neighbor : cur->getFriends())
                {
                    if(!visited[neighbor])
                    {
                        visited[neighbor] = true;
                        stck.push(neighbor);
                        group.push_back(neighbor);
                    }
                }
            }
            groups.push_back(group);
        }
    }
    return groups;
}


void Network::addPost(int ownerId, std::string message, int likes, int type,
                      std::string authorName, int parentId)
{
    User* u = getUser(ownerId);
    assert(u != nullptr);
    int messageId = getPostCount();
    Post* p = nullptr;
    switch (type)
    {
    case 1: //regular Post
        p = new Post(messageId, ownerId, message, likes);
        break;

    case 2: //public IncomingPost
        p = new IncomingPost(messageId, ownerId, message, likes, true, authorName);
        break;

    case 3: //private IncomingPost
        p = new IncomingPost(messageId, ownerId, message, likes, false, authorName);
        break;

    case 4: //Comment
        {
        p = new Comment(messageId, ownerId, message, likes, authorName, parentId);
        Post* parent = getPost(parentId);
        if(parent != nullptr)
            parent->addComment(messageId);
        
        break;
        }

    default:
        break;
    }

    if(p != nullptr)
        u->addPost(p);
}

std::string readName(std::stringstream& sstr)
{
    std::string fname; sstr >> fname; std::string lname; sstr >> lname;
    return fname + " " + lname;
}


int Network::readPosts(const char* fname)
{
    std::ifstream myfile(fname);

    if(myfile.fail())
        return -1;

    std::string myline;

    int n;
    getline(myfile, myline);
    std::stringstream sstr(myline);
    sstr >> n;

    int i = 0;
    //Iterate through the n Posts in the file
    while(i < n)
    {
        int messageId = 0;
        std::string message = "";
        int ownerId = 0;
        int likes = 0;
        int type = 0;
        std::string author = "";
        int parentId = 0;
        std::set<int> likers = {};

        for(int j = 0; j < 8; ++j)
        {
            getline(myfile, myline);
            std::stringstream sstr(myline);

            switch (j)
            {
            case 0:
                sstr >> messageId;
                break;

            case 1:
            {
                std::string line;
                while(sstr >> line)
                    message = message + line + " ";
                break;
            }

            case 2:
                sstr >> ownerId;
                break;

            case 3:
                sstr >> likes;
                break;

            case 4:
                sstr >> type;
                break;

            case 5:
                if(type != 1)
                    author = readName(sstr);
                break;

            case 6:
                sstr >> parentId;
                break;

            case 7:
                int likerId;
                while(sstr >> likerId)
                    likers.insert(likerId);

            default:
                break;
            }
        }

        if(type == 1) //regular Post
            addPost(ownerId, message, likes, 1);
        else if(type == 2) //public IPost
            addPost(ownerId, message, likes, 2, author);
        else if(type == 3) //private IPost
            addPost(ownerId, message, likes, 3, author);
        else if(type == 4) //Comment
            addPost(ownerId, message, likes, 4, author, parentId);

        ++i;
    }
    return 0;
}

bool comparePosts(Post* p1, Post* p2)
{
    return (p1->getMessageId() < p2->getMessageId());
}

int Network::writePosts(char* fname)
{
    std::vector<Post*> posts;
    for(User* u : users_)
        for(Post* p : u->getPosts())
            posts.push_back(p);

    std::sort(posts.begin(), posts.end(), comparePosts);
    int n = posts.size();

    std::ofstream myfile(fname);
    if(myfile.fail())
        return -1;

    myfile << n << "\n";
    for(int i = 0; i < n; ++i)
    {
        Post* post = posts[i];
        int type = post->type();

        myfile << post->getMessageId() << "\n\t";
        myfile << post->getMessage() << "\n\t";
        myfile << post->getOwnerId() << "\n\t";
        myfile << post->getLikes() << "\n\t";

        switch (type)
        {
        case 1: //regular Post
            myfile << "1\n\n\n";
            break;

        case 2: //public IncomingPost
            myfile << "2\n\t";
            myfile << post->getAuthor() << "\n\n";
            break;

        case 3: //private IncomingPost
            myfile << "3\n\t";
            myfile << post->getAuthor() << "\n\n";
            break;

        case 4: //Comment
            myfile << "4\n\t" << post->getAuthor() << "\n\t";
            myfile << post->getParentId() << "\n";
            break;

        default:
            break;
        }
        myfile << "\t";
        std::set<int> likers = post->getLikers();
        for(auto it = likers.begin(); it != likers.end(); ++it)
            myfile << *it << " ";
        myfile << "\n";
    }
    return 0;
}

std::vector<Post*> Network::getComments(Post* parent) const
{
    std::vector<Post*> output;
    if(parent != nullptr)
    {
        for(int i : parent->getComments())
        {
            Post* c = getPost(i);
            if(c != nullptr)
                output.push_back(c);
        }
    }
    return output;
}

Post* Network::getPost(int messageId) const
{
    for(User* u: users_)
    {
        for(Post* p : u->getPosts())
        {
            if(p->getMessageId() == messageId)
                return p;
        }
    }
    return nullptr;
}

bool Network::areFriends(User* user1, User* user2) const
{
    int id2 = user2->getId();
    std::set<int> friends = user1->getFriends();
    return (std::find(friends.begin(), friends.end(), id2) != friends.end());
}

