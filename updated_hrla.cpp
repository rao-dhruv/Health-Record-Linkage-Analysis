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
using namespace std;
// Function to convert string to tm
tm stringToDate(const string& dateStr) {
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%m%d%Y");
    return tm;
}

// Helper function to safely access map values
string getMapValue(const map<string, string>& record, const string& key) {
    auto it = record.find(key);
    if (it != record.end()) {
        return it->second;
    }
    return "";  // Return empty string if key not found
}

int main() {
    // Timing the process
    clock_t currTS_p1	= clock();
    
    // Reading cluster indices
    vector<vector<int>> clusters;
    ifstream inFile("/Users/dhruvrao/Desktop/Work/SequentialRLA/Out_recInds.csv");
    
    if (!inFile.is_open()) {
        cerr << "Failed to open file!" << endl;
        return 1;
    }
    
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        vector<int> indices;
        string token;
        
        while (getline(ss, token, ',')) {
            if (!token.empty() && all_of(token.begin(), token.end(), ::isdigit)) {
                indices.push_back(stoi(token));
            }
        }
        
        if (!indices.empty()) {
            clusters.push_back(indices);
        }
    }
    inFile.close();
    
    cout << "The total length of cluster is: " << clusters.size() << "\n";

    // Reading the main dataset
    vector<map<string, string>> mainData;
    ifstream dataFile("/Users/dhruvrao/Desktop/Work/SequentialRLA/data/updated_ds1_50k_fl");

    if (!dataFile.is_open()) {
        cerr << "Failed to open the dataset!" << endl;
        return 1;
    }

    vector<string> columns = {"SSN", "LN", "FN", "DOD", "DOB", "Provider", "BloodDate", "BloodCost", "XrayDate", "XrayCost", "DentalDate", "DentalCost"};
    string recordLine;
    while (getline(dataFile, recordLine)) {
        map<string, string> record;
        stringstream ss(recordLine);
        string token;
        int colIdx = 0;

        while (getline(ss, token, ',')) {
            record[columns[colIdx++]] = token;
        }
        mainData.push_back(record);
    }
    dataFile.close();

    // Perform cost analysis
    ofstream outFile("/Users/dhruvrao/Desktop/Work/SequentialRLA/data/cost1_updated_ds1_50k_fl");
    
    if (!outFile.is_open()) {
        cerr << "Failed to open output file!" << endl;
        return 1;
    }

    double total_blood_cost_sum = 0;
    double total_xray_cost_sum = 0;
    double total_dental_cost_sum = 0;

    for (size_t i = 0; i < clusters.size(); ++i) {
        vector<map<string, string>> clusterData;

        // Gathering data by index from the clusters
        for (int idx : clusters[i]) {
            clusterData.push_back(mainData[idx]);
        }

        // Sorting and calculating BloodCost savings
        sort(clusterData.begin(), clusterData.end(), [](const map<string, string>& a, const map<string, string>& b) {
            tm bloodDate1 = stringToDate(getMapValue(a, "BloodDate"));
            tm bloodDate2 = stringToDate(getMapValue(b, "BloodDate"));
            time_t time1 = mktime(&bloodDate1);
            time_t time2 = mktime(&bloodDate2);
            return time1 < time2;
        });

        for (size_t j = 0; j < clusterData.size() - 1; ++j) {
            tm current_date = stringToDate(getMapValue(clusterData[j], "BloodDate"));
            tm next_date = stringToDate(getMapValue(clusterData[j + 1], "BloodDate"));
            if (difftime(mktime(&next_date), mktime(&current_date)) <= 7 * 24 * 60 * 60) {
                clusterData[j]["BloodCostSaving"] = getMapValue(clusterData[j + 1], "BloodCost");
            }
        }

        // Sorting and calculating XrayCost savings
        sort(clusterData.begin(), clusterData.end(), [](const map<string, string>& a, const map<string, string>& b) {
            tm xrayDate1 = stringToDate(getMapValue(a, "XrayDate"));
            tm xrayDate2 = stringToDate(getMapValue(b, "XrayDate"));
            time_t time1 = mktime(&xrayDate1);
            time_t time2 = mktime(&xrayDate2);
            return time1 < time2;
        });

        for (size_t j = 0; j < clusterData.size() - 1; ++j) {
            tm current_date = stringToDate(getMapValue(clusterData[j], "XrayDate"));
            tm next_date = stringToDate(getMapValue(clusterData[j + 1], "XrayDate"));
            if (difftime(mktime(&next_date), mktime(&current_date)) <= 150 * 24 * 60 * 60) {
                clusterData[j]["XrayCostSaving"] = getMapValue(clusterData[j + 1], "XrayCost");
            }
        }

        // Sorting and calculating DentalCost savings
        sort(clusterData.begin(), clusterData.end(), [](const map<string, string>& a, const map<string, string>& b) {
            tm dentalDate1 = stringToDate(getMapValue(a, "DentalDate"));
            tm dentalDate2 = stringToDate(getMapValue(b, "DentalDate"));
            time_t time1 = mktime(&dentalDate1);
            time_t time2 = mktime(&dentalDate2);
            return time1 < time2;
        });

        for (size_t j = 0; j < clusterData.size() - 1; ++j) {
            tm current_date = stringToDate(getMapValue(clusterData[j], "DentalDate"));
            tm next_date = stringToDate(getMapValue(clusterData[j + 1], "DentalDate"));
            if (difftime(mktime(&next_date), mktime(&current_date)) <= 180 * 24 * 60 * 60) {
                clusterData[j]["DentalCostSaving"] = getMapValue(clusterData[j + 1], "DentalCost");
            }
        }

        // Summing up the cost savings for the current cluster
        double blood_cost_sum = 0;
        double xray_cost_sum = 0;
        double dental_cost_sum = 0;

        for (const auto& rec : clusterData) {
            if (!getMapValue(rec, "BloodCostSaving").empty()) {
                blood_cost_sum += stod(getMapValue(rec, "BloodCostSaving"));
            }
            if (!getMapValue(rec, "XrayCostSaving").empty()) {
                xray_cost_sum += stod(getMapValue(rec, "XrayCostSaving"));
            }
            if (!getMapValue(rec, "DentalCostSaving").empty()) {
                dental_cost_sum += stod(getMapValue(rec, "DentalCostSaving"));
            }
        }
        
        total_blood_cost_sum += blood_cost_sum;
        total_xray_cost_sum += xray_cost_sum;
        total_dental_cost_sum += dental_cost_sum;
        cout << fixed << setprecision(0) << noshowpoint;
        // Writing results to the file
        outFile << "Cluster " << i + 1 << " sorted records:\n";
        outFile << "\nTotal BloodCost Savings: " << blood_cost_sum << ", Total XrayCost Savings: " << xray_cost_sum << ", Total DentalCost Savings: " << dental_cost_sum << "\n\n";
    }

    outFile.close();

    double readRec_t	= (double)(clock() - currTS_p1) / CLOCKS_PER_SEC;
    cout << fixed << setprecision(0) << noshowpoint;
    cout << "Total Blood Cost saved = " << total_blood_cost_sum << "\n";
    cout << "Total Xray Cost saved = " << total_xray_cost_sum << "\n";
    cout << "Total Dental Cost saved = " << total_dental_cost_sum << "\n";
    cout << "Total time taken for the process is: " << readRec_t << " seconds\n";

    return 0;
}
