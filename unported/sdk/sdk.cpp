
/*
 *
 * Temporary file for fixing defines in the sdk
 *
 */

// Doing this to shut up msvc from complaining... Like fucking hell does it
// complain about everything >_>
#if defined(_WIN32) && !defined(NDEBUG)

#include "sdk.hpp"

// stolen from the sdk lol
extern void _ExitOnFatalAssert(const tchar *pFile, int line) { assert(0); }
extern void CallAssertFailedNotifyFunc(const char *pchFile, int nLine,
                                       const char *pchMessage) {
    assert(0);
}
extern bool ShouldUseNewAssertDialog() { return true; }
extern bool DoNewAssertDialog(const tchar *pFile, int line,
                              const tchar *pExpression) {
    return false;
}
extern void _SpewInfo(SpewType_t type, const tchar *pFile, int line) {}
extern SpewRetval_t _SpewMessage(PRINTF_FORMAT_STRING const tchar *pMsg, ...)
    FMTFUNCTION(1, 2) {
    return SPEW_ABORT;
}
#endif
