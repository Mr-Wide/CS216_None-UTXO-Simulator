#include <bits/stdc++.h>
using namespace std;

#include "mining.h" 
#include "../Part3_VivekSahu/mempool.h"
#include "../Niketans/utxoman.h"

// PART 4: Mining Simulation
void mine_block(
    string miner_address,
    Mempool &mempool,
    UTXOManager &utxo,
    int max_txs 
) {
    max_txs=5;
    // 1. Get transactions from mempool
    vector<transaction> txs = mempool.get_top_transactions(max_txs);

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
            if (utxo.hasUTXO(inp.prev_txid, inp.output_index)) {
                utxo.removeUTXO(inp.prev_txid, inp.output_index);
            }
        }

        // Create outputs
        for (int i = 0; i < tx.outputs.size(); i++) {
            utxo.addUTXO(
                tx.txid,
                i,
                tx.outputs[i].amount,
                tx.outputs[i].address
            );
        }

        total_fees += tx.fee;
    }

    // 3. Miner reward (fees only)
    if (total_fees > 0) {
        // Create a UTXO for the miner consisting ONLY of the collected fees
        utxo.addUTXO("fees_" + txs[0].txid, 0, total_fees, miner_address);
        cout << "Miner " << miner_address << " received " << total_fees << " BTC in fees." << endl;
    }

    // 4. Remove mined transactions from mempool
    for (auto &tx : txs) {
        mempool.remove_transaction(tx.txid);
    }

    cout << "Block mined successfully!\n";
    cout << "Miner reward: " << total_fees << " BTC\n";
}
