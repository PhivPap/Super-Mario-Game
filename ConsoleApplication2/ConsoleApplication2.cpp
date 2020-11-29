#include <iostream>
#include <fstream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "Engine.h"

class UnitTest : app::App {
public:
	virtual void Initialise(void) {
		std::cout << "Initialise\n";
	}
	virtual void Load(void) {
		std::cout << "Load\n";
	}
	virtual void Run(void) { 
		std::cout << "Run\n";
	}
	virtual void RunIteration(void) {
	}
	virtual void Clear(void) {
		std::cout << "Clear\n";
	}
	void Main() {
		std::cout << sizeof(game) << std::endl;
		app::App::Main();
	}
};

int main(){
	UnitTest utest;
	utest.Main();

	ALLEGRO_BITMAP* bitmap;
	al_init();
	al_init_image_addon(); //bitmaps, pngs, jpegs etc
	/* mario is 16x32  */
	bitmap = al_load_bitmap("super_mario.png");
	
	std::cout << (bitmap == NULL) << std::endl;


    //std::cout << "Hello World!\n";
    //ALLEGRO_DISPLAY* display = NULL;
    //al_init();
    //display = al_create_display(640, 480);
    //std::cout << "Hello World2\n";
    //al_rest(10.0);
}