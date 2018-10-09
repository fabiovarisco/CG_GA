#include "OBJReader.h"


OBJReader::OBJReader()
{
}


OBJReader::~OBJReader()
{
}


Mesh *OBJReader::read(std::string filename) {
	Mesh *mesh = new Mesh();
	std::cout << "Started Reading OBJ File\n";
	std::cout << filename;
	std::cout << "\n";
	std::ifstream arq(filename);

	Group *current = NULL;
	while (!arq.eof()) {
		std::string line;
		getline(arq, line);
		std::stringstream sline;
		sline << line;
		std::string temp;
		std::string temp2;
		sline >> temp;
		
		if (temp == "mtllib") {
			std::cout << "Material File";
			sline >> temp2;
			std::cout << temp2;
			std::cout << "\n";
			mesh->materialsFile = temp2;
		}
		else if (temp == "o") {
			std::cout << "Reading object";
			sline >> temp2;
			std::cout << temp2;
			std::cout << "\n";
		}
		else if (temp == "v") {
			readVertex(sline, *mesh);
		}
		else if (temp == "vt") {
			readMapping(sline, *mesh);
		}
		else if (temp == "vn") {
			readNormal(sline, *mesh);
		}
		else if (temp == "g") {
			if (current)
			{
				if (current->getName() == "default") {
					sline >> temp2;
					current->setName(temp2);
					continue;
				}
				else {
					mesh->getGroups().push_back(current);
				}
			}
			current = new Group();
			sline >> temp2;
			current->setName(temp2);
		}
		else if (temp == "usemtl") {
			if (!current) {
				current = new Group();
				current->setName("default");
			}
			sline >> temp2;
			current->setMaterial(temp2);
		}
		else if (temp == "f") {
			readFace(sline, *current);
		}
		else if (temp == "#") {
			continue;
		}
	}
	if (current)
		mesh->getGroups().push_back(current);

	return mesh;
}

void OBJReader::readVertex(std::stringstream& sline, Mesh& mesh) {
	float x, y, z;
	sline >> x >> y >> z;

	mesh.getVertexes().push_back(glm::vec3(x, y, z));
}

void OBJReader::readMapping(std::stringstream& sline, Mesh& mesh) {
	float x, y;
	sline >> x >> y;

	mesh.getMappings().push_back(glm::vec2(x, y));
}

void OBJReader::readNormal(std::stringstream& sline, Mesh& mesh) {
	float x, y, z;
	sline >> x >> y, z;

	mesh.getNormals().push_back(glm::vec3(x, y, y));
}

void OBJReader::readFace(std::stringstream& sline, Group& group) {
	Face* f = new Face();
	std::string token;

	for (int i = 0; i < 3; i++) {
		sline >> token; // v/t/n, por exemplo
		std::stringstream stoken;
		stoken << token;
		int aux = readNextInt(stoken);
		if (aux != -1)
			f->getVerts().push_back(aux);
		aux = readNextInt(stoken);
		if (aux != -1)
			f->getTexts().push_back(aux);
		aux = readNextInt(stoken);
		if (aux != -1)
			f->getNorms().push_back(aux);
	}
	group.getFaces().push_back(f);
	bool has4 = (bool) (sline >> token);
	if (has4) {
		Face* f2 = new Face();
		f2->getVerts().push_back(f->getVerts().at(0));
		f2->getVerts().push_back(f->getVerts().at(2));
		if (!f->getTexts().empty()) {
			f2->getTexts().push_back(f->getTexts().at(0));
			f2->getTexts().push_back(f->getTexts().at(2));
		}
		if (!f->getNorms().empty()) {
			f2->getNorms().push_back(f->getNorms().at(0));
			f2->getNorms().push_back(f->getNorms().at(2));
		}
		std::stringstream stoken;
		stoken << token;
		int aux = readNextInt(stoken);
		if (aux != -1)
			f2->getVerts().push_back(aux);
		aux = readNextInt(stoken);
		if (aux != -1)
			f2->getTexts().push_back(aux);
		aux = readNextInt(stoken);
		if (aux != -1)
			f2->getNorms().push_back(aux);
		group.getFaces().push_back(f2);
	}
	
}

int OBJReader::readNextInt(std::stringstream& stoken) {
	std::string aux;
	getline(stoken, aux, '/');
	if (aux.empty()) {
		return -1;
	}
	else {
		return std::stoi(aux) - 1;
	}
}