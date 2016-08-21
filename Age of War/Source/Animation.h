#ifndef Animation_H_
#define Animation_H_

#include <PYRO/SpriteNode.h>

namespace data { struct AnimationData; }
class Animation : public pyro::SceneNode
{
public:
	Animation(const data::AnimationData& animation_data, pyro::SpriteNode* node);
public:
	void restart();
	inline bool isAnimationOngoing() const { return animation_ongoing_; }
private:
	void updateNodeProperties();
	virtual void updateCurrent(sf::Time dt) override;

private:
	const data::AnimationData& animation_data_;
	pyro::SpriteNode*          node_;

	unsigned                   current_rect_;
	sf::Time                   elapsed_time_;
	bool                       animation_ongoing_;
};
#endif