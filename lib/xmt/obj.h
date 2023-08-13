#ifndef XMT_OBJ_H
#define XMT_OBJ_H
typedef struct {
    int type;
    void *data;
} xmt_obj;

typedef struct {
    xmt_obj obj;
    int val;
} xmt_obj_int;

typedef struct {
    xmt_obj obj;
    const char *val;
} xmt_obj_str;

typedef struct {
    xmt_obj obj;
    xmt_obj **val;
    int length;
} xmt_obj_array;

typedef struct {
    xmt_obj *key;
    xmt_obj *val;
} xmt_obj_pair;

typedef struct {
    xmt_obj obj;
    xmt_obj_pair *val;
    int length;
} xmt_obj_map;

xmt_obj * xmt_obj_mkint(int val);
xmt_obj * xmt_obj_mkstr(const char *str, size_t sz);
xmt_obj * xmt_obj_mkarray(int length);
xmt_obj ** xmt_obj_array_values(xmt_obj *obj);
xmt_obj * xmt_obj_mkmap(int length);
void xmt_obj_map_insert(xmt_obj *map,
                        const char *key,
                        size_t sz,
                        xmt_obj *val,
                        int pos);
void xmt_obj_map_insert_v2(xmt_obj *map,
                           xmt_obj *key,
                           xmt_obj *val,
                           int pos);
void xmt_print(xmt_obj *obj);
void xmt_obj_del(xmt_obj **obj);
xmt_obj * xmt_obj_map_lookup(xmt_obj *map, const char *key);
#endif
