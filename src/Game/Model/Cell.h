#pragma once

#include <glm/glm.hpp>

namespace MinerSpeed
{
    class Item;
}

namespace MinerSpeed
{

    class Cell
    {
    public:
        Cell();
        Cell(const glm::uvec2 position);
        Cell(const Cell &instance);
        Cell	&operator=(const Cell &r);
        void	Swap(Cell &r);
        virtual	~Cell();

    public:
        void		SetItem(MinerSpeed::Item &item);
        MinerSpeed::Item	&GetItem()					const;
        void		SetMarked(const bool value);
        const bool	IsMarked()					const;
        const		glm::uvec2 &GetPosition()	const;


    private:
        MinerSpeed::Item	*mItem;
        size_t		mColIndex;
        size_t		mRowIndex;
        bool		mIsMarked;
        glm::uvec2	mPosition;
    };
}
