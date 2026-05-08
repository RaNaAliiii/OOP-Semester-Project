#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
//  Forward declarations
class User;
class Page;
class Post;
class Comment;

//  Activity
class Activity
{
    int    type;   // 0 = none
    char* value;
public:
    Activity()
    {
        type = 0;
        value = nullptr;
    }

    ~Activity()
    {
        delete[] value;
        value = nullptr;
    }

    void set(int t, const char* v)
    {
        type = t;
        delete[] value;
        size_t len = strlen(v) + 1;
        value = new char[len];
        strcpy_s(value, len, v);
    }

    bool hasActivity() const { return type != 0; }

    void display() const
    {
        if (type == 0) return;
        const char* label = "";
        if (type == 1) label = "feeling";
        else if (type == 2) label = "thinking about";
        else if (type == 3) label = "making";
        else if (type == 4) label = "celebrating";
        cout << " — is " << label << " " << value;
    }
};

//  Comment
class Comment
{
    char* commentID;
    char* postID;
    char* authorID;   // user or page ID
    char* text;
public:
    Comment()
    {
        commentID = postID = authorID = text = nullptr;
    }

    ~Comment()
    {
        delete[] commentID;
        delete[] postID;
        delete[] authorID;
        delete[] text;
        commentID = postID = authorID = text = nullptr;
    }

    void setCommentID(const char* id)
    {
        delete[] commentID;
        size_t len = strlen(id) + 1;
        commentID = new char[len];
        strcpy_s(commentID, len, id);
    }

    void setPostID(const char* id)
    {
        delete[] postID;
        size_t len = strlen(id) + 1;
        postID = new char[len];
        strcpy_s(postID, len, id);
    }

    void setAuthorID(const char* id)
    {
        delete[] authorID;
        size_t len = strlen(id) + 1;
        authorID = new char[len];
        strcpy_s(authorID, len, id);
    }

    void setText(const char* t)
    {
        delete[] text;
        size_t len = strlen(t) + 1;
        text = new char[len];
        strcpy_s(text, len, t);
    }

    const char* getCommentID() const { return commentID; }
    const char* getPostID()    const { return postID; }
    const char* getAuthorID()  const { return authorID; }
    const char* getText()      const { return text; }
};

//  Post
class Post
{
public:
    static const int MAX_COMMENT = 10;
    static const int MAX_LIKES = 10;

private:
    char* postID;
    char* text;
    int       day, month, year;
    int       postType;          // 1 or 2 from file
    Activity  activity;
    char* ownerID;           // user or page I
    // likes
    char** likedBy;
    int       likeCount;
    // comments
    Comment** comments;
    int       commentCount;
    char* originalPostID;

public:
    Post()
    {
        postID = text = ownerID = originalPostID = nullptr;
        day = month = year = postType = 0;
        likedBy = nullptr;
        likeCount = 0;
        comments = nullptr;
        commentCount = 0;
    }

    ~Post()
    {
        delete[] postID;
        delete[] text;
        delete[] ownerID;
        delete[] originalPostID;

        for (int i = 0; i < likeCount; i++)
            delete[] likedBy[i];
        delete[] likedBy;
        likedBy = nullptr;

        for (int i = 0; i < commentCount; i++)
            delete comments[i];
        delete[] comments;
        comments = nullptr;
    }

    void setPostID(const char* id)
    {
        delete[] postID;
        size_t len = strlen(id) + 1;
        postID = new char[len];
        strcpy_s(postID, len, id);
    }

    void setText(const char* t)
    {
        delete[] text;
        size_t len = strlen(t) + 1;
        text = new char[len];
        strcpy_s(text, len, t);
    }

    void setDate(int d, int m, int y) { day = d; month = m; year = y; }

    void setPostType(int t) { postType = t; }

    void setActivity(int t, const char* v) { activity.set(t, v); }

    void setOwnerID(const char* id)
    {
        delete[] ownerID;
        size_t len = strlen(id) + 1;
        ownerID = new char[len];
        strcpy_s(ownerID, len, id);
    }

    void setOriginalPostID(const char* id)
    {
        delete[] originalPostID;
        size_t len = strlen(id) + 1;
        originalPostID = new char[len];
        strcpy_s(originalPostID, len, id);
    }

    void addLike(const char* uid)
    {
        if (likeCount >= MAX_LIKES) return;

        // check duplicate
        for (int i = 0; i < likeCount; i++)
            if (strcmp(likedBy[i], uid) == 0) return;

        // grow array
        char** tmp = new char* [likeCount + 1];
        for (int i = 0; i < likeCount; i++)
            tmp[i] = likedBy[i];
        delete[] likedBy;
        likedBy = tmp;

        size_t len = strlen(uid) + 1;
        likedBy[likeCount] = new char[len];
        strcpy_s(likedBy[likeCount], len, uid);
        likeCount++;
    }

    bool addComment(Comment* c)
    {
        if (commentCount >= MAX_COMMENT) return false;

        Comment** tmp = new Comment * [commentCount + 1];
        for (int i = 0; i < commentCount; i++)
            tmp[i] = comments[i];
        delete[] comments;
        comments = tmp;

        comments[commentCount] = c;
        commentCount++;
        return true;
    }

    const char* getPostID()         const { return postID; }
    const char* getText()           const { return text; }
    const char* getOwnerID()        const { return ownerID; }
    const char* getOriginalPostID() const { return originalPostID; }
    int  getDay()                   const { return day; }
    int  getMonth()                 const { return month; }
    int  getYear()                  const { return year; }
    int  getPostType()              const { return postType; }
    int  getLikeCount()             const { return likeCount; }
    int  getCommentCount()          const { return commentCount; }

    const char* getLikedByID(int i) const
    {
        if (i < 0 || i >= likeCount) return nullptr;
        return likedBy[i];
    }

    Comment* getComment(int i) const
    {
        if (i < 0 || i >= commentCount) return nullptr;
        return comments[i];
    }

    bool isLikedBy(const char* uid) const
    {
        for (int i = 0; i < likeCount; i++)
            if (strcmp(likedBy[i], uid) == 0) return true;
        return false;
    }

// check if post is within last 24 hours of given reference date
    bool isRecent(int refDay, int refMonth, int refYear) const
    {
        // 1. If the post is on the exact same day
        if (year == refYear && month == refMonth && day == refDay)
            return true;

        // 2. If the post is from the previous day (within 24 hours, same month)
        if (year == refYear && month == refMonth && day == refDay - 1)
            return true;

        // 3. If the post is from the previous day (and the month just changed)
        if (year == refYear && month == refMonth - 1 && refDay == 1 && (day == 30 || day == 31))
            return true;

        return false;
    }

    void display(const char* ownerName, bool withComments) const
    {
        cout << "=================================\n";
        cout << "Post ID  : " << postID << "\n";
        cout << "Owner    : " << ownerName;
        activity.display();
        cout << "\n";
        cout << "Date     : " << day << "/" << month << "/" << year << "\n";
        if (originalPostID)
            cout << "Memory of: " << originalPostID << "\n";
        cout << "Content  : " << text << "\n";
        cout << "Likes    : " << likeCount << "\n";

        if (withComments)
        {
            cout << "Comments (" << commentCount << "):\n";
            for (int i = 0; i < commentCount; i++)
            {
                cout << "  [" << comments[i]->getCommentID() << "] "
                    << comments[i]->getAuthorID() << ": "
                    << comments[i]->getText() << "\n";
            }
        }
    }
};

//  Page
class Page
{
    char* pageID;
    char* title;

    Post** posts;
    int     postCount;

public:
    Page()
    {
        pageID = title = nullptr;
        posts = nullptr;
        postCount = 0;
    }

    ~Page()
    {
        delete[] pageID;
        delete[] title;
        delete[] posts;
        posts = nullptr;
    }

    void setPageID(const char* id)
    {
        delete[] pageID;
        size_t len = strlen(id) + 1;
        pageID = new char[len];
        strcpy_s(pageID, len, id);
    }

    void setTitle(const char* t)
    {
        delete[] title;
        size_t len = strlen(t) + 1;
        title = new char[len];
        strcpy_s(title, len, t);
    }

    void addPost(Post* p)
    {
        Post** tmp = new Post * [postCount + 1];
        for (int i = 0; i < postCount; i++)
            tmp[i] = posts[i];
        delete[] posts;
        posts = tmp;
        posts[postCount] = p;
        postCount++;
    }

    const char* getPageID()  const { return pageID; }
    const char* getTitle()   const { return title; }
    int         getPostCount() const { return postCount; }
    Post* getPost(int i)  const
    {
        if (i < 0 || i >= postCount) return nullptr;
        return posts[i];
    }

    void display() const
    {
        cout << "====================================\n";
        cout << "Page : " << title << "  [" << pageID << "]\n";
        cout << "Posts: " << postCount << "\n";
        cout << "====================================\n";
        for (int i = 0; i < postCount; i++)
            posts[i]->display(title, false);
    }
};

//  User
class User
{
    char* userID;
    char* name;

    char** friendIDs;
    int     friendCount;

    char** likedPageIDs;
    int     likedPageCount;

    Post** timelinePosts;   // posts owned by this user
    int     timelineCount;

public:
    User()
    {
        userID = name = nullptr;
        friendIDs = nullptr;  friendCount = 0;
        likedPageIDs = nullptr;  likedPageCount = 0;
        timelinePosts = nullptr;  timelineCount = 0;
    }

    ~User()
    {
        delete[] userID;
        delete[] name;

        for (int i = 0; i < friendCount; i++)
            delete[] friendIDs[i];
        delete[] friendIDs;
        friendIDs = nullptr;

        for (int i = 0; i < likedPageCount; i++)
            delete[] likedPageIDs[i];
        delete[] likedPageIDs;
        likedPageIDs = nullptr;

        delete[] timelinePosts;
        timelinePosts = nullptr;
    }

    void setUserID(const char* id)
    {
        delete[] userID;
        size_t len = strlen(id) + 1;
        userID = new char[len];
        strcpy_s(userID, len, id);
    }

    void setName(const char* n)
    {
        delete[] name;
        size_t len = strlen(n) + 1;
        name = new char[len];
        strcpy_s(name, len, n);
    }

    void addFriendID(const char* id)
    {
        char** tmp = new char* [friendCount + 1];
        for (int i = 0; i < friendCount; i++)
            tmp[i] = friendIDs[i];
        delete[] friendIDs;
        friendIDs = tmp;

        size_t len = strlen(id) + 1;
        friendIDs[friendCount] = new char[len];
        strcpy_s(friendIDs[friendCount], len, id);
        friendCount++;
    }

    void addLikedPageID(const char* id)
    {
        char** tmp = new char* [likedPageCount + 1];
        for (int i = 0; i < likedPageCount; i++)
            tmp[i] = likedPageIDs[i];
        delete[] likedPageIDs;
        likedPageIDs = tmp;

        size_t len = strlen(id) + 1;
        likedPageIDs[likedPageCount] = new char[len];
        strcpy_s(likedPageIDs[likedPageCount], len, id);
        likedPageCount++;
    }

    void addTimelinePost(Post* p)
    {
        Post** tmp = new Post * [timelineCount + 1];
        for (int i = 0; i < timelineCount; i++)
            tmp[i] = timelinePosts[i];
        delete[] timelinePosts;
        timelinePosts = tmp;
        timelinePosts[timelineCount] = p;
        timelineCount++;
    }

    const char* getUserID()       const { return userID; }
    const char* getName()         const { return name; }
    int         getFriendCount()  const { return friendCount; }
    int         getLikedPageCount() const { return likedPageCount; }
    int         getTimelineCount()  const { return timelineCount; }

    const char* getFriendID(int i) const
    {
        if (i < 0 || i >= friendCount) return nullptr;
        return friendIDs[i];
    }

    const char* getLikedPageID(int i) const
    {
        if (i < 0 || i >= likedPageCount) return nullptr;
        return likedPageIDs[i];
    }

    Post* getTimelinePost(int i) const
    {
        if (i < 0 || i >= timelineCount) return nullptr;
        return timelinePosts[i];
    }

    bool isFriendWith(const char* id) const
    {
        for (int i = 0; i < friendCount; i++)
            if (strcmp(friendIDs[i], id) == 0) return true;
        return false;
    }

    bool hasLikedPage(const char* id) const
    {
        for (int i = 0; i < likedPageCount; i++)
            if (strcmp(likedPageIDs[i], id) == 0) return true;
        return false;
    }
};

//  SocialNetworkApp
class SocialNetworkApp
{
    User** users;
    int       userCount;

    Page** pages;
    int       pageCount;

    Post** posts;
    int       postCount;

    User* currentUser;

    // Reference date for last 24 hours
    int refDay, refMonth, refYear;

    User* findUser(const char* id) const
    {
        for (int i = 0; i < userCount; i++)
            if (strcmp(users[i]->getUserID(), id) == 0)
                return users[i];
        return nullptr;
    }

    Page* findPage(const char* id) const
    {
        for (int i = 0; i < pageCount; i++)
            if (strcmp(pages[i]->getPageID(), id) == 0)
                return pages[i];
        return nullptr;
    }

    Post* findPost(const char* id) const
    {
        for (int i = 0; i < postCount; i++)
            if (strcmp(posts[i]->getPostID(), id) == 0)
                return posts[i];
        return nullptr;
    }

    // resolve display name for any ID
    const char* resolveName(const char* id) const
    {
        User* u = findUser(id);
        if (u) return u->getName();
        Page* p = findPage(id);
        if (p) return p->getTitle();
        return id;
    }

    // File Loaders

    void loadUsers(const char* filename)
    {
        ifstream fin(filename);
        if (!fin)
        {
            cout << "Error: Cannot open " << filename << "\n";
            return;
        }

        fin >> userCount;
        users = new User * [userCount];
        for (int i = 0; i < userCount; i++)
            users[i] = new User();

        char token[64];
        for (int i = 0; i < userCount; i++)
        {
            fin >> token;                      // userID
            users[i]->setUserID(token);


            char firstName[64], lastName[64], fullName[128];
            fin >> firstName;

            streampos pos = fin.tellg();
            fin >> lastName;

            // if lastName starts with 'u' followed by digit, or 'p' followed by digit, or is "-1"
            // then it's not part of the name
            bool lastNameIsToken = false;
            if (strcmp(lastName, "-1") == 0)
                lastNameIsToken = true;
            else if ((lastName[0] == 'u' || lastName[0] == 'p') && lastName[1] >= '0' && lastName[1] <= '9')
                lastNameIsToken = true;

            if (lastNameIsToken)
            {
                users[i]->setName(firstName);
                strcpy_s(token, 64, lastName);
                goto processRest;
            }
            else
            {
                sprintf_s(fullName, 128, "%s %s", firstName, lastName);
                users[i]->setName(fullName);
                fin >> token; 
            }

        processRest:

            while (strcmp(token, "-1") != 0)
            {
                users[i]->addFriendID(token);
                fin >> token;
            }

            // read liked pages until -1
            fin >> token;
            while (strcmp(token, "-1") != 0)
            {
                users[i]->addLikedPageID(token);
                fin >> token;
            }
        }
        fin.close();
    }

    void loadPages(const char* filename)
    {
        ifstream fin(filename);
        if (!fin)
        {
            cout << "Error: Cannot open " << filename << "\n";
            return;
        }

        fin >> pageCount;
        pages = new Page * [pageCount];
        for (int i = 0; i < pageCount; i++)
            pages[i] = new Page();

        char id[64];
        char titleBuf[256];
        fin.ignore();

        for (int i = 0; i < pageCount; i++)
        {
            fin >> id;
            pages[i]->setPageID(id);

            fin.ignore();
            fin.getline(titleBuf, 256);
            pages[i]->setTitle(titleBuf);
        }
        fin.close();
    }

    void readRestOfLine(ifstream& fin, char* buf, int size)
    {
        fin.ignore();
        fin.getline(buf, size);
    }

    void loadPosts(const char* filename)
    {
        ifstream fin(filename);
        if (!fin)
        {
            cout << "Error: Cannot open " << filename << "\n";
            return;
        }

        fin >> postCount;

        posts = new Post * [postCount];
        for (int i = 0; i < postCount; i++)
            posts[i] = new Post();

        char token[256];

        for (int i = 0; i < postCount; i++)
        {
            // Line 1: postType postID
            int ptype;
            char pid[64];
            fin >> ptype >> pid;
            posts[i]->setPostType(ptype);
            posts[i]->setPostID(pid);

            // Line 2: day month year
            int d, m, y;
            fin >> d >> m >> y;
            posts[i]->setDate(d, m, y);

            // date (max date)
            if (y > refYear || (y == refYear && m > refMonth) ||
                (y == refYear && m == refMonth && d > refDay))
            {
                refDay = d; refMonth = m; refYear = y;
            }

            // Line 3: text 
            char textBuf[512];
            readRestOfLine(fin, textBuf, 512);
            posts[i]->setText(textBuf);

            // Line 4: either activityNum activityValue  OR  ownerID
            fin >> ws;
            char peek = fin.peek();

            if (peek >= '1' && peek <= '4')
            {
                int actType;
                fin >> actType;
                char actVal[256];
                readRestOfLine(fin, actVal, 256);
                // trim leading spaces
                int start = 0;
                while (actVal[start] == ' ') start++;
                posts[i]->setActivity(actType, actVal + start);

                fin >> token;
                posts[i]->setOwnerID(token);
            }
            else
            {
                fin >> token;
                posts[i]->setOwnerID(token);
            }

            // Attach post to owner
            User* owner = findUser(posts[i]->getOwnerID());
            if (owner)
                owner->addTimelinePost(posts[i]);
            else
            {
                Page* pg = findPage(posts[i]->getOwnerID());
                if (pg)
                    pg->addPost(posts[i]);
            }

            fin >> token;
            while (strcmp(token, "-1") != 0)
            {
                posts[i]->addLike(token);
                fin >> token;
            }
        }
        fin.close();
    }

    void loadComments(const char* filename)
    {
        ifstream fin(filename);
        if (!fin)
        {
            cout << "Error: Cannot open " << filename << "\n";
            return;
        }

        int total;
        fin >> total;

        char cid[64], pid[64], aid[64];
        char textBuf[512];
        fin.ignore();

        for (int i = 0; i < total; i++)
        {
            fin >> cid >> pid >> aid;
            fin.ignore();
            fin.getline(textBuf, 512);

            Post* p = findPost(pid);
            if (!p) continue;

            Comment* c = new Comment();
            c->setCommentID(cid);
            c->setPostID(pid);
            c->setAuthorID(aid);
            c->setText(textBuf);

            if (!p->addComment(c))
                delete c;  // post is full
        }
        fin.close();
    }

    // Menu helpers

    void printSeparator() const
    {
        cout << "\n====================================\n";
    }

    //Set Current User
    void setCurrentUser()
    {
        printSeparator();
        cout << "Enter User ID: ";
        char id[64];
        cin >> id;

        User* u = findUser(id);
        if (!u)
        {
            cout << "User not found.\n";
            return;
        }
        currentUser = u;
        cout << "Current user set to: " << currentUser->getName() << "\n";
    }

    //View Home 
    void viewHome() const
    {
        if (!currentUser)
        {
            cout << "No current user set.\n";
            return;
        }
        printSeparator();
        cout << "HOME — " << currentUser->getName()
            << "  (showing posts from " << refDay << "/" << refMonth << "/" << refYear << ")\n";
        printSeparator();

        bool found = false;

        // Posts from friends
        cout << "\n== Friend Posts ==\n";
        for (int i = 0; i < currentUser->getFriendCount(); i++)
        {
            const char* fid = currentUser->getFriendID(i);
            User* fr = findUser(fid);
            if (!fr) continue;
            for (int j = 0; j < fr->getTimelineCount(); j++)
            {
                Post* p = fr->getTimelinePost(j);
                if (p->isRecent(refDay, refMonth, refYear))
                {
                    cout << "\n[" << fr->getName() << "]\n";
                    p->display(fr->getName(), false);
                    found = true;
                }
            }
        }

        // Posts from liked pages
        cout << "\n== Liked Page Posts ==\n";
        for (int i = 0; i < currentUser->getLikedPageCount(); i++)
        {
            const char* pgid = currentUser->getLikedPageID(i);
            Page* pg = findPage(pgid);
            if (!pg) continue;
            for (int j = 0; j < pg->getPostCount(); j++)
            {
                Post* p = pg->getPost(j);
                if (p->isRecent(refDay, refMonth, refYear))
                {
                    cout << "\n[" << pg->getTitle() << "]\n";
                    p->display(pg->getTitle(), false);
                    found = true;
                }
            }
        }

        if (!found)
            cout << "No recent posts to show.\n";
    }

    // Like a Post
    void likePost()
    {
        if (!currentUser)
        {
            cout << "No current user set.\n";
            return;
        }
        printSeparator();
        cout << "Enter Post ID to like: ";
        char id[64];
        cin >> id;

        Post* p = findPost(id);
        if (!p)
        {
            cout << "Post not found.\n";
            return;
        }
        if (p->getLikeCount() >= Post::MAX_LIKES)
        {
            cout << "Post has reached max likes (10).\n";
            return;
        }
        if (p->isLikedBy(currentUser->getUserID()))
        {
            cout << "You have already liked this post.\n";
            return;
        }
        p->addLike(currentUser->getUserID());
        cout << "Post liked successfully! Total likes: " << p->getLikeCount() << "\n";
    }

    //View People Who Liked a Post
    void viewLikes() const
    {
        printSeparator();
        cout << "Enter Post ID: ";
        char id[64];
        cin >> id;

        Post* p = findPost(id);
        if (!p)
        {
            cout << "Post not found.\n";
            return;
        }

        cout << "People who liked post [" << id << "] (" << p->getLikeCount() << "):\n";
        if (p->getLikeCount() == 0)
        {
            cout << "  No likes yet.\n";
            return;
        }
        for (int i = 0; i < p->getLikeCount(); i++)
        {
            const char* lid = p->getLikedByID(i);
            cout << "  " << (i + 1) << ". " << resolveName(lid) << " [" << lid << "]\n";
        }
    }

    //Comment on a Post
    void commentOnPost()
    {
        if (!currentUser)
        {
            cout << "No current user set.\n";
            return;
        }
        printSeparator();
        cout << "Enter Post ID: ";
        char pid[64];
        cin >> pid;

        Post* p = findPost(pid);
        if (!p)
        {
            cout << "Post not found.\n";
            return;
        }
        if (p->getCommentCount() >= Post::MAX_COMMENT)
        {
            cout << "Post has reached maximum comments (10).\n";
            return;
        }

        cout << "Enter your comment: ";
        cin.ignore();
        char textBuf[512];
        cin.getline(textBuf, 512);

        // Generate new comment ID
        char newCID[16];
        sprintf_s(newCID, 16, "c%d", p->getCommentCount() + 100);

        Comment* c = new Comment();
        c->setCommentID(newCID);
        c->setPostID(pid);
        c->setAuthorID(currentUser->getUserID());
        c->setText(textBuf);

        if (p->addComment(c))
            cout << "Comment added successfully!\n";
        else
        {
            delete c;
            cout << "Could not add comment.\n";
        }
    }

    //View a Post
    void viewPost() const
    {
        printSeparator();
        cout << "Enter Post ID: ";
        char id[64];
        cin >> id;

        Post* p = findPost(id);
        if (!p)
        {
            cout << "Post not found.\n";
            return;
        }

        p->display(resolveName(p->getOwnerID()), true);

        // Resolve names for comments
        cout << "\n  Resolved names:\n";
        for (int i = 0; i < p->getCommentCount(); i++)
        {
            Comment* c = p->getComment(i);
            cout << "  " << resolveName(c->getAuthorID())
                << ": " << c->getText() << "\n";
        }
    }

    //Share a Memory
    void shareMemory()
    {
        if (!currentUser)
        {
            cout << "No current user set.\n";
            return;
        }
        printSeparator();
        cout << "Enter Post ID to share as memory: ";
        char origID[64];
        cin >> origID;

        Post* orig = findPost(origID);
        if (!orig)
        {
            cout << "Post not found.\n";
            return;
        }

        cout << "Enter your text for this memory: ";
        cin.ignore();
        char textBuf[512];
        cin.getline(textBuf, 512);

        // Create new memory post
        char newPID[16];
        sprintf_s(newPID, 16, "post%d", postCount + 1);

        Post* mem = new Post();
        mem->setPostID(newPID);
        mem->setText(textBuf);
        mem->setDate(refDay, refMonth, refYear);
        mem->setPostType(2);
        mem->setOwnerID(currentUser->getUserID());
        mem->setOriginalPostID(origID);

        // Grow posts array
        Post** tmp = new Post * [postCount + 1];
        for (int i = 0; i < postCount; i++)
            tmp[i] = posts[i];
        delete[] posts;
        posts = tmp;
        posts[postCount] = mem;
        postCount++;

        currentUser->addTimelinePost(mem);

        cout << "Memory shared as post [" << newPID << "]!\n";
        cout << "Original post: " << origID << "\n";
    }

    //View User Profile
    void viewProfile() const
    {
        printSeparator();
        cout << "Enter User ID (leave blank for current user): ";
        cin.ignore();
        char id[64];
        cin.getline(id, 64);

        User* u = nullptr;
        if (strlen(id) == 0)
            u = currentUser;
        else
            u = findUser(id);

        if (!u)
        {
            cout << "User not found.\n";
            return;
        }

        cout << "\n====== Profile: " << u->getName() << " [" << u->getUserID() << "] ======\n";
        cout << "Friends    : " << u->getFriendCount() << "\n";
        cout << "Liked Pages: " << u->getLikedPageCount() << "\n";
        cout << "Posts on Timeline (" << u->getTimelineCount() << "):\n";

        for (int i = 0; i < u->getTimelineCount(); i++)
        {
            u->getTimelinePost(i)->display(u->getName(), false);
        }
    }

    //View Friend List
    void viewFriendList() const
    {
        printSeparator();
        cout << "Enter User ID: ";
        char id[64];
        cin >> id;

        User* u = findUser(id);
        if (!u)
        {
            cout << "User not found.\n";
            return;
        }

        cout << "Friends of " << u->getName() << " (" << u->getFriendCount() << "):\n";
        if (u->getFriendCount() == 0)
        {
            cout << "  No friends.\n";
            return;
        }
        for (int i = 0; i < u->getFriendCount(); i++)
        {
            const char* fid = u->getFriendID(i);
            User* fr = findUser(fid);
            cout << "  " << (i + 1) << ". ";
            if (fr)
                cout << fr->getName() << " [" << fid << "]\n";
            else
                cout << fid << "\n";
        }
    }

    //View Page
    void viewPage() const
    {
        printSeparator();
        cout << "Enter Page ID: ";
        char id[64];
        cin >> id;

        Page* pg = findPage(id);
        if (!pg)
        {
            cout << "Page not found.\n";
            return;
        }
        pg->display();
    }

    //Main Menu
    void printMenu() const
    {
        cout << "\n====================================\n";
        cout << "      SOCIAL NETWORK APPLICATION\n";
        cout << "====================================\n";
        if (currentUser)
            cout << "  Logged in as: " << currentUser->getName() << "\n";
        else
            cout << "  No user logged in.\n";
        cout << "====================================\n";
        cout << " 1.  Set Current User\n";
        cout << " 2.  View Home (Recent Posts)\n";
        cout << " 3.  Like a Post\n";
        cout << " 4.  View Likes on a Post\n";
        cout << " 5.  Comment on a Post\n";
        cout << " 6.  View a Post\n";
        cout << " 7.  Share a Memory\n";
        cout << " 8.  View User Profile\n";
        cout << " 9.  View Friend List\n";
        cout << " 10. View a Page\n";
        cout << " 0.  Exit\n";
        cout << "====================================\n";
        cout << "Choice: ";
    }

public:
    SocialNetworkApp()
    {
        users = nullptr;  userCount = 0;
        pages = nullptr;  pageCount = 0;
        posts = nullptr;  postCount = 0;
        currentUser = nullptr;
        refDay = 14; refMonth = 11; refYear = 2017;
    }

    ~SocialNetworkApp()
    {
        // Delete posts 
        for (int i = 0; i < postCount; i++)
            delete posts[i];
        delete[] posts;
        posts = nullptr;

        // Delete pages 
        for (int i = 0; i < pageCount; i++)
            delete pages[i];
        delete[] pages;
        pages = nullptr;

        // Delete users
        for (int i = 0; i < userCount; i++)
            delete users[i];
        delete[] users;
        users = nullptr;
    }

    void loadData()
    {
        loadPages("Pages.txt");
        loadUsers("Users.txt");
        loadPosts("Posts.txt");
        loadComments("Comments.txt");
        cout << "Data loaded: " << userCount << " users, "
            << pageCount << " pages, "
            << postCount << " posts.\n";
    }

    void Run()
    {
        loadData();

        int choice;
        do
        {
            printMenu();
            cin >> choice;

            switch (choice)
            {
            case 1:  setCurrentUser();  break;
            case 2:  viewHome();        break;
            case 3:  likePost();        break;
            case 4:  viewLikes();       break;
            case 5:  commentOnPost();   break;
            case 6:  viewPost();        break;
            case 7:  shareMemory();     break;
            case 8:  viewProfile();     break;
            case 9:  viewFriendList();  break;
            case 10: viewPage();        break;
            case 0:  cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 0);
    }
};
//  main
int main()
{
    SocialNetworkApp app;
    app.Run();
    return 0;
}