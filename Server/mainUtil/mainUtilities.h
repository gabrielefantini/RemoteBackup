//
// Created by neppa on 24/08/20.
//

#ifndef SERVER_MAINUTILITIES_H
#define SERVER_MAINUTILITIES_H

#include <iostream>
#include <map>
#include <boost/filesystem.hpp>
#include "./../hashManager/hashManager.h"

#define USERS_FILE "./../mainUtil/users.txt"
#define MAXLEN 50
#define BACKUP_FOLDER "./../../server/backups"

namespace fs=boost::filesystem;

std::string get_server_dir(const std::string &path);
void cs_to_sc(std::string &path,const std::string &from,const std::string &to);

int setup_users(std::map<std::string,std::string> &m);

std::map<std::string,std::string> setup_map(std::string &folder_path);
void print_map(std::map<std::string,std::string> &m);

void compare_and_process(std::map<std::string,std::string> &client_map,std::map<std::string,std::string> &server_map,const std::string &cpath, const std::string &spath);


#endif //SERVER_MAINUTILITIES_H
