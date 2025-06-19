#ifndef _STRUCTURE_INFO_MANAGER_H
#define _STRUCTURE_INFO_MANAGER_H

#include "StructureInfo.h"

class StructureInfoManager {
public:
    static void register_info(StructureType type, const StructureInfo& info) {
        get_infos()[type] = info;
    }

    static StructureInfo& get_info(StructureType type) {
        return get_infos().at(type);
    }

private:
    inline static std::unordered_map<StructureType, StructureInfo> Structure_infos;
    static std::unordered_map<StructureType, StructureInfo>& get_infos() {
        return Structure_infos;
    }
};

template<typename T>
struct AutoRegisterStructureInfo {
    AutoRegisterStructureInfo(StructureType type, const StructureInfo& info) {
        StructureInfoManager::register_info(type, info);
    }
};

#endif