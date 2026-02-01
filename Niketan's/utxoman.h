// utxo_manager.h
#ifndef UTXO_MANAGER_H
#define UTXO_MANAGER_H

#include <string>
#include <map>
#include <utility>  // for std::pair
using std::string;
// Represents a single UTXO
struct UTXO {
    string txid;
    int output_index;
    double amount;
    string owner;
    UTXO();
    UTXO(string ID, int output, double amnt,string own);
};

class UTXOManager {
private:
    // Key: (txid, output_index), Value: UTXO data
    // TODO: What map type? map or unordered_map?
    std::map<std::pair<string, int>, UTXO*> utxo_set;
    long long count;
public:
    UTXOManager();
    string make_tx_id();
    void savetoFile(const string& filename);
    void loadfromFile(const string& filename);
    // Add a new UTXO to the confirmed set
    void addUTXO(const string& txid, int output_index, 
                 double amount, const string& owner);
    
    // Remove a UTXO (when it gets spent)
    bool removeUTXO(const string& txid, int output_index);
    
    // Check if a UTXO exists
    bool hasUTXO(const string& txid, int output_index) const;
    
    // Get a specific UTXO
    UTXO* getUTXO(const string& txid, int output_index) const;
    
    // Calculate total balance for an address
    double getBalance(const string& address) const;
    
    // Display all UTXOs (for debugging)
    void display() const;
};

#endif
