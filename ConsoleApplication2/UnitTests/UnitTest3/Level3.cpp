#include "Level3.h"
#include "Util.h"
#include "SystemClock.h"

#define HOP_Y_SPEED -220 // px/s
#define HOP_Y_SPEED_SMALL -130

static inline const std::string GetMarioStateId(MARIO_STATE_T mario_state_t) {
	switch (mario_state_t) {
	case MARIO_STATE_T::MARIO_STANDING_RIGHT:	return "_sr";
	case MARIO_STATE_T::MARIO_STANDING_LEFT:	return "_sl";
	case MARIO_STATE_T::MARIO_RUNNING_RIGHT:	return "_rr";
	case MARIO_STATE_T::MARIO_RUNNING_LEFT:		return "_rl";
	case MARIO_STATE_T::MARIO_JUMPING_RIGHT:	return "_jr";
	case MARIO_STATE_T::MARIO_JUMPING_LEFT:		return "_jl";
	case MARIO_STATE_T::MARIO_BREAKING_RIGHT:	return "_br";
	case MARIO_STATE_T::MARIO_BREAKING_LEFT:	return "_bl";
	case MARIO_STATE_T::MARIO_DYING:			return "_dead";
	default: assert(0);
	}
}

void UnitTest3::GameOver() {

	auto* game_over_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("GAME_OVER");
	auto* game_over_animator = new TickAnimator();
	

	game_over_animator->SetOnFinish(
		[&](Animator* animator) {
			game_finished = true;
			AnimatorManager::GetSingleton().AddGarbage(animator);
		}
	);

	game_over_animator->Start(*game_over_animation, SystemClock::Get().milli_secs());
}

std::string UnitTest3::GetLastCheckpoint(uint x) {
	//std::string last_cp = "";
	auto cp_count = checkpoints.size();
	uint index;
	for (index = 0; index < cp_count; index++) {
		if (scenes[checkpoints[index]].mario.x > x)
			break;
	}
	if (index > 0)
		return checkpoints[index - 1];
	return checkpoints[0];
}


void UnitTest3::SetPiranhas() {
	auto piranhas = sprite_manager.GetTypeList("PIRANHA");
	for (auto* plant : piranhas) {
		float init_x, init_y;
		plant->SetHasDirectMotion(true);
		plant->GetPos(init_x, init_y);

		plant->main_animator->SetOnAction(
			[&, plant, init_y](Animator* animator, const Animation& anim) {
				plant->SetFrame(((FrameRangeAnimator*)(animator))->GetCurrFrame());
				float x, y;
				plant->GetPos(x, y);
				if (y < init_y - 22) {
					plant->SetVelocity({ 0, 10 });
				}
				else if (y >= init_y) {
					float mario_x, mario_y;
					mario->GetPos(mario_x, mario_y);
					if (mario_x > x + 20 || mario_x < x - 10)
						plant->SetVelocity({ 0, -10 });
					else
						plant->SetVelocity({ 0,0 });
				}
			}
		);
	}
}

void UnitTest3::SetScene(const std::string& scene_id) {
	auto scene = scenes[scene_id];
	view_win.x = scene.camera.x;
	view_win.y = scene.camera.y;
	auto mario_box = mario->GetBox();
	if((int)scene.mario.y - (int)mario_box.h < 0)
		mario->SetPos(scene.mario.x, 0);
	else
		mario->SetPos(scene.mario.x, scene.mario.y - mario_box.h);
	tile_view_win.x = view_win.x;
	tile_view_win.y = 0;
	tile_win_moved = true;

	/*FilterScrollDistance(view_win.x, view_win.w, dist_to_mid, map_dim.w);
	view_win.x += dist_to_mid;
	tile_win_moved = TileAllignedViewBoundCheck();*/
}

void UnitTest3::MarioEnterHPipe(Pipe* pipe) {
	auto* pipe_in_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("PIPE_IN");
	auto* pipe_animator = new TickAnimator();
	pipe_animator->SetOnStart(
		[&](Animator* animator, const Animation& anim) {
			mario->SetVelocity({ 20, 0 });
			mario->SetHasDirectMotion(true);
		}
	);

	pipe_animator->SetOnFinish(
		[&, pipe](Animator* animator) {
			//SetScene(pipe->GetScene());
			auto scene = scenes[pipe->GetScene()];
			view_win.x = scene.camera.x;
			view_win.y = scene.camera.y;
			auto mario_box = mario->GetBox();
			mario->SetPos(scene.mario.x, scene.mario.y - mario_box.h);
			tile_view_win.x = view_win.x - TILE_WIDTH;
			tile_view_win.y = 0;
			tile_win_moved = true;
			mario->SetHasDirectMotion(false);
			mario->SetUpdateBoundAreaPos(true);
			animator_manager.AddGarbage(animator);
		}
	);

	pipe_animator->Start(*pipe_in_animation, SystemClock::Get().milli_secs());
}

void UnitTest3::MarioEnterVPipe(Pipe* pipe) {
	auto* pipe_in_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("PIPE_IN");
	auto* pipe_animator = new TickAnimator();

	pipe_animator->SetOnStart(
		[&](Animator* animator, const Animation& anim) {
			mario->SetVelocity({ 0, 20 });
			mario->SetHasDirectMotion(true);
		}
	);

	pipe_animator->SetOnFinish(
		[&, pipe](Animator* animator) {
			//SetScene(pipe->GetScene());
			auto scene = scenes[pipe->GetScene()];
			view_win.x = scene.camera.x;
			view_win.y = scene.camera.y;
			mario->SetPos(scene.mario.x, scene.mario.y);
			tile_view_win.x = view_win.x - TILE_WIDTH;
			tile_view_win.y = 0;
			tile_win_moved = true;
			mario->SetHasDirectMotion(false);
			mario->SetUpdateBoundAreaPos(true);
			animator_manager.AddGarbage(animator);
		}
	);

	pipe_animator->Start(*pipe_in_animation, SystemClock::Get().milli_secs());
}

//returns true if rect1 sits on rect2
static inline bool RectAboveRect(const Rect& rect1, const Rect& rect2) {
	return (rect1.y + rect1.h < rect2.y + 4);
}

// true = left, false = right
static inline void DefaultOrientationSet(Sprite* sprite, bool orientation) {
	if (!orientation)	// go right
		sprite->main_animator->Start((MovingAnimation*)(AnimationHolder::Get().GetAnimation(sprite->GetState() + "R")),
			SystemClock::Get().milli_secs());
	else			// go left
		sprite->main_animator->Start((MovingAnimation*)(AnimationHolder::Get().GetAnimation(sprite->GetState() + "L")),
			SystemClock::Get().milli_secs());
}

static void KillSprite(Sprite* sprite) {
	auto* animator = sprite->main_animator;
	if (animator != nullptr) {
		AnimatorManager::GetSingleton().AddGarbage(sprite->main_animator);
	}
	CollisionChecker::GetSingleton().AddGarbage(sprite); // when collect is called, will destroy all collision with this sprite
	SpriteManager::GetSingleton().AddGarbage(sprite);
}

static void SpriteHop(Sprite* sprite) {
	auto& current_vel = sprite->GetVelocity();
	sprite->SetVelocity({ current_vel.x, HOP_Y_SPEED });
}

static void SpriteHopSmall(Sprite* sprite) {
	auto& current_vel = sprite->GetVelocity();
	sprite->SetVelocity({ current_vel.x, HOP_Y_SPEED_SMALL });
}

void UnitTest3::DamageMario() {
	if (mario->GetState() == "mario") {
		MarioDeath();
		return;
	}

	// transform to small
	auto* mario_transform_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("MARIO_DAMAGE");
	auto* mario_transform_animator = new TickAnimator();

	mario_transform_animator->SetOnStart(
		[this](Animator* animator, const Animation& anim) {
			mario->SetState("mario");
			mario->SetUniformBox({ mario_small.w, mario_small.h });
			start_animator(MARIO_STATE_T::MARIO_STANDING_RIGHT);
			mario->SetVelocity({ 0,0 });
			mario->SetVisibility(false);
			is_flashing = true;
			collision_checker.AddGarbage(mario);
			mario->GetGravityHandler().RemoveGravity();
			mario->GetGravityHandler().Check(mario->GetBoxF());
			mario->SetHasDirectMotion(true);
		}
	);
	mario_transform_animator->SetOnAction(
		[this](Animator* animator, const Animation& anim) {
			//start_animator(); // random anim 0-6
			mario->SetVisibility(!mario->isVisible());
		}
	);
	mario_transform_animator->SetOnFinish(
		[this](Animator* animator) {
			mario->SetVisibility(true);
			mario->SetHasDirectMotion(false);
			mario->GetGravityHandler().SetGravity();
			animator_manager.AddGarbage(animator);

			auto* mario_punishment_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("SUPERMARIO_TRANSITION");
			auto* mario_punishment_animator = new TickAnimator();
			mario_punishment_animator->SetOnAction(
				[this](Animator* animator, const Animation& anim) {
					mario->SetVisibility(!mario->isVisible());
				}
			);
			mario_punishment_animator->SetOnFinish(
				[this](Animator* animator) {
					mario->SetVisibility(true);
					is_flashing = false;
					SetMarioCollisions();
					animator_manager.AddGarbage(animator);
				}
			);
			mario_punishment_animator->Start(*mario_punishment_animation, SystemClock::Get().milli_secs());
		}
	);
	mario_transform_animator->Start(*mario_transform_animation, SystemClock::Get().milli_secs());
}

void UnitTest3::MarioDeath() {

	auto* mario_death_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("MARIO_DEATH_TIMER");
	auto* mario_death_animator = new TickAnimator();

	mario_death_animator->SetOnStart(
		[this](Animator* animator, const Animation& animation) {
			mario->SetState("mario");
			start_animator(MARIO_STATE_T::MARIO_DYING);
			SpriteHop(mario);
			mario->GetGravityHandler().RemoveGravity();
			mario->SetVelocity({ 0, 80 });
			mario->SetHasDirectMotion(true);
		}
	);

	mario_death_animator->SetOnFinish(
		[this](Animator* animator) {
			// get last checkpoints CamPos
			// start over all sprites
			lives -= 1;
			if (lives == 0)
			{
				std::cout << "Game over" << std::endl;
				game_done = 2;
				GameOver();
			}
			else
			{
				reset = true;
				float x,y;
				mario->GetPos(x,y);
				last_checkpoint = GetLastCheckpoint(x);

			}
			mario->SetHasDirectMotion(false);
			animator_manager.AddGarbage(animator);
		}
	);
	mario_death_animator->Start(*mario_death_animation, SystemClock::Get().milli_secs());
	collision_checker.AddGarbage(mario);
}

static inline void DeathWithHopAnim(Sprite* sprite) {
		
	auto* sprite_death_anim = (TickAnimation*)AnimationHolder::Get().GetAnimation("MARIO_DEATH_TIMER");
	auto* sprite_death_animator = new TickAnimator();
	
	sprite_death_animator->SetOnStart(
		[sprite](Animator* animator, const Animation& animation) {
			sprite->SetHasDirectMotion(true);
			//collision_checker.AddGarbage(goomba);
			SpriteHop(sprite);
			//goomba->SetFrame();
			sprite->SetVelocity({0,80});
		}
	);
	sprite_death_animator->SetOnFinish(
		[sprite](Animator* animator) {
			KillSprite(sprite);
		}
	);
	sprite_death_animator->Start(*sprite_death_anim, SystemClock::Get().milli_secs());
}

void GktDeath(Sprite* gkt) {
	CollisionChecker::GetSingleton().AddGarbage(gkt);
	DeathWithHopAnim(gkt);
}

// death_animation 0: duck, 1: jump and fall
void UnitTest3::GoombaDeath(Sprite* goomba, bool death_animation) {
	CollisionChecker::GetSingleton().AddGarbage(goomba);

	if (death_animation == false) {
		goomba->main_animator->SetOnFinish(
			[goomba](Animator* animator) {
				KillSprite(goomba);
			}
		);
		((FrameRangeAnimator*)goomba->main_animator)->Start((FrameRangeAnimation*)(AnimationHolder::Get().GetAnimation("GOOMBA_DEATH")),
			SystemClock::Get().milli_secs());
	}
	else {
		DeathWithHopAnim(goomba);
	}
}

void UnitTest3::SetMarioUpShroomCollision(Sprite* mario, Sprite* shroom) {
	collision_checker.Register(mario, shroom,
		[&](Sprite* mario, Sprite* shroom) {
			if (shroom->main_animator->HasFinished())
				return;
			score += 1000;
			lives += 1;
			KillSprite(shroom);
		}
	);
}

void UnitTest3::SetMarioSuperShroomCollision(Sprite* mario, Sprite* shroom) {
	collision_checker.Register(mario, shroom,
		[&](Sprite* mario, Sprite* shroom) {

			if (shroom->main_animator->HasFinished())
				return;

			score += 1000;
			KillSprite(shroom);

			if (mario->GetState() == "MARIO") {
				return;
			}

			auto* super_mario_transition_anim = (TickAnimation*)AnimationHolder::Get().GetAnimation("SUPERMARIO_TRANSITION");
			auto* super_mario_transition = new TickAnimator();
			super_mario_transition->SetOnStart(
				[&, mario](Animator* animator, const Animation& anim) {
					mario->SetState("MARIO"); // == supermario 	
					mario->SetUniformBox({ mario_big.w, mario_big.h });
					float x, y;
					mario->GetPos(x, y);
					mario->SetPos(x, y - 16);
					auto vel = mario->GetVelocity();
					if (vel.x < 0)
						start_animator(MARIO_STATE_T::MARIO_STANDING_LEFT);
					else
						start_animator(MARIO_STATE_T::MARIO_STANDING_RIGHT);
					mario->SetVelocity({ 0,0 });
					mario->GetGravityHandler().RemoveGravity();
					mario->GetGravityHandler().Check(mario->GetBoxF());
					mario->SetHasDirectMotion(true);
				}
			);
			super_mario_transition->SetOnAction(
				[&, mario](Animator* animator, const Animation& anim) {
					mario->SetVisibility(!mario->isVisible());
				}
			);
			super_mario_transition->SetOnFinish(
				[&, mario](Animator* animator) {
					mario->SetVisibility(true);
					mario->SetHasDirectMotion(false);
					mario->GetGravityHandler().SetGravity();
					animator_manager.AddGarbage(animator);
				}
			);
			super_mario_transition->Start(*super_mario_transition_anim, SystemClock::Get().milli_secs());
		}
	);
}

void UnitTest3::SetMarioStarCollision(Sprite* mario, Sprite* star) {
	collision_checker.Register(mario, star,
		[&](Sprite* mario, Sprite* star) {

			if (star->main_animator->HasFinished())
				return;

			score += 1000;
			KillSprite(star);

			auto* invincible_mario_tick_anim = (TickAnimation*)AnimationHolder::Get().GetAnimation("INVINCIBLE_TIMER");
			auto* invincible_mario_tick_animator = new TickAnimator(); //INVINCIBLE_TIMER

			invincible_mario_tick_animator->SetOnStart(
				[&, mario](Animator* animator, const Animation& anim) {
					//std::cout << " invincible " << std::endl;
					is_invincible = true;
				}
			);

			invincible_mario_tick_animator->SetOnAction(
				[&, mario](Animator* animator, const Animation& anim) {
					mario->SetVisibility(!mario->isVisible());
				}
			);

			invincible_mario_tick_animator->SetOnFinish(
				[&, mario](Animator* animator) {
					// render not invicible
					//std::cout << "Not invincible anymore" << std::endl;
					is_invincible = false;
					mario->SetVisibility(true);
					animator_manager.AddGarbage(animator);
				}
			);
			invincible_mario_tick_animator->Start(*invincible_mario_tick_anim, SystemClock::Get().milli_secs());
		}
	);
}

// pre: all sprites are created first.
void UnitTest3::SetMarioCollisions() {
	auto goomba_sprites = sprite_manager.GetTypeList("GOOMBA");
	for (auto* goomba : goomba_sprites) {
		collision_checker.Register(mario, goomba,
			[&](Sprite* mario, Sprite* goomba) {
				// if mario invincible: kill goomba and return
				if (is_invincible) {
					GoombaDeath(goomba, true);
					score += 100;
					return;
				}

				if (RectAboveRect(mario->GetBox(), goomba->GetBox())) {
					SpriteHop(mario);
					GoombaDeath(goomba, false);
					score += 100;
				}
				else {
					DamageMario();
				}
			}
		);
	}

	auto gkt_sprites = sprite_manager.GetTypeList("GKT");
	for (auto* gkt : gkt_sprites) {
		collision_checker.Register(mario, gkt,
			[&](Sprite* mario, Sprite* gkt) {
				// if mario invincible: kill gkt and return
				if (is_invincible) {
					// kill gkt
					GktDeath(gkt);
					return;
				}

				auto gkt_state = gkt->GetState();
				if (gkt_state == "GKT_W") { // GKT walkin
					if (RectAboveRect(mario->GetBox(), gkt->GetBox())) { // stun it
						SpriteHop(mario);
						gkt->SetState("GKT_S"); // gkt stunned now
						((FrameListAnimator*)gkt->main_animator)->Start((FrameListAnimation*)AnimationHolder::Get().GetAnimation("GKT_S"), SystemClock::Get().milli_secs());
						return;
					}
					DamageMario();
				}
				else if (gkt_state == "GKT_S") { // GKT stunned
					// knock it
					auto mario_box = mario->GetBox();
					auto gkt_box = gkt->GetBox();
					auto mario_center_x = mario_box.x + (mario_box.w / 2);
					auto gkt_center_x = gkt_box.x + (gkt_box.w / 2);
					gkt->SetState("GKT_K");
					DefaultOrientationSet(gkt, mario_center_x > gkt_center_x);

					auto* gkt_collision_pause = new TickAnimator();
					auto* animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("M_GKT_PAUSE_COLLISION");
					auto action = collision_checker.Get(mario, gkt);
					collision_checker.AddGarbage(mario, gkt);
					gkt_collision_pause->SetOnFinish(
						[&, mario, gkt, action](Animator* animator) {
							collision_checker.Register(mario, gkt, action);
							animator_manager.AddGarbage(animator);
						}
					);
					gkt_collision_pause->Start(*animation, SystemClock::Get().milli_secs());

				}
				else {  // GKT_K
					if (RectAboveRect(mario->GetBox(), gkt->GetBox())) {
						SpriteHop(mario);
						gkt->SetState("GKT_S");
						((FrameListAnimator*)gkt->main_animator)->Start((FrameListAnimation*)AnimationHolder::Get().GetAnimation("GKT_S"), SystemClock::Get().milli_secs());
						return;
					}
					DamageMario();
				}
			}
		);
	}

	auto piranha_sprites = sprite_manager.GetTypeList("PIRANHA");
	for (auto* piranha : piranha_sprites) {
		collision_checker.Register(mario, piranha,
			[&](Sprite* mario, Sprite* piranha) {
				if (is_invincible) {
					score += 200;
					KillSprite(piranha);
					return;
				}
				DamageMario();
			}
		);
	}

	auto qmark_sprites = sprite_manager.GetTypeList("QMARK");
	for (auto* qmark : qmark_sprites) {
		collision_checker.Register(mario, qmark,
			[&](Sprite* mario, Sprite* qmark) {
				if (!RectAboveRect(qmark->GetBox(), mario->GetBox())) // if mario not under qmark return.
					return;
				
				auto i = qmark_map.find(qmark);
				assert(i != qmark_map.end());
				auto& list = i->second;
				
				auto* hidden = list.front();
				list.pop_front();

				//std::cout << hidden->GetTypeId() << std::endl;
				if (hidden->GetTypeId() == "COIN") {
					((FrameRangeAnimator*)hidden->main_animator)->Start((FrameRangeAnimation*)AnimationHolder::Get().GetAnimation("COIN"), SystemClock::Get().milli_secs());

				}
				else {
					((FrameRangeAnimator*)hidden->main_animator)->Start((FrameRangeAnimation*)AnimationHolder::Get().GetAnimation(hidden->GetState() + "R"), SystemClock::Get().milli_secs());

				}
				hidden->SetMovingSprite(true);
				SpriteHopSmall(hidden);
				SetDefaultGravity(hidden);

				bool qmark_emptied = list.empty();


				auto action = collision_checker.Get(mario, qmark);
				collision_checker.AddGarbage(mario, qmark);
				
				
				auto* up_qmark_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("QMARK_HOP");
				auto* up_qmark_animator = new TickAnimator();

				up_qmark_animator->SetOnStart(
					[&, qmark](Animator* animator, const Animation& animation) {
						qmark->SetVelocity({ 0, -16 });
					}
				);

				up_qmark_animator->SetOnFinish(
					[&, mario, qmark, hidden, qmark_emptied, action](Animator* animator) {
						hidden->SetHasDirectMotion(false);
						if (hidden->GetTypeId() == "COIN") {
							score += 100;
							coins += 1;
							KillSprite(hidden);
						}
						else if (hidden->GetTypeId() == "UP_SHROOM") {
							SetMarioUpShroomCollision(mario, hidden);
						}
						else if (hidden->GetTypeId() == "SUPER_SHROOM") {
							SetMarioSuperShroomCollision(mario, hidden);
						}
						else if (hidden->GetTypeId() == "STAR") {
							SetMarioStarCollision(mario, hidden);
						}
						else
							assert(0);


						//AnimatorManager::GetSingleton().AddGarbage(animator);
						auto* down_qmark_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("QMARK_HOP");
						auto* down_qmark_animator = new TickAnimator();

						down_qmark_animator->SetOnStart(
							[qmark](Animator* animator, const Animation& animation) {
								qmark->SetVelocity({ 0, 16 });
							}
						);
						down_qmark_animator->SetOnFinish(
							[mario, qmark, qmark_emptied, action](Animator* animator) {
								
								if (qmark_emptied)
									((FrameRangeAnimator*)qmark->main_animator)->Start((FrameRangeAnimation*)AnimationHolder::Get().GetAnimation("QMARK_DED"), SystemClock::Get().milli_secs());
								else {
									qmark->SetVelocity({ 0,0 });
									CollisionChecker::GetSingleton().Register(mario, qmark, action);
								}
									
								AnimatorManager::GetSingleton().AddGarbage(animator);
							}
						);
						down_qmark_animator->Start(*down_qmark_animation, SystemClock::Get().milli_secs());
					}
				);
				up_qmark_animator->Start(*up_qmark_animation, SystemClock::Get().milli_secs());
			}
		);

	}

	/*auto coin_sprites = sprite_manager.GetTypeList("COIN");
	for (auto* coin : coin_sprites) {
		collision_checker.Register(mario, coin,
			[&](Sprite* mario, Sprite* coin) {
				if (coin->main_animator->HasFinished())
					return;
				score += 100;
				coins += 1;
				KillSprite(coin);
			}
		);
	}*/

	auto coin_sprites = sprite_manager.GetTypeList("COIN_FLOAT");
	for (auto* coin : coin_sprites) {
		collision_checker.Register(mario, coin,
			[&](Sprite* mario, Sprite* coin) {
				score += 100;
				coins += 1;
				KillSprite(coin);
			}
		);
	}

//	auto star_sprites = sprite_manager.GetTypeList("STAR");
//	for (auto* star : star_sprites) {
//		collision_checker.Register(mario, star,
//			[&](Sprite* mario, Sprite* star) {
//				
//				if(star->main_animator->HasFinished())
//					return;
//				
//				score += 1000;
//				KillSprite(star);
//
//				auto* invincible_mario_tick_anim = (TickAnimation*)AnimationHolder::Get().GetAnimation("INVINCIBLE_TIMER");
//				auto* invincible_mario_tick_animator = new TickAnimator(); //INVINCIBLE_TIMER
//
//				invincible_mario_tick_animator->SetOnStart(
//					[&, mario](Animator* animator, const Animation& anim) {
//						std::cout << " invincible " << std::endl;
//						is_invincible = true;
//					}
//				);
//
//				invincible_mario_tick_animator->SetOnAction(
//					[&, mario](Animator* animator, const Animation& anim) {
//#pragma warning ("TODO")
//						// some special effect maybe flashing 
//					}
//				);
//
//				invincible_mario_tick_animator->SetOnFinish(
//					[&, mario](Animator* animator) {
//						// render not invicible
//						std::cout << "Not invincible anymore" << std::endl;
//						is_invincible = false;
//						animator_manager.AddGarbage(animator);
//					}
//				);
//				invincible_mario_tick_animator->Start(*invincible_mario_tick_anim, SystemClock::Get().milli_secs());
//			}
//		);
//	}

	//auto super_shroom_sprites = sprite_manager.GetTypeList("SUPER_SHROOM");
	//for (auto* shroom : super_shroom_sprites) {
	//	collision_checker.Register(mario, shroom,
	//		[&](Sprite* mario, Sprite* shroom) {

	//			if(shroom->main_animator->HasFinished())
	//				return;

	//			score += 1000;
	//			KillSprite(shroom);

	//			if (mario->GetState() == "MARIO") {
	//				return;
	//			}

	//			auto* super_mario_transition_anim = (TickAnimation*)AnimationHolder::Get().GetAnimation("SUPERMARIO_TRANSITION");
	//			auto* super_mario_transition = new TickAnimator();
	//			super_mario_transition->SetOnStart(
	//				[&, mario](Animator* animator, const Animation& anim) {
	//					mario->SetState("MARIO"); // == supermario 	
	//					mario->SetUniformBox({ mario_big.w, mario_big.h });
	//					float x, y;
	//					mario->GetPos(x, y);
	//					mario->SetPos(x, y - 16);
	//					auto vel = mario->GetVelocity();
	//					if (vel.x < 0)
	//						start_animator(MARIO_STATE_T::MARIO_STANDING_LEFT);
	//					else
	//						start_animator(MARIO_STATE_T::MARIO_STANDING_RIGHT);
	//					mario->SetVelocity({ 0,0 });
	//					mario->GetGravityHandler().RemoveGravity();
	//					mario->GetGravityHandler().Check(mario->GetBoxF());
	//					mario->SetHasDirectMotion(true);
	//				}
	//			);
	//			super_mario_transition->SetOnAction(
	//				[&, mario](Animator* animator, const Animation& anim) {
	//					mario->SetVisibility(!mario->isVisible());
	//				}
	//			);
	//			super_mario_transition->SetOnFinish(
	//				[&, mario](Animator* animator) {
	//					mario->SetVisibility(true);
	//					mario->SetHasDirectMotion(false);
	//					mario->GetGravityHandler().SetGravity();
	//					animator_manager.AddGarbage(animator);
	//				}
	//			);
	//			super_mario_transition->Start(*super_mario_transition_anim, SystemClock::Get().milli_secs());
	//		}
	//	);
	//}

	/*auto up_shroom_sprites = sprite_manager.GetTypeList("UP_SHROOM");
	for (auto* shroom : up_shroom_sprites) {
		collision_checker.Register(mario, shroom,
			[&](Sprite* mario, Sprite* shroom) {
				if(shroom->main_animator->HasFinished())
					return;
				score += 1000;
				lives += 1;
				KillSprite(shroom);
			}
		);
	}*/
}

void UnitTest3::CreateMario() {
	uint mario_max_speed_x = main_config.GetUint("M_MAX_SPEED_X");
	uint mario_acceleration_x = main_config.GetUint("M_ACCELERATION_X");
	uint mario_air_acceleration_x = main_config.GetUint("M_AIR_ACCELERATION_X");
	int mario_acceleration_y = -main_config.GetInt("M_ACCELERATION_Y");
	int mario_initial_jump_speed = -main_config.GetInt("M_INIT_JUMP_SPEED");
	uint mario_jump_sustained_loops = main_config.GetUint("MAX_M_JUMP_SUSTAIN");

	auto mario_film = AnimationFilmHolder::Get().GetFilm("MARIO");
	auto* mario_start_animation = (FrameRangeAnimation*)AnimationHolder::Get().GetAnimation("mario_sr");
	mario = new Sprite(0, 0, mario_film, "mario");
	
	mario->SetUniformBox({ mario_small.w, mario_small.h });
	SetScene(last_checkpoint);
	mario->SetVelocity({ 0,0 });

	mario->SetState("mario");
	is_invincible = false;

	mario_state = MARIO_STATE_T::MARIO_STANDING_RIGHT;
	auto* mario_animator = new FrameRangeAnimator();
	mario->main_animator = (MovingAnimator*)mario_animator;

	mario_animator->SetOnAction(
		[&](Animator* animator, const Animation& anim) {
			mario->SetFrame(((FrameRangeAnimator*)animator)->GetCurrFrame());
		}
	);

	((FrameRangeAnimator*)mario->main_animator)->Start((FrameRangeAnimation*)mario_start_animation, SystemClock::Get().milli_secs());

	auto* mario_tick_animation = (TickAnimation*)AnimationHolder::Get().GetAnimation("MARIO_MOVE");
	auto* mario_tick_animator = new TickAnimator();

	double delay = (float)mario_tick_animation->GetDelay() / 1000;


	start_animator = [&](MARIO_STATE_T mario_state_t) {
		std::string anim_id = mario->GetState() + GetMarioStateId(mario_state_t);
		if (mario->GetGravityHandler().IsFalling())
		{
			// first time we enter jump animation
			if (mario_state != MARIO_STATE_T::MARIO_JUMPING_RIGHT && mario_state != MARIO_STATE_T::MARIO_JUMPING_LEFT)
			{
				((FrameRangeAnimator*)mario->main_animator)->Start((FrameRangeAnimation*)AnimationHolder::Get().GetAnimation(anim_id), SystemClock::Get().milli_secs());
				mario_state = mario_state_t;
			}
		}
		else
		{
			if (mario_state != mario_state_t)
			{
				((FrameRangeAnimator*)mario->main_animator)->Start((FrameRangeAnimation*)AnimationHolder::Get().GetAnimation(anim_id), SystemClock::Get().milli_secs());
				mario_state = mario_state_t;
			}
		}
	};

	// TODO MOTION:
	// MARIO SMTIMES FALLIN FASTER WTF
	// SPRINT LAT3R? 
	mario_tick_animator->SetOnAction(
		[&, delay, mario_max_speed_x, mario_acceleration_x, mario_acceleration_y, mario_initial_jump_speed, mario_air_acceleration_x, mario_jump_sustained_loops]
	(Animator* animator, const Animation& anim) {
			auto mario_vel = mario->GetVelocity();
			float x, y, dx, dy;
			mario->GetPos(x, y);
			double new_vel_x = mario_vel.x;
			double new_vel_y = mario_vel.y;
			static int jump_sustained_loops = 0;

			if (mario->GetHasDirectMotion()) {
				dx = new_vel_x * delay;
				dy = new_vel_y * delay;
				mario->SetPos(x + dx, y + dy);
				return;
			}

			if (mario->GetGravityHandler().IsFalling())
			{
				if (mario_vel.x < 0)
				{
					start_animator(MARIO_STATE_T::MARIO_JUMPING_LEFT);
				}
				else if (mario_vel.x > 0)
				{
					start_animator(MARIO_STATE_T::MARIO_JUMPING_RIGHT);
				}
				else
				{
					if (mario_state == MARIO_STATE_T::MARIO_STANDING_RIGHT)
						start_animator(MARIO_STATE_T::MARIO_JUMPING_RIGHT);
					else
						start_animator(MARIO_STATE_T::MARIO_JUMPING_LEFT);
				}
			}

			if (movement_keys[ALLEGRO_KEY_W]) {
				if (mario_jump_cd.HasFinished()) {
					if (!mario->GetGravityHandler().IsFalling()) {
						jump_sustained_loops = 0;
						new_vel_y = (double)mario_initial_jump_speed - (0.2 * std::abs(new_vel_x));
					}
					else if (jump_sustained_loops != -1 && jump_sustained_loops < mario_jump_sustained_loops) {
						// keep going up
						jump_sustained_loops++;
						new_vel_y += mario_acceleration_y * delay;
					}
					else
						jump_sustained_loops = -1;
				}
			}
			else {
				jump_sustained_loops = -1;
			}

			if (movement_keys[ALLEGRO_KEY_S]) {
				if (auto* p = pipe_manager.GetVPipeCollision(mario->GetBox())) {
					MarioEnterVPipe((Pipe*)p);
					return;
				}
//#pragma message("REMOVE THIS!")
//				reset = true;
			}

			auto actual_x_acceleration = mario->GetGravityHandler().IsFalling() ? mario_air_acceleration_x : mario_acceleration_x;
			if (movement_keys[ALLEGRO_KEY_A]) {
				if (new_vel_x >= 0) { // hard break
					if (new_vel_x == 0) {
						if (mario_state == MARIO_STATE_T::MARIO_STANDING_RIGHT)
							start_animator(MARIO_STATE_T::MARIO_STANDING_RIGHT);
						else
							start_animator(MARIO_STATE_T::MARIO_STANDING_LEFT);
					}
					else
					{
						start_animator(MARIO_STATE_T::MARIO_BREAKING_LEFT);
					}
					new_vel_x -= 3.0 * actual_x_acceleration * delay;
				}
				else {
					start_animator(MARIO_STATE_T::MARIO_RUNNING_LEFT);
					new_vel_x -= actual_x_acceleration * delay;
				}
			}
			else if (movement_keys[ALLEGRO_KEY_D]) {
				if (auto* p = pipe_manager.GetHPipeCollision(mario->GetBox())) {
					if (!mario->GetGravityHandler().IsFalling()) {
						MarioEnterHPipe((Pipe*)p);
						return;
					}
				}

				if (new_vel_x <= 0) { // hard break
					if (new_vel_x == 0)
						start_animator(MARIO_STATE_T::MARIO_STANDING_RIGHT);
					else
						start_animator(MARIO_STATE_T::MARIO_BREAKING_RIGHT);
					new_vel_x += 3.0 * actual_x_acceleration * delay;
				}
				else {
					start_animator(MARIO_STATE_T::MARIO_RUNNING_RIGHT);
					new_vel_x += actual_x_acceleration * delay;
				}
			}
			else {
				// smooth break


				if (new_vel_x > 0)
					new_vel_x -= 1.3 * actual_x_acceleration * delay;
				else if (new_vel_x < 0)
					new_vel_x += 1.3 * actual_x_acceleration * delay;

				if (std::abs(new_vel_x) < 2 && new_vel_x != 0) {
					if (new_vel_x < 0)
						start_animator(MARIO_STATE_T::MARIO_STANDING_LEFT);
					else
						start_animator(MARIO_STATE_T::MARIO_STANDING_RIGHT);
					new_vel_x = 0;
				}
			}

			if (new_vel_x > mario_max_speed_x)
				new_vel_x = mario_max_speed_x;

			if (new_vel_x < -(double)mario_max_speed_x)
				new_vel_x = -(double)mario_max_speed_x;

			auto tmpx = dx = new_vel_x * delay;
			auto tmpy = dy = new_vel_y * delay;
			FilterGridMotion(mario->GetBoxF(), dx, dy);
			
			
			if (x + dx >= game_over_x && x + dx < scenes["underground_scene"].camera.x){
				game_done = 1;
				GameOver();
			}

			if (x < view_win.x) {
				x = view_win.x;
				dx = 0;
				new_vel_x = 0;
			}


			if (tmpx != dx) { // x motion denied
				new_vel_x = 0;
			}
			if (tmpy != dy) { // y motion denied
				jump_sustained_loops = -1; // stop the jump
				new_vel_y = 0;
			}

			mario->SetVelocity({ new_vel_x, new_vel_y });

			mario->SetPos(x + dx, y + dy);

			mario->SetUpdateBoundAreaPos((dx != 0) || (dy != 0));

			int dist_to_mid = ((x + dx) + 16 - view_win.x) - DIS_WIDTH / 2;
			if (dist_to_mid > 0) { // if x2 of mario is further than the mid of view_win.
				FilterScrollDistance(view_win.x, view_win.w, dist_to_mid, map_dim.w);
				view_win.x += dist_to_mid;
				tile_win_moved = TileAllignedViewBoundCheck();
			}

			mario->GetGravityHandler().Check(mario->GetBoxF());

			
		}
	);
	SetMarioGravity(mario);
	mario_tick_animator->Start(*mario_tick_animation, SystemClock::Get().milli_secs());
}

void UnitTest3::SetMarioGravity(Sprite* sprite, TickAnimation* fall_update) {
	auto speed_increase = g_acceleration * ((double)fall_update->GetDelay() / 1000);

	auto& gravity = sprite->GetGravityHandler();
	gravity.SetGravity();
	gravity.SetOnStartFalling(
		[sprite, fall_update, speed_increase, &gravity, this]() {
			auto* gravity_animator = new TickAnimator();
			gravity_animator->SetOnFinish(
				[sprite, fall_update, speed_increase, this](Animator* gravity_animator) {
					gravity_animator->SetOnFinish(nullptr); // this is prob unnecessary
					gravity_animator->SetOnAction(
						[sprite, speed_increase, this](Animator* gravity_animator, const Animation& gravity_fall) {
							auto& current_vel = sprite->GetVelocity();
							auto new_vel_y = current_vel.y + speed_increase;
							if (new_vel_y > max_fall_speed)
								new_vel_y = max_fall_speed;
							sprite->SetVelocity({ current_vel.x, new_vel_y });
						}
					);
					((TickAnimator*)gravity_animator)->Start(*fall_update, SystemClock::Get().milli_secs());
				}
			);
			gravity.SetOnStopFalling(
				[sprite, gravity_animator, this]() {
					mario_jump_cd.Start(*(TickAnimation*)AnimationHolder::Get().GetAnimation("JUMP_CD"), SystemClock::Get().milli_secs());
					animator_manager.AddGarbage(gravity_animator);
					auto sprite_box = sprite->GetBox();
					if (sprite_box.y + sprite_box.h > view_win.h) { // SPRITE FELL - KILL IT
						MarioDeath();
						return;
					}		
					if (mario->GetVelocity().x > 0) {
						start_animator(MARIO_STATE_T::MARIO_RUNNING_RIGHT);
					}
					else if (mario->GetVelocity().x < 0) {
						start_animator(MARIO_STATE_T::MARIO_RUNNING_LEFT);
					}
					else {
						if (mario_state == MARIO_STATE_T::MARIO_JUMPING_LEFT)
							start_animator(MARIO_STATE_T::MARIO_STANDING_LEFT);
						else if (mario_state == MARIO_STATE_T::MARIO_JUMPING_RIGHT)
							start_animator(MARIO_STATE_T::MARIO_STANDING_RIGHT);
					}
				}
			);
			gravity_animator->Start(*(TickAnimation*)AnimationHolder::Get().GetAnimation("FALL_DELAY"), SystemClock::Get().milli_secs());
		}
	);
	gravity.SetOnSolidGround(
		[this](const Rect_f& rect) {
			float dx = 0;
			float dy = 1;
			FilterGridMotion(rect, dx, dy);
			if (dy < 1)
				return true;
			return false;
		}
	);
}


void UnitTest3::SetDefaultGravity(Sprite* sprite, TickAnimation* fall_update) {
	auto speed_increase = g_acceleration * ((double)fall_update->GetDelay() / 1000);

	auto& gravity = sprite->GetGravityHandler();
	gravity.SetGravity();
	gravity.SetOnStartFalling(
		[sprite, fall_update, speed_increase, &gravity, this]() {
			auto* gravity_animator = new TickAnimator();
			gravity_animator->SetOnFinish(
				[sprite, fall_update, speed_increase, this](Animator* gravity_animator) {
					gravity_animator->SetOnFinish(nullptr); // this is prob unnecessary
					gravity_animator->SetOnAction(
						[sprite, speed_increase, this](Animator* gravity_animator, const Animation& gravity_fall) {
							auto& current_vel = sprite->GetVelocity();
							auto new_vel_y = current_vel.y + speed_increase;
							if (new_vel_y > max_fall_speed)
								new_vel_y = max_fall_speed;
							sprite->SetVelocity({ current_vel.x, new_vel_y });
						}
					);
					((TickAnimator*)gravity_animator)->Start(*fall_update, SystemClock::Get().milli_secs());
				}
			);
			gravity.SetOnStopFalling(
				[sprite, gravity_animator, this]() {
					auto current_vel = sprite->main_animator->GetAnim().GetVelocity();
					sprite->SetVelocity({ current_vel.x, 0 });
					AnimatorManager::GetSingleton().AddGarbage(gravity_animator);

					auto sprite_box = sprite->GetBox();
					if (sprite_box.y + sprite_box.h > view_win.h) { // SPRITE FELL - KILL IT
						KillSprite(sprite);
					}
				}
			);
			gravity_animator->Start(*(TickAnimation*)AnimationHolder::Get().GetAnimation("FALL_DELAY"), SystemClock::Get().milli_secs());
		}
	);
	gravity.SetOnSolidGround(
		[this](const Rect_f& rect) {
			float dx = 0;
			float dy = 1;
			FilterGridMotion(rect, dx, dy);
			if (dy < 1)
				return true;
			return false;
		}
	);
}

template <class T1, class T2>
void CreateGoombaAnimators(std::list<Sprite*> sprites, T2* animation) {
	for (auto* sprite : sprites) {
		T1* animator = new T1();
		animator->SetOnStart(
			[sprite](Animator* animator, const Animation& anim) {
				sprite->SetVelocity(anim->GetVelocity());
			}
		);
		animator->SetOnAction(
			[sprite](Animator* animator, const Animation& anim) {
				sprite->SetFrame(((T1*)(animator))->GetCurrFrame());
			}
		);
		animator->Start(animation, SystemClock::Get().milli_secs());
	}
}


static void DefaultCollisionHandler(Sprite* s1, Sprite* s2) {
	auto& s1_vel = s1->GetVelocity();
	auto& s2_vel = s2->GetVelocity();
	// this is not the way
	
	auto relative_directions = s1_vel.x * s2_vel.x;
	if (relative_directions < 0) { // face 2 face
		DefaultOrientationSet(s1, s1_vel.x > 0);
		DefaultOrientationSet(s2, s2_vel.x > 0);
	}
	else if (relative_directions == 0) {
		if (s1_vel.x == 0)
			DefaultOrientationSet(s2, s2_vel.x > 0);
		else
			DefaultOrientationSet(s1, s1_vel.x > 0);
	}
	else {
		float x1, x2, tmp;
		s1->GetPos(x1, tmp);
		s2->GetPos(x2, tmp);
		if (s1_vel.x > 0) {
			x1 < x2 ? DefaultOrientationSet(s1, true) : DefaultOrientationSet(s2, true);
		}
		else {
			x1 < x2 ? DefaultOrientationSet(s2, false) : DefaultOrientationSet(s1, false);
		}
	}
}

/*
* T1: Animator sub class
* T2: Animation sub class
*
* For each sprite in list, creates new Animator,
* add animation to the animator.
*/
template <class T1, class T2>
void CreateStaticAnimators(std::list<Sprite*> sprites, T2* animation) {
	for (auto* sprite : sprites) {
		T1* animator = new T1();
		animator->SetOnAction(
			[sprite](Animator* animator, const Animation& anim) {
				sprite->SetFrame(((T1*)(animator))->GetCurrFrame());
			}
		);
		animator->Start(animation, SystemClock::Get().milli_secs());
		sprite->main_animator = (MovingAnimator*)animator;
	}
}

/*
* T1: Animator sub class
* T2: Animation sub class
*
* For each sprite in list, creates new Animator,
* add animation to the animator.
*/
template <class T1, class T2>
void CreateMovingAnimators(std::list<Sprite*> sprites, T2* animation) {
	for (auto* sprite : sprites) {
		T1* animator = new T1();
		animator->SetOnStart(
			[sprite](Animator* animator, const Animation& anim) {
				auto& anim_vel = ((const T2&)anim).GetVelocity();
				if (sprite->GetGravityHandler().IsFalling()) {
					auto& sprite_vel = sprite->GetVelocity();
					sprite->SetVelocity({ 0, sprite_vel.y });
				}
				else {
					sprite->SetVelocity(anim_vel);
				}
			}
		);
		animator->SetOnAction(
			[sprite](Animator* animator, const Animation& anim) {
				sprite->SetFrame(((T1*)(animator))->GetCurrFrame());
			}
		);
		animator->Start(animation, SystemClock::Get().milli_secs());
		sprite->SetVelocity(animation->GetVelocity());
		sprite->main_animator = (MovingAnimator*)animator;
		sprite->SetMovingSprite(true);
	}
}


void CreateSpecialMovingAnimators(Sprite* sprite) {
	
	auto* animator = new FrameRangeAnimator();
	animator->SetOnStart(
		[sprite](Animator* animator, const Animation& anim) {
			auto& anim_vel = ((const FrameRangeAnimation&)anim).GetVelocity();
			if (sprite->GetGravityHandler().IsFalling()) {
				auto& sprite_vel = sprite->GetVelocity();
				sprite->SetVelocity({ 0, sprite_vel.y });
			}
			else {
				sprite->SetVelocity(anim_vel);
			}
		}
	);
	animator->SetOnAction(
		[sprite](Animator* animator, const Animation& anim) {
			sprite->SetFrame(((FrameRangeAnimator*)animator)->GetCurrFrame());
		}
	);
	//sprite->SetVelocity(animation->GetVelocity());
	sprite->main_animator = (MovingAnimator*)animator;
	sprite->SetMovingSprite(false);
}

void UnitTest3::CreatePipeInstances() {
	auto hpipes = map_info_parser.GetList("HPIPE");
	for (auto& hpipe : hpipes) {
		auto info = map_info_parser.GetList(hpipe);
		if (info.size() < 3) // if pipe has no scene
			continue;
		uint x = stoul(info[0]);
		uint y = stoul(info[1]);
		auto& emerge_scene = info[2];
		uint y0 = y - 2;
		uint y1 = y + 20;
		new HPipe(emerge_scene, y0, y1, x);
	}

	auto vpipes = map_info_parser.GetList("VPIPE");
	for (auto& vpipe : vpipes) {
		auto info = map_info_parser.GetList(vpipe);
		if (info.size() < 3) // if pipe has no scene
			continue;
		uint x = stoul(info[0]);
		uint y = stoul(info[1]);
		auto& emerge_scene = info[2];
		uint x0 = x + 2;
		uint x1 = x + 14;
		new VPipe(emerge_scene, x0, x1, y);
	}
}

//std::vector<int> GetListInt(std::string&);
std::list<Sprite*> UnitTest3::LoadSpriteList(std::vector<std::string>& list, const AnimationFilm* sprite_film, const std::string& sprite_type, const std::string& default_state) {
	std::list<Sprite*> sprites;
	Sprite* sprite;
	for (auto& str : list) {
		auto sprite_loc = map_info_parser.GetPoint(str);
		sprite = new Sprite(sprite_loc.x, sprite_loc.y, sprite_film, sprite_type);
		sprite->SetState(default_state);
		sprites.push_front(sprite);
	}
	return sprites;
}

std::list<Sprite*> UnitTest3::LoadQmarkSpriteList(std::vector<std::string>& list, const std::string& default_state) {
	std::list<Sprite*> sprites;
	auto* sprite_film = AnimationFilmHolder::Get().GetFilm("QMARK");
	std::list<Sprite*> qmark_list;
	Sprite* sprite;
	for (auto& str : list) {
		auto point = map_info_parser.GetPoint(str);
		sprite = new Sprite(point.x, point.y, sprite_film, "QMARK");
		sprite->SetHasDirectMotion(true);
		sprites.push_front(sprite);
		auto qmark_list = map_info_parser.GetList(str);
		std::list<Sprite*> qmark_sprite_list;
		
		for (int i = 2; i < qmark_list.size(); ++i) {
			auto item_inside = new Sprite(point.x, point.y, AnimationFilmHolder::Get().GetFilm(qmark_list[i]), 150, qmark_list[i]); // z_order 50 < 100
			qmark_sprite_list.push_back(item_inside);
			auto inside_qmark_info = main_config.GetList(qmark_list[i]); // [0] = type(range/list), [1] = is_static, [2] = init_state, [3] = init_anim
			assert(inside_qmark_info[0] == "range");
			item_inside->SetState(inside_qmark_info[2]);
			item_inside->SetHasDirectMotion(true); // disable latr
			
			if (inside_qmark_info[1] == "false" ) {
				CreateSpecialMovingAnimators(item_inside);
			}
			else {
				assert(0);
			}
		}

		qmark_map[sprite] = qmark_sprite_list;
	}
	return sprites;
}


static void SetCollisions(const std::string& type1, const std::string& type2, std::function<void(Sprite* s1, Sprite* s2)> handler) {
	auto type1_sprites = SpriteManager::GetSingleton().GetTypeList(type1);
	auto type2_sprites = SpriteManager::GetSingleton().GetTypeList(type2);
	for (auto type1_sprite : type1_sprites) {
		for (auto type2_sprite : type2_sprites) {
			if (type1_sprite == type2_sprite)
				continue;
			CollisionChecker::GetSingleton().Register(type1_sprite, type2_sprite, handler);
		}
	}
}

void UnitTest3::SpriteLoader() {
	auto& film_holder = AnimationFilmHolder::Get();
	auto& anim_holder = AnimationHolder::Get();

	auto npc_type_list = main_config.GetList("NPCS");
	for (auto& npc_type : npc_type_list) {
		auto npc_type_info = main_config.GetList(npc_type); // [0] = type(range/list), [1] = is_static, [2] = init_state, [3] = init_anim
		auto sprite_list_str = map_info_parser.GetList(npc_type);
		auto sprite_film = film_holder.GetFilm(npc_type);
		
		std::list<Sprite*> sprites_loaded;
		if (npc_type == "QMARK")
			sprites_loaded = LoadQmarkSpriteList(sprite_list_str, npc_type_info[2]);

		else
			sprites_loaded = LoadSpriteList(sprite_list_str, sprite_film, npc_type, npc_type_info[2]);

		if (npc_type_info[0] == "range") { // if sprite is range
			auto* fr_animation = (FrameRangeAnimation*)anim_holder.GetAnimation(npc_type_info[3]);
			if (npc_type_info[1] == "false") { // if sprite is moving:
				CreateMovingAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);
			}
			else
				CreateStaticAnimators<FrameRangeAnimator, FrameRangeAnimation>(sprites_loaded, fr_animation);
		}
		else if (npc_type_info[0] == "list") { // if sprite is list
			auto* fl_animation = (FrameListAnimation*)anim_holder.GetAnimation(npc_type_info[3]);
			if (npc_type_info[1] == "false") { // if sprite is moving:
				//moving_sprites.insert(moving_sprites.end(), sprites_loaded.begin(), sprites_loaded.end());
				CreateMovingAnimators<FrameListAnimator, FrameListAnimation>(sprites_loaded, fl_animation);
			}
			else
				CreateStaticAnimators<FrameListAnimator, FrameListAnimation>(sprites_loaded, fl_animation);
		}
		else { // if type is not range or list
			assert(0);
		}
	}


	SetCollisions("GOOMBA", "GOOMBA", DefaultCollisionHandler);
	SetCollisions("GKT", "GKT", gkt_collision_handler);
	SetCollisions("GOOMBA", "GKT", gkt_collision_handler);



	auto gkts = sprite_manager.GetTypeList("GKT");
	auto goombas = sprite_manager.GetTypeList("GOOMBA");
	auto stars = sprite_manager.GetTypeList("STAR");
	auto upshrooums = sprite_manager.GetTypeList("UP_SHROOM");
	auto super_shrooms = sprite_manager.GetTypeList("SUPER_SHROOM");
	for (auto* sprite : gkts)
		SetDefaultGravity(sprite);
	for (auto* sprite : goombas)
		SetDefaultGravity(sprite);
	for (auto* sprite : stars)
		SetDefaultGravity(sprite);
	for (auto* sprite : upshrooums)
		SetDefaultGravity(sprite);
	for (auto* sprite : super_shrooms)
		SetDefaultGravity(sprite);

	SetPiranhas();
}

void UnitTest3::GarbageCollect() {
	animator_manager.GarbageCollect();
	collision_checker.GarbageCollect();
	sprite_manager.GarbageCollect();
}

void UnitTest3::InstallPauseResumeHandler() {
	std::function<void(void)> pause_resume = [&](void) {
		if (!game.IsPaused()) {
			AnimatorManager::GetSingleton().TimeShift(SystemClock::Get().milli_secs() - game.GetPauseTime());
		}
	};
	game.SetOnPauseResume(pause_resume);
}

static void TogglePauseResume(app::Game& game) {
	if (game.IsPaused())
		game.Resume();
	else
		game.Pause(SystemClock::Get().milli_secs());
}

void UnitTest3::ReloadAllSprites() {
	animator_manager.DestroyAll();
	collision_checker.DestroyAll();
	sprite_manager.DestroyAll();
	qmark_map.clear();
	Load();
}

UnitTest3::UnitTest3() :
	sprite_manager(SpriteManager::GetSingleton()),
	animator_manager(AnimatorManager::GetSingleton()),
	collision_checker(CollisionChecker::GetSingleton()),
	pipe_manager(PipeManager::GetSingleton()) {

	main_config.SetNewParser(MAIN_CONFIG_PATH);
	max_fall_speed = main_config.GetDouble("MAX_FALL_SPEED");
	g_acceleration = main_config.GetDouble("G_ACCELERATION");

	input_mario = [&] {
		if (kb_event_b) {
			if (kb_event.type == ALLEGRO_EVENT_KEY_DOWN) {
				movement_keys[kb_event.keyboard.keycode] = true;
				if (kb_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					TogglePauseResume(game);
			}
			else if (kb_event.type == ALLEGRO_EVENT_KEY_UP) {
				movement_keys[kb_event.keyboard.keycode] = false;
			}
		}
	};


	display_center_text = [&] {
		if (game.IsPaused()) {
			al_draw_text(font0, font0_color, 500, 200, ALLEGRO_ALIGN_CENTRE, "PAUSED");
		}
		if (game_done != 0) {
			if (game_done == 1) {
				al_draw_text(font0, font0_color, 500, 200, ALLEGRO_ALIGN_CENTRE, "WIN!!!");
			}
			else { // lost
				al_draw_text(font0, font0_color, 500, 200, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
			}
		}
	};

	display_texts = [&] {
		// ----- static -----
		al_draw_text(font0, font0_color, 96, 5, ALLEGRO_ALIGN_CENTRE, "SCORE");
		al_draw_text(font0, font0_color, 288, 5, ALLEGRO_ALIGN_CENTRE, "COINS");
		al_draw_text(font0, font0_color, 480, 5, ALLEGRO_ALIGN_CENTRE, "WORLD");
		al_draw_text(font0, font0_color, 672, 5, ALLEGRO_ALIGN_CENTRE, "TIME");
		al_draw_text(font0, font0_color, 864, 5, ALLEGRO_ALIGN_CENTRE, "LIVES");
		// ----- non-static -----
		al_draw_textf(font0, font0_color, 96, 32, ALLEGRO_ALIGN_CENTRE, "%u", score);
		al_draw_textf(font0, font0_color, 288, 32, ALLEGRO_ALIGN_CENTRE, "%u", coins);
		al_draw_textf(font0, font0_color, 480, 32, ALLEGRO_ALIGN_CENTRE, "%s", map_id.c_str());
		al_draw_textf(font0, font0_color, 672, 32, ALLEGRO_ALIGN_CENTRE, "%u", time_left);
		al_draw_textf(font0, font0_color, 864, 32, ALLEGRO_ALIGN_CENTRE, "%u", lives);
	};

	animator_refresh = [&] {
		animator_manager.Progress(SystemClock::Get().milli_secs());
	};

	display_sprites = [&] {
		auto& display_list = sprite_manager.GetDisplayList();
		for (auto& i : display_list) {
			i->Display(NULL, view_win, default_clipper);
		}
	};


	rect_movement = [&] {
		ALLEGRO_EVENT timer_event;
		if (al_get_next_event(timer_queue0, &timer_event)) {
			rect_mvmnt.y = 0;
			rect_mvmnt.x = 0;
			if (movement_keys[ALLEGRO_KEY_W]) {
				rect_mvmnt.y -= RECT_MAX_SPEED_Y;
			}
			if (movement_keys[ALLEGRO_KEY_S]) {
				rect_mvmnt.y += RECT_MAX_SPEED_Y;
			}
			if (movement_keys[ALLEGRO_KEY_A]) {
				rect_mvmnt.x -= RECT_MAX_SPEED_X;
			}
			if (movement_keys[ALLEGRO_KEY_D]) {
				rect_mvmnt.x += RECT_MAX_SPEED_X;
			}

			float dx, dy;
			dx = rect_mvmnt.x * RECT_UPDATE_POS;
			dy = rect_mvmnt.y * RECT_UPDATE_POS;
			FilterGridMotion(rectangle, dx, dy);
			rectangle.x += dx;
			rectangle.y += dy;
			if (rectangle.x < view_win.x) // cannot go left
				rectangle.x = view_win.x;
			// this below isnt real distance. its x coord compared to mid :O
			int dist_to_mid = (rectangle.x + rectangle.w - view_win.x) - DIS_WIDTH / 2;
			if (dist_to_mid > 0) { // if x2 of mario is further than the mid of view_win.
				FilterScrollDistance(view_win.x, view_win.w, dist_to_mid, map_dim.w);
				view_win.x += dist_to_mid;
				tile_win_moved = TileAllignedViewBoundCheck();
			}
		}
	};

	gkt_collision_handler = [&](Sprite* s1, Sprite* s2) {
		if (s1->GetTypeId() == "GKT" && s1->GetState() == "GKT_K") {
			score += 100;
			if (s2->GetTypeId() == "GKT")
				GktDeath(s2);
			else if (s2->GetTypeId() == "GOOMBA")
				GoombaDeath(s2,true);
			else
				assert(0);
			return;
		}
		if (s2->GetTypeId() == "GKT" && s2->GetState() == "GKT_K") {
			score += 100;
			if (s1->GetTypeId() == "GKT")
				GktDeath(s1);
			else if (s1->GetTypeId() == "GOOMBA")
				GoombaDeath(s1,true);
			else
				assert(0);
			return;
		}

		auto& s1_vel = s1->GetVelocity();
		auto& s2_vel = s2->GetVelocity();
		// this is not the way

		auto relative_directions = s1_vel.x * s2_vel.x;
		if (relative_directions < 0) { // face 2 face
			DefaultOrientationSet(s1, s1_vel.x > 0);
			DefaultOrientationSet(s2, s2_vel.x > 0);
		}
		else if (relative_directions == 0) {
			if (s1_vel.x == 0)
				DefaultOrientationSet(s2, s2_vel.x > 0);
			else
				DefaultOrientationSet(s1, s1_vel.x > 0);
		}
		else {
			float x1, x2, tmp;
			s1->GetPos(x1, tmp);
			s2->GetPos(x2, tmp);
			if (s1_vel.x > 0) {
				x1 < x2 ? DefaultOrientationSet(s1, true) : DefaultOrientationSet(s2, true);
			}
			else {
				x1 < x2 ? DefaultOrientationSet(s2, false) : DefaultOrientationSet(s1, false);
			}
		}
	};


	destruct = [&] {
		if (reset) {
			ReloadAllSprites();
			reset = false;
		}
		GarbageCollect();
	};
}


void UnitTest3::Initialise(void) {
	UnitTest2::Initialise();
	al_init_font_addon();
	al_init_ttf_addon();
	Clipper::InitViewWindow(&view_win);
	InstallPauseResumeHandler();

	AnimationFilmHolder::Get().LoadAll(FILMS_DATA_PATH, FilmParser);
	AnimationHolder::Get().LoadAll(ANIMS_FRAME_LIST_PATH, ANIMS_FRAME_RANGE_PATH, ANIMS_TICK_PATH);

	auto font0_path = main_config.GetStr("FONT0");
	auto font0_size = main_config.GetInt("FONT0_SIZE");
	font0 = al_load_font(font0_path.c_str(), font0_size, 0);
	font0_color = al_map_rgb(255, 255, 255);

	map_id = map_info_parser.GetStr("MAP_ID");
	time_left = map_info_parser.GetUint("TIME");
	lives = map_info_parser.GetUint("LIVES");
	checkpoints = map_info_parser.GetList("CHECKPOINTS");

	for (auto& scene_id : map_info_parser.GetList("SCENES")) {
		auto cam_mario_pos = map_info_parser.GetListInt(scene_id);
		UnitTest3::scenes[scene_id] = { {(uint)cam_mario_pos[0], (uint)cam_mario_pos[1]},
										{(uint)cam_mario_pos[2], (uint)cam_mario_pos[3]} };
	}

	mario_small = main_config.GetDim("mario");
	mario_big = main_config.GetDim("MARIO");
	
	game_over_x = map_info_parser.GetInt("GAME_OVER_X");

	CreatePipeInstances();


	game.PushbackRender(render_terrain);
	game.PushbackRender(display_sprites);
	game.PushbackRender(display_texts);
	game.PushbackRender(display_center_text);
	game.PushbackRender(flip_display);
	game.PushbackInput(input_events0);
	game.PushbackInput(input_mario);
	game.PushbackAnim(animator_refresh);
	game.PushbackDestruct(destruct);


	last_checkpoint = checkpoints[0];
}


void UnitTest3::Load(void) {


	CreateMario();
	SpriteLoader();
	SetMarioCollisions();

	// ------------------------------------
	auto* default_mover_refresh = (TickAnimation*)AnimationHolder::Get().GetAnimation("DEFAULT_MOVER");
	float time_delay = (float)default_mover_refresh->GetDelay() / 1000;
	//new TickAnimation("timer", 16, 0, true);
	auto* mover_animator = new TickAnimator();
	mover_animator->SetOnAction(
		[&, time_delay](Animator* animator, const Animation& anim) {
			auto sprites = sprite_manager.GetDisplayList();
			for (auto* sprite : sprites) {
				if (!sprite->IsMovingSprite())
					continue;

				float dx, dy, x, y;
				sprite->GetPos(x, y);
				auto velocity = sprite->GetVelocity();
				auto tmp_dx = dx = velocity.x * time_delay;
				dy = velocity.y * time_delay;

				if (sprite->GetHasDirectMotion()) {
					sprite->SetPos(x + dx, y + dy);
					sprite->SetUpdateBoundAreaPos((dx != 0) || (dy != 0));
					continue;
				}

				FilterGridMotion(sprite->GetBoxF(), dx, dy);
				sprite->SetPos(x + dx, y + dy);
				// remove this.
				if ((velocity.x != 0) && (tmp_dx != dx)) { // motion denied
					DefaultOrientationSet(sprite, velocity.x > 0);
				}
				sprite->SetUpdateBoundAreaPos((dx != 0) || (dy != 0));
				//sprite->SetVelocity({ -sprite->GetVelocity().x, 0 });
				//if (dx == 0) // motion denied
					//sprite->main_animator->Start(goleft/goright, TIME);
				sprite->GetGravityHandler().Check(sprite->GetBoxF());
			}
#pragma message ("why collision check here?")
			collision_checker.Check();
		}
	);
	mover_animator->Start(*default_mover_refresh, SystemClock::Get().milli_secs());
	// ------------------------------------

	auto* every_second_tick = (TickAnimation*)AnimationHolder::Get().GetAnimation("EVRY_SEC");
	auto* every_second_animator = new TickAnimator();
	every_second_animator->SetOnAction(
		[&](Animator* animator, const Animation& anim) {
			time_left--;
			if (time_left == 0) {
				MarioDeath();
				time_is_up = true;
				game_done = 2;
				GameOver();
				//game_finished = true;
				//std::cout << "YOU NOOB\n";
			}
		}
	);
	every_second_animator->Start(*every_second_tick, SystemClock::Get().milli_secs());
}

void UnitTest3::Clear(void) {
	animator_manager.DestroyAll();
	collision_checker.DestroyAll();
	sprite_manager.DestroyAll();
	qmark_map.clear();
	al_destroy_font(font0);
	UnitTest2::Clear();
	auto b = AnimationFilmHolder::Get().GetBitmapLoader();
	b.CleanUp(); //clean bitmaps
	AnimationFilmHolder::Get().CleanUp(); //clean animation films
}

void UnitTest3::Main() {
	UnitTest2::Main();
}