#ifndef ValueDisplayAccorder_H_
#define ValueDisplayAccorder_H_

#include <PYRO/Text.h>

class ValueDisplayAccorder : private sf::NonCopyable
{
private:
	class ValueDisplay : public pyro::SceneNode 
	{
	public:
		ValueDisplay(const sf::Vector2f& starting_velocity, const sf::Vector2f& acceleration,
			         const sf::Font& font, sf::Time original_lifetime, unsigned value,
					 pyro::SceneNode* owner_node);
	public:
		virtual bool isDestroyed() const override;
	private:
		virtual void updateCurrent(sf::Time dt) override;

	private:
		pyro::Text*                     text_;
		sf::Vector2f                    velocity_;
		const sf::Vector2f&             acceleration_;
		pyro::utils::Resource<sf::Time> lifetime_;
	};

public:
	ValueDisplayAccorder(const sf::Vector2f& starting_velocity, const sf::Vector2f& acceleration,
		                 const sf::Font& font, sf::Time lifetime);
public:
	void addValueDisplay(unsigned short value, pyro::SceneNode* owner_node);

private:
	const sf::Vector2f value_velocity_;
	const sf::Vector2f value_acceleration_;
	const sf::Time     value_lifetime_;

	const sf::Font&    font_;
};
#endif