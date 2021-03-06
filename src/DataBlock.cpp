//
//  DataBlock.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "DataBlock.h"

byte DataBlock::key[AES_KEY_SIZE] = {0};
b_index_t DataBlock::instanceCounter = 0;

//This constructor should only be used in preprocessing.
DataBlock::DataBlock(){ 
	version = 0;
	index = instanceCounter;
	instanceCounter++;
	memset(fidBytes, 0, FILEID_SIZE);
	memset(block, 0, BLOCK_SIZE);
	dataSize = 0;
	isBlockEncrypted = false;
}

DataBlock::DataBlock(b_index_t index){
	version = 0;
	this->index = index;
	memset(fidBytes, 0, FILEID_SIZE);
	memset(block, 0, BLOCK_SIZE);
	dataSize = 0;
	isBlockEncrypted = false;
}

DataBlock::~DataBlock(){
}

/* This function should be run before doing anything. Otherwise 0 key will be used.*/
void DataBlock::setKey(byte key[]){
	memcpy(this->key, key, AES_KEY_SIZE);
}

void DataBlock::make(fileID fid, byte block[], dataSize_t dataSize, version_t version) {
	this->fid = fid;
	this->fid.get(fidBytes);
	this->version = version;
	this->dataSize = dataSize;
	memcpy(this->block, block, dataSize);
	addTrailer();
	encrypt();
}

void DataBlock::addTrailer(){
	if(dataSize < MAX_BLOCK_DATA_SIZE)
		addPadding();
	else
		block[PADBYTE_LOC] = 1;

	fid.get(fidBytes);
	memcpy(&block[FILEID_LOC], fidBytes, FILEID_SIZE);
	memcpy(&block[VERSION_LOC], reinterpret_cast<byte*>(&version), sizeof(version_t));
}

void DataBlock::addPadding(){
	block[dataSize] = 1;
}

void DataBlock::removePadding(){
	for(int i = MAX_BLOCK_DATA_SIZE; i > 0; i--){
		if(block[i] == 1){
			dataSize = i;
			break;
		}
	}
}

void DataBlock::parse(byte block[]){
	memcpy(this->block, block, BLOCK_SIZE);
//	this->block = block;
	version = *(version_t*)((this->block)+VERSION_LOC);
	isBlockEncrypted = true;

	decrypt();

	memcpy(fidBytes, (this->block)+FILEID_LOC, FILEID_SIZE);
	fileID fid(fidBytes);
	this->fid = fid;

	removePadding(); //This will populate dataSize
}

void DataBlock::update(fileID fid, byte block[], dataSize_t dataSize){
	version++;
	make(fid, block, dataSize, version);
	memset(this->block+dataSize+1, 0, MAX_BLOCK_DATA_SIZE-dataSize); //Zeros the bytes of the previous block
}

void DataBlock::updateVersion(){
	version++;
	memcpy(&block[VERSION_LOC], static_cast<byte*>(static_cast<void*>(&version)), sizeof(version_t));
	encrypt();
}

void DataBlock::clear(){
/* This function needs to be followed by updateVersion */	
	byte zerofid[FILEID_SIZE] = {0};
	fileID fid(zerofid);
	this->fid = fid;
	this->fid.get(fidBytes);
	dataSize = 0;
	memset(block, 0, BLOCK_SIZE-sizeof(version_t));
//	memcpy(&block[VERSION_LOC], static_cast<byte*>(static_cast<void*>(&version)), sizeof(version_t));
//	encrypt();
	
}

void DataBlock::encrypt(){
	AES cipher;
	makeIV();
//	byte dummyblock[BLOCK_SIZE-sizeof(version_t)];
//	cipher.ENC_CTR(dummyblock, dummyblock, BLOCK_SIZE-sizeof(version_t), key, iv);
	cipher.ENC_CTR(block, block, BLOCK_SIZE-sizeof(version_t), key, iv);
	isBlockEncrypted = true;

}

void DataBlock::decrypt(){
	AES cipher;
	makeIV();
//	byte dummyblock[BLOCK_SIZE-sizeof(version_t)];
//	cipher.DEC_CTR(dummyblock, dummyblock, BLOCK_SIZE-sizeof(version_t), key, iv);
	cipher.DEC_CTR(block, block, BLOCK_SIZE-sizeof(version_t), key, iv);
	isBlockEncrypted = false;

}

void DataBlock::makeIV(){
	memcpy(iv, static_cast<byte*>(static_cast<void*>(&index)), sizeof(b_index_t));
	memcpy(iv+sizeof(b_index_t), static_cast<byte*>(static_cast<void*>(&version)), sizeof(version_t));
	memset(iv+sizeof(b_index_t)+sizeof(version_t), 0, AES_BLOCK_SIZE-sizeof(b_index_t)-sizeof(version_t));
}

dataSize_t DataBlock::getDataSize(){
	return dataSize;
}

void DataBlock::getEncrypted(byte block[]){
	if(!isBlockEncrypted)
		encrypt();
	memcpy(block, this->block, BLOCK_SIZE);
//	block = this->block;
}

void DataBlock::getDecrypted(byte block[]){
	if(isBlockEncrypted)
		decrypt();
	memcpy(block, this->block, BLOCK_SIZE);
//	block = this->block;
}

void DataBlock::getDecryptedData(byte data[]){
	if(isBlockEncrypted)
		decrypt();
	memcpy(data, this->block, dataSize);
}

bool DataBlock::isOccupied(){
//	higherfid_t higherFid = *(higherfid_t*)(fidBytes);
//	return higherFid ? true : false;
	return *(uint64_t*)(fidBytes);
}

bool DataBlock::fidMatchCheck(fileID& fid){
	byte thisfidBytes[FILEID_SIZE];
	byte fidBytes[FILEID_SIZE];

	this->fid.get(thisfidBytes);
	fid.get(fidBytes);

	return !memcmp(thisfidBytes, fidBytes, FILEID_SIZE);
}

void DataBlock::encryptIfEmpty(){
	if(!isOccupied()){
		encrypt();		
	}
}
