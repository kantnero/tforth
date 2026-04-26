#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INT_TYPE 1
#define STR_TYPE 2 /* can also work for symbols */ 


/* To store each type obj*/
typedef struct obj {
	int ref;
	int obj_type;
	union {
		int i;
		struct {
			char *s;
			size_t len;
		} str;
	} type;
} obj;

#define MAX 256
typedef struct stack {
	obj *data[MAX];
	int top;
} stack;


/* create an obj for each type */
obj *create_obj(size_t type, char *value) {
	obj *o = malloc(sizeof(obj));
	if (!o) return NULL;

	o->ref = 1;

	if (type == INT_TYPE) {
		int n = atoi(value);
		
		o->obj_type = type;
		o->type.i = n;
		return o;
	}
	else if (type == STR_TYPE) {
		size_t l = strlen(value);
		o->obj_type = type;
		o->type.str.s = malloc(l + 1);
		if (!o->type.str.s) return NULL;
		o->type.str.len = l;
		memcpy(o->type.str.s, value, l);
		o->type.str.s[l] = 0;
		return o;
	}
	else {
		fprintf(stderr, "TYPE NOT SPECIFIED\n");
		free(o);
		return NULL;
	}
}
void push(obj *o, stack *s) {
	if (s->top >= 256 || o == NULL) 
		return;
	s->data[s->top] = o;
	s->top++;
}

obj *pop(stack *s) {
	return s->data[--s->top];
}

void parse(char *p, stack *s) {
	char *token;
	token = strtok(p, " \n\t");
	
	while (token != NULL) {
		
		if (isdigit((unsigned char)token[0])) {
			obj *o = create_obj(INT_TYPE, token);
			push(o, s);
		}
		else {
			if (strcmp(token, "*") == 0) {
				obj *a = pop(s);
				obj *b = pop(s);
			//	printf("%d\t%d\n", a->type.i, b->type.i);
				int r = a->type.i * b->type.i;
			}
		}
		token = strtok(NULL, " \n\t");
	}
}
int main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "Usage: <filename>\n");
		return 1;
	}

	FILE *fp = fopen(argv[1], "r"); //open file read-only mode 
	
	if (fp == NULL) {
		perror("open");
		return 1;
	}
	char buf[MAX];
	stack s;
	s.top = 0;
	while(fgets(buf, sizeof(buf), fp) != NULL) {
		printf("%s", buf);
		parse(buf, &s);
	}
	// loop through and free stack
	return 0;
}
