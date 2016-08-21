#ifndef GameState_H_
#define GameState_H_

#include "../BasePlayer.h"
#include "../BaseAI.h"

#include <PYRO/State.h>
#include <PYRO/Audio/MusicPlayer.h>

class GameState : public pyro::State
{
public:
	enum SceneLayers { Background, Bases, Units, Projectiles, Gui, Cursor, SceneLayerCount };
protected:
	enum class MusicID { Soundtrack };

public:
	GameState(pyro::StateStack* stack, sf::RenderWindow* window);
	virtual ~GameState();
public:
	void unpauseMusic();

	virtual bool handleEvent(const sf::Event& event) override;
	virtual bool update(sf::Time dt) override;
	virtual void draw() override;
private:
	void buildScene();
	virtual void setupResources() override;

private:
	pyro::VertexArrayNode*                       cursor_;
	sf::Texture                                  background_texture_;
	sf::Texture                                  cursor_texture_;
											     
protected:									     
	pyro::SceneNode                              scene_graph_;
	std::vector<pyro::SceneNode*>                scene_layers_;
											     
	pyro::TextureHolder<Unit::UnitType>          unit_textures_;
	pyro::TextureHolder<Turret::TurretType>      turret_textures_;
	pyro::TextureHolder<Base::BaseType>          base_textures_;
											     
	std::vector<std::unique_ptr<data::BaseData>> base_data_;
	BasePlayer*                                  base_player_;
	BaseAI*                                      base_opponent_;
											     
	pyro::MusicPlayer<MusicID>		             music_player_;
	pyro::SoundPlayer<Unit::SoundID>             sound_player_;
											     
	sf::Font                                     damage_font_;
											     
	sf::IntRect                                  window_bounds_;
	sf::FloatRect                                world_bounds_;
};
#endif