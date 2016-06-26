#ifndef ValueDisplays_H_
#define ValueDisplays_H_

#include <PYRO/Text.h>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

class ValueDisplays : public sf::Drawable
{
private:
	struct ValueDisplay {
		pyro::Text                      text;
		sf::Vector2f                    velocity;
		sf::Vector2f                    acceleration;
		pyro::utils::Resource<sf::Time> lifetime;

		ValueDisplay(ValueDisplays* owner, sf::FloatRect gBounds,
			         sf::FloatRect enemyGBounds, unsigned value);
		void operator=(const ValueDisplay& copy);
	};
private:
	const sf::Vector2f        mValueVelocity;
	const sf::Vector2f        mValueAcceleration;
	const sf::Time            mValueLifetime;

	std::vector<ValueDisplay> mValueDisplays;
	sf::Font&                 mFont;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	ValueDisplays(const sf::Vector2f& velocity, const sf::Vector2f& acceleration,
		          const sf::Time& lifetime, sf::Font& font);
public:
	void addValueDisplay(sf::FloatRect gBounds, sf::FloatRect enemyGBounds, unsigned value);
	void update(sf::Time dt);
};
#endif