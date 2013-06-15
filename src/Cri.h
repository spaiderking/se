//
// CRI.h
// BlindStorage
//

#ifndef __BlindStorage__CRI__
#define __BlindStorage__CRI__

#include <vector>
using std::vector;

#include "DataBlock.h"
#include "TBlock.h"
#include "CRIBlock.h"
#include "PRSubset.h"
#include "parameters.h"
#include "Debug.h"
#include "fileID.h"

class CRI{
private:
	higherfid_t higherfid;
	vector<CRIBlock> blocks;

public:
	CRI(higherfid_t higherfid);
	~CRI();

	bool isEmpty();
	void parseBytes(vector<byte> blocksBytes);
	void makeBytes(vector<byte> blocksBytes);
	void addFile(prSubsetSize_t size, prSubsetSeed_t seed, byte lowerFid[]);
	void search(fileID fid, CRIBlock& block);
	void search(byte lowerFid[], CRIBlock& block);
};

#endif /* defined(__BlindStorage__CRI__) */
