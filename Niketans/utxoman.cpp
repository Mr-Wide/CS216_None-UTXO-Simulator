// utxo_manager.cpp
#include "utxoman.h"
#include <iostream>
#include <fstream>
#include <sstream>

UTXOManager::UTXOManager() : count(0) {}

using std::string,std::cin,std::cout;
string UTXOManager::make_tx_id(){
    string TX_ID = "TXID";
    TX_ID += std::to_string(count);
    return TX_ID;
}
UTXO::UTXO(string ID, int output, double amnt,string own){
        txid = ID;
        output_index = output;
        amount = amnt;
        owner = own;
}
 void UTXOManager::savetoFile(const string& filename){
    std::ofstream file(filename);
    file << count << "\n";
    for(auto &[key,utxo]:utxo_set){
        file << utxo->txid << ","
             << utxo->output_index << ","
             << utxo->amount << ","
             << utxo->owner << "\n";
    }
 }
void UTXOManager::loadfromFile(const string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) return;

    // Clean old data (IMPORTANT)
    for (auto &p : utxo_set) delete p.second;
    utxo_set.clear();

    string line;
    bool firstLine = true;

    while (getline(file, line)) {
        if (firstLine) {
            count = stoll(line);
            firstLine = false;
            continue;
        }

        string txid, owner;
        int index;
        double amount;

        std::stringstream ss(line);
        string temp;

        getline(ss, txid, ',');
        getline(ss, temp, ','); index = stoi(temp);
        getline(ss, temp, ','); amount = stod(temp);
        getline(ss, owner);

        UTXO* utxo = new UTXO(txid, index, amount, owner);
        utxo_set[{txid, index}] = utxo;
    }
}
void UTXOManager::addUTXO(const string& txid, int output_index,
                          double amount, const string& owner) {
    // TODO: Create a UTXO struct
    UTXO* utxo = new UTXO(txid,output_index,amount,owner);
    // TODO: Insert into utxo_set with key (txid, output_index)
    std::pair key_pair = make_pair(txid,output_index);
    count++;
    utxo_set[key_pair] = utxo;
}


bool UTXOManager::removeUTXO(const string& txid, int output_index) {
    auto it = utxo_set.find({txid, output_index});
    if (it == utxo_set.end()) return false;

    delete it->second;
    utxo_set.erase(it);
    count--;
    return true;
}

bool UTXOManager::hasUTXO(const string& txid, int output_index) const {
    // TODO: Check if key exists in utxo_set
    return utxo_set.find({txid, output_index}) != utxo_set.end();
}

UTXO* UTXOManager::getUTXO(const string& txid, int output_index) const {
    auto it = utxo_set.find({txid, output_index});
    return (it == utxo_set.end()) ? nullptr : it->second;
}

double UTXOManager::getBalance(const string& address) const {
    // TODO: Loop through all UTXOs
    double f =0.0;
    for(auto i : utxo_set){
        if(i.second->owner==address){
            f += i.second->amount;
        }
    }
    return f;
    // TODO: Sum amounts where owner == address
}

void UTXOManager::display() const {
    // TODO: Print all UTXOs nicely
    for(auto i: utxo_set){
        cout << "----------------------"<<'\n';
        cout << "UTXO:"<<" " <<i.second->txid<<'\n';
        cout << "Owner:" <<" "<< i.second->owner << '\n';
        cout << "Amount:" << " "<<i.second->amount << '\n';
        cout << "OutPut Index:" <<" "<< i.second->output_index << '\n';
        cout << "----------------------"<<'\n';
    }
}
// // /*
// // int main(){
// //     UTXOManager Manager;
// //     while(false){
// //         cout << "Input the information required for the UTXO" << std::endl;
// //         string Owner;
// //         cout << "Input the Owner's Name" << std::endl;
// //         cin >> Owner;
// //         double amt;
// //         cout << "Amount you would like to share" << std::endl;
// //         cin >> amt;
// //         cout << "Out Index" << std::endl;
// //         int output;
// //         cin >> output;
// //         string tx__id = Manager.make_tx_id();
// //         Manager.addUTXO(tx__id,output,amt,Owner);
// //         cout << "UTXO SucessFully Added" << std::endl;
// //         cout << "Would you like to add another UTXO?" << std::endl;
// //         char tmp;
// //         cin >> tmp;
// //         if(tmp=='y') continue;
// //         else break;
// //     }
// //     string filepath = "./chainstate.txt";
// //     Manager.loadfromFile(filepath);
// //     Manager.display();
// //     Manager.savetoFile(filepath);
// // }
// // */
