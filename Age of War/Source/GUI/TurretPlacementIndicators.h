#ifndef TurretPlacementIndicators_H_
#define TurretPlacementIndicators_H_

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <memory>
#include <array>

namespace gui
{
	class TurretPlacementIndicators : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	{
	private:
		sf::VertexArray                  mIndicators;
		std::unique_ptr<sf::VertexArray> mHoverLayer;
		int                              mSelectedIndicator;
		bool                             mShouldCreateTurret;

		sf::RenderWindow&                mWindow;

	private:
		bool hover(unsigned indicator);
		void turretPlacementOccupied(unsigned indicator, bool flag);

		void setup(const std::array<sf::FloatRect, 3>& windowRects);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		TurretPlacementIndicators(const std::array<sf::FloatRect, 3>& windowRects,
			                      sf::RenderWindow& window);
	public:
		void handleEvent(const sf::Event& event);
		void update();

		inline bool shouldCreateTurret() const { return mShouldCreateTurret; }
		inline int getTurretIndicator() const { return mSelectedIndicator; }
	};
}
#endif