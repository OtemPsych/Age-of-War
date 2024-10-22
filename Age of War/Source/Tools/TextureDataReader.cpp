#include "TextureDataReader.h"

#include <fstream>
#include <iostream>
#include <string>

std::vector<data::AnimationFrame> readTextureData(const std::string& unit_name,
	                                              const std::string& anim_type)
{
	std::vector<data::AnimationFrame> frame_data;

	std::ifstream fin("Assets/TextureData/" + unit_name + "Data.json", std::ios::in);
	if (fin.is_open()) 
	{
		unsigned short animTypeIndex = 1;

		while (!fin.eof()) 
		{
			fin.seekg(0);
			std::string strNeeded = unit_name + "_" + anim_type + std::to_string(animTypeIndex++) + ".png";
			std::string strRead;

			// Find Filename
			do {
				std::getline(fin, strRead);
			} while (strRead.find(strNeeded) == std::string::npos && !fin.eof());
			if (fin.eof())
				break;

			// Find Frame
			std::getline(fin, strRead);
			fin.seekg((size_t)fin.tellg() - (strRead.length() + 1));

				// Find Rect Start
			sf::FloatRect rect;
			char ch;
			do {
				fin.get(ch);
			} while (ch != '{');

				// Find Values
			std::function<std::string()> findNextValue([&fin]() -> std::string 
			{
				std::string value = "";
				char ch;

				do {
					fin.get(ch);
				} while (ch != ':');

				while (true) {
					fin.get(ch);
					if (ch != ',' && ch != '}')
						value += ch;
					else
						break;
				}

				return value;
			});

			rect.left = std::stof(findNextValue());
			rect.top = std::stof(findNextValue());
			rect.width = std::stof(findNextValue());
			rect.height = std::stof(findNextValue());

			// Find Pivot
			for (unsigned i = 0; i < 6; i++)
				std::getline(fin, strRead);
			fin.seekg((size_t)fin.tellg() - (strRead.length() + 1));

				// Find Origin Start
			sf::Vector2f origin;
			do {
				fin.get(ch);
			} while (ch != '{');

				// Find Values
			origin.x = std::stof(findNextValue());
			origin.y = std::stof(findNextValue());

			frame_data.emplace_back();
			frame_data.back().texture_rect = rect;
			frame_data.back().origin = origin;
		}

		fin.close();
	}
	else
		std::cerr << "Unable to open file: " << unit_name << "Data.json" << std::endl;

	return std::move(frame_data);
}