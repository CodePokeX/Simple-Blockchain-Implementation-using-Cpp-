#include "Block.h"
#include "SHA256.h"
#include <sstream>
#include <iomanip>
#include <iostream>

// Constructor initializes block data and calculates the initial hash
Block::Block(std::string previousHash, std::string sender, std::string receiver, double amount,std::string hsh,int non)
    : previousHash(previousHash), sender(sender), receiver(receiver), amount(amount), nonce(non), hash(hsh),timestamp(time(nullptr)) {
}

Block::Block(std::string previousHash, std::string sender, std::string receiver, double amount)
    : previousHash(previousHash), sender(sender), receiver(receiver), amount(amount), nonce(0), timestamp(std::time(nullptr)) {
    hash = calculateHash();
}

// Calculate the hash based on block content
std::string Block::calculateHash() {
    std::stringstream ss;
    ss << previousHash << sender << receiver <<std::fixed << std::setprecision(2) <<  amount << nonce;
    return SHA256::hash(ss.str());
}

// Get the current hash of the block
std::string Block::getHash() const {
    return hash;
}

int Block::getNonce() const{
    return nonce;
}
// Get the previous block's hash
std::string Block::getPreviousHash() const {
    return previousHash;
}

// Return the transaction data as a formatted string
std::string Block::getTransactionData() const {
    std::stringstream ss;
    ss << sender <<" "<<  receiver <<" "<< amount;
    return ss.str();
}

// Mine the block by adjusting the nonce until the hash meets the difficulty
void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0');  // Target string with 'difficulty' leading zeros

    // Loop to find a nonce that generates a hash matching the difficulty level
    while (hash.substr(hash.size() - difficulty, difficulty) != target) {
        nonce++;
        hash = calculateHash();
        std::cout<<"Hash "<<hash<<std::endl;
    }
    std::cout << "Block mined: " << hash << std::endl;
}

// Recalculate the hash of the current block, for example if data changes
void Block::recalculateHash() {
    hash = calculateHash();
}




