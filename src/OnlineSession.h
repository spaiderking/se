//
//  OnlineSession.h
//  BlindStorage
//

#ifndef __BlindStorage__Dfile__
#define __BlindStorage__Dfile__

#include <stdint.h>
#include <iostream>
#include "Communicator.h"
#include "PRSubset.h"
#include "Ddisk.h"
#include "PRF.h"
#include "TBlock.h"
#include "Tdisk.h"
#include "Debug.h"
#include "Cri.h"
#include "DiskCommunicator.h"

class OnlineSession {
private:
	DiskCommunicator dcomm;
	string filename;
	fileID fid;
	TBlock tBlock;
	PRSubset criPRSubset;
	PRSubset filePRSubset;
	CRIBlock criBlock;
	CRI cri;
	b_index_t numBlocks;
	b_index_t numFileBlocks;
	vector<DataBlock> blocks;
	vector<DataBlock> fileBlocks;
	vector<DataBlock> criBlocks;
	uint32_t criBlockIndex;
	
	void readT(t_index_t TRecordIndex, byte block[]);
	void writeT(t_index_t TRecordIndex, byte block[]);

	void readD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]);
	void writeD(b_index_t blockIndices[], b_index_t numBlocks, byte blocks[]);

	void readCRI(PRSubset& prSubset, CRI& cri);

public:
	OnlineSession();
	~OnlineSession();
	int64_t read(string filename, byte*& file, b_index_t numBlocksToRead = 0);
	void write(byte contents[], size_t size);
	void remove(string filename);
	/*! Use homorphic encryption to support this */
	void rename();
};

#endif /* defined(__BlindStorage__Dfile__) */
