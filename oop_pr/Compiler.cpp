#include "Compiler.h"

Compiler* Compiler::Instance(const string& filepath1, const string& filepath2){
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
//ovde imamo narno stablo i ukoliko cvor zavisi od promenljive nju zamenjujemo cvorom dodele vrednosti toj promenljivoj
void Compiler:: tieUp() {
	Element* Assign = 0;
	Element* child = 0;
	for(int i=0 ; i<all_operations.size();i++){
		for (int j =0; i<all_operations[i]->getIn().size(); i++){
			child = all_operations[i]->getIn()[j];
			if (child->getType() == VARIABLE){
				Assign = returnAss(child->getDestination());
				all_operations[i]->getIn()[j] = Assign; //ovo ispraviti na setIn
				delete child; //ne bi trebalo da postoje viseci pokazivaci
				
			}
		}
		
	}
} //posle ove operacije ne bi trebalo da postoje cvorovi sa varijablama vec da je ceo fajl uvezan u stablo


Element* Compiler:: birth(Element* mother){ //fja koja treba da se poziva u okviru napredne kompilacije za cvorove od vise od troje dece
	Element* operand;
	stack <Element*> operands;
	Operation* newMother =0;
	
	for (int j = 0; j<mother->getIn().size();j++){
		operand = mother->getIn()[j];
		if((operand->getReady() == 1)&&(operands.size()<2))
		operands.push(operand);
	}
	switch (mother->getType())
	{
	case(ADDITION): 
		newMother = new Addition(ADDITION); break; 
	case(MULTIPLICATION):
		newMother = new Multiplication(MULTIPLICATION); break;
	}
	newMother->setIn(operands.top());
	operands.pop();
	newMother->setIn(operands.top());
	operands.pop();

	mother->popIn();
	mother->popIn();

	mother->setIn(newMother);

	return newMother;

}

void Compiler:: compileAdvanced(){
	string new_file_name = "glupi_pera.txt";
	/*
	unsigned int len_without_txt = new_file_name.length() - 4;
	new_file_name.resize(len_without_txt);
	new_file_name.insert(len_without_txt, ".imf");*/
	this->filename = new_file_name;
	fstream outputFile(new_file_name, ios::out);

	
	
	string toOutput;
	
	setRootsReady();
	Element* soon_printed;
	
	while (this->roots_ready == 0) {
		for (int i = 0; i < all_operations.size(); i++) {

			if (all_operations[i]->getIn().size() > 2) {
				soon_printed = birth(all_operations[i]);
			}
			else soon_printed = all_operations[i];
			
			compileOne(soon_printed,toOutput);
			outputFile << toOutput << endl;
		}
		setRootsReady();
	}
	
	
	outputFile.close();

}

void Compiler::compileOne(Element* soon_printed, string& toOutput){
	
	int static id_ = 1;
	int static token_id = 1;
	Element* child1 = 0;
	Element* child2 = 0;
	child1  = soon_printed->getIn()[0];
	if (soon_printed->getIn().size() == 2) child2 = soon_printed->getIn()[1];
	string destination;
	char mother_operation;
	mother_operation = defineMotherOp(soon_printed);


	if (child2) {
		if (child1->getReady() & child2->getReady()) {
			toOutput = "[" + to_string(id_) + "]" + " " + mother_operation + " " + "t" + to_string(token_id) + " " +
				child2->getDestination() + " " + child1->getDestination();
			soon_printed->setReady(1);
			destination = 't' + to_string(token_id++);
			soon_printed->setDestination(destination);
			id_++;

		}
	}
	else {
		if (child1->getReady()) {
			toOutput = "[" + to_string(id_) + "]" + " " + "=" + " " + soon_printed->getDestination() + " "
				+ child1->getDestination();
			soon_printed->setReady(1);
			id_++;
		}
	}
	
}

Element* Compiler:: returnAss(string destination){
	for (int i = 0; i<all_operations.size(); i++){
		if (all_operations[i]->getDestination() == destination)
			return all_operations[i];
	}
}

void Compiler::compile(){
	string new_file_name = "glupi_pera.txt";
	/*
	unsigned int len_without_txt = new_file_name.length() - 4;
	new_file_name.resize(len_without_txt);
	new_file_name.insert(len_without_txt, ".imf");*/
	this->filename = new_file_name;
	fstream outputFile(new_file_name, ios::out);

	string toOutput;
	//ofstream outputFile;// an output file stream object!
	//outputFile.open("new_file_name");
							 // open the file named "input_da
	setRootsReady();
	
	
	while (this->roots_ready == 0) {
		for (int i = 0; i < all_operations.size(); i++) {
			compileOne(all_operations[i],toOutput);
			outputFile << toOutput << endl;
			toOutput.clear();
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

char Compiler::defineMotherOp(Element* mother){

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

Compiler::Compiler(const string& filepath1, const string& filepath2){
	Configuration::Instance()->read(filepath1);
	Program::Instance()->read(filepath2);
	this->filename = filepath2;
	int j = 0;
	map<int, string> help = Program::Instance()->getVarNameExp();
	for (auto i = help.begin(); i != help.end(); i++) {
		string* expres = &(i->second);
		
		 ExpressionTree* tree = new ExpressionTree();
		 tree->inToPost(expres, i->first);
		 tree->build();
		 this->forest_gump_.push_back(tree);
	}
	catchOperation();
	tieUp();
	compile();
}

