#ifndef UTILITIES_H
#define UTILITIES_H

#include "recordComparator.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <cstring>
#include <ctime> 
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

bool invalidChar(int c) {  
    return !(c>=0 && c <128);
}


void getFormattedDataFromCSV(vector<string>& file_paths, vector<vector<string> > &vec2D, int &attributes, int &totalRecords) {
    string line;

	for(int f=0; f<file_paths.size(); f++){
		ifstream records(file_paths[f]);
		// cout<< "Reading file: "<< file_paths[f] << endl;
		while (getline (records, line)) {
			vector<string> result;
			boost::split(result, line, boost::is_any_of(","));
			vector<string> vec;
			for(int i=0; i<result.size(); i++) {
				auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
											return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch);
										});
				result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator
    			result[i].erase(remove_if(result[i].begin(),result[i].end(), [](unsigned char c){return !(c>=0 && c <128);}), result[i].end());

				boost::to_lower(result[i]);
				vec.push_back(result[i]);
			}
			vec2D.push_back(vec);
		}
		records.close();

	}
	attributes = vec2D[0].size();
	totalRecords = vec2D.size();
}


void getFormattedDataFromCSV(vector<string>& file_paths, vector<vector<string> > &vec2D, int &attributes, int &totalRecords, vector<int> &excludedColumns) {
    string line;

	for(int f=0; f<file_paths.size(); f++){
		ifstream records(file_paths[f]);
		// cout<< "Reading file: "<< file_paths[f] << endl;
		while (getline (records, line)) {
			vector<string> result;
			boost::split(result, line, boost::is_any_of(","));
			vector<string> vec;
			int excludedColPtr = 0;
			int excludedColNumbers = excludedColumns.size();
			for(int i=0; i<result.size(); i++) {

				// Do not read excluded columns
				if(excludedColPtr < excludedColNumbers){
					if(i == excludedColumns[excludedColPtr]){
						excludedColPtr++;
						continue;
					}
				}

				auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
											return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch);
										});
				result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator
    			result[i].erase(remove_if(result[i].begin(),result[i].end(), [](unsigned char c){return !(c>=0 && c <128);}), result[i].end());

				boost::to_lower(result[i]);
				vec.push_back(result[i]);
			}
			vec2D.push_back(vec);
		}
		records.close();

	}
	attributes = vec2D[0].size();
	totalRecords = vec2D.size();
}


void getFormattedDataFromCSV(vector<string>& file_paths, vector<vector<string> > &vec2D, int longAttr, int &attributes, int &totalRecords) {
    string line;
    int ind = 0;

	for(int f=0; f<file_paths.size(); f++){
		ifstream records(file_paths[f]);
		while (getline (records, line)) {
			vector<string> result;
			boost::split(result, line, boost::is_any_of(","));
			vector<string> vec;
			for(int i=0; i<result.size(); i++) {
				auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
											return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch);
										});
				result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator

				if(i == longAttr){
					string s="";
					for(int j = 0; j<result[i].size(); j++){
						if (isupper(result[i][j])){
							s.push_back(result[i][j]);
						}
					}
					result[i] = s;
				}

				boost::to_lower(result[i]);
				vec.push_back(result[i]);
			}
			vec2D.push_back(vec);
			ind++;
		}
		records.close();

	}
	attributes = vec2D[0].size();
	totalRecords = vec2D.size();
}


void getFormattedDataFromCSV(vector<string>& file_paths, vector<vector<string> > &vec2D, int longAttr, int &attributes, int &totalRecords, int mode) {
    string line;
    int ind = 0;
	// Easy files
	if (mode == 0) {
		for(int f=0; f<file_paths.size(); f++){
			ifstream records(file_paths[f]);
			while (getline (records, line)) {
				vector<string> result;
				boost::split(result, line, boost::is_any_of(","));
				vector<string> vec;
				for(int i=0; i<result.size(); i++) {
					if (i==12 || i==13){
						continue;
					}
					auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
												return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch) || invalidChar(ch);
											});
					result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator

					if(i == longAttr){
						string s="";
						for(int j = 0; j<result[i].size(); j++){
							if (isupper(result[i][j])){
								s.push_back(result[i][j]);
							}
						}
						result[i] = s;
					}

					boost::to_lower(result[i]);
					vec.push_back(result[i]);
				}
				vec2D.push_back(vec);
				ind++;
			}
			records.close();
		}
	} else if (mode == 1) { // Medium Files
		for(int f=0; f<file_paths.size(); f++){
			ifstream records(file_paths[f]);
			while (getline (records, line)) {
				vector<string> result;
				boost::split(result, line, boost::is_any_of(","));
				vector<string> vec;
				for(int i=0; i<result.size(); i++) {
					// Remove slary
					// if (i==11 || i==12){
					// 	continue;
					// }
					auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
												return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch) || !std::isprint(ch);
											});
					result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator

					if(i == longAttr){
						string s="";
						for(int j = 0; j<result[i].size(); j++){
							if (isupper(result[i][j])){
								s.push_back(result[i][j]);
							}
						}
						result[i] = s;
					}

					boost::to_lower(result[i]);
					vec.push_back(result[i]);
				}
				vec2D.push_back(vec);
				ind++;
			}
			records.close();
		}
	} else if (mode == 2) { // Hard Files
		for(int f=0; f<file_paths.size(); f++){
			ifstream records(file_paths[f]);
			while (getline (records, line)) {
				vector<string> result;
				boost::split(result, line, boost::is_any_of(","));
				vector<string> vec;
				for(int i=0; i<result.size(); i++) {
					auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
												return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch) || invalidChar(ch);
											});
					result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator

					if(i == longAttr){
						string s="";
						for(int j = 0; j<result[i].size(); j++){
							if (isupper(result[i][j])){
								s.push_back(result[i][j]);
							}
						}
						result[i] = s;
					}

					boost::to_lower(result[i]);
					vec.push_back(result[i]);
				}
				vec2D.push_back(vec);
				ind++;
			}
			records.close();
		}
	}
	

	attributes = vec2D[0].size();
	totalRecords = vec2D.size();
}


void getFormattedDataFromCSV(vector<string>& file_paths, vector<vector<string> > &vec2D, int longAttr, int &attributes, int &totalRecords, int &g_lenmax, int mode) {
    string line;
    int ind = 0;
	// Easy files
	if (mode == 0) {
		for(int f=0; f<file_paths.size(); f++){
			ifstream records(file_paths[f]);
			while (getline (records, line)) {
				vector<string> result;
				boost::split(result, line, boost::is_any_of(","));
				vector<string> vec;
				for(int i=0; i<result.size(); i++) {
					if (i==12 || i==13){
						continue;
					}
					auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
												return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch);
											});
					result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator

					if(i == longAttr){
						string s="";
						for(int j = 0; j<result[i].size(); j++){
							if (isupper(result[i][j])){
								s.push_back(result[i][j]);
							}
						}
						result[i] = s;
					}

					boost::to_lower(result[i]);
					g_lenmax = result[i].size() > g_lenmax? result[i].size() : g_lenmax;
					vec.push_back(result[i]);
				}
				vec2D.push_back(vec);
				ind++;
			}
			records.close();
		}
	} else if (mode == 1) { // Medium Files
		for(int f=0; f<file_paths.size(); f++){
			ifstream records(file_paths[f]);
			while (getline (records, line)) {
				vector<string> result;
				boost::split(result, line, boost::is_any_of(","));
				vector<string> vec;
				for(int i=0; i<result.size(); i++) {
					if (i==11 || i==12){
						continue;
					}
					auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
												return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch);
											});
					result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator

					if(i == longAttr){
						string s="";
						for(int j = 0; j<result[i].size(); j++){
							if (isupper(result[i][j])){
								s.push_back(result[i][j]);
							}
						}
						result[i] = s;
					}
					g_lenmax = result[i].size() > g_lenmax? result[i].size() : g_lenmax;
					boost::to_lower(result[i]);
					vec.push_back(result[i]);
				}
				vec2D.push_back(vec);
				ind++;
			}
			records.close();
		}
	} else if (mode == 2) { // Hard Files
		for(int f=0; f<file_paths.size(); f++){
			ifstream records(file_paths[f]);
			while (getline (records, line)) {
				vector<string> result;
				boost::split(result, line, boost::is_any_of(","));
				vector<string> vec;
				for(int i=0; i<result.size(); i++) {
					if (i==10 || i==11){
						continue;
					}
					auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
												return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch);
											});
					result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator

					if(i == longAttr){
						string s="";
						for(int j = 0; j<result[i].size(); j++){
							if (isupper(result[i][j])){
								s.push_back(result[i][j]);
							}
						}
						result[i] = s;
					}
					g_lenmax = result[i].size() > g_lenmax? result[i].size() : g_lenmax;
					boost::to_lower(result[i]);
					vec.push_back(result[i]);
				}
				vec2D.push_back(vec);
				ind++;
			}
			records.close();
		}
	}
	

	attributes = vec2D[0].size();
	totalRecords = vec2D.size();
}


void getFormattedDataFromCSV(string& file_path, vector<vector<string> > &vec2D, int &attributes, int &totalRecords) {
    string line;
    ifstream records(file_path);
    int ind = 0;

    while (getline (records, line)) {
        vector<string> result;
        boost::split(result, line, boost::is_any_of(","));
        vector<string> vec;
		for(int i=0; i<result.size(); i++) {
			// auto last = std::remove_if(result[i].begin(), result[i].end(), [](auto ch) {
        	// 							return ::ispunct(ch) || ::iswpunct(ch);
    		// 						});
			// result[i].erase(last, result[i].end()); //Remove junk left by remove_if() at the end of iterator
			if(result[i][0] == '\0') {
				cout<< "NULL STRING FOUND" << endl;
			}
			boost::to_lower(result[i]);
			if(result[i][0] == '\0') {
				cout<< "NULL STRING FOUND Again" << endl;
			}
			vec.push_back(result[i]);
		}
        vec2D.push_back(vec);
		ind++;
    }
    records.close();

	attributes = vec2D[0].size();
	totalRecords = vec2D.size();
}


void getCombinedData(vector<vector<string> > &vec2D, vector<pair<int, string> > &combinedData, int &lenMax) {
	long long int totSize = 0;
	combinedData.resize(vec2D.size());
    int attributes = vec2D[1].size();
	int max = 0;

	for (int i = 0; i< vec2D.size(); i++) {
		pair<int, string> p;
		p.first = i;
        for(int j=1; j<attributes; j++) {
            p.second += vec2D[i][j];
        }
		combinedData[i]=p;
		totSize += p.second.size();
		if (max<p.second.size()) {
			max = p.second.size();
		}
	}
	lenMax = max;
	string strSample(lenMax, '0');
	cout<< " Avg record length: " << (double)(((double)totSize) / (double)combinedData.size()) << endl;
	// Padding to make all characters same size
    for (int i = 0; i < vec2D.size(); ++i) {
		int lenDiff		= lenMax - combinedData[i].second.size();
		if(lenDiff > 0)
			combinedData[i].second	+= strSample.substr(0, lenDiff);
	}
}


void getCombinedData(vector<vector<string> > &vec2D, vector<pair<int, string> > &combinedData, int &lenMax, int groundTruthIndex) {
	string strSample(1000, '0');
	combinedData.resize(vec2D.size());
    int attributes = vec2D[0].size();
	lenMax = 0;
	for (int i = 0; i< vec2D.size(); i++) {
		pair<int, string> p;
		p.first = i;
        for(int j=0; j<attributes; j++) {
			if(j == groundTruthIndex) {
				continue;
			}
            p.second += vec2D[i][j];
        }
		combinedData[i]=p;
		if (lenMax < p.second.size()) {
			lenMax = p.second.size();
		}
	}

	// Padding to make all characters same size
    for (int i = 0; i < vec2D.size(); ++i) {
		int lenDiff		= lenMax - combinedData[i].second.length();
		if(lenDiff > 0){
			combinedData[i].second	+= strSample.substr(0, lenDiff);
		}
	}
}


void getExactMatches(int &totalUniqueRecords, vector<vector<int> > &exactmatches, vector<pair<int, string> > &combinedData) {
	vector<int> tempVec;
	tempVec.push_back(combinedData[0].first);
	for (int i = 1; i < combinedData.size(); ++i) {
		if(combinedData[i].second.compare(combinedData[i - 1].second) == 0)
			tempVec.push_back(combinedData[i].first);
		else {
			exactmatches.push_back(tempVec);
			tempVec.clear();
			tempVec.push_back(combinedData[i].first);
		}
	}
	exactmatches.push_back(tempVec);
	totalUniqueRecords = exactmatches.size();
	cout << "total exact clusters: " << totalUniqueRecords << endl;
}


void getExactAttrs(vector<vector<int> > &exactmatches, vector<pair<int, string> > &combinedData) {
	vector<int> tempVec;
	tempVec.push_back(combinedData[0].first);
	for (int i = 1; i < combinedData.size(); ++i) {
		if(combinedData[i].second.compare(combinedData[i - 1].second) == 0)
			tempVec.push_back(combinedData[i].first);
		else {
			exactmatches.push_back(tempVec);
			tempVec.clear();
			tempVec.push_back(combinedData[i].first);
		}
	}
	exactmatches.push_back(tempVec);
}


void getCombinedData_attr(vector<vector<string> > &vec2D, vector<pair<int, string> > &combinedData, int &lenMax, int attrIndex) {
	string strSample(50, '0');
	combinedData.resize(vec2D.size());
	int max = 0;
	for (int i = 0; i< vec2D.size(); i++) {
		pair<int, string> p;
		p.first = i;
		p.second = vec2D[i][attrIndex];
		combinedData[i]=p;
		if (max<p.second.size()) {
			max = p.second.size();
		}
	}
	lenMax = max;
	// Padding to make all characters same size
    for (int i = 0; i < combinedData.size(); ++i) {
		int lenDiff		= lenMax - combinedData[i].second.length();
		if(lenDiff > 0)
			combinedData[i].second	+= strSample.substr(0, lenDiff);
	}
}


void radixSort_std(int &lenMax, vector<pair<int, string> > &strDataArr){
	int numRecords = strDataArr.size();
	cout<< "Total Records: " << numRecords << endl;
	vector<pair<int, string> > tempArr(numRecords);
	for (int i = lenMax - 1; i >= 0; --i) {
		vector<long long int> countArr(256, 0);

		for (int j = 0; j < numRecords; ++j) {
			if (((int)(strDataArr[j].second)[i])>255 || (strDataArr[j].second)[i]<0){
				cout<< "Char Error: " << (strDataArr[j].second)[i] << endl;
				cout<< "Full Record: " << endl;
				cout<< strDataArr[j].second << endl;
			}
			countArr[(strDataArr[j].second)[i]]++;
		}
		
		for (int k = 1; k < 256; ++k){
			countArr[k]	+= countArr[k - 1];
		}

		for (int j = numRecords - 1; j >= 0; --j){
			// if(countArr[(strDataArr[j].second)[i]] <= 0){
			// 	cout<< "error val: " << countArr[(strDataArr[j].second)[i]] << endl;
			// }
			countArr[(strDataArr[j].second)[i]] = countArr[(strDataArr[j].second)[i]] - 1;
			if (countArr[(strDataArr[j].second)[i]]<0){
				cout<< "Val ERROR: " << endl;
			}
			tempArr[countArr[(strDataArr[j].second)[i]]]	= strDataArr[j];
		}

		for (int j = 0; j < numRecords; ++j){
			strDataArr[j] = tempArr[j];
		}
	}
}


void radixSort_utility(int &lenMax, vector<pair<int, string> > &strDataArr){
	int numRecords = strDataArr.size();
	vector<pair<int, string> > tempArr(numRecords);
	
	for (int i = lenMax - 1; i >= 0; --i) {
		vector<int> countArr(42, 0);
		
		for (int j = 0; j < numRecords; ++j) {
			countArr[(strDataArr[j].second)[i] - 48]++;
		}
		
		for (int k = 1; k < 42; ++k)
			countArr[k]	+= countArr[k - 1];
		
		for (int j = numRecords - 1; j >= 0; --j)
			tempArr[--countArr[(strDataArr[j].second)[i] - 48]]	= strDataArr[j];
		
		for (int j = 0; j < numRecords; ++j)
			strDataArr[j] = tempArr[j];
	}
}


void getCombinedData_noPad(vector<vector<string> > &vec2D, vector<pair<int, string> > &combinedData, int &lenMax) {
	combinedData.resize(vec2D.size());
    int attributes = vec2D[0].size();
	int max = 0;
	for (int i = 0; i< vec2D.size(); i++) {
		pair<int, string> p;
		p.first = i;
        for(int j=1; j<attributes; j++) {
            p.second += vec2D[i][j];
        }
		combinedData[i]=p;
		if (max<p.second.size()) {
			max = p.second.size();
		}
	}
	lenMax = max;
}


void radixSort_utility_uneven(int lenMax, vector<pair<int, string> > &strDataArr){
	int numRecords = strDataArr.size();
	// first sort them based on sizes
	// then sort all strings sharing same size
	// place them one after another
	// write a function that sorts strings within a certain region in input vector

	vector<pair<int, string> > tempArr(numRecords);
	vector<pair<int, int> > lenRegions;

	// Step 1:
	int start = 0;
	int end = 0;
	vector<int> countLenArr(lenMax, 0);
	for (int i = 0; i < numRecords; ++i) {
		countLenArr[strDataArr[i].second.size()]++;
	}
	for (int k = 1; k < lenMax; ++k) {
		start = countLenArr[k - 1];
		countLenArr[k]	+= countLenArr[k - 1];
		end = countLenArr[k];
		if(start != end) {
			pair<int, int> p;
			p.first = start;
			p.second = end;
			lenRegions.push_back(p);
		}
	}
	for (int i = numRecords - 1; i >= 0; --i) {
			tempArr[--countLenArr[strDataArr[i].second.size()]]	= strDataArr[i];
	}

	for (int i = 0; i < numRecords; ++i){
		strDataArr[i] = tempArr[i];
	}

	// Next Step: Do sorting for each segments

	for(int s=0; s<lenRegions.size(); s++){
		int start = lenRegions[s].first;
		int end = lenRegions[s].second;
		int curLen = strDataArr[start].second.size();
		for (int i = curLen - 1; i >= 0; --i) {
			vector<int> countArr(128, 0);
			countArr[0] = start;
			for (int j = start; j < end; ++j) {
				countArr[(strDataArr[j].second)[i]]++;
			}

			for (int k = 1; k < 128; ++k)
				countArr[k]	+= countArr[k - 1];
			
			for (int j = end - 1; j >= start; --j)
				tempArr[--countArr[(strDataArr[j].second)[i]]]	= strDataArr[j];
			
			for (int j = start; j < end; ++j)
				strDataArr[j] = tempArr[j];
		}
	}
}


void radixSort_utility_uneven_improved(int lenMax, vector<pair<int, string> > &strDataArr){
	int numRecords = strDataArr.size();
	// first sort them based on sizes
	// then sort all strings sharing same size
	// place them one after another
	// write a function that sorts strings within a certain region in input vector
	clock_t currTS_p1	= clock();
	vector<pair<int, string> > tempArr(numRecords);
	vector<pair<int, int> > lenRegions;
	vector<int> regionLens;

	// Step 1:
	int start = 0;
	int end = 0;
	vector<int> countLenArr(lenMax+1, 0);
	for (int i = 0; i < numRecords; ++i) {
		countLenArr[strDataArr[i].second.size()]++;
	}
	for (int k = 1; k <= lenMax; ++k) {
		start = countLenArr[k - 1];
		countLenArr[k]	+= countLenArr[k - 1];
		end = countLenArr[k];
		if(start != end) {
			pair<int, int> p;
			p.first = start;
			p.second = end;
			lenRegions.push_back(p);
			regionLens.push_back(k);
		}
	}
	for (int i = numRecords - 1; i >= 0; --i) {
			tempArr[--countLenArr[strDataArr[i].second.size()]]	= strDataArr[i];
	}

	for (int i = 0; i < numRecords; ++i){
		strDataArr[i] = tempArr[i];
	}
	cout<< "Lenmax: " << lenMax << endl;
	double step_1	= (double)(clock() - currTS_p1) / CLOCKS_PER_SEC;
	cout<< "Step 1 Time "<< step_1 << endl;
	clock_t currTS_p2	= clock();
	// Next Step: Do sorting for each segments
	int startRegion = regionLens.size()-1;
	// reuse variable
	start = -1;
	for (int i = lenMax - 1; i >= 0; --i) {
		while((regionLens[startRegion]>i) && (startRegion >= 0)){
			startRegion--;
		}
		if(startRegion >= 0) {
			start = lenRegions[startRegion].second;
		} else {
			start = 0;
		}
		
		// cout<<"At char: "<< i << " start Reg: " << startRegion << "Region lens: " << regionLens[startRegion]<< endl;
		vector<int> countArr(128, 0);
		countArr[0] = start;
		for (int j = start; j < numRecords; ++j) {
			countArr[(strDataArr[j].second)[i]]++;
		}

		for (int k = 1; k < 128; ++k)
			countArr[k]	+= countArr[k - 1];
		
		for (int j = numRecords - 1; j >= start; --j)
			tempArr[--countArr[(strDataArr[j].second)[i]]]	= strDataArr[j];
		
		for (int j = start; j < numRecords; ++j)
			strDataArr[j] = tempArr[j];
	}
	// for(int i=0; i<numRecords-1; i++){
	// 	if(strDataArr[i].second > strDataArr[i+1].second){
	// 		cout<< "Error: " << strDataArr[i].second << " and " << strDataArr[i+1].second << endl;
	// 	}
	// }
	double step_2	= (double)(clock() - currTS_p2) / CLOCKS_PER_SEC;
	cout<< "Step 2 Time "<< step_2 << endl;
}


string generateCompoundBlockingString(vector<string> &record, int startingAttr, int prefixLen, int groundTruthIndex){
	int attributes = record.size();
	int checkNextN = 10;
	string blockingStr = "";
	for(int i=startingAttr; i < startingAttr + checkNextN; i++){
		if(i%attributes == groundTruthIndex) continue;
		for(int j=0; j<prefixLen; j++){
			if (j>= record[i%attributes].size()){
				blockingStr += '0';
			} else {
				blockingStr += record[i%attributes][j];
			}
		}
	}
	return blockingStr;
}


void getNormalBlockingIDVector(int KMER, string blockingStr, vector<long long int> &blockIDs) {
	string strSample(KMER, 'a');
	int alphabetSize = 36;
	long long int blockIDRange = pow(alphabetSize, KMER);
	long long int blockID;

	if(blockingStr.size() == 0){
		return;
	}

	if(blockingStr.size() < KMER) {                    
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

		blockIDs.push_back(blockID);

		if(blockID<0 || blockID>=blockIDRange) {
		    cout<< "Invalid BlockID: " << blockID << endl;
		    cout<< "BlockingString: " << blockingStr << endl;
		}
	}	
}


void getSuperBlockingIDArray(int KMER, string blockingStr, vector<long long int> &blockIDs) {
	int alphabetSize = 36;
	long long int blockIDRange = pow(alphabetSize, KMER+1);
	string strSample(KMER, 'a');
	long long int blockID;
	long long int perAplhaBlocks = pow(alphabetSize, KMER);

	if(blockingStr.size() == 0){
		return;
	}

	if(blockingStr.size() < KMER) {                    
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

		int blockStrCharCode = (int)blockingStr[0];

		if ((blockStrCharCode >= 97) && (blockStrCharCode <= 122)) {
			blockID = (blockingStr[0]-97)*perAplhaBlocks + blockID;
		} else if ((blockStrCharCode >= 48) && (blockStrCharCode <= 57)){
			blockID = (blockingStr[0]-48 + 26)*perAplhaBlocks + blockID;
		}

		blockIDs.push_back(blockID);

		if(blockID<0 || blockID>=blockIDRange) {
			cout<< "Invalid BlockID: " << blockID << endl;
			cout<< "BlockingString: " << blockingStr << endl;
		}
	}	

}


void generateSampledLinks(vector<vector<string>> &vec2D, vector<pair<int, int>> &sampledLinks, double sampleProb, int groundTruthIndex, int seed){
	string strSample(50, 'a');
	vector<pair<int, string>> combinedRecGroundTruths;
	int lenMax = 0;
	// Copy the ground truths
	for(int i=0; i<vec2D.size(); i++){
		pair<int, string> p;
		p.first = i;
		p.second = vec2D[i][groundTruthIndex];
		if (p.second.size() > lenMax){
			lenMax = p.second.size();
		}
		combinedRecGroundTruths.push_back(p);
	}
	// cout<< "Got Combined Grounth Truths" << endl;
	// Pad them for Sorting
	for (int i = 0; i < combinedRecGroundTruths.size(); ++i) {
		int lenDiff		= lenMax - combinedRecGroundTruths[i].second.length();
		if(lenDiff > 0)
			combinedRecGroundTruths[i].second	+= strSample.substr(0, lenDiff);
	}
	// cout<< "Padded Combined Grounth Truths" << endl;
	// sort the groundTruths
	radixSort_std(lenMax, combinedRecGroundTruths);
	// cout<< "Sorted Combined Grounth Truths" << endl;

	// Sample the true links
	/* initialize random seed: */
  	srand(seed);
	int totalLinks = 0;
	for(int i=0; i<combinedRecGroundTruths.size()-1; i++){
		for(int j= i+1; j<combinedRecGroundTruths.size(); j++){
			if(combinedRecGroundTruths[i].second == combinedRecGroundTruths[j].second){
				totalLinks++;
				// Do a coin toss
				double coinVal = (double)rand() / RAND_MAX;
				if (coinVal <= sampleProb) {
					pair<int, int> p;
					p.first = combinedRecGroundTruths[i].first;
					p.second = combinedRecGroundTruths[j].first;
					sampledLinks.push_back(p);
				}
			} else {
				break;
			}
		}
	}
	cout<< "Total true links: " << totalLinks << endl;
	cout<< "Sampled number of links: " << sampledLinks.size() << endl;

}


void getDistMatrix(vector<vector<string>> &vec2D, vector<pair<int, int>> &sampledLinks, map<int, int> &alteredAttrIndicesMap, int groundTruthIndex, vector<vector<double>> &distMatrix, int g_lenmax){
	int numAttrs = vec2D[0].size();
	int largeThreshold = g_lenmax;
	RecordComparator recCom;
	for(int i=0; i<sampledLinks.size(); i++){
		for(int j=0; j<numAttrs; j++){
			if(j == groundTruthIndex) {
				continue;
			}
			if((vec2D[sampledLinks[i].first][j] == "") && (vec2D[sampledLinks[i].second][j] == "")) {
				distMatrix[i][j] = -3.0;
				continue;
			}
			if (vec2D[sampledLinks[i].second][j] == ""){
				distMatrix[i][j] = -1.0;
				continue;
			}
			if(vec2D[sampledLinks[i].first][j] == "") {
				distMatrix[i][j] = -2.0;
				continue;
			}
			
			if(alteredAttrIndicesMap.count(j) == 1){
				int alternateIndex = alteredAttrIndicesMap[j];
				if(vec2D[sampledLinks[i].second][alternateIndex] == "") {
					distMatrix[i][j] = -1.0;
					continue;
				} else {
					double dist1 = recCom.calculateBasicED(vec2D[sampledLinks[i].first][j], vec2D[sampledLinks[i].second][j], largeThreshold);
					double dist2 = recCom.calculateBasicED(vec2D[sampledLinks[i].first][j], vec2D[sampledLinks[i].second][alternateIndex], largeThreshold);
					distMatrix[i][j] = min(dist1, dist2);
					continue;
				}
			} else {
				distMatrix[i][j] = recCom.calculateBasicED(vec2D[sampledLinks[i].first][j], vec2D[sampledLinks[i].second][j], largeThreshold);
			}
		}
	}
}


void printWeightMatrix(vector<vector<double>> weightMatrix){
	// Print Weight Matrix
	for(int i=0; i<weightMatrix.size();i++){
		cout<< "Attr " << i << " : ";
		for(int j=0; j<weightMatrix[i].size(); j++){
			cout<< weightMatrix[i][j] << " ";
		}
		cout<< endl;
	}
}


void printMissedValMatrix(vector<vector<double>> missedValMatrix){
	// Print Missed Val Matrix
	for(int i=0; i<missedValMatrix.size();i++){
		cout<< "Attr " << i << " : " << missedValMatrix[i][0] << " " << missedValMatrix[i][1] << " " << missedValMatrix[i][2] <<endl;
	}
}


void getWeightMatrix(vector<vector<double>> &distMatrix, vector<vector<double>> &weightMatrix, int groundTruthIndex){
	for(int i=0; i<weightMatrix.size(); i++){
		if(i == groundTruthIndex) continue;
		for(int j=0; j<distMatrix.size(); j++){
			if(distMatrix[j][i] == -1.0){
				continue;
			} else {
				weightMatrix[i][(int)distMatrix[j][i]]++;
			}
		}
	}
	// Print Weight Matrix
	// printWeightMatrix(weightMatrix);
}


void getWeightMatrix(vector<vector<double>> &distMatrix, vector<vector<double>> &weightMatrix, int groundTruthIndex, vector<vector<double>> &MissedValMatrix){
	for(int i=0; i<weightMatrix.size(); i++){
		if(i == groundTruthIndex) continue;
		for(int j=0; j<distMatrix.size(); j++){
			if(distMatrix[j][i] == -1.0){
				MissedValMatrix[i][0]++;
			} else if (distMatrix[j][i] == -2.0){
				MissedValMatrix[i][1]++;
			} else if (distMatrix[j][i] == -3.0){
				MissedValMatrix[i][2]++;
			} else {
				weightMatrix[i][(int)distMatrix[j][i]]++;
			}
		}
	}
}


void getCumulativeWeightMatrices(vector<vector<double>> &weightMatrix){
	for(int i=0; i<weightMatrix.size();i++){
		for(int j = weightMatrix[i].size()-2; j>= 0; j--){
			weightMatrix[i][j] += weightMatrix[i][j+1];
		}
	}
	// printWeightMatrix(weightMatrix);
}


void getCumulativeWeightMatrices(vector<vector<double>> &weightMatrix, vector<vector<double>> &MissedValMatrix){
	for(int i=0; i<weightMatrix.size();i++){
		// missing two values is worse than missing one value
		MissedValMatrix[i][2] += MissedValMatrix[i][0];
		MissedValMatrix[i][2] += MissedValMatrix[i][1];
		weightMatrix[i][weightMatrix[i].size()-1] += MissedValMatrix[i][2];
		for(int j = weightMatrix[i].size()-2; j>= 0; j--){
			weightMatrix[i][j] += weightMatrix[i][j+1];
			// weightMatrix[i][j] += MissedValMatrix[i][2];
		}
	}
}


void normalizeWeights(vector<vector<double>> &weightMatrix, int groundTruthIndex, double numSampledLinks){
	for(int i=0; i<weightMatrix.size();i++){
		if(i == groundTruthIndex) continue;
		for(int j=0; j<weightMatrix[i].size(); j++){
			weightMatrix[i][j] = (double)weightMatrix[i][j]/numSampledLinks;
		}
	}
	// printWeightMatrix(weightMatrix);
}


void normalizeWeights(vector<vector<double>> &weightMatrix, vector<vector<double>> &MissedValMatrix, int groundTruthIndex, double numSampledLinks){
	for(int i=0; i<weightMatrix.size();i++){
		if(i == groundTruthIndex) continue;
		// double numMissedVal = MissedValMatrix[i][0] + MissedValMatrix[i][1];
		// double numPresentVal = numSampledLinks - numMissedVal;
		for(int j=0; j<weightMatrix[i].size(); j++){
			weightMatrix[i][j] = (double)weightMatrix[i][j]/numSampledLinks;
		}
		MissedValMatrix[i][0] = (MissedValMatrix[i][0]/numSampledLinks) * -1.40;
		MissedValMatrix[i][1] = (MissedValMatrix[i][1]/numSampledLinks) * -1.40;
		MissedValMatrix[i][2] = (MissedValMatrix[i][2]/numSampledLinks) * -1.40;
		// MissedValMatrix[i][0] = 0.0;
		// MissedValMatrix[i][1] = 0.0;
	}
	cout<< "PRINTING NORMALIZED WEIGHT MATRIX" << endl;
	printWeightMatrix(weightMatrix);
	cout<< endl;
	cout<< "PRINTING NORMALIZED MISSED VALUE MATRIX" << endl;
	printMissedValMatrix(MissedValMatrix);
}


void estimateAttributeThresholds(vector<vector<string>> &vec2D, vector<pair<int, int>> &sampledLinks, map<int, int> &alteredAttrIndicesMap , int groundTruthIndex, vector<double> &attrThresholds, double &global_t, double satisfyThreshold){
	int numAttrs = vec2D[0].size();
	int largeThreshold = 10;
	int satisfactionPoint = ceil(((double)sampledLinks.size())*satisfyThreshold) - 1;
	attrThresholds.resize(numAttrs, 0.0);
	vector<double> gDistances;
	RecordComparator recCom;
	gDistances.resize(sampledLinks.size(), 0.0);
	for(int i=0; i< numAttrs; i++){
		if(i == groundTruthIndex) {
			continue;
		}
		vector<double> aDistances;
		aDistances.resize(sampledLinks.size(), 0.0);
		for(int j=0; j<sampledLinks.size(); j++){
			if((vec2D[sampledLinks[j].first][i] == "") || (vec2D[sampledLinks[j].second][i] == "")) {
				aDistances[j] = 0.0;
			} else {
				double dist = recCom.calculateBasicED(vec2D[sampledLinks[j].first][i], vec2D[sampledLinks[j].second][i], largeThreshold);
				aDistances[j] = dist;
				if(dist>=largeThreshold){
					cout<<endl;
					for(int u=0; u<numAttrs; u++) {
						cout<< vec2D[sampledLinks[j].first][u] << " " ;
					}
					cout<< endl;
					for(int u=0; u<numAttrs; u++) {
						cout<< vec2D[sampledLinks[j].second][u] << " " ;
					}
					cout<< endl;
					cout<< endl;
				}
			}
		}
		if(alteredAttrIndicesMap.count(i) == 1){
			int alternateIndex = alteredAttrIndicesMap[i];

			for(int j=0; j<sampledLinks.size(); j++){
				if((vec2D[sampledLinks[j].first][i] == "") || (vec2D[sampledLinks[j].second][alternateIndex] == "")) {
					aDistances[j] = 0.0;
				} else {
					double dist = recCom.calculateBasicED(vec2D[sampledLinks[j].first][i], vec2D[sampledLinks[j].second][alternateIndex], largeThreshold);
					aDistances[j] = min(dist, aDistances[j]);
				}
			}
		}
		for(int j=0; j<sampledLinks.size(); j++){
			gDistances[j] += aDistances[j];
		}
		sort(aDistances.begin(),aDistances.end());
		attrThresholds[i] = aDistances[satisfactionPoint];
	}
	sort(gDistances.begin(),gDistances.end());
	global_t = gDistances[satisfactionPoint];
	for(int i=0; i<numAttrs; i++){
		cout<< "Attr " << i << " has threshold Value: " << attrThresholds[i] << endl;
	}
	cout<< "Global threshold: " << global_t << endl;
}


void estimateThresholdsAndGlobalWeight(vector<vector<double>> &distMatrix, vector<vector<double>> &weightMatrix, vector<double> &attrThresholds, double &global_t, int groundTruthIndex){
	global_t = MAXFLOAT;
	for(int i=0; i<distMatrix[0].size(); i++){
		double maxVal = 0.0;
		for(int j=0; j<distMatrix.size(); j++){
			maxVal = maxVal<distMatrix[j][i]? distMatrix[j][i]: maxVal;
		}
		attrThresholds[i] = maxVal;
	}
	cout<< "calculated attribute threshold values" << endl;
	for(int i=0; i<distMatrix.size(); i++){
		double cumulativeWeight = 0.0;
		for(int j=0; j<distMatrix[0].size(); j++){
			cumulativeWeight+= weightMatrix[j][(int)distMatrix[i][j]];
		}
		global_t = global_t<cumulativeWeight?global_t:cumulativeWeight;
	}
	cout<< "calculated global bar" << endl;
	for(int i=0; i<attrThresholds.size(); i++){
		cout<< "Attr: " << i << " threshold: " << attrThresholds[i] << endl;
	}
	cout<< "Global weight Bar: " << global_t << endl;
}


void estimateThresholdsAndGlobalWeight(vector<vector<double>> &distMatrix, vector<vector<double>> &missedValMatrix, vector<vector<double>> &weightMatrix, vector<double> &attrThresholds, double &global_t, int groundTruthIndex){
	global_t = 0.0;
	double satisfactionPoint = 0.95;
	vector<double> cumulativeWeights;
	cumulativeWeights.resize(distMatrix.size(), 0.0);
	vector<double> thresholds;
	thresholds.resize(distMatrix.size(), 0.0);
	for(int i=0; i<distMatrix[0].size(); i++){
		for(int j=0; j<distMatrix.size(); j++){
			// maxVal = maxVal<distMatrix[j][i]? distMatrix[j][i]: maxVal;
			thresholds[j] = distMatrix[j][i];
		}
		sort(thresholds.begin(),thresholds.end());
		attrThresholds[i] = thresholds[(int)(distMatrix.size()*satisfactionPoint)];
	}
	cout<< "calculated attribute threshold values" << endl;

	for(int i=0; i<distMatrix.size(); i++){
		double cumulativeWeight = 0.0;
		for(int j=0; j<distMatrix[0].size(); j++){
			if(distMatrix[i][j] >= 0.0){
				cumulativeWeight+= (double)weightMatrix[j][(int)distMatrix[i][j]];
			} 
			// else if (distMatrix[i][j] == -1.0) {
			// 	cumulativeWeight+= missedValMatrix[j][0];
			// } else if (distMatrix[i][j] == -2.0){
			// 	cumulativeWeight+= missedValMatrix[j][1];
			// } else if (distMatrix[i][j] == -3.0){
			// 	cumulativeWeight+= missedValMatrix[j][2];
			// }
		}
		cumulativeWeights[i] = cumulativeWeight;
		// global_t = global_t>cumulativeWeight?global_t:cumulativeWeight;
	}

	for(int i=0; i<attrThresholds.size(); i++){
		cout<< "Attr: " << i << " threshold: " << attrThresholds[i] << endl;
	}
	satisfactionPoint = 0.95;
	sort(cumulativeWeights.begin(),cumulativeWeights.end());

	// cout<< "Printing Sorted Global Weights" << endl;
	// for(int i=0; i<10; i++){
	// 	cout<< "Sorted Weight: " << i << " : " << cumulativeWeights[i] << endl;
	// }
	global_t = cumulativeWeights[cumulativeWeights.size() - (int)cumulativeWeights.size()*satisfactionPoint];
	cout<< "Global weight Bar: " << global_t << endl;
}


void estimateWeightedAttributeThresholds(vector<vector<string>> &vec2D, vector<pair<int, int>> &sampledLinks, map<int, int> &alteredAttrIndicesMap , int groundTruthIndex, vector<double> &attrThresholds, vector<double> weights, double &global_t, double satisfyThreshold){
	int numAttrs = vec2D[0].size();
	int largeThreshold = 10;
	int satisfactionPoint = ceil(((double)sampledLinks.size())*satisfyThreshold) - 1;
	attrThresholds.resize(numAttrs, 0.0);
	vector<double> gDistances;
	RecordComparator recCom;
	gDistances.resize(sampledLinks.size(), 0.0);
	for(int i=0; i< numAttrs; i++){
		if(i == groundTruthIndex) {
			continue;
		}
		vector<double> aDistances;
		aDistances.resize(sampledLinks.size(), 0.0);
		for(int j=0; j<sampledLinks.size(); j++){
			if((vec2D[sampledLinks[j].first][i] == "") || (vec2D[sampledLinks[j].second][i] == "")) {
				aDistances[j] = 0.0;
			} else {
				double dist = recCom.calculateBasicED(vec2D[sampledLinks[j].first][i], vec2D[sampledLinks[j].second][i], largeThreshold);
				aDistances[j] = dist*weights[i];
			}
		}
		if(alteredAttrIndicesMap.count(i) == 1){
			int alternateIndex = alteredAttrIndicesMap[i];

			for(int j=0; j<sampledLinks.size(); j++){
				if((vec2D[sampledLinks[j].first][i] == "") || (vec2D[sampledLinks[j].second][alternateIndex] == "")) {
					aDistances[j] = 0.0;
				} else {
					double dist = recCom.calculateBasicED(vec2D[sampledLinks[j].first][i], vec2D[sampledLinks[j].second][alternateIndex], largeThreshold);
					aDistances[j] = min(dist, aDistances[j]) * weights[i];
				}
			}
		}
		for(int j=0; j<sampledLinks.size(); j++){
			gDistances[j] += aDistances[j];
		}
		sort(aDistances.begin(),aDistances.end());
		attrThresholds[i] = aDistances[satisfactionPoint];
	}
	sort(gDistances.begin(),gDistances.end());
	global_t = gDistances[satisfactionPoint];
	for(int i=0; i<numAttrs; i++){
		cout<< "Attr " << i << " has threshold Value: " << attrThresholds[i] << endl;
	}
	cout<< "Global threshold: " << global_t << endl;
}


void estimateMaxMissingValueThreshold(vector<vector<string>> vec2D, vector<pair<int, int>> sampledLinks, map<int, int> alteredAttrIndices, int groundTruthIndex, int &missingValueCountThreshold, double satisfyThreshold){
	int numAttrs = vec2D[0].size();
	int satisfactionPoint = ceil(((double)sampledLinks.size())*satisfyThreshold) - 1;
	cout<< "Total Points: " << sampledLinks.size() << endl;
	cout<< "Satisfaction Point: " << satisfactionPoint << endl;
	vector<double> gMisses;
	gMisses.resize(sampledLinks.size(), 0.0);

	for(int j=0; j<sampledLinks.size(); j++){
		double mutualMiss = 0.0;
		for(int i= 0; i<numAttrs; i++) {
			if(alteredAttrIndices.count(i) == 1){
				if (vec2D[sampledLinks[j].first][i].size() == 0 || vec2D[sampledLinks[j].second][i].size() == 0 || vec2D[sampledLinks[j].second][alteredAttrIndices[i]].size() == 0){
                    mutualMiss += 1.0;
                    continue;
                }
			}
			if((vec2D[sampledLinks[j].first][i] == "") || (vec2D[sampledLinks[j].second][i] == "")) {
				mutualMiss += 1.0;
				continue;
			}
		}
		gMisses[j] = mutualMiss;
	}

	sort(gMisses.begin(),gMisses.end());

	missingValueCountThreshold = gMisses[satisfactionPoint];
	cout<< "Minimum missed values at: " <<sampledLinks.size()*satisfyThreshold << endl;
	cout<< "Global Missed values: " << missingValueCountThreshold << endl;	
}


void estimateBlockingAttribute(vector<vector<string>> &vec2D, vector<pair<int, int>> &sampledLinks, int groundTruthIndex, int &blockingAttr, vector<int> &sequenceOfAttrs, map<int, int> &alteredAttrIndicesMap){
	int numAttrs = vec2D[0].size();
	vector<double> blockingReductionRatios;
	vector<double> blockingPairCompleteness;
	blockingReductionRatios.resize(numAttrs, 0.0);
	blockingPairCompleteness.resize(numAttrs, 0.0);
	int kmer = 5;
	long long int totalPossibleComp =  (sampledLinks.size() * (sampledLinks.size() - 1))/2;
	for(int i=0; i<numAttrs; i++){
		map<long long int, set<int>> blockRecIDMap;
		int truePairsCovered = 0;
		if(i == groundTruthIndex) {
			continue;
		}
		for(int j=0; j<sampledLinks.size(); j++){
			vector<long long int> u_bids;
			vector<long long int> v_bids;
			vector<long long int> intersection;
			// getNormalBlockingIDVector(kmer, vec2D[sampledLinks[j].first][i], u_bids);
			// getNormalBlockingIDVector(kmer, vec2D[sampledLinks[j].second][i], v_bids);
			getSuperBlockingIDArray(kmer, vec2D[sampledLinks[j].first][i], u_bids);
			getSuperBlockingIDArray(kmer, vec2D[sampledLinks[j].second][i], v_bids);
			sort(u_bids.begin(), u_bids.end());
			sort(v_bids.begin(), v_bids.end());
			set_intersection(u_bids.begin(),u_bids.end(),
                          v_bids.begin(),v_bids.end(),
                          back_inserter(intersection));
			if(intersection.size()>0){
				truePairsCovered++;
			}
			for(int k=0; k<u_bids.size(); k++){
				blockRecIDMap[u_bids[k]].insert(sampledLinks[j].first);
			}
			for(int k=0; k<v_bids.size(); k++){
				blockRecIDMap[v_bids[k]].insert(sampledLinks[j].second);
			}

		}
		blockingPairCompleteness[i] = (double)(((double) truePairsCovered)/ ((double)sampledLinks.size()));
		long long int compAfterBlocking = 0;
		for(auto const& p : blockRecIDMap){
			long long n = p.second.size();
			long long int compForThisBlock = (n*(n-1))/2;
			compAfterBlocking += compForThisBlock;
		}

		blockingReductionRatios[i] = 1.0 - (double)(((double)compAfterBlocking) / ((double)totalPossibleComp));
		if(alteredAttrIndicesMap.count(i) == 1){
			blockingReductionRatios[i] = blockingReductionRatios[i] - (1 - blockingReductionRatios[i]);
		}
		cout<< "Attribute: " << i << " Reduction Ratio: " << blockingReductionRatios[i] << " Pair Completeness: "<< blockingPairCompleteness[i] << endl;
	}
	// Set Blocking attribute
	double max = 0.0;
	int max_bb_index = 0;
	vector<pair<double, int>> bbAttrPairList;
	for(int i=0; i<numAttrs; i++){
		if(blockingReductionRatios[i] > max){
			max = blockingReductionRatios[i];
			max_bb_index = i;
		}
	}
	blockingAttr = max_bb_index;


	// Set Sequence of Attributes to Compare
	for(int i=0; i<numAttrs; i++){
		pair<double, int> p;
		p.first = blockingReductionRatios[i];
		p.second = i;
		bbAttrPairList.push_back(p);
	}

	sort(bbAttrPairList.begin(), bbAttrPairList.end(), [](auto &left, auto &right) {
    	return left.first > right.first;
	});

	for(int i=0; i<numAttrs; i++){
		cout<< "BRR: " << bbAttrPairList[i].first <<  " of Attr: " << bbAttrPairList[i].second << endl;
		sequenceOfAttrs.push_back(bbAttrPairList[i].second);
	}
}


// void estimateMultiLvlBlockingAttribute(vector<vector<string>> &vec2D, vector<pair<int, int>> &sampledLinks, int groundTruthIndex, int &blockingAttr, vector<int> &sequenceOfAttrs, map<int, int> &alteredAttrIndicesMap){
// 	int numAttrs = vec2D[0].size();
// 	int kmer = 5;
// 	int lvl = 3;
// 	int topK = 3;
// 	float brrWeight = 1.0;
// 	float pcWeight = 1.0;

// 	vector<pair<vector<int>, float>> blokingKeyArrs;
// 	// initial blocking attrs
// 	for(int i =0; i<attr; i++){
// 		if(i == groundTruthIndex) {
// 			continue;
// 		}
// 		pair<vector<int>, float> keyPerf;
// 		keyPerf.first.push_back(i);
// 		keyPerf.second = 0.0;
// 		blockingKeyArrs.push_back(keyPerf);
// 	}

// 	long long int totalPossibleComp =  (sampledLinks.size() * (sampledLinks.size() - 1))/2;

// 	for(int i=0; i<numAttrs; i++){
// 		map<long long int, set<int>> blockRecIDMap;
// 		int truePairsCovered = 0;
// 		if(i == groundTruthIndex || i ==0) {
// 			continue;
// 		}
// 		for(int j=0; j<sampledLinks.size(); j++){
// 			vector<long long int> u_bids;
// 			vector<long long int> v_bids;
// 			vector<long long int> intersection;
// 			// getNormalBlockingIDVector(kmer, vec2D[sampledLinks[j].first][i], u_bids);
// 			// getNormalBlockingIDVector(kmer, vec2D[sampledLinks[j].second][i], v_bids);
// 			getSuperBlockingIDArray(kmer, vec2D[sampledLinks[j].first][i], u_bids);
// 			getSuperBlockingIDArray(kmer, vec2D[sampledLinks[j].second][i], v_bids);
// 			sort(u_bids.begin(), u_bids.end());
// 			sort(v_bids.begin(), v_bids.end());
// 			set_intersection(u_bids.begin(),u_bids.end(),
//                           v_bids.begin(),v_bids.end(),
//                           back_inserter(intersection));
// 			if(intersection.size()>0){
// 				truePairsCovered++;
// 			}
// 			for(int k=0; k<u_bids.size(); k++){
// 				blockRecIDMap[u_bids[k]].insert(sampledLinks[j].first);
// 			}
// 			for(int k=0; k<v_bids.size(); k++){
// 				blockRecIDMap[v_bids[k]].insert(sampledLinks[j].second);
// 			}

// 		}
// 		blockingPairCompleteness[i] = (double)(((double) truePairsCovered)/ ((double)sampledLinks.size()));
// 		long long int compAfterBlocking = 0;
// 		for(auto const& p : blockRecIDMap){
// 			long long n = p.second.size();
// 			long long int compForThisBlock = (n*(n-1))/2;
// 			compAfterBlocking += compForThisBlock;
// 		}

// 		blockingReductionRatios[i] = 1.0 - (double)(((double)compAfterBlocking) / ((double)totalPossibleComp));
// 		if(alteredAttrIndicesMap.count(i) == 1){
// 			blockingReductionRatios[i] = blockingReductionRatios[i] - (1 - blockingReductionRatios[i]);
// 		}
// 		cout<< "Attribute: " << i << " Reduction Ratio: " << blockingReductionRatios[i] << " Pair Completeness: "<< blockingPairCompleteness[i] << endl;
// 	}
// 	// Set Blocking attribute
// 	double max = 0.0;
// 	int max_bb_index = 0;
// 	vector<pair<double, int>> bbAttrPairList;

// 	// Set Sequence of Attributes to Compare
// 	for(int i=0; i<numAttrs; i++){
// 		pair<double, int> p;
// 		p.first = blockingReductionRatios[i];
// 		p.second = i;
// 		bbAttrPairList.push_back(p);
// 	}

// 	sort(bbAttrPairList.begin(), bbAttrPairList.end(), [](auto &left, auto &right) {
//     	return left.first > right.first;
// 	});

// 	for(int i=0; i<numAttrs; i++){
// 		cout<< "BRR: " << bbAttrPairList[i].first <<  " of Attr: " << bbAttrPairList[i].second << endl;
// 		sequenceOfAttrs.push_back(bbAttrPairList[i].second);
// 	}
// }


void estimateCompoundBlockingAttribute(vector<vector<string>> &vec2D, vector<pair<int, int>> &sampledLinks, int groundTruthIndex, int &blockingAttr, vector<int> &sequenceOfAttrs, map<int, int> &alteredAttrIndicesMap){
	int numAttrs = vec2D[0].size();
	vector<double> blockingReductionRatios;
	vector<double> blockingPairCompleteness;
	blockingReductionRatios.resize(numAttrs, 0.0);
	blockingPairCompleteness.resize(numAttrs, 0.0);
	int kmer = 6;
	int prefixLen = 1;
	long long int totalPossibleComp =  (sampledLinks.size() * (sampledLinks.size() - 1))/2;
	for(int i=0; i<numAttrs; i++){
		map<long long int, set<int>> blockRecIDMap;
		int truePairsCovered = 0;
		if(i == groundTruthIndex || i ==0) {
			continue;
		}
		for(int j=0; j<sampledLinks.size(); j++){
			vector<long long int> u_bids;
			vector<long long int> v_bids;
			vector<long long int> intersection;
			string blkstr1 = generateCompoundBlockingString(vec2D[sampledLinks[j].first], i, prefixLen, groundTruthIndex);
			string blkstr2 = generateCompoundBlockingString(vec2D[sampledLinks[j].second], i, prefixLen, groundTruthIndex);
			getSuperBlockingIDArray(kmer, blkstr1, u_bids);
			getSuperBlockingIDArray(kmer, blkstr2, v_bids);
			// getNormalBlockingIDVector(kmer, blkstr1, u_bids);
			// getNormalBlockingIDVector(kmer, blkstr2, v_bids);
			sort(u_bids.begin(), u_bids.end());
			sort(v_bids.begin(), v_bids.end());
			set_intersection(u_bids.begin(),u_bids.end(),
                          v_bids.begin(),v_bids.end(),
                          back_inserter(intersection));
			if(intersection.size()>0){
				truePairsCovered++;
			}
			for(int k=0; k<u_bids.size(); k++){
				blockRecIDMap[u_bids[k]].insert(sampledLinks[j].first);
			}
			for(int k=0; k<v_bids.size(); k++){
				blockRecIDMap[v_bids[k]].insert(sampledLinks[j].second);
			}
		}

		blockingPairCompleteness[i] = (double)(((double) truePairsCovered)/ ((double)sampledLinks.size()));
		long long int compAfterBlocking = 0;
		for(auto const& p : blockRecIDMap){
			long long n = p.second.size();
			long long int compForThisBlock = (n*(n-1))/2;
			compAfterBlocking += compForThisBlock;
		}

		blockingReductionRatios[i] = 1.0 - (double)(((double)compAfterBlocking) / ((double)totalPossibleComp));
		if(alteredAttrIndicesMap.count(i) == 1){
			blockingReductionRatios[i] = blockingReductionRatios[i] - (1 - blockingReductionRatios[i]);
		}
		cout<< "Attribute: " << i << " Reduction Ratio: " << blockingReductionRatios[i] << " Pair Completeness: "<< blockingPairCompleteness[i] << endl;
	}
	// Set Blocking attribute
	double max = 0.0;
	int max_bb_index = 0;
	vector<pair<double, int>> bbAttrPairList;
	for(int i=0; i<numAttrs; i++){
		if(blockingReductionRatios[i] > max){
			max = blockingReductionRatios[i];
			max_bb_index = i;
		}
	}
	blockingAttr = max_bb_index;


	// Set Sequence of Attributes to Compare
	for(int i=0; i<numAttrs; i++){
		pair<double, int> p;
		p.first = blockingReductionRatios[i];
		p.second = i;
		bbAttrPairList.push_back(p);
	}

	sort(bbAttrPairList.begin(), bbAttrPairList.end(), [](auto &left, auto &right) {
    	return left.first > right.first;
	});

	for(int i=0; i<numAttrs; i++){
		cout<< "BRR: " << bbAttrPairList[i].first <<  " of Attr: " << bbAttrPairList[i].second << endl;
		sequenceOfAttrs.push_back(bbAttrPairList[i].second);
	}
}


#endif