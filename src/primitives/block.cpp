// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <crypto/argon2.h>
#include <hash.h>
#include <tinyformat.h>
#include <utilstrencodings.h>
#include <crypto/common.h>

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetProofOfWorkHash() const
{
    uint256 hash;
    CARGON2 argon2;
    argon2.Write(UBEGIN(nVersion), UEND(nVersion) - UBEGIN(nVersion))
          .Write(UBEGIN(hashPrevBlock), UEND(hashPrevBlock) - UBEGIN(hashPrevBlock))
          .Write(UBEGIN(hashMerkleRoot), UEND(hashMerkleRoot) - UBEGIN(hashMerkleRoot))
          .Write(UBEGIN(nTime), UEND(nTime) - UBEGIN(nTime))
          .Write(UBEGIN(nBits), UEND(nBits) - UBEGIN(nBits))
          .Write(UBEGIN(nNonce), UEND(nNonce) - UBEGIN(nNonce))
          .Finalize(UBEGIN(hash));
    argon2.Reset();
    return hash;
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
