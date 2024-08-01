#define STRINGIFY(src) #src

static const char* STATIC_WEB_MAIN =
#include "index.html.h"
;

static const char* STATIC_WEB_CSS = 
#include "styles.css.h"
;

static const char* STATIC_WEB_SCRIPT = 
#include "script.js.h"
;

static const char* LIB_THREE_JS = 
#include "threejs.js.h"
;
