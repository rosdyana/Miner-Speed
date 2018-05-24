#include "GameView.h"
#include "../Audio/Audio.h"
#include "BoardView.h"
#include "../Game.h"

#include <king/Engine.h>
#include <string>
#include <time.h>


namespace MinerSpeed
{

    const char	*GameView::WELCOME_TEXT		=	"Welcome To The Game! Click To Begin.";
    const char	*GameView::GAME_OVER_TEXT	=	"Game Over. Time Is Up.";
    const int	GameView::MAX_PLAYING_TIME	=	15;

    unsigned int	GameView::TIME_PASSED	=	0;

	bool isGameOver = false;
	bool isAlmostDone = false;

    unsigned int GameView::TIMER_CALLBACK(unsigned int interval, void *param)
    {
        GameView::TIME_PASSED += 1;
        return interval;
    }

    //********************************************************************************************************************************
    GameView::GameView()
        : View()
        , mBoardView(nullptr)
		, mAudio(nullptr)
        , mCurrentState(GameView::State::STATE_MIN)
        , mTime(0.0f)
        , mWelcomeTextPosition(0.0f, 0.0f)
        , mGameOverTextPosition(0.0f, 0.0f)
        , mPlayingTimeLabelPosition(0.0f, 0.0f)
        , myTimerID(NULL)
    {
    }

    //********************************************************************************************************************************
    GameView::~GameView()
    {
        if (myTimerID) {
            SDL_RemoveTimer(myTimerID);
        }
        delete mBoardView;
    }

    //********************************************************************************************************************************
    void GameView::Init(Game &game)
    {
        View::Init(game);
        mBoardView = new BoardView();
        assert(mBoardView != nullptr);
        mBoardView->Init(game, mPosition);
		mAudio = new Audio();
		assert(mAudio != nullptr);
    }

    //********************************************************************************************************************************
    void GameView::InitPosition()
    {
        int screenWidth = mEngine->GetWidth();
        int screenHeight = mEngine->GetHeight();
        int backgroundWidth = mEngine->GetTextureWidth(King::Engine::TEXTURE_BACKGROUND);
        int backgroundHeight = mEngine->GetTextureHeight(King::Engine::TEXTURE_BACKGROUND);
        float welcomeTextWidth = mEngine->CalculateStringWidth(GameView::WELCOME_TEXT);
        float gameOverTextWidth = mEngine->CalculateStringWidth(GameView::GAME_OVER_TEXT);
        mPosition = glm::vec2((screenWidth - backgroundWidth) / 2, 0.0f);
        mWelcomeTextPosition = glm::vec2((screenWidth - welcomeTextWidth) / 2, backgroundHeight / 2);
        mGameOverTextPosition = glm::vec2((screenWidth - gameOverTextWidth) / 2, backgroundHeight / 2);
        mPlayingTimeLabelPosition = glm::vec2(mPosition.x + 115, mPosition.y + 450);
    }

    //********************************************************************************************************************************
    void GameView::Start()
    {
        SetCurrentState(GameView::State::STATE_WELCOME);
        mBoardView->Start();
    }

    //********************************************************************************************************************************
    void GameView::Update()
    {

        mEngine->Render(King::Engine::TEXTURE_BACKGROUND, mPosition.x, mPosition.y);
        mBoardView->Update();

        switch (mCurrentState) {
            case GameView::State::STATE_WELCOME : {
                UpdateWelcome();
                break;
            }
            case GameView::State::STATE_PLAYING: {
                UpdatePlaying();
                break;
            }
            case GameView::State::STATE_GAME_OVER: {
                UpdateGameOver();
                break;
            }
            default : {
                assert(false);
            }
        }
    }

    //********************************************************************************************************************************
    void GameView::UpdateWelcome()
    {
        mEngine->Write(GameView::WELCOME_TEXT, mWelcomeTextPosition.x, mWelcomeTextPosition.y, 0.0f);

        if (mEngine->GetMouseButtonDown()) {
            mTime = 0.0f;
            mBoardView->setEnabled(true);
            GameView::TIME_PASSED = 0;
            myTimerID = SDL_AddTimer(1000, TIMER_CALLBACK, NULL);
            SetCurrentState(GameView::State::STATE_PLAYING);
        }
    }

    //********************************************************************************************************************************
    void GameView::UpdatePlaying()
    {
        if (GameView::TIME_PASSED <= GameView::MAX_PLAYING_TIME) {
            UpdatePlayingTimeLabel(GameView::MAX_PLAYING_TIME - GameView::TIME_PASSED);
        } else {
            SDL_RemoveTimer(myTimerID);
            myTimerID = NULL;
            mBoardView->setEnabled(false);
            SetCurrentState(GameView::State::STATE_GAME_OVER);
        }
    }

    //********************************************************************************************************************************
    void GameView::UpdateGameOver()
    {
        mEngine->Write(GameView::GAME_OVER_TEXT, mGameOverTextPosition.x, mGameOverTextPosition.y, 0.0f);
		if (!isGameOver) {
			mAudio->PlaySound(Audio::SoundFx::Explosion);
			mAudio->StopMusic();
			isGameOver = true;
		}
		
    }

    //********************************************************************************************************************************
    void GameView::SetCurrentState(State value)
    {
        mCurrentState = value;
    }

    //********************************************************************************************************************************
    void GameView::UpdatePlayingTimeLabel(const size_t delta)
    {
        std::string s = std::to_string(delta);
        char const *pChar = s.c_str();
        float labelWidth = mEngine->CalculateStringWidth(pChar);
        mEngine->Write(pChar, mPlayingTimeLabelPosition.x - labelWidth / 2, mPlayingTimeLabelPosition.y, 0.0f);
		if (delta <= 10) {
			if (!isAlmostDone) {
				mAudio->PlaySound(Audio::SoundFx::HurryUp);
				isAlmostDone = true;
			}
		}
    }
}
