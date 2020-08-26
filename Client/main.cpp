//
// Created by gabriele on 23/08/20.
//

#include <map>
#include "hashManager/hashManager.h"
#include "fileWatcher/FileWatcher.h"

void print_map(std::map<std::string,std::string> &m){
    for (auto& x: m) {
        std::cout << x.first << " : " << x.second << std::endl;
    }
    std::cout<<"\n";
}

int main(){
    std::map<std::string,std::string> localMap;
    std::string localPath = "/home/gabriele/Desktop/ClientFolder";

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
                modify(path_to_watch, localMap);
                std::cout<<path_to_watch<<" modified\n";
                break;
            case FileStatus::erased:
                std::cout<<path_to_watch<<" erased\n";
                remove(path_to_watch, localMap);
                //std::cout<<"map updated:\n";
                break;
            case FileStatus::do_update:
                print_map(localMap);
                break;
            default:
                std::cout<<"Error: unknown file status.\n";
        }
    });
    return 0;
    }