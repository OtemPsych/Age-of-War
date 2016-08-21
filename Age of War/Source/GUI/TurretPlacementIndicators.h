#ifndef TurretPlacementIndicators_H_
#define TurretPlacementIndicators_H_

#include <memory>
#include <array>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <PYRO/VertexArrayNode.h>

namespace gui
{
	class TurretPlacementIndicators : public pyro::VertexArrayNode, private sf::NonCopyable
	{
	public:
		TurretPlacementIndicators(const std::array<sf::FloatRect, 3>& window_rects,
			                      sf::RenderWindow* window);
	public:
		virtual void updateCurrent(sf::Time dt) override;

		int getTurretIndicator();
		inline bool shouldCreateTurret() const { return should_create_turret_; }
	private:
		bool hover(unsigned indicator);
		void occupyTurretPlacement(unsigned indicator, bool flag);
		void init(const std::array<sf::FloatRect, 3>& window_rects);

	private:
		sf::RenderWindow*      window_;

		pyro::VertexArrayNode* hover_layer_;
		int                    selected_indicator_;
		bool                   should_create_turret_;
	};

	//class TurretPlacementIndicators : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	//{
	//private:
	//	sf::VertexArray                  mIndicators;
	//	std::unique_ptr<sf::VertexArray> mHoverLayer;
	//	int                              mSelectedIndicator;
	//	bool                             mShouldCreateTurret;

	//	sf::RenderWindow&                mWindow;

	//private:
	//	bool hover(unsigned indicator);
	//	void turretPlacementOccupied(unsigned indicator, bool flag);

	//	void setup(const std::array<sf::FloatRect, 3>& windowRects);
	//	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	//public:
	//	TurretPlacementIndicators(const std::array<sf::FloatRect, 3>& windowRects,
	//		                      sf::RenderWindow& window);
	//public:
	//	bool handleEvent(const sf::Event& event);
	//	void update();

	//	inline int getTurretIndicator() const { return mSelectedIndicator; }
	//};
}
#endif