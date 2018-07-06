#include <boost/test/unit_test.hpp>
#include "amount.h"
//#include "test/miner_tests.cpp" //blockinfo
#include "miner.h"
#include "miner.cpp"
#include "main.h" //mempool, global variable
#include "test/test_bitcoin.h" //entryhelper
#include "txmempool.h"

//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Miner2_tests


BOOST_FIXTURE_TEST_SUITE(miner2_tests, TestingSetup)


static
struct {
    unsigned char extranonce;
    unsigned int nonce;
} blockinfo[] = {
    {4, 0xa4a3e223}, {2, 0x15c32f9e}, {1, 0x0375b547}, {1, 0x7004a8a5},
    {2, 0xce440296}, {2, 0x52cfe198}, {1, 0x77a72cd0}, {2, 0xbb5d6f84},
    {2, 0x83f30c2c}, {1, 0x48a73d5b}, {1, 0xef7dcd01}, {2, 0x6809c6c4},
    {2, 0x0883ab3c}, {1, 0x087bbbe2}, {2, 0x2104a814}, {2, 0xdffb6daa},
    {1, 0xee8a0a08}, {2, 0xba4237c1}, {1, 0xa70349dc}, {1, 0x344722bb},
    {3, 0xd6294733}, {2, 0xec9f5c94}, {2, 0xca2fbc28}, {1, 0x6ba4f406},
    {2, 0x015d4532}, {1, 0x6e119b7c}, {2, 0x43e8f314}, {2, 0x27962f38},
    {2, 0xb571b51b}, {2, 0xb36bee23}, {2, 0xd17924a8}, {2, 0x6bc212d9},
    {1, 0x630d4948}, {2, 0x9a4c4ebb}, {2, 0x554be537}, {1, 0xd63ddfc7},
    {2, 0xa10acc11}, {1, 0x759a8363}, {2, 0xfb73090d}, {1, 0xe82c6a34},
    {1, 0xe33e92d7}, {3, 0x658ef5cb}, {2, 0xba32ff22}, {5, 0x0227a10c},
    {1, 0xa9a70155}, {5, 0xd096d809}, {1, 0x37176174}, {1, 0x830b8d0f},
    {1, 0xc6e3910e}, {2, 0x823f3ca8}, {1, 0x99850849}, {1, 0x7521fb81},
    {1, 0xaacaabab}, {1, 0xd645a2eb}, {5, 0x7aea1781}, {5, 0x9d6e4b78},
    {1, 0x4ce90fd8}, {1, 0xabdc832d}, {6, 0x4a34f32a}, {2, 0xf2524c1c},
    {2, 0x1bbeb08a}, {1, 0xad47f480}, {1, 0x9f026aeb}, {1, 0x15a95049},
    {2, 0xd1cb95b2}, {2, 0xf84bbda5}, {1, 0x0fa62cd1}, {1, 0xe05f9169},
    {1, 0x78d194a9}, {5, 0x3e38147b}, {5, 0x737ba0d4}, {1, 0x63378e10},
    {1, 0x6d5f91cf}, {2, 0x88612eb8}, {2, 0xe9639484}, {1, 0xb7fabc9d},
    {2, 0x19b01592}, {1, 0x5a90dd31}, {2, 0x5bd7e028}, {2, 0x94d00323},
    {1, 0xa9b9c01a}, {1, 0x3a40de61}, {1, 0x56e7eec7}, {5, 0x859f7ef6},
    {1, 0xfd8e5630}, {1, 0x2b0c9f7f}, {1, 0xba700e26}, {1, 0x7170a408},
    {1, 0x70de86a8}, {1, 0x74d64cd5}, {1, 0x49e738a1}, {2, 0x6910b602},
    {0, 0x643c565f}, {1, 0x54264b3f}, {2, 0x97ea6396}, {2, 0x55174459},
    {2, 0x03e8779a}, {1, 0x98f34d8f}, {1, 0xc07b2b07}, {1, 0xdfe29668},
    {1, 0x3141c7c1}, {1, 0xb3b595f4}, {1, 0x735abf08}, {5, 0x623bfbce},
    {2, 0xd351e722}, {1, 0xf4ca48c9}, {1, 0x5b19c670}, {1, 0xa164bf0e},
    {2, 0xbbbeb305}, {2, 0xfe1c810a},
};

void checkScore_CTxMemPoolEntry(CTxMemPoolEntry entry){
    int64_t mod_fee = entry.GetModifiedFee();
    if (mod_fee == 0) {
        std::cout << "modified fee is 0" << std::endl;
    } else {
        std::cout << mod_fee / entry.GetTxSize() << std::endl;    
    }
}

BOOST_AUTO_TEST_CASE(Check_ScoreCompareClass)
{
	TestMemPoolEntryHelper entry;
	CMutableTransaction txFirst;
	txFirst.vin.resize(1);
	txFirst.vin[0].scriptSig = CScript() << OP_11;
	txFirst.vout.resize(1);
	txFirst.vout[0].scriptPubKey = CScript() << OP_11 << OP_EQUAL;
	txFirst.vout[0].nValue = 100LL;

    CMutableTransaction txSecond;
	txSecond.vin.resize(1);
	txSecond.vin[0].scriptSig = CScript() << OP_11;
	txSecond.vin[0].prevout.hash = txFirst.GetHash();
	txSecond.vin[0].prevout.n = 0;
	txSecond.vout.resize(1);
	txSecond.vout[0].scriptPubKey = CScript() << OP_11 << OP_EQUAL;
	txSecond.vout[0].nValue = 100LL;

	CTxMemPool txPool(CFeeRate(0));
	txPool.addUnchecked(txFirst.GetHash(), entry.FromTx(txFirst));
	txPool.addUnchecked(txSecond.GetHash(), entry.FromTx(txSecond));
	CTxMemPool::txiter iterFirst = txPool.mapTx.begin();
	CTxMemPool::txiter iterSecond = txPool.mapTx.end();
    checkScore_CTxMemPoolEntry(*iterFirst);
    checkScore_CTxMemPoolEntry(*iterSecond);

	BOOST_CHECK(ScoreCompare()(iterSecond, iterFirst)); //if iterFirst < itersecond => true;
    //BOOST_CHECK(sc(iterFirst, iterSecond));
}

BOOST_AUTO_TEST_CASE(UpdataTime_function){
	CBlockHeader blockHeader;
    blockHeader.nTime=0;
	Consensus::Params consensus_params;
	CBlockIndex block_index = CBlockIndex();
	//blockHeader.ntime = 100;
	//block_index.ntime = 100;

	BOOST_CHECK(UpdateTime(&blockHeader, consensus_params, &block_index));
}

// 

    

    //make a new transaction
    // CMutableTransaction tx;
    // tx.vin.resize(1);
    // tx.vin[0].scriptSig = CScript() << OP_0 << OP_0 << OP_0 << OP_NOP << OP_CHECKMULTISIG << OP_1;
    // tx.vin[0].prevout.hash = txFirst[0]->GetHash();
    // tx.vin[0].prevout.n = 0;
    // tx.vout.resize(1);
    // tx.vout[0].nValue = 5000000000LL;
    // for (unsigned int i = 0; i < 1001; ++i)
    // {
    //     tx.vout[0].nValue -= 1000000;
    //     uint256 hash = tx.GetHash();
    //     bool spendsCoinbase = (i == 0) ? true : false; // only first tx spends coinbase
    //     // If we do set the # of sig ops in the CTxMemPoolEntry, template creation passes
    //     mempool.addUnchecked(hash, entry.Fee(1000000).Time(GetTime()).SpendsCoinbase(spendsCoinbase).SigOps(20).FromTx(tx));
    //     tx.vin[0].prevout.hash = hash;
    // }
    
    // ScoreCompare sc;
    // //without update, the fee delta is 0.
    // //so compare fee * size
    // CTxMemPool::txiter iter =  mempool.mapTx.begin()
    
    // for(CTxMemPool::txiter iter =  mempool.mapTx.begin(); iter != mempool.mapTx.end()-1; iter++){
    // 	//as each entry has the same fee = 1000000 and feedelta = 0, so ScoreCOmpare only compare size.
    // 	CTxMemPool::txiter nextIter = iter + 1;
    // 	if(iter->GetTxSize() > nextIter->GetTxSize()){
    // 		BOOST_CHECK(sc(iter, nextIter));
    // 	}
    // }


BOOST_AUTO_TEST_CASE(IncreamentExtraNonceTest){
    //to increase the nonce: sice nonce is changed, the hash of a block should be changed 
    
    //initialize a transaction
    //CScript scriptPubKeyIn = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    CMutableTransaction coinbaseTx;
    coinbaseTx.vin.resize(1);
    coinbaseTx.vin[0].prevout.SetNull();
    coinbaseTx.vout.resize(1);
    //coinbaseTx.vout[0].scriptPubKey = scriptPubKeyIn;
    CTransaction ctx = CTransaction(coinbaseTx);

    //initialize a new block
    CBlock block1 = CBlock();
    block1.vtx.push_back(ctx);
    CBlockIndex blockIndex = CBlockIndex(block1); //initilized block index: so height is 0.
    

    unsigned int extranonce = 1; // just a random number
    uint256 original_hash = block1.GetHash();
    std::cout << "original 64-hash of 256-hash: " << original_hash.GetCheapHash() << std::endl;
    //seg fault happened before, because there is no transaction in block.
    for(int i = 0; i < 100; i++){
        IncrementExtraNonce(&block1, &blockIndex, extranonce);
        uint256 updated_hash = block1.GetHash();
        std::cout << "new 64-hash of 256-hash: " << updated_hash.GetCheapHash() << std::endl;
        BOOST_CHECK(original_hash != updated_hash);
        original_hash = updated_hash;
    }

    // std::cout << "2nd extranonce is: " << extranonce <<std::endl;
    // IncrementExtraNonce(&block1, &blockIndex, extranonce);
    // uint256 updated_SecondHash = block1.GetHash();
    // std::cout << "3rd 64-hash of 256-hash: " << updated_SecondHash.GetCheapHash() << std::endl;
    // std::cout << "3rd extranonce is: " << extranonce <<std::endl;
}

BOOST_AUTO_TEST_CASE(ProcessBlockFoundTest){
    CMutableTransaction coinbaseTx;
    coinbaseTx.vin.resize(1);
    coinbaseTx.vin[0].prevout.SetNull();
    coinbaseTx.vout.resize(1);
    CTransaction ctx = CTransaction(coinbaseTx);

    CBlock block1 = CBlock();
    block1.vtx.push_back(ctx);
    CBlockIndex blockIndex = CBlockIndex(block1);

    //this function requires a block chain's info, so add block to chain
    //chainActive: global variable - chain
    block1.hashPrevBlock = chainActive.Tip()->GetBlockHash();

    const CChainParams& chainparams = Params(CBaseChainParams::MAIN);
    bool result = ProcessBlockFound(&block1, chainparams);
    std::cout << result <<std::endl;
}

BOOST_AUTO_TEST_SUITE_END()