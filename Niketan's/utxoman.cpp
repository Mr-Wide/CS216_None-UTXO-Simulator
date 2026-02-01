// utxo_manager.cpp
#include "utxoman.h"
#include <iostream>
#include <fstream>

using std::string,std::cin,std::cout;
string UTXOManager::make_tx_id(){
    string TX_ID = "TXID";
    TX_ID += std::to_string(count);
    return TX_ID;
}
UTXO::UTXO(){};
UTXO::UTXO(string ID, int output, double amnt,string own){
        txid = ID;
        output_index = output;
        amount = amnt;
        owner = own;
}
UTXOManager::UTXOManager() {
    // Constructor - utxo_set is already initialized
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
    string line="";
    while (std::getline(file,line)){
        //parsing it.
        if(line.find(',') == std::string::npos) count = stoi(line);
        int j=0,cnt = 0;
        UTXO* utxo = new UTXO;
        for(int i=0;i<=line.length();i++){
            if(line[i]==',' || i==line.length()){
                string inp = line.substr(j,i-j);
                
                if(cnt==0){
                    utxo->txid = inp;
                }
                if(cnt==1){
                    utxo->output_index = std::stoi(inp);
                }
                if(cnt==2){
                    utxo->amount =  static_cast<double>(std::stoi(inp));
                }
                if(cnt==3){
                    utxo->owner =  inp;
                }
                j = i+1;
                cnt++;
                continue;
            }
        }
        if(cnt == 4) utxo_set[make_pair(utxo->txid,utxo->output_index)] = utxo;
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
    // TODO: Find and erase from utxo_set
    std::pair key_pair = make_pair(txid,output_index);
    bool removed;
    auto tmp = utxo_set[key_pair];
    if(tmp==nullptr) {
        return false;
    }
    delete tmp;
    count--;
    return true;
    // Return true if found and removed, false otherwise
}

bool UTXOManager::hasUTXO(const string& txid, int output_index) const {
    // TODO: Check if key exists in utxo_set
    std::pair key_pair = make_pair(txid,output_index);
    auto tmp =  utxo_set.find(key_pair);
    if(tmp->second==nullptr)return false;
    return true;
}

UTXO* UTXOManager::getUTXO(const string& txid, int output_index) const {
    // TODO: Return the UTXO if it exists 
    std::pair key_pair = make_pair(txid,output_index);
    auto tmp =  utxo_set.find(key_pair);
    return tmp->second;
    // What if it doesn't exist? Throw exception or return empty UTXO?
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
int main(){
    UTXOManager Manager;
    while(false){
        cout << "Input the information required for the UTXO" << std::endl;
        string Owner;
        cout << "Input the Owner's Name" << std::endl;
        cin >> Owner;
        double amt;
        cout << "Amount you would like to share" << std::endl;
        cin >> amt;
        cout << "Out Index" << std::endl;
        int output;
        cin >> output;
        string tx__id = Manager.make_tx_id();
        Manager.addUTXO(tx__id,output,amt,Owner);
        cout << "UTXO SucessFully Added" << std::endl;
        cout << "Would you like to add another UTXO?" << std::endl;
        char tmp;
        cin >> tmp;
        if(tmp=='y') continue;
        else break;
    }
    string filepath = "./chainstate.txt";
    Manager.loadfromFile(filepath);
    Manager.display();
    Manager.savetoFile(filepath);
}
