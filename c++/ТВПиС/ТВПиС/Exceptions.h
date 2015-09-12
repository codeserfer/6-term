#pragma once

enum Exceptions
{
	CharError,						// 0
	StringError,
	FileReaderError,
	TypeNameLengthError,
	UnknownType,
	UnknownOperation,				// 5
	MissingBracket,
	ArgumentOutOfRange,
	InvalidOperation,
	NoTree,
	TokenNotFound,					//10
	VariableNameNotFound,
	VariableTypeNotFound,
	RedefinitionVariable,
	ConstInitializationError,
	VariableInitializationError,	//15
	ArrayInitializationError,
	InitializationError,
	InvalidCharacter,
	UninitializedConstant,
	VariableNameIsNotSpecified,		// 20
	InvalidVariableName,
	ArraySizeIsNotAConstant,
	ArraySizeIsNotAnInteger,
	IndexIsNotSpecified,
	ArrayIndexIsNotAnInteger,		// 25
	ConstantReinitialization,
	SystemFunctionIsNotSupported,
	MissingArguments,
	UnexpectedEndOfFile,
};