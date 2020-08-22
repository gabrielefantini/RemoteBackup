//
// Created by gabriele on 20/08/20.
//
#include <openssl/sha.h>
#include <boost/filesystem.hpp>
#include <stdio.h>
#include <iostream>
#include <map>


#ifndef SHA1_HASHMANAGER_H
#define SHA1_HASHMANAGER_H
namespace filesys = boost::filesystem;

int add(const char* path, std::map<std::string,std::string> &localStructure);
int modify(std::string path, std::map<std::string,std::string> &localStructure);
int remove(std::string path, std::map<std::string,std::string> &localStructure);

#endif //SHA1_HASHMANAGER_H
