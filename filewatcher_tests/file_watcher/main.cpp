#include <iostream>
#include <boost/filesystem.hpp>
#include "FileWatcher.h"

namespace fs=std::experimental::filesystem;
namespace fs1=boost::filesystem;

void create_backup_initial(std::string path_from,std::string path_to){
    std::cout<<"path_from: "<<path_from<<"\n";
    std::cout<<"path_to: "<<path_to<<"\n\n";
    fs1::create_directory(path_to);

    for (auto &file : fs1::recursive_directory_iterator(path_from)) {
        if(fs1::is_regular_file(file.path())){
            std::string fpath=file.path().string();
            std::cout<<"file: "<<fpath<<"\n";
            std::size_t pos=fpath.find_first_not_of(path_from);
            std::string new_fp=path_to+fpath.substr(pos-1);
            fs1::path new_fpath=new_fp;
            fs1::copy_file(fpath,new_fpath);
            std::cout<<"new file: "<<new_fpath<<"\n\n";
        }
        else if(fs1::is_directory(file.path())){
            std::string dpath=file.path().string();
            std::cout<<"dir: "<<dpath<<"\n";
            std::size_t pos=dpath.find_first_not_of(path_from);
            std::string new_dp=path_to+dpath.substr(pos-1);
            fs1::path new_dpath=new_dp;
            fs1::create_directory(new_dpath);
            std::cout<<"new dir: "<<new_dpath<<"\n\n";
        }

        else
            std::cout<<"irregular file\n\n";
    }
    std::cout<<"backup dir initialized!\n\n";
}
void copy_file(std::string fpath,std::string path_from,std::string path_to){
    std::size_t pos=fpath.find_first_not_of(path_from);
    std::string new_fp=path_to+fpath.substr(pos-1);
    fs1::path new_fpath=new_fp;
    fs1::copy_file(fpath,new_fpath);
    std::cout<<"file created: "<<fpath<<"\nnew file: "<<new_fpath<<"\n\n";
}
void modify_file(std::string fpath,std::string path_from,std::string path_to){
    std::size_t pos=fpath.find_first_not_of(path_from);
    std::string new_fp=path_to+fpath.substr(pos-1);
    fs1::path new_fpath=new_fp;
    fs1::remove(new_fpath);
    fs1::copy_file(fpath,new_fpath);
    std::cout<<"file modified: "<<fpath<<"\nupdated file: "<<new_fpath<<"\n\n";
}
void remove_file(std::string fpath,std::string path_from,std::string path_to){
    std::size_t pos=fpath.find_first_not_of(path_from);
    std::string new_fp=path_to+fpath.substr(pos-1);
    fs1::path new_fpath=new_fp;
    fs1::remove(new_fpath);
    std::cout<<"file removed: "<<fpath<<"\nfile removed: "<<new_fpath<<"\n\n";
}

int main() {
    const std::string path="./../../fw_test";
    const std::string path_backup="./../../fw_test_backup";

    create_backup_initial(path,path_backup);

    Filewatcher fw(path,path_backup,std::chrono::milliseconds(5000));

    fw.start([] (std::string path_to_watch,std::string path_from,std::string path_to,FileStatus status) -> void {
       //ignore irregular files
       if(!fs::is_regular_file(fs::path(path_to_watch)) && status!=FileStatus::erased){
           return;
       }

        switch (status) {
           case FileStatus::created:
                copy_file(path_to_watch,path_from,path_to);
                break;
            case FileStatus::modified:
                modify_file(path_to_watch,path_from,path_to);
                break;
            case FileStatus::erased:
                remove_file(path_to_watch,path_from,path_to);
                break;
            default:
                std::cout<<"Error: unknown file status.\n";
        }
    });
    return 0;
}
