#pragma once
#include <functional>
#include "Types.h"
#include "AnimationFilm.h"
//#include "MotionQuantizer.h"
#include "Clipper.h"
#include "BoundingArea.h"

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

public:
	Sprite(int, int, const AnimationFilm*, const std::string&);
	~Sprite();
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
	const BoundingArea* GetBoundArea(void) const;
	const std::string&	GetTypeId(void);
	void				SetVisibility(bool);
	bool				isVisible(void) const;
	bool				CollisionCheck(const Sprite*) const;
	void				Display(ALLEGRO_BITMAP*, const Rect&, const Clipper&) const;
	void				QuantizerSetRange(int, int);

	void				SetVelocity(const Velocity&);
	const Velocity&		GetVelocity(void);
};

