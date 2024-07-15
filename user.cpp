#include "user.h"
#include <cassert>

User::User()
{
    id_ = 0;
    name_ = "";
    year_ = 0;
    zip_ = 0;
    friends_ = {};
}

User::User(int id, const std::string & name, int year, int zip, std::set<int> friends)
{
    id_ = id;
    name_ = name;
    year_ = year;
    zip_ = zip;
    friends_ = friends;
}

User::~User()
{
    for(Post* p: messages_)
    {
        assert(p != nullptr);
        delete p;
    }
}

void User::addFriend(int id)
{
    //iterate through friends_ to make sure this ID isn't in it
    if(friends_.find(id) == friends_.end())
    {
        friends_.insert(id);
    }
}

void User::deleteFriend(int id)
{
    //iterate through friends_ to make sure this ID is in it
    if(friends_.find(id) != friends_.end())
    {
        friends_.erase(id);
    }
}

void User::addPost(Post* post)
{
    assert(post != nullptr);
    messages_.push_back(post);
}


std::string User::getPostsString(int howMany, bool showOnlyPublic)
{
    int n = messages_.size();
    std::string output;
    for(int i = n - 1; i >= std::max(0, (n - howMany)); --i)
    {
        Post* post = messages_[i];
        if(showOnlyPublic && !post->getIsPublic())
            continue;
        else
        {
            output += post->toString();
            output += "\n\n";
        }
    }
    return output;
}

std::vector<Post*> User::getPostsVector(int howMany, bool showOnlyPublic)
{
    int n = messages_.size();
    std::vector<Post*> output;
    for(int i = n - 1; i >= std::max(0, (n - howMany)); --i)
    {
        Post* post = messages_[i];
        if((showOnlyPublic && !post->getIsPublic()) || post->type() == 4)
            howMany += 1;
        else
            output.push_back(post);
    }
    return output;
}
