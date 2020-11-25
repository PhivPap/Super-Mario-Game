#include <iostream>
#include <allegro5/allegro.h>

int main(){
    std::cout << "Hello World!\n";
    ALLEGRO_DISPLAY* display = NULL;
    al_init();
    display = al_create_display(640, 480);
    std::cout << "Hello World2\n";
    al_rest(10.0);
}

// clean :d