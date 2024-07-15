#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"
#include <sstream>

//Constants
char USERS_READ_FILE[128] = "users2.txt";
char USERS_WRITE_FILE[128] = "users2.txt";
char POSTS_READ_FILE[128] = "posts2.txt";
char POSTS_WRITE_FILE[128] = "posts2.txt";
int MAX_POSTS_DISPLAYED = 10;
int MAX_COMMENTS_DISPLAYED = 10;

SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);
    display();

    net_->readUsers(USERS_READ_FILE);
    net_->readPosts(POSTS_READ_FILE);

    ui->friendsTable->setColumnWidth(0, 198);
    ui->suggestionsTable->setColumnWidth(0, 148);
    ui->suggestionsTable->setColumnWidth(1, 100);
    ui->postsTable->setColumnWidth(0, 290);
    ui->postsTable->setColumnWidth(1, 40);
    ui->commentsTable->setColumnWidth(0, 650);

    connect(ui->loginButton, &QPushButton::clicked, this, &SocialNetworkWindow::checkLogin);
    connect(ui->logout, &QPushButton::clicked, this, &SocialNetworkWindow::logout);

    connect(ui->friendsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::friendClicked);
    connect(ui->suggestionsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::suggestionClicked);
    connect(ui->postsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::postClicked);

    connect(ui->backToProfile, &QPushButton::clicked, this, &SocialNetworkWindow::backClicked);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &SocialNetworkWindow::addFriendClicked);
    connect(ui->removeFriendButton, &QPushButton::clicked, this, &SocialNetworkWindow::removeFriendClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &SocialNetworkWindow::searchAndGo);
    connect(ui->createAccButton, &QPushButton::clicked, this, &SocialNetworkWindow::checkAccount);
    connect(ui->createAcc, &QPushButton::clicked, this, &SocialNetworkWindow::createAccount);
    connect(ui->postBackButton, &QPushButton::clicked, this, &SocialNetworkWindow::postBackClicked);
    connect(ui->commentButton, &QPushButton::clicked, this, &SocialNetworkWindow::comment);
    connect(ui->addPostButton, &QPushButton::clicked, this, &SocialNetworkWindow::addPost);
    connect(ui->backToPrev, &QPushButton::clicked, this, &SocialNetworkWindow::backToPrevClicked);
}

SocialNetworkWindow::~SocialNetworkWindow()
{
    delete net_;
    delete ui;
}

void SocialNetworkWindow::logout()
{
    loggedInUser_ = nullptr;
    currentUser_ = nullptr;
    display();
}


void SocialNetworkWindow::searchAndGo()
{
    std::string attemptedName = ui->searchEdit->toPlainText().toStdString();
    User* attemptedUser = net_->getUser(net_->getId(attemptedName));
    if(attemptedUser != nullptr)
    {
        updateCurrent(attemptedUser);
        ui->searchEdit->clear();
        ui->searchError->clear();
        display();
    }
    else
    {
        ui->searchEdit->clear();
        ui->searchError->setText("User not found.");
    }
}

void SocialNetworkWindow::createAccount()
{
    ui->loginBox->hide();
    ui->createAccBox->show();
}

void SocialNetworkWindow::checkAccount()
{
    int id = net_->numUsers(); std::set<int> friends = {};
    std::string first = ui->firstEdit->toPlainText().toStdString();
    std::string last = ui->lastEdit->toPlainText().toStdString();
    std::string attemptedName = first + " " + last;

    int attemptedId = net_->getId(attemptedName);
    if(attemptedId == -1)
    {
        int year = ui->yearEdit->toPlainText().toInt();
        int zip = ui->zipEdit->toPlainText().toInt();
        User* newUser = new User(id, attemptedName, year, zip, friends);
        net_->addUser(newUser);
        net_->writeUsers(USERS_WRITE_FILE);
        ui->createAccBox->hide();
        ui->loginBox->show();
    }
    else
        ui->firstMessage->setText("Invalid entry. First name:");
}

void SocialNetworkWindow::checkLogin()
{
    std::string attemptedName = ui->loginEdit->toPlainText().toStdString();
    User* attemptedUser = net_->getUser(net_->getId(attemptedName));
    if(attemptedUser != nullptr)
    {
        currentUser_ = loggedInUser_ = attemptedUser;
        display();
    }
    else
    {
        ui->loginEdit->clear();
        ui->loginMessage->setText("User not found. Enter your name:");
    }
}

void SocialNetworkWindow::backToPrevClicked()
{
    currentUser_ = prevUser_;
    display();
}

void SocialNetworkWindow::friendClicked(int row, int column)
{
    std::string name = ui->friendsTable->item(row, column)->text().toStdString();
    updateCurrent(net_->getUser(net_->getId(name)));
    display();
}

void SocialNetworkWindow::suggestionClicked(int row, int column)
{
    std::string name = ui->suggestionsTable->item(row, 0)->text().toStdString();
    if(column == 0)
    {
        updateCurrent(net_->getUser(net_->getId(name)));
    }

    else if(column == 1)
    {
        net_->addConnection(loggedInUser_->getName(), name);
        net_->writeUsers(USERS_WRITE_FILE);
    }
    display();
}

void SocialNetworkWindow::updateCurrent(User *u)
{
    prevUser_ = currentUser_;
    currentUser_ = u;
}

void SocialNetworkWindow::postClicked(int row, int column)
{
    Post* cur = currentPosts_[row];
    std::set<int> likers = cur->getLikers();
    int id = loggedInUser_->getId();
    bool alreadyLiked = (std::find(likers.begin(), likers.end(), id) != likers.end());
    if(column == 0)
        expandedPost_ = cur;
    else if(column == 1 && !alreadyLiked)
    {
        int likes = cur->getLikes();
        cur->setLikes(likes + 1);
        cur->addLiker(id);
        net_->writePosts(POSTS_WRITE_FILE);
    }
    display();
}

void SocialNetworkWindow::backClicked()
{
    updateCurrent(loggedInUser_);
    display();
}

void SocialNetworkWindow::postBackClicked()
{
    expandedPost_ = nullptr;
    display();
}

void SocialNetworkWindow::addFriendClicked()
{
    net_->addConnection(loggedInUser_->getName(), currentUser_->getName());
    net_->writeUsers(USERS_WRITE_FILE);
    display();
}

void SocialNetworkWindow::removeFriendClicked()
{
    net_->deleteConnection(loggedInUser_->getName(), currentUser_->getName());
    net_->writeUsers(USERS_WRITE_FILE);
    display();
}

void addToTable(QTableWidget* table, QString column1, QString column2 = "")
{
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(column1));
    if(column2 != "")
        table->setItem(row, 1, new QTableWidgetItem(column2));
}

void SocialNetworkWindow::comment()
{
    std::string text = ui->enterComment->toPlainText().toStdString();
    net_->addPost(currentUser_->getId(), text, 0, 4, loggedInUser_->getName(), expandedPost_->getMessageId());
    net_->writePosts(POSTS_WRITE_FILE);
    display();
}

void SocialNetworkWindow::addPost()
{
    int ownerId = currentUser_->getId();
    std::string authorName = loggedInUser_->getName();
    std::string text = ui->postEdit->toPlainText().toStdString();

    if(ui->privateCheck->isChecked()) // Private incoming post, even on your own page
        net_->addPost(ownerId, text, 0, 3, authorName);
    else
    {
        if(currentUser_ != loggedInUser_)
            net_->addPost(ownerId, text, 0, 2, authorName);
        else
            net_->addPost(ownerId, text, 0, 1);
    }
    net_->writePosts(POSTS_WRITE_FILE);
    display();
}

void SocialNetworkWindow::display()
{
    //Default: clear the screen every time it updates.
    //Hiding containers and their widgets:
    ui->expandedPost->hide();
    ui->searchBox->hide();
    ui->loginBox->hide();
    ui->createAccBox->hide();
    ui->friendsBox->hide();
    ui->addPostBox->hide();
    ui->suggestionsBox->hide();
    ui->postsTable->hide();

    //Clearing any text entries:
    QList<QTextEdit*> edits = ui->centralwidget->findChildren<QTextEdit*>();
    for(auto edit: edits)
        edit->clear();

    //hiding free buttons/etc.
    ui->logout->hide();
    ui->addFriendButton->hide();
    ui->removeFriendButton->hide();
    ui->profileLabel->hide();
    ui->postsLabel->hide();
    ui->backToProfile->hide();
    ui->backToPrev->hide();

    if(currentUser_ == nullptr)
        ui->loginBox->show();

    else
    {
        //1. If there is a post currently expanded, populate and display it.
        if(expandedPost_ != nullptr)
        {
            QString message = QString::fromStdString(expandedPost_->toString());
            ui->expandedPostText->setText(message);

            std::vector<Post*> comments = net_->getComments(expandedPost_);
            ui->commentsTable->setRowCount(0);
            for(Post* c: comments)
            {
                std::string message = "\n" + c->toString() + "\n";
                addToTable(ui->commentsTable, QString::fromStdString(message));
            }
            ui->expandedPost->show();
            ui->commentsTable->resizeRowsToContents();
        }

        else
        {
        //2. Make labels
            std::string prefix = (currentUser_ == loggedInUser_) ? "My " :
                                                                     currentUser_->getName() + "'s ";
            ui->profileLabel->setText(QString::fromStdString(prefix+"Profile"));

        //3. Reset and fill in the Friends table
            ui->friendsTable->setRowCount(0);
            for(int i : currentUser_->getFriends())
            {
                User* u = net_->getUser(i);
                assert(u != nullptr);

                QString name = QString::fromStdString(u->getName());
                addToTable(ui->friendsTable, name);
            }

        //4. Reset and fill in the Suggestions table
            ui->suggestionsTable->setRowCount(0);
            int score = 0;
            std::vector<int> suggestions = net_->suggestFriends(currentUser_->getId(), score);
            for(int i : suggestions)
            {
                std::string name = net_->getUser(i)->getName();
                addToTable(ui->suggestionsTable, QString::fromStdString(name), "Add Friend");
            }

        //5. Show the 5 most recent posts on the user's wall, checking for private posts.
            if(currentUser_ == loggedInUser_ || net_->areFriends(currentUser_, loggedInUser_))
                currentPosts_ = currentUser_->getPostsVector(MAX_POSTS_DISPLAYED, false);
            else
                currentPosts_ = currentUser_->getPostsVector(MAX_POSTS_DISPLAYED, true);

            ui->postsTable->setRowCount(0);
            for(Post* p : currentPosts_)
            {
                std::string message = "\n" + p->toString() + "\n";
                QString likes = "\U00002764" + QString::number(p->getLikes());
                addToTable(ui->postsTable, QString::fromStdString(message), likes);
            }
            ui->postsTable->show();
            ui->postsTable->resizeRowsToContents();

        //6. Show widgets on all profiles
            ui->logout->show();
            ui->searchBox->show();
            ui->profileLabel->show();
            ui->postsLabel->show();
            ui->addPostBox->show();
            ui->friendsBox->show();

        //7. Show widgets for user's own profile
            if(currentUser_ == loggedInUser_)
            {
                ui->suggestionsBox->show();
                if(prevUser_ != nullptr)
                    ui->backToPrev->show();
            }

        //8. Show widgets for other profiles
            else
            {
                ui->backToProfile->show();
                if(net_->areFriends(currentUser_, loggedInUser_))
                    ui->removeFriendButton->show();
                else
                    ui->addFriendButton->show();
            }
        }
    }
}
