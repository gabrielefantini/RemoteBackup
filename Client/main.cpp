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

        //ignora i casi in cui si riceve la modifica di una cartella (avviene quando un file al suo interno viene modificato, apparentemente)
        /*if(!fs::is_regular_file(fs::path(path_to_watch)) && status==FileStatus::modified){
            return;
        }*/

        switch (status) {
            case FileStatus::created:
                std::cout<<path_to_watch<<" created\n";
                add(path_to_watch, localMap);
                break;
            case FileStatus::modified:
                //per ora, non gestendo in contenuti, non succede nulla alla modifica di un file
                std::cout<<path_to_watch<<" modified\n";
                break;
            case FileStatus::erased:
                std::cout<<path_to_watch<<" erased\n";
                remove(path_to_watch, localMap);
                //std::cout<<"map updated:\n";
                //print_map(backup_);
                break;
            case FileStatus::do_update:
                modify(path_to_watch, localMap);
                break;
            default:
                std::cout<<"Error: unknown file status.\n";
        }
    });
    return 0;
    }