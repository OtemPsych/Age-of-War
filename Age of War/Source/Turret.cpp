#include "Turret.h"
#include "DataTables.h"

#include <PYRO/Math.h>

Turret::Turret(const sf::Font& damage_font, const sf::Vector2f& base_size, Side side,
	           data::TurretData* turret_data, data::ValueDisplayData* value_display_data,
			   pyro::SceneNode* gui_scene_layer, pyro::SceneNode* proj_scene_layer)
	: Entity(side, turret_data)
	, turret_data_(turret_data)
	, gui_scene_layer_(gui_scene_layer)
	, proj_scene_layer_(proj_scene_layer)
	, attack_rate_(sf::Time::Zero)
	, damage_display_manager_(nullptr)
{
	auto damage_display_manager(std::make_unique<ValueDisplayManager>(value_display_data));
	damage_display_manager_ = damage_display_manager.get();
	gui_scene_layer_->attachChild(std::move(damage_display_manager));
	//scale(data.scale, data.scale);
}

Turret::~Turret()
{
	for (auto& itr = projectiles_.begin(); itr != projectiles_.end(); itr++) {
		proj_scene_layer_->detachChild(*(*itr));
	}
	projectiles_.clear();
}

void Turret::attack(HealthEntity* enemy)
{
	if (enemy_targeted_) {
		if (attack_rate_ >= turret_data_->rate.value.current) {
			spawnProjectile();
			if (side_ == Side::Enemy)
				projectiles_.back()->move(-getGlobalBounds().width, 0.f);

			sf::Vector2f distanceVec(enemy->getPosition() - getPosition());
			setRotation(pyro::math::toDegrees(std::atan2f(distanceVec.y, distanceVec.x)));
			if (side_ == Side::Enemy)
				rotate(-180.f);

			attack_rate_ = sf::Time::Zero;
		}
	}
	else if (enemyInRange(enemy)) {
		enemy_targeted_ = enemy;
		attack_rate_ = sf::Time::Zero;
	}

	//if (elapsed_attack_rate_ >= turret_data_->rate.value.original)
	//{
	//	if (side_ == Side::Ally)
	//	{
	//		float turretStartPoint = getPosition().x + getGlobalBounds().width / 2.f;
	//		float unitStartPoint = enemy->getPosition().x - enemy->getGlobalBounds().width / 2.f;

	//		if (turretStartPoint + turret_data_->range.value.current >= unitStartPoint)
	//		{
	//			sf::Vector2f distanceVec(enemy->getPosition() - getPosition());
	//			setRotation(pyro::math::toDegrees(std::atan2(distanceVec.y, distanceVec.x)));

	//			spawnProjectile();
	//			elapsed_attack_rate_ = sf::Time::Zero;
	//		}
	//	}
	//	else {
	//		float turretStartPoint = getPosition().x - getGlobalBounds().width / 2.f;
	//		float unitStartPoint = enemy->getPosition().x + enemy->getGlobalBounds().width / 2.f;

	//		if (turretStartPoint - turret_data_->range.value.current <= unitStartPoint)
	//		{
	//			sf::Vector2f distanceVec(getPosition() - enemy->getPosition());
	//			setRotation(-pyro::math::toDegrees(std::atan2(distanceVec.y, distanceVec.x)));

	//			spawnProjectile();
	//			elapsed_attack_rate_ = sf::Time::Zero;
	//		}
	//	}
	//}

	//if (!projectiles_.empty() && enemy->getGlobalBounds().intersects(projectiles_.front()->getGlobalBounds())) {
	//	enemy->receiveDamage(turret_data_->damage.value.current);
	//	damage_display_manager_->addValueDisplay(getGlobalBounds(), enemy->getGlobalBounds(), turret_data_->damage.value.current);
	//	parent_->detachChild(*projectiles_.front());
	//	projectiles_.erase(projectiles_.begin());
	//}

	//enemy_position_ = enemy->getPosition();
}

void Turret::nullifyEnemyTargeted()
{
	enemy_targeted_ = nullptr;

	for (auto& itr = projectiles_.begin(); itr != projectiles_.end(); itr++) {
		proj_scene_layer_->detachChild(*(*itr));
	}
	projectiles_.clear();
}

void Turret::spawnProjectile()
{
	auto projectile(std::make_unique<pyro::VertexArrayNode>(sf::Vector2f(5.f, 3.f)));
	projectile->setFillColor(sf::Color::Red);
	projectile->setPosition(getPosition());

	projectiles_.push_back(projectile.get());
	proj_scene_layer_->attachChild(std::move(projectile));
}

bool Turret::enemyInRange(HealthEntity* enemy)
{
	if (enemy) {
		const sf::FloatRect gbounds(getGlobalBounds());
		const sf::FloatRect enemy_gbounds(enemy->getGlobalBounds());

		if (side_ == Side::Ally) {
			if (gbounds.left + gbounds.width + turret_data_->range.value.current >= enemy_gbounds.left)
				return true;
		}
		else {
			if (gbounds.left - turret_data_->range.value.current <= enemy_gbounds.left + enemy_gbounds.width)
				return true;
		}
	}

	return false;
}

void Turret::updateCurrent(sf::Time dt)
{
	if (enemy_targeted_) {
		for (auto& projectile : projectiles_) {
			const sf::Vector2f proj_pos(projectile->getPosition());
			const sf::Vector2f enemy_pos(enemy_targeted_->getPosition());
			const float distance = pyro::math::getHypotenuse(enemy_pos - proj_pos);

			sf::Vector2f velocity(turret_data_->projectile_speed * (enemy_pos.x - proj_pos.x) / distance,
				                  turret_data_->projectile_speed * (enemy_pos.y - proj_pos.y) / distance);

			projectile->move(velocity * dt.asSeconds());
		}

		for (size_t i = 0; i < projectiles_.size(); ++i) {
			if (enemy_targeted_->getGlobalBounds().intersects(projectiles_[i]->getGlobalBounds())) {
				enemy_targeted_->receiveDamage(turret_data_->damage.value.current);
				damage_display_manager_->addValueDisplay(getGlobalBounds(), enemy_targeted_->getGlobalBounds(),
					                                     turret_data_->damage.value.current);

				proj_scene_layer_->detachChild(*projectiles_[i]);
				projectiles_.erase(projectiles_.begin() + i);
			}
		}

		attack_rate_ += dt;
	}

	//elapsed_attack_rate_ += dt;

	//for (auto& projectile : projectiles_)
	//{
	//	sf::Vector2f pos(getPosition());
	//	float distance = pyro::math::getHypotenuse(enemy_position_ - pos);

	//	sf::Vector2f velocity(turret_data_->projectile_speed * (enemy_position_.x - pos.x) / distance * dt.asSeconds(),
	//		                  turret_data_->projectile_speed * (enemy_position_.y - pos.y) / distance * dt.asSeconds());
	//	if (side_ == Side::Enemy)
	//		velocity = sf::Vector2f(-velocity.x, velocity.y);

	//	projectile->move(velocity);

	//	//for (unsigned i = 0; i < projectile->vertices_.getVertexCount(); i++)
	//	//	projectile->vertices_[i].position += velocity;
	//}

	//if (!projectiles_.empty() && pyro::math::getHypotenuse(projectiles_.front()->vertices_[0].position) > turret_data_->range.value.current * 1.5f) {
	//	parent_->detachChild(*projectiles_.front());
	//	projectiles_.erase(projectiles_.begin());
	//}
}

//#include <SFML/Graphics/RenderTarget.hpp>
//
//Turret::Turret(Side side, sf::Font& damageDisplayFont, sf::Vector2f baseSize, data::TurretData& data,
//	           const pyro::TextureHolder<TurretType>& textures)
//	: Entity(side, EntityType::Turret, textures.get(static_cast<TurretType>(data.turretType)))
//	, mTurretType(static_cast<TurretType>(data.turretType))
//	, mDamage(data.damage.value.current)
//	, mRange(data.range.value.current)
//	, mAttackRate(data.rate.value.current)
//	, mProjectileSpeed(data.projectileSpeed)
//	, mDamageDisplays(sf::Vector2f(0.f, 30.f), sf::Vector2f(0.f, -1.8f), sf::seconds(0.75f), damageDisplayFont)
//{
//	scale(data.scale, data.scale);
//}
//
//void Turret::spawnProjectile()
//{
//	const sf::Vector2f size(5.f, 3.f);
//	const sf::FloatRect gBounds(getGlobalBounds());
//
//	sf::VertexArray vert(sf::Quads);
//	vert.append(sf::Vertex(sf::Vector2f(   0.f, -size.y), sf::Color::Red));
//	vert.append(sf::Vertex(sf::Vector2f(size.x, -size.y), sf::Color::Red));
//	vert.append(sf::Vertex(sf::Vector2f(size.x,  size.y), sf::Color::White));
//	vert.append(sf::Vertex(sf::Vector2f(   0.f,  size.y), sf::Color::Red));
//
//	mProjectiles.push_back(std::move(vert));
//}
//
//void Turret::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	sf::RenderStates projState = states.transform * getTransform();
//	for (const auto& projectile : mProjectiles)
//		target.draw(projectile, projState);
//
//	Entity::draw(target, states);
//}
//
//void Turret::drawDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	target.draw(mDamageDisplays, states);
//}
//
//void Turret::attack(Unit& unit)
//{
//	if (mAttackRate.current >= mAttackRate.original)
//	{
//		if (mSide == Side::Ally)
//		{
//			float turretStartPoint = getPosition().x + getGlobalBounds().width / 2.f;
//			float unitStartPoint = unit.getPosition().x - unit.getGlobalBounds().width / 2.f;
//
//			if (turretStartPoint + mRange >= unitStartPoint)
//			{
//				sf::Vector2f distanceVec(unit.getPosition() - getPosition());
//				mSprite.setRotation(pyro::math::toDegrees(std::atan2(distanceVec.y, distanceVec.x)));
//
//				spawnProjectile();
//				mAttackRate.current = sf::Time::Zero;
//			}
//		}
//		else {
//			float turretStartPoint = getPosition().x - getGlobalBounds().width / 2.f;
//			float unitStartPoint = unit.getPosition().x + unit.getGlobalBounds().width / 2.f;
//
//			if (turretStartPoint - mRange <= unitStartPoint)
//			{
//				sf::Vector2f distanceVec(getPosition() - unit.getPosition());
//				mSprite.setRotation(-pyro::math::toDegrees(std::atan2(distanceVec.y, distanceVec.x)));
//
//				spawnProjectile();
//				mAttackRate.current = sf::Time::Zero;
//			}
//		}
//	}
//
//	if (!mProjectiles.empty() && unit.getGlobalBounds().contains(getTransform().transformPoint(mProjectiles.front()[0].position)))
//	{
//		unit.receiveDamage(mDamage);
//		mDamageDisplays.addValueDisplay(getGlobalBounds(), unit.getGlobalBounds(), mDamage);
//		mProjectiles.erase(mProjectiles.begin());
//	}
//
//	mEnemyPosition = unit.getPosition();
//}
//
//void Turret::update(sf::Time dt)
//{
//	mAttackRate.current += dt;
//
//	for (auto& projectile : mProjectiles)
//	{
//		sf::Vector2f pos(getPosition());
//		float distance = pyro::math::getHypotenuse(mEnemyPosition - pos);
//
//		sf::Vector2f velocity(mProjectileSpeed * (mEnemyPosition.x - pos.x) / distance * dt.asSeconds(),
//			                  mProjectileSpeed * (mEnemyPosition.y - pos.y) / distance * dt.asSeconds());
//		if (mSide == Side::Enemy)
//			velocity = sf::Vector2f(-velocity.x, velocity.y);
//
//		for (unsigned i = 0; i < projectile.getVertexCount(); i++)
//			projectile[i].position += velocity;
//	}
//
//	if (!mProjectiles.empty() && pyro::math::getHypotenuse(mProjectiles.front()[0].position) > mRange * 1.5f)
//		mProjectiles.erase(mProjectiles.begin());
//
//	mDamageDisplays.update(dt);
//}