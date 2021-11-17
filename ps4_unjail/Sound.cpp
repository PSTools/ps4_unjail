//
//#include <stdio.h>
//#include <vectormath.h>
//#include <vector>
//#include <string>
//#include <assert.h>
////#include <sampleutil.h>
//#include <pthread.h>
#include <audioout.h>
//#include "UserService.h"
//#include "SoundUtil.h"


//
//
////*!
////* @~English
////* @brief This is output WAV data path.
////* @~Japanese
////* @brief 出力WAVデータのパスです。
////*/
//#define BGM_DATA	((char *)"/app0/audio_video/sound/wave/pad_spk.wav")
//
//size_t sceUserMainThreadStackSize = 512 * 1024;	//E 512KiB
//size_t sceLibcHeapSize = 32 * 1024 * 1024;
//
//
//
//class AudioOut
//{
//
//public:
//
//	/*!
//	* @~English
//	* @brief Thread state initial value.
//	* @~Japanese
//	* @brief スレッド状態値初期値。
//	*/
//	static const int32_t AUDIO_STATUS_NONE = 0;
//
//	/*!
//	* @~English
//	* @brief Audio output thread state processing end value.
//	* @~Japanese
//	* @brief 音声出力スレッド状態値処理終了値。
//	*/
//	static const int32_t AUDIO_STATUS_END = 1;
//
//	/*!
//	* @~English
//	* @brief Maximum channels number of output buffers.
//	* @details This is maximum channels number of output buffers.
//	* @~Japanese
//	* @brief 出力バッファ最大チャンネル数
//	* @details 出力バッファ最大チャンネル数です。
//	*/
//	const int32_t MULTI = 8;
//
//	/*!
//	* @~English
//	* @brief Output grain size. 
//	* @details This is output grain size. 
//	* @~Japanese
//	* @brief 出力粒度
//	* @details 出力粒度です。
//	*/
//	const int32_t USER_GRAIN = 256;
//
//	/*!
//	* @~English
//	* @brief Number of outputs. 
//	* @details This is number of outputs.
//	* @~Japanese
//	* @brief outputs出力数
//	* @details outputs出力数です。
//	*/
//	static const int32_t OUTPUTS_NUM = 4;
//
//	/*!
//	* @~English
//	* @brief User ID.
//	* @details This is User ID.
//	* 
//	* @~Japanese
//	* @brief ユーザID。
//	* @details ユーザIDです。
//	* 
//	*/
//	SceUserServiceUserId m_UserId[OUTPUTS_NUM];
//
//	/*!
//	* @~English
//	* @brief AudioOut virtual deivce port handle.
//	* @details This is AudioOut virtual deivce port handle.
//	* 
//	* @~Japanese
//	* @brief AudioOut仮想デバイスポートハンドル
//	* @details AudioOut仮想デバイスポートハンドルです。
//	* 
//	*/
//	int32_t	m_handle[OUTPUTS_NUM];
//
//	/*!
//	* @~English
//	* @brief Constructor 
//	* @details This is a constructor. 
//	* 
//	* @~Japanese
//	* @brief コンストラクタ 
//	* @details コンストラクタです。
//	*/
//	AudioOut(void)
//	{
//		m_thread_sound = 0;
//		m_dataSize = 0;
//		m_StateFlag = AUDIO_STATUS_NONE;
//	}
//
//	static void *_soundThread(void* arg)
//	{
//		AudioOut* audioOut = (AudioOut*)arg;
//		audioOut->soundThread();
//		return NULL;
//	}
//
//	/*!
//	* @~English
//	* @brief Thread processing to output a WAV file data.
//	* @details This is the thread processing to output a WAV file data.
//	* 
//	* @~Japanese
//	* @brief WAVファイルデータを出力するスレッド処理
//	* @details WAVファイルデータを出力するスレッド処理です。
//	* 
//	*/
//	void *soundThread(void)
//	{
//		int32_t  returnCode = 0;
//		int32_t  volume[MULTI];
//		int loginUserNumber = 0;
//		uint32_t outputPosition = 0;
//		int16_t  remainingAdjustBuffer[USER_GRAIN * MULTI];
//		SceUserServiceLoginUserIdList loginUserList;
//		SceUserServiceEvent event;
//
//		SceAudioOutOutputParam param[OUTPUTS_NUM];
//		SceAudioOutPortState portState;
//		int16_t  old_volume[OUTPUTS_NUM] = { -1, -1, -1, -1 };
//		char UserName[OUTPUTS_NUM][SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1];
//		SceUserServiceUserColor UserColor[OUTPUTS_NUM];
//
//		printf ( "\n---Start-SoundThread---\n" );
//
//		/*E Check the login user the current state. */
//		/*J 現状のログインユーザ状態を知る */
//		returnCode = sceUserServiceGetLoginUserIdList(&loginUserList);
//		if(returnCode != 0) {
//			goto term;
//		}
//		for ( int i = 0 ; i < OUTPUTS_NUM ; i++ ) {
//			if ( loginUserList.userId[i] != -1 ) {
//				m_UserId[loginUserNumber] = loginUserList.userId[i];
//				loginUserNumber++;
//			}
//			else {
//				break;
//			}
//		}
//
//		printf( "Initial Login User num:%d\n", loginUserNumber );
//
//		/*E Open the output port. */
//		/*J ポートを開く */
//		for ( int i = 0 ; i < loginUserNumber ; i++ ) {
//			if ( m_UserId[i] == 0 ) {
//				continue;
//			}
//			m_handle[i] = sceAudioOutOpen(m_UserId[i], 
//				SCE_AUDIO_OUT_PORT_TYPE_PADSPK, 
//				0, /* index */ 
//				USER_GRAIN, 
//				48000, 
//				SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO);
//			if ( m_handle[i] < 0 ) {
//				printf( "Error: sceAudioOutOpen() %0x\n", m_handle[i] );
//				goto term;
//			}
//
//			/*E Get login user name. */
//			/*J ログインユーザ名の取得 */
//			returnCode = sceUserServiceGetUserName( m_UserId[i], &UserName[i][0], 
//				SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1 ); 
//			if (returnCode != 0) { 
//				printf ( "Error: sceUserServiceGetUserName() 0x%08x\n", returnCode );
//				goto term;
//			} 
//			/*E Get user color. */
//			/*J ユーザカラーの取得 */
//			returnCode = sceUserServiceGetUserColor( m_UserId[i], &UserColor[i] ); 
//			if (returnCode != 0) { 
//				printf ( "Error: sceUserServiceGetUserColor() 0x%08x\n", returnCode );
//				goto term;
//			} 
//		}
//
//		//J  パッドスピーカ向けの音がメインからミックス出力される場合の
//		//J  音量バランスを予めチェックしておいてください
//		//J  もし、その場合の音量バランスが不満なら、sceAudioOutSetMixLevelPadSpk()で対処して下さい
//		//E  Please check volume balance beforehand in case a PADSPK sound goes to MAIN.
//		//E  If you are dissatisfied in the volume balance in that case, 
//		//E  please use sceAudioOutSetMixLevelPadSpk()
//
//		/*E Set volume value */
//		/*J ボリューム値の設定 */
//		for (int i = 0; i < SCE_AUDIO_OUT_CHANNEL_MAX; i ++) {
//			volume[i] = SCE_AUDIO_VOLUME_0dB;
//		}
//
//		/*E Set volume */
//		/*J ボリュームの設定 */
//		for ( int i = 0 ; i < loginUserNumber ; i++ ) {
//			if ( m_UserId[i] == 0 ) {
//				continue;
//			}
//			sceAudioOutSetVolume( m_handle[i], 
//				( SCE_AUDIO_VOLUME_FLAG_FL_CH | SCE_AUDIO_VOLUME_FLAG_FR_CH
//				| SCE_AUDIO_VOLUME_FLAG_CNT_CH | SCE_AUDIO_VOLUME_FLAG_LFE_CH 
//				| SCE_AUDIO_VOLUME_FLAG_RL_CH  | SCE_AUDIO_VOLUME_FLAG_RR_CH
//				| SCE_AUDIO_VOLUME_FLAG_BL_CH  | SCE_AUDIO_VOLUME_FLAG_BR_CH ), volume);
//		}
//
//		do
//		{
//			uint16_t *choiceOutputMemory;
//
//			/*E Getting of the logged-in user information. */
//			/*J ログインユーザ情報の取得 */
//			returnCode = sceUserServiceGetEvent(&event);
//			if ( returnCode < 0 ) {
//				if (returnCode != SCE_USER_SERVICE_ERROR_NO_EVENT) {
//					printf("User Service Event Error:0x%08x\n", returnCode);
//				}
//			}
//			else {
//				/*E log in */
//				/*J ログイン */
//				if ( event.eventType == 0 ) {
//					int VacantIndex = -1;
//					/*E Find out if there is vacant area. */
//					/*J 空きが無いか調べる */
//					for (int i = 0 ; i < OUTPUTS_NUM ; i++ ) {
//						/*E Vacant? */
//						/*J 空いている? */
//						if ( m_UserId[i] == 0 ) {
//							if ( VacantIndex == -1 ) {
//								/*E The store this position. */
//								/*J 空きあり(最初に見つけた場所を使う) */
//								m_UserId[i] = event.userId;
//								loginUserNumber++;
//								VacantIndex = i;
//							}
//						}
//						else {
//							/*E Check the double registration. */
//							/*J 二重登録をチェック。 */
//							if (event.userId == m_UserId[i] ) {
//								/*E Double registration. */
//								/*J 重複登録 */
//								break;
//							}
//						}
//					}
//
//					/*E Registration to Vacant Position. */
//					/*J 空きに登録 */
//					if ( VacantIndex != -1 ) {
//						m_handle[VacantIndex] = sceAudioOutOpen(m_UserId[VacantIndex], 
//							SCE_AUDIO_OUT_PORT_TYPE_PADSPK, 
//							0, /* index */ 
//							USER_GRAIN, 
//							48000, 
//							SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO);
//						if ( m_handle[VacantIndex] < 0 ) {
//							printf( "Error: sceAudioOutOpen() 0x%08x\n", m_handle[VacantIndex] );
//							goto term;
//						}
//						/*E Get login user name. */
//						/*J ログインユーザ名の取得 */
//						returnCode = sceUserServiceGetUserName( m_UserId[VacantIndex], &UserName[VacantIndex][0], 
//							SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1 ); 
//						if (returnCode != 0) { 
//							printf ( "Error: sceUserServiceGetUserName() 0x%08x\n", returnCode );
//							goto term;
//						} 
//						/*E Get user color. */
//						/*J ユーザカラーの取得 */
//						returnCode = sceUserServiceGetUserColor( m_UserId[VacantIndex], &UserColor[VacantIndex] ); 
//						if (returnCode != 0) { 
//							printf ( "Error: sceUserServiceGetUserColor() 0x%08x\n", returnCode );
//							goto term;
//						} 
//					}
//				}
//				/*E log out */
//				/*J ログアウト */
//				else if ( event.eventType == 1 ) {
//					/*E Check the registered */
//					/*J 登録済みを確認 */
//					for (int i = 0 ; i < OUTPUTS_NUM ; i++ ) {
//						if ( event.userId == m_UserId[i] ) {
//							//E Waiting final data output.
//							//J 最後のデータの出力待ち
//							param[i].handle = m_handle[i];
//							param[i].ptr = NULL;
//							returnCode = sceAudioOutOutputs(&param[0], loginUserNumber);
//							if ( returnCode < 0 ) {
//								printf("Error: sceAudioOutOutputs() 0x%08x\n", returnCode);
//								goto term;
//							}
//							//E Release output port.
//							//J ポートの解放
//							returnCode = sceAudioOutClose(m_handle[i]);
//							if ( returnCode < 0 ) {
//								printf("Error: sceAudioOutClose() 0x%08x\n", returnCode);
//							}
//							/*E Deregistration. */
//							/*J ユーザの削除 */
//							m_UserId[i] = 0;
//							m_handle[i] = 0;
//							old_volume[i] = -1;
//							memset( &UserName[i][0], 0, sizeof(SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1) );
//							loginUserNumber--;
//							break;
//						}
//					}
//				}
//			}
//
//			/*E Get port status for print pad speaker volume. */
//			/*J ボリュームは更新された ? */
//			for (int i = 0 ; i < OUTPUTS_NUM ; i++ ) {
//				if ( m_UserId[i] != 0 ) {
//					/*E Get port status. */
//					/*J ポートステータスの取得 */
//					returnCode = sceAudioOutGetPortState( m_handle[i], &portState );
//					if ( returnCode < 0 ) {
//						printf ( "Error: sceAudioOutGetPortState() 0x%08x\n", returnCode );
//						goto term;
//					}
//
//					/*E Volume has been updated ? */
//					/*J ボリュームは更新された ? */
//					if ( old_volume[i] != portState.volume ) {
//						/*E debug print output to observe pad speaker volume. */
//						/*J ポートステータス中のパッドスピーカのボリュームをデバッグ出力する */
//						printf("[UserName:%s(Color:%s)] pad speaker volume change to %d\n", 
//							&UserName[i][0], 
//							UserColor[i] == SCE_USER_SERVICE_USER_COLOR_BLUE  ? "BLUE"  :
//							UserColor[i] == SCE_USER_SERVICE_USER_COLOR_RED   ? "RED"   :
//							UserColor[i] == SCE_USER_SERVICE_USER_COLOR_GREEN ? "GREEN" :
//							UserColor[i] == SCE_USER_SERVICE_USER_COLOR_PINK  ? "PINK"  : "Unknown",
//							portState.volume );
//						old_volume[i] = portState.volume;
//					}
//				}
//			}
//
//			/*E The remaining amount of data is less than the data output unit? */
//			/*J 残りのデータ量がデータ出力単位に満たない? */
//			if ( ( m_dataSize - outputPosition ) < ( USER_GRAIN * m_Channels * sizeof(uint16_t) ) ) {
//				/*E Copy to buffer the rest of the data. */
//				/*J 残りのデータをバッファへコピーする */
//				memcpy( remainingAdjustBuffer, &m_WavFileData[outputPosition], m_dataSize - outputPosition );
//				/*E The output data is remainingAdjustBuffer. */
//				/*J WAVファイルデータ展開先はremainingAdjustBufferです */
//				choiceOutputMemory = (uint16_t *)remainingAdjustBuffer;
//			}
//			else {
//				/*E The output data is m_WavFileData. */
//				/*J WAVファイルデータ展開先はm_WavFileDataです */
//				choiceOutputMemory = (uint16_t *)&m_WavFileData[outputPosition];
//			}
//
//			//E Regist to Outputs parameter
//			//J Outputsパラメータへの登録
//			memset( param, 0 , sizeof(param) );
//			for (int i = 0, cnt = 0 ; i < OUTPUTS_NUM ; i++ ) {
//				if ( m_handle[i] != 0 ) {
//					param[cnt].handle = m_handle[i];
//					param[cnt].ptr = choiceOutputMemory;
//					cnt++;
//				}
//			}
//			/*E Output */
//			/*J 出力 */
//			returnCode = sceAudioOutOutputs(&param[0], loginUserNumber);
//			if ( returnCode < 0 ) {
//				printf("Error: sceAudioOutOutputs() 0x%08x\n", returnCode);
//				goto term;
//			}
//			outputPosition += ( USER_GRAIN * m_Channels * sizeof(uint16_t) );
//		}
//		while ( (m_StateFlag != AUDIO_STATUS_END) && (outputPosition < m_dataSize) );
//
//		//E Regist to final Outputs parameter.
//		//J 最終Outputsパラメータへの登録
//		memset( param, 0 , sizeof(param) );
//		for ( int i = 0, cnt = 0 ; i < OUTPUTS_NUM ; i++ ) {
//			if ( m_handle[i] != 0 ) {
//				param[cnt].handle = m_handle[i];
//				param[cnt].ptr = NULL;
//				cnt++;
//			}
//		}
//
//		//E Waiting final data output.
//		//J 最後のデータの出力待ち
//		returnCode = sceAudioOutOutputs(&param[0], loginUserNumber);
//		if ( returnCode < 0 ) {
//			printf("Error: sceAudioOutOutputs() 0x%08x\n", returnCode);
//			goto term;
//		}
//
//		//E Release output port.
//		//J ポートの解放
//		for ( int i = 0 ; i < OUTPUTS_NUM ; i++ ) {
//			if ( m_handle[i] == 0 ) {
//				continue;
//			}
//			returnCode = sceAudioOutClose(m_handle[i]);
//			if ( returnCode < 0 ) {
//				printf("Error: sceAudioOutClose() 0x%08x\n", returnCode);
//				goto term;
//			}
//		}
//
//term:
//
//		//E Clear handle number information.
//		//J ハンドル番号情報のクリア
//		for( int i = 0 ; i < OUTPUTS_NUM ; i++ ) {
//			if ( m_handle[i] == 0 ) {
//				continue;
//			}
//			printf("End-SoundThread:PortHandle 0x%x\n", m_handle[i] );
//		}
//
//		//E Setting terminate to state flag.
//		//J 状態フラグに終了設定。
//		m_StateFlag = AUDIO_STATUS_END;
//
//		return NULL;
//
//	}
//
//	int32_t initialize( int8_t *sound, uint32_t ch, uint32_t size )
//	{
//		int32_t returnCode = 0;
//		m_WavFileData = sound;
//		m_Channels = ch;
//		m_dataSize = size;
//
//		if ( m_StateFlag == AUDIO_STATUS_END ) {
//			/*E Initialization of user service. */
//			/*J ユーザサービス初期化 */
//			returnCode = sceUserServiceInitialize(NULL);
//			if(returnCode != 0) {
//				printf("sceUserServiceInitialize Error:0x%08x", returnCode );
//			}
//			m_StateFlag = AUDIO_STATUS_NONE;
//		}
//
//		/*E AudioOut initialize */
//		/*J AudioOut初期処理  */
//		returnCode = sceAudioOutInit();
//		if ( ( returnCode != 0 ) && ( returnCode != SCE_AUDIO_OUT_ERROR_ALREADY_INIT ) ) {
//			fprintf( stderr, "Failed sceAudioOutInit():0x%08x\n", returnCode );
//			return 1;
//		}	
//
//		/*E Create of WAV file data output thread. */
//		/*J WAVファイルデータ出力用スレッドの生成 */
//		returnCode = pthread_create(&m_thread_sound,  NULL, AudioOut::_soundThread, this);
//		if (returnCode) {										/*J	成功は0　　それ以外はエラー */
//			printf("ERROR: CreateThread 0x%x\n", returnCode);
//			goto term;
//		}
//
//		return 0;
//
//term:
//
//		return returnCode;
//	}
//
//	int32_t finalize(void)
//	{
//		//E Setting terminate to state flag.
//		//J 状態フラグに終了設定。
//		m_StateFlag = AUDIO_STATUS_END;
//
//		//E Waiting for end of threads process.
//		//J 出力スレッドが終了するまで待つ。
//		pthread_join( m_thread_sound, NULL );  
//
//		return 0;
//	}
//
//	/*E Get state value. */
//	/*J 状態値の取得 */
//	int32_t GetStatus(void)
//	{
//		return (m_StateFlag);
//	}
//
//private:
//
//	/*!
//	* @~English
//	* @brief WAV data output thread information structure.
//	* @details This is WAV data output thread information structure.
//	* 
//	* @~Japanese
//	* @brief WAVデータ出力スレッド情報構造体。
//	* @details WAVデータ出力スレッド情報構造体です。
//	* 
//	*/
//	pthread_t	m_thread_sound;
//
//	/*!
//	* @~English
//	* @brief Audio data length.
//	* @details This is audio data length.
//	* 
//	* @~Japanese
//	* @brief オーディオデータ長。
//	* @details オーディオデータ長です。
//	* 
//	*/
//	uint32_t	m_dataSize;
//
//	/*!
//	* @~English
//	* @brief state flag.
//	* @details This is state flag.
//	* 
//	* @~Japanese
//	* @brief 状態フラグ。
//	* @details 状態フラグです。
//	* 
//	*/
//	volatile uint32_t	m_StateFlag;
//
//	/*!
//	* @~English
//	* @brief Audio data.
//	* @details This is audio data.
//	* 
//	* @~Japanese
//	* @brief オーディオデータ。
//	* @details オーディオデータです。
//	* 
//	*/
//	int8_t * m_WavFileData;
//
//	/*!
//	* @~English
//	* @brief Audio data channels.
//	* @details This is number of audio data channels.
//	* 
//	* @~Japanese
//	* @brief オーディオデータチャンネル数。
//	* @details オーディオデータのチャンネル数です。
//	* 
//	*/
//	uint32_t m_Channels;
//};
//
//
//const int32_t  WAVE_FILE_RIFF_HEADER_SIZE = 0x0C;
//
//const int32_t  WAVE_FILE_FORMAT_CHUNK_SIZE = 0x10;
//
//const int32_t  WAVE_FILE_CHUNK_ID_LENGTH = 0x04;
//
//const int32_t  WAVE_FILE_HEADER_LENGTH = 44;
//
//const int32_t  WAVE_FILE_CHUNK_SIZE_LENGTH =0x04;
//
//AudioOut m_audioout;
//
//int8_t * m_WavFileData;
//
//uint32_t m_dataSize;
//
//uint32_t m_Channels;
//
//int readWavFile(void)
//	{
//		int32_t returnCode = 0;
//		int32_t wavFileDataPosition = 0;
//		int32_t wavFileFileDescriptor = 0;
//		SceKernelStat wavFileState;
//
//		//E WAV file open.
//		//J WAVファイルオープン
//		wavFileFileDescriptor = sceKernelOpen( BGM_DATA, O_RDONLY, 0666 );
//		if (wavFileFileDescriptor < 0) {
//			printf ("sceKernelOpen() 0x%x\n", wavFileFileDescriptor);
//			returnCode = wavFileFileDescriptor ;
//			goto term;
//		}
//
//		//E reset offset
//		//J ファイルの先頭にセット
//		returnCode = sceKernelLseek(wavFileFileDescriptor, 0, SEEK_SET); 
//		if (returnCode < 0) {
//			printf ("sceKernelLseek() 0x%x\n", returnCode);
//			goto term;
//		}
//
//		//E Get file size.
//		//J ファイルサイズの取得
//		returnCode = sceKernelFstat( wavFileFileDescriptor, &wavFileState );
//		if (returnCode < 0) {
//			printf ("sceKernelFstat() 0x%x\n", returnCode);
//			goto term;
//		}
//
//		//E Allocate memory for WAV file data.
//		//J WAVファイルデータ用のメモリ確保
//		m_WavFileData = (int8_t *)malloc( wavFileState.st_size );
//		if (m_WavFileData == NULL ) {
//			printf ("ERROR: malloc()\n");
//			goto term;
//		}
//
//		//E Reading WAV file data.
//		//J WAVファイルデータ取得
//		returnCode = sceKernelRead( wavFileFileDescriptor, m_WavFileData, wavFileState.st_size );
//		if (returnCode < 0) {
//			printf ("ERROR: sceKernelRead() 0x%x\n",returnCode);
//			goto term;
//		}
//
//		//E WAV file close.
//		//J WAVファイルクローズ
//		returnCode = sceKernelClose( wavFileFileDescriptor );
//		if (returnCode < 0) {
//			printf ("ERROR: sceKernelClose() 0x%x\n",returnCode);
//			goto term;
//		}
//
//		//E Get number of channels. 
//		//J チャンネル数取得
//		wavFileDataPosition = WAVE_FILE_RIFF_HEADER_SIZE + WAVE_FILE_CHUNK_ID_LENGTH + WAVE_FILE_CHUNK_SIZE_LENGTH;
//		m_Channels = (uint32_t)( ( m_WavFileData[wavFileDataPosition + 0] << 0 ) | 
//						   ( m_WavFileData[wavFileDataPosition + 1] << 8 ) );
//
//		//E Get data chunk data length. 
//		//J データチャンクのデータサイズ
//		wavFileDataPosition += WAVE_FILE_FORMAT_CHUNK_SIZE + WAVE_FILE_CHUNK_ID_LENGTH;
//		m_dataSize = (uint32_t)( ( (uint32_t)( m_WavFileData[wavFileDataPosition + 0] <<  0 ) & 0x000000FF ) | 
//								 ( (uint32_t)( m_WavFileData[wavFileDataPosition + 1] <<  8 ) & 0x0000FF00 ) | 
//								 ( (uint32_t)( m_WavFileData[wavFileDataPosition + 2] << 16 ) & 0x00FF0000 ) | 
//								 ( (uint32_t)( m_WavFileData[wavFileDataPosition + 3] << 24 ) & 0xFF000000 ) );
//		return returnCode;
//
//	term:
//		//E Close WAV file. 
//		//J WAVファイルクローズ
//		if ( wavFileFileDescriptor != 0 ) {
//			sceKernelClose(wavFileFileDescriptor);
//		}
//
//		//E Relese WAV file data memory. 
//		//J WAVファイルデータ用メモリ解放
//		if ( m_WavFileData != NULL ) {
//			free(m_WavFileData);
//			m_WavFileData  = NULL;
//		}
//		return returnCode;
//	}
//
//
//int readWavFile(char * PathToSound)
//	{
//		int32_t returnCode = 0;
//		int32_t wavFileDataPosition = 0;
//		int32_t wavFileFileDescriptor = 0;
//		SceKernelStat wavFileState;
//
//		//E WAV file open.
//		//J WAVファイルオープン
//		wavFileFileDescriptor = sceKernelOpen( PathToSound, O_RDONLY, 0666 );
//		if (wavFileFileDescriptor < 0) {
//			printf ("sceKernelOpen() 0x%x\n", wavFileFileDescriptor);
//			returnCode = wavFileFileDescriptor ;
//			goto term;
//		}
//
//		//E reset offset
//		//J ファイルの先頭にセット
//		returnCode = sceKernelLseek(wavFileFileDescriptor, 0, SEEK_SET); 
//		if (returnCode < 0) {
//			printf ("sceKernelLseek() 0x%x\n", returnCode);
//			goto term;
//		}
//
//		//E Get file size.
//		//J ファイルサイズの取得
//		returnCode = sceKernelFstat( wavFileFileDescriptor, &wavFileState );
//		if (returnCode < 0) {
//			printf ("sceKernelFstat() 0x%x\n", returnCode);
//			goto term;
//		}
//
//		//E Allocate memory for WAV file data.
//		//J WAVファイルデータ用のメモリ確保
//		m_WavFileData = (int8_t *)malloc( wavFileState.st_size );
//		if (m_WavFileData == NULL ) {
//			printf ("ERROR: malloc()\n");
//			goto term;
//		}
//
//		//E Reading WAV file data.
//		//J WAVファイルデータ取得
//		returnCode = sceKernelRead( wavFileFileDescriptor, m_WavFileData, wavFileState.st_size );
//		if (returnCode < 0) {
//			printf ("ERROR: sceKernelRead() 0x%x\n",returnCode);
//			goto term;
//		}
//
//		//E WAV file close.
//		//J WAVファイルクローズ
//		returnCode = sceKernelClose( wavFileFileDescriptor );
//		if (returnCode < 0) {
//			printf ("ERROR: sceKernelClose() 0x%x\n",returnCode);
//			goto term;
//		}
//
//		//E Get number of channels. 
//		//J チャンネル数取得
//		wavFileDataPosition = WAVE_FILE_RIFF_HEADER_SIZE + WAVE_FILE_CHUNK_ID_LENGTH + WAVE_FILE_CHUNK_SIZE_LENGTH;
//		m_Channels = (uint32_t)( ( m_WavFileData[wavFileDataPosition + 0] << 0 ) | 
//						   ( m_WavFileData[wavFileDataPosition + 1] << 8 ) );
//
//		//E Get data chunk data length. 
//		//J データチャンクのデータサイズ
//		wavFileDataPosition += WAVE_FILE_FORMAT_CHUNK_SIZE + WAVE_FILE_CHUNK_ID_LENGTH;
//		m_dataSize = (uint32_t)( ( (uint32_t)( m_WavFileData[wavFileDataPosition + 0] <<  0 ) & 0x000000FF ) | 
//								 ( (uint32_t)( m_WavFileData[wavFileDataPosition + 1] <<  8 ) & 0x0000FF00 ) | 
//								 ( (uint32_t)( m_WavFileData[wavFileDataPosition + 2] << 16 ) & 0x00FF0000 ) | 
//								 ( (uint32_t)( m_WavFileData[wavFileDataPosition + 3] << 24 ) & 0xFF000000 ) );
//		return returnCode;
//
//	term:
//		//E Close WAV file. 
//		//J WAVファイルクローズ
//		if ( wavFileFileDescriptor != 0 ) {
//			sceKernelClose(wavFileFileDescriptor);
//		}
//
//		//E Relese WAV file data memory. 
//		//J WAVファイルデータ用メモリ解放
//		if ( m_WavFileData != NULL ) {
//			free(m_WavFileData);
//			m_WavFileData  = NULL;
//		}
//		return returnCode;
//	}
//
//
//int PlaySound(void)
//{
//		int returnCode = 0;
//
//		/*E Initialization of user service. */
//		/*J ユーザサービス初期化 */
//		returnCode = sceUserServiceInitialize(NULL);
//		if(returnCode != 0){
//			return returnCode;
//		}
//
//		/*E Get WAV file information. */
//		/*J WAVファイル情報の取得 */
//		returnCode = readWavFile();
//		if (returnCode < 0) {
//			return returnCode;
//		}
//
//		/*E WAV file output initial processing. */
//		/*J WAVファイル出力初期処理 */
//		returnCode = m_audioout.initialize( &m_WavFileData[WAVE_FILE_HEADER_LENGTH], m_Channels, m_dataSize );
//		if(returnCode != 0){
//			return returnCode;
//		}
//}
//
//
//int PlaySound(char * Path)
//{
//
//		int returnCode = 0;
//
//		StartAudioInit();
//
//
//		/*E Initialization of user service. */
//		/*J ユーザサービス初期化 */
//		returnCode = sceUserServiceInitialize(NULL);
//		if(returnCode != 0){
//			return returnCode;
//		}
		
//		/*E Get WAV file information. */
//		/*J WAVファイル情報の取得 */
//		returnCode = readWavFile(Path);
//		if (returnCode < 0) {
//			return returnCode;
//		}
//
//		/*E WAV file output initial processing. */
//		/*J WAVファイル出力初期処理 */
//		returnCode = m_audioout.initialize( &m_WavFileData[WAVE_FILE_HEADER_LENGTH], m_Channels, m_dataSize );
//		if(returnCode != 0){
//			return returnCode;
//		}
//}