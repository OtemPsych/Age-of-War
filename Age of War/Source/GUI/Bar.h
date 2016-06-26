#ifndef Bar_H_
#define Bar_H_

#include <SFML/Graphics/RectangleShape.hpp>

namespace gui
{
	class Bar : public sf::Transformable, public sf::Drawable
	{
	private:
		sf::RectangleShape mOutline;
	protected:
		sf::VertexArray    mVertices;

	protected:
		sf::FloatRect getGlobalBounds() const;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		Bar(sf::FloatRect entityBounds, bool horizontal, const sf::Color& color);
		virtual ~Bar();
	public:
		void update(unsigned total, unsigned current);
	};
}
#endif