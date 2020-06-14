#include "cjson_wrapper.h"

/*
Format e.g.:
{
    "requestId":"0bc458b5-47f6-472f-bd88-3624696e9776",
    "code":0,   // params is invalid, when code != 0
    "message":"",
    "params":{
          "key1":"value1",
          "key2":"value2"
    }
}
*/
#define GWC_KEY_MSG_CODE                   "code"
#define GWC_KEY_MSG_ID                     "requestId"
#define GWC_KEY_MSG_MESSAGE                "message"
#define GWC_KEY_MSG_PARAMS                 "params"

static int parse_json_func( const char *payload )
{
    cJSON *root = NULL;
    cJSON *params = NULL;
    char uuid[64+4] = {0};
    char msg[128+4] = {0};
    uint64_t code;
    int ret = -1;

    if (!payload)
    {
        loge("Params is NULL.\n");
        goto exit;
    }

    root = cJSON_Parse(payload);
    JSON_NULL_GOTO(root, exit, "Get payload root failed: %s", payload);
    JSON_GET_STRING(root, GWC_KEY_MSG_ID, exit, uuid, 64);
    JSON_TRY_GET_NUMBER(root, GWC_KEY_MSG_CODE, exit, code, 0);
    JSON_TRY_GET_STRING(root, GWC_KEY_MSG_MESSAGE, exit, msg, 128, "success");
    JSON_GET_OBJECT(root, GWC_KEY_MSG_PARAMS, exit, params);
    
    ret = 0;
exit:
    JSON_FREE(root);
    return ret;
}
