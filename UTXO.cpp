#include <iostream>
#include <vector>
#include <map>
using namespace std;

class UTXOManager{
public:
    map<pair<string,int>,pair<float,string>> u; 
    struct tx{
        string tx_id;
        int index;
        float amount;
        string owner;
    };
    UTXOManager(){
        
        // first pair is for tx_id, index
        //second pair is for amount,owner pair
    }

    void add_utxo(string tx_id, int index, float amount, string owner){
        auto key=make_pair(tx_id,index);
        //following are some basic checks such as no -ve money and no duplicates.
        if(amount<=0) printf("UTXO cannot have negative or 0 BTC!!!");
        else if(u.find(key)!=u.end()) cout<<"the damn key :"<<tx_id<<" "<<index<<" already exists!!!";
        else u[key]=make_pair(amount,owner);
    }

    void remove_utxo(string tx_id, int index){
        auto key=make_pair(tx_id,index);
        if(u.find(key)==u.end()) printf("the damn key never existed!!!");
        else u.erase(key);
    }
    float get_balance(string owner){
        float balance=0;
        for(auto iter:u) if(iter.second.second==owner) balance+=iter.second.first;
        return balance;
    }
    bool exists(string tx_id,int index){
        auto key=make_pair(tx_id,index);
        return u.find(key)!=u.end();
    }
    
    vector<tx> get_utxos_for_owner(string owner){
        vector<tx> result;
        for(auto x:u){
            if(x.second.second==owner){
                result.push_back({x.first.first,x.first.second,x.second.first,owner});
            }
        }

        return result;
    }
};

int main(){
    return 0;
}