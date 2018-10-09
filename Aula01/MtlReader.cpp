#include "MtlReader.h"


static vector<string> explode(string delimiter, string source, int limit = numeric_limits<int>::max()) {
	// PHP returns FALSE in this case, but C++ doesn't handle variant types well, and a
	// reasonable facsimile of FALSE is tricky with std::vector<>, so we'll just throw.
	if (delimiter.empty())
		throw std::invalid_argument("Empty delimiter in explode()");

	// Sanitize the limit to something meaningful
	if (limit == 0)
		limit = 1;

	std::vector<std::string> result;

	// Handle the simple case of zero delimiter matches
	if (source.find(delimiter) == std::string::npos) {
		// PHP returns array() if limit is negative, array(source) if limit is positive
		if (limit > 0)
			result.push_back(source);

		return result;
	}

	std::vector<std::string>::size_type begin = 0;
	std::vector<std::string>::size_type end = 0;

	// Explode up to any positive limit, exclusive
	while (end != std::string::npos) {
		if (limit > 0 && result.size() == limit - 1) {
			// PHP populates the last element (ie. result[limit - 1]) with the remaining 
			// string when limit is positive and less than the number of total tokens.
			result.push_back(source.substr(begin));
			break;
		}

		/*
		Conventional C++ manual string tokenization.
		*/
		std::vector<std::string>::size_type substr_len = std::string::npos;
		std::vector<std::string>::size_type next_tok = std::string::npos;

		end = source.find_first_of(delimiter, begin);

		if (end != std::string::npos) {
			substr_len = end - begin;
			next_tok = end + delimiter.size();
		}

		result.push_back(source.substr(begin, substr_len));
		begin = next_tok;
	}

	// Negative limits trim that amount from the right side of the result. It's
	// easier to do this after the explode loop because we don't need to retain
	// unexploded parts of the string like with a positive limit.
	if (limit < 0) {
		limit = abs(limit);

		if (limit < result.size())
			result.resize(result.size() - limit);
		else {
			// PHP returns array() if all items or more would be trimed
			result.clear();
		}
	}

	return result;
}

MtlReader::MtlReader()
{
}


MtlReader::~MtlReader()
{
}

unordered_map<string, Material*> MtlReader::readMaterialsFile(std::string filename) {
	ifstream infile(filename);
	string line;

	unordered_map<string, Material*> materials;
	Material* material = NULL;
	while (getline(infile, line))
	{
		if (!line.empty()) {
			if (line.at(0) != '#') { //ignorar comentarios

				//check for tabs
				int pos = line.find("\t");
				if (pos >= 0) line = line.substr(pos + 1, line.size());
				pos = line.find(" ");

				string firstWord = line.substr(0, pos);

				if (firstWord.compare("newmtl") == 0) {
					if (material)
						materials[material->mtlName] = material;
					material = new Material();
					material->mtlName = line.substr(pos + 1, line.size());
				}
				else {
					vector<string> temp = explode(" ", line.substr(pos + 1, line.size()));
					if (firstWord.compare("Ns") == 0)
						material->ns = (atof(temp.at(0).c_str()));
					else if (firstWord.compare("Ni") == 0)
						material->ni = (atof(temp.at(0).c_str()));
					else if (firstWord.compare("d") == 0)
						material->d = (atof(temp.at(0).c_str()));
					else if (firstWord.compare("illum") == 0)
						material->illum = (atof(temp.at(0).c_str()));
					else if (firstWord.compare("Tf") == 0)
						material->tf = glm::vec3(atof(temp.at(0).c_str()), atof(temp.at(1).c_str()), atof(temp.at(2).c_str()));
					else if (firstWord.compare("Ka") == 0)
						material->ka = glm::vec3(atof(temp.at(0).c_str()), atof(temp.at(1).c_str()), atof(temp.at(2).c_str()));
					else if (firstWord.compare("Kd") == 0)
						material->kd = glm::vec3(atof(temp.at(0).c_str()), atof(temp.at(1).c_str()), atof(temp.at(2).c_str()));
					else if (firstWord.compare("Ks") == 0)
						material->ks = glm::vec3(atof(temp.at(0).c_str()), atof(temp.at(1).c_str()), atof(temp.at(2).c_str()));
					else if (firstWord.compare("Ke") == 0)
						material->ke = glm::vec3(atof(temp.at(0).c_str()), atof(temp.at(1).c_str()), atof(temp.at(2).c_str()));
					else if (firstWord.compare("map_Kd") == 0)
						material->map_kd = temp.at(0);
				}
			}
		}
	}
	if (material)
		materials[material->mtlName] = material;
	return materials;
}
