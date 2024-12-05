// 0x6080604052348015600e575f80fd5b5060a580601a5f395ff3fe6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fea26469706673582212208c8b10127671ef1a5655a80df15550f338bf0035a3de1fde0c9d35694658d98064736f6c634300081a0033

// 3 sections
// 1. Contract Creation
// 2. Runtime
// 3. Metadata

// Free Memory Pointer
PUSH1 0x80      // [0x80]
PUSH1 0x40      // [0x80, 0x40]
MSTORE          // []       Memory: 0x40 -> 0x80

// If someone sent value with this call, revert!
// Otherwise jump to 0x0e PC

CALLVALUE       // [msg.value]
DUP1            // [msg.value, msg.value]
ISZERO          // [msg.value, msg.value == 0]
PUSH1 0x0e      // [msg.value, msg.value == 0, 0x0e(PC)]
JUMPI           // [msg.value]
PUSH0           // [msg.value, 0x00]
DUP1            // [msg.value, 0x00, 0x00]
REVERT          // [msg.value]

// Jump dest if msg.value == 0
JUMPDEST        // [msg.value]
POP             // []
PUSH1 0xa5      // [0xa5]
DUP1            // [0xa5, 0xa5]
PUSH1 0x1a      // [0xa5, 0xa5, 0x1a]
PUSH0           // [0xa5, 0xa5, 0x1a, 0x00]
CODECOPY        // Stack: [0xa5], Memory: [Runtime Code]
PUSH0           // Stack: [0xa5, 0x00], Memory: [Runtime Code]
RETURN          // []
INVALID         // []

// 2. Runtime
// Free Memory Pointer
PUSH1 0x80
PUSH1 0x40
MSTORE

CALLVALUE       // [msg.value]
DUP1            // [msg.value, msg.value]
ISZERO          // [msg.value, msg.value == 0]
PUSH1 0x0e      // [msg.value, msg.value == 0, 0x0e]
JUMPI           // [msg.value]
// Jump to continue if msg.value == 0

PUSH0           // [msg.value, 0x00]
DUP1            // [msg.value, 0x00, 0x00]
REVERT          // []

// If msg.value == 0, start here!
JUMPDEST        // [msg.value]
POP             // []
// This is checking to see if there's enough calldata for a function selector
PUSH1 0x04      // [0x04]
CALLDATASIZE    // [0x04, calldata_size]
LT              // [calldata_size < 0x04]
PUSH1 0x30      // [calldata_size < 0x04, 0x30]
JUMPI           // []
// If calldata_size < 0x04 -> calldata_Jump

PUSH0           // [0x00]
CALLDATALOAD    // [calldata(32 bytes)]
PUSH1 0xe0      // [calldata, 0xe0]
SHR             // [function_selector] -> shift 0xe0(224 bits) of calldata to the right
DUP1            // [function_selector, function_selector]
PUSH4 0xcdfead2e    // [function_selector, function_selector, 0xcdfead2e]  
EQ                  // [function_selector, function_selector == 0xcdfead2e]
PUSH1 0x34          // [function_selector, function_selector == 0xcdfead2e, 0x34]
JUMPI               // [function_selector]
// If function_selector == 0xcdfead2e  -> JUMP (setNumberOfHorses) 

DUP1                // [function_selector, function_selector]
PUSH4 0xe026c017    // [function_selector, function_selector, 0xe026c017]
EQ                  // [function_selector, function_selector == 0xe026c017]
PUSH1 0x45          // [function_selector, function_selector == 0xe026c017, 0x45]
JUMPI               // [function_selector]
// If function_selector == 0xe026c017 -> JUMP (readNumberOfHorses)

// calldata_jump
// Revert Jumpdest
JUMPDEST        // []
PUSH0           // [0x00]
DUP1            // [0x00, 0x00]
REVERT          // []

// We came to JUMPDEST -> bcoz of some JUMPIF (JUMPI)
// UpdateHorseNumber jump dest 1
// Setup jumping Program Counter in the stack
JUMPDEST        // [function_selector]
PUSH1 0x43      // [function_selector, 0x43]
PUSH1 0x3f      // [function_selector, 0x43, 0x3f]
CALLDATASIZE    // [function_selector, 0x43, 0x3f, calldata_size]
PUSH1 0x04      // [function_selector, 0x43, 0x3f, calldata_size, 0x04]
PUSH1 0x59      // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x59]
JUMP            // [function_selector, 0x43, 0x3f, calldata_size, 0x04]

// Jump dest 4
// We can now run a sstore to store our number to storage
// 1. Function dispatch
// 2. Checked for msg.value
// 3. Checked that calldata is long enough
// 4. Received the number to use from the calldata

JUMPDEST        // [function_selector, 0x43, calldata (of numberToUpdate)]
PUSH0           // [function_selector, 0x43, calldata (of numberToUpdate), 0x00]
SSTORE          // [function_selector, 0x43], Storage: [calldata (of numberToUpdate]
JUMP            // [function_selector, 0x43]

// Jump dest 5
JUMPDEST        // [function_selector]
STOP            // [function_selector]

// Jump dest for readNumberOfHorses
JUMPDEST        // [function_selector]
PUSH0           // [function_selector, 0x00]
SLOAD           // [function_selector, numHorses]
PUSH1 0x40      // [function_selector, numHorses, 0x40]
MLOAD           // [function_selector, numHorses, 0x80], Memory: [0x40: 0x80] (free memory pointer)
SWAP1           // [function_selector, 0x80, numHorses]
DUP2            // [function_selector, 0x80, numHorses, 0x80]
MSTORE          // [function_selector, 0x80], Memory: [0x80: numHorses], memory occupied, later should update free memory pointer
PUSH1 0x20      // [function_selector, 0x80, 0x20]
ADD             // [function_selector, 0x80 + 0x20(0xa0)]
PUSH1 0x40      // [function_selector, 0xa0, 0x40]
MLOAD           // [function_selector, 0xa0, 0x80]
DUP1            // [function_selector, 0xa0, 0x80, 0x80]
SWAP2           // [function_selector, 0x80, 0x80, 0xa0]
SUB             // [function_selector, 0x80, 0xa0 - 0x80]
SWAP1           // [function_selector, 0xa0 - 0x80, 0x80]
// Return a value of size (0xa0 - 0x80 = 32 bytes) which is located in memory at 0x80.
// Since memory is no more used, it does not update the free memory pointer.
 
RETURN

// UpdateHorseNumber Jump dest 2
// Check to see if there's a value to update the horse number to
// 4 bytes for function selector, (0x20) or 32 bytes for horse number
JUMPDEST        // [function_selector, 0x43, 0x3f, calldata_size, 0x04]
PUSH0           // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00]
PUSH1 0x20      // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00, 0x20]
DUP3            // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00, 0x20, 0x04] - duplicate 3rd item on stack
DUP5            // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00, 0x20, 0x04, calldata_size] - duplicate 5th item on stack
SUB             // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00, 0x20, calldata_size - 0x04]
// Is there more calldata than the function selector?
// Function selector (4 bytes) + data (32 bytes) --> FALSE
SLT             // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00, (calldata_size - 0x04) < 0x20]
ISZERO          // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00, ((calldata_size - 0x04) < 0x20) == 0]
PUSH1 0x68      // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00, more_calldata_than_selector??, 0x68]
JUMPI           // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00]
// There'll be a jump to jump dest 3 if there is more calldata than:
// function selector + 0x20

// Revert if there isn't enough calldata
PUSH0           // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00, 0x00]
DUP1            // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00, 0x00, 0x00]
REVERT          // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00]

// Jump dest 3
// Grabbing the calldata for updating the horse number
JUMPDEST        // [function_selector, 0x43, 0x3f, calldata_size, 0x04, 0x00]
POP             // [function_selector, 0x43, 0x3f, calldata_size, 0x04]
// Ignore the function selector, and just grab the input
// 0xcdfead2e   0000000000000000000000000000000000000000000000000000000000000001 (32 bytes)

CALLDATALOAD    // [function_selector, 0x43, 0x3f, calldata_size, calldata (of numberToUpdate)]
SWAP2           // [function_selector, 0x43, calldata (of numberToUpdate), calldata_size, 0x3f]
SWAP1           // [function_selector, 0x43, calldata (of numberToUpdate), 0x3f, calldata_size]
POP             // [function_selector, 0x43, calldata (of numberToUpdate), 0x3f]
JUMP
// Jump to jump dest 4

INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
KECCAK256
DUP13
DUP12
LT
SLT
PUSH23 0x71ef1a5655a80df15550f338bf0035a3de1fde0c9d3569
CHAINID
PC
INVALID
DUP1
PUSH5 0x736f6c6343
STOP
ADDMOD
BYTE
STOP
CALLER