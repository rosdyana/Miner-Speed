#pragma once

#include "View.h"

#include <sdl/SDL.h>

namespace MinerSpeed
{
    class BoardView;
    class Audio;
} // namespace MinerSpeed

namespace MinerSpeed
{

    class GameView : public View
    {

    public:
        enum State {
            STATE_MIN,
            STATE_WELCOME,
            STATE_PLAYING,
            STATE_GAME_OVER,
            STATE_MAX
        };

    private:
        static const char *WELCOME_TEXT;
        static const char *GAME_OVER_TEXT;
        static const char *TRY_AGAIN_TEXT;
        static const int MAX_PLAYING_TIME;

    private:
        static unsigned int TIME_PASSED;
        static unsigned int TIMER_CALLBACK(unsigned int interval, void *param);

    public:
        GameView();
        virtual ~GameView();

    public:
        virtual void Init(Game &game);
        virtual void InitPosition();
        virtual void Start();
        virtual void Update();

    private:
        void UpdateWelcome();
        void UpdatePlaying();
        void UpdateGameOver();

    private:
        void SetCurrentState(State value);
        void GameView::UpdatePlayingTimeLabel(const size_t delta);

    private:
        MinerSpeed::BoardView *mBoardView;
        MinerSpeed::Audio *mAudio;
        State mCurrentState;
        float mTime;
        float mDuration;
        glm::vec2 mWelcomeTextPosition;
        glm::vec2 mGameOverTextPosition;
        glm::vec2 mTryAgainTextPosition;
        glm::vec2 mPlayingTimeLabelPosition;
        SDL_TimerID myTimerID;
    };
} // namespace MinerSpeed
