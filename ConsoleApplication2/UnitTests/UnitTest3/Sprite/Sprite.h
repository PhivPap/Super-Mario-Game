#pragma once
#include <functional>
#include "Types.h"
#include "AnimationFilm.h"
#include "MotionQuantizer.h"
#include "Clipper.h"
#include "BoundingArea.h"

class Sprite {
public:
	using Mover = std::function<void(const Rect&, int&, int&)>;
protected:
	byte frame_no = 0;
	Rect frame_box;
	int x = 0, y = 0;
	bool is_visible = false;
	AnimationFilm* curr_film = nullptr;
	BoundingArea* bounding_area = nullptr;
	uint z_order = 0;
	std::string type_id, state_id;
	Mover mover;
	MotionQuantizer quantizer;
public:
	Sprite(int, int, AnimationFilm*, const std::string&);
	void SetMover(const Mover&);
	const Rect GetBox(void) const;
	void Move(int, int);
	void SetPos(int, int);
	void SetZorder(uint);
	uint GetZorder(void);
	void SetFrame(byte);
	byte GetFrame(void) const;
	void SetBoundingArea(const BoundingArea&);
	void SetBoundingArea(BoundingArea*);
	const BoundingArea* GetBoundArea(void) const;
	const std::string& GetTypeId(void);
	void SetVisibility(bool);
	bool isVisible(void) const;
	bool CollisionCheck(const Sprite*) const;
	void Display(ALLEGRO_BITMAP*, const Rect&, const Clipper&) const;
};

