#include "os.h"
#include "cx.h"
#include <stdbool.h>

#ifndef PARSE_H
#define PARSE_H

#define MAX_BIP32_PATH 10

#define ADD_PRE_FIX_STRING "T"
#define ADDRESS_SIZE 21
#define TOKENID_SIZE 7
#define BASE58CHECK_ADDRESS_SIZE 34
#define BASE58CHECK_PK_SIZE 64
#define HASH_SIZE 32

#define TRC20_DATA_FIELD_SIZE 68

#define SUN_DIG 6
#define ADD_PRE_FIX_BYTE_MAINNET 0x41
// #define MAX_RAW_TX 240
#define MAX_RAW_TX 65
// #define MAX_RAW_SIGNATURE 200
#define MAX_RAW_SIGNATURE 65
#define MAX_TOKEN_LENGTH 67

#define PB_TYPE 0x07
#define PB_FIELD_R 0x03
#define PB_VARIANT_MASK 0x80
#define PB_BASE128 0x80
#define PB_BASE128DATA 0x7F

#include "../proto/core/Contract.pb.h"

typedef union {
  protocol_TransferContract transfer_contract;
  protocol_TransferAssetContract transfer_asset_contract;
  protocol_TriggerSmartContract trigger_smart_contract;
  protocol_VoteWitnessContract vote_witness_contract;
  protocol_ProposalCreateContract proposal_create_contract;
  protocol_ExchangeCreateContract exchange_create_contract;
  protocol_ExchangeInjectContract exchange_inject_contract;
  protocol_ExchangeWithdrawContract exchange_withdraw_contract;
  protocol_ExchangeTransactionContract exchange_transaction_contract;
} contract_t;

typedef enum parserStatus_e {
    USTREAM_PROCESSING,
    USTREAM_FINISHED,
    USTREAM_FAULT
} parserStatus_e;

typedef enum contractType_e {
    ACCOUNTCREATECONTRACT = 0,
    TRANSFERCONTRACT,
    TRANSFERASSETCONTRACT,
    VOTEASSETCONTRACT,
    VOTEWITNESSCONTRACT,
    WITNESSCREATECONTRACT,
    ASSETISSUECONTRACT,
    WITNESSUPDATECONTRACT = 8,
    PARTICIPATEASSETISSUECONTRACT,
    ACCOUNTUPDATECONTRACT,
    FREEZEBALANCECONTRACT,
    UNFREEZEBALANCECONTRACT,
    WITHDRAWBALANCECONTRACT,
    UNFREEZEASSETCONTRACT,
    UPDATEASSETCONTRACT,
    PROPOSALCREATECONTRACT,
    PROPOSALAPPROVECONTRACT,
    PROPOSALDELETECONTRACT,
    SETACCOUNTIDCONTRACT,
    CUSTOMCONTRACT,
    CREATESMARTCONTRACT = 30,
    TRIGGERSMARTCONTRACT,
    EXCHANGECREATECONTRACT = 41,
    EXCHANGEINJECTCONTRACT,
    EXCHANGEWITHDRAWCONTRACT,
    EXCHANGETRANSACTIONCONTRACT,
    UPDATEENERGYLIMITCONTRACT,
    ACCOUNTPERMISSIONUPDATECONTRACT
} contractType_e;

typedef struct stage_t {
    uint16_t total;
    uint16_t count;
} stage_t;

typedef struct txContext_t {
    cx_sha256_t *sha2;
    bool initialized;
    uint8_t queueBuffer[60];
    uint8_t queueBufferLength;
} txContext_t;

typedef struct publicKeyContext_t {
    cx_ecfp_public_key_t publicKey;
    uint8_t address[ADDRESS_SIZE];
    uint8_t address58[BASE58CHECK_ADDRESS_SIZE];
} publicKeyContext_t;

typedef struct {
  uint32_t indices[MAX_BIP32_PATH];
  uint8_t length;
} bip32_path_t;

typedef struct transactionContext_t {
    bip32_path_t bip32_path;
    uint8_t rawTx[MAX_RAW_TX];
    uint32_t rawTxLength;
    uint8_t hash[HASH_SIZE];
    uint8_t signature[MAX_RAW_SIGNATURE];
    uint8_t signatureLength;
} transactionContext_t;

typedef struct txContent_t {
    uint64_t amount;
    uint64_t amount2;
    uint64_t exchangeID;
    uint64_t bandwidth;
    uint8_t account[ADDRESS_SIZE];
    uint8_t destination[ADDRESS_SIZE];
    uint8_t contractAddress[ADDRESS_SIZE];
    uint8_t TRC20Amount[32];
    uint8_t decimals[2];
    uint8_t tokenNames[2][MAX_TOKEN_LENGTH];
    uint8_t tokenNamesLength[2];
    uint8_t resource;
    uint8_t TRC20Method;
    uint32_t customSelector;
    contractType_e contractType;
    uint64_t dataBytes;
    publicKeyContext_t *publicKeyContext;
} txContent_t;

bool setContractType(uint8_t type, void * out);
bool setExchangeContractDetail(uint8_t type, void * out);

bool parseTokenName(uint8_t token_id, uint8_t *data, uint32_t dataLength, txContent_t *context);
bool parseExchange(const uint8_t *data, size_t dataLength, txContent_t *context);

unsigned short print_amount(uint64_t amount, uint8_t *out,
                                uint32_t outlen, uint8_t sun);
bool adjustDecimals(const char *src, uint32_t srcLength, char *target,
                    uint32_t targetLength, uint8_t decimals);



void initTx(txContext_t *context, cx_sha256_t *sha2, txContent_t *content);

parserStatus_e processTx(uint8_t *buffer, uint32_t length, txContent_t *content);

#endif
