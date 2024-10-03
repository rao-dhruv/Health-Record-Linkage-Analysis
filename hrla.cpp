#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <chrono>
#include <algorithm>

// Function to convert string to std::tm
std::tm stringToDate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%m%d%Y");
    return tm;
}

// Helper function to safely access map values
std::string getMapValue(const std::map<std::string, std::string>& record, const std::string& key) {
    auto it = record.find(key);
    if (it != record.end()) {
        return it->second;
    }
    return "";  // Return empty string if key not found
}

int main() {
    // Timing the process
    auto start = std::chrono::high_resolution_clock::now();
    
    // Reading cluster indices
    std::vector<std::vector<int>> clusters;
    std::ifstream inFile("/Users/dhruvrao/Desktop/Work/SequentialRLA/Out_recInds.csv");
    
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::vector<int> indices;
        std::string token;
        
        while (std::getline(ss, token, ',')) {
            if (!token.empty() && std::all_of(token.begin(), token.end(), ::isdigit)) {
                indices.push_back(std::stoi(token));
            }
        }
        
        if (!indices.empty()) {
            clusters.push_back(indices);
        }
    }
    inFile.close();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto work1 = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "Time taken for cluster making is: " << work1 << " seconds\n";
    std::cout << "The total length of cluster is: " << clusters.size() << "\n";

    // Reading the main dataset
    std::vector<std::map<std::string, std::string>> mainData;
    std::ifstream dataFile("/Users/dhruvrao/Desktop/Work/SequentialRLA/data/updated_ds1_50k_fl");

    if (!dataFile.is_open()) {
        std::cerr << "Failed to open the dataset!" << std::endl;
        return 1;
    }

    std::vector<std::string> columns = {"SSN", "LN", "FN", "DOD", "DOB", "Provider", "BloodDate", "BloodCost", "XrayDate", "XrayCost", "DentalDate", "DentalCost"};
    std::string recordLine;
    while (std::getline(dataFile, recordLine)) {
        std::map<std::string, std::string> record;
        std::stringstream ss(recordLine);
        std::string token;
        int colIdx = 0;

        while (std::getline(ss, token, ',')) {
            record[columns[colIdx++]] = token;
        }
        mainData.push_back(record);
    }
    dataFile.close();

    // Perform cost analysis
    std::ofstream outFile("/Users/dhruvrao/Desktop/Work/SequentialRLA/data/cost1_updated_ds1_50k_fl");
    
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file!" << std::endl;
        return 1;
    }

    double total_blood_cost_sum = 0;
    double total_xray_cost_sum = 0;
    double total_dental_cost_sum = 0;

    start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < clusters.size(); ++i) {
        std::vector<std::map<std::string, std::string>> clusterData;

        // Gathering data by index from the clusters
        for (int idx : clusters[i]) {
            clusterData.push_back(mainData[idx]);
        }

        // Sorting and calculating BloodCost savings
        std::sort(clusterData.begin(), clusterData.end(), [](const std::map<std::string, std::string>& a, const std::map<std::string, std::string>& b) {
            std::tm bloodDate1 = stringToDate(getMapValue(a, "BloodDate"));
            std::tm bloodDate2 = stringToDate(getMapValue(b, "BloodDate"));
            std::time_t time1 = std::mktime(&bloodDate1);
            std::time_t time2 = std::mktime(&bloodDate2);
            return time1 < time2;
        });

        for (size_t j = 0; j < clusterData.size() - 1; ++j) {
            std::tm current_date = stringToDate(getMapValue(clusterData[j], "BloodDate"));
            std::tm next_date = stringToDate(getMapValue(clusterData[j + 1], "BloodDate"));
            if (std::difftime(std::mktime(&next_date), std::mktime(&current_date)) <= 7 * 24 * 60 * 60) {
                clusterData[j]["BloodCostSaving"] = getMapValue(clusterData[j + 1], "BloodCost");
            }
        }

        // Sorting and calculating XrayCost savings
        std::sort(clusterData.begin(), clusterData.end(), [](const std::map<std::string, std::string>& a, const std::map<std::string, std::string>& b) {
            std::tm xrayDate1 = stringToDate(getMapValue(a, "XrayDate"));
            std::tm xrayDate2 = stringToDate(getMapValue(b, "XrayDate"));
            std::time_t time1 = std::mktime(&xrayDate1);
            std::time_t time2 = std::mktime(&xrayDate2);
            return time1 < time2;
        });

        for (size_t j = 0; j < clusterData.size() - 1; ++j) {
            std::tm current_date = stringToDate(getMapValue(clusterData[j], "XrayDate"));
            std::tm next_date = stringToDate(getMapValue(clusterData[j + 1], "XrayDate"));
            if (std::difftime(std::mktime(&next_date), std::mktime(&current_date)) <= 150 * 24 * 60 * 60) {
                clusterData[j]["XrayCostSaving"] = getMapValue(clusterData[j + 1], "XrayCost");
            }
        }

        // Sorting and calculating DentalCost savings
        std::sort(clusterData.begin(), clusterData.end(), [](const std::map<std::string, std::string>& a, const std::map<std::string, std::string>& b) {
            std::tm dentalDate1 = stringToDate(getMapValue(a, "DentalDate"));
            std::tm dentalDate2 = stringToDate(getMapValue(b, "DentalDate"));
            std::time_t time1 = std::mktime(&dentalDate1);
            std::time_t time2 = std::mktime(&dentalDate2);
            return time1 < time2;
        });

        for (size_t j = 0; j < clusterData.size() - 1; ++j) {
            std::tm current_date = stringToDate(getMapValue(clusterData[j], "DentalDate"));
            std::tm next_date = stringToDate(getMapValue(clusterData[j + 1], "DentalDate"));
            if (std::difftime(std::mktime(&next_date), std::mktime(&current_date)) <= 180 * 24 * 60 * 60) {
                clusterData[j]["DentalCostSaving"] = getMapValue(clusterData[j + 1], "DentalCost");
            }
        }

        // Summing up the cost savings for the current cluster
        double blood_cost_sum = 0;
        double xray_cost_sum = 0;
        double dental_cost_sum = 0;

        for (const auto& rec : clusterData) {
            if (!getMapValue(rec, "BloodCostSaving").empty()) {
                blood_cost_sum += std::stod(getMapValue(rec, "BloodCostSaving"));
            }
            if (!getMapValue(rec, "XrayCostSaving").empty()) {
                xray_cost_sum += std::stod(getMapValue(rec, "XrayCostSaving"));
            }
            if (!getMapValue(rec, "DentalCostSaving").empty()) {
                dental_cost_sum += std::stod(getMapValue(rec, "DentalCostSaving"));
            }
        }
        
        total_blood_cost_sum += blood_cost_sum;
        total_xray_cost_sum += xray_cost_sum;
        total_dental_cost_sum += dental_cost_sum;

        // Writing results to the file
        outFile << "Cluster " << i + 1 << " sorted records:\n";
        outFile << "\nTotal BloodCost Savings: " << blood_cost_sum << ", Total XrayCost Savings: " << xray_cost_sum << ", Total DentalCost Savings: " << dental_cost_sum << "\n\n";
    }

    outFile.close();

    end = std::chrono::high_resolution_clock::now();
    auto work2 = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    std::cout << "Total Blood Cost saved = " << total_blood_cost_sum << "\n";
    std::cout << "Total Xray Cost saved = " << total_xray_cost_sum << "\n";
    std::cout << "Total Dental Cost saved = " << total_dental_cost_sum << "\n";
    std::cout << "Time taken for Analyzing all Cost Work is: " << work2 << " seconds\n";
    std::cout << "Total time taken for the process is: " << work1 + work2 << " seconds\n";

    return 0;
}