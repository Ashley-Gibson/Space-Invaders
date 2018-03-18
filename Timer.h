#pragma once

typedef unsigned int Uint32;

class Timer
{
private:
	Uint32 startTicks_;
	Uint32 pausedTicks_;
	bool started_;
	bool paused_;
public:
	Timer(void);
	~Timer(void);

	void Start();
	void Restart();
	void Pause();
	void Resume();
	void Stop();

	Uint32 GetElapsedTimeAsMilliseconds() const;
	float GetElapsedTimeAsSeconds() const;
	bool HasStarted() const;
	bool IsPaused() const;
};

