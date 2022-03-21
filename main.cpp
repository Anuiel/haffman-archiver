#include "Archiver/Encoder/Encoder.h"
#include "Archiver/Decoder/Decoder.h"
#include <iostream>
#include <string>
#include <iomanip>

class Timer {
public:
    Timer() : Start(std::chrono::steady_clock::now()) {
    }
    static Timer CreateTimer() {
        return Timer();
    }
    ~Timer() {
        auto finish = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - Start);
        std::cout << "Done in " << std::setprecision(2) << static_cast<double>(elapsed.count()) / 1000 << " s" << std::endl;
    }

private:
    std::chrono::steady_clock::time_point Start;
};

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cout << "Wrong command\nTry -h for further information\n";
        return 0;
    }
    std::string command = argv[1];
    if (command == "-h") {
        std::cout << "Very cringe archiver\n"
                     "\t -c archive_name file1 file2 ... - archives file1, file2 ... into archive_name\n"
                     "\t -d archive_name - unzips the files and puts them in the current directory\n";
        return 0;
    }
    if (command == "-c") {
        if (argc <= 2) {
            std::cout << "Missing files to archive\n";
            return 0;
        }
        std::string archive_name = argv[2];
        auto timer = Timer::CreateTimer();
        try {
            Haffman::Encoder encoder(archive_name);
            for (size_t i = 3; i + 1 < static_cast<size_t>(argc); ++i) {
                encoder.AddFile(argv[i], false);
            }
            encoder.AddFile(argv[argc - 1], true);
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    } else if (command == "-d") {
        if (argc == 1) {
            std::cout << "Missing files to unzip\n";
            return 0;
        }
        auto timer = Timer::CreateTimer();
        std::string archive_name = argv[2];
        try {
            Haffman::Decoder decoder;
            decoder.Decode(archive_name);
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    } else {
        std::cout << "Wrong command\nTry -h for further information\n";
    }
}
