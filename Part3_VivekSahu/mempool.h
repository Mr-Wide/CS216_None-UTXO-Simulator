#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <vector>
#include <set>
#include <string>
#include "../Niketan's/utxoman.h"
#include "verifytx.h"


/*
 Transaction Structures
*/
/*
struct TxInput {
    std::string prev_tx;
    int index;
    std::string owner;
};

struct TxOutput {
    double amount;
    std::string address;
};

struct Transaction {
    std::string tx_id;
    std::vector<TxInput> inputs;
    std::vector<TxOutput> outputs;
    double fee;
};
*/
/*
 Mempool Class
*/

class Mempool {
private:
    int max_size;

public:
    std::vector<transaction> transactions;
    std::set<std::pair<std::string,int>> spent_utxos;

    Mempool(int maxSize = 50);

    // Validate and add transaction
    bool add_transaction(const transaction& tx,
                         const UTXOManager& utxo_manager,
                         std::string& error);

    // Remove transaction after mining
    void remove_transaction(const std::string& tx_id);

    // Get top N transactions by fee
    std::vector<transaction> get_top_transactions(int n) const;

    // Clear mempool
    void clear();
};

#endif
