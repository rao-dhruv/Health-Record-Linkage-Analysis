#ifndef RECORD_COMPARATOR_H
#define RECORD_COMPARATOR_H

#include "unionFind.h"
#include <map>
#include <vector>
#include <iostream>

using namespace std;

class RecordComparator{
    public:
        int threshold = 99;
        int cumulativeDistanceThreshold = 0;
        vector<int> attrDistThreshold;
        int matArr[100][100] = {0};
        int attributes = 0;
        // For Census Synth Datasets
        double global_t = 0.0;
        vector<double> attrThresholds;
        vector<vector<double>> weights;
        vector<vector<double>> missingValWeights;
        vector<int> attrSequence;
        vector<int> comparingAttrs;
        map<int, int> alteredAttrInds;
        int gTruthInd = -1;

        void setComparator(int gThreshold, vector<int>& perAttrThreshold){
            this->cumulativeDistanceThreshold = gThreshold;
            this->attrDistThreshold = perAttrThreshold;
            this->attributes = attrDistThreshold.size();
        }

        void setComparator(int groundTruthIndex, int gThreshold, vector<int>& perAttrThreshold){
            this->cumulativeDistanceThreshold = gThreshold;
            this->attrDistThreshold = perAttrThreshold;
            this->attributes = attrDistThreshold.size();
            this->gTruthInd = groundTruthIndex;
        }

        void setComparator(int groundTruthIndex, int gThreshold, vector<int>& perAttrThreshold, vector<int> &comparingAttrs){
            this->cumulativeDistanceThreshold = gThreshold;
            this->attrDistThreshold = perAttrThreshold;
            // this->attributes = attrDistThreshold.size();
            this->gTruthInd = groundTruthIndex;
            this->comparingAttrs = comparingAttrs;
        }

        void setComparator(double global_t, vector<double> &attrThresholds, vector<int> &sequenceOfAttrToCompare, map<int, int> &alteredAttrIndices, int groundTruthIndex){
            this->global_t = global_t;
            this->gTruthInd = groundTruthIndex;
            this->attributes = attrThresholds.size();
            this->attrThresholds = attrThresholds;
            this->attrSequence = sequenceOfAttrToCompare;
            this->alteredAttrInds = alteredAttrIndices;
        }

        void setComparator(double global_t, vector<double> &attrThresholds, vector<int> &sequenceOfAttrToCompare, map<int, int> &alteredAttrIndices, int groundTruthIndex, vector<vector<double>> weights){
            this->global_t = global_t;
            this->gTruthInd = groundTruthIndex;
            this->attributes = attrThresholds.size();
            this->attrThresholds = attrThresholds;
            this->attrSequence = sequenceOfAttrToCompare;
            this->alteredAttrInds = alteredAttrIndices;
            this->weights = weights;
        }

        void setComparator(double global_t, vector<double> &attrThresholds, vector<int> &sequenceOfAttrToCompare, map<int, int> &alteredAttrIndices, int groundTruthIndex, vector<vector<double>> weights, vector<vector<double>> missingValWeights){
            this->global_t = global_t;
            this->gTruthInd = groundTruthIndex;
            this->attributes = attrThresholds.size();
            this->attrThresholds = attrThresholds;
            this->attrSequence = sequenceOfAttrToCompare;
            this->alteredAttrInds = alteredAttrIndices;
            this->weights = weights;
            this->missingValWeights = missingValWeights;
        }

        int calculateBasicED2(string &str1, string &str2, int threshRem){
            int row, col, i, j;
            row = str1.length() + 1;
            col = str2.length() + 1;

            for (i = 0; i < row; i++)
            {
                for (j = 0; j < col; j++)
                {
                    if (i == 0)
                        matArr[i][j] = j;
                    else if (j == 0)
                        matArr[i][j] = i;
                    else
                    {
                        if ((int)str1[i - 1] == (int)str2[j - 1])
                            matArr[i][j] = min(min(matArr[i - 1][j - 1], matArr[i - 1][j] + 1), matArr[i][j - 1] + 1);
                        else
                            matArr[i][j] = min(min(matArr[i - 1][j - 1] + 1, matArr[i - 1][j] + 1), matArr[i][j - 1] + 1);
                            // if(i>1 && j>1) {
                            // 	if((str1[i-1]==str2[j-2]) && (str1[i-2] == str2[j-1])) {
                            // 		matArr[i][j] = min(matArr[i][j], matArr[i-2][j-2]+1);
                            // 	}
                            // }
                    }

                    if ((row - col) == (i - j) && (matArr[i][j] > threshRem))
                    {
                        return threshold + 1;
                    }
                }
            }
            
            return (matArr[row - 1][col - 1]);
        }

        int calculateBasicED(string &str1, string &str2, int threshRem){
            int dist = threshRem;

            if (abs((int)(str1.length() - str2.length())) > dist)
                return threshold + 1;
            else if ((2 * dist + 1) >= max(str1.length(), str2.length()))
                return calculateBasicED2(str1, str2, dist);
            else
            {
                
                string s1, s2;
                int row, col, diagonal;
                int i, j;

                if (str1.length() > str2.length()){
                    s1 = str2;
                    s2 = str1;
                } else {
                    s1 = str1;
                    s2 = str2;
                }

                row = s1.length() + 1;
                col = 2 * dist + 1;
                diagonal = dist + s2.length() - s1.length();

                for (i = 0; i < dist + 1; i++)
                {
                    for (j = dist - i; j < col; j++)
                    {
                        if (i == 0)
                            matArr[i][j] = j - dist;
                        else if (j == (dist - i))
                            matArr[i][j] = matArr[i - 1][j + 1] + 1;
                        else if (j != (col - 1))
                        {
                            if ((int)s1[i - 1] == (int)s2[j - (dist - i) - 1])
                                matArr[i][j] = min(min(matArr[i - 1][j], matArr[i - 1][j + 1] + 1), matArr[i][j - 1] + 1);
                            else {
                                matArr[i][j] = min(min(matArr[i - 1][j] + 1, matArr[i - 1][j + 1] + 1), matArr[i][j - 1] + 1);
                            }
                                    
                        }
                        else
                        {
                            if ((int)s1[i - 1] == (int)s2[j - (dist - i) - 1])
                                matArr[i][j] = min(matArr[i - 1][j], matArr[i][j - 1] + 1);
                            else
                                matArr[i][j] = min(matArr[i - 1][j] + 1, matArr[i][j - 1] + 1);
                        }

                        if ((j == diagonal) && matArr[i][j] > dist)
                            return threshold + 1;
                    }
                }

                for (i = dist + 1; i < s2.length() - dist + 1; i++)
                {
                    for (j = 0; j < col; j++)
                    {
                        if (j == 0)
                        {
                            if ((int)s1[i - 1] == (int)s2[j + (i - dist) - 1])
                                matArr[i][j] = min(matArr[i - 1][j], matArr[i - 1][j + 1] + 1);
                            else
                                matArr[i][j] = min(matArr[i - 1][j] + 1, matArr[i - 1][j + 1] + 1);
                        }
                        else if (j != (col - 1))
                        {
                            if ((int)s1[i - 1] == (int)s2[j + (i - dist) - 1])
                                matArr[i][j] = min(min(matArr[i - 1][j], matArr[i - 1][j + 1] + 1), matArr[i][j - 1] + 1);
                            else
                                matArr[i][j] = min(min(matArr[i - 1][j] + 1, matArr[i - 1][j + 1] + 1), matArr[i][j - 1] + 1);
                        }
                        else
                        {
                            if ((int)s1[i - 1] == (int)s2[j + (i - dist) - 1])
                                matArr[i][j] = min(matArr[i - 1][j], matArr[i][j - 1] + 1);
                            else
                                matArr[i][j] = min(matArr[i - 1][j] + 1, matArr[i][j - 1] + 1);
                        }
                        if ((j == diagonal) && (matArr[i][j] > dist))
                            return threshold + 1;
                    }
                }

                for (i = s2.length() - dist + 1; i < row; i++)
                {
                    for (j = 0; j < col - i + s2.length() - dist; j++)
                    {
                        if (j == 0)
                        {
                            if ((int)s1[i - 1] == (int)s2[j + (i - dist) - 1])
                                matArr[i][j] = min(matArr[i - 1][j], matArr[i - 1][j + 1] + 1);
                            else
                                matArr[i][j] = min(matArr[i - 1][j] + 1, matArr[i - 1][j + 1] + 1);
                        }
                        else
                        {
                            if ((int)s1[i - 1] == (int)s2[j + (i - dist) - 1])
                                matArr[i][j] = min(min(matArr[i - 1][j], matArr[i - 1][j + 1] + 1), matArr[i][j - 1] + 1);
                            else
                                matArr[i][j] = min(min(matArr[i - 1][j] + 1, matArr[i - 1][j + 1] + 1), matArr[i][j - 1] + 1);
                        }
                        if ((j == diagonal) && (matArr[i][j] > dist))
                            return threshold + 1;
                    }
                }
                return matArr[row - 1][diagonal];
            }
        }

        bool isLinkageOk(vector<string> &a, vector<string> &b)
        {
            int cumulativeDist = 0;
            for (int i = 0; i < comparingAttrs.size(); i++)
            {
                int dist = calculateBasicED(a[comparingAttrs[i]], b[comparingAttrs[i]], this->attrDistThreshold[i]);
                if (dist > this->attrDistThreshold[i]){
                    return false;
                } else {
                    cumulativeDist += dist;
                    if (cumulativeDist > this->cumulativeDistanceThreshold){
                        return false;
                    }
                }
            }
            return true;
        }

        bool isLinkageOk(string &a, string &b)
        {
            int dist = calculateBasicED(a, b, this->cumulativeDistanceThreshold);
            if (dist > this->cumulativeDistanceThreshold){
                return false;
            }
            return true;
        }

        bool isLinkageOk(vector<string> &a, vector<string> &b, int rec_i, int rec_j, vector<vector<int>> &parentArrVec)
        {
            int cumulativeDist = 0;
            for (int i = 1; i < parentArrVec.size(); i++)
            {	
                if (parentArrVec[i][rec_i] != parentArrVec[i][rec_j]){
                    return false;
                }
            }
            return this->isLinkageOk(a,b);
        }

        bool isLinkageOk(vector<string> &a, vector<string> &b, int mode)
        {
            double tot_dist = 0.0;
            double costWeight = 0.6;

            if (mode == 1) {
                
                for (int i = 0; i < attributes; i++)
                {	
                    if(attrSequence[i] == gTruthInd){
                        continue;
                    }
                    if(a[attrSequence[i]].size() == 0 || b[attrSequence[i]].size() == 0){
                        tot_dist += ((double)this->attrThresholds[attrSequence[i]])*costWeight;
                        continue;
                    }
                    if(alteredAttrInds.count(attrSequence[i]) > 0){
                        if (a[attrSequence[i]].size() == 0 || b[attrSequence[i]].size() == 0 || b[alteredAttrInds[attrSequence[i]]].size() == 0){
                            tot_dist += ((double)this->attrThresholds[attrSequence[i]])*costWeight;
                            continue;
                        }
                        double dist1 = (double)calculateBasicED(a[attrSequence[i]], b[attrSequence[i]], this->attrThresholds[attrSequence[i]]);
                        double dist2 = (double)calculateBasicED(a[attrSequence[i]], b[alteredAttrInds[attrSequence[i]]], this->attrThresholds[attrSequence[i]]);
                        double dist = min(dist1, dist2);
                        if(dist <= this->attrThresholds[attrSequence[i]]){
                            tot_dist += dist;
                        } else {
                            return false;
                        }
                    } else {
                        double dist = calculateBasicED(a[attrSequence[i]], b[attrSequence[i]], this->attrThresholds[attrSequence[i]]);
                        if(dist <= this->attrThresholds[attrSequence[i]]){
                            tot_dist += dist;
                        } else {
                            return false;
                        }
                    }
                    if(tot_dist > global_t){
                        return false;
                    }
                }
                if(tot_dist > global_t){
                    return false;
                }
            } else if (mode == 2) { // weighted medium file case
                for (int i = 0; i < attributes; i++)
                {	
                    if(attrSequence[i] == gTruthInd){
                        continue;
                    }
                    if(alteredAttrInds.count(attrSequence[i]) > 0){
                        double dist1 = (double)calculateBasicED(a[attrSequence[i]], b[attrSequence[i]], this->attrThresholds[attrSequence[i]]);
                        double dist2 = (double)calculateBasicED(a[attrSequence[i]], b[alteredAttrInds[attrSequence[i]]], this->attrThresholds[attrSequence[i]]);
                        double dist = min(dist1, dist2);
                        if(dist <= this->attrThresholds[attrSequence[i]]){
                            tot_dist += weights[i][dist];
                        } else {
                            return false;
                        }
                    } else {
                        double dist = calculateBasicED(a[attrSequence[i]], b[attrSequence[i]], this->attrThresholds[attrSequence[i]]);
                        if(dist <= this->attrThresholds[attrSequence[i]]){
                            tot_dist += weights[i][dist];
                        } else {
                            return false;
                        }
                    }
                }
                if(tot_dist >= global_t){
                    return true;
                } else {
                    return false;
                }
            } else if (mode == 3) { // weighted Hard file case
                for (int i = 0; i < attributes; i++)
                {	
                    if(attrSequence[i] == gTruthInd){
                        continue;
                    }
                    if(a[attrSequence[i]].size() == 0 && b[attrSequence[i]].size() == 0){
                        tot_dist += missingValWeights[attrSequence[i]][2];
                        continue;
                    }
                    if(a[attrSequence[i]].size() == 0){
                        tot_dist += missingValWeights[attrSequence[i]][1];
                        continue;
                    }
                    if (b[attrSequence[i]].size() == 0){
                        tot_dist += missingValWeights[attrSequence[i]][0];
                        continue;
                    }
                    if(alteredAttrInds.count(attrSequence[i]) > 0){
                        if (b[alteredAttrInds[attrSequence[i]]].size() == 0){
                            tot_dist += missingValWeights[attrSequence[i]][0];
                            continue;
                        }
                        double dist1 = (double)calculateBasicED(a[attrSequence[i]], b[attrSequence[i]], this->attrThresholds[attrSequence[i]]);
                        double dist2 = (double)calculateBasicED(a[attrSequence[i]], b[alteredAttrInds[attrSequence[i]]], this->attrThresholds[attrSequence[i]]);
                        double dist = min(dist1, dist2);
                        if(dist <= this->attrThresholds[attrSequence[i]]){
                            tot_dist += weights[i][dist];
                        } else {
                            return false;
                        }
                    } else {
                        double dist = calculateBasicED(a[attrSequence[i]], b[attrSequence[i]], this->attrThresholds[attrSequence[i]]);
                        if(dist <= this->attrThresholds[attrSequence[i]]){
                            tot_dist += weights[i][dist];
                        } else {
                            return false;
                        }
                    }
                }
                if(tot_dist >= global_t){
                    return true;
                } else {
                    return false;
                }
            }
            return true;
        }
};



#endif