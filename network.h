#ifndef NETWORK_H
#define NETWORK_H

#include "user.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>

class Network
{
public:
    //pre: none
    //post: a new Network is created with an empty set of users
    Network();

    //pre: none
    //post: releases any dynamically-allocated users from memory
    ~Network();

    //pre: the User is not in the network
    //post: the User has been added to the network
    void addUser(User*);

    //pre: s1 and s2 are the names of two different, valid Users
    //post: if s1 and s2 are not already friends,
    //      they are added to each other's friend lists.
    //      If they are already friends, nothing happens.
    //      If either user is invalid, returns -1
    int addConnection(const std::string & s1, const std::string & s2);

    //pre: s1 and s2 are the names of two different, valid Users
    //post: if s1 and s2 are already friends,
    //      they are removed from each other's friend lists.
    //      If they are not already friends, nothing happens.
    //      If either user is invalid, returns -1
    int deleteConnection(const std::string & s1, const std::string & s2);

    //pre: none
    //post: returns the user with this ID, or nullptr if the ID is invalid
    User* getUser(int id) const;

    //pre: none
    //post: prints the usernames of all users in the network
    void printAllUsers() const;

    //pre: none
    //post: returns the ID of the user with that name
    //      if the user is not in the network, returns -1
    int getId(const std::string & name) const;

    //pre: none
    //post: returns the number of Users in the network
    int numUsers() const;

    //pre: fname is a valid file in the correct format
    //post: new Users are created with the information in the file
    //      and added to users_
    int readUsers(const char* fname);

    //pre: fname is a valid file
    //post: info on all the users in the network
    //      is written into fname
    int writeUsers(const char* fname) const;

    //pre: from and to are valid user IDs
    //post: returns a list of user IDs in the shortest past between from and to
    std::vector<int> shortestPath(int from, int to) const;

    //pre: from is a valid user ID
    //post: returns the first user ID that is the specified distance away
    std::vector<int> distanceUser(int from, int& to, int distance) const;

    //pre: "who" is a valid user ID
    //post: returns a list of users who are at a distance of 2 from "who", as well as the number of friends they have in common.
    std::vector<int> suggestFriends(int who, int& score) const;

    //pre: none
    //post: all connected components in the network have been found and stored
    std::vector<std::vector<int> > groups() const;

    //pre: ownerId is a valid User ID
    //post: adds the new post to the messages vector of the user whose id is ownerId
    void addPost(int ownerId, std::string message, int likes, int type,
                 std::string authorName = "", int parentId = 0);

    //pre: ownerId is a valid user ID
    //post: returns the given user's post string
    std::vector<Post*> getPostsVector(int ownerId, int howMany, bool showOnlyPublic)
    {
        User * u = getUser(ownerId);
        if(u != nullptr)
            return u->getPostsVector(howMany, showOnlyPublic);
        else
            return {nullptr};
    }

    //pre: fname is a valid file name and format
    //post: the posts described in that file have been created and assigned to this network's Users
    int readPosts(const char* fname);

    //pre: none
    //post: all posts in this network are stored in this file
    int writePosts(char* fname);

    //new methods

    //pre: none
    //post: returns a vector of all comments on this Post
    std::vector<Post*> getComments(Post* parent) const;

    //pre: valid message ID
    //post: returns a pointer to the post with this ID
    Post* getPost(int messageId) const;

    //pre: both are valid IDs
    //post: returns true if both users are friends
    bool areFriends(User* user1, User* user2) const;

private:

    std::vector<User*> users_;

    int getPostCount()
    {
        int postCount = 0;
        for(User* u : users_)
            postCount += u->getPosts().size();
        return postCount;
    }

};

#endif //NETWORK_H
