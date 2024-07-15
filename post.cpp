#include "post.h"
#include <sstream>

Post::Post()
{
    messageId_ = 0;
    ownerId_ = 0;
    message_ = "";
    likes_ = 0;
    comments_ = {};
}

Post::Post(int messageId, int ownerId, std::string message, int likes)
{
    messageId_ = messageId;
    ownerId_ = ownerId;
    message_ = message;
    likes_ = likes;
    comments_ = {};
}

std::string Post::toString() const
{
    return message_;
}

IncomingPost::IncomingPost()
    : Post()
{
    isPublic_ = false;
    author_ = "";
}

IncomingPost::IncomingPost(int messageId, int ownerId, std::string message, int likes,
                           bool isPublic, std::string author)
    : Post(messageId, ownerId, message, likes)
{
    isPublic_ = isPublic;
    author_ = author;
}

std::string IncomingPost::toString() const
{
    std::string s = author_;
    s += " wrote";
    if(!isPublic_)
        s += " (private)";
    s += ": ";
    s += Post::toString();
    return s;
}

std::string Comment::toString() const
{
    std::string s = IncomingPost::getAuthor();
    s += " commented: ";
    s += Post::toString();
    return s;
}

bool Post::addComment(int messageId)
{
    if(messageId > 0)
    {
        comments_.insert(messageId);
        return true;
    }
    else
        return false;
}
