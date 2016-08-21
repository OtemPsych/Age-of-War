#ifndef ValueDisplayManager_H_
#define ValueDisplayManager_H_

#include <PYRO/Text.h>

namespace data { struct ValueDisplayData; }
class ValueDisplayManager : public pyro::SceneNode
{
private:
	struct ValueDisplay : public pyro::SceneNode {
		pyro::Text*  text;
		sf::Vector2f velocity;
		sf::Time     lifetime;

		ValueDisplay(const sf::FloatRect& gbounds,
			         const sf::FloatRect& enemy_gbounds, unsigned value,
			         data::ValueDisplayData* value_display_data);
	};

public:
	explicit ValueDisplayManager(data::ValueDisplayData* value_display_data);
public:
	void addValueDisplay(const sf::FloatRect& lbounds,
		                 const sf::FloatRect& enemy_lbounds, unsigned value);
private:
	virtual void updateCurrent(sf::Time dt) override;

private:
	data::ValueDisplayData*    value_display_data_;
	std::vector<ValueDisplay*> value_displays_;
};
#endif