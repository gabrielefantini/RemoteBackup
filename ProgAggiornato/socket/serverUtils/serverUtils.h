//
// Created by lollo on 22/12/20.
//

#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <map>
#include <set>
#include "../hashManager/hashManager.h"

#ifndef SOCKET_SERVERUTILS_H
#define SOCKET_SERVERUTILS_H

#define BACKUP "../backup/"
#define MAXLEN 100
#define USERS_FILE "../serverUtils/folders.txt"

namespace fs=boost::filesystem;

int setupUsers(std::map<std::pair<std::string,std::string>,int> &m);
void printUserMap(std::map<std::pair<std::string,std::string>,int> &m);
std::pair<std::string,int> getBackupDir(std::map<std::pair<std::string,std::string>,int> &user_map,std::string &usr,std::string &path);
std::string getTmpDir(std::string &bpath);
std::set<std::string> checkForFile(std::map<std::string,std::string> &client_map,std::map<std::string,std::string> &server_map);
void updateBackupFolder(std::map<std::string,std::string> &client_map,std::map<std::string,std::string> &server_map,std::string &tmp,std::string &backup,std::string client_dir);
std::string getMapPath(std::string &backupPath);
int setupLocalMap(std::string &mapPath,std::map<std::string,std::string> &m);
int saveLocalMap(std::string &mapPath,std::map<std::string,std::string> &m);
void flushTmp(std::string &tmp);
#endif //SOCKET_SERVERUTILS_H
