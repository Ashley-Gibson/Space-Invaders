#include "Timer.h"
#include "SDL.h"

Timer::Timer(void)
	: startTicks_(0), pausedTicks_(0), started_(false), paused_(false)
{
}

Timer::~Timer(void)
{
}

void Timer::Start()
{
	if (!started_) {
		started_ = true;
		paused_ = false;
		startTicks_ = SDL_GetTicks();
	}
}

void Timer::Restart()
{
	started_ = true;
	paused_ = false;
	startTicks_ = SDL_GetTicks();
	pausedTicks_ = 0;
}

void Timer::Pause()
{
	if (started_ & !paused_) {
		paused_ = true;
		pausedTicks_ = SDL_GetTicks() - startTicks_;
	}
}

void Timer::Resume()
{
	if (paused_) {
		paused_ = false;
		startTicks_ = SDL_GetTicks() - pausedTicks_;
		pausedTicks_ = 0;
	}
}

void Timer::Stop()
{
	if (started_) {
		started_ = false;
		paused_ = false;
		startTicks_ = 0;
		pausedTicks_ = 0;
	}
}

Uint32 Timer::GetElapsedTimeAsMilliseconds() const
{
	if (paused_) {
		return pausedTicks_;
	}
	return SDL_GetTicks() - startTicks_;
}

float Timer::GetElapsedTimeAsSeconds() const
{
	if (paused_) {
		return pausedTicks_ / 1000.f;
	}
	return (SDL_GetTicks() - startTicks_) / 1000.f;
}

bool Timer::HasStarted() const
{
	return started_;
}

bool Timer::IsPaused() const
{
	return paused_;
}