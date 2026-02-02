#ifndef MINING_H
#define MINING_H

#include <string>
using namespace std;

// Forward declarations (avoid circular includes)
class Mempool;
class UTXOManager;

// PART 4: Mining function
void mine_block(
    string miner_address,
    Mempool &mempool,
    UTXOManager &utxo,
    int max_txs = 5
);

#endif
