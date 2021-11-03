// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018 The ojacoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256(""));
    //700000:

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1587975855, // * UNIX timestamp of last checkpoint block
    1679026,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    4000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xb5;
        pchMessageStart[1] = 0xa1;
        pchMessageStart[2] = 0xf2;
        pchMessageStart[3] = 0xa7;
        vAlertPubKey = ParseHex("025208d9c58b8edb90e7eed266d2aed4b5d1c81a6668083a296bf37798acbdc9ba");
        nDefaultPort = 5817;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 8100; // 75%
        nRejectBlockOutdatedMajority = 10260; // 95%
        nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)
        nMinerThreads = 0;
        nTargetTimespan = 5 * 60; // 5 minutes
        nTargetSpacing = 60; // 64 seconds
        nMaturity = 10;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 720000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 99999999;
        nPOSStartBlock = 99999999;
        nMandatoryUpgradeBlock = 182750;
        nUpgradeBlockVersion = 8;                                   // Block headers must be this version after upgrade block
        nZerocoinStartHeight = 2100000000;                          // OFF
        //nZerocoinStartTime = 4000000000;
        nModifierUpdateBlock = -1;
        nBlockEnforceSerialRange = -1;                              // Enforce serial range starting this block
        nBlockRecalculateAccumulators = nZerocoinStartHeight + 10;  // Trigger a recalculation of accumulators
        nBlockFirstFraudulent = nZerocoinStartHeight;               // First block that bad serials emerged
        nBlockLastGoodCheckpoint = nZerocoinStartHeight;            // Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = -1;                              // Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0*COIN;                            // Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = nZerocoinStartHeight + 20;               // The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1525158000;                           // Sporks signed after (GMT): Tuesday, May 1, 2018 7:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1527811200;                            // Fully reject old spork key after (GMT): Friday, June 1, 2018 12:00:00 AM

        const char* pszTimestamp = "ojacoin is born";
        CMutableTransaction txNew;
        txNew.nVersion = 1;
        txNew.nTime = 1489479450;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 0 << CScriptNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        //txNew.vout[0].scriptPubKey = CScript() << ParseHex("04c10e83b2703ccf322f7dbd62dd5855ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd42774babea0a073b2ed0c9") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1625443096;
        genesis.nBits = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 8194118;

        uint256 hashTarget = uint256().SetCompact(genesis.nBits);

        assert(genesis.GetHash() <= hashTarget);

        hashGenesisBlock = genesis.GetHash();
        //printf("Merkle hash main: %s\n", genesis.hashMerkleRoot.ToString().c_str());
        //printf("Block hash main: %s\n", hashGenesisBlock.ToString().c_str());
        assert(genesis.hashMerkleRoot == uint256("0xc5f622bad72a7eb7bf7e0699b8810e4a0371d742250d8332d3a4a444522a59ed"));
        assert(hashGenesisBlock == uint256("0x000000e1fc6cc3e6332e286f0ff88d381bd64bc91b6ba56bc57d9d63cff2b789"));
		
	vSeeds.push_back(CDNSSeedData("13.37.73.48","13.37.73.48"));
	vSeeds.push_back(CDNSSeedData("96.8.118.120","96.8.118.120"));
	vSeeds.push_back(CDNSSeedData("192.3.228.237","192.3.228.237"));
	vSeeds.push_back(CDNSSeedData("107.174.52.177","107.174.52.177"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 115);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 116);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 161);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0xf9).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "025208d9c58b8edb90e7eed266d2aed4b5d1c81a6668083a296bf37798acbdc9ba";
        strSporkKeyOld = "025208d9c58b8edb90e7eed266d2aed4b5d1c81a6668083a296bf37798acbdc9ba";
        strObfuscationPoolDummyAddress = "Ebsz2mBcw5hb1vqnBx7TP6CSaWt8sqTiz5";
        nStartMasternodePayments = 1403728576; //Wed, 25 Jun 2014 20:36:16 GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 10; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zeca to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xe7;
        pchMessageStart[1] = 0xe3;
        pchMessageStart[2] = 0xe2;
        pchMessageStart[3] = 0xe1;
        vAlertPubKey = ParseHex("025208d9c58b8edb90e7eed266d2aed4b5d1c81a6668083a296bf37798acbdc9ba");
        nDefaultPort = 15817;
        bnProofOfWorkLimit = ~uint256(0) >> 16;
        nEnforceBlockUpgradeMajority = 4320; // 75%
        nRejectBlockOutdatedMajority = 5472; // 95%
        nToCheckBlockUpgradeMajority = 5760; // 4 days
        nMinerThreads = 0;
        nTargetTimespan = 10 * 60; // 10 minutes
        nTargetSpacing = 48; // 48 seconds
        nLastPOWBlock = 2100000000;
        nPOSStartBlock = 50;
        nMaturity = 10;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = -1; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 2100000000;
        //nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = -1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = nZerocoinStartHeight + 10; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = nZerocoinStartHeight; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = nZerocoinStartHeight; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = -1; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = nZerocoinStartHeight + 20; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1522454400; //!> Reject old spork key after Saturday, March 31, 2018 12:00:00 AM GMT

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1612138687;
        genesis.nBits = bnProofOfWorkLimit.GetCompact(); //0x1f00ffff;
        genesis.nNonce = 1050100;

        uint256 hashTarget = uint256().SetCompact(genesis.nBits);

        assert(genesis.GetHash() <= hashTarget);

        hashGenesisBlock = genesis.GetHash();
        printf("Merkle hash test: %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("Block hash test: %s\n", hashGenesisBlock.ToString().c_str());
        assert(genesis.hashMerkleRoot == uint256("0xc5f622bad72a7eb7bf7e0699b8810e4a0371d742250d8332d3a4a444522a59ed"));
        assert(hashGenesisBlock == uint256("0x0000b301b6a653548e0c15b32eff3b82784d8d668fb60b237bd3e5e50b50e49d"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 92);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 41);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 220);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "025208d9c58b8edb90e7eed266d2aed4b5d1c81a6668083a296bf37798acbdc9ba";
        strSporkKeyOld = "025208d9c58b8edb90e7eed266d2aed4b5d1c81a6668083a296bf37798acbdc9ba";
        strObfuscationPoolDummyAddress = "eNdw2pzf2rirEmCfYGgUQdxJSo7ue6JyLK";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nDefaultPort = 51479;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 10 * 60; // 10 minutes
        nTargetSpacing = 48; // 48 seconds
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1454124731;
        genesis.nBits = bnProofOfWorkLimit.GetCompact(); //0x207fffff;
        genesis.nNonce = 116385;

        uint256 hashTarget = uint256().SetCompact(genesis.nBits);

        assert(genesis.GetHash() <= hashTarget);

        hashGenesisBlock = genesis.GetHash();
        //printf("Merkle hash reg: %s\n", genesis.hashMerkleRoot.ToString().c_str());
        //printf("Block hash reg: %s\n", hashGenesisBlock.ToString().c_str());
        assert(genesis.hashMerkleRoot == uint256("0xc5f622bad72a7eb7bf7e0699b8810e4a0371d742250d8332d3a4a444522a59ed"));
        assert(hashGenesisBlock == uint256("0x7a677f426d36b57dda9b67b5b1b544a4380a78c4de12ab82b5f42bd5e0fe9157"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
