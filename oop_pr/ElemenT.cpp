
#include<Sched.h>
#include <math.h>
#include <algorithm> 
#include<Interfaces.h>

#include"ElemenT.h"
#include"ExMachina.h"
#include"Memory.h"


using namespace std;

Element::~Element() {

	int in_size = in_.size();
	for (int i = 0; i < in_size; i++)
		in_.pop_back();
	in_ready_.clear();
	destination_.clear();
	in_values_.clear();
}

vector<Element*> Element::getIn(){
	return this->in_;
}

double Element::getOutValue(){
	return this->out_value_;
}

Element_Type Element::getType(){
	return this->type_;
}

vector<bool> Element::getInReady() {
	return this->in_ready_;
}

double Element::getDuration() {
	return this->duration_;
}
int Element::getId(){
	return this->id_;
}
void Element::setReady(bool ready){
	this->ready_ = ready;
}
void Element::setDuration(double dur){
	this->duration_ = dur;
}
void Element::setOutValue(double value){
	this->out_value_ = value;
}
void Element::setIn(Element* child){
	this->in_.push_back(child);
}
void Element::setInByIndex(int index, Element* insert_el){
	int child_size = this->in_.size();
	for (int i = 0; i < child_size; i++) {
		if (i == index) {
			in_[i] = insert_el; // on sad treba da pokazuje na ono na sta pokazuje el
		}
	}

}
void Element::popIn(){
	in_.pop_back();
}

// 1 2 3 4 5 6 7 8 9
//std::rotate(myvector.begin(),myvector.begin()+3,myvector.end());
// 4 5 6 7 8 9 1 2 3*/

void Element::kickTheBaby(int index){

	
	auto it = in_.begin() + index;
	rotate(it, it + 1, in_.end());
	in_.pop_back();
	
}
void Element::kickTheBabys(vector<int>& index){
	sort(index.begin(), index.end(), greater<int>());
	for (auto id : index) {
		auto it = in_.begin() + id;
		rotate(it, it + 1, in_.end());
	}
	for (int i= 0; i< index.size();i++)
		in_.pop_back();


}
string Element::getDestination(){
	return this->destination_;
}
bool Element::getReady(){
	return this->ready_;
}
void Element::notify(ID id){
	this->ready_ = 1;
	ExMachina::Instance()->eventOccured(id,this->duration_, Scheduler::Instance()->getCurTime());
	in_from_childred_out();
	in_to_out();

	if ((this->destination_).size()==1) {
		ExMachina::Instance()->setMem(destination_, out_value_);

	}
	
}
void Element ::setDestination(string d) {
	this->destination_ = d;
}

void Operation::in_from_childred_out() {
	for (int i = 0; i < this->in_.size(); i++) {
		in_values_.push_back(in_[i]->getOutValue());
	}
}


void Addition::in_to_out() {

	this->out_value_ = this->in_values_[0] + this->in_values_[1]; // kada budemo vrsili operaciju imacemo binarno stablo
}


void Multiplication::in_to_out() {

	this->out_value_ = in_values_[0] * in_values_[1];

}


void Exponentiation::in_to_out() {
	this->out_value_ = pow(in_values_[0],in_values_[1]); // proveriti koji se stepenuje kojim
}

void Assignment::in_to_out() {
	this->out_value_ = in_values_.front(); // samo ima jedno dete koje prisvaja
}

void Subtraction::in_to_out(){
	this->out_value_ = this->in_values_[0] - this->in_values_[1]; // kada budemo vrsili operaciju imacemo binarno stablo
}
