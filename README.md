# DPOS-implemetation-in-cpp

This project is about implementing DPOS consensus to make the Land Management system more safe and reliable!

what is DPOS?
Delegated Proof Of Stake (DPoS) is a consensus algorithm which is an improvement of the core ideas of Proof Of Stake. An election system is used to select the nodes that verify blocks in the DPos system. These nodes are referred as as "block producers" or  "witnesses."

DPoS consensus operates as follows:
\
Voting :\
In DPoS consensus users can either directly vote or give their voting power to another entity to vote on their behalf. The chosen witness are responsible for generating blocks by verifying transactions. If they verify and sign all transactions in a block, they are rewarded, which is usually shared with those who have voted for witness. If a witness fails to verify all transactions in the given time, block is missed, all transactions are left  unverified and no reward is distributed to that witness. The reward is added up to reward of the next witness which verifies that block.
\
Code Explanation:

Classes created\
•	Node: This class represents a node/user and their properties in a blockchain network, including the stake, the total reward earned, etc.\
•	Transaction: This class includes the features of each transaction done. It includes the buyerID, sellerID, value of transaction, timestamp,reward associated.\
•	Property: Includes features of a property, includes ID, current owner and current value\
•	Block: contains all the properties of a block, includes previous hash, merkle root, nonce, timestamp

Functions created:\
•	getTimeStamp() : returns the time stamp that is stored in the block header.\
•	getMerkleRoot() : returns the Merkle root by hashing the transaction IDs.\
•	getHashOfHeader() : returns the hash of the block header by hashing the prevHash, Merkle tree root, timestamp, nonce.\
•	addUser() : Used to add a node in the blockchain network,takes the necessary input from user, generates the new User\
•	addTransactions() : Used to create a transaction,takes the necessary input from user, generates the transaction and adds the transaction to the transactionPool.\
•	chooseWitnesses() : Voting system used to elect the witnesses. Implemented using DPOS consensus Algorithm. The number of witnesses can be changes in the code, this function returns a vector<int> of witness IDs\
  •	verifytransaction() : Verification done by witnesses, returns the bool(T/F) for each transaction;\
  •	addBlock() : Used to add a block in the blockchain network. Every block contains 2 transactions. This function is executed once the queue containing the transaction reaches size of 2. The number of transactions can be changed in the code.\
  •	viewChain(): shows details of every block in the chain.\
  •	generateNewBlock(): chooses the witnesses, verifies the transactions, and updates the reward value of the witnesses.
