#include "Blockchain.h"
#include <fstream>
#include <iostream>
#include <sstream>

Blockchain::Blockchain() : head(nullptr), lastBlock(nullptr), difficulty(3) {
    loadFromFile("blockchain_data.txt");
}

Blockchain::~Blockchain() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current->block;
        delete current;
        current = next;
    }
}

void Blockchain::addBlock(const std::string& sender, const std::string& receiver, double amount) {
    std::string previousHash = lastBlock ? lastBlock->block->getHash() : "0";
    Block* newBlock = new Block(previousHash, sender, receiver, amount);
    newBlock->mineBlock(difficulty);

    Node* newNode = new Node(newBlock);
    if (!head) head = newNode;
    if (lastBlock) lastBlock->next = newNode;
    lastBlock = newNode;

    saveBlockToFile(*newBlock);
}

void Blockchain::saveBlockToFile(const Block& block) {
    std::ofstream file("blockchain_data.txt", std::ios::app);
    file << block.getPreviousHash() << " " << block.getTransactionData() << " " << block.getHash() << " "<<block.getNonce()<<"\n";
    file.close();
}

void Blockchain::saveBlockchainToFile() {
    std::ofstream file("blockchain_data.txt", std::ios::trunc);
    Node* current = head;
    while (current) {
        Block* block = current->block;
        file << block->getPreviousHash() << " " << block->getTransactionData() << " " << block->getHash() << " "<<block->getNonce()<<"\n";
        current = current->next;
    }
    file.close();
}

void Blockchain::createGenesisBlock() {
    Block* genesisBlock = new Block("0", "Genesis", "Genesis", 0);
    genesisBlock->mineBlock(difficulty);

    head = new Node(genesisBlock);
    lastBlock = head;

    saveBlockToFile(*genesisBlock);
    std::cout << "Genesis block created and saved to file.\n";
}

bool Blockchain::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open blockchain data file.\n";
        return false;
    }

    if (file.peek() == std::ifstream::traits_type::eof()) {
        createGenesisBlock();
        file.close();
        return true;
    }

    std::string previousHash, sender, receiver, hash;
    double amount;
    int nonce;
    Node* prevNode = nullptr;

    while (file >> previousHash >> sender >> receiver >> amount >> hash>> nonce) {
    
        Block* newBlock = new Block(previousHash, sender, receiver, amount,hash,nonce);

        // Link the new block in the chain
        Node* newNode = new Node(newBlock);
        if (!head) head = newNode;
        if (prevNode) prevNode->next = newNode;
        prevNode = newNode;

        // Set the new node as the last block
        lastBlock = newNode;
    }
    file.close();
    std::cout<<"Blockchain Loaded Successfully!"<<std::endl;
    // Validate the entire blockchain for integrity
   return validateBlockchain();
}

bool Blockchain::validateBlockchain() {
    if (!head || !head->next || !lastBlock) return true;  // Empty or only genesis block is valid by default

    Node* current = head->next;
    Node* prevNode = head;

    // Step 1: Recalculate and update the hash and previous hash for each block
    while (current) {

        Block* currentBlock = current->block;
        // Update the current block's previous hash to match the hash of the previous block
        currentBlock->setPreviousHash(prevNode->block->getHash());
        // Recalculate the hash of the current block
        currentBlock->recalculateHash();
        prevNode = current;
        current = current->next;
    }

    // Step 2: Save the updated blockchain to file
    saveBlockchainToFile();
    // Step 3: Check the last block's hash for difficulty level
    std::string target(difficulty, '0');
    if (lastBlock->block->getHash().substr(lastBlock->block->getHash().size()-difficulty, difficulty) != target) {
        std::cout << "Last block does not meet difficulty. Removing invalid last block...\n";
        removeLastBlock();
        return validateBlockchain();  // Re-run validation after removal
    }

    return true;
}

void Blockchain::removeLastBlock() {
    if (!head) return;

    if (head == lastBlock) {
        delete head->block;
        delete head;
        head = lastBlock = nullptr;
    } else {
        Node* current = head;
        while (current->next != lastBlock) {
            current = current->next;
        }
        delete lastBlock->block;
        delete lastBlock;
        lastBlock = current;
        lastBlock->next = nullptr;
    }
    saveBlockchainToFile();
}

void Blockchain::printBlockchain() const {
    Node* current = head;
    while (current) {
        std::cout << "Transaction: " << current->block->getTransactionData() << "\n"
                  << "Hash: " << current->block->getHash() << "\n"
                  << "Previous Hash: " << current->block->getPreviousHash() << "\n\n";
        current = current->next;
    }
}
