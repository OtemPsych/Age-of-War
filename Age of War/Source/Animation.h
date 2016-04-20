#ifndef Animation_H_
#define Animation_H_

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <vector>

class Animation
{
private:
	sf::Sprite&				  mSprite;
	std::vector<sf::IntRect>& mTextureRects;
	unsigned				  mCurrentRect;
	sf::Time				  mTimePerRect;
	sf::Time				  mElapsedTime;
	const bool				  mRepeat;
							  
	bool					  mAnimationOngoing;

public:
	Animation(sf::Sprite& sprite, std::vector<sf::IntRect>& textureRects,
			  sf::Time duration, bool repeat = false);
public:
	void update(sf::Time dt);
	void restart();

	inline bool isAnimationOngoing() const { return mAnimationOngoing; }
};
#endif