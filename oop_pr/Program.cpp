#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#include <cctype>
#include "Program.h"

using namespace std;
Program::Program(string inputFile) {
	// Otvaramo fajl u 'in' rezimu rada (samo citanje)
	fstream inputFile(filename, ios::in);


	// Metoda 'peek()' dohvata sledeci karakter koji bi se procitao ali ga ne cita
	int i = 0;
	while (inputFile.peek() != EOF) {
		

		char var;
		var << inputFile;
		variables[i++] = var;
		char buffer;
		buffer<<inputFile;
		
		//sadrzi sve posle znaka = 
		j = 0;
		while (inputFile.peek() != '\n') {
			char c;
			c << inputFile;
			this->var_exp[i][j++] = c;
		}
		buffer << inputFile; // da bi pokupio znak za novi red


	}
	inputFile.close();
}
// izraz a^b^c cita se kao a^(b^c)
const string& Program::inToPost(string exp) { // da bism ose otarasili mogucih zagrada i lakse sastavili sintaksno stablo
	string postfix;
	stack <char> s; //na steku ce biti samo operacije i zagrade
	int j = 0;
	for (int i = 0; i < sizeof(exp); i++) {

		if (isdigit(exp[i]) | isalpha(exp[i])) //ako je operand stavi na stek
			s.push(exp[i]);
		else if (exp[i] == '(') //oznacava pocetak deo izraza izdvojenog zagradom
			s.push(exp[i]);
		else if (exp[i] == ')') {
			while (s.top() != '(') {
				postfix[j++] = s.top();
				s.pop();
			}
			s.pop();
		}

		if (exp[i] == '^') //jaca je od svih ostalih operacija, desno asocijativna ako naidje na ^ svakako se gura na stek
			s.push(exp[i]);
		
// empty() daje tacno ako je stek prazan

		if (exp[i] == '*') {
			if ((s.top() == '+') | (s.empty()) |( s.top()=='(')) //ako operator ima vecu prednost od onog na vrhu ili je na vrhu otovrena zagrada
				s.push(exp[i]);

			else { //ovde ulece ako je na topu ^ , * levo asocijativna operacija
				while ((s.top() != '+') | (!s.empty()) | (s.top() != '('))  
				{
					postfix[j++] = s.top();
					s.pop();
				}
				s.push(exp[i]);
			}
		}
		if (exp[i] == '+') { //on je slabiji od svih
			if ( (s.empty()) |( s.top()=='(')) 
				s.push(exp[i]);
			
			while ((!s.empty()) | (s.top() != '('))
			{
				postfix[j++] = s.top();
				s.pop();
			}
			s.push(exp[i]);


		}
	}

}

