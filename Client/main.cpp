//
// Created by gabriele on 23/08/20.
//

#include <map>
#include "hashManager/hashManager.h"
#include "fileWatcher/FileWatcher.h"

int main(){
    std::map<std::string,std::string> localMap;
    std::string localPath = "path";

    create_backup_initial(localPath, localMap);

    Filewatcher fw(localPath,std::chrono::milliseconds(5000));

    fw.start([&localMap, localPath] (std::string path_to_watch,FileStatus status) -> void {

        switch (status) {
            case FileStatus::created:
                std::cout<<path_to_watch<<" created\n";
                add(path_to_watch, localMap);
                break;
            case FileStatus::modified:
                std::cout<<path_to_watch<<" modified\n";
                modify(path_to_watch, localMap);
                break;
            case FileStatus::erased:
                std::cout<<path_to_watch<<" erased\n";
                remove(path_to_watch, localMap);
                break;
            case FileStatus::do_update:
                //future funzioni di invio al server
                break;
            default:
                std::cout<<"Error: unknown file status.\n";
        }
    });
    return 0;
    }
