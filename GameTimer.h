#pragma once
class GameTimer final
{
public:
	GameTimer(void);
	~GameTimer(void);

	float GetElapsedSeconds() const { return m_ElapsedSeconds; }
	float GetTotalSeconds() const { return m_TotalSeconds; }
	int GetFPS() const { return m_FPS; }

	void Reset();
	void Start();
	void Stop();
	void Update();

private:

	float m_ElapsedSeconds;
	float m_TotalSeconds;
	float m_SecondsPerCount;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	bool m_IsStopped;

	int m_FPS;
	float m_FpsTimer;
	int m_FpsCount;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameTimer(const GameTimer& t);
	GameTimer& operator=(const GameTimer& t);
};

