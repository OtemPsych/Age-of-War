#ifndef TextureDataReader_H_
#define TextureDataReader_H_

#include "../DataTables.h"

#include <SFML/System/NonCopyable.hpp>

std::vector<data::AnimationFrame> readTextureData(const std::string& unit_name,
	                                              const std::string& anim_type);
#endif 