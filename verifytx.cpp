#include "verifytx.h"
#include <set>
using namespace std;

transaction::transaction(string ID){
    txid = ID;
}

void transaction::addInput(string prev_txid, int index, string own){
    inputs.push_back({prev_txid,index,own});
}

void transaction::addOutput(double amnt,string address){
    outputs.push_back({amnt,address});
}

bool transaction::isValid(const UTXOManager& utxo_manager, string& errorMessage) const {
    double totalInputAmount = 0.0;
    double totalOutputAmount = 0.0;
    std::set<std::pair<string, int>> spent;

    //the following is for rules 1,2 and 3
    for (const auto& in : inputs) {
        // 2.No double-spending in inputs(sameUTXOtwiceinsametransaction)
        if (spent.count({in.prev_txid, in.output_index})) {
            errorMessage = "Double-spend detected in same transaction!";
            return false;
        }
        spent.insert({in.prev_txid, in.output_index});

        // 1.All inputs must exist inUTXOset
        if (!utxo_manager.hasUTXO(in.prev_txid, in.output_index)) {
            errorMessage = "Input UTXO does not exist!";
            return false;
        }

        // 3. Sum(inputs)≥Sum(outputs)(difference=fee)
        UTXO* utxo = utxo_manager.getUTXO(in.prev_txid, in.output_index);
        totalInputAmount += utxo->amount;
    }

    //4.No negative amounts in outputs
    for (const auto& out : outputs) {
        if (out.amount < 0) {
            errorMessage = "Negative output amount detected!";
            return false;
        }
        totalOutputAmount += out.amount;
    }

    // 3. Sum(inputs)≥Sum(outputs)(difference=fee)
    if (totalInputAmount < totalOutputAmount) {
        errorMessage = "Insufficient funds: Input sum less than output sum!";
        return false;
    }

    // mempool not done
    return true;
}