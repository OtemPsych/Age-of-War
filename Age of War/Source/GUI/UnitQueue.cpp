#include "UnitQueue.h"

namespace gui
{
	UnitQueue::UnitData::UnitData(Unit::UnitType type, sf::Time spawn_time)
		: type(type)
		, spawn_time(spawn_time)
	{
	}

	UnitQueue::UnitQueue(const sf::FloatRect& spawn_bar_bounds)
	{
		identifiers_.fill(nullptr);
		setupIdentifiers(spawn_bar_bounds);
	}

	void UnitQueue::push(const UnitData& unit_data)
	{
		queue_.push(std::move(unit_data));
		updateIdentifiers(Identifier::Fill);
	}

	UnitQueue::UnitData UnitQueue::getNextUnitData()
	{
		UnitData unit_data(queue_.front());
		queue_.pop();
		updateIdentifiers(Identifier::Empty);

		return std::move(unit_data);
	}

	sf::FloatRect UnitQueue::getLocalBounds() const
	{
		return sf::FloatRect(identifiers_.front()->getPosition(), identifiers_.back()->getPosition() 
			                                                     + identifiers_.back()->vertices_[2].position 
																 - identifiers_.front()->getPosition());
	}

	void UnitQueue::updateIdentifiers(Identifier id)
	{
		if (id == Identifier::Fill) {
			size_t id_pos = queue_.size() - 1;
			for (size_t i = 4; i < identifiers_[id_pos]->vertices_.getVertexCount(); ++i)
				identifiers_[id_pos]->vertices_[i].color = sf::Color(179, 89, 0);
		}
		else if (!queue_.empty()) {
			size_t id_pos = queue_.size();
			for (size_t i = 4; i < identifiers_[id_pos]->vertices_.getVertexCount(); ++i)
				identifiers_[id_pos]->vertices_[i].color = sf::Color(51, 51, 51);
		}
	}

	void UnitQueue::setupIdentifiers(const sf::FloatRect& spawn_bar_bounds)
	{
		const sf::Vector2f size(10.f, 10.f);
		const float thickness = 1.f;
		const float margin = 8.f;

		for (size_t i = 0; i < identifiers_.size(); ++i) {
			auto identifier(std::make_unique<pyro::VertexArrayNode>(size + sf::Vector2f(thickness, thickness)));
			identifier->vertices_.append(sf::Vertex(sf::Vector2f(thickness, thickness), sf::Color(51, 51, 51)));
			identifier->vertices_.append(sf::Vertex(sf::Vector2f(size.x,    thickness), sf::Color(51, 51, 51)));
			identifier->vertices_.append(sf::Vertex(sf::Vector2f(size.x,    size.y),    sf::Color(51, 51, 51)));
			identifier->vertices_.append(sf::Vertex(sf::Vector2f(thickness, size.y),    sf::Color(51, 51, 51)));

			identifier->setPosition(sf::Vector2f((size.x + margin) * i - margin * identifiers_.size() - thickness,
				                                 -size.y * 2 - thickness));
			identifiers_[i] = identifier.get();
			attachChild(std::move(identifier));
		}

		setOriginFlags(pyro::utils::OriginFlag::CenterX | pyro::utils::OriginFlag::Bottom);
		setPosition(spawn_bar_bounds.width / 2.f, -5.f);
	}

	void UnitQueue::updateCurrent(sf::Time dt)
	{
		activateDrawing(ActivationTarget::Both, !queue_.empty());
	}
}