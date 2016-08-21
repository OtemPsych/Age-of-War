#include "Animation.h"
#include "DataTables.h"

Animation::Animation(const data::AnimationData& animation_data, pyro::SpriteNode* node)
	: animation_data_(animation_data)
	, node_(node)
	, current_rect_(0)
	, elapsed_time_(sf::Time::Zero)
	, animation_ongoing_(true)
{
	updateNodeProperties();
}

void Animation::restart()
{
	current_rect_ = 0;
	animation_ongoing_ = true;
}

void Animation::updateNodeProperties()
{
	node_->setTextureRect(animation_data_.frames[current_rect_].texture_rect);

	const sf::FloatRect node_lbounds(node_->getLocalBounds());
	const sf::Vector2f& node_origin = animation_data_.frames[current_rect_].origin;
	node_->setOrigin(node_origin.x * node_lbounds.width, node_origin.y * node_lbounds.height);
}

void Animation::updateCurrent(sf::Time dt)
{
	bool reached_end = !(current_rect_ < animation_data_.frames.size() - 1 || animation_data_.repeat);

	if (animation_ongoing_ && !reached_end 
		&& (elapsed_time_ += dt) >= animation_data_.frames[current_rect_].frame_duration) 
	{
		if (!reached_end || animation_data_.repeat) {
			updateNodeProperties();
			elapsed_time_ = sf::Time::Zero;

			if (animation_data_.repeat) {
				if (current_rect_ < animation_data_.frames.size() - 1)
					current_rect_++;
				else
					current_rect_ = 0;
			}
			else {
				current_rect_++;
			}
		}
		else {
			animation_ongoing_ = false;
		}
	}
}