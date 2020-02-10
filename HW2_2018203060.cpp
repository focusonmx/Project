//2018203060 ������
#include <iostream>
#include <stack>
#include <cctype>
#include <cstring>
#include <sstream>//stringstream��
#include <cstdlib>

using namespace std;

double postfix(istream& ins);
int evaluate_stack(stack<double>& numbers, stack<char>& operations);


int main(void) {
	
	string inputs;
	while (cin >> inputs && inputs != "EOI")
	{		
		stringstream ss(inputs);
		postfix(ss);
	}
}

double postfix(istream& ins) { //infix�� postfix�� �ٲٰ� ���
	string expression;
	stack<char> storage;
	char item = NULL;
	char output=NULL;
	string outputs;
	stack <double> numbers;
	stack <char> operations;
	double number;
	char symbol;
	char v=NULL;
	char blank = ' ';
	const char DECIMAL = '.';
	
	while (ins&&ins.peek() != '\n')
	{
		//���ʰ�ȣ ���ÿ� ���
		if (ins.peek() == '('|| ins.peek() == '{'|| ins.peek() == '[') {
			ins >> item;
			storage.push(item);
			
		}
		//���ڴ� output string��
		else if (isdigit(ins.peek()) || ins.peek() == DECIMAL) {
			ins >> output;
			outputs.push_back(output);
			
		}
		//���ϱ� ������ ó�� ���
		else if (strchr("*", ins.peek()) != NULL) {
			if (!storage.empty() && (storage.top() != '(' && storage.top() != '[' 
				&& storage.top() != '{') && (storage.top() != '+'&&storage.top() != '-'))
			{
				outputs.push_back(blank);
				output = storage.top();
				outputs.push_back(output);
				storage.pop();
				ins >> item;
				storage.push(item);
			}
			else
			{
				outputs.push_back(blank);
				ins >> item;
				storage.push(item);
			}
		}
		//������ ������ ó�����
		else if (strchr("/", ins.peek()) != NULL) {
			if (!storage.empty()  && (storage.top() != '(' && storage.top() != '['
				&& storage.top() != '{') && (storage.top() != '+'&&storage.top() != '-'))
			{
				outputs.push_back(blank);
				output = storage.top();
				outputs.push_back(output);
				storage.pop();
				ins >> item;
				
				storage.push(item);

			}
			else
			{
				outputs.push_back(blank);
				ins >> item;
				
				storage.push(item);
			}
			
		}
		// ���ϱ� ���� ������ ó�����
		else if (strchr("+-", ins.peek()) != NULL) {
				if (storage.empty() != 1 && (storage.top() != '(' && storage.top() != '[' && storage.top() != '{'))
				{
					outputs.push_back(blank);
					output = storage.top();
					outputs.push_back(output);
					storage.pop();
					ins >> item;
				
					storage.push(item);
				}
				else
				{
					outputs.push_back(blank);
					ins >> item;
					
					storage.push(item);
				}

		}
		//�ݴ°�ȣ')' ó�����
		else if (ins.peek() == ')'){ 
			while ((!storage.empty()) && (storage.top() != '(')){
				outputs.push_back(blank);
				output = storage.top();
				outputs.push_back(output);
				storage.pop();
			}
			if (storage.empty())
			{
				cout << "Error!: unbalanced parentheses\n";
				return 0;
			}//����ó��
			else if (storage.top() == '(') {
				storage.pop();
			}
			ins.ignore();
		}
		//�ݴ°�ȣ'}' ó�����
		else if (ins.peek() == '}') {
			while ((!storage.empty()) && (storage.top() != '{')) {
				outputs.push_back(blank);
				output = storage.top();
				outputs.push_back(output);
				storage.pop();
			}
			if (storage.empty())
			{
				cout << "Error!: unbalanced parentheses\n";
				return 0;
			}//����ó��
			if (storage.top() == '{') {
				storage.pop();
			}
			ins.ignore();
		}
		//�ݴ°�ȣ']' ó�����
		else if (ins.peek() == ']') {
			while ((!storage.empty()) && (storage.top() != '[')) {
				outputs.push_back(blank);
				output = storage.top();
				outputs.push_back(output);
				storage.pop();
			}
			if (storage.empty())
			{
				cout << "Error!: unbalanced parentheses\n";
				return 0;
			}//����ó��
			if (storage.top() == '[') {
				storage.pop();
			}
			ins.ignore();
		}

		
	}
	while (!storage.empty())//���� �����ڵ� ������ + ���� ��ȣ ���������� ����
	{
		output = storage.top();
		if ((output == '(' )||( output == '{' )|| (output == '['))
		{
			cout << "Error!: unbalanced parentheses\n";
			return 0;
		}//����ó��
		else {
			outputs.push_back(blank);
			outputs.push_back(output);
			storage.pop();
		}
	}
	

	 stringstream inputss(outputs);// output ��Ʈ���� ��� postfix�� ����ϱ����� ��Ʈ������ �ٲٱ�

	 while (inputss)
	 {
		 
		 if (isdigit(inputss.peek()) || inputss.peek() == DECIMAL) {
			 inputss >> number;
			 numbers.push(number); 
		 }
		 else if (strchr("+-/*", inputss.peek()) != NULL) {
			 int v;
			 inputss >> symbol;
			 operations.push(symbol);
			 v=evaluate_stack(numbers, operations);
			 if(v==-1)
			 {
				 cout << "Error!: divide by zero\n";//0���� ������ ����ó��				 
				 return 0;
			 }
		 }
		 else {
			 inputss.ignore();
		 }
		
	 }

	 cout << fixed;//�Ҽ��� ���ڸ� ǥ��
	 cout.precision(3);//�Ҽ��� ���ڸ� ǥ��
	 cout << numbers.top() << endl;
	 return 0;
	 
	
}


int evaluate_stack(stack<double>& numbers, stack<char>& operations)
{
	double operand1, operand2;
	operand2 = numbers.top();
	numbers.pop();
	operand1 = numbers.top();
	numbers.pop();

	switch (operations.top()) {
	case '+':numbers.push(operand1 + operand2); break;
	case '-':numbers.push(operand1 - operand2); break;
	case '*':numbers.push(operand1 * operand2); break;
	case '/':if (operand2 == 0) {
		return -1;//0���� ������ ����ó���� ���� ��ȯ
	} numbers.push(operand1 / operand2); break;

	}
	operations.pop();
	return 0;
}

