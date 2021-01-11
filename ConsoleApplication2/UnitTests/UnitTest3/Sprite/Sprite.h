#pragma once
#include <functional>
#include "Types.h"
#include "AnimationFilm.h"
//#include "MotionQuantizer.h"
#include "Clipper.h"
#include "BoundingArea.h"
#include "MovingAnimator.h"
#include "GravityHandler.h"

class Sprite {
public:
	using Mover = std::function<void(const Rect&, int&, int&)>;

protected:
	int horiz_max = 0, vert_max = 0; // quantum
	bool quantizer_used = false;
	byte frame_no = 0;
	Rect frame_box;
	float x = 0, y = 0;
	Velocity velocity;
	bool is_visible = false;
	const AnimationFilm* curr_film = nullptr;
	BoundingArea* bounding_area = nullptr;
	uint z_order = 0;
	std::string type_id, state_id;
	Mover mover;
	//MotionQuantizer quantizer;

	bool direct_motion = false;
	GravityHandler gravity;

	bool update_bound_area_pos = false; // when this is true GetBoundArea() will 
public:
	MovingAnimator*		main_animator;
	Sprite(int, int, const AnimationFilm*, const std::string&);
	~Sprite();
	void				SetState(const std::string&);
	const std::string&	GetState(void);
	void				GetPos(float&, float&);
	void				SetMover(const Mover&);
	const Rect_f		GetBoxF(void) const;
	const Rect			GetBox(void) const;
	void				Move(int, int);
	void				SetPos(float, float);
	void				SetZorder(uint);
	uint				GetZorder(void);
	void				SetFrame(byte);
	byte				GetFrame(void) const;
	void				SetBoundingArea(const BoundingArea&);
	void				SetBoundingArea(BoundingArea*);
	void				ResetBoundingArea(const BoundingArea&);
	void				ResetBoudingArea(BoundingArea*);
	const BoundingArea* GetBoundArea(void);
	const std::string&	GetTypeId(void);
	void				SetVisibility(bool);
	bool				isVisible(void) const;
	bool				CollisionCheck(Sprite*);
	void				Display(ALLEGRO_BITMAP*, const Rect&, const Clipper&) const;
	void				QuantizerSetRange(int, int);

	void				SetVelocity(const Velocity&);
	const Velocity&		GetVelocity(void);

	GravityHandler&		GetGravityHandler(void);
	void				SetHasDirectMotion(bool direct_motion);
	bool				GetHasDirectMotion(void) const;

	void				SetUpdateBoundAreaPos(bool);
};

