//
// Created by Jayden on 14/03/2024.
//

#include "CSVWriter.h"
#include <string>
#include <filesystem>

CSVWriter::CSVWriter(const std::string &filename) {

    std::string baseDir = "./out/";
    std::filesystem::create_directories(baseDir);
    std::string filePath = baseDir + filename;

    outFile.open(filePath);
}

CSVWriter::~CSVWriter() {
    if (outFile.is_open()) {
        outFile.close();
    }
}

void CSVWriter::writeRow(const std::vector<std::string> &row) {
    for (size_t i = 0; i < row.size(); i++) {
        outFile << row[i];
        if (i < row.size() - 1) {
            outFile << ",";
        }
    }
    outFile << "\n";
}
