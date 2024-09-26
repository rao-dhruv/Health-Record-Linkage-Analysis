// Sequential Record Linkage

#include "unionFind.h"
#include "recordComparator.h"
#include "blockingMethods.h"
#include "utilities.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <ctime> 
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <random>
#include <time.h>
#include <tuple>
#include <unistd.h>
#include <utility>
#include <vector>

#define SAMPLE_STRING_SIZE 100

using namespace std;
using namespace boost;


int alphabetSize = 36;
int attributes;
int blockingAttr;
int groundTruthInd;
int lenMax;
int kmer;
int totalRecords;
int totalUniqueRecords;
int totalUniqueBlocks;

long long int totalCompRequired = 0;
long long int totalEdges = 0;

int* uniqueRecordIDs;

map<int, vector<int> > approxConnectedComponents;

vector<pair<int, string> > combinedData;
vector<vector<int> > exactmatches;
vector<vector<int> > finalConnectedComponents;
vector<vector<string>> uniqueRecords;
vector<vector<string> > vec2D;


UnionFind uf;
RecordComparator recCom;


void getUniqueEntries() {
	uniqueRecordIDs = new int[totalUniqueRecords];
	uniqueRecords.resize(totalUniqueRecords);
	for(size_t i=0; i<totalUniqueRecords; i++){
		uniqueRecords[i].resize(attributes);
	}

	for (size_t i = 0; i < totalUniqueRecords; i++) {
        uniqueRecordIDs[i] = exactmatches[i][0];
		for(size_t j=0; j<attributes; j++){
			uniqueRecords[i][j] = vec2D[exactmatches[i][0]][j];
		}
    }
}


void processAllBlocks(BlockingMethods &bm){
	for(int i=0; i<bm.boundaryArr.size(); i++){
		int start = bm.boundaryArr[i].first;
		int range = bm.boundaryArr[i].second;
		for(int j=start; j<start+range-1; j++){
			for(int k=j+1; k<start+range; k++){
				int recid_j = bm.blockingIDList[j].second;
				int recid_k = bm.blockingIDList[k].second;
				if(!uf.isConnected(recid_j, recid_k)){
				 	if(recCom.isLinkageOk(uniqueRecords[recid_j], uniqueRecords[recid_k])) {
						uf.weightedUnion(recid_j, recid_k);
				 		totalEdges++;
				 	}
				}
			}
		}
	}
	cout<< "Total Edge found: " << totalEdges << endl;
}


void mergeEdges(UnionFind &ufTemp) {
	for(int i=0; i< totalUniqueRecords; i++) {
		int root_i = uf.find(i);
		int root_mt = ufTemp.find(i);
		if( root_i != root_mt){
			uf.weightedUnion(root_i, root_mt);
		}
	}
}


void getBlockingKeyData(vector<pair<int, string> > &blockingKeyData) {
	string strSample(SAMPLE_STRING_SIZE, '0');
	int max = 0;
	for (int i = 0; i< totalUniqueRecords; i++) {
		pair<int, string> p;
		p.first = i;
		p.second = uniqueRecords[i][blockingAttr];
		blockingKeyData[i] = p;
		if (max<blockingKeyData[i].second.size()) {
			max = blockingKeyData[i].second.size();
		}
	}
	lenMax = max;
	// Padding to make all characters same size
    for (int i = 0; i < totalUniqueRecords; ++i) {
		int lenDiff		= lenMax - blockingKeyData[i].second.length();
		if(lenDiff > 0)
			blockingKeyData[i].second	+= strSample.substr(0, lenDiff);
	}
}


void doSortedComp() {
	clock_t currTS_f0	= clock();
	vector<pair<int,string> > headlessCopies;
	headlessCopies.resize(2*totalUniqueRecords);
	getBlockingKeyData(headlessCopies);
	clock_t currTS_f1	= clock();
	for(int i=0; i< uniqueRecords.size(); i++) {
		headlessCopies[totalUniqueRecords+i].first = i;
		headlessCopies[totalUniqueRecords+i].second = headlessCopies[i].second.substr(1,lenMax) + '0';
	}
	clock_t currTS_f2	= clock();

	radixSort_std(lenMax, headlessCopies);

	clock_t currTS_f3	= clock();
	int extraEdges = 0;
	for (int i = 0; i < headlessCopies.size()-1; i++) {
		int j = 1;
		while (headlessCopies[i].second.compare(headlessCopies[i+j].second) == 0) {
			int recID_i = headlessCopies[i].first;
            int recID_j = headlessCopies[i+j].first;
            if (!uf.isConnected(recID_i, recID_j)) {
                if(recCom.isLinkageOk(uniqueRecords[recID_i], uniqueRecords[recID_j])) {
					uf.weightedUnion(recID_i, recID_j);
					extraEdges++;
					totalEdges++;
				}
			}
			j++;
			if(i+j >= headlessCopies.size()){
				break;
			}
		}
	}
	clock_t currTS_f4	= clock();

	double dataRead_t	= (double)(currTS_f1 - currTS_f0) / CLOCKS_PER_SEC;
	double getHeadLessCp= (double)(currTS_f2 - currTS_f1) / CLOCKS_PER_SEC;
	double sorting_t	= (double)(currTS_f3 - currTS_f2) / CLOCKS_PER_SEC;
	double comp_t		= (double)(currTS_f4 - currTS_f3) / CLOCKS_PER_SEC;

	cout<< "DataRead time: " << dataRead_t << endl;
	cout<< "getHeadLessCp time: " << getHeadLessCp << endl;
	cout<< "sorting_t time: " << sorting_t << endl;
	cout<< "comp_t time: " << comp_t << endl;
	cout<< "LenMax: " << lenMax << endl; 
	cout<< "Edges Added by Sorting: "<< extraEdges << endl;
}


void findConnComp()
{
	int root, edgeTotal;

    for (int i = 0; i < totalUniqueRecords; i++)
	{
		root = uf.find(i);
		approxConnectedComponents[root].push_back(i);
	}

    cout<< " Single Linkage Connected Components: " << approxConnectedComponents.size()<<endl;
}


void findFinalConnectedComp() {
    int totalNodes = 0;
    int pairsAccessed = 0;
    for (auto const& p : approxConnectedComponents) {
        pairsAccessed++;
        int numComponents = p.second.size();
		totalNodes+=numComponents;
        bool distmat[numComponents][numComponents];
        vector<vector<string>> dataArr(numComponents); // to make cache-efficient, keep records in a row

		// Copy Data in cluster
        for(int c=0; c<p.second.size(); c++) {
            dataArr[c] = uniqueRecords[p.second[c]];
        };

		// generate a 2D matrix filled with all pair comparision results
        for (int i =0; i<numComponents; i++) {
            distmat[i][i] = true;
            for (int j = i+1; j < numComponents; j++)
            {
                if (recCom.isLinkageOk(dataArr[i], dataArr[j])) {
                    distmat[i][j] = true;
                    distmat[j][i] = true;
                } else {
                    distmat[i][j] = false;
                    distmat[j][i] = false;
                }
            }
        }

        bool nodesConsidered[numComponents];
        for(int i=0; i<numComponents; i++) {
            nodesConsidered[i] = false;
        }

        for(int i=0; i<numComponents; i++) {
            if(nodesConsidered[i] == false) {
                vector<int> connectedComponent;
                connectedComponent.push_back(p.second[i]);
                nodesConsidered[i] = true;
                for(int j=0; j<numComponents; j++) {
                    if ((distmat[i][j] == true) && (nodesConsidered[j]==false)) {
                        connectedComponent.push_back(p.second[j]);
                        nodesConsidered[j] = true;
                    }
                }
                finalConnectedComponents.push_back(connectedComponent);
            }
        }
    }
    // cout<< "Total Nodes: "<< totalNodes << " unique records: " << totalUniqueRecords << endl;
    cout<< "Complete Linkage Connected Components: "<< finalConnectedComponents.size()<<endl;
}


void writeFinalConnectedComponentToFile(string& result_file_name_groundTruths, string &result_file_name_recInds, int groundTruthInd) {
	ofstream out_file;
    out_file.open(result_file_name_groundTruths);

	for(int i = 0; i< finalConnectedComponents.size(); i++) {
        for(int j = 0; j< finalConnectedComponents[i].size(); j++) {
            for(int k=0; k< exactmatches[finalConnectedComponents[i][j]].size(); k++) {
                out_file << uniqueRecords[finalConnectedComponents[i][j]][groundTruthInd] << ",";
			}
		}
        out_file<< "\n";
	}
	out_file.close();

	out_file.open(result_file_name_recInds);

	for(int i = 0; i< finalConnectedComponents.size(); i++) {
        for(int j = 0; j< finalConnectedComponents[i].size(); j++) {
            for(int k=0; k< exactmatches[finalConnectedComponents[i][j]].size(); k++) {
                out_file << exactmatches[finalConnectedComponents[i][j]][k] << ",";
			}
		}
        out_file<< "\n";
	}
	out_file.close();
}


int main(int argc, char** argv) {
	// Start timing from here
	clock_t currTS_p0	= clock();

	// Set the path to the data files
	string filePath = "./data/";
	// string filePath = "/home/joyanta/Documents/Research/Record_Linkage/codes/my_codes/ds_single_datasets/";


	// Set attributes to be compared & corresponding distance thresholds
	vector<int> comparingAttrs{1,2,3,4}; // Example: LN,FN,DOD,DOB

	// vector<int> comparingAttrs{0, 2, 3} // Example: Title, Manufacturer, Price

	vector<int> attrThresholds{1,1,1,1};

	// Set total cumulative largest distance threshold
	int global_t = 1; 	// Set this a large number if you don't want to use this feature 

	if(comparingAttrs.size() != attrThresholds.size()){
		cout<< "ERROR: Comparing Attributes and Set Thresholds Array Size mismatch" << endl;
		return -1; 
	}

	// Add columns that should be excluded from processing
	vector<int> excludedColumns{5,6,7,8,9,10,11};  	// Ex. exclude Description column

	// Set the Ground truth index
	groundTruthInd = 0;


	// Set the Blocking key index
	blockingAttr = 1; //LN

	// Set Blocking Kmer
	kmer = 3;


	/** 
	 * 	Sort the exclusion list and update new index for 
	 * groundtruth, blocking attr, & comparing attributes
	**/

	sort(excludedColumns.begin(), excludedColumns.end());
	sort(comparingAttrs.begin(), comparingAttrs.end());

	vector<int> diffCount(comparingAttrs.size(), 0);

	int newGroundTruthInd = groundTruthInd;
	int newBlockingAttr = blockingAttr;

	for(int i=0; i<excludedColumns.size(); i++){
		if(excludedColumns[i] < blockingAttr){
			newBlockingAttr--;
		}
		if(excludedColumns[i] < groundTruthInd){
			newGroundTruthInd--;
		}
		for(int j=0; j<comparingAttrs.size(); j++){
			if(excludedColumns[i] < comparingAttrs[j]){
				diffCount[j]++;
			} else if (excludedColumns[i] == comparingAttrs[j]) {
				cout<< "Error: Comparing attr : " << comparingAttrs[j] << " Appears in exclusion list" << endl;
				return -1;
			}
		}

	}
	blockingAttr = newBlockingAttr;
	groundTruthInd = newGroundTruthInd;
	
	cout<< "Updated Blocking ATTR: " << blockingAttr << endl;
	cout<< "Updated ground truth: " << groundTruthInd << endl;

	for(int i=0; i<comparingAttrs.size(); i++){
		cout<< "Comparing attribute: " << comparingAttrs[i] << " has new index: " << comparingAttrs[i] - diffCount[i] << endl;
		comparingAttrs[i] -= diffCount[i];
	}


	// Read records from the input files
	vector<string> files;
	if (argc < 2){
		cout << "No input file Specified!" << endl;
		return -1;
	}

	for (int i = 1; i < argc; i++)
	{
		string fPath = filePath + argv[i];
		files.push_back(fPath);
	}
	getFormattedDataFromCSV(files, vec2D, attributes, totalRecords, excludedColumns);

	double dataRead_t	= (double)(clock() - currTS_p0) / CLOCKS_PER_SEC;
    cout<< "DataRead time: "<< dataRead_t << endl;
	cout<< "Total Records: " << vec2D.size() << endl;

	clock_t currTS_p1	= clock();

	lenMax = 0;
	totalUniqueRecords = 0;
	getCombinedData(vec2D, combinedData, lenMax);
	double comb_t	= (double)(clock() - currTS_p1) / CLOCKS_PER_SEC;
	cout<< "Combined Data time: "<< comb_t << endl;

	cout<<"LenMax: " << lenMax << endl;
	clock_t currTS_p2	= clock();
	radixSort_std(lenMax, combinedData);

	// When length of the records is large, O(nlogn) sorting might be faster
	// sort(combinedData.begin(), combinedData.end(), [](const pair<int, string>& a, const pair<int, string>& b) {
    //             int res = a.second.compare(b.second);
	// 			if (res<0){
	// 				return true;
	// 			}
	// 			return false;
    //         });

	double sorting_t	= (double)(clock() - currTS_p2) / CLOCKS_PER_SEC;
	cout<< "Sorting time "<< sorting_t << endl;

	clock_t currTS_p3	= clock();
	getExactMatches(totalUniqueRecords, exactmatches, combinedData);
	getUniqueEntries();
	double exactClustering_t	= (double)(clock() - currTS_p3) / CLOCKS_PER_SEC;
	cout<< "De-duplication Time "<< exactClustering_t << endl;

	// Set the Union Find Data Structure
	uf.setVariable(totalUniqueRecords);

	// Set the record comparator
	// Note: if comparing attr size is more than 100 char size, 
	// update the matArr initialization in RecordComparator class
	recCom.setComparator(groundTruthInd, global_t, attrThresholds, comparingAttrs);


	// Do Blocking
	clock_t currTS_p5	= clock();
	BlockingMethods bm;
	bm.setBlocking(kmer, alphabetSize, blockingAttr, uniqueRecords);
	// Call appripriete blocking type
	// bm.getNormalBlockingIDArray();
	bm.getSuperBlockingIDArray();
 
	bm.sortBlockingIDArray();
	bm.removeRedundentBlockingID();
	bm.findBlockBoundaries();
	double blockingArray_t	= (double)(clock() - currTS_p5) / CLOCKS_PER_SEC;
	cout<< "Blocking Time "<< blockingArray_t << endl;

	clock_t currTS_p6	= clock();
	processAllBlocks(bm);
	double recordComp_t	= (double)(clock() - currTS_p6) / CLOCKS_PER_SEC;
	cout<< "Record Comparison Time "<< recordComp_t << endl;


	// Optional sorting step only for SuperBlocking
	// clock_t currTS_p4	= clock();
	// doSortedComp();
	// double sortedComp_t	= (double)(clock() - currTS_p4) / CLOCKS_PER_SEC;
	// cout<< "Superblocking Sorting Time "<< sortedComp_t << endl;


	// Find Connected Components
	clock_t currTS_p7	= clock();
	findConnComp();
	double singleLinkage_t	= (double)(clock() - currTS_p7) / CLOCKS_PER_SEC;
	cout<< "Single linkage Time "<< singleLinkage_t << endl;

	// Find Complete Linkage
	clock_t currTS_p8	= clock();
	findFinalConnectedComp();
	double connCompFind_t	= (double)(clock() - currTS_p8) / CLOCKS_PER_SEC;
	cout<< "Complete Linkage Time "<< connCompFind_t << endl;

	// Write Output to files
	clock_t currTS_p9	= clock();
	string outname1 = "Out_gTruths.csv";
	string outname2 = "Out_recInds.csv";

	writeFinalConnectedComponentToFile(outname1, outname2, groundTruthInd);
	double fileWriteTime	= (double)(clock() - currTS_p9) / CLOCKS_PER_SEC;
	cout<< "Data Write Time "<< fileWriteTime << endl;

	double total_t	= (double)(clock() - currTS_p0) / CLOCKS_PER_SEC;
	cout<< "TOTAL TIME: "<< total_t << endl;


	return 0;
}