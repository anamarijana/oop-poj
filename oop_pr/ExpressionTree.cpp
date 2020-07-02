#include"ExpressionTree.h"
#include <stack>


ExpressionTree::ExpressionTree(string *expression) {
	//pravimo jednostavno binarno stablo od postorder izraza // simple compilation
	stack <Element*> s;
	Element* curr = 0;
	Operation* curr_op = 0;
	int children_number;
	string exp = *expression;
	
	string final_destination ;
	string destination;
	int constant;

	final_destination += exp[0];//za prvog ne pravimo node
	for (int i = 1; i < exp.size(); i++) {

		if (isdigit(exp[i])) {
			curr = new Constant(CONSTANT);
			s.push(curr);
			constant = exp[i];
			curr->setOutValue(constant);
			destination = exp[i];
			curr->setDestination(destination);
		}
		else if (isalpha(exp[i])) {
			curr = new Variable(VARIABLE);
			s.push(curr);
			destination = exp[i];
			curr->setDestination(destination);

		}
		else {//CONSTANT, VARIABLE, ADDITION, MULTIPLICATION, EXPONENTIATION, ASSIGNMENT 
			if (exp[i] == '+') {
				curr_op = new Addition(ADDITION);
				curr_op->setDuration(Configuration::returnInstance()->getAddTime());
			}
			else if (exp[i] == '*') {
				curr_op = new  Multiplication(MULTIPLICATION);
				curr_op->setDuration(Configuration::returnInstance()->getMultiTime());
			}
			else if (exp[i] == '^') {
				curr_op = new Exponentiation(EXPONENTIATION);
				curr_op->setDuration(Configuration::returnInstance()->getExpTime());
			}
			else {
				curr_op = new Assignment(ASSIGNMENT);
				curr_op->setDuration(Configuration::returnInstance()->getAssTime());

				
			}
			Element* child = 0;

			if (exp[i] == '=') children_number = 1;
			else children_number = 2;
			for (int j = 0; j < children_number; j++) {
				
				child = s.top();
				curr_op->setIn(child);
				s.pop();
				
			}
			s.push(curr_op);
		
			this->operations_.push_back(curr_op);
		}


	}

	this->root_ = s.top(); //na kraju na steku ostaje samo koreni cvor

	this->root_->setDestination(final_destination);
}
ExpressionTree::~ExpressionTree(){
}
//distributivnost
//samo operacije koje imaju asocijativnost +* smemo da grupisemo u narno stablo
//stepenovanje ne smemo da diramo
//distributivnost!! samo ako imamo zagrade??
void ExpressionTree::binaryToNary(){
	
	Element* child = 0;
	Element* grand_child1 = 0;
	Element* grand_child2 = 0;
	for (int i = 0; i<operations_.size();i++){
		if((operations_[i]->getType() != EXPONENTIATION) && (operations_[i]->getType() != ASSIGNMENT)){
			for (int j = 0; j<2;j++){
				child = operations_[i]->getIn()[j];
				if(operations_[i]->getType()== child->getType()){
					
					//cuvamo decu te iste operacije
					grand_child1 = child->getIn()[0];
					grand_child2 = child->getIn()[1];
					
					removeChild(grand_child1, grand_child2);

					operations_[i]->setIn(grand_child1);
					operations_[i]->setIn(grand_child2);

				}
			}
				
		
		}
	}
}

void ExpressionTree::removeChild(Element* GrandChild1, Element* GrandChild2){
	for (auto& pointer : operations_)
	{
		if ((pointer->getIn()[0]==GrandChild1)&& (pointer->getIn()[1] == GrandChild2))
		{
			delete pointer;
			pointer = nullptr;
		}
	}
	operations_.erase(std::remove(operations_.begin(), operations_.end(), nullptr), operations_.end());
	

}


void ExpressionTree::calculate(const string& filepath){
}

vector<Operation*> ExpressionTree::getOps(){
	return this->operations_;
}

Element* ExpressionTree::getRoot(){
	return this->root_;
}
