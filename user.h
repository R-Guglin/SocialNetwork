#ifndef USER_H
#define USER_H
#include "post.h"
#include <vector>
#include <string>
#include <set>

class User
{
public:

    //pre: none
    //post: a new default User has been created
    User();

    //pre: none
    //post: a new User has been created with the given data
    User(int id, const std::string & name, int year, int zip, std::set<int> friends);

    ~User();

    //pre: the given id is NOT in friends_
    //post: the id has been added to friends_
    void addFriend(int id);

    //pre: the given id IS in friends_
    //post: the id has been removed from friends_
    void deleteFriend(int id);

    //pre: none
    //post: returns the ID number of this User
    int getId() const { return id_; }

    //pre: none
    //post: returns the name of this User
    std::string getName() const { return name_; }

    //pre: none
    //post: returns the birth year of this User
    int getYear() const { return year_; }

    //pre: none
    //post: returns the ZIP code of this User
    int getZip() const { return zip_; }

    //pre: none
    //post: returns a reference to the set of friends of this User
    std::set<int>& getFriends() { return friends_; }

    //pre: receives a valid pointer to a Post
    //post: messages_ now contains that pointer
    void addPost(Post*);

    //pre: none
    //post: returns a copy of the user's wall
    std::vector<Post*> getPosts() const { return messages_; }

    //pre: none
    //post: returns a string that holds the most recent howMany posts (or all posts, if there are less than howMany)
    std::string getPostsString(int howMany, bool showOnlyPublic);

    //pre: none
    //post: returns a vector that holds the most recent howMany posts (or all posts, if there are less than howMany)
    //      and does not include comments.
    std::vector<Post*> getPostsVector(int howMany, bool showOnlyPublic);

private:

    int id_;

    std::string name_;

    int year_;

    int zip_;

    std::set<int> friends_;

    std::vector<Post*> messages_;

};

#endif // USER_H
