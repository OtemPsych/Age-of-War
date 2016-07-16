#include <fstream>
#include <iostream>
#include <string>

#include "TextureDataReader.h"

data::UnitData::TextureData readTextureData(const std::string& unit_name, const std::string& anim_type)
{
	data::UnitData::TextureData data;

	std::ifstream fin("TextureData/" + unit_name + "Data.json", std::ios::in);
	if (fin.is_open()) 
	{
		unsigned short animTypeIndex = 1;

		while (!fin.eof()) 
		{
			fin.seekg(0);
			std::string str_needed = unit_name + "_" + anim_type + std::to_string(animTypeIndex++) + ".png";
			std::string str_read;

			// Find Filename
			do {
				std::getline(fin, str_read);
			} while (str_read.find(str_needed) == std::string::npos && !fin.eof());
			if (fin.eof())
				break;

			// Find Frame
			std::getline(fin, str_read);
			fin.seekg((size_t)fin.tellg() - (str_read.length() + 1));

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
				std::getline(fin, str_read);
			fin.seekg((size_t)fin.tellg() - (str_read.length() + 1));

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