#pragma once

struct Block
{
	bool	used;
	int		size;
	void*	offset;

	Block*	next;
	Block*	prev;
};