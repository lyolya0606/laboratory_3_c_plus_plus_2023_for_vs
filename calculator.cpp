/*
	 Calculator.cpp 
   Модуль, отвечающий за вычисление выражений.
   Парсер выражения подаётся в функцию evaluate_expression.
   Все промежуточные вычисления и результаты в формате float.
 
   Особые ситуации:
      В случае возникновения ошибок выбрасываются исключения типа 
    const char *
*/

#include "stdafx.h"
#include <cmath>
#include <string>

#include "calculator.h"
#include "name_table.h"

// Чтение минимально неделимого элемента выражения
float prim(Parser& parser) {
	parser.get_lexem();

	switch (parser.get_last().type) {
	case LT_Number: {
		const float val = parser.get_last().value;
		parser.get_lexem();
		return val;
	}
	case LT_Identifier: {
		std::string name(parser.get_last().name);
		parser.get_lexem();
		return NT.GetVariable(name);
	}
	case LT_Delimiter: switch (parser.get_last().delimiter) {
	case '-': return -prim(parser);
	case '(': {
		const float val = evaluate_expression(parser);
		if (parser.get_last().type == LT_Delimiter && parser.get_last().delimiter == ')') {
			parser.get_lexem();
			return val;
		}
		throw R"(')' expected)";
	}
	default: {
		throw "primary expected";
	}
	}
	default: {
		throw "primary expected";
	}
	}
}

// Обрабатка возведения в степень
float term_pow(Parser& parser) {
	float left = prim(parser);

	while (true) {
		if (parser.get_last().type == LT_Delimiter) {
			switch (parser.get_last().delimiter) {
			case '^': {
				left = pow(left, prim(parser));
			}
				break;
			default: return left;
			}
		} else {
			return left;
		}
	}
}

// Обрабатка умножения и деления
float term(Parser& parser) {
	float left = term_pow(parser);

	while (true) {
		if (parser.get_last().type == LT_Delimiter) {
			switch (parser.get_last().delimiter) {
			case '*': left *= term_pow(parser);
				break;
			case '/': {
				constexpr float precision = 1.0e-5f;
				const float d = term_pow(parser);
				if (fabs(d) > precision) {
					left /= d;
				} else {
					throw "Divide by zero";
				}
			}
				break;
			default: return left;
			}
		} else {
			return left;
		}
	}
}

// Обрабатка сложения и вычитания
float plus_minus(Parser& parser) {
	float left = term(parser);

	while (true) {
		if (parser.get_last().type == LT_Delimiter) {
			switch (parser.get_last().delimiter) {
			case '+': left += term(parser);
				break;
			case '-': left -= term(parser);
				break;
			default: return left;
			}
		} else {
			return left;
		}
	}
}


// Вычисление выражения
float evaluate_expression(Parser& parser) {
	float left = plus_minus(parser);

	while (true) {
		if (parser.get_last().type == LT_Delimiter) {
			switch (parser.get_last().delimiter) {
			case '<': left = left < plus_minus(parser);
				break;
			case '>': left = left > plus_minus(parser);
				break;
			case '=': left = left == plus_minus(parser);
				break;
			default: return left;
			}
		} else {
			return left;
		}
	}
}