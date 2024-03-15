//
// Created by Jayden on 14/03/2024.
//

#ifndef FILTERS_CSVWRITER_H
#define FILTERS_CSVWRITER_H

#include <fstream>
#include <iostream>
#include <string>

class CSVWriter {
private:
    std::ofstream outFile;
public:
    CSVWriter(const std::string& filename);

    ~CSVWriter();

    void writeRow(const std::vector<std::string>& row);
};


#endif //FILTERS_CSVWRITER_H
