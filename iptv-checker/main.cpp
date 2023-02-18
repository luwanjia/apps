#include <cstdint>
#include <string>
#include <curl/curl.h>
#include <iostream>

size_t write_cb(char* ptr, size_t size, size_t nmemb, void* userdata) {


    return size * nmemb;
}


int32_t func(const std::string& url, const uint32_t tm_expire) {
    // 1. 创建一个curl句柄
    CURL* curl = nullptr;
    CURLcode res = CURLE_OK;

    // 2. 初始化curl句柄
    curl = curl_easy_init();
    if (curl == nullptr) {
        std::cout << "-- error: curl_easy_init failed." << std::endl;
        return -1;
    }

    // 3. 设置参数
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, url.c_str());

    // 0 disable messages
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, &tm_expire);

    // 禁止DNS缓存
    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 0L);


    res = curl_easy_perform(curl);
    double time_namelookup = 0;
    double time_connect = 0;
    double time_starttransfer = 0;
    double time_total = 0;
    char* ip = nullptr;
    uint32_t port = 0;
    int32_t http_code = 0;
    if(res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &time_namelookup);
        curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &time_connect);
        curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &time_starttransfer);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time_total);
        curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip);
        curl_easy_getinfo(curl, CURLINFO_PRIMARY_PORT, &port);

        std::cout << "-- info: http_code = " << http_code << ", time_namelookup = " << time_namelookup << ", time_connect = " << time_connect << ", time_starttransfer = " << time_starttransfer \
          << ", time_total = " << time_total << std::endl;
        std::cout << "-- info: ip = " << ip << ", port = " << port << std::endl;
    }

    curl_easy_cleanup(curl);

    return http_code;
}
int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "-- Usage: iptv-checker <url> <timeout>" << std::endl;
        return -1;
    }

    std::string url = argv[1];
    uint32_t tm_expire = atoi(argv[2]);
    
    int32_t http_code = 0;
    http_code = func(url, tm_expire);

    std::cout << "-- http_code = " << http_code << std::endl;

    return 0;
}
