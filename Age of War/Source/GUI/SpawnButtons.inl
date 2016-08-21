#include "../Unit.h"

#include <PYRO/Utils.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace gui
{
	template <typename T, typename K>
	SpawnButtons<T, K>::SpawnButtons(const TData& Tdata, const pyro::TextureHolder<K>& textures,
		                             const sf::Vector2f& button_size, sf::RenderWindow* window)
		: VertexArrayNode(sf::Vector2f(0.f, 0.f))
		, Tdata_(Tdata)
		, window_(window)
		, stat_tooltip_(nullptr)
		, button_colors_(std::make_pair(sf::Color(0, 128, 255), sf::Color(140, 140, 140)))
		, Ttype_(-1)
	{
		init(textures, button_size);
	}

	template <typename T, typename K>
	int SpawnButtons<T, K>::getTypeClicked()
	{
		int type_indicator = Ttype_;
		Ttype_ = -1;

		return type_indicator;
	}

	template <typename T, typename K>
	std::unique_ptr<pyro::VertexArrayNode> SpawnButtons<T, K>::getButtonBox(unsigned indicator)
	{
		auto box(std::make_unique<pyro::VertexArrayNode>(buttons_[indicator].first->getSize()));
		box->setTexture(*buttons_[indicator].first->getTexture());
		//box->setFillColor(buttons_[indicator].first->getFil);
		box->setPosition(buttons_[indicator].first->getWorldPosition());

		return std::move(box);
	}

	template <typename T, typename K>
	void SpawnButtons<T, K>::updateButtonOverlay(unsigned gold)
	{
		for (size_t i = 0; i < buttons_.size(); ++i) {
			const sf::Color& color = Tdata_[i]->cost > gold ? button_colors_.second : button_colors_.first;
			for (size_t j = 4; j < buttons_[i].second->vertices_.getVertexCount(); ++j) {
				buttons_[i].second->vertices_[j].color = color;
			}
		}

		//for (size_t i = 0; i < buttons_.size(); ++i)
		//	buttons_[i].second->setFillColor(Tdata_[i]->cost > gold ? button_colors_.second : button_colors_.first);
	}

	template <typename T, typename K>
	void SpawnButtons<T, K>::updateCurrent(sf::Time dt)
	{
		if (getGlobalBounds().contains(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_))))
			for (size_t i = 0; i < buttons_.size(); ++i)
				if (buttons_[i].first->hover()) {
					stat_tooltip_->changeStatsDisplayed(Tdata_[i].get());
					stat_tooltip_->activateDrawing(ActivationTarget::Both, true);
					return;
				}

		stat_tooltip_->activateDrawing(ActivationTarget::Both, false);
	}

	template <typename T, typename K>
	void SpawnButtons<T, K>::handleEventCurrent(const sf::Event& event)
	{
		for (size_t i = 0; i < buttons_.size(); ++i)
			if (buttons_[i].first->clicked(event))
				Ttype_ = i;
	}

	template <typename T, typename K>
	void SpawnButtons<T, K>::init(const pyro::TextureHolder<K>& textures, const sf::Vector2f& button_size)
	{
		const float thickness = 3.f;
		const float margin = thickness + 10.f;
		const float total_button_size_x = button_size.x + margin;

		overlay_texture_.loadFromFile("Assets/Textures/UnitBackground.png");

		// Init Buttons
		for (size_t i = 0; i < Tdata_.size(); ++i) {
			// Init Overlay
			auto overlay(std::make_unique<pyro::VertexArrayNode>(button_size + sf::Vector2f(thickness, thickness)));
			overlay->setFillColor(sf::Color::Black);

			overlay->vertices_.append(sf::Vertex(sf::Vector2f(thickness,     thickness)));
			overlay->vertices_.append(sf::Vertex(sf::Vector2f(button_size.x, thickness)));
			overlay->vertices_.append(sf::Vertex(sf::Vector2f(button_size.x, button_size.y)));
			overlay->vertices_.append(sf::Vertex(sf::Vector2f(thickness,     button_size.y)));

			overlay->setPosition(total_button_size_x * (i + 0.5f), margin);

			// Init Button
			auto button(std::make_unique<pyro::gui::Button>(button_size - sf::Vector2f(thickness, thickness), window_));
			button->setTexture(&textures.get(static_cast<K>(i)));
			if (Tdata_[i]->icon_rect.width != 0)
				button->setTextureRect(Tdata_[i]->icon_rect);
			button->setPosition(overlay->getPosition() + sf::Vector2f(thickness, thickness));

			// Attach Elements
			buttons_.emplace_back(std::make_pair(button.get(), overlay.get()));
			attachChild(std::move(overlay));
			attachChild(std::move(button));
		}

		// Init General Overlay
		setTexture(overlay_texture_);
		vertices_[0].position = buttons_.front().second->getPosition() + sf::Vector2f(-margin, -margin);
		vertices_[1].position = buttons_.back().second->getPosition() + sf::Vector2f(button_size.x + margin, -margin);
		vertices_[2].position = buttons_.back().second->getPosition() + button_size + sf::Vector2f(margin, margin);
		vertices_[3].position = buttons_.front().second->getPosition() + sf::Vector2f(-margin, button_size.y + margin);

		// Init Stat Tooltip
		auto stat_tooltip(std::make_unique<StatTooltip>(Tdata_[0]->stat_types));
		stat_tooltip->setPosition(vertices_[3].position.x + (vertices_[2].position.x - vertices_[3].position.x) / 2.f,
			                      vertices_[3].position.y + 10.f);
		stat_tooltip_ = stat_tooltip.get();
		attachChild(std::move(stat_tooltip));

		setOriginFlags(pyro::utils::OriginFlag::CenterX | pyro::utils::OriginFlag::Top);
	}

	template <typename T, typename K>
	std::pair<std::vector<sf::Vector2f>, sf::Vector2f> SpawnButtons<T, K>::getButtonProperties() const
	{
		std::pair<std::vector<sf::Vector2f>, sf::Vector2f> properties;
		for (const auto& button : buttons_)
			properties.first.emplace_back(button.first->getWorldPosition());
		properties.second = buttons_.front().first->getSize();

		return std::move(properties);
	}

	//template <typename T, typename K>
	//SpawnButtons<T, K>::SpawnButtons(const TData& data, sf::RenderWindow& window,
	//	                             const pyro::TextureHolder<K>& textures, sf::Vector2f buttonSize)
	//	: mButtonOutlines(sf::Quads)
	//	, mButtonColors(std::make_pair(sf::Color(0, 128, 255), sf::Color(140, 140, 140)))
	//	, mButtonOverlay(sf::Quads, 4)
	//	, mStatTooltip(nullptr)
	//	, mTData(data)
	//	, mWindow(window)
	//{
	//	setup(textures, buttonSize);
	//}

	//template <typename T, typename K>
	//void SpawnButtons<T, K>::setup(const pyro::TextureHolder<K>& textures, sf::Vector2f buttonSize)
	//{
	//	sf::Vector2f buttonPos;
	//	if (std::is_same<K, Unit::UnitType>::value)
	//		buttonPos = sf::Vector2f(mWindow.getSize().x / 2.f - mTData.size() / 2.f * totalButtonSizeX,
	//		                         margin);
	//	else
	//		buttonPos = sf::Vector2f(mWindow.getSize().x / 4.f - mTData.size() / 2.f * totalButtonSizeX,
	//		                         margin);
	//
	//	for (unsigned i = 0; i < mTData.size(); i++)
	//	{
	//		mButtons.emplace_back(std::make_pair(pyro::gui::Button(buttonSize, &mWindow), sf::VertexArray(sf::Quads, 4)));
	//
	//		mButtons[i].first.setPosition(buttonPos.x + totalButtonSizeX * (i + 0.5f), buttonPos.y);
	//
	//		mButtonOutlines.append(sf::Vertex(mButtons.back().first.getPosition() + sf::Vector2f(-thickness,               -thickness),               sf::Color::Black));
	//		mButtonOutlines.append(sf::Vertex(mButtons.back().first.getPosition() + sf::Vector2f(buttonSize.x + thickness, -thickness),               sf::Color::Black));
	//		mButtonOutlines.append(sf::Vertex(mButtons.back().first.getPosition() + sf::Vector2f(buttonSize.x + thickness, buttonSize.y + thickness), sf::Color::Black));
	//		mButtonOutlines.append(sf::Vertex(mButtons.back().first.getPosition() + sf::Vector2f(-thickness,               buttonSize.y + thickness), sf::Color::Black));
	//		
	//		mButtons[i].first.setTexture(&textures.get(static_cast<K>(i)));
	//		if (mTData[i]->icon_rect.width == 0) {
	//			const sf::Vector2f textureSize(static_cast<sf::Vector2f>(mButtons[i].first.getTexture()->getSize()));
	//			mButtons[i].first.setTextureRect(sf::FloatRect(0.f, 0.f, textureSize.x, textureSize.y));
	//		}
	//		else {
	//			mButtons[i].first.setTextureRect(static_cast<sf::FloatRect>(mTData[i]->icon_rect));
	//		}
	//
	//		const sf::Vector2f& buttonPos = mButtons[i].first.getPosition();
	//		sf::VertexArray& overlay = mButtons[i].second;
	//		overlay[0].position = buttonPos;
	//		overlay[1].position = sf::Vector2f(buttonPos.x + buttonSize.x, buttonPos.y);
	//		overlay[2].position = buttonPos + buttonSize;
	//		overlay[3].position = sf::Vector2f(buttonPos.x, buttonPos.y + buttonSize.y);
	//	}
	//}

	//template <typename T, typename K>
	//void SpawnButtons<T, K>::draw(sf::RenderTarget& target, sf::RenderStates states) const
	//{
	//	states.transform *= getTransform();
	//	sf::RenderStates overlayStates(states);
	//	overlayStates.texture = &mButtonOverlayTexture;
	//	target.draw(mButtonOverlay, overlayStates);
	//
	//	target.draw(mButtonOutlines, states);
	//	for (const auto& button : mButtons) {
	//		target.draw(button.second, states);
	//		target.draw(button.first, states);
	//	}
	//
	//	target.draw(*mStatTooltip, states);
	//}

	//template <typename T, typename K>
	//int SpawnButtons<T, K>::handleEvent(const sf::Event& event)
	//{
	//	for (unsigned i = 0; i < mButtons.size(); i++)
	//		if (mButtons[i].first.clicked(event))
	//			return i;
	//
	//	return -1;
	//}

	//template <typename T, typename K>
	//void SpawnButtons<T, K>::update()
	//{
	//	sf::Vector2f mousePos(static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow)));
	//
	//	if (mousePos.x > mButtonOverlay[0].position.x && mousePos.y > mButtonOverlay[0].position.y
	//	 && mousePos.x < mButtonOverlay[2].position.x && mousePos.y < mButtonOverlay[2].position.y)
	//		for (unsigned i = 0; i < mButtons.size(); i++)
	//			if (mButtons[i].first.hover()) {
	//				mStatTooltip->changeStatsDisplayed(mTData[i].get());
	//				mStatTooltip->activateDrawing(pyro::SceneNode::ActivationTarget::Both, true);
	//				return;
	//			}
	//
	//	mStatTooltip->activateDrawing(pyro::SceneNode::ActivationTarget::Both, false);
	//}

	//template <typename T, typename K>
	//void SpawnButtons<T, K>::updateButtonOverlay(unsigned gold)
	//{
	//	for (unsigned i = 0; i < mButtons.size(); i++) {
	//		const sf::Color& color(mTData[i]->cost > gold ? mButtonColors.second : mButtonColors.first);
	//		for (unsigned j = 0; j < 4; j++)
	//			mButtons[i].second[j].color = color;
	//	}
	//}

	//template <typename T, typename K>
	//std::vector<sf::Vector2f> SpawnButtons<T, K>::getButtonPositions() const
	//{
	//	std::vector<sf::Vector2f> buttonPositions;
	//	for (const auto& button : mButtons)
	//		buttonPositions.emplace_back(button.first.getPosition());

	//	return std::move(buttonPositions);
	//}

	//template <typename T, typename K>
	//std::unique_ptr<pyro::VertexArrayNode> SpawnButtons<T, K>::getButtonBox(unsigned i)
	//{
	//	auto box(std::make_unique<pyro::VertexArrayNode>(mButtons[i].first.getSize()));
	//	box->setTexture(*mButtons[i].first.getTexture());
	//	box->setFillColor(mButtons[i].first.getVertices()[0].color);
	//	box->setPosition(mButtons[i].first.getVertices()[0].position);

	//	return std::move(box);
	//}
}