#pragma once
class Timer {
  private:
	float timer = 0;
	float maxTime = 0;
	float second = 0;
	bool isActive = false;
	bool end = false;
	bool endMoment = false;
	bool isDekey = false;
  public:
	Timer();
	Timer(const float maxTime, const float second, 
		const bool isDekey = false);
	void Init();
	// 初めから
	void Reset();
	// タイマーセット
	void Set(const float timer, const float maxTime, 
		const float second, const bool isDekey = false);
	// スタート
	void Start();
	// スタート ＆ セット
	void Start(const float timer, const float maxTime, 
		const float second, const bool isDekey = false);
	// ストップ
	void Stop();
	// 終わり(強制)
	void End();
	// アップデート
	void Update(const bool loop = false);
	// ----- Getter ----- //
	// 動いているか
	bool IsActive() { return isActive; }
	// 終わっているか
	bool IsEnd() { return end; }
	// 終わった瞬間
	bool isEndMoment() { return endMoment; }
	// 減少版か
	bool IsDekey() { return isDekey; }
	// 最大時間
	float MaxTime() { return maxTime; }
	// 現在の時間
	float Current() { return timer; }
	// 残りの時間
	float Remain() { return maxTime - timer; }
};