//
// Created by neppa on 19/08/20.
//

#include "FileWatcher.h"

#include <boost/filesystem.hpp>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>

//questa è l'implementazione del filewatcher totalmente affine a quella del tutorial linkato (eccetto per la sostituzione della libreria std con quella di boost)

namespace fs=boost::filesystem;


Filewatcher::Filewatcher(std::string path_to_watch,std::chrono::duration<int, std::milli> delay) : path_to_watch(path_to_watch), delay(delay) {
    for (auto &file : fs::recursive_directory_iterator(path_to_watch)) {
        paths_[file.path().string()]=fs::last_write_time(file);
    }
}


void Filewatcher::start(const std::function<void (std::string,FileStatus)> &action){
    int do_update;
    while (Filewatcher::running_){
        std::cout<<"---\n"; //debug
        do_update=false;
        std::this_thread::sleep_for(delay); //wait delay



        auto it=paths_.begin();
        //check for erased
        while(it!=paths_.end()){
            if(!fs::exists(it->first)){
                action(it->first,FileStatus::erased);
                it=paths_.erase(it);
                do_update=true;
            }else
                it++;
        }

        //check for created/modified
        for (auto &file : fs::recursive_directory_iterator(path_to_watch)) {
            auto current_file_last_write_time=fs::last_write_time(file);

            //file creation
            if(!contains(file.path().string())){
                paths_[file.path().string()]=current_file_last_write_time;
                action(file.path().string(),FileStatus::created);
                do_update=true;
            }else{
                if(paths_[file.path().string()]!=current_file_last_write_time){
                    paths_[file.path().string()]=current_file_last_write_time;
                    action(file.path().string(),FileStatus::modified);
                    do_update=true;
                }
            }
        }

        if(do_update)
            action("",FileStatus::do_update);
    }
}

bool Filewatcher::contains(const std::string &key){
        auto element=paths_.find(key);
        return element != paths_.end();
}

