#ifdef __cplusplus
extern "C" {
#endif
	typedef unsigned char HashOut[32];
	void Hash(const char *data, size_t size, HashOut HashResult);
#ifdef __cplusplus
}
#endif
