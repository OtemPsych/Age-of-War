#ifndef StatTooltip_H_
#define StatTooltip_H_

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <PYRO/Text.h>

#include "../DataTables.h"

namespace gui
{
	class StatTooltip : public pyro::VertexArrayNode, private sf::NonCopyable
	{
	private:
		using Stat = std::pair<pyro::Text*, pyro::Text*>;

	public:
		explicit StatTooltip(const std::vector<data::UnitTurretData::StatType>& stat_types);
	public:
		void changeStatsDisplayed(const data::UnitTurretData* data);
	private:
		void init(const std::vector<data::UnitTurretData::StatType>& stat_types);

	private:
		sf::Font          font_;
		pyro::Text*       title_;
		pyro::Text*       cost_;
		std::vector<Stat> stats_;
	};
}
#endif