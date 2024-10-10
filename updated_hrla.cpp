#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;

// Converting string to a tm date
tm stringToDate(const string& dateStr) {
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%m%d%Y");
    return tm;
}

string getMapValue(const map<string, string>& record, const string& key) {
    auto it = record.find(key);
    if (it != record.end()) {
        return it->second;
    }
    return "";
}

int main() {
    // Time Start
    clock_t currTS_p1	= clock();

    // Reading the main dataset
    // columns = {"SSN", "LN", "FN", "DOD", "DOB", "Provider", "BloodDate", "BloodCost", "XrayDate", "XrayCost", "DentalDate", "DentalCost"};
    string dataFile = "/Users/dhruvrao/Desktop/Work/SequentialRLA/data/updated_ds1.1.1";
    //Reading Cluster
    string inFile = "/Users/dhruvrao/Desktop/Work/SequentialRLA/Out_recInds.csv";

    int bloodDateCol = 6;
    int xRayDateCol = 8;
    int dentalDateCol = 10;
    string line;
    vector<vector<string>> mainData;
    vector<vector<vector<pair<string, int>>>> clusters;

    // Read the main dataset
    ifstream records(dataFile);
    while (getline(records, line)) {
        vector<string> result;
        boost::split(result, line, boost::is_any_of(","));
        vector<string> vec;
        for(int i=0; i<result.size(); i++) {
            auto last = remove_if(result[i].begin(), result[i].end(), [](char ch) {
                return ::ispunct(ch) || ::iswpunct(ch) || ::isspace(ch);
            });
            result[i].erase(last, result[i].end());
            result[i].erase(remove_if(result[i].begin(), result[i].end(), [](unsigned char c){return !(c>=0 && c <128);}), result[i].end());

            boost::to_lower(result[i]);
            vec.push_back(result[i]);
        }
        mainData.push_back(vec);
    }
    records.close();

    // Read the indices from the file
    ifstream indexf(inFile);
    while (getline(indexf, line)) {
        vector<string> indices;
        boost::split(indices, line, boost::is_any_of(","));
        vector<pair<string, int>> bloodInfo;
        vector<pair<string, int>> xrayInfo;
        vector<pair<string, int>> dentalInfo;
        vector<vector<pair<string, int>>> cluster;
        for(int i=0; i<indices.size(); i++) {
            if(indices[i].size() > 0) {
                pair<string, int> bloodTest;
                bloodTest.first = mainData[stoi(indices[i])][bloodDateCol];
                bloodTest.second = stoi(mainData[stoi(indices[i])][bloodDateCol+1]);
                bloodInfo.push_back(bloodTest);

                pair<string, int> xrayTest;
                xrayTest.first = mainData[stoi(indices[i])][xRayDateCol];
                xrayTest.second = stoi(mainData[stoi(indices[i])][xRayDateCol+1]);
                xrayInfo.push_back(xrayTest);

                pair<string, int> dentalTest;
                dentalTest.first = mainData[stoi(indices[i])][dentalDateCol];
                dentalTest.second = stoi(mainData[stoi(indices[i])][dentalDateCol+1]);
                dentalInfo.push_back(dentalTest);
            }
        }
        cluster.push_back(bloodInfo);
        cluster.push_back(xrayInfo);
        cluster.push_back(dentalInfo);
        clusters.push_back(cluster);
    }
    indexf.close();

    // Output Cost File
    ofstream outFile("/Users/dhruvrao/Desktop/Work/SequentialRLA/data/cost1");

    if (!outFile.is_open()) {
        cerr << "Failed to open output file!" << endl;
        return 1;
    }

    double total_blood_cost_sum = 0;
    double total_xray_cost_sum = 0;
    double total_dental_cost_sum = 0;

    vector<int> bloodSavings;
    vector<int> xRaySavings;
    vector<int> dentalSavings;

    for (size_t i = 0; i < clusters.size(); ++i) {
        int bloodSave = 0;
        vector<pair<string, int>> bloodInfo = clusters[i][0];
        sort(bloodInfo.begin(), bloodInfo.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            tm dateA = stringToDate(a.first);  // Storing in local variables
            tm dateB = stringToDate(b.first);
            return mktime(&dateA) < mktime(&dateB);  // Using local variables
        });

        for (size_t j = 0; j < bloodInfo.size() - 1; ++j) {
            // Date difference logic for blood tests (within 7 days)
            tm current_date = stringToDate(bloodInfo[j].first);
            tm next_date = stringToDate(bloodInfo[j+1].first);
            double diff_seconds = difftime(mktime(&next_date), mktime(&current_date));
            if (diff_seconds <= 7 * 24 * 60 * 60) {
                bloodSave += bloodInfo[j+1].second;
            }
        }
        bloodSavings.push_back(bloodSave);

        int xRaySave = 0;
        vector<pair<string, int>> xRayInfo = clusters[i][1];
        sort(xRayInfo.begin(), xRayInfo.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            tm dateA = stringToDate(a.first);  
            tm dateB = stringToDate(b.first);
            return mktime(&dateA) < mktime(&dateB);
        });

        for (size_t j = 0; j < xRayInfo.size() - 1; ++j) {
            // Date difference logic for X-rays (within 150 days)
            tm current_date = stringToDate(xRayInfo[j].first);
            tm next_date = stringToDate(xRayInfo[j+1].first);
            double diff_seconds = difftime(mktime(&next_date), mktime(&current_date));
            if (diff_seconds <= 150 * 24 * 60 * 60) {
                xRaySave += xRayInfo[j+1].second;
            }
        }
        xRaySavings.push_back(xRaySave);

        int dentalSave = 0;
        vector<pair<string, int>> dentalInfo = clusters[i][2];
        sort(dentalInfo.begin(), dentalInfo.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            tm dateA = stringToDate(a.first);  
            tm dateB = stringToDate(b.first);
            return mktime(&dateA) < mktime(&dateB);
        });

        for (size_t j = 0; j < dentalInfo.size() - 1; ++j) {
            // Date difference logic for dental tests (within 180 days)
            tm current_date = stringToDate(dentalInfo[j].first);
            tm next_date = stringToDate(dentalInfo[j+1].first);
            double diff_seconds = difftime(mktime(&next_date), mktime(&current_date));
            if (diff_seconds <= 180 * 24 * 60 * 60) {
                dentalSave += dentalInfo[j+1].second;
            }
        }
        dentalSavings.push_back(dentalSave);

        total_blood_cost_sum += bloodSave;
        total_xray_cost_sum += xRaySave;
        total_dental_cost_sum += dentalSave;

        // Writing results to the file
        outFile << "Cluster " << i + 1 << " sorted records:\n";
        outFile << "\nTotal Blood Cost Savings: " << bloodSave << ", Total X-ray Cost Savings: " << xRaySave << ", Total Dental Cost Savings: " << dentalSave << "\n\n";
    }

    outFile.close();

    double readRec_t = (double)(clock() - currTS_p1) / CLOCKS_PER_SEC;
    cout << fixed << setprecision(0) << noshowpoint;
    cout << "Total Blood Cost saved = " << total_blood_cost_sum << "\n";
    cout << "Total X-ray Cost saved = " << total_xray_cost_sum << "\n";
    cout << "Total Dental Cost saved = " << total_dental_cost_sum << "\n";
    cout << "Total time taken for the process is: " << readRec_t << " seconds\n";

    return 0;
}
