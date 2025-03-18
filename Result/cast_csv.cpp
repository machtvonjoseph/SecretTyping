#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void processCSV(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error: Unable to open input file!\n";
        return;
    }

    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Error: Unable to create output file!\n";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string value;

        // Read all columns
        while (std::getline(ss, value, ',')) {
            fields.push_back(value);
        }

        // Ensure there are at least 3 columns to modify
        size_t numCols = fields.size();
        if (numCols >= 3) {
            for (size_t i = numCols - 3; i < numCols; ++i) {
                try {
                    int signedVal = std::stoll(fields[i]); // Convert to signed int
                    uint64_t unsignedVal = static_cast<uint64_t>(signedVal); // Cast to unsigned
                    fields[i] = std::to_string(unsignedVal); // Replace with unsigned value
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Failed to convert " << fields[i] << " to unsigned integer.\n";
                }
            }
        }

        // Write modified line to output file
        for (size_t i = 0; i < fields.size(); ++i) {
            outFile << fields[i];
            if (i < fields.size() - 1) outFile << ","; // Keep CSV format
        }
        outFile << "\n";
    }

    std::cout << "Processed CSV saved to: " << outputFile << "\n";
}

int main() {
    std::string inputFile = "bst_time_sweep.csv";  // Input CSV file name
    std::string outputFile = "bst_time_sweep_casted.csv"; // Output CSV file name

    processCSV(inputFile, outputFile);

    return 0;
}