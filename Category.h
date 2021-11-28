//
// Created by Anton on 28/11/2021.
//

#ifndef PVPARENA_CATEGORY_H
#define PVPARENA_CATEGORY_H

namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1,
		Player = 1 << 1,
	};
}

#endif //PVPARENA_CATEGORY_H
