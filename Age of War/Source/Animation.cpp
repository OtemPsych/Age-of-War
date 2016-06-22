#include "Animation.h"

Animation::Animation(sf::Sprite& sprite, gStruct::TextureData& textureRects,
					 sf::Time duration, bool repeat)
	: mSprite(sprite)
	, mTextureRects(textureRects)
	, mCurrentRect(0)
	, mTimePerRect(duration / static_cast<float>(textureRects.size()))
	, mElapsedTime(sf::Time::Zero)
	, mRepeat(repeat)
	, mAnimationOngoing(true)
{
}

void Animation::update(sf::Time dt)
{
	if (mAnimationOngoing && (mElapsedTime += dt) >= mTimePerRect)
	{
		if (mCurrentRect < mTextureRects.size() - 1 || mRepeat)
		{
			mSprite.setTextureRect(mTextureRects[mCurrentRect].first);

			sf::FloatRect lBounds(mSprite.getLocalBounds());
			sf::Vector2f origin(mTextureRects[mCurrentRect].second);
			mSprite.setOrigin(origin.x * lBounds.width, origin.y * lBounds.height);

			mElapsedTime = sf::Time::Zero;

			if (mRepeat)
				if (mCurrentRect < mTextureRects.size() - 1)
					mCurrentRect++;
				else
					mCurrentRect = 0;
			else
				mCurrentRect++;
		}
		else
			mAnimationOngoing = false;
	}
}

void Animation::restart()
{
	mCurrentRect = 0;
	mAnimationOngoing = true;
}