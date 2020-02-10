//2018203060 이유진
#include <iostream>
#include <stack>
#include <cctype>
#include <cstring>
#include <sstream>//stringstream용
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

double postfix(istream& ins) { //infix를 postfix로 바꾸고 계산
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
		//왼쪽괄호 스택에 담기
		if (ins.peek() == '('|| ins.peek() == '{'|| ins.peek() == '[') {
			ins >> item;
			storage.push(item);
			
		}
		//숫자는 output string에
		else if (isdigit(ins.peek()) || ins.peek() == DECIMAL) {
			ins >> output;
			outputs.push_back(output);
			
		}
		//곱하기 연산자 처리 방법
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
		//나누기 연산자 처리방법
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
		// 더하기 뺴기 연산자 처리방법
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
		//닫는괄호')' 처리방법
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
			}//예외처리
			else if (storage.top() == '(') {
				storage.pop();
			}
			ins.ignore();
		}
		//닫는괄호'}' 처리방법
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
			}//예외처리
			if (storage.top() == '{') {
				storage.pop();
			}
			ins.ignore();
		}
		//닫는괄호']' 처리방법
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
			}//예외처리
			if (storage.top() == '[') {
				storage.pop();
			}
			ins.ignore();
		}

		
	}
	while (!storage.empty())//남은 연산자들 꺼내기 + 만약 괄호 남아있으면 오류
	{
		output = storage.top();
		if ((output == '(' )||( output == '{' )|| (output == '['))
		{
			cout << "Error!: unbalanced parentheses\n";
			return 0;
		}//예외처리
		else {
			outputs.push_back(blank);
			outputs.push_back(output);
			storage.pop();
		}
	}
	

	 stringstream inputss(outputs);// output 스트링에 담긴 postfix식 계산하기위해 스트림으로 바꾸기

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
				 cout << "Error!: divide by zero\n";//0으로 나눌때 예외처리				 
				 return 0;
			 }
		 }
		 else {
			 inputss.ignore();
		 }
		
	 }

	 cout << fixed;//소수점 세자리 표현
	 cout.precision(3);//소수점 세자리 표현
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
		return -1;//0으로 나눌때 예외처리를 위한 반환
	} numbers.push(operand1 / operand2); break;

	}
	operations.pop();
	return 0;
}

