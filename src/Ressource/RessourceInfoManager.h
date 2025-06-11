#ifndef _RESSOURCE_INFO_MANAGER_H
#define _RESSOURCE_INFO_MANAGER_H

#include "RessourceInfo.h"

class RessourceInfoManager {
public:
    static void register_info(RessourceType type, const RessourceInfo& info) {
        get_infos()[type] = info;
    }

    static RessourceInfo& get_info(RessourceType type) {
        return get_infos().at(type);
    }

private:
    inline static std::unordered_map<RessourceType, RessourceInfo> Ressource_infos;
    static std::unordered_map<RessourceType, RessourceInfo>& get_infos() {
        return Ressource_infos;
    }
};

template<typename T>
struct AutoRegisterInfo {
    AutoRegisterInfo(RessourceType type, const RessourceInfo& info) {
        RessourceInfoManager::register_info(type, info);
    }
};

#endif