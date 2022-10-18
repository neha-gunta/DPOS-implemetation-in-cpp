#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <set>
#include "sha256.h"


#define numberOfWitnesses 2
#define transacPerBlock 4
#define difficulty 2

using namespace std;
//generates a time stamp in string;
string getTimeStamp(){
    time_t curr_time;
    tm*curr_tm;
    char date_string[100];
    char time_string[100];
    
    time(&curr_time);
    curr_tm = localtime(&curr_time);
    
    strftime(date_string,50,"%B %d %Y %T",curr_tm);
    return date_string;
}
bool sortbysec(const pair<int,int> &a,
              const pair<int,int> &b)
{
    return (a.second > b.second);
}


//generates a SHA 256 hash
string getHash(string s){
    return sha256(s);
    
}


//geerates a merkle root
string getMerkleRoot(vector<int> transactions){
    // string hash1=getHash(to_string(transactions[0])) + getHash(to_string(transactions[1]));
    // string hash2=getHash(to_string(transactions[2])) + getHash(to_string(transactions[4]));
    // return getHash(hash1)+getHash(hash2);
    queue<string> q;
    for(int i=0;i<transactions.size();i++){
        q.push(getHash(to_string(transactions[i])));
    }
    string str;
    q.push("");
    while(q.size()>=2){
        if(q.front()==""){
            q.push("");
            q.pop();
            if(q.size()==2) return q.front();
            continue;
        }
        str=q.front();
        q.pop();
        if(q.front()==""){
            q.push(getHash(str+str));
        }
        else{
        q.push(getHash(str+q.front()));
        q.pop();
        }
    }
    return "4e5e494fa316ffc82b8252b23524f1433639858267d641c1217059dc4403e045";
    
}

//generates hash of header
string getHashOfHeader(string prevHash,string merkleroot,int nonce,string timestamp){
    
    return getHash(getHash(prevHash+merkleroot)+getHash(to_string(nonce)+timestamp));
}
//finding nonce based on difficulty set
int findNonce(string prevHash,string ts,string hash){
    long int n=0;
    string str;
    int x=0;
    while(true){
        str=getHashOfHeader(prevHash,hash,n,ts);
        x=0;
        for(int i=0;i<difficulty;i++){
            if(str[i]=='0') x++;
        }
        if(x==difficulty) return n;
        n++;
    }
    return n;
}

class Node{
    public:
    int ID;
    int stake;
    int reward=0;
    vector<int> propIDs;
    Node(int id,int s,vector<int> Pids={}){
        ID=id;
        stake=s;
        propIDs=Pids;
    }
    void removeProp(int propID){
        for(int i=0;i<propIDs.size();i++){
            if(propIDs[i]==propID) propIDs[i]=-1;
        }
    }
    void addProp(int propID){
        propIDs.push_back(propID);
    }
};
class transaction{
    public:
    int ID;
    int buyerID;
    int sellerID;
    int rewardAmt;
    int propID;
    int transactionVal;
    
    string timeStamp;
    
    transaction(int id,int buyerid,int sellerid,int reward,int propid,int val){
         ID=id;
     buyerID=buyerid;
     sellerID=sellerid;
     rewardAmt=reward;
     propID=propid;
     transactionVal=val;
     timeStamp=getTimeStamp();
        
    }
};

class property{
    public:
    int ID;
    int ownerID;
    int value;
    vector<transaction> transactionList;
    
    property(int id,int ownerid,int val){
        ID=id;
        ownerID=ownerid;
        value=val;
    }
    
    void updateOwner(int ownerID){
        this->ownerID=ownerID;
    }
    
    void printtransactions(){
        if(transactionList.size()==0)cout<<"Nothing to print\n";
        for(int i=0;i<transactionList.size();i++){
            cout<<transactionList[i].sellerID<<" sold to "<<transactionList[i].buyerID<<" the property wth ID:"<<transactionList[i].propID<<" on "<<transactionList[i].timeStamp<<endl;
        }
    }
};

string latestHash="0"; //the header hash of the last block, it is the prevHash of the next block;
class Block{
    public:
    string timeStamp;
    string hashOfData;
    string prevHash;
    int nonce;
    vector<int> transacIDs;
    Block(vector<int> transIDs){
        transacIDs=transIDs;
        timeStamp=getTimeStamp();
        hashOfData=getMerkleRoot(transIDs);
        string temp=prevHash;
        prevHash=latestHash;
        nonce=findNonce(temp,timeStamp,hashOfData);
        latestHash=getHashOfHeader(temp,hashOfData,nonce,timeStamp);
       
        // latestHash=getMerkleRoot({hashOfData,timeStamp,prevHash,to_string(nonce)});
    }   


};


vector<property> allProps;
vector<Node> allUsers;
vector<Block> blockchain;
vector<int> delegates;

queue<transaction> transactionPool;
queue<transaction> validTrans;

int numberOfUsers=0;
int numberOfProperties=0;
int numberOfTransactions=0;

void createProp(){
    property p1(0,-1,200);
    property p2(1,-1,200);
    property p3(2,-1,200);
    property p4(3,-1,200);
    property p5(4,-1,200);
    property p6(5,-1,200);
    property p7(6,-1,200);
    numberOfProperties=7;
    
    allProps.push_back(p1);
    allProps.push_back(p2);
    allProps.push_back(p3);
    allProps.push_back(p4);
    allProps.push_back(p5);
    allProps.push_back(p6);
    allProps.push_back(p7);
    
} //a function to create the properties
void assignOwners(){
    allProps[0].updateOwner(1);
    allProps[1].updateOwner(2);
    allProps[2].updateOwner(0);
    allProps[3].updateOwner(0);
    allUsers[1].propIDs.push_back(0);
    allUsers[2].propIDs.push_back(1);
    allUsers[0].propIDs.push_back(2);
    allUsers[0].propIDs.push_back(3);
}// a function to assign owners to the properties created

void createUsers(){
    Node u1(0,200);
    Node u2(1,300);
    Node u3(2,400);
    allUsers.push_back(u1);
    allUsers.push_back(u2);
    allUsers.push_back(u3);
    numberOfUsers=3;
    
}// a function to create some existing users
void addUser(){
    cout<<"Enter the stake of the user in Rupees"<<endl;
    int stake;
    cin>>stake;
    cout<<"Enter the property IDs owned by the new user(enter -1 at the end)"<<endl;
    int id;
    
    vector<int> props;
    while(true){
        cin>>id;
        if(id==-1)break;
            if(allProps[id].ownerID!=-1){
                cout<<"this property is already owned by user with ID:"<<allProps[id].ownerID<<endl;
                continue;
            }
            props.push_back(id);
    }
    Node newUser(numberOfUsers++,stake,props);
    allUsers.push_back(newUser);
    cout<<"User Successfully Created with userID:"<<newUser.ID<<endl;
    
}// a function to add user


//a function to implement voting and choose witnesses to verify transaction of the Block
vector<int> chooseWitnesses(){
    
    set<int> s;
    vector<int> v;
    while(s.size()<numberOfWitnesses){
        int x=rand()%numberOfUsers;
        s.insert(x);
        
    }
    set<int>::iterator itr=s.begin();
    for(;itr!=s.end();itr++){
        v.push_back(*itr);
    }
    
    return v;
}

//a function to verify transactions based on owner authentication
bool verifytransaction(transaction trans){
    if(trans.buyerID>=numberOfUsers||trans.sellerID>=numberOfUsers) return false;
    if(trans.buyerID==trans.sellerID) return false;
    if(trans.sellerID==allProps[trans.propID].ownerID){
        
        return true;
    }
    return false;
}


//a funtion to stimulate the checking of transactions, creating a block and calculating the reward
int addBlock(vector<int> witnesses){
    vector<int> transIDs;
    int reward=0;
    if(transactionPool.size()==0){
        
        return -1;
    } 
    while(transactionPool.size()!=0){
        
        if(validTrans.size()>=transacPerBlock) break;
        
        int numberOfVerifies=0;
        for(int i=0;i<witnesses.size();i++){
            if(verifytransaction(transactionPool.front())) numberOfVerifies++;;
        }
        
        if(numberOfVerifies>(2*(witnesses.size()/3)+1)){ 
            validTrans.push(transactionPool.front());// if more than 2/3 of the witnesses validate it, it is a valid transaction
            allProps[validTrans.front().propID].updateOwner(validTrans.front().buyerID);
        }
        
        transactionPool.pop();
    }
    
    if(validTrans.size()==0){
        
        return -2;
    } 
    
    
    int x=0;
    // if(validTrans.size()==transacPerBlock){
        
        while(validTrans.empty()!=true){
            
        transIDs.push_back(validTrans.front().ID);
        
        
        x=validTrans.front().propID;
         
        allProps[x].updateOwner(validTrans.front().buyerID); //updateOwner
        allProps[x].transactionList.push_back(validTrans.front()); //add to transactionList
        allProps[x].value=validTrans.front().transactionVal; //upadate val;
        
        allUsers[validTrans.front().buyerID].addProp(x);
        allUsers[validTrans.front().sellerID].removeProp(x);
        
        reward+=validTrans.front().rewardAmt;
        
        validTrans.pop();
        
        }
    
    
    Block b(transIDs);
    blockchain.push_back(b);
    return reward;
    
    // for(int i=0;i<transacPerBlock;i++){
        
    //     if(transactionPool.empty()) return -1;
    //     int numberOfVerifies=0;
    //     for(int i=0;i<witnesses.size();i++){
    //         if(verifytransaction(transactionPool.front())) numberOfVerifies++;;
    //     }
    //     if(numberOfVerifies>(2*(witnesses.size()/3)+1)) {
    //         transIDs.push_back(transactionPool.front().ID);
    //         allProps[transactionPool.front().propID].updateOwner(transactionPool.front().buyerID);
    //         allProps[transactionPool.front().propID].transactionList.push_back(transactionPool.front());
    //         allProps[transactionPool.front().propID].value=transactionPool.front().transactionVal;
    //         reward+=transactionPool.front().rewardAmt;
    //     }
    //     transactionPool.pop();
            
    // }
    //  Block b(transIDs);
    // blockchain.push_back(b);
    // return reward;
   
    
}

//a funtion that chooses the witnesses, calls the addBlock function and also updates the reward of the user
void generateNewBlock(){
    
    int reward=addBlock(delegates);  
    
    queue<transaction> temp=transactionPool;
    if(reward==-1) {cout<<"transactionPool empty\n" ;return;}
    if(reward==-2) {cout<<"number of valid transaction is 0, hence block is not created\n" ;return;}
    
    cout<<"Block Successfully added and reward distributed to witnesses\n"; 
    for(int i=0;i<numberOfWitnesses;i++){ //equally divide the reward among the witnesses
    allUsers[delegates[i]].reward+=reward/numberOfWitnesses;
    cout<<delegates[i]<<" received "<<reward/numberOfWitnesses<<endl;
    }
}

//A function to record buy and sell transaction of user
void addTransactions(){
    int bid,sid,pid,v,r;
    cout<<"Adding a transaction"<<endl;
    cout<<"Enter buyer ID"<<endl;
    cin>>bid;
    cout<<"Enter seller ID"<<endl;
    cin>>sid;
    cout<<"Enter property ID"<<endl;
    cin>>pid;
    cout<<"Enter value of transaction"<<endl;
    cin>>v;
    cout<<"Enter the reward of transaction"<<endl;
    cin>>r;
    transaction newTransaction(numberOfTransactions++,bid,sid,r,pid,v);
    transactionPool.push(newTransaction);
    cout<<"transaction Successfully Created"<<endl;
    while(transactionPool.size()>=transacPerBlock){//automatic generation of block
        cout<<"Number of transactions in pool more than transacPerBlock permitted, trying to create a block\n";
        generateNewBlock();
    }
    
}

//a function to view the transactions history of a property
void viewHistory(){
    cout<<"enter the property ID"<<endl;
    int pid=0;
    cin>>pid;
    allProps[pid].printtransactions();
}

//a funtion to add some transactions to the transactionPool
void dummyTransactions(){
    transaction newTransaction(numberOfTransactions++,1,2,1,1,30);
    transaction newTransaction1(numberOfTransactions++,2,1,10,1,30);
    transaction newTransaction2(numberOfTransactions++,1,2,100,1,30);
    transaction newTransaction3(numberOfTransactions++,2,1,1000,1,30);
    transaction newTransaction4(numberOfTransactions++,1,2,10000,1,30);
    transactionPool.push(newTransaction);
    transactionPool.push(newTransaction1);
    transactionPool.push(newTransaction2);
    transactionPool.push(newTransaction3);
    transactionPool.push(newTransaction4);
}

void printChain(){
    for(int i=0;i<blockchain.size();i++){
        cout<<"Block :         "<<i<<endl;
        cout<<"previous hash : "<<blockchain[i].prevHash<<endl;
        cout<<"timeStamp :     "<<blockchain[i].timeStamp<<endl;
        cout<<"Nonce :         "<<blockchain[i].nonce<<endl;
        cout<<"merkleroot :    "<<blockchain[i].hashOfData<<endl;
        cout<<"block data(transaction IDs):";;
        for(int j=0;j<blockchain[i].transacIDs.size();j++){
            cout<<blockchain[i].transacIDs[j]<<",";
        }
        cout<<endl;cout<<endl;
    }
}
void printUserDetails(){
    int userID;
    cout<<"enter user id"<<endl;
    cin>>userID;
    Node n=allUsers[userID];
    cout<<"User:"<<userID<<endl;
    cout<<"ID                   :"<<userID<<endl;
    cout<<"stake                :"<<n.stake<<endl;
    cout<<"reward               :"<<n.reward<<endl;
    cout<<"properties owned(IDs):";
    for(int i=0;i<n.propIDs.size();i++){
        if(n.propIDs[i]==-1)continue;
        cout<<n.propIDs[i]<<",";
    } 
    cout<<endl;
    
}
void electDelegates(){
    if(validTrans.size()==0)
    {delegates=chooseWitnesses();cout<<"elected delegates are:";
        for(int i=0;i<delegates.size();i++)cout<<delegates[i]<<",";
    }
    else
    cout<<"cannot perform election now"<<endl;;
    
}

void View_delegates(){
    cout<<"The delegates are:"<<endl;
    for(int i=0;i<delegates.size();i++){
      cout<<delegates[i]<<",";
    }
}
int main(int argc, char *argv[]){
    createProp();
    createUsers();
    assignOwners();
    delegates=chooseWitnesses();// initial choosing of witnesses
    dummyTransactions();
    
    vector<int> null={-1};
    Block genesis(null);
    blockchain.push_back(genesis);
    
    int choice=10;
    while(true){
        
        cout<<"\nenter a choice:\n1. Add new User\n2. Add new transaction\n3. View transactions of a property.\n4. Print the blockchain \n5. Print User details\n6. Perform delegates election\n7. Add Block\n8. View existing delegates\nenter -1 to stop running the code"<<endl;
        cin>>choice;
        switch(choice){
        case 1:addUser(); break;
        case 2:addTransactions(); break;
        case 3:viewHistory(); break;
        case 4:printChain(); break;
        case 5:printUserDetails(); break;
        case 6:electDelegates(); break;
        case 7:generateNewBlock();break;
        case 8:View_delegates();break;
        case -1: return 0;
        default:cout<<"Invalid choice"<<endl;
        }
        
    }
    
    
   




   
   
    
}