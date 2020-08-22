#include <iostream>
#include <map>
#include "FileWatcher.h"
#include <boost/filesystem.hpp>

namespace fs=boost::filesystem;

//to_backup_path: compone la stringa del nuovo path (es. da cartella_originale/file1 a cartella_backup/file1)
/*std::string to_backup_path(std::string path,std::string path_from,std::string path_to){
    std::size_t pos=path.find(path_from);
    std::string substr=path.substr(pos+path_from.length());
    std::string new_path=path_to+substr;
    return new_path;
}*/

//create_backup_initial: fa una scansione iniziale della cartella da monitorare e popola la map dei path (backup_)
std::map<std::string,std::string> create_backup_initial(std::string path_from,std::string path_to){
    std::map<std::string,std::string> map;
    for (auto &file : fs::recursive_directory_iterator(path_from)) {
            std::string path=file.path().string();
            if(fs::is_directory(file))
                map.insert(std::pair<std::string,std::string>(path,"directory"));
            else{
            /*std::string new_path=to_backup_path(path,path_from,path_to);
            std::size_t pos=path.find(path_from);
            std::string substr=path.substr(pos+path_from.length());
            std::string new_path=path_to+substr;*/
            //std::cout<<new_path<<"\n";
            map.insert(std::pair<std::string,std::string>(path,"hash_<"+fs::path(path).filename().string()+">"));
            }
    }
    //std::cout<<"backup map initialized!\n\n"; //debug
    return map;
}
//print_map: per stampare la map dei path (per debug)
void print_map(std::map<std::string,std::string> m){
    for (auto& x: m)
        std::cout<<x.first<<" : "<<x.second<<std::endl;
    std::cout<<"\n\n";
}
void compare_and_process(std::map<std::string,std::string> m1,std::map<std::string,std::string> m2);

int main() {
    const std::string path="./../../dir_to_watch";
    const std::string path_backup="./../../backup_dir";
    std::map<std::string,std::string> client_map;
    std::map<std::string,std::string> server_map; //debug, per testing sarà uguale alla situazione iniziale del client

    client_map=create_backup_initial(path,path_backup);
    server_map=create_backup_initial(path,path_backup); //debug
    //print_map(client_map);

    Filewatcher fw(path,std::chrono::milliseconds(5000));

    //var passate dal main: backup_(reference),path,path_backup
    //var ricevute dal chiamante: path_to_watch(= effettivo path del file che è variato),status(created/modified/erased)
    fw.start([&client_map,&server_map,path,path_backup] (std::string path_to_watch,FileStatus status) -> void {

        //ignora i casi in cui si riceve la modifica di una cartella (avviene quando un file al suo interno viene modificato, apparentemente)
        /*if(!fs::is_regular_file(fs::path(path_to_watch)) && status==FileStatus::modified){
            return;
        }*/



        switch (status) {
            case FileStatus::created:
                std::cout<<path_to_watch<<" created\n";
                if(fs::is_regular_file(fs::path(path_to_watch)))
                    client_map.insert(std::pair<std::string,std::string>(path_to_watch,"hash_<"+fs::path(path_to_watch).filename().string()+">"));
                else
                    client_map.insert(std::pair<std::string,std::string>(path_to_watch,"directory"));
                //std::cout<<"map updated:\n";
                //print_map(backup_);
                break;
            case FileStatus::modified:
                //per ora, non gestendo in contenuti, non succede nulla alla modifica di un file
                std::cout<<path_to_watch<<" modified\n";
                break;
            case FileStatus::erased:
                std::cout<<path_to_watch<<" erased\n";
                client_map.erase(path_to_watch);
                //std::cout<<"map updated:\n";
                //print_map(backup_);
                break;
            case FileStatus::do_update:
                compare_and_process(client_map,server_map);
                break;
            default:
                std::cout<<"Error: unknown file status.\n";
        }
    });
    return 0;
}

//prototipo della funzione di confronto (solo per debugging al momento)
//m1: structDigest del client
//m2: structDigest del server (che in realtà verrà ricevuta dal socket)
void compare_and_process(std::map<std::string,std::string> m1,std::map<std::string,std::string> m2){
    std::cout<<"\nclient map:\n";
    print_map(m1);
    std::cout<<"server map:\n";
    print_map(m2);

    for(auto& x: m1){
        if(x.second=="directory") {
            //caso directory
            //std::cout << x.first << " is a dir\n";
            if(m2.find(x.first)==m2.end()){
                std::cout<<"dir "<<x.first<<" missing -> order to create_directory.\n";
            }

        }
        else {
            //caso file
            //std::cout << x.first << " is a file\n";

            //GESTIONE CASO SPOSTAMENTO: il file potrebbe essere in un altro path (che sta per essere eliminato),
            //altrimenti viene inviato
            if(m2.find(x.first)==m2.end()){
                std::string path_found="";
                for(auto& y: m2)
                    if(y.second==x.second && fs::path(y.first).filename().string()==fs::path(x.first).filename().string()) //controllo sia filename che digest (contenuto)
                        path_found+=y.first;
                if(path_found!="")
                    std::cout<<"file has to be moved: "<<path_found<<" -> "<<x.first<<"\n";
                else
                    std::cout<<"file "<<x.first<<" missing -> sending to server.\n";
            }

        }
    }
    for(auto& x: m2){
        if(x.second=="directory") {
            //caso directory
            //std::cout << x.first << " is a dir\n";
            if(m1.find(x.first)==m1.end()){
                std::cout<<"dir "<<x.first<<" has to be deleted.\n";
            }
        }
        else {
            //caso file
            //std::cout << x.first << " is a file\n";
            if(m1.find(x.first)==m1.end()){
                std::cout<<"file "<<x.first<<" has to be deleted.\n";
            }

        }
    }

}