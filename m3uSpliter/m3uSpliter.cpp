#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <list>
#include <sys/stat.h>
#include <sys/types.h>

const std::string M3U_MAKE = "#EXTM3U";
const std::string M3U_INFO = "#EXTINF";
const std::string M3U_TAIL = ".m3u8";

const uint32_t M3U_INFO_LEN = 7;

bool isFileExist(const std::string& name) {
    return (access( name.c_str(), F_OK ) != -1 );
}

uint64_t getIPTVs(std::istream& iss, std::list<std::pair<std::string, std::string>>& iptvs) {
    std::string lineStr;
    std::string m3uInfo;
    std::string m3uUrl;

    while (!iss.eof())
    {
        std::getline(iss, m3uInfo);
        if (m3uInfo.substr(0, M3U_INFO_LEN) != M3U_INFO) {
            continue;
        }

        std::getline(iss, m3uUrl);
        if (m3uUrl.rfind(M3U_TAIL.c_str()) != m3uUrl.size() - 5) {
            continue;
        }

        iptvs.push_back(std::pair<std::string, std::string>(m3uInfo, m3uUrl));
    }
    
    return iptvs.size();
}

bool CreateDir(const std::string& dirName) {
    return mkdir(dirName.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IRWXO) == 0;
}

uint32_t toM3uFiles(const std::string& dirName, const std::list<std::pair<std::string, std::string>>& iptvs) {
    std::string m3uName;
    for (const auto& pairM3u : iptvs) {
        m3uName = pairM3u.first;
        m3uName = m3uName.substr(m3uName.rfind(',') + 1);
        m3uName += ".m3u";
        if (!dirName.empty()) {
            CreateDir(dirName);
            m3uName = dirName + "/" + m3uName;
        }

        std::ofstream ofile(m3uName.c_str());
        if (!ofile.is_open()) {
            continue;
        }
        ofile << M3U_MAKE << std::endl;
        ofile << pairM3u.first << std::endl;
        ofile << pairM3u.second << std::endl;

        ofile.close();
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "-- Error: please input m3u file name." << std::endl;
        return -1;
    }

    std::string dirName;
    if (argc > 2) {
        dirName = argv[2];
    }

    std::string fName = argv[1];
    if (!isFileExist(fName)) {
        std::cout << "-- Error: file not exist." << std::endl;
        return -1;
    }

    std::ifstream ifile(fName);
    if (!ifile.is_open()) {
        std::cout << "-- Error: open file failed." << std::endl;
        return -1;
    }

    std::list<std::pair<std::string, std::string>> iptvList;
    uint64_t iptvNum = getIPTVs(ifile, iptvList);
    std::cout << "-- IPTVs: " << iptvNum << std::endl;

    ifile.close();

    toM3uFiles(dirName, iptvList);

    return 0;
}