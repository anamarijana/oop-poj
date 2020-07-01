#include "Compiler.h"

Compiler* Compiler::Instance(const string& filepath1, const string filepath2){
	static Compiler *instance = new Compiler(filepath1,filepath2);
	return instance;
}

void Compiler::catchOperation(){
	for (int i = 0; i < forest_gump_.size(); i++) {
		
		for (int j = 0; j < forest_gump_[i]->getOps().size(); j++) {
			all_operations.push_back(forest_gump_[i]->getOps()[j]);
		}
	}
}

void Compiler::compile(){
	setRootsReady();
	int id = 1;
	int token_id = 1;
	
	string new_file_name = "glupi_pera.txt";
	/*
	unsigned int len_without_txt = new_file_name.length() - 4;
	new_file_name.resize(len_without_txt);
	new_file_name.insert(len_without_txt, ".imf");*/
	this->filename = new_file_name;
	fstream outputFile(new_file_name, ios::out);
	
	while (this->roots_ready == 0) {
		for (int i = 0; i < all_operations.size(); i++) {
			Element* child1 = 0;
			Element* child2 = 0;
			child1 = all_operations[i]->getIn()[0];
			if(all_operations[i]->getIn().size()==2) child2 = all_operations[i]->getIn()[1];
			string destination;
			char mother_operation;
			mother_operation = defineMotherOp(all_operations[i]);
			

			if (child2) {
				if (child1->getReady() & child2->getReady()) {
					outputFile << "[" << id << "]" << " " << mother_operation << " " << "t" << token_id << " " <<
						child2->getDestination() << " " << child1->getDestination()<< endl;
					all_operations[i]->setReady(1);
					destination = 't' + to_string(token_id++);
					all_operations[i]->setDestination(destination);
					id++;
				
				}
			}
			else {
				if (child1->getReady()) {
					outputFile << "[" << id << "]" << " " << "=" << " " << all_operations[i]->getDestination() << " "
						 << child1->getDestination() << endl;
					all_operations[i]->setReady(1);
					id++;
				}
			}
		}
		setRootsReady();
	}
	outputFile.close();
}

void Compiler::setRootsReady(){
	this->roots_ready = 1;
	for (int i = 0; i < forest_gump_.size(); i++) {
		this-> roots_ready = this->roots_ready & this->forest_gump_[i]->getRoot()->getReady();
	}
}

const string& Compiler::giveMachinaFile(){
	return this->filename;
}

char Compiler::defineMotherOp(Operation* mother){

	Element_Type type = mother->getType();

	switch (type)
	{

	case ADDITION:
		return '+'; break;
	case MULTIPLICATION:
		return '*'; break;
	case EXPONENTIATION:
		return '^'; break;
	case ASSIGNMENT:
		return '='; break;

	}
	
}

Compiler::Compiler(const string& filepath1, const string filepath2){
	Configuration::Instance(filepath1);
	Program::Instance(filepath2);
	this->filename = filepath2;
	int j = 0;
	map<int, string> help = Program::returnInstance()->getVarNameExp();
	for (auto i = help.begin(); i != help.end(); i++) {
		string* expres = &(i->second);
		 ExpressionTree* tree = new ExpressionTree(expres);
		 this->forest_gump_.push_back(tree);
	}
	catchOperation();
	compile();
}

