#include "DownloadHTTPS.h"


#define SSL_HEAP_SIZE	(304 * 1024)
#define HTTP_HEAP_SIZE	(48 * 1024)
#define NET_HEAP_SIZE	(16 * 1024)
#define TEST_USER_AGENT	"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:69.0) Gecko/20100101 Firefox/69.0"
#define TARGET_URL	"https://sample.siedev.net/api_libhttp-http_get-https.html"


SceInt32 http_get(int libhttpCtxId, const char *targetUrl);
SceUChar8* http_download(int libhttpCtxId, const char *targetUrl);

static void printSslError(SceInt32 sslErr, SceUInt32 sslErrDetail)
{
	switch (sslErr){
	case (SCE_HTTPS_ERROR_CERT):			/* Verify error */
		/* Internal error at verifying certificate*/
		if (sslErrDetail & SCE_HTTPS_ERROR_SSL_INTERNAL){
			printf("ssl verify error: unexpcted error\n");
		}
		/* Error of server certificate or CA certificate */
		if (sslErrDetail & SCE_HTTPS_ERROR_SSL_INVALID_CERT){
			printf("ssl verify error: invalid server cert or CA cert\n");
		}
		/* Server hostname and server certificate are mismatched*/
		if (sslErrDetail & SCE_HTTPS_ERROR_SSL_CN_CHECK){
			printf("ssl verify error: invalid server hostname\n");
		}
		/* Server certificate or CA certificate is expired.*/
		if (sslErrDetail & SCE_HTTPS_ERROR_SSL_NOT_AFTER_CHECK){
			printf("ssl verify error: server cert or CA cert had expired\n");
		}
		/* Server certificate or CA certificate is before validated.*/
		if (sslErrDetail & SCE_HTTPS_ERROR_SSL_NOT_BEFORE_CHECK){
			printf("ssl verify error: server cert or CA cert isn't validated yet.\n");
		}
		/* Unknown CA error */
		if (sslErrDetail & SCE_HTTPS_ERROR_SSL_UNKNOWN_CA){
			printf("ssl verify error: unknown CA\n");
		}
		break;
	case (SCE_HTTPS_ERROR_HANDSHAKE):		/* fail to ssl-handshake  */
		printf("ssl error: handshake error\n");
		break;
	case (SCE_HTTPS_ERROR_IO):				/* Error of Socket IO */
		printf("ssl error: io error\n");
		break;
	case (SCE_HTTP_ERROR_OUT_OF_MEMORY):	/* Out of memory*/
		printf("ssl error: out of memory\n");
		break;
	case (SCE_HTTPS_ERROR_INTERNAL):		/* Unexpected Internal Error*/
		printf("ssl error: unexpcted error\n");
		break;
	default:
		break;
	}
	return;
}

static void printSslCertInfo(int libsslCtxId,SceSslCert *sslCert)
{
	SceInt32 ret;
	SceUChar8 *sboData = NULL ;
	SceSize sboLen, counter;

	ret = sceSslGetSerialNumber(libsslCtxId, sslCert, NULL, &sboLen);
	if (ret < 0){
		printf ("sceSslGetSerialNumber() returns 0x%x\n", ret);
	}
	else {
		sboData = (SceUChar8*)malloc(sboLen);
		if ( sboData != NULL ) {
			ret = sceSslGetSerialNumber(libsslCtxId, sslCert, sboData, &sboLen);
			if (ret < 0){
				printf ("sceSslGetSerialNumber() returns 0x%x\n", ret);
			}
			else {
				printf("Serial number=");
				for (counter = 0; counter < sboLen; counter++){
					printf("%02X", sboData[counter]);
				}
				printf("\n");
			}
			free(sboData);
		}
	}
}

static SceInt32 sslCallback(int libsslCtxId,unsigned int verifyErr,SceSslCert * const sslCert[],int certNum,void *userArg)
{
	SceInt32 i;
	(void)userArg;

	printf("Ssl callback:\n");
	printf("\tbase tmpl[%x]\n", (*(SceInt32*)(userArg)) );

	if (verifyErr != 0){
		printSslError((SceInt32)SCE_HTTPS_ERROR_CERT, verifyErr);
	}
	for (i = 0; i < certNum; i++){
		printSslCertInfo(libsslCtxId,sslCert[i]);
	}
	if (verifyErr == 0){
		return SCE_OK;
	} else {
		return -1;
	}
}

static SceInt32 netInit(void)
{
	int ret;
	int libnetMemId;

	/* libnet */
	ret = sceNetInit();
	if (ret < 0) {
		printf("[SAMPLE] %s,%d ret=%x\n", __FUNCTION__, __LINE__, ret);
		goto error;
	}
	ret = sceNetPoolCreate("simple", NET_HEAP_SIZE, 0);
	if (ret < 0){
		goto net_term;
	}
	libnetMemId = ret;

	return libnetMemId;

net_term:
	sceNetTerm();
error:
	return ret;
}

static SceInt32 netShutdown(int libnetMemId)
{
	SceInt32 ret;

	/* libnet */
	ret = sceNetPoolDestroy(libnetMemId);
	if (ret < 0) {
		printf("[SAMPLE] %s,%d ret=%x\n", __FUNCTION__, __LINE__, ret);
	}
	ret = sceNetTerm();
	if (ret < 0) {
		printf("[SAMPLE] %s,%d ret=%x\n", __FUNCTION__, __LINE__, ret);
	}

	return ret;
}

SceInt32 main(int argc, const char *const argv[])
{
	SceInt32 ret;
	SceInt32 returnCode = SCE_OK;
	const char *targetUrl=NULL;
	int libnetMemId = 0;
	int libsslCtxId = 0;
	int libhttpCtxId = 0;

	if (argc < 2){
		targetUrl = TARGET_URL;
	} else{
		targetUrl = argv[1];
	}

	ret = netInit();
	if (ret < 0){
		goto term;
	}
	libnetMemId = ret;

	ret = sceSslInit(SSL_HEAP_SIZE);
	if (ret < 0){
		printf("sceSslInit() error: 0x%08X\n", ret);
		goto net_term;
	}
	libsslCtxId = ret;

	ret = sceHttpInit(libnetMemId, libsslCtxId, HTTP_HEAP_SIZE);
	if (ret < 0){
		printf("sceHttpInit() error: 0x%08X\n", ret);
		goto ssl_term;
	}
	libhttpCtxId = ret;

	// Message for SDK sample auto test
	printf("## api_libhttp/https: INIT SUCCEEDED ##\n");

	returnCode = http_get(libhttpCtxId, targetUrl);

	ret = sceHttpTerm(libhttpCtxId);
	if (ret < 0){
		printf("sceHttpEnd() error: 0x%08X\n", ret);
	}

ssl_term:
	ret = sceSslTerm(libsslCtxId);
	if (ret < 0){
		printf("sceSslEnd() error: 0x%08X\n", ret);
	}

net_term:
	netShutdown(libnetMemId);
term:
	// Message for SDK sample auto test
	printf("## api_libhttp/https: FINISHED ##\n");
	return returnCode;
}

SceInt32 http_get(int libhttpCtxId, const char *targetUrl)
{
	SceInt32 ret, tmplId=0, connId=0, reqId=0, statusCode;
	SceULong64	contentLength;
	SceBool		finFlag=SCE_FALSE;
	SceUChar8 recvBuf[1500];

	ret = sceHttpCreateTemplate(libhttpCtxId, TEST_USER_AGENT, SCE_HTTP_VERSION_1_1, SCE_TRUE);
	if (ret < 0){
		printf("sceHttpCreateTemplate() error: 0x%08X\n", ret);
		goto error;
	}
	tmplId = ret;

	/* Perform http_get without server verification */
	ret = sceHttpsDisableOption(tmplId,SCE_HTTPS_FLAG_SERVER_VERIFY);
	if (ret < 0){
		printf("sceHttpsDisableOption() error: 0x%08X\n", ret);
		goto error;
	}

	/* Register SSL callback */
	ret = sceHttpsSetSslCallback(tmplId, sslCallback, (void*)&tmplId);
	if (ret < 0) {
		printf("sceHttpsSetSslCallback() error: 0x%08X\n", ret);
		goto error;
	}

	ret = sceHttpCreateConnectionWithURL(tmplId, targetUrl, SCE_TRUE);
	if (ret < 0){
		printf("sceHttpCreateConnectionWithURL() error: 0x%08X\n", ret);
		goto error;
	}
	connId = ret;

	ret = sceHttpCreateRequestWithURL(connId, SCE_HTTP_METHOD_GET, targetUrl, 0);
	if (ret < 0){
		printf("sceHttpCreateRequestWithURL() error: 0x%08X\n", ret);
		goto error;
	}
	reqId = ret;

	ret = sceHttpSendRequest(reqId, NULL, 0);
	if (ret < 0){
		printf("sceHttpSendRequest() error: 0x%08X\n", ret);
		goto error;
	}

	ret = sceHttpGetStatusCode(reqId, &statusCode);
	if (ret < 0){
		printf("sceHttpGetStatusCode() error: 0x%08X\n", ret);
		goto error;
	}
	printf("response code = %d\n", statusCode);

	if(statusCode == 200){
		int contentLengthType;
		ret = sceHttpGetResponseContentLength(reqId, &contentLengthType, &contentLength);
		if(ret < 0){
			printf("sceHttpGetContentLength() error: 0x%08X\n", ret);
			goto error;
		}else {
			if (contentLengthType == SCE_HTTP_CONTENTLEN_EXIST) {
				printf("Content-Length = %lu\n", contentLength);
			}
		}
		while(finFlag != SCE_TRUE){
			ret = sceHttpReadData(reqId, recvBuf, sizeof(recvBuf));
			if (ret < 0){
				goto error;
			} else if (ret == 0){
				finFlag = SCE_TRUE;
			}
			printf("\n sceHttpReadData() read %d bytes\n", ret);
		}
	}

error:
	if (reqId > 0){
		ret = sceHttpDeleteRequest(reqId);
		if (ret < 0){
			printf("sceHttpDeleteRequest() error: 0x%08X\n", ret);
		}
	}
	if (connId > 0){
		ret = sceHttpDeleteConnection(connId);
		if (ret < 0){
			printf("sceHttpDeleteConnection() error: 0x%08X\n", ret);
		}
	}
	if (tmplId > 0){
		ret = sceHttpDeleteTemplate(tmplId);
		if (ret < 0){
			printf("sceHttpDeleteTemplate() error: 0x%08X\n", ret);
		}
	}
	return ret;
}

SceUChar8* Download(const char * targetUrl)
{

	char buffer[5120];
	SceInt32 ret;
	SceInt32 returnCode = SCE_OK;
	SceUChar8* returnValue;

	int libnetMemId = 0;
	int libsslCtxId = 0;
	int libhttpCtxId = 0;

	ret = netInit();
	if (ret < 0){
		goto term;
	}
	libnetMemId = ret;

	ret = sceSslInit(SSL_HEAP_SIZE);
	if (ret < 0){
		printf("sceSslInit() error: 0x%08X\n", ret);
		sprintf(buffer, "sceSslInit() error: 0x%08X\n", ret);//dont want all those enters
		notify(buffer);
		goto net_term;
	}
	libsslCtxId = ret;

	ret = sceHttpInit(libnetMemId, libsslCtxId, HTTP_HEAP_SIZE);
	if (ret < 0){
		printf("sceHttpInit() error: 0x%08X\n", ret);
		sprintf(buffer, "sceHttpInit() error: 0x%08X\n", ret);//dont want all those enters
		notify(buffer);
		goto ssl_term;
	}
	libhttpCtxId = ret;

	// Message for SDK sample auto test
	printf("## api_libhttp/https: INIT SUCCEEDED ##\n");
	//notify("## api_libhttp/https: INIT SUCCEEDED ##\n");
	returnValue = http_download(libhttpCtxId, targetUrl);

	ret = sceHttpTerm(libhttpCtxId);
	if (ret < 0){
		printf("sceHttpEnd() error: 0x%08X\n", ret);
		sprintf(buffer, "sceHttpEnd() error: 0x%08X\n", ret);//dont want all those enters
		notify(buffer);
	}

ssl_term:
	ret = sceSslTerm(libsslCtxId);
	if (ret < 0){
		printf("sceSslEnd() error: 0x%08X\n", ret);
		sprintf(buffer, "sceSslEnd() error: 0x%08X\n", ret);//dont want all those enters
		notify(buffer);
	}

net_term:
	netShutdown(libnetMemId);
term:
	// Message for SDK sample auto test
	printf("## api_libhttp/https: FINISHED ##\n");
	//notify("## api_libhttp/https: FINISHED ##\n");
	return returnValue;
}

SceUChar8* http_download(int libhttpCtxId, const char *targetUrl)
{
	SceInt32 ret, tmplId=0, connId=0, reqId=0, statusCode;
	SceULong64	contentLength;
	SceBool		finFlag=SCE_FALSE;
	SceUChar8 recvBuf[1500];
	char buffer[5120];

	ret = sceHttpCreateTemplate(libhttpCtxId, TEST_USER_AGENT, SCE_HTTP_VERSION_1_1, SCE_TRUE);
	if (ret < 0){
		printf("sceHttpCreateTemplate() error: 0x%08X\n", ret);

		sprintf(buffer, "sceHttpCreateTemplate() error: 0x%08X\n", ret);//dont want all those enters

		notify(buffer);
		goto error;
	}
	tmplId = ret;

	ret = sceHttpSetNonblock(tmplId, SCE_TRUE) ;
	if ( ret < 0 ) {
		printf("sceHttpSetNonblock() error: 0x%08X\n",ret) ;
		goto error ;
	}


	/* Perform http_get without server verification */
	//ret = sceHttpsDisableOption(tmplId,SCE_HTTPS_FLAG_SERVER_VERIFY);
	//if (ret < 0){
	//	printf("sceHttpsDisableOption() error: 0x%08X\n", ret);
	//	sprintf(buffer, "sceHttpsDisableOption() error: 0x%08X\n", ret);//dont want all those enters

	//	notify(buffer);
	//	goto error;
	//}

	/* Register SSL callback */
	ret = sceHttpsSetSslCallback(tmplId, sslCallback, (void*)&tmplId);
	if (ret < 0) {
		printf("sceHttpsSetSslCallback() error: 0x%08X\n", ret);
		sprintf(buffer, "sceHttpsSetSslCallback() error: 0x%08X\n", ret);//dont want all those enters
		notify(buffer);
		goto error;
	}

	ret = sceHttpCreateConnectionWithURL(tmplId, targetUrl, SCE_TRUE);
	if (ret < 0){
		printf("sceHttpCreateConnectionWithURL() error: 0x%08X\n", ret);
		sprintf(buffer, "sceHttpCreateConnectionWithURL() error: 0x%08X\n", ret);//dont want all those enters
		notify(buffer);
		goto error;
	}
	connId = ret;

	ret = sceHttpCreateRequestWithURL(connId, SCE_HTTP_METHOD_GET, targetUrl, 0);
	if (ret < 0){
		printf("sceHttpCreateRequestWithURL() error: 0x%08X\n", ret);
		sprintf(buffer, "sceHttpCreateRequestWithURL() error: 0x%08X\n", ret);//dont want all those enters
		notify(buffer);
		goto error;
	}
	reqId = ret;

	ret = sceHttpSendRequest(reqId, NULL, 0);
	if (ret < 0){
		printf("sceHttpSendRequest() error: 0x%08X\n", ret);
		sprintf(buffer, "sceHttpSendRequest() error: 0x%08X\n", ret);//dont want all those enters
		notify(buffer);
		goto error;
	}

	ret = sceHttpGetStatusCode(reqId, &statusCode);
	if (ret < 0){
		printf("sceHttpGetStatusCode() error: 0x%08X\n", ret);
		sprintf(buffer, "sceHttpGetStatusCode() error: 0x%08X\n", ret);//dont want all those enters
		notify(buffer);
		goto error;
	}
	printf("response code = %d\n", statusCode);
	sprintf(buffer, "response code = %d\n", ret);//dont want all those enters
	notify(buffer);
	if(statusCode == 200){
		int contentLengthType;
		ret = sceHttpGetResponseContentLength(reqId, &contentLengthType, &contentLength);
		if(ret < 0){
			printf("sceHttpGetContentLength() error: 0x%08X\n", ret);
			sprintf(buffer, "sceHttpGetContentLength() error: 0x%08X\n", ret);//dont want all those enters
			notify(buffer);
			goto error;
		}else {
			if (contentLengthType == SCE_HTTP_CONTENTLEN_EXIST) {
				printf("Content-Length = %lu\n", contentLength);
				sprintf(buffer, "Content-Length = %lu\n", ret);//dont want all those enters
				notify(buffer);
			}
		}
		while(finFlag != SCE_TRUE){
			ret = sceHttpReadData(reqId, recvBuf, sizeof(recvBuf));
			if (ret < 0){
				goto error;
			} else if (ret == 0){
				finFlag = SCE_TRUE;
			}
			printf("\n sceHttpReadData() read %d bytes\n", ret);
			sprintf(buffer, "\n sceHttpReadData() read %d bytes\n", ret);//dont want all those enters
			notify(buffer);
		}
	}

error:
	if (reqId > 0){
		ret = sceHttpDeleteRequest(reqId);
		if (ret < 0){
			printf("sceHttpDeleteRequest() error: 0x%08X\n", ret);
			sprintf(buffer, "sceHttpDeleteRequest() error: 0x%08X\n", ret);//dont want all those enters
			notify(buffer);
		}
	}
	if (connId > 0){
		ret = sceHttpDeleteConnection(connId);
		if (ret < 0){
			printf("sceHttpDeleteConnection() error: 0x%08X\n", ret);
			sprintf(buffer, "sceHttpDeleteConnection() error: 0x%08X\n", ret);//dont want all those enters
			notify(buffer);
		}
	}
	if (tmplId > 0){
		ret = sceHttpDeleteTemplate(tmplId);
		if (ret < 0){
			printf("sceHttpDeleteTemplate() error: 0x%08X\n", ret);
			sprintf(buffer, "sceHttpDeleteTemplate() error: 0x%08X\n", ret);//dont want all those enters
			notify(buffer);
		}
	}
	return recvBuf;
}