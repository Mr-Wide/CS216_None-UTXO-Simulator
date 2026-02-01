#ifndef VERIFY_TX_H
#define VERIFY_TX_H

#include <iostream>
#include <string>
#include <vector>
#include "Niketan's/utxoman.h"
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
    vector<tx_input> inputs;
    vector<tx_output> outputs;
     
    transaction(string ID);
    void addInput(string prev_txid,int index, string own);
    void addOutput(double amnt,string address);

    bool isValid(const UTXOManager& utxo_manager, string& errorMessage) const;
};


#endif