

#include "Scene.h"


int main (){

	Scene* scene = new Scene();

	scene->init("C:\\Users\\I844141\\Documents\\Unisinos\\2018-2\\computacao_grafica\\obj_files\\stool\\", "stool.obj");

	scene->draw();

}