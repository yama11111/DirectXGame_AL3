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
	// ���߂���
	void Reset();
	// �^�C�}�[�Z�b�g
	void Set(const float timer, const float maxTime, 
		const float second, const bool isDekey = false);
	// �X�^�[�g
	void Start();
	// �X�^�[�g �� �Z�b�g
	void Start(const float timer, const float maxTime, 
		const float second, const bool isDekey = false);
	// �X�g�b�v
	void Stop();
	// �I���(����)
	void End();
	// �A�b�v�f�[�g
	void Update(const bool loop = false);
	// ----- Getter ----- //
	// �����Ă��邩
	bool IsActive() { return isActive; }
	// �I����Ă��邩
	bool IsEnd() { return end; }
	// �I������u��
	bool isEndMoment() { return endMoment; }
	// �����ł�
	bool IsDekey() { return isDekey; }
	// �ő厞��
	float MaxTime() { return maxTime; }
	// ���݂̎���
	float Current() { return timer; }
	// �c��̎���
	float Remain() { return maxTime - timer; }
};