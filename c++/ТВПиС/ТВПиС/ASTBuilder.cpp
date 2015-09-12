#include "stdafx.h"
#include "ASTBuilder.h"
#include "Exception.h"

ASTBuilder::ASTBuilder(char* sourceCode)
{
	Automat* automat = new Automat(AUTOMAT);
	FileReader* fileReader = new FileReader(sourceCode);

	try
	{
		tokens = automat->Work(fileReader->ReadSourceCode());
	}
	catch (Exceptions e)
	{
		throw new Exception("Ошибка конечного автомата", 0);
	}

	if (automat) delete automat;
	if (fileReader) delete fileReader;

	// Приведение списка лексем к новому виду.
	tokens = OldTokenToNewToken(tokens);

	//PrintList(tokens);

	// Расстановка приоритетов операций.
	Prioritize(tokens);

	astRoot = currentList = nullptr;
	currentScope = nullptr;
}

// Запуск выполения дерева.
void ASTBuilder::Run()
{
	if (astRoot)
		astRoot->Execute();
	else
		//throw Exceptions::NoTree;
		throw new Exception("Необходимо построить синтексное дерево до выполнения.", 0);
		
}

// Вывод списка лексем, результата работы конечного автомата.
void ASTBuilder::PrintList(List* tokens)
{
	char* stringToken[] = { "String", "SingleLineComment", "MultilineComment",
		"Digit", "Char", "Bracket", "ReservedWord", "Star", "CompOper", "LogicOper", "BiteOp",
		"Assignment", "SysFunction", "Separator", "UserType", "Space", "NewLine", "ReservedType",
		"Condition", "ApOp", "IncOrDec" };

	cout << "Type\t\tValue\t\tPriority\n";
	for (int i = 0; i < tokens->Count(); i++)
	{
	NewToken* token = (NewToken*)tokens->GetAccessor(i);

	cout << stringToken[token->type] << " ";
	cout << token->string << " ";
	cout << token->priority << " ";
	cout << token->value << " ";
	cout << token->lineIndex << endl;
	}
}

// Деструктор.
ASTBuilder::~ASTBuilder()
{
	if (astRoot) delete astRoot;

	astRoot = currentList = nullptr;
	currentScope = nullptr;
}

// Построение дерева операций, которое затем выполняется командой Execute()
// и дерева областей видимости, хранящего константы, переменные, массивы.
void ASTBuilder::Build()
{
	try
	{
		astRoot = currentList = new TList();
		currentScope = currentList->GetScope();

		ParseMultiLine(0, tokens->Count());
	}
	catch (Exceptions e)
	{
		if (e == Exceptions::ArgumentOutOfRange)
			//throw Exceptions::UnexpectedEndOfFile;
			throw new Exception("UnexpectedEndOfFile: неожиданный конец файла.", 0);

		throw e;
	}
}

// Возвращает индекс парной для openingBracketIndex скобки.
// Если скобка не найдена или openingBracketIndex не является скобкой,
// выбрасывает Exceptions::MissingBracket.
int ASTBuilder::ClosingBracketIndex(int openingBracketIndex)
{
	char* openingBracket = ((NewToken*)(tokens->GetAccessor(openingBracketIndex)))->string;
	char* closingBracket;

	if (strcmp(openingBracket, "(") == 0)
	{
		closingBracket = ")";
	}
	else if (strcmp(openingBracket, "{") == 0)
	{
		closingBracket = "}";
	}
	else if (strcmp(openingBracket, "[") == 0)
	{
		closingBracket = "]";
	}
	else
	{
		//throw Exceptions::MissingBracket;
		throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (openingBracketIndex))->lineIndex);
	}

	for (int count = 0, i = openingBracketIndex; i < tokens->Count(); i++)
	{
		char* next = ((NewToken*)(tokens->GetAccessor(i)))->string;

		if (strcmp(next, closingBracket) == 0)
		{
			count--;

			if (count == 0)
			{
				return i;
			}
		}

		if (strcmp(next, openingBracket) == 0)
		{
			count++;
		}
	}

	//throw Exceptions::MissingBracket;
	throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (openingBracketIndex))->lineIndex);
}

// Возвращает true, если число является дробным, и false в противном случае.
bool ASTBuilder::IsFraction(char* s)
{
	bool e = false;
	bool x = false;

	for (int i = 0; i < strlen(s); i++)
	{
		if (s[i] == '.') return true;
		if (s[i] == 'e' || s[i] == 'E') e = true;
		if (s[i] == 'x' || s[i] == 'X') x = true;
	}

	if (e && !x) return true;
	return false;
}

// Возвращает true, если участок является выражением, содержащим
// арфметические, логические операции или системные функции,
// и false в противном случае.
bool ASTBuilder::IsAnExpression(int start, int end)
{
	for (int i = start; i <= end; i++)
	{
		NewToken* token = (NewToken*)tokens->GetAccessor(i);

		if (token->type == Automat::Bracket && strcmp(token->string, "[") == 0)
			i = ClosingBracketIndex(i);

		if (token->type == Automat::ApOp || token->type == Automat::CompOper || token->type == Automat::IncOrDec ||
			token->type == Automat::SysFunction || token->type == Automat::Assignment)
			return true;
	}

	return false;

}

// Конвертирует в double целые числа в различных системах и дробные числа.
double ASTBuilder::StrToDouble(char* s)
{
	return IsFraction(s) ? atof(s) : strtol(s, nullptr, 0);
}

// Находит индекс операции с минимальным приоритетом в списке tokens.
int ASTBuilder::FindOperationWithMinimalPriority(int startPosition, int endPosition)
{
	// 16 уровней приоритета операций от 1 до 16.
	int min = 17, result = -1;

	for (int i = startPosition; i <= endPosition; i++)
	{
		NewToken* l = (NewToken*)tokens->GetAccessor(i);

		if (isOperator(l->type))
		{
			// Квадратные скобки.
			if (!strcmp(l->string, "[") && min == 17)
			{
				int f = ClosingBracketIndex(i);

				if (f == endPosition)
					return i;
				else
					i = f;
			}

			// Присваивание.
			if (l->priority < min && l->priority == 2)
			{
				min = l->priority;
				result = i;
			}

			// Другие операции.
			if (l->priority <= min && l->priority != 2)
			{
				min = l->priority;
				result = i;
			}
		}

		// Смещение на закрывающую круглую скобку.
		if (l->type == Automat::Bracket && strcmp(l->string, "(") == 0)
				i = ClosingBracketIndex(i);
	}

	if (result == -1) 
		//throw Exceptions::InvalidOperation;
		throw new Exception("InvalidOperation: недопустимая операция.", ((NewToken*)tokens->GetAccessor (startPosition))->lineIndex);


	return result;
}

// Проставляет значения приоритета для операций в списке tokens
// от 1 до 16, где 16 - наивысший приоритет.
void ASTBuilder::Prioritize(List* tokenList)
{
	NewToken* current;
	NewToken* prev;

	for (int i = 0; i < tokenList->Count(); i++)
	{
		current = ((NewToken*)(tokenList->GetAccessor(i)));
		prev = NULL;
		if (i > 0)
		{
			prev = ((NewToken*)(tokenList->GetAccessor(i - 1)));
		}

		if (isOperator(current->type))
		{
			string s = "";
			s += current->string;

			// Запятая.
			if (s == ",")
			{
				current->priority = 1;
				continue;
			}

			// Присваивание.
			if (s == "=")
			{
				current->priority = 2;
				continue;
			}

			// Тернартая условная операция (отсутствует).
			if ((s == "?") || (s == ":"))
			{
				current->priority = 3;
				continue;
			}

			// Логическое ИЛИ.
			if (s == "||")
			{
				current->priority = 4;
				continue;
			}

			// Логическое И.
			if (s == "&&")
			{
				current->priority = 5;
				continue;
			}

			// Побитовое ИЛИ.
			if (s == "|")
			{
				current->priority = 6;
				continue;
			}

			// Побитовое исключающее ИЛИ.
			if (s == "^")
			{
				current->priority = 7;
				continue;
			}

			// Разыменование указателя (отсутствует), взятие адреса (отсутствует),
			// унарные плюс и минус, префиксные инкремент и декремент.
			if ((s == "*") || (s == "&") || (s == "+") || (s == "-") || (s == "++") || (s == "--"))
				current->priority = 15;

			// Бинарные плюс и минус.
			if (((s == "+") || (s == "-")) && ((prev->type == Automat::Token::Digit) || prev->type == Automat::Token::UserType
				|| prev->type == Automat::Token::IncOrDec || !strcmp(prev->string, ")") || !strcmp(prev->string, "]")))
			{
				current->priority = 12;
				continue;
			}

			// Побитовое И.
			if ((s == "&") && (prev->type == Automat::Token::UserType))
			{
				current->priority = 8;
				continue;
			}

			// Равенство и неравенство.
			if ((s == "==") || (s == "!="))
			{
				current->priority = 9;
				continue;
			}

			// Меньше, больше, меньше или равно, больше или равно.
			if ((s == "<") || (s == ">") || (s == "<=") || (s == ">="))
			{
				current->priority = 10;
				continue;
			}
				
			// Умножение, деление, получение остатка от деления.
			if (((s == "*") || (s == "/") || (s == "%")) && ((prev->type == Automat::Token::Digit)
				|| (prev->type == Automat::Token::UserType) || !strcmp(prev->string, "]") || !strcmp(prev->string, ")")))
			{
				current->priority = 13;
				continue;
			}
			
			// Суффиксный инкремент и декремент, выбор элемента по указателю (отсутствует), взятие элемента массива.
			if (((s == "++") || (s == "--") || (s == "->") || (s == "[")) && (prev->type == Automat::Token::UserType) || (s == "]"))
				current->priority = 16;

			// Системные функции
			if (current->type == Automat::SysFunction)
				current->priority = 15;
		}
	}
}

// Поиск индекса токена token в списке tokens с позиции startPosition по endPosition.
// Если токен не найден, выбрасывается Exceptions::TokenNotFound.
int ASTBuilder::FindToken(int startPosition, int endPosition , char* token)
{
	for (int i = startPosition; i <= endPosition && i < tokens->Count(); i++)
	{
		if (!strcmp(((NewToken*)tokens->GetAccessor(i))->string, token))
			return i;
	}
	
	//throw Exceptions::TokenNotFound;
	throw new Exception("TokenNotFound: токен не найден.", ((NewToken*)tokens->GetAccessor (startPosition))->lineIndex);
}

// Возвращает true, если token является операцией и false в противном случае.
bool ASTBuilder::isOperator(Automat::Token token)
{
	switch (token)
	{
		case Automat::Token::ApOp:
		case Automat::Token::Assignment:
		case Automat::Token::BiteOp:
		case Automat::Token::CompOper:
		case Automat::Token::Condition:
		case Automat::Token::IncOrDec:
		case Automat::Token::LogicOper:
		case Automat::Token::ReservedWord:
		case Automat::Token::Separator:
		case Automat::Token::Star:
		case Automat::Token::SysFunction:
			return true;
		default:
			return false;
	}
}

// Конвертация токенов в новый формат NewToken с приоритетами
// с получением значения value для чисел.
List* ASTBuilder::OldTokenToNewToken(List* old)
{
	List* newTokenList = new List(sizeof(NewToken));
	int countLines = 1;

	for (int i = 0; i < old->Count(); i++)
	{
		Automat::ParseResult* oldToken = (Automat::ParseResult*)old->GetAccessor(i);

		// Отсев пробелов, переносов строк и комментариев.
		if (oldToken->token == Automat::Token::Space ||
			oldToken->token == Automat::Token::NewLine ||
			oldToken->token == Automat::Token::SingleLineComment ||
			oldToken->token == Automat::Token::MultilineComment)
		{
			if (oldToken->token == Automat::NewLine) countLines++;
			continue;
		}

		if (oldToken->token == Automat::ReservedWord)
		{
			if (strcmp(oldToken->string, "true") == 0)
			{
				oldToken->token = Automat::Digit;
				oldToken->string = "1";
			}
			if (strcmp(oldToken->string, "false") == 0)
			{
				oldToken->token = Automat::Digit;
				oldToken->string = "0";
			}
		}

		// Если число, то получение его значения, иначе 0.
		double value = 0;

		if (oldToken->token == Automat::Token::Digit)
		{
			value = StrToDouble(oldToken->string);
		}

		if (oldToken->token == Automat::Token::SysFunction)
		{
			// Для системных функций, возвращающих значение (всех, кроме input и output),
			// значение value = 1.
			if (strcmp(oldToken->string, "output") && strcmp(oldToken->string, "input"))
			{
				value = 1;
			}
		}

		
		// Приоритеты для операций расставляются позже методом Prioritize(List* tokenList).
		NewToken* newToken = new NewToken(oldToken->token, oldToken->string, value, 0, countLines);
		newTokenList->Add(newToken);
	}

	return newTokenList;
}

// Если единственный токен в строке, он может быть или константой или переменной.
// Возвращает (TConst : TNode) или (TVariable : TNode), для добавления в дерево TList* root.
// Если токен не является константой или переменной, Exceptions::InvalidOperation.
// Если переменная не найдена, Exceptions::VariableNameNotFound.
TNode* ASTBuilder::ParseSingleTokenLine(int index)
{
	NewToken* firstElement = (NewToken*)tokens->GetAccessor(index);

	// Константа.
	if (firstElement->type == Automat::Digit || firstElement->type == Automat::Char)
	{
		if (firstElement->type == Automat::Digit)
		{
			TValue* value = new TValue(firstElement->value, TypeList::Instance().GetTypeIndex("double"));
			return new TConst(value);
		}
		else
		{
			char symbol = firstElement->string[1];
			TValue* value = new TValue(symbol, TypeList::Instance().GetTypeIndex("char"));
			return new TConst(value);
		}
	}

	// Переменная или константа.
	if (firstElement->type == Automat::UserType)
	{
		TValueKeeper* var = currentScope->Find(firstElement->string);

		// Переменная не найдена.
		if (var == nullptr)
			//throw Exceptions::VariableNameNotFound;
			throw new Exception("VariableNameNotFound: необъявленная переменная или константа.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
		
		// Переменная хранит ссылку на массив значений.
		if (var->IsReference())
			//throw Exceptions::MissingBracket;
			throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

		if (var->IsConst())
		{
			// Константа.
			return new TConst(var->GetValue());
		}
		else
		{
			// Переменная.
			return new TVariable(var->GetValue());
		}
	}

	//throw Exceptions::InvalidCharacter;
	throw new Exception("InvalidCharacter: недопустимый символ.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
}

// Объявление и инициализация константы const.
// Значение константы должно быть явно указано и известно при компиляции:
// ect.: const int a = 5;
void ASTBuilder::ParseConstInitialization(int& index)
{
	// index	index + 1	index + 2	index + 3	index + 4	index + 5 = semicolon
	// const	type		name		=			value		;

	// index	index + 1	index + 2	index + 3	index + 4	index + 5	index + 6 = semicolon
	// const	type		name		=			- (+)		value		;

	int semicolon = FindToken(index, tokens->Count(), ";");
	int lenght = semicolon - index;

	if (lenght != 5 && lenght != 6)
		//throw Exceptions::ConstInitializationError;
		throw new Exception("ConstInitializationError: ошибка инициализации константы.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	NewToken* type = (NewToken*)tokens->GetAccessor(++index);	// Тип.

	if (TypeList::Instance().GetTypeIndex(type->string) == -1)
		//throw Exceptions::UnknownType;
		throw new Exception("UnknownType: неподдерживаемый тип данных.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	NewToken* name = (NewToken*)tokens->GetAccessor(++index);	// Имя константы.

	if (name->type != Automat::UserType)
		//throw Exceptions::ConstInitializationError;
		throw new Exception("ConstInitializationError: ошибка инициализации константы.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	if (currentScope->FindInCurrentScope(name->string) != nullptr)
		//throw Exceptions::RedefinitionVariable;
		throw new Exception("RedefinitionVariable: недопустимое переопределение переменной.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	NewToken* assignment = (NewToken*)tokens->GetAccessor(++index);

	if (assignment->type != Automat::Assignment)
		//throw Exceptions::UninitializedConstant;
		throw new Exception("UninitializedConstant: неинициализированная константа.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	int sign = 1; // Знак.

	if (lenght == 6)
	{
		NewToken* signToken = (NewToken*)tokens->GetAccessor(++index);

		if (strcmp(signToken->string, "-") == 0)
			sign = -1;
		else if (strcmp(signToken->string, "+") == 0)
			sign = 1;
		else
			//throw Exceptions::ConstInitializationError;
			throw new Exception("ConstInitializationError: ошибка инициализации константы.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
	}

	NewToken* value = (NewToken*)tokens->GetAccessor(++index);	// Значение.

	if (value->type == Automat::Digit)
	{
		currentScope->Push(new TValueKeeper(name->string, new TValue(sign * (value->value), TypeList::Instance().GetTypeIndex(type->string), 0), true));
		return;
	}

	//throw Exceptions::ConstInitializationError;
	throw new Exception("ConstInitializationError: ошибка инициализации константы.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
}

// Объявление (и инициализация) переменной или объявление статического массива.
// Неинициализированным пользователю переменным и элементам массивов присваивается значение 0.
// Если переменная инициализируется значением явно, соответствующая TBinaryOperation* для операции =
// добавляется в currentList для последующего вычисления.
void ASTBuilder::ParseInitialization(int& index)
{
	// 1. Переменная
	// index	index + 1	index + 2 = semicolon
	// type		name		;

	// 2. Переменная с присваиванием значения
	// index	index + 1	index + 2	index + 3	index + n = semicolon
	// type		name		=			value		;

	// 3. Массив
	// index	index + 1	index + 2	index + 3	index + 4	index + 5 = semicolon
	// type		name		[			size		]			;

	int semicolon = FindToken(index, tokens->Count(), ";");

	if (semicolon - index < 2)
		//throw Exceptions::VariableNameIsNotSpecified;
		throw new Exception("VariableNameIsNotSpecified: не указано имя переменной.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	// Тип переменной.
	NewToken* type = (NewToken*)tokens->GetAccessor(index);

	if (TypeList::Instance().GetTypeIndex(type->string) == -1)
		//throw Exceptions::UnknownType;
		throw new Exception("UnknownType: неподдерживаемый тип данных.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	// Имя переменной.
	NewToken* name = (NewToken*)tokens->GetAccessor(++index);

	if (name->type != Automat::UserType)
		//throw Exceptions::InvalidVariableName;
		throw new Exception("InvalidVariableName: недопустимое имя переменной.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	if (currentScope->FindInCurrentScope(name->string) != nullptr)
		//throw Exceptions::RedefinitionVariable;
		throw new Exception("RedefinitionVariable: недопустимое переопределение переменной.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	if (semicolon - index == 1)
	{
		// 1. Переменная без присваивания: добавляем в хеш со значением по умолчанию 0. +
		currentScope->Push(new TValueKeeper(name->string, new TValue(0, TypeList::Instance().GetTypeIndex(type->string), 0)));
		index = semicolon;
		return;
	}

	NewToken* bracketOrAssignment = (NewToken*)tokens->GetAccessor(++index);

	// 2. Переменная с присваиванием.
	if (bracketOrAssignment->type == Automat::Assignment)
	{
		TValue* tValue = new TValue(0, TypeList::Instance().GetTypeIndex(type->string), 0);

		// Добавляем переменную в хеш со значением по умолчанию 0.
		currentScope->Push(new TValueKeeper(name->string, tValue));

		// Присваивание.
		NewToken* assignment = (NewToken*)tokens->GetAccessor(index);
		if (assignment->type != Automat::Assignment)
			//throw Exceptions::InvalidOperation;
			throw new Exception("InvalidOperation: недопустимая операция.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

		// Символьный тип данных.
		if (!strcmp(type->string, "char"))
		{
			char symbol = ((NewToken*)tokens->GetAccessor(++index))->string[1];

			currentList->Push(new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex("="), new TVariable(currentScope->FindInCurrentScope(name->string)->GetValue()), new TConst(new TValue(symbol, TypeList::Instance().GetTypeIndex("char")))));
			return;
		}

		// Получение значения.
		TNode* value = ParseLine(++index, semicolon - 1); 

		currentList->Push(new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex("="), new TVariable(currentScope->FindInCurrentScope(name->string)->GetValue()), value));

		index = semicolon;
		return;
	}

	// 3. Массив.
	if (bracketOrAssignment->type == Automat::Bracket && semicolon - index == 3)
	{
		// Размер.
		NewToken* size = (NewToken*)tokens->GetAccessor(++index);
		int integerSize = 0;

		// Целое положительное числовое значение.
		if (size->type == Automat::Digit)
		{
			if (size->value <= 0 || this->IsFraction(size->string))
				//throw Exceptions::ArraySizeIsNotAnInteger;
				throw new Exception("ArraySizeIsNotAnInteger: размер массива должен быть целым положительным числом.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

			integerSize = (int)size->value;
		}
		// Константа, объявленная ранее.
		else if (size->type == Automat::UserType)
		{
			TValueKeeper* constSize = (currentScope->FindInCurrentScope(size->string));

			if (constSize == nullptr)
			{
				constSize = (currentScope->Find(size->string));

				if (constSize == nullptr)
				{
					//throw Exceptions::VariableNameNotFound;
					throw new Exception("VariableNameNotFound: необъявленная переменная или константа.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
				}
				else
				{
					if (constSize->IsConst())
					{
						if (constSize->GetValue()->GetType() == TypeList::Instance().GetTypeIndex("int"))
						{
							integerSize = (int)constSize->GetValue()->GetValue();

							if (integerSize <= 0)
								//throw Exceptions::ArraySizeIsNotAnInteger;
								throw new Exception("ArraySizeIsNotAnInteger: размер массива должен быть целым положительным числом.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
						}
						else
							//throw Exceptions::ArraySizeIsNotAnInteger;
							throw new Exception("ArraySizeIsNotAnInteger: размер массива должен быть целым положительным числом.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
					}
					else
						//throw Exceptions::ArraySizeIsNotAConstant;
						throw new Exception("ArraySizeIsNotAConstant: размер массива должен быть задан константой.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
				}
			}
			else
			{
				if (constSize->IsConst())
				{
					if (constSize->GetValue()->GetType() == TypeList::Instance().GetTypeIndex("int"))
					{
						integerSize = (int)constSize->GetValue()->GetValue();

						if (integerSize <= 0)
							//throw Exceptions::ArraySizeIsNotAnInteger;
							throw new Exception("ArraySizeIsNotAnInteger: размер массива должен быть целым положительным числом.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
					}
					else
						//throw Exceptions::ArraySizeIsNotAnInteger;
						throw new Exception("ArraySizeIsNotAnInteger: размер массива должен быть целым положительным числом.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
				}
				else
					//throw Exceptions::ArraySizeIsNotAConstant;
					throw new Exception("ArraySizeIsNotAConstant: размер массива должен быть задан константой.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
			}
		}

		// Закрывающая скобка.
		index++;
		if (index != ClosingBracketIndex(index - 2))
			//throw Exceptions::ArrayInitializationError;
			throw new Exception("ArrayInitializationError: ошибка объявления массива.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

		currentScope->Push(new TValueKeeper(name->string, new TValue(0, TypeList::Instance().GetTypeIndex(type->string), integerSize)));
		index = semicolon;
		return;
	}

	//throw Exceptions::InitializationError;
	throw new Exception("InitializationError: ошибка объявления или инициализации.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
}

// Конструкция if-else.
TNode* ASTBuilder::ParseIfElse(int& index)
{
	// if (condition)
	// {
	//		body;
	// }
	// else *
	// { *
	//		body; *
	// } *
	// * - optional

	// 1. Условие if.
	NewToken* openingParenthesis = (NewToken*)tokens->GetAccessor(++index);

	if (strcmp(openingParenthesis->string, "(") != 0)
		//throw Exceptions::MissingBracket;
		throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	int closingParenthesis = ClosingBracketIndex(index);

	TNode* condition = ParseLine(index, closingParenthesis);
	index = closingParenthesis;

	// 2. Тело then.
	NewToken* openingBrace = (NewToken*)tokens->GetAccessor(++index);
	if (strcmp(openingBrace->string, "{") != 0)
		//throw Exceptions::MissingBracket; 
		throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	int closingBrace = ClosingBracketIndex(index);

	// Создаем TList для тела then с соответствующей областью видимости Scope.
	// Текущую currentList сохраняем в tempList.
	// Меняем currentList и currentScope на TList и Scope тела цикла.
	TList* thenBranch = new TList(currentList);
	TList* tempList = currentList;
	currentList = thenBranch;
	currentScope = thenBranch->GetScope();

	// Парсим тело then.
	ParseMultiLine(index + 1, closingBrace - 1);

	// Восстанавливаем currentList и область видимости currentScope.
	currentList = tempList;
	currentScope = currentList->GetScope();

	index = closingBrace;

	// 3. Тело else.

	// Есть ли вообще код после тела then. @ Об этом, вероятно, везде стоит думать.
	if (index == tokens->Count() - 1)
	{
		return new TIf(condition, thenBranch, nullptr);
	}

	int indexBeforeElseBranch = index;

	NewToken* elseToken = (NewToken*)tokens->GetAccessor(++index);
	TList* elseBranch = nullptr;

	// Если есть else.
	if (elseToken && strcmp(elseToken->string, "else") == 0)
	{
		NewToken* openingBrace = (NewToken*)tokens->GetAccessor(++index);
		if (openingBrace->type != Automat::Bracket)
			//throw Exceptions::MissingBracket;
			throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

		int closingBrace = ClosingBracketIndex(index);

		// Аналогично then.
		elseBranch = new TList(currentList);
		tempList = currentList;
		currentList = elseBranch;
		currentScope = elseBranch->GetScope();

		ParseMultiLine(index + 1, closingBrace - 1);

		currentList = tempList;
		currentScope = currentList->GetScope();

		index = closingBrace;
	}

	if (elseBranch == nullptr) index = indexBeforeElseBranch;

	return new TIf(condition, thenBranch, elseBranch);
}

// Цикл while.
TNode* ASTBuilder::ParseWhile(int& index)
{
	// while (condition)
	// {
	//		body;
	// }

	// 1. Условие.
	NewToken* openingParenthesis = (NewToken*)tokens->GetAccessor(++index);

	if (strcmp(openingParenthesis->string, "(") != 0)
		//throw Exceptions::MissingBracket;
		throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
	int closingParenthesis = ClosingBracketIndex(index);

	TNode* condition = ParseLine(index, closingParenthesis);

	index = closingParenthesis;

	// 2. Тело цикла.
	NewToken* openingBrace = (NewToken*)tokens->GetAccessor(++index);
	if (strcmp(openingBrace->string, "{") != 0)
		//throw Exceptions::MissingBracket;
		throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
	int closingBrace = ClosingBracketIndex(index);

	TList* body = new TList(currentList);
	TList* tempList = currentList;
	currentList = body;
	currentScope = body->GetScope();

	ParseMultiLine(index + 1, closingBrace - 1);

	index = closingBrace;

	currentList = tempList;
	currentScope = tempList->GetScope();

	return new TWhile(condition, body);
}

// Имена переменных и обращения к элементам массивов.
TNode* ASTBuilder::ParseVariableName(int& index, TNodeType& type)
{
	// Название переменной.
	NewToken* variableName = (NewToken*)tokens->GetAccessor(index++);

	TValueKeeper* keeper = currentScope->Find(variableName->string);
	if (keeper == nullptr)
		//throw Exceptions::VariableNameNotFound;
		throw new Exception("VariableNameNotFound: необъявленная переменная или константа.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);


	// Переменная хранит ссылку на массив значений.
	// Для доступа к нужному элементу необходимо указание индекса элемента в квадратных скобках.
	if (keeper->IsReference())
	{
		//  Квадратные скобки.
		NewToken* brackets = (NewToken*)tokens->GetAccessor(index);

		if (brackets->type == Automat::Bracket && strcmp(brackets->string, "[") == 0)
		{
			int closeBracket = ClosingBracketIndex(index);

			// Индекс элемента.
			TNode* elemIndex = ParseLine(index + 1, closeBracket - 1);

			index = closeBracket + 1;

			type = TNodeType::Array;
			return new TArray(keeper->GetValue(), elemIndex);
		}
		else
		{
			// Нет скобок после переменной, хранящей ссылку на массив.
			//throw Exceptions::IndexIsNotSpecified;
			throw new Exception("IndexIsNotSpecified: не указан индекс элемента массива.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
			
		}
	}
	else
	{
		// Константа. 
		if (keeper->IsConst())
		{
			type = TNodeType::Const;
			return new TConst(keeper->GetValue());
		}
		// Переменная.
		else
		{
			type = TNodeType::Variable;
			return new TVariable(keeper->GetValue());
		}
	}
}

// Для парсинга единственной строки (TNode*)
// 1. Единственный токен в строке (TConst* или TVariable*) или элемент массива (TVariable*)
// 2. Line не является выражением, содержащим арифметические, логические операции,
//    операции присваивания или системные функции.
// 3. Арифметические операции (TUnaryOperation* или TBinaryOperation*)
// 4. Операции сравнение (TBinaryOperation*)
// 5. Инкремент и декремент
// 6. Системные функции, возвращающие значение (TUnaryOperation*)
// 7. Присваивание
// 8. Логические операции
TNode* ASTBuilder::ParseLine(int start, int end)
{
	NewToken* token = (NewToken*)tokens->GetAccessor(start);

	// Убираем окаймляющие скобки.
	NewToken* i = token;
	while (i->type == Automat::Token::Bracket && ClosingBracketIndex(start) == end)
	{
		start++;
		end--;

		i = (NewToken*)tokens->GetAccessor(start);
	}

	// 1. Единственный токен в строке.
	if (end == start)
	{
		return ParseSingleTokenLine(start);
	}

	// 2. Line не является выражением, содержащим арифметические, логические операции,
	// операции присваивания или системные функции.
	if (IsAnExpression(start, end) == false)
	{
		TNodeType type; // Не нужен, строго говоря. @
		return ParseVariableName(start, type);
	}

	// Поиск операции с минимальным приоритетом.
	int pos = FindOperationWithMinimalPriority(start, end);
	NewToken* element = (NewToken*)tokens->GetAccessor(pos);

	// 3. Арифметические операции.
	if (element->type == Automat::ApOp)
	{
		// Унарные операции.
		if (element->priority == 15)
		{
			return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(pos + 1, end));
		}

		return new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(start, pos - 1), ParseLine(pos + 1, end));
	}

	// 4. Операторы сравнения.
	if (element->type == Automat::CompOper)
	{
		return new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(start, pos - 1), ParseLine(pos + 1, end));
	}

	// 5. Инкремент и декремент.
	if (element->type == Automat::IncOrDec)
	{
		// Инкремент.
		if (!strcmp(element->string, "++"))
		{
			if (((NewToken*)tokens->GetAccessor(pos + 1))->type == Automat::UserType)
			{
				// Префиксный.
				return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex("++о"), ParseLine(pos + 1, end));
			}

			// Суффиксный.
			return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex("о++"), ParseLine(start, pos - 1));
		}
		if (!strcmp(element->string, "--"))
		{
			if (((NewToken*)tokens->GetAccessor(pos + 1))->type == Automat::UserType)
			{
				// Префиксный.
				return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex("--о"), ParseLine(pos + 1, end));
			}

			// Суффиксный.
			return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex("о--"), ParseLine(start, pos - 1));
		}

		//throw Exceptions::UnknownOperation;
		throw new Exception("UnknownOperation: неподдерживаемая операция.", ((NewToken*)tokens->GetAccessor (start))->lineIndex);
	}

	// 6. Системные функции, возвращающие значение.
	if (element->type == Automat::SysFunction && element->value)
	{
		NewToken* bracket = (NewToken*)tokens->GetAccessor(++pos);

		if (bracket->type != Automat::Bracket)
			//throw Exceptions::MissingBracket;
			throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (pos))->lineIndex);

		int closeBracket = ClosingBracketIndex(pos);

		// Не указаны аргументы функции.
		if (closeBracket - pos == 1)
			//throw Exceptions::MissingArguments;
			throw new Exception("MissingArguments: пропущены аргументы при вызове функции.", ((NewToken*)tokens->GetAccessor (pos))->lineIndex);

		if (UnaryOperationList::Instance().IsOperation(element->string))
		{
			TNode* argument = ParseLine(pos, closeBracket);
			TUnaryOperation* unaryOperation = new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(element->string), argument);
			return unaryOperation;
		}

		if (BinaryOperationList::Instance().IsOperation(element->string))
		{
			int separatorIndex = -1;

			for (int i = pos; i < closeBracket; i++)
			{
				NewToken* separator = (NewToken*)tokens->GetAccessor(i);

				if (separator->type == Automat::Separator && strcmp(separator->string, ",") == 0)
				{
					separatorIndex = i;
					break;
				}
			}

			// Не указаны все аргументы функции.
			if (separatorIndex == -1)
				//throw Exceptions::MissingArguments;
				throw new Exception("MissingArguments: пропущены аргументы при вызове функции.", ((NewToken*)tokens->GetAccessor (pos))->lineIndex);

			TNode* first = ParseLine(pos + 1, separatorIndex - 1);
			TNode* second = ParseLine(separatorIndex + 1, closeBracket - 1);

			TBinaryOperation* binaryOperation = new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex(element->string), first, second);
			return binaryOperation;
		}

		//throw Exceptions::SystemFunctionIsNotSupported;
		throw new Exception("SystemFunctionIsNotSupported: неподдерживаемая системная функция.", ((NewToken*)tokens->GetAccessor (start))->lineIndex);
	}

	// 7. Присваивание
	// a = <expression>;
	// a [<index>] = <expression>;
	if (element->type == Automat::Assignment)
	{
		TNodeType resultType;
		TNode* left = ParseVariableName(start, resultType);
		TNode* right = ParseLine(pos + 1, end);

		// Присваивание значения константе запрещено.
		if (resultType == TNodeType::Const)
			//throw Exceptions::ConstantReinitialization;
			throw new Exception("ConstantReinitialization: изменение значения константы невозможно.", ((NewToken*)tokens->GetAccessor (start))->lineIndex);

		return new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex("="), left, right);
	}

	//8. Логические операции
	if (element->type == Automat::LogicOper)
	{
		if (strcmp(element->string, "!"))
		{
			return new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(start, pos - 1), ParseLine(pos + 1, end));
		}
		else
		{
			NewToken* bracket = (NewToken*)tokens->GetAccessor(++pos);

			if (bracket->type != Automat::Bracket)
				//throw Exceptions::MissingBracket;
				throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (pos))->lineIndex);

			int closeBracket = ClosingBracketIndex(pos);
			return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(pos + 1, closeBracket - 1));
		}
	}

	//throw Exceptions::InvalidOperation;
	throw new Exception("InvalidOperation: недопустимая операция.", ((NewToken*)tokens->GetAccessor (start))->lineIndex);
}

// Для парсинга программы и тела конструкций if и циклов.
// Не возвращает значение, все TNode* цепляются к currentList.
// 1. Объявление (и инициализация) констант, переменных, массивов.
// 2. Циклы if (TIfElse*) и while (TWhile)
// 3. Функции input() и output().
// 4. Обращение и присваивание значения переменным и элементам массивов.
void ASTBuilder::ParseMultiLine(int start, int end)
{
	for (int i = start; i < end && i < tokens->Count(); i++)
	{
		NewToken* token = (NewToken*)tokens->GetAccessor(i);

		// Пропуск точки с запятой.
		if (token->type == Automat::Separator)
			continue;

		// 1. Объявление константы.
		if (strcmp(token->string, "const") == 0)
		{
			// const int a = <expression>;
			ParseConstInitialization(i);
			continue;
		}

		// 2. Объявление переменных и массивов.
		// тип имя = <expression>;
		// тип имя[размер];
		if (token->type == Automat::Token::ReservedType)
		{
			ParseInitialization(i);
			continue;
		}

		// 3. Конструкции if и while.
		if (token->type == Automat::Token::Condition)
		{
			if (strcmp(token->string, "if") == 0)
			{
				currentList->Push(ParseIfElse(i));
				continue;
			}
			else if (strcmp(token->string, "while") == 0)
			{
				currentList->Push(ParseWhile(i));
				continue;
			}
			else
			{
				//throw Exceptions::InvalidOperation;
				throw new Exception("InvalidOperation: недопустимая операция.", ((NewToken*)tokens->GetAccessor (i))->lineIndex);

			}
		}

		// 4. Системные функции c одним аргументом input и output, не возвращающие значение.
		if (token->type == Automat::Token::SysFunction && token->value == 0)
		{
			// Аргумент функции в круглых скобках.
			NewToken* bracket = (NewToken*)tokens->GetAccessor(++i);

			if (bracket->type != Automat::Token::Bracket)
				//throw Exceptions::MissingBracket;
				throw new Exception("MissingBracket: пропущена скобка.", ((NewToken*)tokens->GetAccessor (i))->lineIndex);

			int closeBracketIndex = ClosingBracketIndex(i);

			// Аргументы не указаны.
			if (closeBracketIndex - i == 1)
				//throw Exceptions::MissingArguments;
				throw new Exception("MissingArguments: пропущены аргументы при вызове функции.", ((NewToken*)tokens->GetAccessor (i))->lineIndex);

			i++;

			NewToken* digitOrChar = (NewToken*)tokens->GetAccessor(i);

			if (digitOrChar->type == Automat::UserType)
			{
				TNode* argument = ParseLine(i, closeBracketIndex - 1);

				// Вывод.
				if (!strcmp(token->string, "output"))
				{
					currentList->Push(new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(token->string), argument));
				}
				// Ввод.
				else
				{
					currentList->Push(new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex("="), argument, new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(token->string), argument)));
				}
			}
			else if (digitOrChar->type == Automat::String)
			{
				// Создание символьного массива.
				int stringLenght = strlen(digitOrChar->string) - 2 + 1;

				TValue* tValueString = new TValue(0, TypeList::Instance().GetTypeIndex("char"), stringLenght);
				
				double* doubleString = new double[stringLenght];
				for (int i = 0, j = 1; i < stringLenght - 1; i++, j++)
				{
					doubleString[i] = digitOrChar->string[j];
				}
				doubleString[stringLenght] = '\0';

				tValueString->SetPointer(doubleString);

				// Имена переменных не обязаны быть уникальными, так как они тут же выводятся. @
				currentScope->Push(new TValueKeeper("@string", tValueString));
				
				currentList->Push(new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(token->string), new TVariable(tValueString)));
			}
			else
			{
				TNode* argument = ParseLine(i, closeBracketIndex - 1);
				currentList->Push(new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(token->string), argument));
			}

			i = closeBracketIndex;

			continue;
		}

		// 5. Обращение к переменной и массивам
		// a = <expression>;
		// a [i] = <expression>; 
		int semicolon = FindToken(i, tokens->Count(), ";");
		currentList->Push(ParseLine(i, semicolon - 1));
		i = semicolon;
	}
}