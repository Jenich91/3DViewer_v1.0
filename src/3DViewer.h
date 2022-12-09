#ifndef SRC_3DVIEWER_H_
#define SRC_3DVIEWER_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1e-6

enum { X, Y, Z, AXIS };

typedef struct matrix_t {
  double **matrix;
  double max;
  size_t rows;
  size_t column;
} matrix_t;

typedef struct polygon_t {
  int *polygon;
  size_t size;
} polygon_t;

typedef struct object_t {
  size_t count_of_vertexes;
  size_t count_of_facets;
  matrix_t matrix3d;
  polygon_t polygon;
} object_t;

typedef void (*rotation_t[AXIS])(matrix_t *, double);

void create_polygon(char *, int **, size_t *);
void create_matrix(char *, double ***, size_t, double *);
void move_matrix(matrix_t *, double, int);
void rotation_matrix(matrix_t *, double, int);
void zoom_matrix(matrix_t *, double);
object_t parsing(char *);
void remove_matrix(matrix_t *);
void remove_polygon(polygon_t *);
void remove_object(object_t *);

#endif  //  SRC_3DVIEWER_H_
