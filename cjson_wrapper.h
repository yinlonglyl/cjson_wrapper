#ifndef CJSON_WRAPPER_H
#define CJSON_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cJSON.h>

#ifndef SIMPLE_LOGGER
#include <stdio.h>
#define logi printf
#define logd printf
#define loge printf
#define logw printf
#else
#include "simple_log.h"
#endif

#define JSON_FREE(Pointer)         \
        do {\
            if (Pointer)    \
            {   \
                cJSON_Delete(Pointer); Pointer = NULL; \
            }\
        } while(0)
        
#define JSON_NULL_GOTO(Pointer, gotoTag, strError, args...)         \
        do {\
            if (!Pointer)    \
            {   \
                loge(strError, ##args); \
                goto gotoTag; \
            }\
        } while(0)
    
#define JSON_NULL_WARNING(Pointer, strError, args...)         \
        do {\
            if (!Pointer)    \
            {   \
                logw(strError, ##args); \
            }\
        } while(0)
    
#define JSON_PARSE_GOTO(Pointer, data, gotoTag, strError, args...)         \
        do {\
            Pointer = cJSON_Parse(data); \
            if (!Pointer)    \
            {   \
                loge(strError, ##args); \
                goto gotoTag; \
            }\
        } while(0)
    
#define JSON_TYPE_MISMATCH_GOTO(Pointer, jsonType, gotoTag, strError, args...)         \
        do {\
            if (Pointer->type != jsonType)    \
            {   \
                loge(strError, ##args); \
                goto gotoTag; \
            }\
        } while(0)
    
#define JSON_GET_OBJECT(jsonObj, key, gotoTag, objStore)         \
        do { \
            objStore = cJSON_GetObjectItem(jsonObj, key); \
            JSON_NULL_GOTO(objStore, gotoTag, "[Json Parse] Get %s failed!", key); \
            JSON_TYPE_MISMATCH_GOTO(objStore, cJSON_Object, gotoTag, "[Json Parse] Get %s type error, should be cJSON_Object.", key); \
        } while(0)
    
#define JSON_GET_ARRAY(jsonObj, key, gotoTag, objStore)         \
        do { \
            objStore = cJSON_GetObjectItem(jsonObj, key); \
            JSON_NULL_GOTO(objStore, gotoTag, "[Json Parse] Get %s failed!", key); \
            JSON_TYPE_MISMATCH_GOTO(objStore, cJSON_Array, gotoTag, "[Json Parse] Get %s type error, should be cJSON_Array.", key); \
        } while(0)

#define JSON_TRY_GET_ARRAY(jsonObj, key, gotoTag, objStore)         \
        do { \
            objStore = cJSON_GetObjectItem(jsonObj, key); \
            if (objStore) { \
                JSON_TYPE_MISMATCH_GOTO(objStore, cJSON_Array, gotoTag, "[Json Parse] Get %s type error, should be cJSON_Array.", key); \
            } else { \
                logd("[JSON Parse] %s is not set. \n", key); \
            } \
        } while(0)

/* Use json_value->valuedouble instead of valueint here to save bigger number.*/
#define JSON_GET_NUMBER(jsonObj, key, gotoTag, numStore)         \
        do { \
            cJSON *json_value = cJSON_GetObjectItem(jsonObj, key); \
            JSON_NULL_GOTO(json_value, gotoTag, "[Json Parse] Get %s failed!", key); \
            JSON_TYPE_MISMATCH_GOTO(json_value, cJSON_Number, gotoTag, "[Json Parse] Get %s type error, should be cJSON_Number.", key); \
            numStore = json_value->valuedouble; \
        } while(0)
    
#define JSON_GET_STRING_MALLOC(jsonObj, key, gotoTag, ptrStore)         \
        do {\
            int val_len = 0; \
            cJSON *json_value = cJSON_GetObjectItem(jsonObj, key); \
            JSON_NULL_GOTO(json_value, gotoTag, "[Json Parse] Get %s failed!", key); \
            JSON_TYPE_MISMATCH_GOTO(json_value, cJSON_String, gotoTag, "[Json Parse] Get %s type error, should be cJSON_String.", key); \
            val_len = strlen(json_value->valuestring); \
            ptrStore = (char*)malloc(val_len + 1); \
            if (NULL == ptrStore) { \
                loge("[Malloc %d bytes] failed! \n", val_len + 1); \
                goto gotoTag; \
            } else { \
                snprintf(ptrStore, val_len+1, "%s", json_value->valuestring); \
            } \
        } while(0)
    
#define JSON_GET_STRING(jsonObj, key, gotoTag, bufStore, lenStore)         \
        do {\
            int val_len = 0; \
            cJSON *json_value = cJSON_GetObjectItem(jsonObj, key); \
            JSON_NULL_GOTO(json_value, gotoTag, "[Json Parse] Get %s failed!", key); \
            JSON_TYPE_MISMATCH_GOTO(json_value, cJSON_String, gotoTag, "[Json Parse] Get %s type error, should be cJSON_String.", key); \
            val_len = strlen(json_value->valuestring); \
            if (val_len > lenStore) { \
                loge("[Json Parse] %s is too long > %d.\n", key, lenStore); \
                goto gotoTag; \
            } else { \
                snprintf(bufStore, lenStore+1, "%s", json_value->valuestring); \
            } \
        } while(0)
    
#define JSON_TRY_GET_STRING(jsonObj, key, gotoTag, bufStore, lenStore, defaultValue)         \
        do {\
            cJSON *json_obj_tmp = cJSON_GetObjectItem(jsonObj, key); \
            if (json_obj_tmp) { \
                JSON_GET_STRING(jsonObj, key, gotoTag, bufStore, lenStore); \
            } else { \
                snprintf(bufStore, lenStore+1, "%s", defaultValue); \
            } \
        } while(0)
    
#define JSON_TRY_GET_NUMBER(jsonObj, key, gotoTag, numStore, defaultValue)         \
        do {\
            cJSON *json_obj_tmp = cJSON_GetObjectItem(jsonObj, key); \
            if (json_obj_tmp) \
            { \
                JSON_GET_NUMBER(jsonObj, key, gotoTag, numStore); \
            } else { \
                numStore = defaultValue; \
            } \
        } while(0)

#define JSON_GET_BOOL(jsonObj, key, gotoTag, numStore)         \
        do {\
            cJSON *json_value = cJSON_GetObjectItem(jsonObj, key); \
            JSON_NULL_GOTO(json_value, gotoTag, "[Json Parse] Get %s failed!", key); \
            numStore = (json_value->type == cJSON_True)? 1:0; \
        } while(0)

#define JSON_GET_STRING_TRUNC(jsonObj, key, gotoTag, bufStore, lenStore)         \
        do {\
            cJSON *json_value = cJSON_GetObjectItem(jsonObj, key); \
            JSON_NULL_GOTO(json_value, gotoTag, "[Json Parse] Get %s failed!", key); \
            JSON_TYPE_MISMATCH_GOTO(json_value, cJSON_String, gotoTag, "[Json Parse] Get %s type error, should be cJSON_String.", key); \
            snprintf(bufStore, lenStore, "%s", json_value->valuestring); \
        } while(0)

#define JSON_ADD_NEW_OBJ2OBJ(rootObj, key, gotoTag, newObj)    \
        do {\
            newObj = cJSON_CreateObject(); \
            JSON_NULL_GOTO(newObj, gotoTag, "[Json Create] Object %s failed!", key); \
            cJSON_AddItemToObject(rootObj, key, newObj); \
        } while(0)

#define JSON_ADD_NEW_OBJ2ARRAY(rootObj, gotoTag, newObj)    \
        do {\
            newObj = cJSON_CreateObject(); \
            JSON_NULL_GOTO(newObj, gotoTag, "[Json Create] Object failed!"); \
            cJSON_AddItemToArray(rootObj, newObj); \
        } while(0)

#define JSON_ADD_NEW_ARRAY2OBJ(rootObj, key, gotoTag, newObj)    \
        do {\
            newObj = cJSON_CreateArray(); \
            JSON_NULL_GOTO(newObj, gotoTag, "[Json Create] Array %s failed!", key); \
            cJSON_AddItemToObject(rootObj, key, newObj); \
        } while(0)

#define JSON_ADD_NONEMPTY_STRING2OBJ(rootObj, key, gotoTag, string)    \
        do {\
            JSON_NULL_GOTO(rootObj, gotoTag, "[Json ADD %s] Param rootObj is NULL!", key); \
            JSON_NULL_GOTO(string, gotoTag, "[Json ADD %s] Param string is NULL!", key); \
            if (0 == strlen(string)) { \
                loge("[Json ADD %s] Param string is Empty.\n", key); \
                goto gotoTag; \
            } \
            cJSON_AddStringToObject(rootObj, key, string); \
        } while(0)

#define JSON_TRY_ADD_STRING2OBJ(rootObj, key, gotoTag, string, defaultValue)    \
        do {\
            JSON_NULL_GOTO(rootObj, gotoTag, "[Json ADD %s] Param rootObj is NULL!", key); \
            if (!string || 0 == strlen(string)) { \
                cJSON_AddStringToObject(rootObj, key, defaultValue); \
            } else {\
                cJSON_AddStringToObject(rootObj, key, string); \
            } \
        } while(0)


#ifdef __cplusplus
}
#endif

#endif

