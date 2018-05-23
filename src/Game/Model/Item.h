#pragma once

namespace MinerSpeed
{

    class Item
    {
    public:
        enum State {
            STATE_MIN,
            STATE_NEUTRAL,
            STATE_SELECTED,
            STATE_MAX
        };

        enum Color {
            COLOR_MIN,
            COLOR_BLUE,
            COLOR_GREEN,
            COLOR_PURPLE,
            COLOR_RED,
            COLOR_YELLOW,
            COLOR_MAX
        };

    public:
        Item();
        Item(const Item &instance);
        Item	&operator=(const Item &r);
        void	Swap(Item &r);
        virtual	~Item();

    public:
        void		SetColor(const Color color);
        const Color	GetColor()	const;
        State		GetState()	const;
        void		SetState(State state);

    private:
        Color	mColor;
        State	mState;
    };
}
