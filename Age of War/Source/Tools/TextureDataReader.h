#ifndef TextureDataReader_H_
#define TextureDataReader_H_

#include "../UnitTurretData.h"

#include <SFML/System/NonCopyable.hpp>

data::UnitData::TextureData readTextureData(const std::string& unit_name, const std::string& anim_type);
#endif