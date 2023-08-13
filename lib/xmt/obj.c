#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "obj.h"

enum {
    TYPE_INT,
    TYPE_STR,
    TYPE_ARRAY,
    TYPE_MAP
};

xmt_obj * xmt_obj_mkint(int val)
{
    xmt_obj_int *obj;
    obj = malloc(sizeof(xmt_obj_int));
    obj->obj.type = TYPE_INT;
    obj->val = val;
    obj->obj.data = obj;
    return &obj->obj;
}

xmt_obj * xmt_obj_mkstr(const char *str, size_t sz)
{
    xmt_obj_str *obj;
    char *outstr;
    obj = malloc(sizeof(xmt_obj_str) + sz + 1);
    obj->obj.type = TYPE_STR;
    /* string bits are allocated at the end of the struct */
    outstr = (char *)&obj[1];
    obj->obj.data = obj;
    memcpy(outstr, str, sz);
    outstr[sz] = '\0';
    obj->val = (const char *)outstr;
    return &obj->obj;
}

xmt_obj * xmt_obj_mkarray(int length)
{
    xmt_obj_array *obj;
    int i;
    obj = malloc(sizeof(xmt_obj_array) + length * sizeof(xmt_obj*));
    obj->obj.type = TYPE_ARRAY;
    obj->obj.data = obj;
    obj->length = length;
    obj->val = (xmt_obj **)&obj[1];
    for (i = 0; i < length; i++) {
        obj->val[i] = NULL;
    }
    return &obj->obj;
}

xmt_obj ** xmt_obj_array_values(xmt_obj *obj)
{
    xmt_obj_array *a;
    if (obj->type != TYPE_ARRAY) return NULL;
    a = obj->data;
    return a->val;
}

xmt_obj * xmt_obj_mkmap(int length)
{
    xmt_obj_map *obj;
    int i;
    obj = malloc(sizeof(xmt_obj_map) + length * sizeof(xmt_obj_pair));
    obj->obj.type = TYPE_MAP;
    obj->obj.data = obj;
    obj->length = length;
    obj->val = (xmt_obj_pair *)&obj[1];
    for (i = 0; i < length; i++) {
        obj->val[i].key = NULL;
        obj->val[i].val = NULL;
    }
    return &obj->obj;
}

void xmt_obj_map_insert(xmt_obj *map,
                        const char *key,
                        size_t sz,
                        xmt_obj *val,
                        int pos)
{
    xmt_obj_map *m;
    if (map->type != TYPE_MAP) return;
    m = map->data;

    if (m->val[pos].key != NULL) return;

    m->val[pos].key = xmt_obj_mkstr(key, sz);
    m->val[pos].val = val;
}

void xmt_obj_map_insert_v2(xmt_obj *map,
                           xmt_obj *key,
                           xmt_obj *val,
                           int pos)
{
    xmt_obj_map *m;
    if (map->type != TYPE_MAP) return;
    m = map->data;

    if (m->val[pos].key != NULL) return;

    m->val[pos].key = key;
    m->val[pos].val = val;
}


void xmt_print(xmt_obj *obj)
{
    switch(obj->type) {
        case TYPE_INT: {
            xmt_obj_int *i;
            i = obj->data;
            printf("%d\n", i->val);
        }
            break;
        case TYPE_STR: {
            xmt_obj_str *s;
            s = obj->data;
            printf("%s\n", s->val);
        }
            break;
        case TYPE_ARRAY: {
            xmt_obj_array *a;
            int i;
            a = obj->data;
            printf("[\n");
            for (i = 0; i < a->length; i++) {
                if (a->val[i] == NULL) {
                    printf("NULL\n");
                } else {
                    xmt_print(a->val[i]);
                }
            }
            printf("]\n");
        }
            break;
        case TYPE_MAP: {
            xmt_obj_map *m;
            int i;
            m = obj->data;
            printf("{\n");
            for (i = 0; i < m->length; i++) {
                if (m->val[i].key != NULL) {
                    xmt_obj_str *key;
                    key = m->val[i].key->data;
                    printf("%s:\n\t", key->val);
                    xmt_print(m->val[i].val);
                }
            }
            printf("}\n");
        }
            break;
        default:
            break;
    }
}

void xmt_obj_del(xmt_obj **obj)
{
    if ((*obj)->type == TYPE_ARRAY) {
        int i;
        xmt_obj_array *a;
        a = (*obj)->data;
        for (i = 0; i < a->length; i++) {
            if (a->val[i] != NULL) {
                xmt_obj_del(&a->val[i]);
            }
        }
    } else if ((*obj)->type == TYPE_MAP) {
        int i;
        xmt_obj_map *m;
        m = (*obj)->data;
        for (i = 0; i < m->length; i++) {
            if (m->val[i].key != NULL) {
                xmt_obj_del(&m->val[i].key);
                xmt_obj_del(&m->val[i].val);
            }
        }
    }
    free(*obj);
}

xmt_obj * xmt_obj_map_lookup(xmt_obj *obj, const char *key)
{
    xmt_obj_map *map;
    int i;

    if (obj->type != TYPE_MAP) return NULL;

    map = obj->data; 

    for (i = 0; i < map->length; i++) {
        xmt_obj *tmp;
        xmt_obj_str *s;

        tmp = map->val[i].key;
        s = tmp->data;

        if (!strcmp(key, s->val)) {
            return map->val[i].val;
        }
    }

    return NULL;
}
