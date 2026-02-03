#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "Niketans/utxoman.h"
#include "Part3_VivekSahu/mempool.h"
#include "verifytx.h"
#include "Thanush_part4/mining.h"

using namespace std;

// Helper to clear input buffer
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Initialize Genesis Block 
void initializeGenesis(UTXOManager& utxoManager) {
    utxoManager.addUTXO("genesis", 0, 50.0, "Alice");
    utxoManager.addUTXO("genesis", 1, 30.0, "Bob");
    utxoManager.addUTXO("genesis", 2, 20.0, "Charlie");
    utxoManager.addUTXO("genesis", 3, 10.0, "David");
    utxoManager.addUTXO("genesis", 4, 5.0,  "Eve");
}

// Simple Coin Selection Algorithm
// Finds enough UTXOs belonging to 'sender' to cover 'amount'
vector<UTXO> selectUTXOs(const UTXOManager& manager, string sender, double targetAmount) {
    vector<UTXO> selected;
    //double currentSum = 0.0;
    
    // In a real app, we would use manager.get_utxos_for_owner(sender)
    // Since that function wasn't fully implemented in your snippets, we iterate manually:
    // Note: You might need to add an iterator or getter to your UTXOManager class
    // For now, this is a conceptual placeholder. 
    // You MUST implement a way to get all UTXOs in UTXOManager to make this work.
    

    
    return selected; 
}

// void createTransactionUI(Mempool& mempool, UTXOManager& utxoManager) {
//     string sender, recipient;
//     double amount;

//     cout << "Enter sender: ";
//     cin >> sender;
//     double balance = utxoManager.getBalance(sender);
//     cout << "Available balance: " << balance << " BTC" << endl;

//     cout << "Enter recipient: ";
//     cin >> recipient;
//     cout << "Enter amount: ";
//     cin >> amount;

//     if (amount > balance) {
//         cout << "Error: Insufficient funds!" << endl;
//         return;
//     }

//     // 1. Create Transaction Object

//     //HERE

//     string txID = "tx_" + sender + "_" + recipient + "_" + to_string(rand() % 1000);
//     transaction newTx(txID);
    

//     // 2. Coin Selection (Greedy Approach)
//     // We need to find specific UTXOs to spend. 
//     // NOTE: This part requires access to the internal UTXO map or a helper function.
//     // Assuming a helper function `getOwnerUTXOs` exists or you iterate the map.
    
//     // For this simulation, we will prompt the user manually if auto-selection isn't built:
//     cout << "--- Manual UTXO Selection (Simplify for assignment) ---" << endl;
//     cout << "Enter UTXO ID to spend (e.g., genesis): ";
//     string uID; cin >> uID;
//     cout << "Enter UTXO Index: ";
//     int uIdx; cin >> uIdx;

//     if (!utxoManager.hasUTXO(uID, uIdx)) {
//         cout << "Error: UTXO not found." << endl;
//         return;
//     }
    
//     UTXO* u = utxoManager.getUTXO(uID, uIdx);
//     if (u->owner != sender) {
//         cout << "Error: Sender does not own this UTXO." << endl;
//         return;
//     }

//     newTx.addInput(uID, uIdx, sender);
    
//     // 3. Outputs & Fee Calculation
//     // Fee is implicit: Input Sum - Output Sum
//     // We'll set a hardcoded fee for simplicity or ask user
//     double fee = 0.001; 
//     double change = u->amount - amount - fee;

//     if (change < 0) {
//         cout << "Error: Not enough in this specific UTXO to cover amount + fee." << endl;
//         return;
//     }

//     newTx.addOutput(amount, recipient);
//     if (change > 0) {
//         newTx.addOutput(change, sender); // Change back to self
//     }
    
//     // We must manually set the fee property if you added it to the class
//     // newTx.fee = fee; 

//     // 4. Submit to Mempool
//     string errorMsg;
//     cout << "Creating transaction..." << endl;
//     if (mempool.add_transaction(newTx, utxoManager, errorMsg)) {
//         cout << "Transaction valid! Fee: " << fee << " BTC" << endl;
//         cout << "Transaction ID: " << newTx.txid << endl;
//         cout << "Transaction added to mempool." << endl;
//     } else {
//         cout << "Transaction Rejected: " << errorMsg << endl;
//     }
// }


//fixed version below

void createTransactionUI(Mempool& mempool, UTXOManager& utxoManager) {
    string sender, recipient;
    double amount;

    cout << "Enter sender: ";
    cin >> sender;
    
    // Ensure getBalance is implemented in your UTXOManager
    double balance = utxoManager.getBalance(sender); 
    cout << "Available balance: " << balance << " BTC" << endl;

    cout << "Enter recipient: ";
    cin >> recipient;
    cout << "Enter amount: ";
    cin >> amount;

    // 1. Manual UTXO Selection 
    cout << "--- Manual UTXO Selection ---" << endl;
    cout << "Enter UTXO ID to spend (e.g., genesis): ";
    string uID; cin >> uID;
    cout << "Enter UTXO Index: ";
    int uIdx; cin >> uIdx;

    // 2. Fetch and Validate UTXO
    UTXO* u = utxoManager.getUTXO(uID, uIdx); // 'u' is defined here

    if (u == nullptr) {
        cout << "Error: UTXO not found." << endl;
        return;
    }
    
    if (u->owner != sender) {
        cout << "Error: Sender does not own this UTXO." << endl;
        return;
    }

    // 3. Create Transaction Object
    string txID = "tx_" + to_string(rand() % 1000);
    transaction newTx(txID); // This sets fee to 0.001 by default

    // 4. Outputs & Fee Calculation
    // Now 'u' and 'newTx' are both ready
    double change = u->amount - amount - newTx.fee;

    if (change < 0) {
        cout << "Error: Not enough in this UTXO to cover amount + 0.001 fee." << endl;
        return;
    }

    newTx.addInput(uID, uIdx, sender);
    newTx.addOutput(amount, recipient);
    
    if (change > 0) {
        newTx.addOutput(change, sender); // Return change to sender
    }
    
    // 5. Submit to Mempool
    string errorMsg;
    cout << "Creating transaction..." << endl;
    if (mempool.add_transaction(newTx, utxoManager, errorMsg)) {
        cout << "Transaction valid! Fee: " << newTx.fee << " BTC" << endl;
        cout << "Transaction added to mempool." << endl;
    } else {
        cout << "Transaction Rejected: " << errorMsg << endl;
    }
}

// void runTestScenarios(Mempool& mempool, UTXOManager& utxoManager) {
//     cout << "Select test scenario:" << endl;
//     cout << "1. Double-spend (Part 5)" << endl;
//     int choice;
//     cin >> choice;

//     if (choice == 1) {
//         cout << "Running Test: Alice tries to spend same UTXO twice (Race Attack)..." << endl;
        
//         // Scenario: Alice has (genesis, 0) = 50 BTC
//         // TX1: Alice -> Bob (10 BTC)
//         transaction tx1("tx_race_1");
//         tx1.addInput("genesis", 0, "Alice");
//         tx1.addOutput(10.0, "Bob");
//         tx1.addOutput(39.9, "Alice"); 

//         // TX2: Alice -> Charlie (10 BTC) using SAME input
//         transaction tx2("tx_race_2");
//         tx2.addInput("genesis", 0, "Alice");
//         tx2.addOutput(10.0, "Charlie");
//         tx2.addOutput(39.9, "Alice");

//         string err;
//         cout << "TX1: Alice -> Bob (10 BTC)... ";
//         if (mempool.add_transaction(tx1, utxoManager, err)) cout << "ACCEPTED (Valid)" << endl;
//         else cout << "REJECTED (" << err << ")" << endl;

//         cout << "TX2: Alice -> Charlie (10 BTC)... ";
//         if (mempool.add_transaction(tx2, utxoManager, err)) cout << "ACCEPTED (Error)" << endl;
//         else cout << "REJECTED (Success: " << err << ")" << endl;
//     }
// }

//fixed version below
void runTestScenarios(Mempool& mempool, UTXOManager& utxoManager) {
    cout << "Select test scenario:" << endl;
    cout << "1. Double-spend (Part 5)" << endl;
    int choice;
    cin >> choice;

    if (choice == 1) {
        cout << "Running Test: Alice tries to spend same UTXO twice..." << endl;
        
        // Scenario: Alice has (genesis, 0) = 50 BTC
        // TX1: Alice -> Bob (10 BTC). Fee is 0.001. Change = 50 - 10 - 0.001 = 39.999
        transaction tx1("tx_race_1");
        tx1.addInput("genesis", 0, "Alice");
        tx1.addOutput(10.0, "Bob");
        tx1.addOutput(39.999, "Alice"); 

        // TX2: Alice -> Charlie (10 BTC) using SAME input
        transaction tx2("tx_race_2");
        tx2.addInput("genesis", 0, "Alice");
        tx2.addOutput(10.0, "Charlie");
        tx2.addOutput(39.999, "Alice");

        string err;
        cout << "TX1: Alice -> Bob (10 BTC)... ";
        if (mempool.add_transaction(tx1, utxoManager, err)) 
            cout << "ACCEPTED (Valid)" << endl;
        else 
            cout << "REJECTED (" << err << ")" << endl;

        cout << "TX2: Alice -> Charlie (10 BTC)... ";
        if (mempool.add_transaction(tx2, utxoManager, err)) 
            cout << "ACCEPTED (Double-spend failed!)" << endl;
        else 
            cout << "REJECTED (Success: " << err << ")" << endl;
    }
}

int main() {
    // Core Components
    UTXOManager utxoManager;
    Mempool mempool(50); // Max size 50

    // Setup Initial State
    initializeGenesis(utxoManager);

    int choice = 0;
    while (choice != 6) {
        // EXACT Interface from PDF Page 7 [cite: 212-225]
        cout << "\n=== Bitcoin Transaction Simulator ===" << endl;
        cout << "Initial UTXOs (Genesis Block):" << endl;
        // Ideally, create a displayGenesis() function, but standard display() works too
        // utxoManager.display(); 
        
        cout << "Main Menu:" << endl;
        cout << "1. Create new transaction" << endl;
        cout << "2. View UTXO set" << endl;
        cout << "3. View mempool" << endl;
        cout << "4. Mine block" << endl;
        cout << "5. Run test scenarios" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            clearInput();
            continue;
        }

        switch (choice) {
            case 1:
                createTransactionUI(mempool, utxoManager);
                break;
            case 2:
                utxoManager.display();
                break;
            case 3:
                cout << "Mempool has " << mempool.transactions.size() << " transactions." << endl;
                // You can add a loop here to print details of txs in mempool
                break;
            case 4: {
                string miner;
                cout << "Enter miner name: ";
                cin >> miner;
                mine_block(miner, mempool, utxoManager,5);
                break;
            }
            case 5:
                runTestScenarios(mempool, utxoManager);
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    }

    return 0;
}