//
// Created by gabriele on 22/12/20.
//
#include <nlohmann/json.hpp>
#include <map>

#ifndef CLIENT_JSON_H
#define CLIENT_JSON_H
using json = nlohmann::json;
// copia la map in un oggetto json
void to_json(json& j, const std::map<std::string,std::string>& map);
// copia l'oggetto json in una map
void from_json(std::map<std::string,std::string>& map, const json& j);
#endif //CLIENT_JSON_H
