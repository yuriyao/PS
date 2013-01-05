/**
 *  测试编译指令
 * gcc -o __web__ web.c ../util/log.c mm.c -lpython2.6 -lcurl
 **/

#include "../include/web.h"


static size_t
write_func(char *ptr, size_t size, size_t nmemb, void *userdata);

/**
 *  只能调用一次的初始化函数
 **/
BOOL 
Ps_WebInit(BOOL ssl)
{
	long flags = CURL_GLOBAL_NOTHING;
	CURLcode code;
	static int init = 0;
	/*只初始化一次*/
	if(init != 0)
		return TRUE;
	init = 1;

	if(ssl)
		flags |= CURL_GLOBAL_SSL;

#if (PLATFORM & WIN32) || (PLATFORM & WIN64)
	flags |= CURL_GLOBAL_WIN32;
#endif

	if((code = curl_global_init(flags)) != 0)
	{
		return FALSE;
	}
	return TRUE;
}

/**
 *  下载文件
 *  url 
 *  p 写入的文件
 **/
BOOL 
Ps_Download(char *url, FILE *p)
{
	return Ps_DownloadTimeout(url, p, 0);
}

/**
 *  下载文件（只是多了超时的时间）
 *  timeout 超时时间（单位秒）
 **/
BOOL 
Ps_DownloadTimeout(char *url, FILE *p, int timeout)
{
	CURL *handle = NULL;
	timeout = (timeout > 0) ? timeout : 0;
	CURLcode code;
	if(url == NULL)
	{
		Ps_Log("Want to download from NULL url\n", Ps_LOG_WARING);
		return FALSE;
	}
	if(p == NULL)
	{
		Ps_Log("Want to download file to NULL file pointer\n", Ps_LOG_WARING);
		return FALSE;
	}
	if((handle = curl_easy_init()) == NULL)
	{
		Ps_Log("CURL easy init failed\n", Ps_LOG_WARING);
		return FALSE;
	}
#ifdef WEB_DEBUG
	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
#endif
	if(timeout > 0)
	{
		curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
	}
	curl_easy_setopt(handle, CURLOPT_URL, url);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)p);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_func);

	if((code = curl_easy_perform(handle)) != 0)
	{
		Ps_Log("Cur easy perform deal failed\n", Ps_LOG_WARING);
		curl_easy_cleanup(handle);
		return FALSE;
	}
	curl_easy_cleanup(handle);
	return TRUE;
}

/**
 * 只能调用一次的清理函数
 **/
BOOL Ps_WebClear()
{
	curl_global_cleanup();
	return TRUE;
}


static size_t
write_func(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	return fwrite(ptr, size, nmemb, userdata);
}


#ifdef WEB_DEBUG
#include "../include/init_clear.h"
int main(int argc, char **argv)
{
	FILE *p;
	Ps_INIT(NULL, NULL, NULL);
	Ps_WebInit(FALSE);
	p = fopen(argv[1], "wb");
	if(!p)
	{
		perror("fopen");
		return -1;
	}
	Ps_Download(argv[2], p);
	Ps_WebClear();
	Ps_CLEAR();
}
#endif