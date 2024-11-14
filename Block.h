#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <ctime>

class Block {
public:
    // Constructor
    Block(std::string previousHash, std::string sender, std::string receiver, double amount,std::string hsh,int non);
    Block(std::string previousHash, std::string sender, std::string receiver, double amount);

    // Getters for block information
    std::string getHash() const;
    std::string getPreviousHash() const;
    std::string getTransactionData() const;
    int getNonce() const;


    // Mining and hash functions
    void mineBlock(int difficulty);
    void recalculateHash(); 
    void setPreviousHash(std::string prevHash){
        previousHash = prevHash;
    } // Function to recalculate the block's hash if needed

private:
    std::string previousHash;  // Hash of the previous block
    std::string sender;        // Transaction sender
    std::string receiver;      // Transaction receiver
    double amount;             // Transaction amount
    time_t timestamp;          // Block timestamp
    int nonce;                 // Nonce value for mining
    std::string hash;          // Current block's hash

    std::string calculateHash();  // Calculate the block's hash based on data
};

#endif
