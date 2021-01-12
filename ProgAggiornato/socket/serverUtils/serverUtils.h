//
// Created by lollo on 22/12/20.
//

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <map>
#include <set>

#ifndef SOCKET_SERVERUTILS_H
#define SOCKET_SERVERUTILS_H

#define BACKUP "../backup/"
#define MAXLEN 50
#define USERS_FILE "../serverUtils/folders.txt"

namespace fs=boost::filesystem;

int setup_users(std::map<std::pair<std::string,std::string>,int> &m);
void print_user_map(std::map<std::pair<std::string,std::string>,int> &m);
std::pair<std::string,int> get_backup_dir(std::map<std::pair<std::string,std::string>,int> &user_map,std::string &usr,std::string &path);
std::string get_tmp_dir(std::string &bpath);
std::set<std::string> check_for_file(std::map<std::string,std::string> &client_map,std::map<std::string,std::string> &server_map);
#endif //SOCKET_SERVERUTILS_H
