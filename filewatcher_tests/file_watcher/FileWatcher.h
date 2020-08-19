//
// Created by neppa on 28/07/20.
//

#pragma once

#include <experimental/filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

namespace fs=std::experimental::filesystem;

//file change statuses
enum class FileStatus {created, modified, erased};

class Filewatcher{
public:
    std::string path_to_watch;
    std::string path_backup;
    std::chrono::duration<int, std::milli> delay; //time int. between checks

    Filewatcher(std::string path_to_watch,std::string path_backup,std::chrono::duration<int, std::milli> delay) : path_to_watch(path_to_watch), path_backup(path_backup), delay(delay) {
        for (auto &file : fs::recursive_directory_iterator(path_to_watch)) {
            paths_[file.path().string()]=fs::last_write_time(file);
        }
    }

    void start(const std::function<void (std::string,std::string,std::string,FileStatus)> &action){
        while (running_){
            std::this_thread::sleep_for(delay); //wait delay

            auto it=paths_.begin();
            while(it!=paths_.end()){
                if(!fs::exists(it->first)){
                    action(it->first,path_to_watch,path_backup,FileStatus::erased);
                    it=paths_.erase(it);
                }else
                    it++;
            }

            //check for created/modified
            for (auto &file : fs::recursive_directory_iterator(path_to_watch)) {
                auto current_file_last_write_time=fs::last_write_time(file);

                //file creation
                if(!contains(file.path().string())){
                    paths_[file.path().string()]=current_file_last_write_time;
                    action(file.path().string(),path_to_watch,path_backup,FileStatus::created);
                }else{
                    if(paths_[file.path().string()]!=current_file_last_write_time){
                        paths_[file.path().string()]=current_file_last_write_time;
                        action(file.path().string(),path_to_watch,path_backup,FileStatus::modified);
                    }
                }
            }
        }
    }

private:
    std::unordered_map<std::string,fs::file_time_type> paths_;
    bool running_=true;

    bool contains(const std::string &key){
        auto element=paths_.find(key);
        return element != paths_.end();
    }
};
