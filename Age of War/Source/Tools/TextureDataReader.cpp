#include "TextureDataReader.h"

#include <fstream>
#include <iostream>
#include <string>

data::UnitData::TextureData readTextureData(const std::string& unit_name,
	                                        const std::string& anim_type)
{
	data::UnitData::TextureData data;

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
			sf::IntRect rect;
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

			rect.left = std::stoi(findNextValue());
			rect.top = std::stoi(findNextValue());
			rect.width = std::stoi(findNextValue());
			rect.height = std::stoi(findNextValue());

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

			data.push_back(std::make_pair(rect, origin));
		}

		fin.close();
	}
	else
		std::cerr << "Unable to open file: " << unit_name << "Data.json" << std::endl;

	return std::move(data);
}