#include <iostream>
#include "FileWatcher.h"
#include <boost/filesystem.hpp>

namespace fs=boost::filesystem;
//to_backup_path: compone la stringa del nuovo path (es. da cartella_originale/file1 a cartella_backup/file1)
std::string to_backup_path(std::string path,std::string path_from,std::string path_to){
    std::size_t pos=path.find(path_from);
    std::string substr=path.substr(pos+path_from.length());
    std::string new_path=path_to+substr;
    return new_path;
}
//create_backup_initial: fa una scansione iniziale della cartella da monitorare e popola la map dei path (backup_)
std::unordered_map<std::string,std::string> create_backup_initial(std::string path_from,std::string path_to){
    std::unordered_map<std::string,std::string> map;
    for (auto &file : fs::recursive_directory_iterator(path_from)) {
            std::string path=file.path().string();
            std::string new_path=to_backup_path(path,path_from,path_to);
            /*std::size_t pos=path.find(path_from);
            std::string substr=path.substr(pos+path_from.length());
            std::string new_path=path_to+substr;*/
            //std::cout<<new_path<<"\n";
            map.insert(std::pair<std::string,std::string>(path,new_path));
    }
    std::cout<<"backup map initialized!\n\n";
    return map;
}
//print_map: per stampare la map dei path (per debug)
void print_map(std::unordered_map<std::string,std::string> m){
    for (auto& x: m)
        std::cout<<x.first<<" => "<<x.second<<std::endl;
    std::cout<<"\n\n";
}

int main() {
    const std::string path="./../../dir_to_watch";
    const std::string path_backup="./../../backup_dir";
    std::unordered_map<std::string,std::string> backup_;

    backup_=create_backup_initial(path,path_backup);
    print_map(backup_);

    Filewatcher fw(path,std::chrono::milliseconds(5000));

    //var passate dal main: backup_(reference),path,path_backup
    //var ricevute dal chiamante: path_to_watch(= effettivo path del file che è variato),status(created/modified/erased)
    fw.start([&backup_,path,path_backup] (std::string path_to_watch,FileStatus status) -> void {
        //ignora i casi in cui si riceve la modifica di una cartella (avviene quando un file al suo interno viene modificato, apparentemente)
        if(!fs::is_regular_file(fs::path(path_to_watch)) && status==FileStatus::modified){
            return;
        }


        switch (status) {
            case FileStatus::created:
                std::cout<<path_to_watch<<" created\n";
                backup_.insert(std::pair<std::string,std::string>(path_to_watch,to_backup_path(path_to_watch,path,path_backup)));
                std::cout<<"map updated:\n";
                print_map(backup_);
                break;
            case FileStatus::modified:
                //per ora, non gestendo in contenuti, non succede nulla alla modifica di un file
                std::cout<<path_to_watch<<" modified\n";
                break;
            case FileStatus::erased:
                std::cout<<path_to_watch<<" erased\n";
                backup_.erase(path_to_watch);
                std::cout<<"map updated:\n";
                print_map(backup_);
                break;
            default:
                std::cout<<"Error: unknown file status.\n";
        }
    });
    return 0;
}
