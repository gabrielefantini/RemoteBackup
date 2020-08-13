#include <iostream>
#include "FileWatcher.h"

namespace fs=std::experimental::filesystem;

int main() {
    Filewatcher fw("./../../fw_test",std::chrono::milliseconds(5000));

    fw.start([] (std::string path_to_watch, FileStatus status) -> void {
       //ignore irregular files
       if(!fs::is_regular_file(fs::path(path_to_watch)) && status!=FileStatus::erased){
           return;
       }

        switch (status) {
           case FileStatus::created:
               std::cout<<"File created: "<<path_to_watch<<std::endl;
                break;
            case FileStatus::modified:
                std::cout<<"File modified: "<<path_to_watch<<std::endl;
                break;
            case FileStatus::erased:
                std::cout<<"File erased: "<<path_to_watch<<std::endl;
                break;
            default:
                std::cout<<"Error: unknown file status.\n";
        }
    });
    return 0;
}
