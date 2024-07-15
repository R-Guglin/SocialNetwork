#ifndef POST_H
#define POST_H

#include <iostream>
#include <string>
#include <set>

class Post
{
public:

    //pre: none
    //post: new default Post created
    Post();

    //pre: none
    //post: new Post created with the specified data
    Post(int messageId, int ownerId, std::string message, int likes);

    virtual ~Post() {}

    //pre: none
    //post: returns "[message_] Liked by [likes_] people."
    virtual std::string toString() const;

    //pre: none
    //post: returns the ID of this Post's message
    int getMessageId() const { return messageId_; }

    //pre: none
    //post: returns the ID of this Post's owner
    int getOwnerId() const { return ownerId_; }

    //pre: none
    //post: returns this Post's message
    std::string getMessage() const { return message_; }

    //pre: none
    //post: returns the number of likes this Post has
    int getLikes() const { return likes_; }

    //pre: none
    //post: returns an empty string
    virtual std::string getAuthor() const { return ""; }

    //pre: none
    //post: returns true
    virtual bool getIsPublic() const { return true; }

    //new methods:
    //pre: none
    //post: returns the post's type (1-Post, 2-public IPost, 3-private IPost, 4-comment)
    virtual int type() const { return 1; }

    //pre: none
    //post: if comment, returns parent ID
    virtual int getParentId() { return 0; }


    //pre: none
    //post: returns the set of message IDs of this post's comments
    std::set<int> getComments() const { return comments_; }

    //pre: none
    //post: stores the ID of someone who has liked this post in likers_
    void addLiker(int likerId){ likers_.insert(likerId); }

    //pre: none
    //post: updates likes_
    void setLikes(int newCount) { likes_ = newCount; }

    //pre: none
    //post: adds that comment's ID to comments_
    bool addComment(int messageId);

    //pre: none
    //post: returns all IDs of people who have liked this Post
    std::set<int> getLikers() { return likers_; }

private:
    int messageId_;
    int ownerId_;
    std::string message_;
    int likes_;
    std::set<int> comments_;
    std::set<int> likers_ = {};
};

class IncomingPost: public Post
{
public:
    //pre: none
    //post: a new default Post is created
    IncomingPost();

    //pre: none
    //post: a new Post is created with the given data
    IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author);

    //pre: none
    //post: returns the author's name
    std::string getAuthor() const { return author_; }

    //pre: none
    //post: returns the publicity of this post
    bool getIsPublic() const { return isPublic_; }

    //pre: none
    /*post: returns "[author_] wrote[private]: [toString]" where [author_] is the value
    of author_, [toString] is the value obtained by calling the Post toString method, and
    [private] is the empty string if isPublic_ is true, and " (private)" if isPublic_ is false.*/
    std::string toString() const;

    int type() const { return (isPublic_ == true ? 2 : 3); }

private:

    bool isPublic_;
    std::string author_;
};

class Comment: public IncomingPost
{
public:
    Comment() : IncomingPost() { parentId_ = 0; }

    Comment(int messageId, int ownerId, std::string message, int likes, std::string author, int parentId)
        : IncomingPost(messageId, ownerId, message, likes, true, author)
    {
        parentId_ = parentId;
    }

    int getParentId() { return parentId_; }

    //pre: none
    //post: returns author_ + "commented" + post text
    std::string toString() const;

    int type() const { return 4; }

private:
    int parentId_;
};


#endif //POST_H
