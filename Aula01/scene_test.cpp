

#include "Scene.h"


int main (){

	Scene* scene = new Scene();

	//scene->init("C:\\Users\\I844141\\Documents\\Unisinos\\2018-2\\computacao_grafica\\obj_files\\paintball\\", "cenaPaintball.obj");
	//scene->init("C:\\Users\\I844141\\Documents\\Unisinos\\2018-2\\computacao_grafica\\obj_files\\mesa\\", "mesa01.obj");
	scene->init("C:\\Users\\I844141\\Documents\\Unisinos\\2018-2\\computacao_grafica\\obj_files\\pokemon\\", "Dragonite.obj");

	scene->draw();

}