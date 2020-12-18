#include "Level1.h"

bool UnitTest::TileAllignedViewBoundCheck() {
	bool moved = false;
	if (view_win.x < tile_view_win.x) {
		tile_view_win.x = TILE_WIDTH * (uint)(view_win.x / TILE_WIDTH);
		moved = true;
	}
	if (view_win.y < tile_view_win.y) {
		tile_view_win.y = TILE_HEIGHT * (uint)(view_win.y / TILE_HEIGHT);
		moved = true;
	}
	if (view_win.x + view_win.w > tile_view_win.x + tile_view_win.w) {
		tile_view_win.x = TILE_WIDTH * (uint)(view_win.x / TILE_WIDTH);
		moved = true;
	}
	if (view_win.y + view_win.h > tile_view_win.y + tile_view_win.h) {
		tile_view_win.y = TILE_HEIGHT * (uint)(view_win.y / TILE_HEIGHT);
		moved = true;
	}
	return moved;
}

void UnitTest::ScrollWithBoundsCheck(int dx, int dy, bool& tile_win_moved) {
	FilterScroll(dx, dy);
	Scroll(dx, dy);
	tile_win_moved = TileAllignedViewBoundCheck();
}
	
void UnitTest::Scroll(int dx, int dy) {
	view_win.x += dx;
	view_win.y += dy;
}

void UnitTest::FilterScrollDistance(uint viewStartCoord, uint viewSize, int& d, uint map_dim) {
	int val = d + viewStartCoord;
	if (val < 0)
		d = -(int)viewStartCoord;
	else if ((val + viewSize) >= map_dim)
		d = map_dim - (viewStartCoord + viewSize + 1);
}

void UnitTest::FilterScroll(int& dx, int& dy) {
	FilterScrollDistance(view_win.x, view_win.w, dx, map_dim.w);
	FilterScrollDistance(view_win.y, view_win.h, dy, map_dim.h);
}

void UnitTest::TileTerrainDisplay() const {
	if(tile_win_moved){
		auto start_row = tile_view_win.y / TILE_HEIGHT;
		auto start_col = tile_view_win.x / TILE_WIDTH;
		auto end_row = (tile_view_win.y + tile_view_win.h - 1) / TILE_HEIGHT;
		auto end_col = (tile_view_win.x + tile_view_win.w - 1) / TILE_WIDTH;
		uint dest_row = 0, dest_col = 0;
		for (auto row = start_row; row <= end_row; row++) {
			for(auto col = start_col; col <= end_col; col++) {
				// source is map, tile bitmap
				// draw to tile alligned bitmap
				al_set_target_bitmap(tile_alligned);
				al_draw_bitmap_region(
					tileset,
					(map[row][col] % tileset_width) * TILE_WIDTH,
					(uint)(map[row][col] / tileset_width) * TILE_HEIGHT,
					TILE_WIDTH,
					TILE_HEIGHT,
					dest_col * TILE_WIDTH,
					dest_row * TILE_HEIGHT,
					0);
					dest_col++;
			}
			dest_col = 0;
			dest_row++;
		}
		al_set_target_backbuffer(display);
	}

	al_draw_bitmap_region(
					tile_alligned,
					view_win.x - tile_view_win.x,
					view_win.y - tile_view_win.y,
					DIS_WIDTH,
					DIS_HEIGHT,
					0,
					0,
					0);
	al_flip_display();
}

void UnitTest::ReadTextMap(std::vector<std::vector<byte>>& map, Dim& map_dim) {
	std::ifstream map_file(TILEMAP_PATH);
	std::string line;
	std::string token = "";
	while (getline(map_file, line)) {
		std::vector<byte> row;
		auto i = 0;
		auto line_len = line.length();
		while (i < line_len) {
			if (line[i] == ',') {
				row.push_back((byte)std::stoi(token, nullptr));
				token.clear();
				i++;
				continue;
			}
			token += line[i++];
		}
		if (!token.empty()) {
			row.push_back((byte)std::stoi(token, nullptr));
			token.clear();
		}
		map.push_back(row);
	}
	map_dim.w = (uint)map[0].size() * TILE_WIDTH;
	map_dim.h = (uint)map.size() * TILE_HEIGHT;
	map_file.close();
}


UnitTest::UnitTest() {
	display = 0;
	tileset = 0;
	tile_alligned = 0;
	display_queue = 0;
	keyboard_queue = 0;
	mouse_queue = 0;
	timer = 0;
	tileset_width = 0;
	game_finished = false;
	tile_win_moved = true;
	frames = 0;
	counter = 0;
	view_win = { 0, 0, DIS_WIDTH, DIS_HEIGHT };
	tile_view_win = { 0, 0, DIS_WIDTH + TILE_WIDTH, DIS_HEIGHT + TILE_HEIGHT };
	map_dim = {0};
	
	done = [=] {
		return game_finished;
	};

	render = [&] {
		frames++;
		TileTerrainDisplay();
	};

	input = [&] {
		ALLEGRO_EVENT display_event;
		ALLEGRO_EVENT kb_event;
		ALLEGRO_EVENT mouse_event;

		if (al_get_next_event(display_queue, &display_event)) {
			if (display_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				game_finished = true;
		}

		tile_win_moved = false;
		if (al_get_next_event(keyboard_queue, &kb_event)) {
			if (kb_event.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (kb_event.keyboard.keycode == ALLEGRO_KEY_UP) {
					ScrollWithBoundsCheck(0, -SCROLL_DIST, tile_win_moved);
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
					ScrollWithBoundsCheck(0, SCROLL_DIST, tile_win_moved);
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
					ScrollWithBoundsCheck(SCROLL_DIST, 0, tile_win_moved);
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
					ScrollWithBoundsCheck(-SCROLL_DIST, 0, tile_win_moved);
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_HOME) {
					view_win.x = 0;
					view_win.y = 0;
					tile_view_win.x = 0;
					tile_view_win.y = 0;
					tile_win_moved = true;
				}
				else if (kb_event.keyboard.keycode == ALLEGRO_KEY_END) {
					view_win.x = map_dim.w - DIS_WIDTH;
					view_win.y = map_dim.h - DIS_HEIGHT;
					tile_view_win.x = view_win.x - TILE_WIDTH;
					tile_view_win.y = view_win.y - TILE_HEIGHT;
					tile_win_moved = true;
				}
			}
		}

		if (al_get_next_event(mouse_queue, &mouse_event)) {
			if (mouse_event.mouse.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				if (mouse_event.mouse.button == 1) {
					//al_hide_mouse_cursor(display);
					//freeze Game (input, animations, physics, etc..). We don't care, mouse scrolling will be discarded prob.
					while (mouse_event.mouse.type != ALLEGRO_EVENT_MOUSE_BUTTON_UP) { 
						//move relative to view window centre (640/2 , 480/2)
						int dx = (mouse_event.mouse.x - DIS_WIDTH / 2 < 0) ? -1 : 1; // move 1 pixel cuz we fast and map small.
						int dy = (mouse_event.mouse.y - DIS_HEIGHT / 2 < 0) ? -1 : 1;
						ScrollWithBoundsCheck(dx, dy, tile_win_moved);
						TileTerrainDisplay();
						al_get_next_event(mouse_queue, &mouse_event);
						//uncomment for click smashing
						//break; 
					}
				}
			}
		}
	};
}

void UnitTest::Initialise(void) {
	al_init();
	al_init_image_addon(); //bitmaps, pngs, jpegs etc
	al_install_mouse();
	al_install_keyboard();
	display_queue = al_create_event_queue();
	keyboard_queue = al_create_event_queue();
	mouse_queue = al_create_event_queue();
	display = al_create_display(DIS_WIDTH, DIS_HEIGHT);
	al_register_event_source(display_queue, al_get_display_event_source(display));
	al_register_event_source(keyboard_queue, al_get_keyboard_event_source());
	al_register_event_source(mouse_queue, al_get_mouse_event_source());
	tileset = al_load_bitmap(TILESET_PATH);
	tileset_width = al_get_bitmap_width(tileset) / TILE_WIDTH;
	tile_alligned = al_create_bitmap(tile_view_win.w, tile_view_win.h);
}
void UnitTest::Load(void) {
	ReadTextMap(map, map_dim);
	game.SetRender(render);
	game.SetDone(done);
	game.SetInput(input);
	old_time = std::chrono::high_resolution_clock::now();
}

void UnitTest::Clear(void) {
	new_time = std::chrono::high_resolution_clock::now();
	std::cout << (double)frames / (double)((double)std::chrono::duration_cast<std::chrono::microseconds>(new_time - old_time).count() / 1000000.0f) << std::endl;
	al_destroy_bitmap(tileset);
	al_destroy_bitmap(tile_alligned);
	al_destroy_display(display);
	al_uninstall_keyboard();
	al_uninstall_mouse();
}

std::vector<std::vector<byte>>& UnitTest::getMapRef() {
	return map;
}

void UnitTest::Main() {
	app::App::Main();
}