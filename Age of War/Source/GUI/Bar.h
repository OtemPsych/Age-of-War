#ifndef Bar_H_
#define Bar_H_

#include <SFML/Graphics/RectangleShape.hpp>

#include <PYRO/VertexArrayNode.h>
#include <PYRO/Utils.h>

namespace gui
{
	class Bar : public pyro::VertexArrayNode
	{
	public:
		Bar(const sf::FloatRect& entity_lbounds, const sf::Color& color, bool horizontal);
		virtual ~Bar();
	public:
		template <typename T>
		void updateBarValue(T current_value, T original_value);
		void correctBarProperties(const sf::FloatRect& entity_lbounds);

	private:
		const sf::Color                        color_;
		bool                                   horizontal_;
	};

	template <typename T>
	void Bar::updateBarValue(T current_value, T original_value)
	{
		vertices_[9].position.x =
		vertices_[10].position.x = current_value * (vertices_[5].position.x - vertices_[4].position.x) 
			                     / original_value;
	}
}
#endif