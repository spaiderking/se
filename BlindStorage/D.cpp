////
////  D.cpp
////  BlindStorage
////
////  Created by Muhammad Naveed on 3/21/13.
////  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
////
//
//#include "D.h"
//#include "parameters.h"
//
//D::D(uint32_t blockIndex, uint32_t fid, unsigned char* rawData, uint32_t size){
//    blocks = new DataBlock*[TOTAL_BLOCKS];
//    for(int i = 0; i < TOTAL_BLOCKS; i++){
//        blocks[i] = new DataBlock(blockIndex, fid, rawData, size);
//    }
//}
//
//D::~D(){
//    delete[] blocks;
//}
//
//void D::add(uint32_t blockIndex, uint32_t fid, unsigned char* rawData, uint32_t size){
//    blocks[blockIndex]->add(blockIndex, fid, rawData, size);
//}
//
//unsigned char* D::get(uint32_t blockIndex){
//    return blocks[blockIndex]->get();
//}
//
//void D::remove(uint32_t blockIndex){
//    for(int i = blockIndex; i < TOTAL_BLOCKS - 1; i++)
//        blocks[i] = blocks[i+1];
//}
//
//void D::remove(uint32_t blocksIndicesToRemove[], uint32_t size){
//    std::sort(blocksIndicesToRemove, blocksIndicesToRemove+size);
//    uint32_t next = 0;
//    for(int i = blocksIndicesToRemove[0]; i < TOTAL_BLOCKS-1; i++){
//        if(i == blocksIndicesToRemove[i])
//            next = blocksIndicesToRemove[i];
//        blocks[i] = blocks[++next];
//    }
//}
//
//void D::encryptUnusedBlocks(){
//    for(int i = 0; i < TOTAL_BLOCKS; i++)
//        if(!blocks[i]->isOccupied())
//            blocks[i]->ENC();
//}
