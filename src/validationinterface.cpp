// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2017 The PIVX developers
// Copyright (c) 2018 The ojacoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "validationinterface.h"

static CMainSignals g_signals;

CMainSignals& GetMainSignals()
{
    return g_signals;
}

void Registervalidationinterface(Cvalidationinterface* pwalletIn) {
// XX42 g_signals.EraseTransaction.connect(boost::bind(&Cvalidationinterface::EraseFromWallet, pwalletIn, _1));
    g_signals.UpdatedBlockTip.connect(boost::bind(&Cvalidationinterface::UpdatedBlockTip, pwalletIn, _1));
    g_signals.SyncTransaction.connect(boost::bind(&Cvalidationinterface::SyncTransaction, pwalletIn, _1, _2));
    g_signals.NotifyTransactionLock.connect(boost::bind(&Cvalidationinterface::NotifyTransactionLock, pwalletIn, _1));
    g_signals.UpdatedTransaction.connect(boost::bind(&Cvalidationinterface::UpdatedTransaction, pwalletIn, _1));
    g_signals.SetBestChain.connect(boost::bind(&Cvalidationinterface::SetBestChain, pwalletIn, _1));
    g_signals.Inventory.connect(boost::bind(&Cvalidationinterface::Inventory, pwalletIn, _1));
    g_signals.Broadcast.connect(boost::bind(&Cvalidationinterface::ResendWalletTransactions, pwalletIn));
    g_signals.BlockChecked.connect(boost::bind(&Cvalidationinterface::BlockChecked, pwalletIn, _1, _2));
// XX42    g_signals.ScriptForMining.connect(boost::bind(&Cvalidationinterface::GetScriptForMining, pwalletIn, _1));
    g_signals.BlockFound.connect(boost::bind(&Cvalidationinterface::ResetRequestCount, pwalletIn, _1));
}

void Unregistervalidationinterface(Cvalidationinterface* pwalletIn) {
    g_signals.BlockFound.disconnect(boost::bind(&Cvalidationinterface::ResetRequestCount, pwalletIn, _1));
// XX42    g_signals.ScriptForMining.disconnect(boost::bind(&Cvalidationinterface::GetScriptForMining, pwalletIn, _1));
    g_signals.BlockChecked.disconnect(boost::bind(&Cvalidationinterface::BlockChecked, pwalletIn, _1, _2));
    g_signals.Broadcast.disconnect(boost::bind(&Cvalidationinterface::ResendWalletTransactions, pwalletIn));
    g_signals.Inventory.disconnect(boost::bind(&Cvalidationinterface::Inventory, pwalletIn, _1));
    g_signals.SetBestChain.disconnect(boost::bind(&Cvalidationinterface::SetBestChain, pwalletIn, _1));
    g_signals.UpdatedTransaction.disconnect(boost::bind(&Cvalidationinterface::UpdatedTransaction, pwalletIn, _1));
    g_signals.NotifyTransactionLock.disconnect(boost::bind(&Cvalidationinterface::NotifyTransactionLock, pwalletIn, _1));
    g_signals.SyncTransaction.disconnect(boost::bind(&Cvalidationinterface::SyncTransaction, pwalletIn, _1, _2));
    g_signals.UpdatedBlockTip.disconnect(boost::bind(&Cvalidationinterface::UpdatedBlockTip, pwalletIn, _1));
// XX42    g_signals.EraseTransaction.disconnect(boost::bind(&Cvalidationinterface::EraseFromWallet, pwalletIn, _1));
}

void UnregisterAllvalidationinterfaces() {
    g_signals.BlockFound.disconnect_all_slots();
// XX42    g_signals.ScriptForMining.disconnect_all_slots();
    g_signals.BlockChecked.disconnect_all_slots();
    g_signals.Broadcast.disconnect_all_slots();
    g_signals.Inventory.disconnect_all_slots();
    g_signals.SetBestChain.disconnect_all_slots();
    g_signals.UpdatedTransaction.disconnect_all_slots();
    g_signals.NotifyTransactionLock.disconnect_all_slots();
    g_signals.SyncTransaction.disconnect_all_slots();
    g_signals.UpdatedBlockTip.disconnect_all_slots();
// XX42    g_signals.EraseTransaction.disconnect_all_slots();
}

void SyncWithWallets(const CTransaction &tx, const CBlock *pblock = NULL) {
    g_signals.SyncTransaction(tx, pblock);
}
