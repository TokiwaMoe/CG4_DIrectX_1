#include "Audio.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

void Audio::Initialize()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2_1, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = XAudio2Create(&xAudio2_2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = xAudio2_1->CreateMasteringVoice(&masterVoice_1);
	result = xAudio2_2->CreateMasteringVoice(&masterVoice_2);
}

void Audio::PlaySEWave(const char* filename, float volume)
{
	HRESULT result;
	// ファイルストリーム
	std::ifstream file;
	// Waveファイルを開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));

	// Dataチャンクの読み込み
	Chunk data;
	file.read((char*)&data, sizeof(data));

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	WAVEFORMATEX wfex{};
	// 波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// 波形フォーマットを元にSourceVoiceの生成
	
	result = xAudio2_1->CreateSourceVoice(&pBgmSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;

	masterVoice_1->SetVolume(volume);

	// 波形データの再生
	result = pBgmSourceVoice->SubmitSourceBuffer(&buf);

	result = pBgmSourceVoice->Start();
}

void Audio::PlayBGMWave(const char* filename, float volume, bool LOOP)
{
	HRESULT result;
	// ファイルストリーム
	std::ifstream file;
	// Waveファイルを開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));

	// Dataチャンクの読み込み
	Chunk data;
	file.read((char*)&data, sizeof(data));

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	WAVEFORMATEX wfex{};
	// 波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// 波形フォーマットを元にSourceVoiceの生成
	result = xAudio2_2->CreateSourceVoice(&pSeSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	if (LOOP == true)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	masterVoice_2->SetVolume(volume);

	// 波形データの再生
	result = pSeSourceVoice->SubmitSourceBuffer(&buf);

	result = pSeSourceVoice->Start();

}
