# CS216 Bitcoin Transaction & UTXO Simulator (C++)

This repository contains a local simulator of Bitcoin’s transaction lifecycle using the UTXO model. It supports transaction validation, a mempool with conflict detection, mining simulation (fee collection), and double-spending prevention.
Optional feature added:
 Added savetoFile writer and loadFromFile parser member functions for the utxoman class, this enables us to have a well defined chainstate which has the information about the UTXOs
## Project layout

Source files:
- main.cpp      (menu-driven simulator)
- verifytx.cpp  (Transaction structure + validation rules)
- mempool.cpp   (mempool management + spent UTXO tracking)
- mining.cpp    (mining simulation / block confirmation)
- utxoman.cpp   (UTXO set management + input/test parser)

Headers:
- 4 header files used by the above modules (kept in the same directory).

## Build requirements
- g++ (C++17)
- GNU make

## Build (compiles everything)
From the repository root:

```bash or windows
make