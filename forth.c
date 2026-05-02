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
				int r = a->type.i * b->type.i;
				
				/* create a new obj for r and push */
				push(o, s);
			}
			if (strcmp(token, "/") == 0) {
				obj *a = pop(s);
				obj *b = pop(s);

			}
			if (strcmp(token, "-") == 0) {
				obj *a = pop(s);
				obj *b = pop(s);

			}
			if (strcmp(token, "+") == 0) {
				obj *a = pop(s);
				obj *b = pop(s);
			}
			/* print the value at the top of the stack */
			if (strcmp(token, ".") == 0) {
				obj *o = pop(s);
				if (o->obj_type == INT_TYPE) {
					printf("%d\n", o->type.i);
				}
				else if (o->obj_type == STR_TYPE) {
					printf("%s\n", o->type.str.s);
				}

			}
		}
		token = strtok(NULL, " \n\t");
	}
}

void interactive_mode(char *buf, stack *s){

	/*TODO:
	 * handle CTRL + D it causes infinite loop */
	while(1) {
		printf(">>> ");
		fgets(buf, MAX, stdin);
		parse(buf, s);
		if (strcmp(buf, "exit") == 0)
			exit(1);
	}

}

int main(int argc, char **argv) {

	char buf[MAX];
	stack s;
	s.top = 0;

	if (argc == 1) {
		interactive_mode(buf, &s);
	}
	if (argc != 2) {
		fprintf(stderr, "Usage: <filename>\n");
		return 1;
	}

	FILE *fp = fopen(argv[1], "r"); //open file read-only mode 
	
	if (fp == NULL) {
		perror("open");
		return 1;
	}

	while(fgets(buf, sizeof(buf), fp) != NULL) {
		printf("%s", buf);
		parse(buf, &s);
	}
	// loop through and free stack
	return 0;
}
