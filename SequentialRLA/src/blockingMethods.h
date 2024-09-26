#ifndef BLOCKING_H
#define BLOCKING_H

#include "utilities.h"

#include <functional>
#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <cstdlib>

using namespace std;

class BlockingMethods{
    public:
        bool isCompoundBlocking = false;
        int KMER = 0;
        int alphabetSize = 36;
        int blockingAttrIndex = 1;
        int groundTruthIndex;
        int prefixLen = 0;
        long long int blockIDRange = -1;
        long long int totalUniqueRecords;
        long long int totalCompRequired;
        vector<pair<long long int, int> > blockingIDList;
        vector<pair<long long int, int> > boundaryArr;
        vector<vector<string>> uniqueRecords;
        vector<string> uniqueAttrs;


        void setBlocking(int k, int alpha, int blockAttr, vector<vector<string>> &uRecords, int gtInd, int pLen, bool useCompoundBlocking){
            KMER = k;
            alphabetSize = alpha;
            blockingAttrIndex = blockAttr;
            uniqueRecords = uRecords;
            totalUniqueRecords = uRecords.size();
            prefixLen = pLen;
            isCompoundBlocking = useCompoundBlocking;
            groundTruthIndex = gtInd;
        }
        
        void setBlocking(int k, int alpha, int blockAttr, vector<vector<string>> &uRecords){
            KMER = k;
            alphabetSize = alpha;
            blockingAttrIndex = blockAttr;
            uniqueRecords = uRecords;
            totalUniqueRecords = uRecords.size();
        }

        void setBlocking(int k, int alpha, vector<string> &uAttrs){
            KMER = k;
            alphabetSize = alpha;
            blockingAttrIndex = -1;
            uniqueAttrs = uAttrs;
            totalUniqueRecords = uAttrs.size();
        }


        void getNormalBlockingIDArray() {
            string strSample(KMER, 'a');
            blockIDRange = pow(alphabetSize, KMER);
            long long int blockID;
            string blockingStr;
            for (int i = 0; i < totalUniqueRecords; i++) {
                if (isCompoundBlocking == false){
                    blockingStr = uniqueRecords[i][this->blockingAttrIndex];
                    while(blockingStr.size() < KMER) {                    
                        blockingStr += strSample.substr(0, KMER - blockingStr.length());
                    }
                } else {
                    blockingStr = generateCompoundBlockingString(uniqueRecords[i], blockingAttrIndex, prefixLen, groundTruthIndex);
                }
                int blkstrSize = blockingStr.size();
                for (int j = 0; j < blkstrSize; ++j) {
                    blockID = 0;
                    for (int k = 0; k < KMER; ++k)
                    {
                        int blockStrCharCode = (int)blockingStr[(j+k)%blkstrSize];
                        if ((blockStrCharCode >= 97) && (blockStrCharCode <= 122)) {
                            blockID += (blockStrCharCode - 97) * pow(alphabetSize,k);
                        } else if ((blockStrCharCode >= 48) && (blockStrCharCode <= 57)){
                            blockID += (blockStrCharCode - 48 + 26) * pow(alphabetSize,k);
                        }
                    }
                    pair<long long int,int> blkRecPair;
                    blkRecPair.first = blockID;
                    blkRecPair.second = i;
                    blockingIDList.push_back(blkRecPair);

                    // if(blockID<0 || blockID>=blockIDRange) {
                    //     cout<< "Invalid BlockID: " << blockID << endl;
                    //     cout<< "BlockingString: " << blockingStr << endl;
                    // }
                }	
            }
        }

        void getNormalBlockingIDArrayAttrs() {
            string strSample(KMER, 'a');
            blockIDRange = pow(alphabetSize, KMER);
            long long int blockID;
            string blockingStr;
            for (int i = 0; i < totalUniqueRecords; i++) {
                blockingStr = uniqueAttrs[i];
                if(blockingStr.size() == 0) {
                        continue;
                }
                while(blockingStr.size() < KMER) {                    
                    blockingStr += strSample.substr(0, KMER - blockingStr.length());
                }
                int blkstrSize = blockingStr.size();
                for (int j = 0; j < blkstrSize; ++j) {
                    blockID = 0;
                    for (int k = 0; k < KMER; ++k)
                    {
                        int blockStrCharCode = (int)blockingStr[(j+k)%blkstrSize];
                        if ((blockStrCharCode >= 97) && (blockStrCharCode <= 122)) {
                            blockID += (blockStrCharCode - 97) * pow(alphabetSize,k);
                        } else if ((blockStrCharCode >= 48) && (blockStrCharCode <= 57)){
                            blockID += (blockStrCharCode - 48 + 26) * pow(alphabetSize,k);
                        }
                    }
                    pair<long long int,int> blkRecPair;
                    blkRecPair.first = blockID;
                    blkRecPair.second = i;
                    blockingIDList.push_back(blkRecPair);

                    // if(blockID<0 || blockID>=blockIDRange) {
                    //     cout<< "Invalid BlockID: " << blockID << endl;
                    //     cout<< "BlockingString: " << blockingStr << endl;
                    // }
                }	
            }
        }

        void getSuperBlockingIDArray() {
            blockIDRange = pow(alphabetSize, KMER+1);
            string strSample(KMER, '0');
            long long int blockID;
            string blockingStr;
            long long int perAplhaBlocks = pow(alphabetSize, KMER);
            for (int i = 0; i < totalUniqueRecords; i++) {
                if (isCompoundBlocking == false){
                    blockingStr = uniqueRecords[i][blockingAttrIndex];
                    if(blockingStr.size() == 0) {
                        continue;
                    }
                    while(blockingStr.size() < KMER) {                    
                        blockingStr += strSample.substr(0, KMER - blockingStr.length());
                    }
                } else {
                    blockingStr = generateCompoundBlockingString(uniqueRecords[i], blockingAttrIndex, prefixLen, groundTruthIndex);
                }
                int blkstrSize = blockingStr.size();
                for (int j = 0; j < blkstrSize; ++j) {
                    blockID = 0;
                    for (int k = 0; k < KMER; ++k)
                    {
                        int blockStrCharCode = (int)blockingStr[(j+k)%blkstrSize];
                        if ((blockStrCharCode >= 97) && (blockStrCharCode <= 122)) {
                            blockID += (blockStrCharCode - 97) * pow(alphabetSize,k);
                        } else if ((blockStrCharCode >= 48) && (blockStrCharCode <= 57)){
                            blockID += (blockStrCharCode - 48 + 26) * pow(alphabetSize,k);
                        }
                    }

                    int blockStrCharCode = (int)blockingStr[0];

                    if ((blockStrCharCode >= 97) && (blockStrCharCode <= 122)) {
                        blockID += (blockStrCharCode-97)*perAplhaBlocks;
                    } else if ((blockStrCharCode >= 48) && (blockStrCharCode <= 57)){
                        blockID += (blockStrCharCode-48 + 26)*perAplhaBlocks;
                    }

                    pair<long long int,int> blkRecPair;
                    blkRecPair.first = blockID;
                    blkRecPair.second = i;
                    blockingIDList.push_back(blkRecPair);

                    // if(blockID<0 || blockID>=blockIDRange) {
                    //     cout<< "Invalid BlockID: " << blockID << endl;
                    //     cout<< "BlockingString: " << blockingStr << endl;
                    // }
                }	
            }
        }

        void getStagedSuperBlockingIDArray(int stage) {
            blockIDRange = pow(alphabetSize, KMER+stage);
            string strSample(KMER, '0');
            long long int blockID;
            string blockingStr;
            long long int perAplhaBlocks = pow(alphabetSize, KMER);
            for (int i = 0; i < totalUniqueRecords; i++) {
                if (isCompoundBlocking == false){
                    blockingStr = uniqueRecords[i][blockingAttrIndex];
                    if(blockingStr.size() == 0) {
                        continue;
                    }
                    while(blockingStr.size() < KMER) {                    
                        blockingStr += strSample.substr(0, KMER - blockingStr.length());
                    }
                } else {
                    blockingStr = generateCompoundBlockingString(uniqueRecords[i], blockingAttrIndex, prefixLen, groundTruthIndex);
                }
                int blkstrSize = blockingStr.size();
                for (int j = 0; j < blkstrSize; ++j) {
                    blockID = 0;
                    for (int k = 0; k < KMER; ++k)
                    {
                        int blockStrCharCode = (int)blockingStr[(j+k)%blkstrSize];
                        if ((blockStrCharCode >= 97) && (blockStrCharCode <= 122)) {
                            blockID += (blockStrCharCode - 97) * pow(alphabetSize,k);
                        } else if ((blockStrCharCode >= 48) && (blockStrCharCode <= 57)){
                            blockID += (blockStrCharCode - 48 + 26) * pow(alphabetSize,k);
                        }
                    }

                    for(int ind = 0; ind<stage; ind++){
                        int blockStrCharCode = (int)blockingStr[stage-ind-1];

                        if ((blockStrCharCode >= 97) && (blockStrCharCode <= 122)) {
                            blockID += (blockStrCharCode-97)* pow(alphabetSize, KMER + ind);
                        } else if ((blockStrCharCode >= 48) && (blockStrCharCode <= 57)){
                            blockID += (blockStrCharCode-48 + 26)* pow(alphabetSize, KMER + ind);
                        }
                    }

                    pair<long long int,int> blkRecPair;
                    blkRecPair.first = blockID;
                    blkRecPair.second = i;
                    blockingIDList.push_back(blkRecPair);

                    // if(blockID<0 || blockID>=blockIDRange) {
                    //     cout<< "Invalid BlockID: " << blockID << endl;
                    //     cout<< "BlockingString: " << blockingStr << endl;
                    // }
                }	
            }
        }

        void sortBlockingIDArray() {
            blockIDRange = pow(alphabetSize, KMER+1);
            long long int numRecords = blockingIDList.size();
            vector<pair<long long int, int>> tempArr(numRecords);
            vector<long long int> countArr(blockIDRange, 0);
            for (int j = 0; j < numRecords; ++j) {
                if(((blockingIDList[j].first) < 0) || (blockingIDList[j].first >=blockIDRange )){
                    cout<< "Invalid BlockID: " << blockingIDList[j].first << endl;
                    cout<< "Blocking ID Range: " << blockIDRange << endl;
                } 
                countArr[blockingIDList[j].first]++;
            }

            // Do prefix sum
            for (long long int k = 1; k < blockIDRange; ++k)
                countArr[k]	+= countArr[k - 1];

            for (int j = numRecords - 1; j >= 0; --j)
                tempArr[--countArr[blockingIDList[j].first]] = blockingIDList[j];

            for (int j = 0; j < numRecords; ++j)
                blockingIDList[j] = tempArr[j];
        }

        void sortBlockingIDArray(int stage) {
            blockIDRange = pow(alphabetSize, KMER+stage);
            long long int numRecords = blockingIDList.size();
            sort(blockingIDList.begin(), blockingIDList.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
                return a.first < b.first;
            });
        }

        void removeRedundentBlockingID() {
            int numRecords = blockingIDList.size();
            vector<pair<long long int, int>> tempArr;
            long long int totalUniqueBlocks = 1;
            tempArr.push_back(blockingIDList[0]);
            for (int i = 1; i<numRecords; i++) {
                if (blockingIDList[i].first != blockingIDList[i-1].first) {
                    totalUniqueBlocks++;
                }
                if ( ! ((blockingIDList[i].first == blockingIDList[i-1].first) && (blockingIDList[i].second == blockingIDList[i-1].second))) {
                    tempArr.push_back(blockingIDList[i]);
                }
            }
            blockingIDList = tempArr;

            cout<< "Total Unique Blocks: "<< totalUniqueBlocks << endl;
            cout<< "Total unique block-rec pairs: " << numRecords << endl;
            cout<< "Removed redundant block-rec pairs: " << numRecords - blockingIDList.size() << endl; 
        }

        void findBlockBoundaries() {
            //just keep starting ind and range.
            // boundaryArr.resize(totalUniqueBlocks);
            int numRecords = blockingIDList.size();
            totalCompRequired = 0;
            long long int startInd = 0;
            int range = 0;
            int curBlockInd = 0;
            for (long long int i = 1; i<numRecords; i++) {
                if (blockingIDList[i].first != blockingIDList[i-1].first) {
                    range = i-startInd;
                    pair<long long int,int> p;
                    p.first = startInd;
                    p.second = range;
                    boundaryArr.push_back(p);
                    long long int lRange = (long long int) range;
                    totalCompRequired += ceil((lRange*(lRange-1))/(long long int)2);
                    curBlockInd++;
                    startInd = i;
                }
            }
            // Enter last Block info
            range = numRecords-startInd;
            long long int lRange = (long long int) range;
            totalCompRequired += ceil((lRange*(lRange-1))/(long long int)2);
            pair<long long int,int> p;
            p.first = startInd;
            p.second = range;
            boundaryArr.push_back(p);
            cout<< "Total Comp Required: " << totalCompRequired << endl;
            cout<< "Total Block Boundaries found: " << boundaryArr.size() << endl;
        }

        void sortBlockingBoundaryArr(){
            sort(boundaryArr.begin(), boundaryArr.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
                return a.second > b.second;
            });
        }

        void findBlockAssignments(int numCores, int numThreads, int compDividerMultiplier, vector<vector<pair<long long int, int> > > &assignedRecordDomain, vector<vector<pair<long long int, int> > > &assignedRecordRange) {
            int totalChunks = numCores*numThreads*compDividerMultiplier;
            assignedRecordDomain.resize(totalChunks);
            assignedRecordRange.resize(totalChunks);

            long long int compThreshold = ceil(totalCompRequired/totalChunks);
            long long int curAssignmentSize = 0;
            int recDomainEndInd = 0;
            int remRecCount = 0;
            int curChunk = 0;

            for (int j = 0; j < boundaryArr.size(); j++)
            {
                int range = boundaryArr[j].second;

                long long int curBlockSize = ceil((range*(range-1))/2);

                // assign all remaining comparisions to the last core
                if(curChunk == totalChunks - 1) {
                    compThreshold = LLONG_MAX;
                }

                if ((curAssignmentSize+curBlockSize) < compThreshold) {
                    assignedRecordDomain[curChunk].push_back(boundaryArr[j]);
                    assignedRecordRange[curChunk].push_back(boundaryArr[j]);
                    curAssignmentSize = curAssignmentSize + curBlockSize;
                } else {
                    remRecCount = boundaryArr[j].second;
                    while(curAssignmentSize + remRecCount < compThreshold){
                        recDomainEndInd++;
                        curAssignmentSize+=remRecCount-1;
                        remRecCount--;
                    }
                    if(recDomainEndInd != 0){
                        pair<long long int,int> dom;
                        dom.first = boundaryArr[j].first;
                        dom.second = recDomainEndInd;
                        assignedRecordDomain[curChunk].push_back(dom);
                        assignedRecordRange[curChunk].push_back(boundaryArr[j]);
                        pair<long long int, int> newBound;
                        newBound.first = boundaryArr[j].first+recDomainEndInd;
                        newBound.second = boundaryArr[j].second-recDomainEndInd;
                        boundaryArr.push_back(newBound);
                        recDomainEndInd = 0;
                        remRecCount = 0;
                    } else {
                        j--;
                    }
                    curChunk++;
                    // cout<< "Assigned Chunk: " << curChunk << endl;
                    curAssignmentSize = 0;
                }
            }
        }

        void findBlockAssignments(int totalChunks, vector<vector<pair<long long int, int> > > &assignedRecordDomain, vector<vector<pair<long long int, int> > > &assignedRecordRange) {
            assignedRecordDomain.resize(totalChunks);
            assignedRecordRange.resize(totalChunks);
            std::sort(boundaryArr.begin(), boundaryArr.end(), [](auto &left, auto &right) {
                return left.second > right.second;
            });

            long long int compThreshold = ceil(totalCompRequired/(long long int)totalChunks);
            cout<< "Per chunk threshold: " << compThreshold << endl;
            long long int curAssignmentSize = 0;
            int curChunk = 0;

            for (int j = 0; j < boundaryArr.size(); j++)
            {
                // cout<< "BoundarySize: " << boundaryArr.size() << " and j: " << j << endl;
                long long int range = boundaryArr[j].second;

                long long int curBlockSize = ceil((range*(range-1))/(long long int)2);

                // assign all remaining comparisions to the last core
                if(curChunk == totalChunks - 1) {
                    compThreshold = LLONG_MAX;
                }

                if ((curAssignmentSize+curBlockSize) < compThreshold) {
                    assignedRecordDomain[curChunk].push_back(boundaryArr[j]);
                    assignedRecordRange[curChunk].push_back(boundaryArr[j]);
                    curAssignmentSize = curAssignmentSize + curBlockSize;
                    continue;
                } else {
                    long long int remRecCount = boundaryArr[j].second;
                    long long int recDomainEndInd = 0;
                    while(curAssignmentSize + remRecCount - 1 < compThreshold){
                        recDomainEndInd++;
                        curAssignmentSize+=remRecCount-1;
                        remRecCount--;
                    }
                    if(recDomainEndInd != 0){
                        pair<long long int,int> dom;
                        dom.first = boundaryArr[j].first;
                        dom.second = recDomainEndInd;
                        assignedRecordDomain[curChunk].push_back(dom);
                        assignedRecordRange[curChunk].push_back(boundaryArr[j]);
                        pair<long long int, int> newBound;
                        newBound.first = boundaryArr[j].first+recDomainEndInd;
                        newBound.second = boundaryArr[j].second-recDomainEndInd;
                        boundaryArr.push_back(newBound);

                        recDomainEndInd = 0;
                        remRecCount = 0;
                        long long int com_T = ceil((boundaryArr[j].second*(boundaryArr[j].second-1))/2);
                        long long int new1_T = ceil((dom.second*(dom.second-1))/2) + (dom.second* (boundaryArr[j].second - dom.second));
                        long long int new2_T = ceil((newBound.second*(newBound.second-1))/2);
                    } else {
                        j--;
                    }
                    curChunk++;
                    curAssignmentSize = 0;
                }
            }
        }
};

#endif


