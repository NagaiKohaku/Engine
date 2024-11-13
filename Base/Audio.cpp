#include "Audio.h"

#include "fstream"

#include "cassert"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
Audio* Audio::GetInstance() {
	static Audio instance;
	return &instance;
}

///=====================================================/// 
/// 初期化
///=====================================================///
void Audio::Initialize() {

	HRESULT result;

	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

///=====================================================/// 
/// 終了処理
///=====================================================///
void Audio::Finalize() {

	xAudio2_.Reset();
}

///=====================================================/// 
/// 音声データの読み込み
///=====================================================///
SoundData Audio::SoundLoad(const char* fileName) {

	/// === ローカル変数 === ///

	//ファイル情報
	std::ifstream file;

	//Riffチャンクの先頭情報
	RiffHeader riff;

	//音声のフォーマット
	FormatChunk format = {};

	//Dataチャンクの先頭情報
	ChunkHeader data;

	//音声データ
	SoundData soundData = {};

	/// === ファイルを開く === ///

	//.wavファイルをバイナリモードで開く
	file.open(fileName, std::ios_base::binary);

	//ファイルが開けなかったらエラー
	assert(file.is_open());

	/// === ファイル識別子の確認 === ///

	//Riffチャンクの読み込み
	file.read((char*)&riff, sizeof(riff));

	//IDが"RIFF"となっているかをチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	//Waveファイルであるかのチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	/// === ファイル構造の読み込み === ///

	//Formatチャンクの先頭部分のみを読み込む
	file.read((char*)&format, sizeof(ChunkHeader));

	//IDが"fmt "となっているかをチェック
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	//Formatチャンクのサイズをチェック
	assert(format.chunk.size <= sizeof(format.fmt));

	//Formatチャンクの内容を読み込む
	file.read((char*)&format.fmt, format.chunk.size);

	/// === 音声情報の読み込み === ///

	//Dataチャンクの読み込み
	file.read((char*)&data, sizeof(data));

	//IDが"bext"となっていたら
	if (strncmp(data.id, "bext", 4) == 0) {

		//読み取り位置をBextチャンクのサイズ分進める
		file.seekg(data.size, std::ios_base::cur);

		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//IDが"junk"となっていたら
	if (strncmp(data.id, "junk", 4) == 0) {

		//読み取り位置をJunkチャンクのサイズ分進める
		file.seekg(data.size, std::ios_base::cur);

		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//IDが"JUNK"となっていたら
	if (strncmp(data.id, "JUNK", 4) == 0) {

		//読み取り位置をJunkチャンクのサイズ分進める
		file.seekg(data.size, std::ios_base::cur);

		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//IDが"LIST"となっていたら
	if (strncmp(data.id, "LIST", 4) == 0) {

		//読み取り位置をListチャンクのサイズ分進める
		file.seekg(data.size, std::ios_base::cur);

		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//Dataチャンクがなかった場合
	if (strncmp(data.id, "data", 4) != 0) {

		//エラーを出す
		assert(0);
	}

	//音声データのサイズ分のバッファ
	char* pBuffer = new char[data.size];

	//音声情報の読み込み
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();

	/// === 音声データの設定 === ///

	//波形フォーマットの設定
	soundData.wfex = format.fmt;

	//バッファアドレスの設定
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);

	//バッファサイズの設定
	soundData.bufferSize = data.size;

	return soundData;
}

///=====================================================/// 
/// 音声データの解放
///=====================================================///
void Audio::SoundUnLoad(SoundData* soundData) {

	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	//音声データの初期化
	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

///=====================================================/// 
/// 音声オブジェクトの生成
///=====================================================///
SoundObject Audio::CreateSoundObject(SoundData soundData, bool isLoop) {

	HRESULT result;

	//波形フォーマットをもとにSoundVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	SoundObject resultSource;

	resultSource.pSourceVoice = pSourceVoice;
	resultSource.buf = buf;
	resultSource.soundData = soundData;

	return resultSource;
}

///=====================================================/// 
/// 音声の再生
///=====================================================///
void Audio::StartSound(SoundObject object) {

	HRESULT result;

	if (isPlayed(object)) {

		StopSound(object);
	}

	result = object.pSourceVoice->SubmitSourceBuffer(&object.buf);
	result = object.pSourceVoice->Start();
}

///=====================================================/// 
/// 音声の停止
///=====================================================///
void Audio::StopSound(SoundObject object) {

	HRESULT result;

	xAudio2_->CommitChanges(object.pSourceVoice->Stop());

	result = object.pSourceVoice->FlushSourceBuffers();
}

///=====================================================/// 
/// 音声が流れているか
///=====================================================///
bool Audio::isPlayed(SoundObject object) {

	XAUDIO2_VOICE_STATE state;

	object.pSourceVoice->GetState(&state);

	if (state.BuffersQueued == 0) {
		return false;
	}

	return true;
}