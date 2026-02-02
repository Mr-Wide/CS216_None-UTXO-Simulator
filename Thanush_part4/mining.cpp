#include <bits/stdc++.h>
using namespace std;

#include "mining.h" 
#include "mempool.h"
#include "utxoman.h"

// PART 4: Mining Simulation
void mine_block(
    string miner_address,
    Mempool &mempool,
    UTXOManager &utxo,
    int max_txs = 5
) {
    // 1. Get transactions from mempool
    vector<Transaction> txs = mempool.get_transactions(max_txs);

    if (txs.empty()) {
        cout << "No transactions to mine.\n";
        return;
    }

    cout << "Mining block with " << txs.size() << " transactions...\n";

    float total_fees = 0.0f;

    // 2. Process each transaction
    for (auto &tx : txs) {

        // Spend inputs
        for (auto &inp : tx.inputs) {
            if (utxo.exists(inp.tx_id, inp.index)) {
                utxo.remove_utxo(inp.tx_id, inp.index);
            }
        }

        // Create outputs
        for (int i = 0; i < tx.outputs.size(); i++) {
            utxo.add_utxo(
                tx.txid,
                i,
                tx.outputs[i].amount,
                tx.outputs[i].owner
            );
        }

        total_fees += tx.fee;
    }

    // 3. Miner reward (fees only)
    if (total_fees > 0) {
        utxo.add_utxo(
            "coinbase_" + miner_address,
            0,
            total_fees,
            miner_address
        );
    }

    // 4. Remove mined transactions from mempool
    for (auto &tx : txs) {
        mempool.remove_transaction(tx.txid);
    }

    cout << "Block mined successfully!\n";
    cout << "Miner reward: " << total_fees << " BTC\n";
}
