#ifndef TextureDataReader_H_
#define TextureDataReader_H_

#include "../GlobalStructs.h"

#include <SFML/System/NonCopyable.hpp>

gStruct::TextureData readTextureData(const std::string& unitName, const std::string& animType);
#endif