#ifndef VERIFY_TX_H
#define VERIFY_TX_H

#include <iostream>
#include <string>
#include <vector>
#include "Niketans/utxoman.h"
using namespace std;

struct tx_input{
    string prev_txid;
    int output_index;
    string owner;
};

struct tx_output{
    double amount;
    string address;
};

class transaction{
public:
    string txid;
    double fee;
    vector<tx_input> inputs;
    vector<tx_output> outputs;
    
    transaction() : txid(""), fee(0.001) {} // Default constructor
    transaction(string ID,double fee=0.001);
    void addInput(string prev_txid,int index, string own);
    void addOutput(double amnt,string address);

    bool isValid(const UTXOManager& utxo_manager, string& errorMessage) const;
};


#endif