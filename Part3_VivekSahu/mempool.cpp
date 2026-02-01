#include "mempool.h"
#include <algorithm>

/*
 Constructor
*/
Mempool::Mempool(int maxSize) {
    max_size = maxSize;
}

/*
 Add transaction to mempool with full validation
*/
bool Mempool::add_transaction(const Transaction& tx,
                              const UTXOManager& utxo_manager,
                              std::string& error)
{
    if ((int)transactions.size() >= max_size) {
        error = "Mempool full";
        return false;
    }

    std::set<std::pair<std::string,int>> used_in_tx;
    double input_sum = 0.0;
    double output_sum = 0.0;

    // Validate inputs
    for (const auto& in : tx.inputs) {
        auto utxo_key = std::make_pair(in.prev_tx, in.index);

        if (!utxo_manager.hasUTXO(in.prev_tx, in.index)) {
            error = "UTXO does not exist";
            return false;
        }

        if (used_in_tx.count(utxo_key)) {
            error = "Double-spend inside transaction";
            return false;
        }

        if (spent_utxos.count(utxo_key)) {
            error = "UTXO already spent in mempool";
            return false;
        }

        used_in_tx.insert(utxo_key);
        UTXO* u = utxo_manager.getUTXO(in.prev_tx, in.index);
        input_sum += u->amount;

    }

    // Validate outputs
    for (const auto& out : tx.outputs) {
        if (out.amount < 0) {
            error = "Negative output amount";
            return false;
        }
        output_sum += out.amount;
    }

    if (input_sum < output_sum) {
        error = "Insufficient funds";
        return false;
    }

    // Accept transaction
    transactions.push_back(tx);
    for (const auto& u : used_in_tx) {
        spent_utxos.insert(u);
    }

    return true;
}

/*
 Remove transaction from mempool after mining
*/
void Mempool::remove_transaction(const std::string& tx_id)
{
    for (auto it = transactions.begin(); it != transactions.end(); ++it) {
        if (it->tx_id == tx_id) {
            for (const auto& in : it->inputs) {
                spent_utxos.erase({in.prev_tx, in.index});
            }
            transactions.erase(it);
            return;
        }
    }
}

/*
 Return top N transactions sorted by fee
*/
std::vector<Transaction> Mempool::get_top_transactions(int n) const
{
    std::vector<Transaction> result = transactions;

    std::sort(result.begin(), result.end(),
              [](const Transaction& a, const Transaction& b) {
                  return a.fee > b.fee;
              });

    if ((int)result.size() > n)
        result.resize(n);

    return result;
}

/*
 Clear entire mempool
*/
void Mempool::clear()
{
    transactions.clear();
    spent_utxos.clear();
}
