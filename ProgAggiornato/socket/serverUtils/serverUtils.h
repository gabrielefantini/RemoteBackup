//
// Created by lollo on 22/12/20.
//

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

#ifndef SOCKET_SERVERUTILS_H
#define SOCKET_SERVERUTILS_H

#define BACKUP "backup/"
#define MAXLEN 50
#define USERS_FILE "../serverUtils/folders.txt"

namespace fs=boost::filesystem;

int setup_users(std::map<std::pair<std::string,std::string>,int> &m);
void print_user_map(std::map<std::pair<std::string,std::string>,int> &m);
std::string get_backup_dir(std::map<std::pair<std::string,std::string>,int> &user_map,std::string &usr,std::string &path);

#endif //SOCKET_SERVERUTILS_H
