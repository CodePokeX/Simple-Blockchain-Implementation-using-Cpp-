#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "Block.h"
#include <string>


class Blockchain {
public:
    Blockchain();
    ~Blockchain();
    void addBlock(const std::string& sender, const std::string& receiver, double amount);
    void printBlockchain() const;

private:
    struct Node {
        Block* block;
        Node* next;
        Node(Block* b) : block(b), next(nullptr) {}
    };

    Node* head;
    Node* lastBlock;
    int difficulty;

    void saveBlockToFile(const Block& block);
    void removeLastBlock();             // Remove the last block from the chain
    bool loadFromFile(const std::string& filename); // Load blockchain from file
    void createGenesisBlock();           // Create and add genesis block if needed
    bool validateBlockchain();           // Validate blockchain integrity
    void saveBlockchainToFile();
};

#endif
