//
// Created by neppa on 19/08/20.
//

#include <chrono>
#include <unordered_map>
#include <string>
#include <functional>


//file change statuses
enum class FileStatus {created, modified, erased, do_update, default_status};

class Filewatcher{
public:
    std::string path_to_watch;
    std::chrono::duration<int, std::milli> delay; //time int. between checks

    Filewatcher(std::string path_to_watch,std::chrono::duration<int, std::milli> delay);

    void start(const std::function<void (std::string,FileStatus)> &action);

private:
    std::unordered_map<std::string,std::time_t> paths_;
    bool running_=true;
    bool contains(const std::string &key);
};

