//
// Created by gabriele on 22/12/20.
//

#include "jsonUtils.h"

void to_json(json& j, const std::map<std::string,std::string>& map) {
    for (auto& x: map) {
        json temporaryj;
        temporaryj += json::object_t::value_type("path", x.first);
        temporaryj += json::object_t::value_type("hash", x.second);
        j += temporaryj;
    }
};
void from_json(std::map<std::string,std::string>& map, const json& j) {
    for (const auto& item : j.items()) {
        map.insert(std::make_pair(item.value()["path"], item.value()["hash"]));
    }
};