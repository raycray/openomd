#include <iostream>
#include "cxxopts.hpp"
#include "omdprintprocessor.h"

uint32_t convertIp(const char* ip)
{
    struct in_addr addr;
    if (inet_pton(AF_INET, ip, &addr))
    {
        return addr.s_addr;
    }
    throw std::exception("wrong ip");
}

int main(int32_t argc, char* argv[])
{
    using namespace std;
    cxxopts::Options options("openomd_tools", "Openomd Tools");
    options.add_options()
        ("f,function", "pcapdump,udpdump", cxxopts::value<string>())
        ("p,protocol", "omdc,omdd", cxxopts::value<string>())
        ("c,pcap", "Pcap file", cxxopts::value<string>())
        ("i,ip", "IP address", cxxopts::value<string>())
        ("t,port", "Ports", cxxopts::value<uint16_t>());
    try
    {
        auto result = options.parse(argc, argv);
        auto function = result["f"].as<string>();
        if (function == "pcapdump")
        {
            if (result.count("p") == 0 || result.count("c") == 0)
            {
                cerr << options.help();
                return -1;
            }
            auto protocol = result["p"].as<string>();
            auto pcapFile = result["c"].as<string>();
            cout << "function=" << function << " p=" << protocol << " pcap=" << pcapFile << " " << endl;
            if (protocol == "omdc")
            {

                openomd::OmdPcapRunner<openomd::OmdcPrintProcessor, openomd::OmdcParser> runner{
                    {{30, convertIp("239.1.1.24"), 51000, convertIp("239.1.127.24"), 51000},
                     {31, convertIp("239.1.1.24"), 51001, convertIp("239.1.127.24"), 51001},
                     {32, convertIp("239.1.1.24"), 51002, convertIp("239.1.127.24"), 51002},
                     {33, convertIp("239.1.1.24"), 51003, convertIp("239.1.127.24"), 51003},
                     {34, convertIp("239.1.1.24"), 51004, convertIp("239.1.127.24"), 51004},
                     {35, convertIp("239.1.1.24"), 51005, convertIp("239.1.127.24"), 51005},
                     {36, convertIp("239.1.1.24"), 51006, convertIp("239.1.127.24"), 51006},
                     {37, convertIp("239.1.1.24"), 51007, convertIp("239.1.127.24"), 51007},
                     {38, convertIp("239.1.1.24"), 51008, convertIp("239.1.127.24"), 51008}}, 
                     pcapFile };
                runner.run();
            }
            else if (protocol == "omdd")
            {
                openomd::OmdPcapRunner<openomd::OmddPrintProcessor, openomd::OmddParser> runner{ {} , pcapFile };
                runner.run();
            }

        }
        else if (function == "udpdump")
        {

        }
        else
        {
            cerr << options.help();
            return -1;
        }
    }
    catch (exception const& ex)
    {
        cerr << ex.what();
        cerr << options.help();
        return -1;
    }
}
