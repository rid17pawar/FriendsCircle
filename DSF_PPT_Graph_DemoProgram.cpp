#include<iostream>
#include<string.h>
using namespace std;
/*
It allows to add new account, login through existing account and then
after logged in, connect with new friend, see friends details, see all friends list etc.
Graph data is  stored in Adjacency List

sample i/p: abc, abc, abc, 1, 2, 2000, 4, 5
               xyz, xyz, xyz, 1, 2, 2000, 3, 7
              mno, mno, mno, 2, 3, 2000, 8, 2
adjacency list:
0->2
1->
2->0
*/

class Node
{
    public:     //each user node structure
        char name[20];
        int index, day, month, year, totalComments, totalLikes, totalFriends;
        Node* next;

        Node()
        {
            index=0;
            day=0;
            month=0;
            year=0;
            totalComments=0;
            totalLikes=0;
            totalFriends=0;
            next=NULL;
        }

        Node(int i, char n[20], int d, int m, int y, int c, int l)
        {
            index= i;
            strcpy(name, n);
            day= d;
            month= m;
            year= y;
            totalComments= c;
            totalLikes= l;
            next=NULL;
        }
};

class Queue
{
    public:
        Node* queueData[10];  //Queue Implementation using Array
        int frontQ=-1;     //queue: [1 2 3 4 5 6]
        int rearQ=-1;       //          ^              ^
                                //       front            rear
        bool isEmpty()
        {
            if(frontQ>rearQ || rearQ==-1)
                return true;
            else
                return false;
        }

        void enqueue(Node* n1)
        {
            if(isEmpty())
            {
                frontQ=rearQ=0;
                queueData[0]=n1;
            }
            else
            {
                queueData[++rearQ]=n1; //preincreament
            }
        }

        Node* dequeue()
        {
            if(isEmpty())
                return NULL;
            else
            {
                Node* temp= queueData[frontQ++]; //postdecreament
                if(isEmpty())
                {
                    frontQ=rearQ=-1;
                }
                return temp;
            }
        }
};

class FriendsGraph
{
    public:
        Node* g[5]; //nodes list [no of max users that can be added]
        int friends[5]; //friends[] = stores no of friends for each user
        int visited[5]; //used in DFS recursive traversal

        void initializeGraph(int n)
        {
             for(int i=0; i<n; i++)
             {
                   g[i]=NULL; //used Adjacency List representation
                   friends[i]=0; //friends count
             }
        }

        void displayGraph(int n)
        {
            for(int i=0; i<n; i++)
            {   //g[i] contains address of user node. So we need to use '->' to point to the node whose address is stored in g[i]
                cout<<"\n\tUser "<<i+1<<" -->";
                cout<<"\n\tIndex: "<<g[i]->index;
                cout<<"\n\tName: "<<g[i]->name;
                cout<<"\n\tDate of Birth [DOB] :";
                cout<<"\n\tDay: "<<g[i]->day;
                cout<<"\n\tMonth: "<<g[i]->month;
                cout<<"\n\tYear: "<<g[i]->year;
                cout<<"\n\tNo of  Comments: "<<g[i]->totalComments;
                cout<<"\n\tNo of Likes: "<<g[i]->totalLikes;
                cout<<"\n\tNo of Friends: "<<friends[i]<<endl;
            }
        }

        void addUserInGraph(int userindex)
        {
            char uname[20];
            int uday, umonth, uyear, ulikes, ucomments;

                cout<<"\tFor User "<<userindex<<" -->"<<endl;
                //value of i is treated as index of node
                cout<<"\tName: ";
                cin>>uname;
                cout<<"\tDate of Birth [DOB] :"<<endl;
                cout<<"\tDay: ";
                cin>>uday;
                cout<<"\tMonth: ";
                cin>>umonth;
                cout<<"\tYear: ";
                cin>>uyear;
                cout<<"\tNo of  Comments: ";
                cin>>ucomments;
                cout<<"\tNo of Likes: ";
                cin>>ulikes;

                g[userindex]=new Node(userindex,  uname, uday, umonth, uyear, ucomments, ulikes);   //create new node and store in graph
        }

        void displayUserInGraph(int i)
        {
             //g[i] contains address of user node. So we need to use '->' to point to the node whose address is stored in g[i]
                cout<<"\n\tIndex: "<<g[i]->index;
                cout<<"\n\tName: "<<g[i]->name;
                cout<<"\n\tDate of Birth [DOB] :";
                cout<<"\n\tDay: "<<g[i]->day;
                cout<<"\n\tMonth: "<<g[i]->month;
                cout<<"\n\tYear: "<<g[i]->year;
                cout<<"\n\tNo of  Comments: "<<g[i]->totalComments;
                cout<<"\n\tNo of Likes: "<<g[i]->totalLikes;
                cout<<"\n\tNo of Friends: "<<friends[i]<<endl;
        }

        void displayAllUsersToConnect(int n, int currentuserindex)
        {
            for(int i=0; i<n; i++)
            {   //g[i] contains address of user node. So we need to use '->' to point to the node whose address is stored in g[i]
                if(i==currentuserindex){ continue; }
                cout<<"\n\tUser "<<i+1<<" -->";
                cout<<"\n\t\tIndex: "<<g[i]->index;
                cout<<"\n\t\tName: "<<g[i]->name;
            }
        }

        void createFriendshipEdge(int user1Index, int user2Index)
        {
            Node *node1, *node2;
            node1=g[user2Index];    //node to search in adjacency list, to connect a new node to it.
            //new Node(*g[user1Index])= pass node whose address is stored in g[i]
            node2=new Node(*g[user1Index]);   //node to connect to node1's list at end, to indicate an edge between both
            node2->next=NULL;

            while(node1->next!=NULL)    //traverse till last node in node1's list, in the adjacency list
            {
                node1=node1->next;
            }
            node1->next=node2;  //add node2 at end of node1's list
            friends[user1Index]++; //no of friends of a user= no of nodes in node1's list= degree of a node( bcoz it is undirected graph)
        }

        void showFriendsDetails(int currentuserindex, int friendindex)
        {
            int flag=0;
            Node *user1, *friendOfUser1;    //user1= points to each node in array,  friendOfUser1= points to each node attached to user1 index.
            //for first node
            user1=g[currentuserindex];
            friendOfUser1= user1->next; // traverse all its friends [BFS]
            while(friendOfUser1!= NULL)
            {
              if(friendOfUser1->index==friendindex)
              {
                flag=1;
                cout<<"\n\tIndex: "<<friendOfUser1->index;
                cout<<"\n\tName: "<<friendOfUser1->name;
                cout<<"\n\tDate of Birth [DOB] :";
                cout<<"\n\tDay: "<<friendOfUser1->day;
                cout<<"\n\tMonth: "<<friendOfUser1->month;
                cout<<"\n\tYear: "<<friendOfUser1->year;
                cout<<"\n\tNo of  Comments: "<<friendOfUser1->totalComments;
                cout<<"\n\tNo of Likes: "<<friendOfUser1->totalLikes;
                cout<<"\n\tNo of Friends: "<<friends[friendOfUser1->index]<<endl;
              }
              friendOfUser1= friendOfUser1->next;  //go to next friend
            }

            if(flag==0){ cout<<"\nCouldn't find friend's details please check your friends list and try again..\n"; }
        }

        void usersFriendsShow(int currentuserindex)
        {
            int flag=0;
            Node *user1, *friendOfUser1;    //user1= points to each node in array,  friendOfUser1= points to each node attached to user1 index.
            //for first node
            user1=g[currentuserindex];
            friendOfUser1= user1->next; // traverse all its friends [BFS]
            while(friendOfUser1!= NULL)
            {
              flag=1;
              cout<<"\n\tUser "<<friendOfUser1->index+1<<" -->";
              cout<<"\n\t\tIndex: "<<friendOfUser1->index;
              cout<<"\n\t\tName: "<<friendOfUser1->name;

              friendOfUser1= friendOfUser1->next;  //go to next friend
            }

            if(flag==0){ cout<<"\nCouldn't find friend's please check your friends list and try again..\n"; }
        }

};

int main()
{
    FriendsGraph graph1;
    //general data
    int mainchoice, n, currentUserIndex, temp;
    string currentUsername, currentUserPassword;
    n=5;
    //data of each user
    string usernames[n]; //max 5 users
    string passwords[n];
    int users_count=0;

    graph1.initializeGraph(n);
    cout<<"\n\t\t\t***************************Welcome to Friends Circle***************************";
    do{
            cout<<"\n\n_____________Menu________________";
            cout<<"\n\n1. Sign Up (New User) \n2. Sign In (Existing User) \n3. Show All Users \n4. Exit \n";
            cin>>mainchoice;
            //main switch
            switch(mainchoice)
            {
                case 1:  if( users_count>= 5 ){ cout<<"\n**can't create new user.. 5 users already exist..**\n";  break; }
                          cout<<"\n__________Sign Up Form_________\n\n";
                          cout<<"Enter new username: ";
                          cin>>usernames[users_count];
                          cout<<"Enter new password: ";
                          cin>>passwords[users_count];
                          cout<<"**Account Created Successfully**\n\n";
                          cout<<"Provide Your Details:\n";
                          //cout<<"username= "<<usernames[users_count]<<", password= "<<passwords[users_count]<<"\n";
                          graph1.addUserInGraph(users_count);  //send new users array index

                          cout<<"**Please log in to use this app**\n";
                          users_count++;
                          //break;

                case 2: cout<<"\n_________Sign In Form_________\n\n";
                          cout<<"Enter your username: ";
                          cin>>currentUsername;
                          cout<<"Enter your password: ";
                          cin>>currentUserPassword;
                          //search for username index
                          temp=-1;
                          for(int i=0; i<n; i++)
                          {
                              if( currentUsername.compare(usernames[i])==0 && currentUserPassword.compare(passwords[i])==0 )
                              {     temp=i;     break;    }
                          }

                          if(temp==-1)
                          {
                              cout<<"**unable to login.. please try again..**\n";
                          }
                          else
                          {
                          currentUserIndex=temp;
                          cout<<"**Account Logged In Successfully**\n";
                          //cout<<"currentUsername= "<<currentUsername<<", currentUserPassword= "<<currentUserPassword;

                           //user activities switch after user logged in
                            int choice;
                            //cout<<"\nin friendsNetworkSwitch:";
                             do{
                                    cout<<"\n\n_____________Choose Operations______________";
                                    cout<<"\n\n1. My Profile \n2. Connect to New Friend \n3. Search for Existing Friend \n4. Show All Friends \n5. Go to main menu \n";
                                    cin>>choice;

                                    switch(choice)
                                    {
                                        case 1:  //cout<<"case1: \n";
                                                  graph1.displayUserInGraph(currentUserIndex);
                                                  break;

                                        case 2:  //cout<<"case2: \n";
                                                  if(users_count<2){ cout<<"\nThere is only "<<users_count<<" user in system..";  break;}
                                                  cout<<"All users in our system:\n";
                                                  graph1.displayAllUsersToConnect(users_count, currentUserIndex);
                                                  cout<<"\nEnter index of user to connect: ";
                                                  cin>>temp;
                                                  if(temp>=users_count || temp==currentUserIndex){ cout<<"Please choose from above list.."; break; }
                                                  graph1.createFriendshipEdge(temp, currentUserIndex);
                                                  graph1.createFriendshipEdge(currentUserIndex, temp); //bcoz undirected graph
                                                  break;

                                        case 3:  //cout<<"case3: \n";
                                                  if(users_count<2){ cout<<"\nThere is only "<<users_count<<" user in system..";  break;}
                                                  cout<<"\nEnter index of friend to see details: ";
                                                  cin>>temp;
                                                  if(temp>=users_count || temp==currentUserIndex){ cout<<"Please check your friends list and try again.."; break; }
                                                  graph1.showFriendsDetails(currentUserIndex, temp);
                                                  break;

                                        case 4:  //cout<<"case4: \n";
                                                  if(users_count<2){ cout<<"\nThere is only "<<users_count<<" user in system..";  break;}
                                                  graph1.usersFriendsShow(currentUserIndex);
                                                  break;

                                        case 5:  cout<<"Redirecting to Main Menu... \n";
                                                  break;

                                        default: cout<<"Enter valid option\n";
                                    }

                                }while(choice!=5);

                          }
                          break;

                case 3: cout<<"All Users:\n";
                          graph1.displayGraph(users_count);
//                          for(int i=0; i<users_count; i++)
//                          {  cout<<"user "<<i<<": "<<usernames[i]<<"\n";  }
                          break;

                case 4: cout<<"Thank You for using our application... \n";
                          break;

                default: cout<<"Enter valid option\n";
            }

        }while(mainchoice!=4);

    return 0;
}
