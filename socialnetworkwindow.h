#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include <QMainWindow>
#include "network.h"
#include "post.h"
#include "user.h"
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class SocialNetworkWindow;
}
QT_END_NAMESPACE


class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

    //Functions triggered by buttons, most of which call display() after modifying fields
    void checkLogin();

    //Table cell clicks

    //pre: column, row in range of the table whose signal called this function
    //post: goes to the profile of the friend whose cell was clicked on
    void friendClicked(int row, int column);

    //pre: column, row in range of the table whose signal called this function
    //post: if column 0, goes to the profile of this cell; if column = 1, adds them as a friend
    void suggestionClicked(int row, int column);

    //pre: column, row in range of the table whose signal called this function
    //post: if column 0, expands the post in this cell; if column = 1, adds a like to the post
    void postClicked(int row, int column);

    //pre: none
    //post: goes back to the user's own page
    void backClicked();

    //pre: none
    //post: escapes out of an expanded post
    void postBackClicked();

    //pre: none
    //post: displays the profile of the previous user
    void backToPrevClicked();

    //pre: none
    //post: adds currentUser_ as a friend if not already friends; updates USERS_WRITE_FILE
    void addFriendClicked();

    //pre: none
    //post: removes currentUser_ as a friend if not already friends; updates USERS_WRITE_FILE
    void removeFriendClicked();

    //pre: none
    //post: resets current and loggedInUser_ to nullptr
    void logout();

    //pre: none
    //post: tries to find a valid User corresponding to text input; if successful, displays their page
    void searchAndGo();

    //pre: none
    //post: displays the account creation panel
    void createAccount();

    //pre: none
    //post: tries to create a new User if the name given is not already in network
    void checkAccount();


    //pre: none
    //post: creates a new comment and updates POSTS_WRITE_FILE
    void comment();

    //pre: none
    //post: creates a new private or public post and updates POSTS_WRITE_FILE
    void addPost();

    //pre: none
    //post: helper function to update currentUser_ AND prevUser_ at once
    void updateCurrent(User* u);

    //pre: none
    //post: godly overlord that handles all visual changes--hiding/unhiding and populating displays
    void display();

private:
    Ui::SocialNetworkWindow *ui;
    Network* net_ = new Network();

    User* loggedInUser_ = nullptr;
    User* currentUser_ = nullptr;
    User* prevUser_ = nullptr;

    std::vector<Post*> currentPosts_;
    Post* expandedPost_ = nullptr;

};
#endif // SOCIALNETWORKWINDOW_H
