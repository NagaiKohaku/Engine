#pragma once
#include "xaudio2.h"

#include "Windows.h"
#include "wrl.h"
#include "stdint.h"

#pragma comment(lib,"xaudio2.lib")

//音声データ
struct SoundData {
	WAVEFORMATEX wfex;       //波形フォーマット
	BYTE* pBuffer;           //バッファの先頭アドレス
	unsigned int bufferSize; //バッファサイズ
};

//音声オブジェクト
struct SoundObject {
	IXAudio2SourceVoice* pSourceVoice; //ソースボイス
	XAUDIO2_BUFFER buf{};              //波形データの設定
	SoundData soundData;               //音声データ
};

class Audio {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	static Audio* GetInstance();

	void Initialize();

	void Finalize();

	SoundData SoundLoad(const char* fileName);

	void SoundUnLoad(SoundData* soundData);

	SoundObject CreateSoundObject(SoundData soundData, bool isLoop);

	void StartSound(SoundObject object);

	void StopSound(SoundObject object);

	bool isPlayed(SoundObject object);

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//チャンクの先頭
	struct ChunkHeader {
		char id[4];   //ID
		int32_t size; //サイズ
	};

	//RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk; //チャンクの先頭
		char type[4];      //音声ファイルの識別子
	};

	//FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk; //チャンクの先頭
		WAVEFORMATEX fmt;  //音声のフォーマット
	};

	///-------------------------------------------///
	/// メンバ変数
	///-------------------------------------------///
private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;
};